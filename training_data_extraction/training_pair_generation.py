# file contains functionality
# for turning a series of audio samples
# into lists of pairs, where each pair
# represents a note and a corresponding duration
# Author: Charlie Street

import math
import numpy as np 
import scipy.io.wavfile as wav 
import scipy.signal as sig 
import os
from multiprocessing import Process

# function generates arrays representing lookup tables
# to find the closest note for a frequency
# first array returned is the raw frequency values
# second array is the corresponding note for my convention
def genFreqNoteArrs():
	
	noteNums = list(range(24,80)) # A2 - E6 inclusive (octave starts from A here)

	A2 = 55.0 #The note in A2 in Hz
	freqs = [A2 * pow(pow(2.0,1.0/12.0),n) for n in range(0,len(noteNums))] # fill by list comprehension

	return freqs, noteNums

# function finds the cent offset between two notes
# freq1, freq2 are the raw frequencies in Hz
def centOffset(freq1,freq2):
	return 1200 * math.log(freq2/freq1, 2.0)

# for a given frequency, find the closest note to it
# using lists of frequencies and corresponding notes
def findClosestNote(freq,freqList,noteList):

	if (freq <= freqList[0]): #if note smaller than our range
		return noteList[0]
	elif (freq >= freqList[len(freqList)-1]): # if note larger than our range
		return noteList[len(noteList)-1]
	else:
		for i in range(0,len(freqList)-1): # only want up to the penultimate item
			if (freq >= freqList[i] and freq < freqList[i+1]):
				# calculate offsets
				leftOffset = abs(centOffset(freq,freqList[i]))
				rightOffset = abs(centOffset(freq,freqList[i+1]))
				if (leftOffset <= rightOffset):
					return noteList[i]
				else:
					return noteList[i+1]

# function takes a single wav file
# and processes it into a list of pairs
# and then writes it to the training file
def processSingleFile(inputPath,trainingPath,segmentSize,overlap,minNoteLength,sampleNo):

	# firstly read in the wav file
	fs, data = wav.read(inputPath)
	data = data.astype(float)

	# calculate some values useful for future calculation
	samplesPerBin = float(segmentSize - overlap)
	minLengthSamples = minNoteLength * fs
	minTimeBins = math.ceil(minLengthSamples/samplesPerBin)
	freqRes = float(fs)/float(segmentSize)

	# get the arrays used for finding notes
	freqList, noteList = genFreqNoteArrs()

	# now convert the audio sample into a spectrogram
	_, _, Zxx = sig.stft(data,fs=fs,window='hann',nperseg=segmentSize,noverlap=overlap)

	# compute magnitude of spectrogram values
	ZxxMag = np.abs(Zxx)

	notes = [] # an array for storing all the (note,duration) pairs
	currentNote = 0 # represents 0 here
	currentLength = 0 # in terms of spectrogram bins (converted to seconds later)

	nonZeroFound = False
	bigEnough = False # ;)

	for j in range(0,ZxxMag.shape[1]): # move forwards in time through each column

		# find max index in current column
		maxIndex = np.argmax(ZxxMag[:,j])
		if (ZxxMag[maxIndex,j] == 0.0): # trying to account for silence
			activeNote = 0

			if (not nonZeroFound): # don't do anything until we reach some non-zero value 
				continue
		else:
			nonZeroFound = True
			activeNoteOne = findClosestNote((freqRes*maxIndex),freqList,noteList)
			activeNoteTwo = findClosestNote((freqRes*maxIndex) + (freqRes/2.0),freqList,noteList)
			if (activeNoteOne == activeNoteTwo):
				activeNote = activeNoteOne
			elif (activeNoteOne == currentNote or activeNoteTwo == currentNote):
				activeNote = currentNote
			else:
				activeNote = activeNoteTwo

		if (activeNote == currentNote): # the current note continues
			currentLength += 1
			if(currentLength == minTimeBins): # if we've reached the minimum length for a note to be considered
				bigEnough = True
		else: # a new note is being played
			if (bigEnough):
				duration = (float(currentLength) * float(samplesPerBin)) / float(fs) # duration in seconds
				notes.append((currentNote,duration))

			bigEnough = False
			currentNote = activeNote
			currentLength = 1 

	if(bigEnough): # if note sustained at end that wasn't written
		duration = (float(currentLength) * float(samplesPerBin)) / float(fs) # duration in seconds
		notes.append((currentNote,duration))

	# now write to the csv file
	if(len(notes) > 1): # we need at least one pair of pairs or we can't train anything!

		#if it ends with silence, remove the ending silence
		if(notes[len(notes)-1][0] == 0):
			notes = notes[0:len(notes)-1]

		# open the csv file
		file = open(trainingPath,'a')
		
		toWrite = '****Sample ' + str(sampleNo) + '****\n' #have an identifier for each new sample
		
		for item in notes: # write all the pairs
			toWrite += str(item[0]) + ',' + str(item[1]) + ',\n'
		file.write(toWrite)
		file.close()

# function generates all of the training data into a csv file
def generateTrainingData():
	print('Started Training Data Generation')

	inputPrefix = 'D:/silenced'
	inputFiles = os.listdir(inputPrefix)

	# where all the training data will be ultimately stored
	trainingPath = 'D:/trainingPairs.csv'

	fileCount = 1

	segmentSize = 8192
	overlap = 7680
	minNoteLength = 0.07

	# loop through each audio file and process them in a
	# separate process to ensure proper cleanup of resources
	for item in inputFiles:
		inPath = inputPrefix + '/' + item
		p = Process(target=processSingleFile,args=(inPath,trainingPath,segmentSize,overlap,minNoteLength,fileCount))
		p.start()
		p.join()

		print('Completed Solo Number: ' + str(fileCount))
		fileCount += 1

	print('Finished Training Data Generation')


# if we are main, then run the training data generation procedure
if __name__ == '__main__':
	generateTrainingData()
