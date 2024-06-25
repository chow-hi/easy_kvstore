add_executable(easy_kvstore 
                easy_kvstore.cc 
                table.h
                table.cc
                kvstore.h 
                kvstore.cc 
                common_def.h
                command_prase.h
                command_prase.cc
                command_factory.h
                singleton.h
                skiplist.h
                inttype.h
                stlpacket.h
                spinlock.h
                )
target_link_libraries(easy_kvstore muduo_net)