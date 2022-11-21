/*
 * CSC-301
 * project1.hpp
 * Fall 2022
 *
 * Partner 1:
 * Partner 2:
 * Date:
 */

#ifndef PROJECT1_HPP_
#define PROJECT1_HPP_

#include <iostream>
#include <vector>

void selectionSort(std::vector<double> &arrayToSort);
void insertionSort(std::vector<double> &arrayToSort);
void bubbleSort(std::vector<double> &arrayToSort);
void mergeSort(std::vector<double> &arrayToSort);
void quickSortHelper(std::vector<double> &arrayToSort, int i, int j);
void quickSort(std::vector<double> &arrayToSort);

#endif  // PROJECT1_HPP_