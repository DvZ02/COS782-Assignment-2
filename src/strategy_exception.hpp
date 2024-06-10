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
    // StrategyException<Key>(const Key *key) : key(key){}

    const char *what() const noexcept override
    {
        // return message;
        // cout << "Strategy: " << key << " not found." << endl;
        return message;
    }

    const Key getKey(){
        return key;
    }

private:
    const char *message;
    const Key *key;

protected:
    void* OnNoStrategy(const Key& key) {
        throw StrategyException("Strategy not found.");
    }
};

#endif