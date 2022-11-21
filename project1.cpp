/*
 * CSC-301
 * project1.cpp
 * Fall 2022
 *
 * Partner 1: Liam Walsh
 */

#include "project1.hpp"

using namespace std;

/*
 * selectionSort
  input is array of type double 
    array is sorted via selection sort algorithm
 */
void selectionSort(vector<double> &arrayToSort) {
    // outter for loop to keep track of unsorted subsection of array 
    // will use to slect range to find min elmement from
    for(int i=0; i< arrayToSort.size()-1; i++ ){
        // set min_index to be whatever i is 
        int min_idx=i;
        // sort winow here is from i+1 to arrayTOSort.size
        for( int j=i+1; j<arrayToSort.size(); j++){
            if(arrayToSort[j]<arrayToSort[min_idx]){
                min_idx=j;
            }
        }
        if (min_idx != i){
            // emplpy C++ swap function to swap pices
            // note C++ swaping for vectors is constant time operation 
            // see documenetation link for more information
            // https://cplusplus.com/reference/algorithm/swap/
            swap(arrayToSort[min_idx], arrayToSort[i]);
            }
    }
    return;
}

/*
 * insertionSort
  input is array of type double 
  return array of sorted doubles which is sorted in place
 */
void insertionSort(vector<double> &arrayToSort) {
    // insertion sort involves working backwards in sorting array.
    // first split the array into sorted and unsorted parts
    // outter loop moves pivot gradulay forward
    for(int i=1; i<arrayToSort.size(); i++){
        double keyValue=arrayToSort[i];
        int j=i-1;
        // while loop 
        // move elements that are greater then keyValuw to 1 postion ahead
        // of keyValues current pos.
        while(j>=0 && arrayToSort[j]>keyValue){
            arrayToSort[j+1]=arrayToSort[j];
            j--;
        }
        // set j+1 to equal keyValue
        arrayToSort[j+1]=keyValue;
    }
    return;
}


/*
 * bubbleSort
  input is array of type double 
  return array of sorted doubles which is sorted in place
 */
void bubbleSort(vector<double> &arrayToSort) {
    // bubble sort outter loop 
    bool swapElem = false;
    for(int i=0; i< arrayToSort.size()-1; i++ ){
    // start with j = 0 and end at n-i-1
    // swap elements where j> j+1 with each other
    // bublle through elements gradulay by swaping them with each other.
    // optumize elements by swa[ check]
        for(int j=0; j<arrayToSort.size()-i-1; j++){
            if (arrayToSort[j]>arrayToSort[j+1]){
                // swap elements at index j and j +1 with each other
                swap(arrayToSort[j], arrayToSort[j+1]);
                // check if bubbleing was perfomed
                swapElem= true;
            }
        }
        // if no bubleing was perfomed then array was sorted and we can break
        if(!swapElem){
            break;
            }
    }
    return;
}

/*
 * mergeSort
  input is array of type double 
  sorts input rray via merge sort sorting algorithm
 */
void mergeSort(vector<double> &arrayToSort) {
    if(arrayToSort.size()<=1){
        return;
    }
    int midPoint=arrayToSort.size() / 2;
    int n=arrayToSort.size();
    // build vectors 
    // first call merge sort on half of array 
    // use python style array sliceses for vecOne and vecTwo to efficently create new array halves to sort
    vector<double> vecOne(arrayToSort.begin(),arrayToSort.begin()+midPoint);
    vector<double> vecTwo(arrayToSort.begin()+midPoint,arrayToSort.begin()+arrayToSort.size());
    // call merge sort on both halves of array
    mergeSort(vecOne);
    mergeSort(vecTwo);
    // merge both halves of the array together into new array useing these counters.
    // i is for vecOne, j is for vectwo and k is for arrayToSort
    int i=0;
    int j=0;
    int k=0;
    // while vecOne and vecTwo haven't been fully iterated
    // iterate across the array
    while(i< vecOne.size() && j<vecTwo.size()){
        // if element in vec one is less then element at curent index in vectwo
        // then add vecOne element to arrayToSort
        // otherwise carry out the oposite statement in problem bellow
        if(vecOne[i]<vecTwo[j]){
            arrayToSort[k]=vecOne[i];
            i++;
        }
        else{
            arrayToSort[k]=vecTwo[j];
            j++;
        }
        k++;
    }
    // add unsorted elements from vec one to main array. happens if main while loop terminates 
    // do getting to end of one of array halves.
    while(i<vecOne.size()){
        arrayToSort[k]=vecOne[i];
        i++;
        k++;
    }
    // similar case just for vecTwo
    while(j<vecTwo.size()){
        arrayToSort[k]=vecTwo[j];
        j++;
        k++;
    }
}

// int partion function
//   input is array of type double 
// return pivot to be used for quicksort alogrithm
// sorts elements to proper place in array via  first element pivot statagey
// returns new index to pivot off of
int partition(vector<double> &arrayToSort, int i, int j){
    // have low index start at i 
    // have high index start at j, end of the array
    int lowIndex = i; 
    int highIndex = j;
    double pivot = arrayToSort[i];
    while (lowIndex < highIndex){
        // if current element at low index smaller then partion then keep advancing 
        while (pivot>=arrayToSort[lowIndex]){
            lowIndex++;
        }
        // if current element at high index is greater than pivot then keep lowering index by 1
        while (pivot<arrayToSort[highIndex]){
            highIndex--;
        }
        // else then if low index is less then high index and you cant keep advancing 
        // then you have elements that are on the wrong side of pvivot
        // so we swap the elements.
        if (lowIndex<highIndex){
            swap(arrayToSort[lowIndex], arrayToSort[highIndex]);
        }
    }
    // lastly we swap element at begining of array with highIndex to properly place pivot in array
    swap(arrayToSort[i], arrayToSort[highIndex]);
    return highIndex;
}
  
/*
 * quickSortHelper
input is array of type double 
 * Note that i is inclusive (i.e., is the first element of subarray).
 * Note that j is not inclusive (i.e., off the end of the subarray).
 * Will sort array passed into function by applying quicksort sorting algorithm
 */
void quickSortHelper(vector<double> &arrayToSort, int i, int j) {
    if(i<j){
        // part is the partion index to base quicksort around
        int part = partition(arrayToSort,  i,  j);
        // sort elments before partion and after the partion
        // via seperatley recursive calls.
        quickSortHelper(arrayToSort, i, part-1);
        quickSortHelper(arrayToSort, part+1, j);
    }
    // base case just return
return;
}

/*
 * quickSort
  calls helper quicksort function taht actually does the majorty of work for quicksort function
  peramitors are passed into quicksortHelper by default are arraytosort, 
  0 for sorting algortihm startpoint,
  and arrayToSort.size()-1 is end point of algorithm
return array of sorted doubles which is sorted in place
 */
void quickSort(vector<double> &arrayToSort){
    // quicksort hellper function call that actulaly does all work
    quickSortHelper(arrayToSort, 0, arrayToSort.size()-1);
    return;

}