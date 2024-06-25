add_executable(kvstore_client
                kvstore_client.cc
                )
target_link_libraries(kvstore_client muduo_net)