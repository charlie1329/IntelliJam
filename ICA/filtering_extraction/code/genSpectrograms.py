import numpy as np 
import scipy.io.wavfile as wav 
import matplotlib.pyplot as plt 
import scipy.signal as sig 

# Author: Charlie Street

preRate, preData = wav.read('../instrument_samples/music_samples/vai_short.wav')
preData = preData.astype(float)

postRate, postData = wav.read('vaiHystPoint3LowPoint1.wav')
postData = postData.T
postData = postData.astype(float)
postData = (postData[0] + postData[1]) / 2.0

segment_size = 8192
overlap = 7680
print(segment_size > overlap)
preF, preT, preZxx = sig.stft(preData,fs=preRate,window='hann',nperseg=segment_size,noverlap=overlap)

postF, postT, postZxx = sig.stft(postData,fs=postRate,window='hann',nperseg=segment_size,noverlap=overlap)

ax1 = plt.subplot(2,1,1)
p1 = ax1.pcolormesh(preT,preF,np.abs(preZxx))
ax1.set_title('STFT Magnitude Before Filtering')
ax1.set_ylabel('Frequency [Hz]')
ax1.set_xlabel('Time [sec]')
ax1.axis([1.0,2.0,0,800])

ax2 = plt.subplot(2,1,2)
p2 = ax2.pcolormesh(postT,postF,np.abs(postZxx))
ax2.set_title('STFT Magnitude After Filtering')
ax2.set_ylabel('Frequency [Hz]')
ax2.set_xlabel('Time [sec]')
ax2.axis([1.0,2.0,0,800])

plt.show()