# file contains functionality for
# converting extracted/isolated guitar tracks
# and forming a training set out of them
# Author: Charlie Street

import math
import numpy as np 
import scipy.io.wavfile as wav


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

	return segments


segs = removeSilence('test/silence_test6.wav',0.5)

for i in range(0,len(segs)):
	print(segs[i].shape)
	wav.write('test/test'+str(i)+'.wav',44100,segs[i])

print(len(segs))