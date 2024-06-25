#include "table.h"
#include <iostream>
#include "stlpacket.h"

using namespace table;

CTable::CTable()
{
    dbase_ = make_shared<CTableRootItemUMap>();
}

CTable::~CTable()
{
}

void CTable::set_path(string path)
{
    dbase_->path_ = path;
}

void CTable::load()
{
    dbase_->load();
}

void CTable::dump()
{
    dbase_->dump();
}

bool CTable::set(string key, string value)
{
    if (key.empty() || value.empty())
        return false;
    shared_ptr<CTableItemString> ptr = make_shared<CTableItemString>();
    ptr->value_ = value;
    dbase_->value_[key] = ptr;
    return true;
}

string CTable::get(string key)
{
    if (key.empty())
        return "";
    
    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemBase> item = it->second;
        if (item->is_string())
        {
            shared_ptr<CTableItemString> ptr = static_pointer_cast<CTableItemString>(item);
            return ptr->value_;
        }
        else
        {
            return "";
        }
    }
    return "";
}

bool CTable::del(string key)
{
    if (key.empty())
        return false;
    
    dbase_->value_.erase(key);
    return true;
}

bool CTable::lset(string key, const std::vector<string> &vec)
{
    if (key.empty() || vec.empty())
        return false;

    shared_ptr<CTableItemList> ptr = make_shared<CTableItemList>();
    ptr->value_.clear();
    for (auto it : vec)
    {
        ptr->value_.push_back(it);
    }
    dbase_->value_[key] = ptr;
    return true;
}

bool CTable::lpush(string key, const std::vector<string> &vec)
{
    if (key.empty() || vec.empty())
        return false;

    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemList> list = static_pointer_cast<CTableItemList>(it->second);
        for (auto it : vec)
        {
            list->value_.push_front(it);
        }
    }
    return true;
}

bool CTable::lpop(string key)
{
    if (key.empty())
        return false;

    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemList> list = static_pointer_cast<CTableItemList>(it->second);
        list->value_.pop_front();
    }
    return true;
}

string CTable::lget(string key)
{
    if (key.empty())
        return "";

    string res;
    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemList> list = static_pointer_cast<CTableItemList>(it->second);
        res = list->value_.front();
    }
    return res;
}

bool CTable::rpush(string key, const std::vector<string> &vec)
{
    if (key.empty() || vec.empty())
        return false;

    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemList> list = static_pointer_cast<CTableItemList>(it->second);
        for (auto it : vec)
        {
            list->value_.push_back(it);
        }
    }
    return true;
}

bool CTable::rpop(string key)
{
    if (key.empty())
        return false;

    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemList> list = static_pointer_cast<CTableItemList>(it->second);
        list->value_.pop_back();
    }
    return true;
}

string CTable::rget(string key)
{
    if (key.empty())
        return "";

    string res;
    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemList> list = static_pointer_cast<CTableItemList>(it->second);
        res = list->value_.back();
    }
    return res;
}

bool CTable::hset(string key, string field, string value)
{
    if (key.empty() || field.empty() || value.empty())
        return false;

    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemUMap> umap = static_pointer_cast<CTableItemUMap>(it->second);
        umap->value_[field] = value;
    }
    else
    {
        shared_ptr<CTableItemUMap> umap = make_shared<CTableItemUMap>();
        dbase_->value_[key] = umap;
        umap->value_[field] = value;
    }
    return true;
}

string CTable::hget(string key, string field)
{
    if (key.empty() || field.empty())
        return "";

    string res;
    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemUMap> umap = static_pointer_cast<CTableItemUMap>(it->second);
        res = umap->value_[field];
    }
    return res;
}

bool CTable::hdel(string key, string field)
{
    if (key.empty() || field.empty())
        return false;

    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemUMap> umap = static_pointer_cast<CTableItemUMap>(it->second);
        umap->value_.erase(field);
    }
    return true;
}

bool CTable::hmset(string key, const vector<std::pair<string, string>> &vec)
{
    if (key.empty() || vec.empty())
        return false;

    shared_ptr<CTableItemUMap> umap;
    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        umap = static_pointer_cast<CTableItemUMap>(it->second);
    }
    else
    {
        umap = make_shared<CTableItemUMap>();
        dbase_->value_[key] = umap;
    }
    for (auto it : vec)
    {
        umap->value_[it.first] = it.second;
    }
    return true;
}

bool CTable::sadd(string key, string field)
{
    if (key.empty() || field.empty())
        return false;

    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemSet> set = static_pointer_cast<CTableItemSet>(it->second);
        set->value_.insert(field);
    }
    else
    {
        shared_ptr<CTableItemSet> set = make_shared<CTableItemSet>();
        set->value_.insert(field);
        dbase_->value_[key] = set;
    }
    return true;
}

bool CTable::sismember(string key, string field)
{
    if (key.empty() || field.empty())
        return false;

    bool res = false;
    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemSet> set = static_pointer_cast<CTableItemSet>(it->second);
        res = set->value_.find(field) != set->value_.end() ? true : false;
    }
    return res;
}

bool CTable::sdel(string key, string field)
{
    if (key.empty() || field.empty())
        return false;

    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemSet> set = static_pointer_cast<CTableItemSet>(it->second);
        set->value_.erase(field);
    }
    return true;
}

bool CTable::zadd(string key, string field, string value)
{
    if (key.empty() || field.empty() || value.empty())
        return false;

    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemSkipList> skiplist = static_pointer_cast<CTableItemSkipList>(it->second);
        auto slist_node = skiplist->value_.searchItem(field);
        if (slist_node == NULL)
        {
            skiplist->value_.addItem(field, value);
        }
        else
        {
            slist_node->value = value;
        }
    }
    else
    {
        shared_ptr<CTableItemSkipList> skiplist = make_shared<CTableItemSkipList>();
        skiplist->value_.addItem(field, value);
        dbase_->value_[key] = skiplist;
    }
    return true;
}

string CTable::zget(string key, string field)
{
    string res = "";
    if (key.empty() || field.empty())
        return res;

    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemSkipList> skiplist = static_pointer_cast<CTableItemSkipList>(it->second);
        auto slist_node = skiplist->value_.searchItem(field);
        if (slist_node != NULL)
        {
            res = slist_node->value;
        }
    }
    return res;
}

bool CTable::zdel(string key, string field)
{
    if (key.empty() || field.empty())
        return false;

    auto it = dbase_->value_.find(key);
    if (it != dbase_->value_.end())
    {
        shared_ptr<CTableItemSkipList> skiplist = static_pointer_cast<CTableItemSkipList>(it->second);
        return skiplist->value_.deleteItem(field);
    }
    return false;
}

string CTableItemUMap::dump()
{
    StlPack p;
    p << value_;
    return p.buffer();
}

void CTableItemUMap::load(string buffer)
{
    StlUnpack up(buffer);
    up >> value_;
}

string CTableItemMap::dump()
{
    StlPack p;
    p << value_;
    return p.buffer();
}

void CTableItemMap::load(string buffer)
{
    StlUnpack up(buffer);
    up >> value_;
}

string CTableItemSet::dump()
{
    StlPack p;
    p << value_;
    return p.buffer();
}

void CTableItemSet::load(string buffer)
{
    StlUnpack up(buffer);
    up >> value_;
}

string CTableItemList::dump()
{
    StlPack p;
    p << value_;
    return p.buffer();
}

void CTableItemList::load(string buffer)
{
    StlUnpack up(buffer);
    up >> value_;
}

string CTableItemSkipList::dump()
{
    return value_.dumpString();
}

void CTableItemSkipList::load(string buffer)
{
    value_.loadString(buffer);
}

void CTableRootItemUMap::dump()
{
    std::ofstream writeFile;
    writeFile.open(path_);
    for (auto it : value_)
    {
        writeFile << it.second->type_to_str() << "<";
        writeFile << it.first << ":" << it.second->dump()<<"\n";
    }
    writeFile.flush();
    writeFile.close();
}

void CTableRootItemUMap::load()
{
    std::ifstream readFile;
    readFile.open(path_);
    if(!readFile.is_open()){
        return;
    }

    std::string line;
    std::string type;
    std::string key;
    std::string value;
    while(std::getline(readFile, line)){
        int index = line.find("<");
        type = line.substr(0, index);
        line = line.substr(index + 1);
        index = line.find(":");
        key = line.substr(0, index);
        value = line.substr(index + 1);
        load_item(atoi(type.c_str()), key, value);
    }
    readFile.close();
}

void CTableRootItemUMap::load_item(int type, string key, string buffer)
{
    switch (type)
    {
    case tstr:
    {
        shared_ptr<CTableItemString> ptr = make_shared<CTableItemString>();
        ptr->value_ = buffer;
        value_[key] = ptr;
    }
    break;
    case tlist:
    {
        shared_ptr<CTableItemList> ptr = make_shared<CTableItemList>();
        StlUnpack up(buffer);
        up >> ptr->value_;
        value_[key] = ptr;
    }
    break;
    case tset:
    {
        shared_ptr<CTableItemSet> ptr = make_shared<CTableItemSet>();
        StlUnpack up(buffer);
        up >> ptr->value_;
        value_[key] = ptr;
    }
    break;
    case tumap:
    {
        shared_ptr<CTableItemUMap> ptr = make_shared<CTableItemUMap>();
        StlUnpack up(buffer);
        up >> ptr->value_;
        value_[key] = ptr;
    }
    break;
    case tskiplist:
    {
        shared_ptr<CTableItemSkipList> ptr = make_shared<CTableItemSkipList>();
        ptr->value_.loadString(buffer);
        value_[key] = ptr;
    }
    break;
    default:
        break;
    }
}