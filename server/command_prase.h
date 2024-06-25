#ifndef KVSTORE_COMMAND
#define KVSTORE_COMMAND

#include <string>
#include <vector>
#include "table.h"
#include <memory>

using namespace table;
using namespace std;

#define GEN_CMD_PRASE(class_name) \
    class C##class_name : public CCmdPraseBase { \
        virtual std::string prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens); \
    };   \

class CCmdPraseBase {
public:
    virtual std::string prase(shared_ptr<CTable> table, const std::vector<std::string> &tokens) = 0;
};

/* string */
GEN_CMD_PRASE(SetCmdPrase)
GEN_CMD_PRASE(GetCmdPrase)
GEN_CMD_PRASE(DelCmdPrase)

/* hash */
GEN_CMD_PRASE(HSetCmdPrase)
GEN_CMD_PRASE(HMSetCmdPrase)
GEN_CMD_PRASE(HGetCmdPrase)
GEN_CMD_PRASE(HDelCmdPrase)

/* list */
GEN_CMD_PRASE(LSetCmdPrase)
GEN_CMD_PRASE(LPopCmdPrase)
GEN_CMD_PRASE(LPushCmdPrase)
GEN_CMD_PRASE(LGetCmdPrase)
GEN_CMD_PRASE(RPopCmdPrase)
GEN_CMD_PRASE(RPushCmdPrase)
GEN_CMD_PRASE(RGetCmdPrase)

/* set */
GEN_CMD_PRASE(SAddCmdPrase)
GEN_CMD_PRASE(SDelCmdPrase)
GEN_CMD_PRASE(SIsMemberCmdPrase)

/* skiplist */
GEN_CMD_PRASE(ZAddCmdPrase)
GEN_CMD_PRASE(ZDelCmdPrase)
GEN_CMD_PRASE(ZGetCmdPrase)

#endif