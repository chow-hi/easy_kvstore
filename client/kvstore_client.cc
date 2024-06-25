#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <queue>
using namespace std;

int cli_sock = 0;
struct sockaddr_in kvserver_addr;
char buf[1024] = {0};
queue<string> multi_que;
bool is_multi = false;

bool test_case(string cmd, string result, bool print = true)
{
    if (send(cli_sock, cmd.c_str(), cmd.size(), 0) < 0)
    {
        std::cout << "Send failed" << std::endl;
        return false;
    }
    // 接收服务器的数据
    int recv_size = 0;
    if ((recv_size = recv(cli_sock, buf, sizeof(buf), 0)) == -1)
    {
        std::cout << "recv failed" << std::endl;
    }

    if (print)
    {
        // 添加结束字符
        buf[recv_size] = '\0';
        if (result.compare(buf) == 0)
        {
            std::cout << ">> " << cmd << " testcase > success \n" << std::endl;
        }
        else{
            std::cout << ">> " << cmd << " testcase > fail  \n" << std::endl;
        }
    }
    return true;
}

bool limit_test()
{
    auto start = std::chrono::high_resolution_clock::now();
    int max_num = 100000;
    string cmd;
    /*for (int i = 0; i < max_num; i++)
    {
        cmd = "HSET KEY ";
        cmd += "FIELD" + std::to_string(i);
        cmd += " VALUE" + std::to_string(i);
        test_case(cmd, "success");
    }*/
    for (int i = 0; i < max_num; i++)
    {
        cmd = "SET ";
        cmd += "KEY" + std::to_string(i);
        cmd += " VALUE" + std::to_string(i);
        test_case(cmd, "success", false);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    printf("code tooks time %d ms\n",(int)(elapsed.count()));
    return true;
}

bool unittest_case()
{
    test_case("SET KEY1 VALUE", "success");
    test_case("GET KEY1", "VALUE");
    test_case("DEL KEY1", "success");

    test_case("HSET KEY2 FIELD1 VALUE2", "success");
    test_case("HGET KEY2 FIELD1", "VALUE2");
    test_case("HDEL KEY2 FIELD1", "success");
    test_case("HMSET KEY3 FIELD3 VALUE3 FIELD4 VALUE4 FIELD5 VALUE5", "success");
    test_case("HGET KEY3 FIELD3", "VALUE3");
    test_case("HGET KEY3 FIELD4", "VALUE4");
    test_case("HGET KEY3 FIELD5", "VALUE5");
    test_case("HDEL KEY3 FIELD3", "success");
    test_case("HDEL KEY3 FIELD4", "success");
    test_case("HDEL KEY3 FIELD5", "success");

    test_case("LSET LIST1 VALUE1 VALUE2 VALUE3", "success");
    test_case("LPUSH LIST1 VALUE0", "success");
    test_case("LPOP LIST1", "success");
    test_case("LGET LIST1", "VALUE1");
    test_case("RPUSH LIST1 VALUE4", "success");
    test_case("RPOP LIST1", "success");
    test_case("RGET LIST1", "VALUE3");

    test_case("SADD SET VALUE1", "success");
    test_case("SADD SET VALUE2", "success");
    test_case("SADD SET VALUE3", "success");
    test_case("SDEL SET VALUE1", "success");
    test_case("SISMEMBER SET VALUE1", "false");
    test_case("SISMEMBER SET VALUE2", "true");

    test_case("MULTI \n"
            "SET MULTIKEY1 VALUE1 \n"
            "LSET MULTILIST1 VALUE2 \n"
            "HSET MULTIHKEY1 FILED3 VALUE3 \n"
            "EXEC", "", false);
    test_case("GET MULTIKEY1", "VALUE1");
    test_case("LGET MULTILIST1", "VALUE2");
    test_case("HGET MULTIHKEY1 FILED3", "VALUE3");
}

bool cmd_line_loop()
{
    // 发送数据
    string msg;
    std::getline(std::cin, msg);
    if (msg.find("MULTI") != msg.npos)
    {
        std::cout << "Success" << std::endl;
        multi_que.push(msg);
        is_multi = true;
        return true;
    }
    else if(msg.find("EXEC") != msg.npos)
    {
        msg.clear();
        while(multi_que.size())
        {
            string cmd = multi_que.front();
            multi_que.pop();
            msg += cmd;
            msg += "\n";
        }
        msg.append("EXEC");
        is_multi = false;
        std::cout << "multi command:" << msg << std::endl;
    }
    else
    {
        if (is_multi)
        {
            multi_que.push(msg);
            std::cout << "Success" << std::endl;
            return true;
        }
    }

    if ((send(cli_sock, msg.c_str(), msg.size(), 0) < 0))
    {
        std::cout << "Send failed" << std::endl;
        return false;
    }
    // 接收服务器的数据
    int recv_size = 0;
    if ((recv_size = recv(cli_sock, buf, sizeof(buf), 0)) == -1)
    {
        std::cout << "recv failed" << std::endl;
    }

    // 添加结束字符
    buf[recv_size] = '\0';
    std::cout << buf << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("wrong params\n");
        return -1;
    }
    cli_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_sock == -1)
    {
        std::cout << "create socket fail" << std::endl;
        return -1;
    }
    string ip = argv[1];
    int port = std::stoi(argv[2]);
    string cmd = "cmdline";
    if (argc > 3)
    {
        cmd = argv[3];
        std::cout << "opr:" << cmd <<std::endl;
    }

    memset(&kvserver_addr, 0, sizeof(struct sockaddr_in));
    kvserver_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    kvserver_addr.sin_family = AF_INET;
    kvserver_addr.sin_port = htons(port);

    if (connect(cli_sock, (struct sockaddr *)&kvserver_addr, sizeof(kvserver_addr)) < 0)
    {
        std::cout << "Connect error." << std::endl;
        return -1;
    }

    string msg;
    
    while (true)
    {
        if (cmd.compare("cmdline") == 0)
        {
            std::cout << ip << ":" << port << "> ";
            cmd_line_loop();
        }
        else if(cmd.compare("unittest") == 0)
        {
            unittest_case();
            break;
        }
        else if(cmd.compare("limittest") == 0)
        {
            limit_test();
            break;
        }
    }

    close(cli_sock);
}