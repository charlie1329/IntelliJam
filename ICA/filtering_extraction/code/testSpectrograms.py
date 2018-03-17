import numpy as np 
import scipy.io.wavfile as wav 
import matplotlib.pyplot as plt 
import scipy.signal as sig 
import sys


#Author: Charlie Street

# function for computing stft and then outputting the spectrogram
# input_path is the wav file to read from (the test file)
# ground_truth is the original guitar stem
def compareSpectrograms(input_path, ground_truth, segment_size, overlap):
	
	print('Starting...')

	# get the test spectrogram

	rate, data = wav.read(input_path)

	data = data.astype(float)
	if(len(data.shape) > 1 and data.shape[1] == 2):
		data = data.T
		data = (data[0] + data[1])/2.0

	f, t, Zxx = sig.stft(data,fs=rate,window='hann',nperseg=segment_size,noverlap=overlap)

	testMat = np.abs(Zxx)

	# get the ground truth spectrogram

	gtRate, gtData = wav.read(ground_truth)

	gtData = gtData.astype(float)

	gtF, gtT, gtZxx = sig.stft(gtData,fs=gtRate,window='hann',nperseg=segment_size,noverlap=overlap)
	
	gtMat = np.abs(gtZxx)

	# compare the two matrices

	totalElements = gtMat.shape[0] * gtMat.shape[1] #used for average later

	lms = 0.0

	#get the index for the low cut off and start from there
	bin_size = rate/segment_size
	index = round(80/bin_size)
	index2 = round(2000/bin_size)

	print('Starting MSE calculation...')

	print(gtMat.shape)
	print(testMat.shape)

	TP = 0
	TN = 0
	FP = 0
	FN = 0

	#threshold = 0.05

	for i in range(0,index):
		for j in range(0,testMat.shape[1]):
			testMat[i,j] = 0

	for i in range(0,index):
		for j in range(0,gtMat.shape[1]):
			gtMat[i,j] = 0

	for i in range(index2,testMat.shape[0]):
		for j in range(0,testMat.shape[1]):
			testMat[i,j] = 0

	for i in range(index2,gtMat.shape[0]):
		for j in range(0,gtMat.shape[1]):
			gtMat[i,j] = 0

	testMat = (testMat - np.mean(testMat)) / np.std(testMat)
	gtMat = (gtMat - np.mean(gtMat)) / np.std(gtMat)

	
	print("Here")

	for i in range(index,index2):
		print("New loop: " + str(i))
		for j in range(0,min(testMat.shape[1],gtMat.shape[1])):
			lms += pow(gtMat[i,j]-testMat[i,j],2.0)

	lms /= ((index2-index) * min(testMat.shape[1],gtMat.shape[1])) # take average

	print("The lms error is: " + str(lms))



if __name__ == '__main__':
	
	if len(sys.argv) != 5:
		print('Incorrect number of input arguments')
	else:
		input_path = sys.argv[1]
		ground_truth = sys.argv[2]
		segment_size = int(sys.argv[3])
		overlap = int(sys.argv[4])
		compareSpectrograms(input_path,ground_truth,segment_size,overlap)