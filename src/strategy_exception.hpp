#include <exception>

#ifndef STRATEGY_EXCEPTION_HPP
#define STRATEGY_EXCEPTION_HPP

class StrategyException : public std::exception
{
public:
    StrategyException(const char *message) : message(message) {}

    const char *what() const noexcept override
    {
        return message;
    }

private:
    const char *message;
};

#endif