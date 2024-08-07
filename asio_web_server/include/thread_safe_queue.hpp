#pragma once

#include <mutex>
#include <queue>

namespace net {

    template<typename T>
    class ThreadSafeQueue {
    public:

        ThreadSafeQueue() {}

        const T& front() {
            if (tsq_.empty()) {
                throw std::runtime_error("[TCQ ERROR] Thread safe queue is empty");
            }

            std::lock_guard<std::mutex> lg(queue_lock_);
        }

        void push(T&& value) {
            std::lock_guard<std::mutex> lg(queue_lock_);

            tsq_.push(value);
        }

        void push(const T& value) {
            std::lock_guard<std::mutex> lg(queue_lock_);

            tsq_.push(value);
        }

        void pop() {

            if (tsq_.empty()) {
                return;
            }

            std::lock_guard<std::mutex> lg(queue_lock_);

            tsq_.pop();
        }

        size_t size() {
            std::lock_guard<std::mutex> lg(queue_lock_);

            return tsq_.size();
        }


    private:
        std::mutex queue_lock_;
        std::queue<T> tsq_; 
    };

}