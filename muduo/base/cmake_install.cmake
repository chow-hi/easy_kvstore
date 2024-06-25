# Install script for directory: /code/easy_kvstore/muduo/base

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/code/build/debug-install")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "0")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/code/easy_kvstore/lib/libmuduo_base.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/base" TYPE FILE FILES
    "/code/easy_kvstore/muduo/base/AsyncLogging.h"
    "/code/easy_kvstore/muduo/base/Atomic.h"
    "/code/easy_kvstore/muduo/base/BlockingQueue.h"
    "/code/easy_kvstore/muduo/base/BoundedBlockingQueue.h"
    "/code/easy_kvstore/muduo/base/Condition.h"
    "/code/easy_kvstore/muduo/base/CountDownLatch.h"
    "/code/easy_kvstore/muduo/base/CurrentThread.h"
    "/code/easy_kvstore/muduo/base/Date.h"
    "/code/easy_kvstore/muduo/base/Exception.h"
    "/code/easy_kvstore/muduo/base/FileUtil.h"
    "/code/easy_kvstore/muduo/base/GzipFile.h"
    "/code/easy_kvstore/muduo/base/LogFile.h"
    "/code/easy_kvstore/muduo/base/LogStream.h"
    "/code/easy_kvstore/muduo/base/Logging.h"
    "/code/easy_kvstore/muduo/base/Mutex.h"
    "/code/easy_kvstore/muduo/base/ProcessInfo.h"
    "/code/easy_kvstore/muduo/base/Singleton.h"
    "/code/easy_kvstore/muduo/base/StringPiece.h"
    "/code/easy_kvstore/muduo/base/Thread.h"
    "/code/easy_kvstore/muduo/base/ThreadLocal.h"
    "/code/easy_kvstore/muduo/base/ThreadLocalSingleton.h"
    "/code/easy_kvstore/muduo/base/ThreadPool.h"
    "/code/easy_kvstore/muduo/base/TimeZone.h"
    "/code/easy_kvstore/muduo/base/Timestamp.h"
    "/code/easy_kvstore/muduo/base/Types.h"
    "/code/easy_kvstore/muduo/base/WeakCallback.h"
    "/code/easy_kvstore/muduo/base/copyable.h"
    "/code/easy_kvstore/muduo/base/noncopyable.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

