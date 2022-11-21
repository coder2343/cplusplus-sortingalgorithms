/*
 * CSC-301
 * project1tests.hpp
 * Fall 2022
 * Eric A. Autry
 * 09/08/22
 */

#ifndef PROJECT1TESTS_HPP_
#define PROJECT1TESTS_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <list>
#include <functional>
#include <chrono>
#include <random>
#include <algorithm>
#include <math.h>

// #include <lapacke.h> // Cannot include properly, have to extern.
extern "C" void dgels_(char*, int*, int*, int*, double*, int*, double*, int*, 
                       double*, int*, int*);

#include "project1.hpp"

void testingSuite(std::function<void(std::vector<double>&)> sortFunc);

double calcLogLogSlope(std::vector<int> &nVals, std::vector<double> &timings);

void measureTime(std::function<void(std::vector<double>&)> sortFunc, 
                 int Nstart, int Nend, int Nstep, bool preSorted, 
                 int numTrials, bool calcSlope, std::string filename);

int main(int argc, char* argv[]);

#endif  // PROJECT1TESTS_HPP_