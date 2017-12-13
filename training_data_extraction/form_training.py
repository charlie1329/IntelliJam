# file contains functionality for
# converting extracted/isolated guitar tracks
# and forming a training set out of them
# Author: Charlie Street

import math


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


