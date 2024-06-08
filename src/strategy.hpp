#include <iostream>
#include <string>
#include <map>

template <typename key, typename ReturnType, typename... Args>
class Strategy
{
private:
    std::map<key, std::function<ReturnType(Args...)>> strategies;
    key current_strategy;

public:
    void register_strategy(key name, std::function<ReturnType(Args...)> functor)
    {
        strategies[name] = functor;
    }

    void set_strategy(key name)
    {
        current_strategy = name;
    }

    ReturnType execute(Args... args)
    {
        if (strategies.find(current_strategy) == strategies.end())
        {
            return strategies.begin()->second(args...);
        }

        return strategies[current_strategy](args...);
    }

    ReturnType execute_by_name(key strategy_name, Args... args)
    {
        return strategies[strategy_name](args...);
    }
};