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
import matplotlib.pyplot as plt # can do some graphs optimising no. of codebook vectors
from mpl_toolkits.mplot3d import Axes3D

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
				newNote = int(row[0])
				if(newNote >= 24): newNote -= 23 # if we have larger values read in, then normalise to start at 1
				currentSequence.append(newNote)


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
		corners.append(np.asarray(newCorner))

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
	D = t[0].shape[0]
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
			for i in range(len(sequence) - L):
				blocks.append(sequence[i:i+L+1])

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
		CBR.append(toChaosRep(t, k, LBlock[0:len(LBlock)-1]))

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
	return math.sqrt(sumSquare)

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
	return math.exp(-float(t)/float(tau))

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
		print('NEW ITERATION: ' + str(t))
		# randomly shuffle X to help remove any sort of bias from input ordering
		random.shuffle(X)

		for i in range(len(X)):
			j = findClosestCodebook(X[i],B)
			B[j] = B[j] + learningRate(t,tau) * (X[i] - B[j])

		newError = quantisationError(X,B) # find the revised error
		
		# check if the codebook vectors have converged
		diff = math.fabs(currentError - newError)
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

		for M in range(1, maxM+1):
			
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
			M.append([])#int(row[0]))
			#Q.append(float(row[1]))

	# convert read in values into numpy array for use with matplotlib
	M = np.asarray(M)
	Q = np.asarray(Q)
	print(M)
	print(Q)

	# set up the plot axes and data to be shown
	plt.title('A graph of Quantisation Error against the number of Codebook Vectors, M')
	plt.xlabel('Number of Codebook Vectors, M')	
	plt.ylabel('Quantisation Error')
	plt.axis([0,30,45000,130000]) # TODO: Set Properly once results actually known!
	plt.plot(M,Q,'bo')

	# display the plot
	plt.show()

#****PROBABILITY DISTRIBUTION FORMING CODE****

# Assuming we already have the codebook vectors,
# we now go about forming the distribution matrix
def formDistributionMatrix(blocks, B, magA, t, k):

	# initialise all matrix elements to zero
	N = np.zeros((len(B),magA)) 

	for block in blocks:
		x = toChaosRep(t,k,block[0:len(block)-1])
		i = findClosestCodebook(x,B)
		a = block[len(block)-1]
		N[i,a] += 1

	return N


#****PREDICTION TESTING CODE****
def predictNext(s,B,N,t,k):

	# bring sequence to chaos representation
	x = toChaosRep(t,k,s)

	# now find the closest codebook vector
	i = findClosestCodebook(x,B)

	# no need to normalise the probabilities
	# just take the maximum value
	# to get the prediction
	#return np.argmax(N[i,:])

	'''
	# use basis of intervals to try and influence decision
	intervalGate = np.asarray([0.5, 0.5, 3/11, 5/11, 7/11, 8/11, 10/11, 1/11, 1, 6/11, 9/11, 4/11, 2/11])

	goBack = 1
	lastNotePlayed = s[len(s) - goBack] # get last note played (not including 0)
	while lastNotePlayed == 0:
		goBack += 1
		lastNotePlayed = s[len(s) - goBack]

	#roll interval array around to match last note played
	shiftedGate = np.roll(intervalGate[1:],lastNotePlayed-1)
	shiftedGate = np.insert(shiftedGate,0,0.5) # add back in the silence element, always at 0'''

	shiftedGate = np.asarray([0.5,1,0,1,1,0,1,0,1,0,1,1,0]) # C Lydian

	# do some random sampling to generate the notes
	N = np.square(N)
	distribution = np.round(N[i,:] * shiftedGate) # apply gate for intervals
	totalSamples = np.sum(distribution)
	randomNo = random.randint(1,totalSamples)
	total = 0
	for i in range(distribution.shape[0]):
		total += distribution[i]
		if(randomNo <= total):
			return i

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
	print('Read In Sequences, number of sequences: ' + str(len(sequences)))

	# form every other part of the training set
	CBR, t, blocks = getDataSet(sequences,L,k,magA)
	print('Got Data Set, length of data set: ' + str(len(CBR)))

	# optimise the vector quantiser
	resultsFile = 'codebookOptimisationMoreOctaves.csv'
	D = math.ceil(math.log(magA,2))
	optimiseNumberOfCodebooks(CBR,maxM,D,maxIterations, errorMargin, tau, resultsFile, repeats)

	# now plot the results as a graph
	# we are looking for the 'knee' in this graph
	plotCodebookGraph(resultsFile)

	# once we know the best number of codebook vectors,
	# then we can train the best model possible

#****TESTS****

# function carries out unit tests on the code
def unitTests():
	print('Testing generateCorners')
	corners = generateCorners(13)
	for i in range(13):
		print(corners[i])
	# expect to see 13 arrays, each 
	# isomorphic to their value in binary

	print('\n\nTesting getAllLBlocks')
	test = [np.random.rand(10)]
	print(len(getAllLBlocks(test,2)))
	print('EXPECTED: 9')

	print('\n\nTesting applyMap')
	t_i = np.asarray([0.0,1.0,0.0,1.0])
	k = 0.5
	x = np.asarray([0.2,0.4,0.6,0.9])
	xNew = applyMap(t_i,k,x)
	print(xNew)
	print('EXPECTED: [0.1 0.7 0.3 0.95]')
	# expect to see [0.1,0.7,0.3,0.95]

	print('\n\nTesting toChaosRep')
	s = [1, 2, 7]
	xChaos = toChaosRep(corners,k,s)
	print(xChaos)
	print('EXPECTED: [0.0625 0.5625 0.8125 0.6875]')
	# expect to see [0.0625, 0.5625, 0.8125, 0.6875]

	print('\n\nTesting readInSequences')
	sequences = readInSequences('../training_data_extraction/test/test_pairs_one_octave.csv')
	print(len(sequences))
	print('EXPECTED: 3')
	print(sequences[0])
	print('EXPECTED: [10 11]')
	print(sequences[1])
	print('EXPECTED: [10 11 12]')
	print(sequences[2])
	print('EXPECTED: [1 2 3 4 5]')

	print('\n\nTesting getDataSet')
	magA = 13
	CBR, t, blocks = getDataSet(sequences, 2, k, magA)
	
	print('Checking t')
	for i in range(magA):
		print(corners[i].all() == t[i].all())
		print("EXPECTED: True")

	print('\nChecking blocks')
	print(len(blocks))
	print('EXPECTED: 7')
	print(blocks[0])
	print('EXPECTED: [10 11]')
	print(blocks[1])
	print('EXPECTED: [10 11]')
	print(blocks[2])
	print('EXPECTED: [11 12]')
	print(blocks[3])
	print('EXPECTED: [1 2]')
	print(blocks[4])
	print('EXPECTED: [2 3]')
	print(blocks[5])
	print('EXPECTED: [3 4]')
	print(blocks[6])
	print('EXPECTED: [4 5]')

	print('\nChecking CBR')
	print(len(CBR))
	print('EXPECTED: 7')
	print(CBR[0])
	print('EXPECTED: [0.875 0.125 0.875 0.625]')
	print(CBR[1])
	print('EXPECTED: [0.875 0.125 0.875 0.625]')
	print(CBR[2])
	print('EXPECTED: [0.875 0.625 0.375 0.375]')
	print(CBR[3])
	print('EXPECTED: [0.125 0.125 0.625 0.375]')
	print(CBR[4])
	print('EXPECTED: [0.125 0.125 0.875 0.625]')
	print(CBR[5])
	print('EXPECTED: [0.125 0.625 0.375 0.375]')
	print(CBR[6])
	print('EXPECTED: [0.125 0.875 0.125 0.625]')

	print('\n\nChecking euclideanDistance')
	x = np.asarray([3, 4, 5, 6])
	y = np.asarray([6, 8, 10, 12])
	print(euclideanDistance(x,y))
	print('EXPECTED: ~ 9.273618495')

	print('\n\nChecking findClosestCodebook')
	x = np.asarray([0.5, 0.6, 0.7, 0.8])
	B = []
	B.append(np.asarray([0, 0, 0, 0]))
	B.append(np.asarray([1, 1, 1, 1]))
	B.append(np.asarray([0.5, 0.7, 0.6, 0.9]))
	B.append(np.asarray([4, 5, 6, 7]))
	print(findClosestCodebook(x,B))
	print('EXPECTED: 2')

	print('\n\nChecking quantisationError')
	x2 = np.asarray([0.1, 0.2, 0.3, 0.4])
	X = [x, x2]
	print(quantisationError(X,B))
	print('EXPECTED: ~0.7209276383')

	print('\n\nChecking learningRate')
	time = 7
	tau = 2
	print(learningRate(time,tau))
	print('EXPECTED: ~0.03019738342')

	print('\n\nChecking vectorQuantisation')
	x1 = np.asarray([1, 2, 3, 4])
	x2 = np.asarray([2, 3, 4, 5])
	X = [x1, x2]
	B, error = vectorQuantisation(X,1,4,20000,0.2,3)
	print(len(B))
	print('EXPECTED: 1')
	print(B[0])
	print('EXPECTED: ~ [1.5 2.5 3.5 4.5]')

	print('\n\nChecking optimiseNumberOfCodebooks')
	optimiseNumberOfCodebooks(X,1,4,20000, 0.2, 3, 'test/testOptimise.csv', 10)
	print('EXPECTED: See csv file')

	print('\n\nChecking formDistributionMatrix')
	N = formDistributionMatrix(blocks,[np.asarray([0,0,0,0])],13,t,k)
	print(N)
	print('EXPECTED: 1 row, 13 columns, [0 0 1 1 1 1 0 0 0 0 0 2 1]')

	print('\n\nChecking predictNext')
	print(predictNext([1,2],[np.asarray([0,0,0,0])],N,t,k))
	print('EXPECTED: 11')

	print('\n\nChecking plotCodebookGraph')
	plotCodebookGraph('test/exampleMResults.csv')



#****MAIN FUNCTION****

# the main function to be executed
if __name__ == '__main__':
	
	# setting up parameters
	trainingFile = 'D:/trainingPairsSoftBound.csv'
	L = 20 # ADJUSTABLE
	k = 0.5 # this value is probably safe as this
	magA = 57 # fixed size of alphabet, 12 notes plus silence/79-24
	maxM = 200 # i want at least 13^2 = 169 to be tried so why not go to 200?
	tau = 4 # ADJUSTABLE
	maxIterations = 1000 # ADJUSTABLE
	errorMargin = 0.001 # ADJUSTABLE
	repeats = 3 # ADJUSTABLE (base on time taken to run)
	#M = 8


	optimiseFPMCodebooks(trainingFile, L, k, magA, maxM, tau, maxIterations, errorMargin, repeats)
	#plotCodebookGraph('codebookOptimisation.csv')

	#B,N,t,k = trainFPM(trainingFile, L, k, magA, M, tau, maxIterations, errorMargin)

	#np.savetxt('B.txt',B,fmt='%f')
	#np.savetxt('N.txt',N,fmt='%f')
	#np.savetxt('t.txt',t,fmt='%f')


	#Run the machine!
	'''
	B = np.loadtxt('B.txt')
	N = np.loadtxt('N.txt')
	t = np.loadtxt('t.txt')
	k = 0.5

	# plot one row of N
	yAxes = np.square(N[0,:])
	xAxes = np.asarray([0,1,2,3,4,5,6,7,8,9,10,11,12])
	plt.bar(xAxes,yAxes,align='center',alpha=0.5)
	plt.xticks(xAxes,xAxes)
	plt.xlabel('Note Choice')
	plt.ylabel('Observations')
	plt.title('A Single Distribution from the N matrix')
	#plt.show()


	newSequence = [11,1,4,6,8,11,1,8]
	numNotes = len(newSequence)
	for i in range(20):
		a = predictNext(newSequence,B,N,t,k)
		newSequence.append(a)

	print('First ' + str(numNotes) + ' notes human, last 20 AI')
	print(newSequence)

	'''
	





