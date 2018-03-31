import numpy as np
import matplotlib.pyplot as plt
import matplotlib

matplotlib.rcParams.update({'font.size': 15})


# all the values to plot the model graph
model_samples = np.asarray([49152,90112,139264,180224,221184,270336,311296,360448,401408,442368,491520,532480,573440,622592,663552,712704,753664,794624,843776,884736,933888,974848,1015808,1064960,1105920,1146880,1196032,1236992,1286144,1327104])
model_avg = np.asarray([1.153782,2.083612,3.195716,4.11577,5.045754,6.165648,7.095624,8.201744,9.1338448,10.06572,11.18362,12.11378,13.03384,14.1457,15.07372,16.1957,17.12378,18.05184,19.16392,20.0955,21.21186,22.14354,23.0656,24.1837,25.10556,26.04556,27.15564,28.08194,29.19562,30.12368])
model_min = np.asarray([1.14197,2.07202,3.19152,4.11156,5.04161,6.16151,7.0916,8.19189,9.122214,10.0617,11.172,12.1022,13.0224,14.1416,15.0619,16.1915,17.1215,18.042,19.1525,20.0915,21.202,22.1315,23.0611,24.1719,25.1016,26.0413,27.1516,28.072,29.1915,30.1119])
model_max = np.asarray([1.16184,2.09154,3.20186,4.12199,5.05167,6.17197,7.10163,8.21197,9.14163,10.0719,11.1917,12.1218,13.042,14.1517,15.0819,16.2018,17.132,18.0617,19.1724,20.1016,21.222,22.1516,23.0718,24.1916,25.1117,26.0519,27.1617,28.0919,29.2017,30.1319])

# all the values to plot the timer graph
timer_samples = np.asarray([44105.8,88247.6,132359.2,176425.8,220506.4,264604,308719.2,352803.4,396930.6,441049.4,485140.6,529232,573320.8,617429,661500.8,705600,749719.4,793959.4,837905.6,882012.6,926127.2,970223.2,1014324.2,1058400.6,1102584,1146619.6,1190725.2,1234809.2,1278903,1323009.6])
timer_avg = np.asarray([1.028438,2.038484,3.032226,4.026438,5.030266,6.028464,7.032084,8.03051,9.040424,10.03236,11.03848,12.03218,13.02838,14.0321,15.03036,16.02038,17.03032,18.04236,19.02638,20.03038,21.0323,22.03044,23.03034,24.03038,25.0422,26.02822,27.03836,28.02826,29.03028,30.03024])
timer_min = np.asarray([1.02074,2.03044,3.03016,4.02042,5.02069,6.02046,7.03005,8.02081,9.03052,10.0302,11.0305,12.0301,13.0204,14.03,15.0204,16.0107,17.0205,18.0401,19.0203,20.0206,21.03,22.0209,23.0206,24.021,25.0401,26.0203,27.0307,28.0203,29.0205,30.0203])
timer_max = np.asarray([1.04024,2.05048,3.04017,4.04038,5.0401,6.04029,7.04002,8.04023,9.05099,10.0402,11.0504,12.0401,13.0403,14.04,15.0403,16.0305,17.0402,18.0505,19.0307,20.0404,21.0407,22.0404,23.0401,24.0403,25.05,26.0402,27.0405,28.0402,29.0404,30.0401])

if __name__ == '__main__':

	model_assert = model_samples.shape[0] == 30 and model_avg.shape[0] == 30 and model_min.shape[0] == 30 and model_max.shape[0] == 30
	timer_assert = timer_samples.shape[0] == 30 and timer_avg.shape[0] == 30 and timer_min.shape[0] == 30 and timer_max.shape[0] == 30

	if(not (model_assert and timer_assert)):
		print('ERROR: Values copied wrong!')

	# set up the graphs
	fig = plt.figure()
	ax1 = fig.add_subplot(121)
	ax2 = fig.add_subplot(122)

	# set titles/axes
	ax1.set_title('Model Thread Speed Test')
	ax1.set_xlabel('Audio Samples Processed')
	ax1.set_ylabel('Time Elapsed (s)')
	ax1.plot(model_samples,model_avg,'bx')
	model_line = np.polyfit(model_samples,model_avg, 1)
	print('Model_line: ' + str(model_line))

	ax1.plot(np.asarray(list(range(0,1400001))), np.poly1d(model_line)(np.asarray(list(range(0,1400001)))),'r')

	ax2.set_title('Timer Thread Speed Test')
	ax2.set_xlabel('Audio Samples Processed')
	ax2.set_ylabel('Time Elapsed (s)')
	ax2.plot(timer_samples,timer_avg,'bx')
	timer_line = np.polyfit(timer_samples,timer_avg, 1)
	print('Timer_line: ' + str(timer_line))
	
	ax2.plot(np.asarray(list(range(0,1400001))), np.poly1d(timer_line)(np.asarray(list(range(0,1400001)))),'r')

	plt.show()