#include <fast/__fast_mutex.hpp>
#include <thread>
namespace xusd{
    namespace fast{

        void staticmutex::lockSlowCase(){
            while (!try_lock())
                std::this_thread::yield();
        }
    }
}
