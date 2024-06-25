#include "command_prase.h"
#include <string>
#include "muduo/base/Logging.h"
#include "common_def.h"

std::string CSetCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->set(tokens[0], tokens[1]) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CGetCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 1)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->get(tokens[0]);
}

std::string CDelCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 1)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->del(tokens[0]) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CHSetCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 3)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->hset(tokens[0], tokens[1], tokens[2]) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CHMSetCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() < 3 && (tokens.size() % 2 != 0))
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    std::vector<std::pair<string, string>> params;
    for (int i = 1; i < tokens.size(); i += 2)
    {
        params.push_back(std::make_pair(tokens[i], tokens[i + 1]));
    }

    return table->hmset(tokens[0], params) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CHGetCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->hget(tokens[0], tokens[1]);
}

std::string CHDelCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->hdel(tokens[0], tokens[1]) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CLSetCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() < 2)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    std::vector<string> vec;
    for (int i = 1; i < tokens.size(); i++)
    {
        vec.push_back(tokens[i]);
    }

    return table->lset(tokens[0], vec) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CLPopCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 1)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->lpop(tokens[0]) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CLPushCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() < 2)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }

    std::vector<string> vec;
    for (int i = 1; i < tokens.size(); i++)
    {
        vec.push_back(tokens[i]);
    }
    return table->lpush(tokens[0], vec) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CLGetCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 1)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->lget(tokens[0]);
}

std::string CRPopCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 1)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->rpop(tokens[0]) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CRPushCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() < 2)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }

    std::vector<string> vec;
    for (int i = 1; i < tokens.size(); i++)
    {
        vec.push_back(tokens[i]);
    }
    return table->rpush(tokens[0], vec) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CRGetCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 1)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->rget(tokens[0]);
}

std::string CSAddCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->sadd(tokens[0], tokens[1]) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CSDelCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->sdel(tokens[0], tokens[1]) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CSIsMemberCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->sismember(tokens[0], tokens[1]) ? COMMAND_TRUE : COMMAND_FALSE;
}

std::string CZAddCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 3)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->zadd(tokens[0], tokens[1], tokens[2]) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CZDelCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->zdel(tokens[0], tokens[1]) ? COMMAND_SUCCESS : COMMAND_FAIL;
}

std::string CZGetCmdPrase::prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens)
{
    if (tokens.size() != 2)
    {
        LOG_DEBUG << "无效的参数列表";
        return COMMAND_FAIL;
    }
    return table->zget(tokens[0], tokens[1]);
}