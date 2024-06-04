#include <iostream>
#include <string>
#include <mutex>
#include <unordered_map>
#include <map>
#include <sstream>
#include <thread>
#include <deque>
#include <cstdlib>
#include <random>

#include "src/problem.hpp"
#include "src/gp.hpp"
#include "src/node.hpp"
#include "src/util.hpp"

using namespace std;

tuple<Problem, Problem> splitProblem(Problem problem, float ratio, int seed = time(NULL))
{
     int split = problem.data.size() * ratio;

     Problem train = Problem(problem.terminals, {}, problem.output_column);
     Problem test = Problem(problem.terminals, {}, problem.output_column);

     deque<map<string, float>> data = problem.data;

     deque<map<string, float>> class_false = {};
     deque<map<string, float>> class_true = {};

     for (map<string, float> row : data)
     {
          if (row[problem.output_column] == 0)
          {
               class_false.push_back(row);
          }
          else
          {
               class_true.push_back(row);
          }
     }

     shuffle(class_false.begin(), class_false.end(), default_random_engine(seed));
     shuffle(class_true.begin(), class_true.end(), default_random_engine(seed - 1));

     float false_train_size = class_false.size() * ratio;
     float true_train_size = class_true.size() * ratio;

     for (int i = 0; i < class_false.size(); i++)
     {
          if (i < false_train_size)
          {
               train.data.push_back(class_false[i]);
          }
          else
          {
               test.data.push_back(class_false[i]);
          }
     }

     for (int i = 0; i < class_true.size(); i++)
     {
          if (i < true_train_size)
          {
               train.data.push_back(class_true[i]);
          }
          else
          {
               test.data.push_back(class_true[i]);
          }
     }

     return make_tuple(train, test);
}

GPParameters getParametersFromEnv(string filepath = ".env")
{
     ifstream file(filepath);

     if (!file.is_open())
     {
          cerr << "Error opening file!" << endl;
          throw exception();
     }

     string line;
     map<string, string> env;

     while (getline(file, line))
     {
          stringstream ss(line);
          string key;
          string value;

          getline(ss, key, '=');
          getline(ss, value);

          env[key] = value;
     }

     GPParameters params = {
         env.find("POPULATION_SIZE") != env.end() ? stoi(env["POPULATION_SIZE"]) : POPULATION_SIZE,
         env.find("NUM_GENERATIONS") != env.end() ? stoi(env["NUM_GENERATIONS"]) : NUM_GENERATIONS,
         env.find("MUTATION_RATE") != env.end() ? stof(env["MUTATION_RATE"]) : MUTATION_RATE,
         env.find("PERMUTATION_RATE") != env.end() ? stof(env["PERMUTATION_RATE"]) : PERMUTATION_RATE,
         env.find("CROSSOVER_RATE") != env.end() ? stof(env["CROSSOVER_RATE"]) : CROSSOVER_RATE,
         env.find("ELITISM_RATE") != env.end() ? stof(env["ELITISM_RATE"]) : ELITISM_RATE,
         env.find("MUTATION_GRADIENT") != env.end() ? stof(env["MUTATION_GRADIENT"]) : MUTATION_GRADIENT,
         env.find("PERMUTATION_GRADIENT") != env.end() ? stof(env["PERMUTATION_GRADIENT"]) : PERMUTATION_GRADIENT,
         env.find("CROSSOVER_GRADIENT") != env.end() ? stof(env["CROSSOVER_GRADIENT"]) : CROSSOVER_GRADIENT,
         env.find("ELITISM_GRADIENT") != env.end() ? stof(env["ELITISM_GRADIENT"]) : ELITISM_GRADIENT,
         env.find("MIN_DEPTH") != env.end() ? stoi(env["MIN_DEPTH"]) : MIN_DEPTH,
         env.find("MAX_DEPTH") != env.end() ? stoi(env["MAX_DEPTH"]) : MAX_DEPTH,
         env.find("GROW_PROPORTION") != env.end() ? stof(env["GROW_PROPORTION"]) : GROW_PROPORTION,
         env.find("ABSOLUTE_NORMAL_TRIAL_SIZE") != env.end() ? stoi(env["ABSOLUTE_NORMAL_TRIAL_SIZE"]) : ABSOLUTE_NORMAL_TRIAL_SIZE,
         env.find("SPLIT_SEED") != env.end() ? stoi(env["SPLIT_SEED"]) : SPLIT_SEED,
         env.find("RUN_SEED") != env.end() ? stoi(env["RUN_SEED"]) : RUN_SEED,
         DEFAULT_THRESHHOLDS,
     };

     return params;
}

GPResult runGP(int seed = 0)
{
     GPParameters params = getParametersFromEnv();

     Problem problem = Problem("diabetes_cleaned.csv", "Outcome");
     deque<string> fields = problem.terminals;
     DEFAULT_THRESHHOLDS = initialize_mean_threshholds(problem.data, DEFAULT_THRESHHOLDS);

     tuple<Problem, Problem> split = splitProblem(problem, 0.8, params.split_seed);

     Problem train = get<0>(split);
     Problem test = get<1>(split);

     cout << "Train size: " << train.data.size() << endl;
     cout << "Test size: " << test.data.size() << endl;

     params.run_seed = seed;

     GeneticProgram gp = GeneticProgram(&train, params, &test);

     GPResult result = gp.run(1);

     cout << "=== Result ===" << endl;
     cout << "Train Fitness: " << result.fitness << endl;
     cout << "Time: " << result.time << "s" << endl;
     cout << "Seed: " << result.seed << endl;
     cout << "=== Test ===" << endl;

     Result test_result = gp.test(result.best, &test);
     cout << "True Positives: " << test_result.true_positives << endl;
     cout << "True Negatives: " << test_result.true_negatives << endl;
     cout << "False Positives: " << test_result.false_positives << endl;
     cout << "False Negatives: " << test_result.false_negatives << endl;
     cout << "Num Cases: " << test_result.num_cases << endl;
     cout << "--- " << endl;

     float test_fitness = (test_result.true_positives + test_result.true_negatives) / (float)test_result.num_cases;

     cout << "Test Fitness: " << test_fitness << endl;

     result.fitness = test_fitness;
     result.result = test_result;

     return result;
}

int main()
{
     deque<GPResult> results = {};

     for (int i = 0; i < 10; i++)
     {
          cout << "=== Run " << i << " ===" << endl;
          GPResult result = runGP(i);
          results.push_back(result);
     }

     for (GPResult result : results)
     {
          cout << "Fitness: " << result.fitness << " True Positives: " << result.result.true_positives << " True Negatives: " << result.result.true_negatives << " False Positives: " << result.result.false_positives << " False Negatives: " << result.result.false_negatives << " Num Cases: " << result.result.num_cases << " Time: " << result.time << " Seed: " << result.seed << endl;
     }

     cout << "=== Summary ===" << endl;

     GPResult best = results[0];
     for (GPResult result : results)
     {
          if (result.fitness > best.fitness)
          {
               best = result;
          }
     }

     cout << "Best Fitness: " << best.fitness << " True Positives: " << best.result.true_positives << " True Negatives: " << best.result.true_negatives << " False Positives: " << best.result.false_positives << " False Negatives: " << best.result.false_negatives << " Num Cases: " << best.result.num_cases << " Time: " << best.time << " Seed: " << best.seed << endl;

     return 0;
}