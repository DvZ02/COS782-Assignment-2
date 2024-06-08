#include <iostream>
#include <string>

#include "src/functor.hpp"
#include "src/strategy.hpp"

using namespace std;

int add(int a, int b)
{
     return a + b;
}

int subtract(int a, int b)
{
     return a - b;
}

enum class Operation
{
     ADD,
     SUBTRACT,
     MULTIPLY,
};

int main()
{

     Functor<int, int, int> add_functor = Functor<int, int, int>(std::function<int(int, int)>(add));
     Functor<int, int, int> subtract_functor = Functor<int, int, int>(std::function<int(int, int)>(subtract));

     Strategy<Operation, int, int, int> int_strategy(Operation::SUBTRACT, subtract_functor);

     int_strategy.register_strategy(Operation::ADD, add_functor);
     int_strategy.register_strategy(Operation::SUBTRACT, subtract_functor);

     int_strategy.set_strategy(Operation::ADD);
     cout << int_strategy(10, 4) << endl;

     int_strategy.set_strategy(Operation::SUBTRACT);
     cout << int_strategy(10, 4) << endl;

     int_strategy.set_strategy(Operation::MULTIPLY);
     cout << int_strategy(10, 4) << endl;

     return 0;
}