#include "kvstore.h"
#include "muduo/base/Logging.h"
#include <sstream>
#include <string>
#include "common_def.h"
#include "command_factory.h"
#include <memory>
#include "table.h"
#include <atomic>
#include <chrono>
#include <queue>
#include "spinlock.h"
#include <sys/types.h>
#include <sys/wait.h>

#define TEST_SERVER_OPS 1

std::atomic<unsigned int> g_request_num(0);
auto g_start = std::chrono::high_resolution_clock::now();
using namespace muduo::net;
bool CKv_Store::parseRequest(Buffer* buf, muduo::net::Buffer* out)
{
    //printf("request %s \n ", buf->toStringPiece().data());
    std::istringstream iss(buf->retrieveAllAsString());
    std::string command;
    iss >> command;
    if (command.empty())
    {
        return false;
    }

    /* 此处处理命令行都是访问内存，处理速度都极快，使用自旋锁比互斥锁性能高很多 */
    slock_.lock();
    if (command == COMMAND_MULTI) {
        std::string line;
        queue<string> que;
        while (std::getline(iss, line, '\n')) 
        {
            std::cout << line << std::endl;
            if (!line.compare(COMMAND_EXEC))
            {
                if (que.empty())
                {
                    out->append("fail, empty command.");
                    break;
                }

                while(que.size())
                {
                    string cmd_line = que.front();
                    que.pop();
                    std::istringstream multi_iss(cmd_line);
                    std::string multi_command;
                    multi_iss >> multi_command;
                    std::vector<std::string> tokens;
                    std::string argc;
                    while (multi_iss >> argc)
                    {
                        tokens.push_back(argc);
                    }
                    cmdExec(multi_command, tokens, out);
                    out->append(" \n");
                }
            }
            else
            {
                que.push(line);
            }
        }
    }
    else if(command == COMMAND_SELECT)
    {
        std::string sindex;
        iss >> sindex;
        int index = std::stoi(sindex);
        bool res = select(index);
        if (res)
            out->append(COMMAND_SUCCESS);
        else
        {
            out->append("fail, error index.");
            return false;
        }
    }
    else {
        /*常规命令处理 */
        std::vector<std::string> tokens;
        std::string argc;
        while (iss >> argc) {
            tokens.push_back(argc);
        }
        cmdExec(command, tokens, out);
    }
    slock_.unlock();
    
    if(TEST_SERVER_OPS)
    {
        g_request_num++;
        if (g_request_num % 300000 == 0) 
        {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - g_start;
            printf("kvstore ops %d\n",  300000 / (int)(elapsed.count()) * 1000);
            g_start = end;
        }
    }
    return true;
}


bool CKv_Store::cmdExec(string cmd, const std::vector<string> &tokens, muduo::net::Buffer* out)
{
    shared_ptr<CCmdPraseBase> cmd_prase = _theCmdFactory.GetCmdPrase(cmd);
    if (!cmd_prase)
    {
        out->append("fail, wrong command.");
        return false;
    }
    shared_ptr<CTable> table = getTable();
    if (!table)
    {
        out->append("fail, table no exist.");
        return false;
    }
    try
    {
        string res_str = cmd_prase->prase(table, tokens);
        if (res_str.empty())
            res_str = COMMAND_FAIL;
        out->append(res_str);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        out->append(COMMAND_FAIL);
        return false;
    }
    return true;
}

shared_ptr<CTable> CKv_Store::getTable()
{
    if (cur_index_ >= tables_.size())
    {
        return NULL;
    }
    
    return tables_[cur_index_];
}

void CKv_Store::dump_file()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        for (auto it : tables_)
        {
            if (it != NULL)
            {
                it->dump();
            }
        }
        exit(0);
    }
    else
    {
        sleep(1);
        int status;
        waitpid(pid, &status, 0); // 等待子进程退出
        //printf("子进程已退出\n");
    }
}

bool CKv_Store::select(int index)
{
    if (index >= 0 && index < MAX_TABLE_NUM)
    {
        cur_index_ = index;
        if (tables_[index] == nullptr)
        {
            tables_[cur_index_] = make_shared<CTable>();
            string path = SAVE_PATH_PRE + std::to_string(cur_index_);
            tables_[cur_index_]->set_path(path);
            tables_[cur_index_]->load();
        }
        return true;
    }
    return false;
}