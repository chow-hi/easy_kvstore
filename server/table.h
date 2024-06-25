#ifndef KVSTORE_TABLE
#define KVSTORE_TABLE
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <list>
#include <set>
#include <memory>
#include "skiplist.h"
#include "spinlock.h"

using namespace std;
namespace table {

enum type{
    tstr,
    tvec,
    tlist,
    tmap,
    tset,
    tumap,
    trumap,
    tskiplist,
};

class CTableItemBase {
public:
    CTableItemBase(type t) : type_(t) {}
    bool is_string() {return type_ == tstr;}
    bool is_list() {return type_ == tlist;}
    bool is_map() {return type_ == tmap;}
    bool is_umap() {return type_ == tumap;}
    bool is_set() {return type_ == tset;}
    bool is_root_umap(){return type_ == trumap;}
    bool is_skiplist() {return type_ == tskiplist;}
    string type_to_str(){return to_string(type_);}
    virtual string dump() = 0;
    virtual void load(string buffer) = 0;
protected:
    char type_;
};

class CTableRootItemUMap {
public:
    CTableRootItemUMap() = default;
    void dump();
    void load();
    void load_item(int type, string key, string buffer);
public:
    string path_;
    unordered_map<string, shared_ptr<CTableItemBase>> value_;
};

class CTableItemString : public CTableItemBase {
public:
    CTableItemString() : CTableItemBase(type::tstr) {}
    virtual string dump() {return value_;};
    virtual void load(string buffer) {value_ = buffer;};
public:
    string value_;
};

class CTableItemUMap : public CTableItemBase {
public:
    CTableItemUMap() : CTableItemBase(type::tumap) {}
    virtual string dump();
    virtual void load(string buffer);
public:
    unordered_map<string, string> value_;
};

class CTableItemMap : public CTableItemBase {
public:
    CTableItemMap() : CTableItemBase(type::tmap) {}
    virtual string dump();
    virtual void load(string buffer);
public:
    map<string, string> value_;
};

class CTableItemSet : public CTableItemBase {
public:
    CTableItemSet() : CTableItemBase(type::tset) {}
    virtual string dump();
    virtual void load(string buffer);
public:
    set<string> value_;
};

class CTableItemList : public CTableItemBase {
public:
    CTableItemList() : CTableItemBase(type::tlist) {}
    virtual string dump();
    virtual void load(string buffer);
public:
    list<string> value_;
};

class CTableItemSkipList : public CTableItemBase {
public:
    CTableItemSkipList() : CTableItemBase(type::tskiplist) {}
    virtual string dump();
    virtual void load(string buffer);
public:
    SkipList<string, string> value_;
};

class CTable {
public:
    CTable();
    ~CTable();
    void set_path(string path);
    void load();
    void dump();
    /* 插入节点 */
    /* 字符串 */
    bool set(string key, string value);
    string get(string key);
    bool del(string key);

    /* list */
    bool lset(string key, const std::vector<string> &vec);
    bool lpush(string key, const std::vector<string> &vec);
    bool lpop(string key);
    string lget(string key);
    bool rpush(string key, const std::vector<string> &vec);
    bool rpop(string key);
    string rget(string key);

    /* hash */
    bool hset(string key, string field, string value);
    string hget(string key, string field);
    bool hdel(string key, string field);
    bool hmset(string key, const vector<std::pair<string, string>> &vec);

    /* set */
    bool sadd(string key, string field);
    bool sismember(string key, string field);
    bool sdel(string key, string field);

    /* skiplist */
    bool zadd(string key, string field, string value);
    string zget(string key, string field);
    bool zdel(string key, string field);

private:
    shared_ptr<CTableRootItemUMap> dbase_;
    SpinLock slock_;
};


};

#endif