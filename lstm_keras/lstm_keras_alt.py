# File takes an alternative approach to training the network
# Author: Charlie Street

# Carry out all the imports
import csv
import numpy as np
from keras.models import Sequential
from keras.models import load_model
from keras.layers import Dense
from keras.layers import LSTM
from keras.layers import Dropout
from keras.callbacks import ModelCheckpoint
from keras.layers import TimeDistributed
from keras.layers.embeddings import Embedding 
from keras.preprocessing import sequence
import keras.backend as K

to_subtract = 22
vector_length = 14
hidden_neurons = 200
n_epochs = 75

def generate_notes(model, length, user_notes):
	input_notes = []
	for item in user_notes:
		input_notes.append(float(item % 12 + 2)/float(vector_length))

	output_notes = []

	X = np.zeros((1,length + len(input_notes), 1))
	for i in range(len(input_notes)):
		X[0, i, :] = np.asarray([input_notes[i]])

	#actually do some predicting
	for i in range(length):
		full_pred = model.predict(X[:,:len(input_notes)+i,:])[0][len(input_notes)+i-1]
		print(full_pred.shape)
		pred = np.argmax(full_pred)
		print(pred - 2)
		output_notes.append(pred - 2)
		X[0, i + len(input_notes), :] = np.asarray([float(pred)/float(vector_length)])

	for i in range(len(output_notes)):
		print('Note ' + str(i) + ': ' + str(output_notes[i]))


#predicts from the lstm model
def predictFromModel(filePath, length, user_notes):
	model = load_model(filePath)
	generate_notes(model,length,user_notes)

#prepare the input data for use
def prepareData(filePath):
	# load the data set in here
	X_train = [] # stored as a list for now
	X_test = []
	y_train = []
	y_test = []


	print('Starting to read in the training set')
	# read in the raw values from the data set
	currentList = []
	with open(filePath, 'r') as csvfile:
		pairReader = csv.reader(csvfile)
		for row in pairReader:
			colNo = 0
			for col in row:
				if colNo == 0:
					if(row[colNo][0] == '*'): # new sample
						if(currentList != []):
							X_train.append(currentList)
							currentList = []
					else:
						currentList.append(int(row[colNo]))
				colNo += 1
		X_train.append(currentList)

	print(len(X_train))

	X_train_last_off = []
	for sample in X_train:
		y_train.append(sample[1:len(sample)])
		X_train_last_off.append(sample[0:len(sample)-1])

	X_train = X_train_last_off

	# do a little bit of error checking here
	for i in range(0,len(X_train)):
		if(len(X_train[i]) != len(y_train[i])):
			print("ERROR IN FORMING Y_TRAIN")

	#set all zeroes to 1
	for i in range(len(X_train)):
		for j in range(len(X_train[i])):
			if(X_train[i][j] == 0):
				X_train[i][j] = 1
			else:
				X_train[i][j] = (X_train[i][j] + 1)
			if(y_train[i][j] == 0):
				y_train[i][j] = 1
			else:
				y_train[i][j] = y_train[i][j] + 1



	max_length = 20
	X_train = sequence.pad_sequences(X_train,maxlen=max_length) 
	y_train = sequence.pad_sequences(y_train,maxlen=max_length)


	# now deal with making this stuff the right size and format
	X_train_formatted = []
	for i in range(len(X_train)):
		current_sample = []
		for j in range(max_length):
			currentVal = X_train[i][j]
			'''if(X_train[i][j] <= 1):
				currentVal = X_train[i][j]
			else:
				currentVal = X_train[i][j] - to_subtract'''
			current_sample.append([float(currentVal)/float(vector_length)])
		X_train_formatted.append(current_sample)

	X_train = np.asarray(X_train_formatted)
	print('X_train shape: ' + str(X_train.shape))

	y_train_hot = []
	for i in range(len(y_train)):
		current_sample = []
		for j in range(max_length):
			current_hot = np.zeros(vector_length)
			index = y_train[i][j]
			'''if(y_train[i][j] <= 1):
				index = y_train[i][j]
			else:
				index = y_train[i][j] - to_subtract'''
			current_hot[index] = 1
			current_sample.append(current_hot)
		y_train_hot.append(current_sample)

	y_train = np.asarray(y_train_hot)
	print('y_train.shape: ' + str(y_train.shape))

	print('Slicing up into training and test sets')
	num_train_samples = int(y_train.shape[0] * 9/10)
	num_test_samples = int(y_train.shape[0] / 10)

	print('Number of Training Samples: ' + str(num_train_samples))
	print('Number of Test Samples: ' + str(num_test_samples))

	X_test = X_train[int(num_train_samples):]
	y_test = y_train[int(num_train_samples):]

	X_train = X_train[0:int(num_train_samples)]
	y_train = y_train[0:int(num_train_samples)]

	if(X_train.shape[0] + X_test.shape[0] != 8247):
		print('Error in slicing x!')
	if(y_train.shape[0] + y_test.shape[0] != 8247):
		print('Error in slicing y!')
	return X_train, y_train, X_test, y_test

#sets up the network and trains it
def createAndTrainModel(X_train,y_train,X_test,y_test):
	model = Sequential()
	model.add(LSTM(hidden_neurons, input_shape=(None,1), return_sequences=True))
	model.add(Dropout(0.3))
	#model.add(LSTM(hidden_neurons, return_sequences=True))
	#model.add(LSTM(hidden_neurons, return_sequences=True))
	model.add(TimeDistributed(Dense(vector_length, activation='softmax')))
	model.compile(loss='categorical_crossentropy', optimizer='rmsprop', metrics=['accuracy'])

	'''filepath='D:/lstm_text/weights-improvement-{epoch:02d}-{val_acc:.2f}.hdf5'
	checkpoint = ModelCheckpoint(filepath, monitor='val_acc', verbose=1, save_best_only=True, mode='max')
	callbacks_list = [checkpoint]'''


	print('Training model')
	model.fit(X_train,y_train,batch_size=1,verbose=2,epochs=n_epochs)
	model.save('D:/lstm_one_octave_150.h5')

	print('Evaluating Model')
	scores = model.evaluate(X_test, y_test, verbose=0)
	print("Accuracy: %.2f%%" % (scores[1]*100))

if __name__ == "__main__":
	#X_train, y_train, X_test, y_test = prepareData('D:/trainingPairsOneOctave.csv')
	#createAndTrainModel(X_train,y_train,X_test,y_test)
	print('Predicting')
	#predictFromModel('D:/lstm_one_octave.h5',8,[10,0,3,5,7,5,3,0,3])
	#predictFromModel('D:/lstm_one_octave.h5',20,[3,6,10,1,3,1,6,8,6])
	predictFromModel('D:/lstm_one_octave.h5',20,[5,7,10,0,5,7,10,0,2,0,10,7,10,0,3,5,8,5,7,5])