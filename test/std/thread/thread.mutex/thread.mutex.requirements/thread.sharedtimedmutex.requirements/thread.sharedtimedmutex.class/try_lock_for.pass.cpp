//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: libcpp-has-no-threads
// UNSUPPORTED: c++03, c++98, c++11

// <shared_mutex>

// class shared_timed_mutex;

// template <class Rep, class Period>
//     bool try_lock_for(const chrono::duration<Rep, Period>& rel_time);

#include <shared_mutex>
#include <thread>
#include <cstdlib>
#include <cassert>

std::shared_timed_mutex m;

typedef std::chrono::steady_clock Clock;
typedef Clock::time_point time_point;
typedef Clock::duration duration;
typedef std::chrono::milliseconds ms;
typedef std::chrono::nanoseconds ns;

void f1()
{
    time_point t0 = Clock::now();
    assert(m.try_lock_for(ms(300)) == true);
    time_point t1 = Clock::now();
    m.unlock();
    ns d = t1 - t0 - ms(250);
    assert(d < ms(50));  // within 50ms
}

void f2()
{
    time_point t0 = Clock::now();
    assert(m.try_lock_for(ms(250)) == false);
    time_point t1 = Clock::now();
    ns d = t1 - t0 - ms(250);
    assert(d < ms(50));  // within 50ms
}

int main()
{
    {
        m.lock();
        std::thread t(f1);
        std::this_thread::sleep_for(ms(250));
        m.unlock();
        t.join();
    }
    {
        m.lock();
        std::thread t(f2);
        std::this_thread::sleep_for(ms(300));
        m.unlock();
        t.join();
    }
}
