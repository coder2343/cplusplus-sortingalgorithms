"""
CSC-301
project1plotting.py
Fall 2022
Eric A. Autry
09/09/22
"""

"""
This script will open the .dat files output by project1tests.cpp and generate 
a series of plots of runtime vs n for the different algs.

Note: This script is not carefully written... I do not claim that this is good
      style, it probably is not. It is also not stable to a wide variety of 
      minor changes (like the format of the dat filenames or the actual data).
      This will really only work in the case that it is used with the project
      code as given in 2022. If that code changes, this script will likely have
      to change as well.
"""

# Import plotting packages.
import matplotlib.pyplot as plt

# Import math for the log function.
import math

# Define the algs...
algData = [['Selection Sort', 'ss'],
           ['Insertion Sort', 'is'],
           ['Bubble Sort', 'bs'],
           ['Merge Sort', 'ms'],
           ['Quick Sort', 'qs']]

# Setup the suffixes.
suffData = [['Unsorted', '_rand.dat'],
            ['Sorted', '_sort.dat']]

# Setup the list of algs to use for log-log plotting.
loglogAlgs = ['Selection Sort', 'Insertion Sort', 'Bubble Sort']

# Create the plot figures.
plt.figure(1) # The unsorted linear plot.
plt.figure(2) # The unsorted log-log plot.
plt.figure(3) # The sorted linear plot.
plt.figure(4) # The sorted log-log plot.

# Loop over the algs and suffixes.
for alg in algData:
    for suff in suffData:
        # Open the file.
        with open(alg[1]+suff[1], 'r') as file:
            # Read the file (they are small, so this isn't too bad).
            lines = file.readlines()

            # Grab the n values and process.
            nValsLine = lines[0]
            nValsStrip = nValsLine.rstrip()
            nValsSplit = nValsStrip.split('\t')
            nVals = [float(n) for n in nValsSplit]

            # Grab the runtimes and process.
            tValsLine = lines[1]
            tValsStrip = tValsLine.rstrip()
            tValsSplit = tValsStrip.split('\t')
            tVals = [float(t) for t in tValsSplit]

            # Get the log of n and t values.
            # Fill with -16 (machine epsilon) if was 0.
            logN = [(math.log10(n) if n>0 else -16) for n in nVals]
            logT = [(math.log10(t) if t>0 else -16) for t in tVals]

            # Check if we are working with sorted or unsorted.
            # I hate this...
            if suff[0] == 'Unsorted':
                # Switch to plt figure 1, the non-log figure.
                plt.figure(1)
                plt.plot(nVals,tVals,label=alg[0])

                # Switch to plt figure 2, the log figure.
                if alg[0] in loglogAlgs:
                    plt.figure(2)
                    plt.plot(logN,logT,label=alg[0])
            else:
                # Switch to plt figure 3, the non-log figure.
                plt.figure(3)
                plt.plot(nVals,tVals,label=alg[0])

                # Switch to plt figure 2, the log figure.
                if alg[0] in loglogAlgs:
                    plt.figure(4)
                    plt.plot(logN,logT,label=alg[0])


# Finish the plots and save.

plt.figure(1) # The unsorted linear plot.
plt.legend(loc='upper right')
plt.title('All runtimes versus n for unsorted input')
plt.xlabel('Input Size n')
plt.ylabel('Runtime (ms)')
plt.savefig('unsorted_lin.png', bbox_inches='tight')

plt.figure(2) # The unsorted log-log plot.
plt.legend(loc='upper left')
plt.title('Log-Log runtimes versus n for unsorted input')
plt.xlabel('log(n)')
plt.ylabel('log(runtime)')
plt.savefig('unsorted_log.png', bbox_inches='tight')

plt.figure(3) # The sorted linear plot.
plt.legend(loc='upper right')
plt.title('All runtimes versus n for sorted input')
plt.xlabel('Input Size n')
plt.ylabel('Runtime (ms)')
plt.savefig('sorted_lin.png', bbox_inches='tight')

plt.figure(4) # The sorted log-log plot.
plt.legend(loc='upper left')
plt.title('Log-Log runtimes versus n for sorted input')
plt.xlabel('log(n)')
plt.ylabel('log(runtime)')
plt.savefig('sorted_log.png', bbox_inches='tight')

# Close the plots.
plt.close('all')
