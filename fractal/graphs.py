# file is used to plot some 3d graphs
# for the dissertation
# Author: Charlie Street

import numpy as np 
import math
import matplotlib.pyplot as plt 
from mpl_toolkits.mplot3d import Axes3D
import matplotlib

matplotlib.rcParams.update({'font.size': 15})

fig = plt.figure()
ax1 = fig.add_subplot(121,projection='3d')
ax2 = fig.add_subplot(122,projection='3d')


N = np.loadtxt('matrices/N_allC.txt')

xpos = []
ypos = []

print(N.shape[0])
print(N.shape[1])
for i in range(N.shape[0]*N.shape[1]):
	xpos.append(i % N.shape[1])
	ypos.append(i % N.shape[0])

zpos = np.zeros(N.shape[0] * N.shape[1])


dx = np.ones(N.shape[0] * N.shape[1])
dy = np.ones(N.shape[0] * N.shape[1])

dz = []

for i in range(len(xpos)):
	dz.append(N[ypos[i],xpos[i]])

colorChoices = ['r','b','g','#500ab2','y','#0aacb2']
colours = []
for i in range(N.shape[0] * N.shape[1]):
	colours.append(colorChoices[i%N.shape[0]])

ax1.bar3d(xpos,ypos,zpos,dx,dy,dz,color=colours)
ax1.set_title('N matrix with temperature T = 1')
ax1.set_xticks(np.arange(0.5,13,1))
ax1.set_xticklabels(xpos)
ax1.set_yticks(np.arange(0.5,6,1))
ax1.set_yticklabels(ypos)
ax1.set_xlabel('Musical Note')
ax1.set_ylabel('Cluster')
ax1.view_init(elev=15,azim=248)
ax1.set_zlabel('Tilted Frequency of Occurence',labelpad=15)


newdz = []
for i in range(len(zpos)):
	newdz.append(math.pow(dz[i],1.0/0.4))
ax2.view_init(elev=15,azim=248)
ax2.bar3d(xpos,ypos,zpos,dx,dy,newdz,color=colours)

ax2.set_xticks(np.arange(0.5,13,1))
ax2.set_xticklabels(xpos)
ax2.set_yticks(np.arange(0.5,6,1))
ax2.set_yticklabels(ypos)

ax2.set_title('N matrix with temperature T = 0.4')
ax2.set_xlabel('Musical Note')
ax2.set_ylabel('Cluster')
ax2.set_zlabel('Tilted Frequency of Occurence')

plt.show() # remember to set to tight layout once graph open!
