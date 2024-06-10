// Lukas Anthonissen - u21434345
// Daniel van Zyl - u21446459

#include <exception>

#ifndef STRATEGY_EXCEPTION_HPP
#define STRATEGY_EXCEPTION_HPP

template<typename Key>
class StrategyException : public std::exception
{
public:
    StrategyException(){
        message = "No strategy";
    };

    StrategyException(const char *message) : message(message){}

    const char *what() const noexcept override
    {
        return message;
    }

    const Key* getKey(){
        return key;
    }

private:
    const char *message;
    const Key *key;

protected:
    void* OnNoStrategy(const Key& key) {
        this->key = &key;
        throw StrategyException("Strategy not found.");
    }
};

#endif