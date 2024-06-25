#ifndef KVSTORE_SINGLETON
#define KVSTORE_SINGLETON

#include <iostream>

template <typename T>
class Singleton {
public:
    static T& getInstance() {
        static T instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton() {}
};

#endif