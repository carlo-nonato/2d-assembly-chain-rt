#include <mutex>
#include <condition_variable>

class Semaphore
{
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_ = 0; // Initialized as locked.

public:    
    Semaphore(unsigned long count = 0);
    void post();
    void wait();
    bool try_wait();
};