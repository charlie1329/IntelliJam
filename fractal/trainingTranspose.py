# file uses the key detection functionality to 
# transpose the entire training file into C Major
# Author: Charlie Street

import csv
import keyDetection as key
import numpy as np

TRAINING_IN = 'D:/trainingPairsOneOctave.csv'
TRAINING_OUT = 'D:/trainingPairsOneOctaveAllC.csv'
SINGLE_KEY = 'C'
MIN_SAMPLE_LENGTH = 21 # coincide with fractal prediction machine stuff

# function reads in from csv file, and 
# stores all samples in a list of lists
# minSampleLength interacts with the fractal prediction machine stuff
def readInSamples(filePath, minSampleLength):
	samples = []
	currentSample = []

	with open(filePath,'r') as readFile:
		csvReader = csv.reader(readFile)
		for row in csvReader: # go through each row
			if(row[0][0] == '*'): # start of a segment
				if(len(currentSample) >= minSampleLength):
					samples.append(currentSample)
					currentSample = []
			else:
				currentSample.append((int(row[0]),float(row[1])))

	if(len(currentSample) >= minSampleLength):
		samples.append(currentSample)

	return samples

# function finds the key of all segments, and then 
# writes the results to a new csv file
def transposeSamples(samples,segmentLength,modulationPenalty):
	with open(TRAINING_OUT, 'w') as outFile:
		i = 1
		for sample in samples:
			outFile.write('****Sample ' + str(i) + '****\n')

			# get the key information
			keys = key.detectKey(sample,segmentLength,modulationPenalty)
			for item in keys:
				segment = sample[item[0]:item[1]]

				segmentJustNotes = []
				segmentJustDurations = []
				for pair in segment:
					segmentJustNotes.append(pair[0])
					segmentJustDurations.append(pair[1])

				transposed = key.transpose(segmentJustNotes,item[2],SINGLE_KEY) # transpose into C Major

				for j in range(len(transposed)):
					outFile.write(str(transposed[j]) + ',' + str(segmentJustDurations[j]) + ',\n')

			print('Finished Transposing Sample: ' + str(i))
			i += 1

def transposeEntireTrainingFile(segmentLength,modulationPenalty):
	print('Starting to transpose')
	samples = readInSamples(TRAINING_IN,MIN_SAMPLE_LENGTH)
	transposeSamples(samples,segmentLength,modulationPenalty)

# carries unit tests out on all functionality in this file
def runTests():
	print('Running trainingTransposeTests')
	samples = readInSamples(TRAINING_IN,MIN_SAMPLE_LENGTH)
	print('Number of Samples: ' + str(len(samples)))
	print('First sample: ' + str(samples[0]) + ', length of first sample: ' + str(len(samples[0])))
	transposeSamples([samples[0]],2.0,2)
	print('To check for correctness, check D:/trainingPairsOneOctaveAllC.csv')


if __name__ == '__main__':
	transposeEntireTrainingFile(2.0,2)
