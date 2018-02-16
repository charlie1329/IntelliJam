# File implements an LSTM in Keras
# This is done in order to test whether it will work in the full system
# Author: Charlie Street

# Carry out all the imports
import csv
import numpy as np
from keras.models import Sequential
from keras.models import load_model
from keras.layers import Dense
from keras.layers import Dropout
from keras.layers import LSTM
from keras.layers import TimeDistributed
from keras.layers.embeddings import Embedding 
from keras.preprocessing import sequence

# fix random seed for reproducibility
np.random.seed(7)

vector_length = 57 # 56 notes + 1 for silence

# load the data set in here
X_train = [] # stored as a list for now
X_test = []
y_train = []
y_test = []


print('Starting to read in the training set')
# read in the raw values from the data set
currentList = []
with open('D:/trainingPairs.csv', 'r') as csvfile:
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

# pad the input sequences
length = 150
#X_train = sequence.pad_sequences(X_train,maxlen=length) 
#y_train = sequence.pad_sequences(y_train,maxlen=length)

print('Converting to one hot vectors')
'''X_train_hot = []
for sample in X_train:
	hot_sample = []
	for value in sample:
		new_value = np.zeros(vector_length)
		if (value == 0):
			new_value[0] = 1
		else:
			new_value[value-23] = 1
		hot_sample.append(new_value)
	X_train_hot.append(np.asarray(hot_sample))

X_train = X_train_hot #np.asarray(X_train_hot)'''

X_train_comp = []
for sample in X_train:
	comp_sample = []
	for value in sample:
		if (value == 0):
			comp_sample.append([0])
		else:
			comp_sample.append([float(value-23)/float(vector_length)])
	X_train_comp.append(np.asarray(comp_sample))
X_train = X_train_comp
#print('X_train shape: ' + str(X_train.shape))
print('X_train shape: ' + str(len(X_train)))

y_train_hot = []
for sample in y_train:
	hot_sample = []
	for value in sample:
		new_value = np.zeros(vector_length)
		if (value == 0):
			new_value[0] = 1
		else:
			new_value[value-23] = 1
		hot_sample.append(new_value)
	y_train_hot.append(np.asarray(hot_sample))

y_train = y_train_hot#np.asarray(y_train_hot)
#print('y_train shape: ' + str(y_train.shape))
print('y_train size: ' + str(len(y_train)))

print('Slicing up into training and test sets')
num_train_samples = int(len(y_train) * 9/10)
num_test_samples = int(len(y_train) / 10)

print('Number of Training Samples: ' + str(num_train_samples))
print('Number of Test Samples: ' + str(num_test_samples))

X_test = X_train[int(num_train_samples):]
y_test = y_train[int(num_train_samples):]

X_train = X_train[0:int(num_train_samples)]
y_train = y_train[0:int(num_train_samples)]

if(len(X_train) + len(X_test) != 8200):
	print('Error in slicing x!')
if(len(y_train) + len(y_test) != 8200):
	print('Error in slicing y!')

print('About to set up model')
hidden_neurons = 100
n_batch = 1
n_epoch = 1

total_epochs = 50

print(X_train[0].shape)
print(y_train[0].shape)

print('Quick test:')
print(len(X_train[0]))
print(X_train[0][0])
print(y_train[0][0])

model = Sequential()
model.add(LSTM(hidden_neurons, batch_input_shape=(n_batch,1,1),return_sequences=True, stateful=True))
model.add(Dropout(0.3))
model.add(LSTM(hidden_neurons*2,return_sequences=True))
model.add(Dropout(0.3))
model.add(LSTM(hidden_neurons))
model.add(Dense(256))
model.add(Dropout(0.3))
model.add(Dense(vector_length, activation='softmax'))
model.compile(loss='categorical_crossentropy', optimizer='rmsprop', metrics=['accuracy'])

# do epochs manually
for i in range(total_epochs):
	print('Epoch: ' + str(i))
	mean_tr_acc = []
	mean_tr_loss = []
	for j in range(len(X_train)): # go through each sample
		print('Training item: ' + str(j))
		for k in range(len(X_train[j])):
			currentX = np.array([[X_train[j][k]]])
			currentY = np.array([y_train[j][k]])
		
			tr_loss, tr_acc = model.train_on_batch(currentX,currentY)

			mean_tr_acc.append(tr_acc)
			mean_tr_loss.append(tr_loss)
		model.reset_states()

	model.save('D:/statefulLSTM3/statefulLSTMEpoch' + str(i) + '.h5')
	print('accuracy training = {}'.format(np.mean(mean_tr_acc)))
	print('loss training = {}'.format(np.mean(mean_tr_loss)))
	print('______________________________________')

	mean_te_acc = []
	mean_te_loss = []
	for j in range(len(X_test)):
		print('Testing item: ' + str(j))
		for k in range(len(X_test[j])):
			currentX = np.array([[X_test[j][k]]])
			currentY = np.array([y_test[j][k]])
			te_loss, te_acc = model.test_on_batch(currentX,currentY)
			mean_te_acc.append(te_acc)
			mean_te_loss.append(te_loss)
		model.reset_states()

	print('accuracy testing = {}'.format(np.mean(mean_te_acc)))
	print('loss testing = {}'.format(np.mean(mean_te_loss)))
	print('__________________________________________')

print('Finished. Just saving model')
model.save('D:/statefulLSTM3/statefulLSTM.h5')



#Previous attempt at training
'''
# reshape everything
X_train = X_train.reshape(num_train_samples, length, vector_length)
X_test = X_test.reshape(num_test_samples, length, vector_length)
y_train = y_train.reshape(num_train_samples, length, vector_length)
y_test = y_test.reshape(num_test_samples, length, vector_length)

print('About to set up model')
# set up the model
hidden_neurons = 100
n_batch = 1
n_epoch = 50

model = Sequential()
model.add(LSTM(hidden_neurons, input_shape=(length, vector_length), return_sequences=True))
model.add(TimeDistributed(Dense(vector_length,activation='softmax')))
model.compile(loss='categorical_crossentropy', optimizer='adam',metrics=['accuracy'])
print(model.summary())

#train the model
print('Starting Training')
model.fit(X_train, y_train, epochs=n_epoch, batch_size=n_batch, verbose=2)

# final evaluation of the model
print('Evaluating Model')
scores = model.evaluate(X_test, y_test, verbose=0)
print("Accuracy: %.2f%%" % (scores[1]*100))

# save the model for future use
print('Saving Model')
model.save('D:/my_lstm.h5')

#can use model.predict to predict stuff
#ill feed in an input phrase, and then get it to keep going based off its
#own predictions

print('Finished')
'''