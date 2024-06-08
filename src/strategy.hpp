#include <iostream>
#include <string>
#include <map>

#include "functor.hpp"
#include "strategy_exception.hpp"

#ifndef STRATEGY_HPP
#define STRATEGY_HPP

template <typename Key, typename ReturnType, typename... Args>
class Strategy
{
private:
    typedef Functor<ReturnType, Args...> FunctorType;

    std::map<Key, FunctorType> strategies;
    Key current_strategy;
    FunctorType default_strategy;

public:

    Strategy(FunctorType default_strategy) : default_strategy(default_strategy) {

    }

    void register_strategy(Key key, FunctorType functor)
    {
        strategies.insert(std::make_pair(key, functor));
    }

    void set_strategy(Key key)
    {
        current_strategy = key;
    }

    ReturnType operator()(Args... args)
    {
        return execute(args...);
    }

    ReturnType execute(Args... args)
    {
        if (strategies.find(current_strategy) == strategies.end())
        {
            throw StrategyException("Strategy not found");
        }

        return strategies.at(current_strategy)(args...);
    }

    ReturnType execute_by_key(Key key, Args... args)
    {
        return strategies[key](args...);
    }
};

#endif