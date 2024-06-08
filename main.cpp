#include <iostream>
#include <string>

#include "src/functor.hpp"
#include "src/strategy.hpp"

using namespace std;

template <typename ReturnType, typename... Args>
Functor<ReturnType, Args...> make_functor(ReturnType (*func)(Args...))
{
     return Functor<ReturnType, Args...>(std::function<ReturnType(Args...)>(func));
}

int add(int a, int b)
{
     return a + b;
}

int subtract(int a, int b)
{
     return a - b;
}

float add_float(float a, float b)
{
     return a + b;
}

int main()
{
     Strategy<string, int, int, int> int_strategy;

     int_strategy.register_strategy("add", make_functor(add));
     int_strategy.register_strategy("subtract", make_functor(subtract));

     int_strategy.set_strategy("add");
     cout << int_strategy.execute(1, 2) << endl;

     int_strategy.set_strategy("subtract");
     cout << int_strategy.execute(1, 2) << endl;

     return 0;
}