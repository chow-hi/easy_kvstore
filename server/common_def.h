
/* 特殊命令行 */
#define COMMAND_MULTI "MULTI"
#define COMMAND_EXEC "EXEC"
#define COMMAND_SELECT "SELECT"

/* 常规命令行 */
/* string */
#define COMMAND_SET "SET"
#define COMMAND_GET "GET"
#define COMMAND_DEL "DEL"

/* hash */
#define COMMAND_HSET "HSET"
#define COMMAND_HMSET "HMSET"
#define COMMAND_HGET "HGET"
#define COMMAND_HDEL "HDEL"

/* list */
#define COMMAND_LSET "LSET"
#define COMMAND_LPOP "LPOP"
#define COMMAND_LGET "LGET"
#define COMMAND_LPUSH "LPUSH"
#define COMMAND_RPOP "RPOP"
#define COMMAND_RGET "RGET"
#define COMMAND_RPUSH "RPUSH"

/* set */
#define COMMAND_SADD "SADD"
#define COMMAND_SDEL "SDEL"
#define COMMAND_SISMEMBER "SISMEMBER"

/* skiplist */
#define COMMAND_ZADD "ZADD"
#define COMMAND_ZDEL "ZDEL"
#define COMMAND_ZGET "ZGET"


/* result */
#define COMMAND_SUCCESS "success"
#define COMMAND_FAIL "fail"
#define COMMAND_TRUE "true"
#define COMMAND_FALSE "false"