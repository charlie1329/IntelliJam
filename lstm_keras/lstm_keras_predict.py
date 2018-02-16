# file tries to take model learnt in keras
# and predict with it to see the output...
# Author: Charlie Street

# Carry out all the imports
import csv
import numpy as np
from keras.models import Sequential
from keras.models import load_model
from keras.layers import Dense
from keras.layers import LSTM
from keras.layers import TimeDistributed
from keras.layers.embeddings import Embedding 
from keras.preprocessing import sequence
import keras.backend as K

def get_internal_state(model):
	h, c = [K.get_value(s) for s, _ in model.state_updates]
	return h,c

vector_length = 57

# first load the model back in 
model = load_model('D:/statefulLSTM3/statefulLSTMEpoch3.h5')

print('Finished loading model')

# check the internal state before I do anything
#print(get_internal_state(model))

note_sequence = [46,48,51,53,55,53,51,48,51]

note_sequence_hot = []
'''
for item in note_sequence:
	new_sample = np.zeros(57)
	new_sample[item-23] = 1
	note_sequence_hot.append(new_sample)

note_sequence = np.asarray(note_sequence_hot)

print(note_sequence.shape)
'''

for item in note_sequence:
	note_sequence_hot.append(float(item-23)/float(vector_length))

note_sequence = note_sequence_hot
print(note_sequence)

# now try and actually predict some stuff 

model.reset_states()
print('Starting prediction')

for sample in note_sequence:
	#print(sample.shape)
	#print('STATE:')
	#print(get_internal_state(model)) # i want this to be changing
	print('SAMPLE: ' + str(sample))
	prediction = model.predict(np.asarray([[[sample]]]))
	print('PREDICTION:')
	#print(prediction)
	print(np.argmax(prediction) + 23)
	print('STATES:')
	print(get_internal_state(model))


notes_played = []
notes_played.append(np.argmax(prediction)+23)

print('NOW GETTING FOLLOW-UP PREDICTIONS')

new_prediction = float(np.argmax(prediction))/float(vector_length)

for i in range(8):
	#print('STATE:')
	#print(get_internal_state(model))
	new_prediction = model.predict(np.asarray([[[new_prediction]]]))
	new_prediction = np.argmax(new_prediction) + 23
	notes_played.append(new_prediction)
	new_prediction = float(new_prediction - 23)/float(vector_length)
	print('GENERATED PREDICTION:')
	print(new_prediction)

print('Printing output in note form:')

for output in notes_played:
	print('Note selected: ' + str(output))

print('Finished')