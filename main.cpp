// Lukas Anthonissen - u21434345

#include <iostream>
#include <string>

#include "src/functor.hpp"
#include "src/strategy.hpp"
#include "src/strategy_exception.hpp"

using namespace std;

int add(int a, int b)
{
     return a + b;
}

int subtract(int a, int b)
{
     return a - b;
}

int devide(int a, int b){
     if(b != 0)
          return a / b;
     
     return 0;
}

float add_float(float a, float b)
{
     return a + b;
}

float subtract_float(float a, float b)
{
     return a - b;
}

float multiply_float(float a, float b)
{
     return a * b;
}

enum class Operation
{
     ADD,
     SUBTRACT,
     MULTIPLY,
     DIVIDE
};

int main()
{

     Functor<int, int, int> add_functor = Functor<int, int, int>(add);
     Functor<int, int, int> subtract_functor = Functor<int, int, int>(subtract);
     Functor<int, int, int> devide_functor = Functor<int, int, int>(devide);

     Functor<float, float, float> add_float_functor = Functor<float, float, float>(add_float);
     Functor<float, float, float> subtract_float_functor = Functor<float, float, float>(subtract_float);
     Functor<float, float, float> multiply_float_functor = Functor<float, float, float>(multiply_float);

     Strategy<Operation, StrategyException, int, int, int> int_strategy(Operation::SUBTRACT, subtract_functor);
     Strategy<Operation, StrategyException, float, float, float> float_strategy(Operation::ADD, add_float_functor);

     int_strategy.register_strategy(Operation::ADD, add_functor);
     int_strategy.register_strategy(Operation::SUBTRACT, subtract_functor);
     int_strategy.register_strategy(Operation::DIVIDE, devide_functor);

     float_strategy.register_strategy(Operation::SUBTRACT, subtract_float_functor);
     float_strategy.register_strategy(Operation::MULTIPLY, multiply_float_functor);

     int_strategy.set_strategy(Operation::ADD);
     cout << "ADD: 10, 4 = " << int_strategy(10, 4) << endl;

     int_strategy.set_strategy(Operation::SUBTRACT);
     cout << "SUBTRACT: 10, 4 = " << int_strategy(10, 4) << endl;

     int_strategy.set_strategy(Operation::DIVIDE);
     cout << "DIVIDE: 10, 4 = " << int_strategy(10, 4) << endl;

     int_strategy.remove_strategy(Operation::DIVIDE);
     cout << "DIVIDE(actually ADD): 10, 4 = " << int_strategy.execute_by_key(Operation::DIVIDE, 10, 4) << endl;

     float_strategy.set_strategy(Operation::SUBTRACT);
     cout << "SUBTRACT: 8.5, 4 = " << float_strategy(8.5, 4) << endl;

     float_strategy.set_strategy(Operation::ADD);
     cout << "ADD: 8.5, 4 = " << float_strategy(8.5, 4) << endl;

     float_strategy.set_strategy(Operation::MULTIPLY);
     cout << "MULTIPLY: 8.5, 4.5 = " << float_strategy(8.5, 4.5) << endl;

     cout << "The next line should throw an exception" << endl;
     int_strategy.set_strategy(Operation::MULTIPLY);
     cout << int_strategy(10, 4) << endl;


     return 0;
}