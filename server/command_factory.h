#ifndef KVSTORE_FACTORY
#define KVSTORE_FACTORY

#include "command_prase.h"
#include <unordered_map>
#include "singleton.h"
#include "common_def.h"

static std::unordered_map<std::string, shared_ptr<CCmdPraseBase>> gcmd_map =
{
    {COMMAND_SET, make_shared<CSetCmdPrase>()},
    {COMMAND_GET, make_shared<CGetCmdPrase>()},
    {COMMAND_DEL, make_shared<CDelCmdPrase>()},
    {COMMAND_HSET, make_shared<CHSetCmdPrase>()},
    {COMMAND_HMSET, make_shared<CHMSetCmdPrase>()},
    {COMMAND_HGET, make_shared<CHGetCmdPrase>()},
    {COMMAND_HDEL, make_shared<CHDelCmdPrase>()},
    {COMMAND_LSET, make_shared<CLSetCmdPrase>()},
    {COMMAND_LPOP, make_shared<CLPopCmdPrase>()},
    {COMMAND_LGET, make_shared<CLGetCmdPrase>()},
    {COMMAND_LPUSH, make_shared<CLPushCmdPrase>()},
    {COMMAND_RPOP, make_shared<CRPopCmdPrase>()},
    {COMMAND_RGET, make_shared<CRGetCmdPrase>()},
    {COMMAND_RPUSH, make_shared<CRPushCmdPrase>()},
    {COMMAND_SADD, make_shared<CSAddCmdPrase>()},
    {COMMAND_SDEL, make_shared<CSDelCmdPrase>()},
    {COMMAND_SISMEMBER, make_shared<CSIsMemberCmdPrase>()},
    {COMMAND_ZADD, make_shared<CZAddCmdPrase>()},
    {COMMAND_ZDEL, make_shared<CZDelCmdPrase>()},
    {COMMAND_ZGET, make_shared<CZGetCmdPrase>()},
};

class CCmdFactory : public Singleton<CCmdFactory>
{
public:
    static shared_ptr<CCmdPraseBase> GetCmdPrase(std::string cmd) 
    {
        return gcmd_map.find(cmd) == gcmd_map.end() ? NULL : gcmd_map[cmd];
    }
};

#define _theCmdFactory CCmdFactory::getInstance()


#endif