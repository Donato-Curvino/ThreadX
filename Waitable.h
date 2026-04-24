//
// Created by donato on 4/23/26.
//

#pragma once
#include <condition_variable>

#include "Lockable.h"

namespace threadx {

template <typename T, class Mtx = std::mutex>
class Waitable : public Lockable<T, Mtx>
{
public:
    using condition_type = std::conditional_t< std::same_as<Mtx, std::mutex>,
        std::condition_variable,
        std::condition_variable_any
    >;
    using lockable_type = Lockable<T, Mtx>;
    using locked_type   = Locked  <T, Mtx>;

    using lockable_type::Lockable;

    Waitable(const Waitable& other) : lockable_type{other} {}
    Waitable& operator=(const Waitable& other) { lockable_type::operator=(other); return *this; }
    Waitable(Waitable&& other) : lockable_type{std::move(other)} {}
    Waitable& operator=(Waitable&& other) { lockable_type::operator=(std::move(other)); return *this; }

    locked_type wait()
    {
        std::unique_lock lock(lockable_type::mtx);
        cv.wait(lock);
        lock.release();
        return lock(std::adopt_lock);
    }

    template <typename Pred>
        requires std::same_as<bool, std::invoke_result_t<Pred>>
    locked_type wait(Pred&& pred)
    {
        std::unique_lock lock(lockable_type::mtx);
        cv.wait(lock, std::forward<Pred>(pred));  // TODO: what if pred needs accessed to lockable_type::value?
        lock.release();
        return lock(std::adopt_lock);
    }

    void notify_one() { cv.notify_one(); }
    void notify_all() { cv.notify_all(); }

private:
    condition_type cv{};
};

} // Close namespace
