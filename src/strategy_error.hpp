#include "strategy_exception.hpp"

#ifndef STRATEGY_ERROR_HPP
#define STRATEGY_ERROR_HPP

template <typename Key, class Functor>
class StrategyError
{
public:
    OnNoStrategy(Key key){
        throw StrategyException("Strategy not found.");
    }
};


#endif