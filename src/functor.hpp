#include <iostream>
#include <functional>

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
