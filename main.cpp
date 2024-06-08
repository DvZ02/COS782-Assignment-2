#include <iostream>
#include <string>

#include "src/functor.hpp"

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
     Functor<int, int, int> add_functor = make_functor(subtract);

     int result = add_functor(3, 4);
     std::cout << "Result of add_functor(3, 4): " << result << std::endl;

     Functor<float, float, float> add_float_functor = Functor<float, float, float>(std::function<float(float, float)>(add_float));

     float result_float = add_float_functor(3.5, 4.5);
     std::cout << "Result of add_float_functor(3.5, 4.5): " << result_float << std::endl;

     return 0;
}