# file contains functionality for
# converting extracted/isolated guitar tracks
# and forming a training set out of them
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

	if (freq <= freqList[0]): # if note smaller than our range
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


# takes the wav file at filePath and segments it based on silences that appear within it
# maxSilence is the maximum amount of silence tolerable in a segment in seconds
# the sampling rate is also returned for completeness
def removeSilence(filePath, maxSilence):
	
	rate, data = wav.read(filePath) #everything I read in should be mono

	#data = data.astype(float)

	silenceSamples = round(rate * maxSilence) # max number of samples of silence before segmentation


	segments = [] # list of segments, all of which can be written back to wave files
	start = 0 # start of current segment
	end = 0 # end of current segment
	silent = False
	justAppend = False
	nonZero = False

	for i in range(0,data.shape[0]):

		# don't do anything until we reach at least some nonzero value 
		if (not nonZero):
			start = i
			if(data[i] != 0.0):
				nonZero = True
			else:
				continue 

		if ((not silent) and (data[i] == 0.0)):
			end = i
			silent = True
		elif (silent and (data[i] != 0.0) and i <= (end + silenceSamples)):
			silent = False
		elif (silent and (data[i] != 0.0) and i > (end + silenceSamples)):
			silent = False
			start = i
			justAppend = False
		elif (silent and (data[i] == 0.0) and i >= (end + silenceSamples) and (not justAppend)):
			segments.append(data[start:end]) # append the current segment
			justAppend = True
		elif (i == data.shape[0] - 1 and (not justAppend)):
			endPoint = end if silent else i
			segments.append(data[start:endPoint]) # make sure to append the final segment

	return segments, rate

# function takes a segment from a wave file and splits it
# into an input sample with an associated predicted output
# this will be returned as a numpy array for the input
# and a list of integers for the predicted output
# data is the mono wave file segment
# fs is the sampling frequency
# segmentSize, overlap are parameters for the short time fourier transform
# minNoteLength is the minimum amount of time a note must last for if it is to be considered valid
# numNotes is how many notes I wish to split by 
def getInOut(data, fs, segmentSize, overlap, minNoteLength, numNotes):

	# first calculate the minimum number of bins for a note
	samplesPerBin = float(segmentSize - overlap)
	minLengthSamples = minNoteLength * fs
	minTimeBins = math.ceil(minLengthSamples/samplesPerBin)
	# i always opt to go over the threshold rather than below
	# i'd much rather have a longer min note length

	# have data stored as floats
	data = data.astype(float)

	# get frequency, note lists
	freqList, noteList = genFreqNoteArrs()

	# now convert the signal into a spectrogram
	_, _, Zxx = sig.stft(data,fs=fs,window='hann',nperseg=segmentSize,noverlap=overlap)

	# have matrix in terms of power spectral density
	ZxxMag = np.abs(Zxx)

	# calculate information about frequency bins
	freqRes = float(fs)/float(segmentSize)

	# now attempt to find the last numNotes notes
	endNotes = []
	currentNote = 0 # 0 represents silence here
	currentLength = 0
	splitAtNextChange = False
	splitPoint = ZxxMag.shape[1] - 1 # the point at which to split between input and output

	for j in range(ZxxMag.shape[1]-1, -1, -1): # for each column going backwards in time

		# find max index in current column
		maxIndex = np.argmax(ZxxMag[:,j])
		if(ZxxMag[maxIndex,j] == 0.0): # trying to account for silence
			activeNote = 0
		else:
			activeNoteOne = findClosestNote((freqRes*maxIndex),freqList,noteList)
			activeNoteTwo = findClosestNote((freqRes*maxIndex) + (freqRes/2.0),freqList,noteList)
			if (activeNoteOne == activeNoteTwo):
				activeNote = activeNoteOne
			elif (activeNoteOne == currentNote or activeNoteTwo == currentNote):
				activeNote = currentNote
			else:
				activeNote = activeNoteTwo


		if (activeNote == currentNote): # if the same note remains
			currentLength += 1
			if (currentLength == minTimeBins and activeNote != 0): # if now considered a proper note
				endNotes.append(currentNote)
				if (len(endNotes) == numNotes):
					splitAtNextChange = True # when this note finishes, stop and split
		else:
			if (splitAtNextChange):
				splitPoint = j + 1 if (j+1 < ZxxMag.shape[1]) else ZxxMag.shape[1] - 1
				break
			currentNote = activeNote
			currentLength = 1

	splitSpectrogram = Zxx[:,0:splitPoint] # remove the section of time included in our output value list

	# convert remaining spectrogram back into the time domain
	_,inputSample = sig.istft(splitSpectrogram,fs=fs,window='hann',nperseg=segmentSize,noverlap=overlap)

	# convert back into the range [-1, 1] such that it can be written back to a wave file
	inputSample = inputSample / np.max(inputSample) 

	return inputSample, list(reversed(endNotes)) # reverse the list so it lies in chronological order


# function takes one file and converts it into the training data format
def createTrainingItem(inPath, outPath, configPath):
	fs, wavFile = wav.read(inPath)

	numNotes = 8
	segment_size = 8192
	overlap = 7680
	inputSample, notes = getInOut(wavFile,fs,int(segment_size),int(overlap),0.07,numNotes)
	if (len(notes) == numNotes):
		wav.write(outPath,fs, inputSample) # write the wav file out 

		# now write to the csv file
		file = open(configPath,'a')
		newTrainingItem = outPath
		for item in notes:
			newTrainingItem += ',' + str(item)

		newTrainingItem += '\n'
		file.write(newTrainingItem)
		file.close()

# function generates all training data and the associated config file
def getAllTrainingData():
	
	print('Started Training Data Formation')

	inputPrefix = 'D:/silenced'
	inputFiles = os.listdir(inputPrefix)

	configPath = 'D:/trainingData.csv'

	outputPrefix = 'D:/training/sample'
	outputSuffix = '.wav'
	fileCount = 1

	for item in inputFiles:
		inPath = inputPrefix + '/' + item
		outPath = outputPrefix + str(fileCount) + outputSuffix
		p = Process(target=createTrainingItem,args=(inPath,outPath,configPath))
		p.start()
		p.join()

		print('Completed Solo Number: ' + str(fileCount))
		fileCount += 1

	print('Finished Training Data Formation')

# function takes a file and splits it through extended periods of silence
# for now I am choosing a max silence threshold of 0.5 seconds
def segmentWavFile(path, startCount, outPrefix, outSuffix):
	segments, rate = removeSilence(path, 0.5)
	
	count = startCount
	
	for item in segments:
		filename = outPrefix + str(count) + outSuffix
		wav.write(filename,rate,item)
		print('Written file: ' + filename)
		count += 1

# runs silence segmentation on all training items
def segmentAllFiles():

	print('Started segmentation process')

	prefix = 'D:/filtered'
	filesNoPrefix = os.listdir(prefix)

	outDir = 'D:/silenced'
	outPrefix = 'D:/silenced/noSilenceSolo'
	outSuffix = '.wav'

	index = 1
	solo = 1

	for item in filesNoPrefix:
		p = Process(target=segmentWavFile,args=(prefix+'/'+item,index,outPrefix,outSuffix))
		p.start()
		p.join()

		print('Completed solo number: ' + str(solo))
		solo += 1
		index = len(os.listdir(outDir)) + 1

	print('Finished segmentation process')

if __name__ == '__main__':

	'''segs, fs = removeSilence('test/silence_test6.wav',0.5)

	for i in range(0,len(segs)):
		print(segs[i].shape)
		wav.write('test/test'+str(i)+'.wav',44100,segs[i])

	print(len(segs))'''

	'''rate, wavFile = wav.read('../../../Dropbox/audio_extraction/permanatingHystPoint3LowPoint1.wav')

	print('Started Function')
	inputSample, notes = getInOut(wavFile,rate,int(8192),int(7680),0.07,8)

	wav.write('test/permSplit.wav',rate,inputSample)

	print(notes)'''

	#segmentAllFiles()
	getAllTrainingData()