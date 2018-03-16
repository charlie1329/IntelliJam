# File expands upon fpm.py
# By changing the training data into directions
# so I can train a model for choosing the directions of notes
# Author: Charlie Street

import numpy as np 
import csv
import fpm
import math
import random
import keyDetection as key

# function is similar to readInSequences
# but finds a sequence of directions
# no movement is ignored
def readInSequenceDirection(trainingFile):

	sequences = []

	# open the csv file
	with open(trainingFile,'r') as trainFile:
		csvTrain = csv.reader(trainFile)

		currentSequence = []
		currentPrevious = -1

		for row in csvTrain:
			if(row[0][0] == '*'): # if start of new sample
				if(currentSequence != []):
					sequences.append(currentSequence)
					currentSequence = []
					currentPrevious = -1
			else: # if continuing a sample
				newNote = int(row[0])
				if(newNote == 0):
					continue
				if(currentPrevious != -1 and newNote != currentPrevious):
					if(newNote > currentPrevious):
						currentSequence.append(1)
					else:
						currentSequence.append(0)
					currentPrevious = newNote

				if(currentPrevious == -1):
					currentPrevious = newNote

		# append the final sequence as it won't get caught in my training file format
		sequences.append(currentSequence)

	return sequences

# basically a carbon copy from fpm.py, but with new reading function used instead
def trainDirFPM(trainingFile, L, k, magA, M, tau, maxIterations, errorMargin):

	# read in the training set
	sequences = readInSequenceDirection(trainingFile)

	# form every other part of the training set
	CBR, t, blocks = fpm.getDataSet(sequences,L,k,magA)

	# carry out vector quantisation
	D = math.ceil(math.log(magA,2))
	B, error = fpm.vectorQuantisation(CBR,M,D,maxIterations,errorMargin,tau)
	print('Quantisation Error: ' + str(error))

	# now form the distribution matrix
	N = fpm.formDistributionMatrix(blocks,B,magA,t,k)

	# return everything necessary to predict in the future
	return B,N,t,k

# carbon copy from fpm.py, but with new reading function
def optimiseDirFPMCodebooks(trainingFile,L,k,magA,maxM,tau,maxIterations,errorMargin,repeats):

	# read in the training set
	sequences = readInSequenceDirection(trainingFile)
	print('Read In Sequences, number of sequences: ' + str(len(sequences)))

	# form every other part of the training set
	CBR, t, blocks = fpm.getDataSet(sequences,L,k,magA)
	print('Got Data Set, length of data set: ' + str(len(CBR)))

	# optimise the vector quantiser
	resultsFile = 'graphs_and_results/codebookOptimisationDirection.csv'
	D = math.ceil(math.log(magA,2))
	fpm.optimiseNumberOfCodebooks(CBR,maxM,D,maxIterations, errorMargin, tau, resultsFile, repeats)

	# once we know the best number of codebook vectors,
	# then we can train the best model possible

# version of prediction for direction
# no tilted distributions!
def predictNextDir(s,B,N,t,k,upInterval):
	# bring sequence to chaos representation
	x = fpm.toChaosRep(t,k,s)

	# now find the closest codebook vector
	i = fpm.findClosestCodebook(x,B)
	#print(N)
	#print('STOP')
	# do some random sampling to find the direction
	distribution = np.copy(N[i,:]) # need to copy to stop bad things happening
	T = 1.9
	if(upInterval > 6 and upInterval != 12):
		distribution[1] *= math.exp(-(upInterval-6.0)/T)
	elif(upInterval < 6 and upInterval != 0):
		distribution[0] *= math.exp(-(6.0-upInterval)/T)
	
	distribution = np.round(distribution)
	print('DIS: ' + str(distribution))
	totalSamples = np.sum(distribution)
	print('SAMPLES: ' + str(totalSamples))
	print(distribution)
	randomNo = random.randint(1,totalSamples)
	total = 0
	for i in range(distribution.shape[0]):
		total += distribution[i]
		if(randomNo <= total):
			return i

# tests the new reading function
def unitTests():
	sequences = readInSequenceDirection('test/directionTest.csv')
	print('Expected: 2, Got: ' + str(len(sequences)))
	print('Expected: [1, 0, 0], Got: ' + str(sequences[0]))
	print('Expected: [0, 1, 1], Got: ' + str(sequences[1]))

# combines the note predictor and direction predictor
# sequence has the durations with it
# outputLen is the length of the output sequence
def combinedPredict(NNote, BNote, tNote, kNote, TNote, NDir, BDir, tDir, kDir, sequence, outputLen):
	absSequence = []
	noteSequence = []
	dirSequence = []
	previous = -1

	# fill up all the appropriate lists
	for i in range(len(sequence)):
		absSequence.append(sequence[i][0]) # just the note
		if(sequence[i][0] == 0):
			noteSequence.append(sequence[i])
			continue
		else:
			noteSequence.append(((sequence[i][0] % 12) + 1,sequence[i][1]))


		if(previous != -1 and previous != sequence[i][0]):
			if(sequence[i][0] > previous):
				dirSequence.append(1)
			else:
				dirSequence.append(0)

			previous = sequence[i][0]

		if(previous == -1):
			previous = noteSequence[i][0]


	# now do the key detection and stuff
	segmentsAndKeys = key.detectKey(noteSequence,2.0,2.0) # split into groupings of two seconds
	transposed = []
	noDuration = []

	for pair in noteSequence:
		noDuration.append(pair[0])

	for segment in segmentsAndKeys:
		transposed.extend(key.transpose(noDuration[segment[0]:segment[1]], segment[2], 'C'))
	
	print(absSequence)
	print(noteSequence)
	print(dirSequence)
	print(transposed)

	endKey = segmentsAndKeys[len(segmentsAndKeys)-1][2] # get the key to transpose back to

	# now generate the note sequence
	startPointNote = len(transposed)
	for _ in range(outputLen):
		transposed.append(fpm.predictNext(transposed,BNote,NNote,tNote,kNote,TNote))

	transposed = key.transpose(transposed,'C',endKey) # transpose back to original key
	predictedSequence = transposed[startPointNote:len(transposed)]
	print(predictedSequence)
	# now generate the direction sequence to generate the end sequence
	startPointOut = len(absSequence) # where to start 
	previousNote = absSequence[startPointOut-1] 
	for i in range(outputLen):
		upInterval = (((predictedSequence[i] - 1) % 12) - (previousNote % 12)) % 12
		dirSequence.append(predictNextDir(dirSequence,BDir,NDir,tDir,kDir,upInterval))
		newDirection = dirSequence[len(dirSequence) - 1]
		if(predictedSequence[i] == 0): # silence
			absSequence.append(0)
		elif ((predictedSequence[i] - 1) % 12 == previousNote % 12): # same note
			# try again
			newNote = previousNote
			secondDraw = predictNextDir(dirSequence[0:len(dirSequence)-1],BDir,NDir,tDir,kDir,upInterval)
			if(secondDraw == 1 and newDirection == 1):
				newNote = previousNote + 12
				if(newNote > 79):
					newNote = previousNote
			elif(secondDraw == 0 and newDirection == 0):
				newNote = previousNote - 12
				if(newNote < 24):
					newNote = previousNote 
			absSequence.append(newNote)
			previousNote = newNote # set for next time round
		else: #standard case
			predictedMod = (predictedSequence[i] - 1) % 12
			previousMod = previousNote % 12
			newNote = -1
			if(newDirection == 1):
				newNote = previousNote + ((predictedMod - previousMod) % 12)
			else:
				newNote = previousNote - ((previousMod - predictedMod) % 12)

			if(newNote < 24):
				newNote += 12
			
			if(newNote > 79):
				newNote -= 12

			absSequence.append(newNote)
			previousNote = newNote



	return absSequence[startPointOut:len(absSequence)]



if __name__ == '__main__':
	trainingFile = 'D:/trainingPairsSoftBound.csv'
	L = 20
	k = 0.5
	magA = 2 # 0 = -, 1 = +
	maxM = 100
	tau = 4
	maxIterations = 1000
	errorMargin = 0.001
	repeats = 3
	M = 5
	#optimiseDirFPMCodebooks(trainingFile,L,k,magA,maxM,tau,maxIterations,errorMargin,repeats)
	#B, N, t, k = trainDirFPM(trainingFile, L, k, magA, M, tau, maxIterations, errorMargin) # 5 found to be the best

	#np.savetxt('matrices/B_dir.txt',B,fmt='%f')
	#np.savetxt('matrices/N_dir.txt',N,fmt='%f')
	#np.savetxt('matrices/t_dir.txt',t,fmt='%f')

	B_note = np.loadtxt('matrices/B_allC.txt')
	N_note = np.loadtxt('matrices/N_allC.txt')
	t_note = np.loadtxt('matrices/t_allC.txt')
	k_note = k
	T_note = 0.4

	B_dir = np.loadtxt('matrices/B_dir.txt')
	B_dir = np.reshape(B_dir,(5,1)) # careful here!
	N_dir = np.loadtxt('matrices/N_dir.txt')

	t_dir = np.loadtxt('matrices/t_dir.txt')
	t_dir = np.reshape(t_dir,(2,1))
	k_dir = k
	outputLen = 20

	print(np.power(N_note,1.0/0.4))

	#sequence = [(46,0.1),(48,0.1),(51,0.1),(53,0.1),(55,0.1),(53,0.1),(51,0.1),(53,0.1),(48,0.1),(51,0.1)]
	sequence = [(50,2),(50,1),(54,1),(55,1),(57,3),(59,2),(57,2),(55,1),(54,1),(55,1),(52,1)]
	predictedSequence = combinedPredict(N_note, B_note, t_note, k_note, T_note, N_dir, B_dir, t_dir, k_dir, sequence, len(sequence))
	print(predictedSequence)
	rhythm = np.asarray([2,1,1,1,3,2,2,1,1,1,1])
	np.random.shuffle(rhythm)
	print(rhythm)

	#sequence = [(51,0.371519),(53, 0.371519),(55, 0.18576),(34, 0.18576),(48, 0.18576),(51, 0.371519),(53, 0.18576),(52, 0.18576)]
	#predictedSequence = combinedPredict(N_note, B_note, t_note, k_note, T_note, N_dir, B_dir, t_dir, k_dir, sequence, 1)

