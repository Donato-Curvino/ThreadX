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

    using lockable_type::Lockable;

    Waitable(const Waitable& other) : lockable_type{other} {}
    Waitable& operator=(const Waitable& other) { lockable_type::operator=(other); return *this; }
    Waitable(Waitable&& other) : lockable_type{std::move(other)} {}
    // using lockable_type::operator=(const lockable_type&);
    Waitable& operator=(Waitable&& other) { lockable_type::operator=(std::move(other)); return *this; }

    Locked<T, Mtx> wait()
    {
        std::unique_lock lock(lockable_type::mtx);
        cv.wait(lock);
        lock.release();
        return lock(std::adopt_lock);
    }

private:
    condition_type cv{};
};

} // Close namespace
