//
// Created by donato on 4/23/26.
//

#pragma once
#include <condition_variable>
#include <functional>

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
        locked_type lockedValue(lockable_type::mtx);
        std::unique_lock lock(lockable_type::mtx, std::adopt_lock);
        cv.wait(lock);
        lock.release();
        return lockedValue;
    }

    template <typename Pred>
        requires std::same_as<bool, std::invoke_result_t<Pred>>
    locked_type wait(Pred&& pred)
    {
        locked_type lockedValue(lockable_type::mtx);
        std::unique_lock lock(lockable_type::mtx, std::adopt_lock);
        cv.wait(lock, std::forward<Pred>(pred));
        lock.release();
        return lockedValue;
    }

    template <typename Pred>
        requires std::same_as<bool, std::invoke_result_t<Pred, T>>
    locked_type wait(Pred&& pred)
    {
        locked_type lockedValue = lockable_type::lock();
        std::unique_lock lock(lockable_type::mtx, std::adopt_lock);

        while (!std::invoke(pred, lockedValue)) { cv.wait(lock); }

        lock.release();
        return lockedValue;
    }

    void notify_one() { cv.notify_one(); }
    void notify_all() { cv.notify_all(); }

private:
    condition_type cv{};
};

} // Close namespace
