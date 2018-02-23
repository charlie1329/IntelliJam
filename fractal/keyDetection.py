# File carries out functionality
# For Musical Key Detection (only Major/Minor Keys)
# And Key Transposing 
# Author: Charlie Street

import numpy as np

# dictionary for going from keys to numbers
# makes it a little easier to work with in practice!
keyToVal = {
	'A'  : 1,
	'A#' : 2,
	'B'  : 3,
	'C'  : 4,
	'C#' : 5,
	'D'  : 6,
	'D#' : 7,
	'E'  : 8,
	'F'  : 9,
	'F#' : 10,
	'G'  : 11,
	'G#' : 12,
	'Am' : 13,
	'A#m': 14,
	'Bm' : 15,
	'Cm' : 16,
	'C#m': 17,
	'Dm' : 18,
	'D#m': 19,
	'Em' : 20,
	'Fm' : 21,
	'F#m': 22,
	'Gm' : 23,
	'G#m': 24 
}

# dictionary for reverse lookup
# also very useful for switching between the two
valToKey = {
	1 : 'A',
	2 : 'A#',
	3 : 'B',
	4 : 'C',
	5 : 'C#',
	6 : 'D',
	7 : 'D#',
	8 : 'E',
	9 : 'F',
	10: 'F#',
	11: 'G',
	12: 'G#',
	13: 'Am',
	14: 'A#m',
	15: 'Bm',
	16: 'Cm',
	17: 'C#m',
	18: 'Dm',
	19: 'D#m',
	20: 'Em',
	21: 'Fm',
	22: 'F#m',
	23: 'Gm',
	24: 'G#m'
}


# function transposes a sequence of notes between two keys
# src_key, dst_key are strings, i.e. keys in keyToVal
# function currently doesn't work Major -> Minor or vice versa
# and only within one octave
def transpose(sequence, src_key, dst_key):
	dstVal = keyToVal[dst_key]
	srcVal = keyToVal[src_key]
	if((dstVal > 12 and srcVal <= 12) or (dstVal <= 12 and srcVal > 12)):
		raise NotImplementedError('Transposing between major and minor keys not currently supported!')
	toShift = keyToVal[dst_key] - keyToVal[src_key]
	transposed = []
	for item in sequence:
		if(item == 0):
			transposed.append(0) # can't transpose silence
		else:
			transposed.append(((item + toShift - 1) % 12) + 1)

	return transposed

# key profiles as described in the paper by Temperley
# the arrays given here are for A Major and A minor
# to get profiles for other keys, just rotate the arrays
majorProfile = np.asarray([5.0, 2.0, 3.5, 2.0, 4.5, 4.0, 2.0, 4.5, 2.0, 3.5, 1.5, 4.0])
minorProfile = np.asarray([5.0, 2.0, 3.5, 4.5, 2.0, 4.0, 2.0, 4.5, 3.5, 2.0, 1.5, 4.0])

# for an input vector, the key value for a particular
# key is calculated
# notesPresent is the flat input vector
# key is the key
def getPitchKeyValue(notesPresent,key):
	
	profile = []

	# get correct profile
	if(key > 12):
		profile = minorProfile
		key -= 12
	else:
		profile = majorProfile

	# rotate the profile appropriately
	profile = np.roll(profile,key-1) # rotate the key around appropriately

	return np.sum(np.multiply(profile,notesPresent))



# this function will aim to detect the key of a sequence of music
# this is based on the work of Temperley
# the result is a list of keys with some way of identifying
# the segment in the original sequence, so it can be transposed.
# sequence consists of note/duration pairs
def detectKey(sequence, segmentLength, modulationPenalty):
	# first split into segments
	# will find closest to segmentLength
	segments = []
	segmentTime = 0.0
	currentSegment = []
	for i in range(len(sequence)):
		note, duration = sequence[i]
		if(segmentTime + duration > segmentLength): # if we reach the end of a segment
			if(currentSegment == []):
				segments.append([note]) # just chuck it in its own segment
				segmentTime = 0.0
				currentSegment = []
			else:
				segments.append(currentSegment)
				segmentTime = duration
				currentSegment = [note]
		else:
			currentSegment.append(note)
			segmentTime += duration
	# add on the last segment for completion's sake
	segments.append(currentSegment)

	# firstly generate input vectors
	pitchKeyVectors = []

	# calculate for each segment
	for segment in segments:
		inputVector = np.zeros(12)
		for note in segment:
			if note != 0: # if not silence
				inputVector[note-1] = 1 # real notes start from 1
		
		# now for each input vector, calculate the pitch key values for each of the 24 keys
		pitchKeyVals = np.zeros(24);
		for key in range(1,25):
			pitchKeyVals[i] = getPitchKeyValue(inputVector,key)

		pitchKeyVectors.append(pitchKeyVals) # now we have the values for the nth segment

	# then take a dynamic programming approach to find the best solution
	bestSums = []
	firstStep = []
	for item in pitchKeyVectors[0]: # no previous steps, so set previous key to -1
		firstStep.append((item,-1))

	bestSums.append(firstStep) # add the first vector initially 

	for i in range(1, len(pitchKeyVectors)): # go through from start to finish
		
		bestSumi = []
		for j in range(len(pitchKeyVectors[i])): # now find the best path to j from the previous segment
			bestJ = -1
			bestJIndex = -1
			for k in range(len(bestSums[i-1])): 
				newSum = pitchKeyVectors[i][j] + bestSums[i-1][k][0]
				
				if (j != k): # add penalty for changing key
					newSum -= modulationPenalty

				if (newSum > bestJ):
					bestJ = newSum
					bestJIndex = k

			bestSumi.append((bestJ,bestJIndex))
		bestSums.append(bestSumi)

	# now back track and retrieve the best path of keys
	bestPath = []
	maxLast = -1
	maxLastIndex = -1
	for i in range(len(bestSums[len(bestSums)-1])): # go through the last part of bestSums and find the starting point (max at the end)
		if(bestSums[len(bestSums-1)][i][0] > maxLast):
			maxLast = bestSums[len(bestSums-1)][i][0]
			maxLastIndex = i

	bestPath.append(maxLastIndex)
	index = 0
	for i in range(len(bestSums)-1, 0, -1):
		bestPath.append(bestSums[i][bestPath[index]][1])
		index += 1

	# segmentsAndKeys is a list of triples (startPoint,endPoint+1,key)
	segmentsAndKeys = []
	currentStartPoint = 0
	for i in range(len(segments)):
		currentEndPoint = currentStartPoint + len(segments[i])
		segmentsAndKeys.append((currentStartPoint,currentEndPoint,bestPath[i])) 
		currentStartPoint = currentEndPoint

	return segmentsAndKeys


# function runs a series of unit tests on the code
def runTests():

	# test the tranposing function
	GPhrase = [11, 3, 6, 10, 6, 3, 11, 0]
	CPhrase = transpose(GPhrase,'G','C')
	print('Expected: [4, 8, 11, 3, 11, 8, 4, 0], Got: ' + str(CPhrase))
	GPhraseBack = transpose(CPhrase,'C','G')
	print('Expected: [11, 3, 6, 10, 6, 3, 11, 0], Got: ' + str(GPhraseBack))

	# test the get pitch ket value function
	segment = [0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0]
	keyVals = []
	for i in range(1,25):
		keyVals.append(getPitchKeyValue(segment,i))
	print('Expected: [8.0, 9.0, 8.0, 14.0, 8.0, 9.0, 10.0, 9.0, 12.0, 5.5, 12.5, 10.5, 10.5, 7.5, 9.5, 11.5, 10.5, 9.0, 6.0, 13.0, 12.0, 5.5, 11.0, 9.5],\n Got: ' + str(keyVals))
	print('Expected Key: C, Got: ' + valToKey[max(range(len(keyVals)), key=lambda x: keyVals[x]) + 1])

	# test the main functionality
	modulationPenalty = 6 # start off like this, 6 is one value used within the paper
	segmentLength = 2.0 # 2 seconds approximately what was used within the paper
	sequence = [(4,0.5),(8,0.5),(11,0.5),(3,0.5),(11,0.5),(11,0.5),(11,0.5),(8,0.5),(5,0.5),(10,0.5),(8,1),(4,0.5),(3,0.5),(8,0.5),(11,0.5)]
	keySequence = detectKey(sequence,segmentLength,modulationPenalty)
	seqAsNotes = []
	for item in keySequence:
		seqAsNotes.append(valToKey[item])

	print('Expected: [C, C, E, C], Got: ' + str(seqAsNotes))


if __name__ == '__main__':
	runTests()