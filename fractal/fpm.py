# File carries out Vector Quantisation
# To preduce a context function
# To form distributions for Fractal Prediction Machines
# The idea is that it can be quickly trained here
# And then the final model can be moved to C++ if it works
# Author: Charlie Street

import math
import random
import csv
import numpy as np
import matplotlib.pyplot as plot # can do some graphs optimising no. of codebook vectors

# finds the euclidean distance between two
# vectors (numpy arrays) x and y
def euclideanDistance(x, y):
	D = x.shape[0]
	sumSquare = 0.0
	for i in range(D):
		sumSquare += pow((x[i] - y[i]), 2.0)
	return sqrt(sumSquare)

# function finds the closest codebook vector
# to point x, and returns the index in B of
# of the closest codebook vector
def findClosestCodebook(x,B):
	minDistance = float('inf') # set to infinitely high value
	minIndex = -1 # nothing found yet

	# loop through the codebook vectors
	for i in range(len(B)):
		newDistance = euclideanDistance(x,B[i])
		if(newDistance < minDistance):
			minDistance = newDistance
			minIndex = i

	# return the index of the closest codebook vector
	return minIndex

# function finds the quantisation error 
# or the total distance of each point
# to its closest codebook vector
def quantisationError(X,B):
	totalError = 0.0

	# loop through each input sample
	for i in range(0,len(X)):
		totalError += euclideanDistance(X[i],B[findClosestCodebook(X[i],B)])

	return totalError

# function finds the current learning rate
# as a function of the time step t, and temperature tau
def learningRate(t, tau):
	return exp(-float(t)/float(tau))

# carries out vector quantisation
# to find a set of codebook vectors
# which represent the centres of M clusters
# in data set X
# maxIterations is the maximum number of times
# to iterate over the training set
# errorMargin is the maximum error difference
# we can tolerate to consider the code book as stabilised 
# D is the dimensionality of the data
# tau is the temperature for the learning rate
def vectorQuantisation(X,M,D,maxIterations,errorMargin,tau):
	
	# randomly initialise the codebook vectors 
	B = []
	for i in range(M):
		# initialise to a D-dim vector containing values in interval [0,1)
		B.append(np.random.rand(D))

	currentError = 0.0 # just initialise to 0, will be far from true value
	for t in range(maxIterations):

		# randomly shuffle X to help remove any sort of bias from input ordering
		random.shuffle(X)

		for i in range(len(X)):
			j = findClosestCodebook(X[i],B)
			B[j] = B[j] + learningRate(t,tau) * (X[i] - B[j])

		newError = quantisationError(X,B) # find the revised error
		
		# check if the codebook vectors have converged
		diff = fabs(currentError,newError)
		currentError = newError
		if(diff <= errorMargin):
			break # escape from the main for loop and return


	return B, currentError # return the error too to save calculations

# function will analyse the quantisation error for different values of M
# this can then be plotted in order to find a 'knee' in the graph
# which represents the optimal number of codebook vectors
# writeCSV is an output file to write the results too
# repeats is the number of repeats to carry out for each M
# to account for random initialisation
def optimiseNumberOfCodebooks(X,maxM,D,maxIterations, errorMargin, tau, writeCSV, repeats):

	with open(writeCSV,'w') as outputFile:
		
		resultWriter = csv.writer(outputFile)

		for M in range(maxM):
			
			print('Trying M = ' + str(M)) # just give some logging

			avgQError = 0.0
			
			# repeat to remove bias provided by random initialisation
			for r in range(repeats):

				print('Repeat ' + str(r) + ' for M = ' + str(M))

				_,error = vectorQuantisation(X,M,D,maxIterations,errorMargin,tau)
				avgQError += error
			
			avgQError /= float(repeats) # get the average quantisation error over repeated runs

			resultWriter.writerow([str(M), str(avgQError)])

	print('Finished Optimising Number Of Codebook Vectors')


# function will plot the results found
# by the optimiseNumberOfCodebooks() function
# it will be plot as a matplotlib graph
# resultsFile is a file path the the output
# from optimiseNumberOfCodebooks() 
def plotCodebookGraph(resultsFile):

	M = [] # values for M, the number of codebook vectors
	Q = [] # values for Quantisation Error

	with open(resultsFile,'r') as results:
		csvReader = csv.reader(results)
		
		# read in the results
		for row in csvReader:
			M.append(int(row[0]))
			M.append(float(row[1]))

	# convert read in values into numpy array for use with matplotlib
	M = np.asarray(M)
	Q = np.asarray(Q)

	# set up the plot axes and data to be shown
	plt.title('A graph of Quantisation Error against the number of Codebook Vectors, M')
	plt.xlabel('Number of Codebook Vectors, M')	
	plt.ylabel('Quantisation Error')
	plt.plot(M,Q,'bx')

	# display the plot
	plt.show()

# the main function to be executed
if __name__ == '__main__':
	print('FILL ME IN')
