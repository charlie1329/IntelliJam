# file converts all fpm matrices out into the format required by the c++ application
# Author: Charlie Street

import csv
import numpy as np

def writeMatrix(mat,outFile):
	print('Writing to: ' + outFile)
	print('Shape: ' + str(mat.shape))

	with open(outFile, 'w') as csvFile:
		for i in range(mat.shape[0]):
			for j in range(mat.shape[1]):
				csvFile.write(str(mat[i,j]) + ',')
			csvFile.write('\n')


if __name__ == '__main__':

	BNote = np.loadtxt('matrices/B_allC.txt')
	NNote = np.loadtxt('matrices/N_allC.txt')
	tNote = np.loadtxt('matrices/t_allC.txt')
	print(BNote.T)

	BDir = np.loadtxt('matrices/B_dir.txt')
	BDir = np.reshape(BDir,(5,1)) # careful here!
	NDir = np.loadtxt('matrices/N_dir.txt')
	tDir = np.loadtxt('matrices/t_dir.txt')
	tDir = np.reshape(tDir,(2,1))

	writeMatrix(BNote.T,'matrix_csv/BNote.csv')
	writeMatrix(NNote,'matrix_csv/NNote.csv')
	writeMatrix(tNote.T,'matrix_csv/tNote.csv')
	writeMatrix(BDir.T,'matrix_csv/BDir.csv')
	writeMatrix(NDir,'matrix_csv/NDir.csv')
	writeMatrix(tDir.T,'matrix_csv/tDir.csv')