import numpy as np 
import scipy.io.wavfile as wav 
import matplotlib.pyplot as plt 
import scipy.signal as sig 
import sys

#Author: Charlie Street

# function for computing stft and then outputting the spectrogram
# input_path is the wav file to read from
# segment_size is essentially the fft size
# overlap is the size of the overlap of the windows/segments
# low_cut_off is the frequency at which to cut off the low-end
# threshold is the fraction of the max psd to take (high threshold)
# low_threshold is for hysteresis thresholding 
# output_file is the output file name
def showSpectrogram(input_path, segment_size, overlap, low_cut_off, threshold, low_threshold, output_file):
	rate, data = wav.read(input_path)

	data = data.T # convert to format I'm comfortable with
	data = data.astype(float)
	data = (data[0] + data[1]) / 2.0 #average the data into mono

	f, t, Zxx = sig.stft(data,fs=rate,window='hann',nperseg=segment_size,noverlap=overlap)

	# do filtering here!!!
	bin_size = rate/segment_size
	index = round(low_cut_off/bin_size)

	for i in range(0,index):
		for j in range(0,Zxx.shape[1]):
			Zxx[i,j] = 0 + 0j

	ZxxMag = np.abs(Zxx) # get absolute values in order to form mask
	highBM = ZxxMag > threshold * (np.max(ZxxMag))

	lowBM = ZxxMag > low_threshold * (np.max(ZxxMag))

	cols = lowBM.shape[1]
	rows = lowBM.shape[0]
	#apply hysteresis thresholding
	for j in range(0,cols):
		for i in range(0,rows):
			if lowBM[i,j] and not highBM[i,j]:
				if (i-1 >= 0 and j-1 >= 0 and highBM[i-1,j-1]):
					highBM[i,j] = True
				elif (i-1 >= 0 and highBM[i-1,j]):
					highBM[i,j] = True
				elif (i-1 >= 0 and j+1 < cols and highBM[i-1,j+1]):
					highBM[i,j] = True
				elif (j-1 >= 0 and highBM[i,j-1]):
					highBM[i,j] = True
				elif (j+1 < cols and highBM[i,j+1]):
					highBM[i,j] = True
				elif (i+1 < rows and j-1 >= 0 and highBM[i+1,j-1]):
					highBM[i,j] = True
				elif (i+1 < rows and highBM[i+1,j]):
					highBM[i,j] = True
				elif (i+1 < rows and j+1 < cols and highBM[i+1,j+1]):
					highBM[i,j] = True


	Zxx = np.multiply(highBM,Zxx)

	#TODO: Add a lo-pass filter here!

	
	'''plt.pcolormesh(t, f, np.abs(Zxx))
	plt.title('STFT Magnitude')
	plt.ylabel('Frequency [Hz]')
	plt.xlabel('Time [sec]')
	plt.show()'''
	_,dataBack = sig.istft(Zxx,fs=rate,window='hann',nperseg=segment_size,noverlap=overlap)
	dataBack /= np.max(dataBack) #put into range [-1, 1]

	wav.write(output_file,rate,dataBack) # reconstruction works fine


if __name__ == '__main__':
	
	if len(sys.argv) != 8:
		print('Incorrect number of input arguments')
	else:
		input_path = sys.argv[1]
		segment_size = int(sys.argv[2])
		overlap = int(sys.argv[3])
		low_cut_off = int(sys.argv[4])
		threshold = float(sys.argv[5])
		low_threshold = float(sys.argv[6])
		output_file = sys.argv[7]
		showSpectrogram(input_path,segment_size,overlap,low_cut_off,threshold,low_threshold,output_file)