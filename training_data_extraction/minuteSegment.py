import os
import numpy as np
import scipy.io.wavfile as wav
import sys

# Author: Charlie Street

# This file has to be run under linux to use fork

# This function goes through all guitar solos
# And makes sure none of them are more than a minute long
def splitByMinute():
	maxLength = 44100 * 60

	inSoloMax = 1434
	outSolo = 1
	
	inSoloPrefix = '/mnt/d/solos/solo'
	outSoloPrefix = '/mnt/d/segmented/segment'
	suffix = '.wav'

	for solo in range(1,inSoloMax):
		outSolo = len(os.listdir('/mnt/d/segmented')) + 1
		fileName = inSoloPrefix + str(solo) + suffix
		
		childpid = os.fork()

		if childpid == 0: #child process
			rate, data = wav.read(fileName)
			if rate != 44100:
				print('Error: wrong sample rate, found sample rate of: ' + str(rate))
			else:
				finished = False
				startPoint = 0
				sizeOfFile = data.shape[0] # everything is assumed mono
				while not finished:
				
					if startPoint + maxLength > sizeOfFile:
						finished = True
						wav.write(outSoloPrefix + str(outSolo) + suffix, rate, data[startPoint:sizeOfFile])
					else:
						wav.write(outSoloPrefix + str(outSolo) + suffix, rate, data[startPoint:(startPoint+maxLength)])
						startPoint += maxLength
					print(outSoloPrefix + str(outSolo) + suffix)
					outSolo += 1

			os._exit(0)
		else:
			os.waitpid(childpid, 0)
			print('Finished splitting solo: ' + str(solo))

	print('Finished segmenting into 1 minute or less')


if __name__ == '__main__':
	print('Started')
	splitByMinute()
