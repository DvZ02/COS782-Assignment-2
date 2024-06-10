#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "functor.hpp"
#include "strategy_exception.hpp"

#ifndef STRATEGY_HPP
#define STRATEGY_HPP

/*
    The strategy must always have at least one strategy registered.

    No strategy may be set if it is not registered.
*/

template <typename Key, template<typename> class StrategyErrorPolicy = StrategyException, typename ReturnType, typename... Args>
class Strategy: StrategyException<Key>
{
private:
    typedef Functor<ReturnType, Args...> FunctorType;

    std::map<Key, FunctorType> strategies;
    Key current_strategy;

public:

    Strategy(Key initial_key, FunctorType initial_strategy){
        current_strategy = initial_key;
        strategies.insert(std::make_pair(initial_key, initial_strategy));
    }

    void register_strategy(Key key, FunctorType functor)
    {
        strategies.insert(std::make_pair(key, functor));
    }

    /*
        Returns false if the strategy is the only one left.
    */
    bool remove_strategy(Key key)
    {
        if (strategies.size() == 1)
        {
            return false;
        }

        strategies.erase(key);

        if (current_strategy == key)
        {
            current_strategy = strategies.begin()->first;
        }

        return true;
    }

    void set_strategy(Key key)
    {
        if (strategies.find(key) == strategies.end())
        {
            StrategyException<Key>::OnNoStrategy(key);
        }

        current_strategy = key;
    }

    ReturnType operator()(Args... args)
    {
        return execute(args...);
    }

    ReturnType execute(Args... args)
    {
        return this->execute_by_key(current_strategy, args...);
    }

    ReturnType execute_by_key(Key key, Args... args)
    {
        if (strategies.find(key) == strategies.end())
        {
            return strategies.begin()->second(args...);
        }

        return strategies.at(key)(args...);
    }

    std::vector<Key> get_keys()
    {
        std::vector<Key> keys;

        for (auto const &pair : strategies)
        {
            keys.push_back(pair.first);
        }

        return keys;
    }
};

#endif