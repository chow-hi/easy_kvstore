#include "kvstore.h"

#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"

using namespace muduo;
using namespace muduo::net;

std::shared_ptr<CKv_Store> g_kvStore;

void onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    conn->setContext(g_kvStore);
    conn->setMessageCallback(
        std::bind(&CKv_Store::onMessage, g_kvStore, _1, _2, _3));
    conn->setTcpNoDelay(true);
  }
}

void timer_cb()
{
  g_kvStore->dump_file();
}

int main(int argc, char* argv[])
{
  int port = 9999;
  int threads = 0;
  string ip = "192.168.146.8";
  if (argc > 1)
    ip = argv[1];
  if (argc > 2)
    port = atoi(argv[2]);
  if (argc > 3)
    threads = atoi(argv[3]);
  InetAddress addr(ip, static_cast<uint16_t>(port));
  LOG_INFO << "kvstore listens on " << addr.toIpPort()
           << " threads " << threads;

  g_kvStore = make_shared<CKv_Store>();
  muduo::net::EventLoop loop;
  TcpServer server(&loop, addr, "kvstore");
  server.setConnectionCallback(onConnection);
  server.setThreadNum(threads);
  server.start();
  loop.runEvery(10, timer_cb);
  loop.loop();
}
