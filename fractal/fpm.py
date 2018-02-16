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

#****DATA SET READING/CONVERSION CODE****

# function takes in the path to a training file
# and reads it into an list of lists (of varying length)
# this list of lists is returned
def readInSequences(trainingFile):

	sequences = []

	# open up the csv file for processing
	with open(trainingFile,'r') as trainFile:
		csvTrain = csv.reader(trainFile)

		currentSequence = []

		for row in csvTrain:
			if (row[0][0] == '*'): # if start of new sample
				if (currentSequence != []):
					sequences.append(currentSequence)
					currentSequence = []
			else: # if continuing a sample
				currentSequence.append(int(row[0]))


		# append the final sequence as it won't get caught in my training file format
		sequences.append(currentSequence)

	return sequences

# function generates a list of vectors
# each vector is a corner on the D dimensional hypercube
# where D = ceil(log |A|)
def generateCorners(magA):
	D = math.ceil(math.log(magA,2))

	corners = []

	# generate the vectors in the same way
	# as you would bitstrings
	for i in range(magA):
		newCorner = np.zeros(D)
		currentVal = i
		for j in range(D):
			if (currentVal >= pow(2,D-1-j)):
				newCorner[j] = 1
				currentVal -= pow(2,D-1-j)
		corners.append(newCorner)

	return corners

# function applies contractive map i
# on point x on the D-dimensional hypercube
# t_i is a corner on this hypercube
# and k is the contraction coefficient
def applyMap(t_i,k,x):
	return (k * x) + ((1.0 - k) * t_i)

# function converts a sequence into 
# a chaos (game/block) representation
# returned is a point on 
# the D-dimensional hypercube,
# representing the sequence
def toChaosRep(t,k,S):
	
	# initial x value is in the centre of the hypercube
	D = math.ceil(math.log(t[0].shape[0],2))
	x = np.zeros(D)
	x.fill(0.5)

	# iterate through the sequence, applying the maps
	for item in S:
		x = applyMap(t[item],k,x)

	return x

# function will take a set of sequences of 
# varying length and will take all L sized blocks
# out of it, and return this as a list of blocks
def getAllLBlocks(sequences, L):
	blocks = []

	for sequence in sequences:
		if (len(sequence) >= L):
			for i in range(len(sequence) - L + 1):
				blocks.append(sequence[i:i+L])

	return blocks

# function takes a training file 
# and converts it onto a D-dimensional hypercube
# returned are the corners
# L is the block size (maximum memory depth)
# k is the contraction coefficient
# mag A is the size of the input alphabet
def getDataSet(sequences, L, k, magA):
	
	t = generateCorners(magA)
	blocks = getAllLBlocks(sequences,L)

	CBR = []

	# for each allowed block, convert in to chaos representation
	for LBlock in blocks:
		CBR.append(toChaosRep(t, k, LBlock))

	# return t too for prediction usage
	# return blocks for N formation
	return CBR, t, blocks


#****VECTOR QUANTISATION CODE****

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
			print('Average Quantisation Error: ' + str(avgQError))

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

#****PROBABILITY DISTRIBUTION FORMING CODE****

# Assuming we already have the codebook vectors,
# we now go about forming the distribution matrix
def formDistributionMatrix(blocks, B, magA, t, k):

	# initialise all matrix elements to zero
	N = np.zeros((len(B),magA)) 

	for block in blocks:
		x = toChaosRep(t,k,block)
		i = findClosestCodebook(x,B)
		a = block[len(block)-1]
		N[i,a] += 1

	return N


#****PREDICTION TESTING CODE****
def predictNext(s,B,N,t,k):

	# bring sequence to chaos representation
	x = toChaosRep(s,t,k)

	# now find the closest codebook vector
	i = findClosestCodebook(x,B)

	# no need to normalise the probabilities
	# just take the maximum value
	# to get the prediction
	return np.argmax(N[i,:])

#****INTEGRATION CODE****

# function trains a single FPM
# from training file trainingFile
# with maximum memory depth L
# with contraction coefficient k
# alphabet size magA
# number of codebook vectors M
# learning rate temperature tau
# maximum number of iterations maxIterations
# quantisation error margin errorMargin
def trainFPM(trainingFile, L, k, magA, M, tau, maxIterations, errorMargin):

	# read in the training set
	sequences = readInSequences(trainingFile)

	# form every other part of the training set
	CBR, t, blocks = getDataSet(sequences,L,k,magA)

	# carry out vector quantisation
	D = math.ceil(math.log(magA,2))
	B, error = vectorQuantisation(CBR,M,D,maxIterations,errorMargin,tau)
	print('Quantisation Error: ' + str(error))

	# now form the distribution matrix
	N = formDistributionMatrix(blocks,B,magA,t,k)

	# return everything necessary to predict in the future
	return B,N,t,k

# function optimises vector quantiser by number of codebooks
# and then plots the results
# parameters same as trainFPM except:
# maxM is the range of codebook vector numbers to try
# repeats is the number of repeats in the optimisation to try
def optimiseFPMCodebooks(trainingFile, L, k, magA, maxM, tau, maxIterations, errorMargin, repeats):

	# read in the training set
	sequences = readInSequences(trainingFile)

	# form every other part of the training set
	CBR, t, blocks = getDataSet(sequences,L,k,magA)

	# optimise the vector quantiser
	resultsFile = 'codebookOptimisation.csv'
	D = math.ceil(math.log(magA,2))
	optimiseNumberOfCodebooks(CBR,maxM,D,maxIterations, errorMargin, tau, resultsFile, repeats)

	# now plot the results as a graph
	# we are looking for the 'knee' in this graph
	plotCodebookGraph(resultsFile)

	# once we know the best number of codebook vectors,
	# then we can train the best model possible

#****MAIN FUNCTION****

# the main function to be executed
if __name__ == '__main__':
	print('FILL ME IN')
	corners = generateCorners(13)
	for i in range(13):
		print(corners[i])

	test = [np.random.rand(10)]
	print(test)
	print(getAllLBlocks(test,2))
