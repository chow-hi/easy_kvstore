#ifndef MUDUO_KVSTORE_H
#define MUDUO_KVSTORE_H
#include "muduo/net/TcpConnection.h"
#include <map>
#include "table.h"
#include <memory>
#include "spinlock.h"

#define MAX_TABLE_NUM 5
#define SAVE_PATH_PRE "/code/easy_kvstore/bin/database"
using namespace table;
class CKv_Store : muduo::noncopyable{
public:

  explicit CKv_Store()
    : keepConn_(true)
  {
    tables_.reserve(MAX_TABLE_NUM);
    tables_.resize(MAX_TABLE_NUM);
    std::fill(tables_.begin(), tables_.end(), nullptr);
    tables_[0] = make_shared<CTable>();
    cur_index_ = 0;

    string path = SAVE_PATH_PRE + std::to_string(0);
    tables_[cur_index_]->set_path(path);
    tables_[cur_index_]->load();
  }

  void onMessage(const muduo::net::TcpConnectionPtr& conn,
                 muduo::net::Buffer* buf,
                 muduo::Timestamp receiveTime)
  {
    muduo::net::Buffer out;
    bool res = parseRequest(buf, &out);
    conn->send(&out);

    if (!keepConn_)
    {
        conn->shutdown();
    }
  }

  void dump_file();

private:
  bool parseRequest(muduo::net::Buffer* buf, muduo::net::Buffer* out);
  bool cmdExec(string cmd, const std::vector<string> &tokens, muduo::net::Buffer* out);
  shared_ptr<CTable> getTable();
  bool select(int index);
  //uint32_t readLen();
  bool keepConn_;
  int cur_index_;
  vector<shared_ptr<CTable>> tables_;
  SpinLock slock_;
};

#endif