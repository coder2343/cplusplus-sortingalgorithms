/*
 * CSC-301
 * project1tests.cpp
 * Fall 2022
 * Eric A. Autry
 * 09/08/22
 */

#include "project1tests.hpp"

using namespace std;

/* testingSuite
 *
 * This function will run a number of tests using the input algorithm, check if
 * the sorting was successful, and print which tests failed (if any).
 *
 * This is not an exhaustive list of tests by any means, but covers the edge
 * cases for your sorting algorithms.
 * 
 * INPUTS
 * sortFunc: function pointer for alg to test.
 * 
 * OUTPUTS
 * Printed statements indicating which tests passed/failed.
 */
void testingSuite(function<void(vector<double>&)> sortFunc) {

    // Create and seed the rng to ensure reproducibility.
    std::default_random_engine rnd{1};
    std::uniform_real_distribution<double> dist(0, 1);

    // Create the list of tests.
    list<vector<double>> testList;
    list<string> testNames;

    vector<double> singletonArray{1};
    testList.push_back(singletonArray);
    testNames.push_back("singleton array");

    vector<double> repeatedElems{1,2,3,4,5,5,4,3,2,1};
    testList.push_back(repeatedElems);
    testNames.push_back("repeated elements");

    vector<double> allRepeatedElems{2,2,2,2,2,2,2,2,2,2};
    testList.push_back(allRepeatedElems);
    testNames.push_back("all repeated elements");
    
    vector<double> descendingOrder{10,9,8,7,6,5,4,3,2,1};
    testList.push_back(descendingOrder);
    testNames.push_back("descending order");
    
    vector<double> sortedInput{1,2,3,4,5,6,7,8,9,10};
    testList.push_back(sortedInput);
    testNames.push_back("sorted input");
    
    vector<double> negInputs{-1,-2,-3,-4,-5,-5,-4,-3,-2,-1};
    testList.push_back(negInputs);
    testNames.push_back("negative inputs");
    
    vector<double> mixedSignInputs{1,2,3,4,5,-1,-2,-3,-4,-5,0};
    testList.push_back(mixedSignInputs);
    testNames.push_back("mixed positive/negative");
    
    vector<double> sizePowTwoMinusOne(pow(2,6)-1);
    vector<double> randRealNums(pow(2,6)-1);
    for ( int i=0; i<sizePowTwoMinusOne.size(); i++ ) {
        sizePowTwoMinusOne[i] = i;
        randRealNums[i] = dist(rnd);
    }
    shuffle(sizePowTwoMinusOne.begin(), sizePowTwoMinusOne.end(), rnd);
    testList.push_back(sizePowTwoMinusOne);
    testNames.push_back("array of size 2^k - 1");
    testList.push_back(randRealNums);
    testNames.push_back("random real numbers");
    
    // Track how many tests tried and passed.
    int tried = 0;
    int passed = 0;

    // Create an iterator for the test names.
    auto nameItr = testNames.begin();

    // Loop over the tests.
    for ( auto testItr=testList.begin(); testItr!=testList.end(); ++testItr ) {

        // Create and sort a copy for comparison later.
        vector<double> sComp((*testItr).begin(), (*testItr).end());
        sort(sComp.begin(), sComp.end());

        // Try to sort, but allow for errors.
        try {
            sortFunc(*testItr);
            if ( is_sorted( (*testItr).begin(), (*testItr).end() ) 
                 and 
                 equal( (*testItr).begin(), (*testItr).end(), sComp.begin() )
               ) {
                tried++;
                passed++;
                cout << "Succeded test " << tried << ": " << *nameItr << endl;
            } else {
                tried++;
                cout << "Failed test " << tried << ": " << *nameItr << endl;
            }
        } catch ( const exception& ex ) {
            tried++;
            cout << "DANGER!" << endl;
            cout << "Test #" << tried << ": " << *nameItr << " threw an error!"
                 << endl;
            cout << "Error: " << typeid(ex).name() << " " << ex.what() << endl;
        } catch (...) {
            cout << "Unknown exception caught!" << endl;
            throw; // Re-throws the exception.
        }

        // Increment the name iterator if not done.
        if ( nameItr != testNames.end() ) {
            ++nameItr;
        }
    }

    // Report on number of passed tests.
    cout << "Passed " << passed << "/" << tried << " tests." << endl;

    return;
}

/* calcLogLogSlope
 *
 * This function uses least squares to find the log-log slope given the input
 * data, the lengths n and the corresponding timing results.
 *
 * INPUTS
 * nVals: the lengths of the arrays used in the timed tests.
 * timings: the timing results for those lengths.
 *
 * OUTPUTS
 * returns the corresponding log-log slope.
 */
double calcLogLogSlope(vector<int> &nVals, vector<double> &timings) {
    // Setup the variables needed for the least squares solver.
    char TRANS = 'N';
    int M = nVals.size();
    int N = 2;
    int NRHS = 1;
    vector<double> Amat(2*M);
    int LDA = M;
    vector<double> Bmat(M);
    int LDB = M;
    int LWORK = 16; // I hate fortran.
    vector<double> WORK(LWORK);
    int INFO;

    // Fill the matrices with the n and timing info.
    for ( int i=0; i<M; i++ ) {
        if ( nVals[i] > 0 ) {
            Amat[i] = log10(nVals[i]);
        } else {
            Amat[i] = -16; // Set to ~ machine epsilon.
        }
        Amat[i+M] = 1;
        if ( timings[i] > 0 ) {
            Bmat[i] = log10(timings[i]);
        } else {
            Bmat[i] = -16; // Set to ~ machine epsilon.
        }
    }

    // Perform the least squares.
    dgels_(&TRANS, &M, &N, &NRHS, &*Amat.begin(), &LDA, &*Bmat.begin(), &LDB, 
           &*WORK.begin(), &LWORK, &INFO);

    // Get the slope and output it.
    return Bmat[0];
}

/* measureTime
 *
 * This function will generate lists of varying lengths and sort them using 
 * your implemented fuction. It will time these sorting operations, and store 
 * the average time across 30 trials of a particular size n, and write the 
 * results to the given filename. It will also print the slope of the log-log 
 * plots generated (if requested).
 *
 * INPUTS
 * sortFunc: function pointer for alg to test.
 * Nstart, Nend, Nstep: the start, end, and step size used to determine the 
 *                      length of the arrays to time.
 * preSorted: set to true to test with only pre-sorted inputs.
 * numTrials: the number of trials to average timing data across.
 * calcSlope: set to true to calculate the slope of the log-log plots.
 * filename: the name of the file for storing the results.
 * 
 * OUTPUTS
 * The timed results will be written to file and statistics about the slope of 
 * the log-log plot will be printed during execution.
 */
void measureTime(function<void(vector<double>&)> sortFunc, int Nstart, 
                 int Nend, int Nstep, bool preSorted, int numTrials, 
                 bool calcSlope, string filename) {
    // Print info about the experiment we are about to perform.
    if (preSorted) {
        cout << "Timing algorithm using only sorted data." << endl;
    } else {
        cout << "Timing algorithm using random data." << endl;
    }
    cout << "Averaging over " << numTrials << " Trials." << endl;

    // Create and seed the rng to ensure reproducibility.
    std::default_random_engine rnd{1};
    std::uniform_real_distribution<double> dist(0, 1);

    // Create an array of the array lengths to time and an array to store the 
    // results of the timed trials.
    vector<int> nVals(1+(Nend-Nstart)/Nstep);
    vector<double> timings(1+(Nend-Nstart)/Nstep); // Default filled with 0s.

    // A flag to ensure that the alg is sorting correctly while we time it.
    bool isCorrect = true;

    // Loop over the different lengths.
    for ( int i=0; i<nVals.size(); i++ ) {
        // Calculate the current n and store that value.
        int n = Nstart + i*Nstep;
        nVals[i] = n;

        // Loop over the trials.
        for ( int tInd=0; tInd<numTrials; tInd++ ) {
            // Create a pre-sorted or random array of the correct length.
            vector<double> arrayToSort(n);
            for ( int j=0; j<n; j++ ) {
                if (preSorted) {
                    arrayToSort[j] = j;
                } else {
                    arrayToSort[j] = dist(rnd);
                }
            }

            // Create and sort a copy for comparison later.
            vector<double> sComp(arrayToSort.begin(), arrayToSort.end());
            sort(sComp.begin(), sComp.end());

            // Time the sort.
            auto start = chrono::high_resolution_clock::now();
            sortFunc(arrayToSort);
            auto stop = chrono::high_resolution_clock::now();

            // Get the duration in microseconds.
            auto duration = chrono::duration_cast<chrono::microseconds>
                            (stop - start);

            // Check if the sorting was correct, and flag if not.
            if ( not is_sorted(arrayToSort.begin(), arrayToSort.end()) 
                 or
                 not equal(arrayToSort.begin(),arrayToSort.end(),sComp.begin())
               ) {
                isCorrect = false;
            }

            // Add the time to our running sum.
            timings[i] += duration.count();
        }

        // Now that we have performed the trials, average the times.
        timings[i] /= numTrials;
    }

    // If the alg failed at some point, report it.
    if (not isCorrect) {
        cout << "Sorting failed during timed trial!!!" << endl;
    }

    // Write n values and timing results to file.
    ofstream outFile;
    outFile.open(filename);
    for ( int i=0; i<nVals.size(); i++ ) {
        outFile << nVals[i] << "\t";
    }
    outFile << "\n";
    for ( int i=0; i<timings.size(); i++ ) {
        outFile << timings[i] << "\t";
    }
    outFile << "\n";
    outFile.close();

    // Perform slope calculation if requested and report the result.
    if (calcSlope) {
        double slope = calcLogLogSlope(nVals, timings);
        cout << "Algorithm's log-log slope: " << slope << endl;
    }

    return;
}

/* main
 * 
 * All of your sorting algorithms get tested and timed.
 */
int main(int argc, char* argv[]) {

    cout << "Testing selectionSort" << endl << endl;
    testingSuite(&selectionSort);
    cout << endl;

    cout << "Testing insertionSort" << endl << endl;
    testingSuite(&insertionSort);
    cout << endl;

    cout << "Testing bubbleSort" << endl << endl;
    testingSuite(&bubbleSort);
    cout << endl;

    cout << "Testing mergeSort" << endl << endl;
    testingSuite(&mergeSort);
    cout << endl;

    cout << "Testing quickSort" << endl << endl;
    testingSuite(&quickSort);
    cout << endl;

    cout << "Timing selectionSort" << endl << endl;
    measureTime(&selectionSort, 100, 4000, 100, false, 30, true, "ss_rand.dat");
    cout << endl;
    measureTime(&selectionSort, 100, 4000, 100, true, 30, true, "ss_sort.dat");
    cout << endl;

    cout << "Timing insertionSort" << endl << endl;
    measureTime(&insertionSort, 100, 4000, 100, false, 30, true, "is_rand.dat");
    cout << endl;
    measureTime(&insertionSort, 2000, 40000, 1000, true, 30, true, 
                "is_sort.dat");
    cout << endl;

    cout << "Timing bubbleSort" << endl << endl;
    measureTime(&bubbleSort, 100, 4000, 100, false, 30, true, "bs_rand.dat");
    cout << endl;
    measureTime(&bubbleSort, 4000, 40000, 2000, true, 30, true, "bs_sort.dat");
    cout << endl;

    cout << "Timing mergeSort" << endl << endl;
    measureTime(&mergeSort, 2000, 40000, 1000, false, 30, false, "ms_rand.dat");
    cout << endl;
    measureTime(&mergeSort, 2000, 40000, 1000, true, 30, false, "ms_sort.dat");
    cout << endl;

    cout << "Timing quickSort" << endl << endl;
    measureTime(&quickSort, 2000, 40000, 1000, false, 30, false, "qs_rand.dat");
    cout << endl;
    measureTime(&quickSort, 100, 4000, 100, true, 30, true, "qs_sort.dat");
    cout << endl;

    return 0;
}
