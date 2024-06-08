#include <iostream>
#include <functional>

#ifndef FUNCTOR_HPP
#define FUNCTOR_HPP

template <typename ReturnType, typename... Args>
class Functor
{
public:
    Functor(std::function<ReturnType(Args...)> func) : func_(func) {}

    ReturnType operator()(Args... args)
    {
        return func_(args...);
    }

private:
    std::function<ReturnType(Args...)> func_;
};

#endif