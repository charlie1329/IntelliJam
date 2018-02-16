# File will take an existing set of training pairs
# And will convert it down to one octave, 
# Merging consecutive identical notes together
# Though this clearly has one problem with it
# It should simplify everything and give it more chance of working
# Author: Charlie Street

import csv

# function does the processing for us
# reads from input file and converts into 
# a modified version working within one octave
def toOneOctave(inputPath,outputPath):

	with open(inputPath, 'r') as inputFile:
		with open(outputPath, 'w') as outputFile:
			inReader = csv.reader(inputFile)
			outWriter = csv.writer(outputFile)

			# loop through the input file
			count = 1
			currentNote = -1
			currentDuration = 0
			for row in inReader:
				if (len(row) == 1):
					if(currentNote != -1):
						outputFile.write(str(currentNote) + ',' + str(currentDuration) + ',\n')
					print('Sample: ' + str(count))
					count += 1
					outputFile.write(row[0] + '\n')
					currentNote = -1
					currentDuration = 0
				else:
					if(int(row[0]) == 0):
						newNote = 0
					else:
						newNote = (int(row[0]) % 12) + 1# get the raw note
					newDuration = float(row[1])
					if (currentNote == -1): # if first of a new sample
						currentNote = newNote
						currentDuration += newDuration
					elif (currentNote == newNote): #keep going
						currentDuration += newDuration
					else: # if new note 
						outputFile.write(str(currentNote) + ',' + str(currentDuration) + ',\n')
						currentNote = newNote
						currentDuration = newDuration
			outputFile.write(str(currentNote) + ',' + str(currentDuration) + ',\n')


if __name__ == '__main__':
	#toOneOctave('test/test_pairs.csv','test/test_pairs_one_octave.csv')
	toOneOctave('D:/trainingPairsSoftBound.csv','D:/trainingPairsOneOctave.csv')