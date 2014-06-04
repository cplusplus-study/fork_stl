#ifndef __FORK_STL__FAST_MUTEX__
#define __FORK_STL__FAST_MUTEX__
#include <thread>
#include <atomic>

namespace xusd{
    namespace fast{
        class staticmutex {
        public:
            void lock();
            bool try_lock();
            void unlock();

        private:
            friend class mutex;

            // Static storage will zero-initialize us automatically, but mutex needs an
            // API for explicit initialization.
            void init();

            void lockSlowCase();

            std::atomic_flag m_flag;
        };

        inline void staticmutex::init() {
            m_flag.clear();
        }

        inline bool staticmutex::try_lock() {
            return !m_flag.test_and_set(std::memory_order_acquire);
        }

        inline void staticmutex::lock() {
            if (!try_lock())
                lockSlowCase();
        }

        inline void staticmutex::unlock() {
            m_flag.clear(std::memory_order_release);
        }


        class mutex : public staticmutex {
            public:
                mutex();
        };

        inline mutex::mutex(){
            init();
        }
    }
}
#endif
