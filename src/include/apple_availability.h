//===------------------------ apple_availability.h ------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef _LIBCPP_SRC_INCLUDE_APPLE_AVAILABILITY_H
#define _LIBCPP_SRC_INCLUDE_APPLE_AVAILABILITY_H

#if defined(__APPLE__)

#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 101300
#define _LIBCPP_USE_UTIMENSAT
#endif
#elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 110000
#define _LIBCPP_USE_UTIMENSAT
#endif
#elif defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#if __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ >= 110000
#define _LIBCPP_USE_UTIMENSAT
#endif
#elif defined(__ENVIRONMENT_WATCH_OS_VERSION_MIN_REQUIRED__)
#if __ENVIRONMENT_WATCH_OS_VERSION_MIN_REQUIRED__ >= 40000
#define _LIBCPP_USE_UTIMENSAT
#endif
#endif // __ENVIRONMENT_.*_VERSION_MIN_REQUIRED__

#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 101200
#define _LIBCPP_USE_CLOCK_GETTIME
#endif
#elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 100000
#define _LIBCPP_USE_CLOCK_GETTIME
#endif
#elif defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#if __ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__ >= 100000
#define _LIBCPP_USE_CLOCK_GETTIME
#endif
#elif defined(__ENVIRONMENT_WATCH_OS_VERSION_MIN_REQUIRED__)
#if __ENVIRONMENT_WATCH_OS_VERSION_MIN_REQUIRED__ >= 30000
#define _LIBCPP_USE_CLOCK_GETTIME
#endif
#endif // __ENVIRONMENT_.*_VERSION_MIN_REQUIRED__

#endif // __APPLE__

#endif // _LIBCPP_SRC_INCLUDE_APPLE_AVAILABILITY_H
