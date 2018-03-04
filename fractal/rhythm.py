# This file will aim to train an ESN
# based off of rhythmic data
# in order to try and learn how to respond rhythmically to phrases
# Author: Charlie Street

import numpy as np
import csv
import random

# function will read in durations from the training file
# will return an list of lists
def readInDurations(trainingFile):
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
				currentSequence.append(float(row[1]))


		# append the final sequence as it won't get caught in my training file format
		sequences.append(currentSequence)

	return sequences

# formulates a training set
# from a lot of samples
def formTrainingSet(sequences,inLength):
	trainingPairs = []
	for sequence in sequences:
		if(len(sequence) > inLength):
			for i in range(len(sequence)-inLength):
				trainingPairs.append((sequence[i:i+inLength],sequence[i+inLength]))

	return trainingPairs

# generate all the necessary matrices
def initialiseESN(reservoirSize,inResWeight, scrWeight, crjWeight, jumpSize):
	inRes = np.asmatrix(np.full((reservoirSize,1),inResWeight))

	for i in range(inRes.shape[0]):
		for j in range(inRes.shape[1]):
			draw = np.random.randint(2)
			if (draw == 1):
				inRes[i,j] *= -1


	reservoir = np.asmatrix(np.zeros((reservoirSize,1)))

	resRes = np.asmatrix(np.zeros((reservoirSize,reservoirSize)))

	for i in range(reservoirSize-1):
		resRes[i+1,i] = scrWeight

	resRes[0,reservoirSize-1] = scrWeight

	for i in range(0, reservoirSize - jumpSize, jumpSize):
		resRes[i,(i+jumpSize)%reservoirSize] = crjWeight
		resRes[(i+jumpSize)%reservoirSize,i] = crjWeight

	resOut = np.asmatrix(np.random.rand(1,reservoirSize))

	return inRes, resRes, reservoir, resOut


# updates the reservoir
def updateReservoir(input, inResWeights, reservoir, resResWeights):
	return np.tanh((inResWeights * input) + (resResWeights * reservoir))

def predict(resOutWeights, reservoir):
	return resOutWeights * reservoir

# takes a set of training pairs, and then converts it into reservoirs with output sample
def computeReservoirs(trainingPairs,inResWeights,resResWeights,reservoir):
	reservoirs = []
	i = 1
	for pair in trainingPairs:
		print('Computing New Reservoir: ' + str(i))
		for inSample in pair[0]:
			reservoir = updateReservoir(inSample,inResWeights,reservoir,resResWeights)

		reservoirs.append((np.copy(reservoir),pair[1]))
		reservoir = np.asmatrix(np.zeros((reservoir.shape[0],reservoir.shape[1])))
		i += 1
	return reservoirs 

# takes a list of pairs and forms an X and t matrix so stuff can be trained properly
def formTrainingMatrices(reservoirs,reservoirSize):
	size = len(reservoirs)
	X = np.asmatrix(np.zeros((size,reservoirSize)))
	t = np.asmatrix(np.zeros((size,1)))

	for i in range(size):
		t[i,0] = reservoirs[i][1]
		X[i,:] = reservoirs[i][0][:,0]

	return X, t

def ridgeRegression(X, t, lamb,reservoirSize):
	beforeInv = X.T * X + lamb * np.asmatrix(np.identity(reservoirSize))
	return np.linalg.inv(beforeInv)*X.T*t

def predictSequence(inSequence,outLength, inRes, resRes, resOut,reservoirSize):
	reservoir = np.asmatrix(np.zeros((reservoirSize,1)))
	for newIn in inSequence:
		reservoir = updateReservoir(newIn,inRes,reservoir,resRes)

	output = []
	for i in range(outLength):
		newDur = predict(resOut,reservoir)
		reservoir = updateReservoir(newDur,inRes,reservoir,resRes)
		output.append(newDur[0,0])

	return output


if __name__ == '__main__':
	trainingFile = 'D:/trainingPairsOneOctaveAllC.csv'
	inputLength = 20
	dur = readInDurations(trainingFile)
	samples = formTrainingSet(dur,inputLength)
	reservoirSize = 200
	inResWeight = 1.0
	resResUni = 0.9
	resResBi = 0.4
	jumpSize = 13
	lamb = 0.005
	'''inRes, resRes, reservoir, resOut = initialiseESN(reservoirSize,inResWeight,resResUni,resResBi,jumpSize)
	reservoirs = computeReservoirs(samples,inRes,resRes,reservoir)
	X, t = formTrainingMatrices(reservoirs,reservoirSize)
	print('Starting Ridge Regression')
	resOutWeights = ridgeRegression(X, t, lamb, reservoirSize)

	np.savetxt('rhythm_mats/inRes.txt',inRes,fmt='%f')
	np.savetxt('rhythm_mats/resRes.txt',resRes,fmt='%f')
	np.savetxt('rhythm_mats/resOut.txt',resOutWeights,fmt='%f')'''

	inRes = np.asmatrix(np.reshape(np.loadtxt('rhythm_mats/inRes.txt'),(reservoirSize,1)))
	resRes = np.asmatrix(np.reshape(np.loadtxt('rhythm_mats/resRes.txt'),(reservoirSize,reservoirSize)))
	resOut = np.asmatrix(np.reshape(np.loadtxt('rhythm_mats/resOut.txt'),(1,reservoirSize)))
	sequence = [0.3,0.3,0.15,0.15,0.3,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.3]
	for i in range(len(sequence)):
		sequence[i] /= 3

	print(predictSequence(sequence,20,inRes,resRes,resOut,reservoirSize))