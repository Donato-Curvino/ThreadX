//
// Created by donato on 4/23/26.
//

#pragma once

#include <mutex>

template <typename T, class Mtx = std::mutex>
    requires requires { typename std::unique_lock<Mtx>; }
class Locked
{
public:
    explicit Locked(T& ref, Mtx mtx) : value_ref(ref), lock(mtx) { }

    // This class may be moved but not copied because it holds a unique lock
    Locked(const Locked&)            = delete;
    Locked& operator=(const Locked&) = delete;
    Locked(Locked&&)                 = default;
    Locked& operator=(Locked&&)      = default;

    ~Locked() = default;

    template <typename L>
        requires requires(L l, Mtx mtx) { std::unique_lock(mtx, l); }
    explicit Locked(T& ref, Mtx mtx, L lock_strategy) : value_ref(ref), lock(mtx, lock_strategy) { }

    T& operator*()        { return value_ref; }
    T& operator*()  const { return value_ref; }
    T& operator->()       { return value_ref; }
    T& operator->() const { return value_ref; }

private:
    T& value_ref;
    std::unique_lock<Mtx> lock;
};


template <typename T, class Mtx = std::mutex>
    requires requires { typename std::unique_lock<Mtx>; }
class Lockable
{
public:
    Lockable() = default;

    template <typename... Args>
    explicit Lockable(Args&&... args) : value(std::forward<Args>(args)...) { }

    // Locks while copying and moving
    Lockable(const Lockable& other) { std::lock_guard(other.mtx), value = other.value; }
    Lockable& operator=(const Lockable& other)
    {
        if (this != &other) {
            std::scoped_lock(mtx, other.mtx), value = other.value;
        }
        return *this;
    }

    Lockable(Lockable&& other) { std::lock_guard(other.mtx), value = std::move(other.value); }
    Lockable& operator=(Lockable&& other)
    {
        if (this != &other) {
            std::scoped_lock(mtx, other.mtx), value = std::move(other.value);
        }
        return *this;
    }


    Locked<T, Mtx> lock() { return Locked(value, mtx); }

private:
    T value;
    Mtx mtx;
};


