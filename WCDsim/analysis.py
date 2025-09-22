import csv
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator
import math
import numpy as np
import scipy.constants as constant



option1 = '8-inchPMT'
option2 = '3-inchPMT+WLS'



path = 'data/'
file = '/data.csv'


output1 = path + option1 + file
output2 = path + option2 + file





outputs = [output1,output2]

fEvent1 = []
fEvent2 = []

fX = []
fY =[]
fZ =[]
fmomX = []
fmomY =[]
fmomZ =[]
fwlen1=[]
fwlen2=[]
fT1=[]
fT2=[]
fmomMag=[]
wav =[]
fE1 = []
fE2 = []
Xmom = np.arange(0,-16.5,-0.5)
theta1 = []
theta2 = []




for i in outputs:

    with open(i) as file:
        csvreader = csv.reader(file)

        for row in csvreader:
            if ( (i == output1)):
                if float(row[7]) > 0:
                    fEvent1.append(float(row[0]))
                    wav = float(row[7])
                    fwlen1.append(wav)
                    fT1.append(float(row[8]))
                    fX.append(float(row[1]))
                    fY.append(float(row[2]))
                    fZ.append(float(row[3]))
                    fmomX = float(row[4])
                    fmomY = float(row[5])
                    fmomZ = float(row[6])
                    mag = math.sqrt(np.square(fmomX) + np.square(fmomY) + np.square(fmomZ))
                    fmomMag.append(mag)
                    theta1.append(Xmom[int(row[9])])
                    E = ((constant.h * constant.c)/(wav*10e-9))/(constant.e)
                    fE1.append(E)
            if ( (i == output2)):
                if float(row[7]) > 0:
                    fEvent2.append(float(row[0]))
                    wav = float(row[7])
                    fwlen2.append(wav)
                    theta2.append(Xmom[int(row[9])])
                    E = ((constant.h * constant.c)/(wav*10e-9))/(constant.e)
                    fE1.append(E)



'''
y1, binEdges1 = np.histogram(fEvent1, bins=20)
y2, binEdges2 = np.histogram(fEvent2, bins=20)


plt.subplot()

bincenters1 = 0.5 * (binEdges1[1:] + binEdges1[:-1])
plt.plot(bincenters1, y1, '-', c='red', alpha = 0.2)
plt.axhline(y1.mean(), color='red', linestyle='dashed', linewidth=2, label=option1)

bincenters2 = 0.5 * (binEdges2[1:] + binEdges2[:-1])
plt.plot(bincenters2, y2, '-', c='green', alpha = 0.2)
plt.axhline(y2.mean(), color='green', linestyle='dashed', linewidth=2, label=option2)

plt.title("Number of photons for each event")# (" + option1 + " vs " + option2 + " )")
plt.xlabel("Event(s)")
plt.ylabel("Frequency of photons")
plt.legend(loc='upper left')

plt.grid()

plt.show()


fZ = np.array(fZ)
my_cmap = plt.get_cmap('hot')
fig = plt.figure()
ax = plt.axes(projection ='3d')
surf = ax.scatter3D(fX, fY, fZ,cmap = my_cmap,edgecolor ='none')
'''

plt.hist(fmomMag, bins=50)
plt.title("Photon momentum magnitude ("+ option1 + ")")
plt.xlabel("Momentum (eV)")
plt.ylabel("Frequeny")
plt.grid()
plt.show()


plt.hist(fwlen1, bins=50, label=option1)
plt.hist(fwlen2, bins=50, label=option2)
plt.title("Photon wavelength (" + option1 + " vs " + option2 + ")")
plt.xlabel("Wavelength (nm)")
plt.ylabel("Frequency")
plt.legend(loc='upper right')
plt.grid()
plt.show()

num1 = []
num2 = []


for mom in Xmom:
    num1.append(theta1.count(mom))
    num2.append(theta2.count(mom))

trap_eff1 = []
trap_eff2 = []

for i in num1:
    trap_eff1.append(i/1000)

for i in num2:
    trap_eff2.append(i/1000)


plt.plot(Xmom,trap_eff1, label=option1)
plt.plot(Xmom,trap_eff2,label=option2)
plt.title("Detection efficiency for different entry angles")
plt.xlabel("X Momentum Direction")
plt.ylabel("Detection efficiency")
plt.legend(loc= 'upper right')
plt.grid()
plt.show()




option1 = ''



o1 = '/' + option1




path = 'data/3-inchPMT+WLS'
file = 'data.csv'


output1 = path + o1 + file




outputs = [output1]

fEvent1 = []

fX = []
fY =[]
fZ =[]
fmomX = []
fmomY =[]
fmomZ =[]
fwlen1=[]
fT1=[]
fmomMag=[]
wav =[]
fE1 = []
Xmom = np.arange(0,-16.5,-0.5)
theta = []



for i in outputs:

    with open(i) as file:
        csvreader = csv.reader(file)

        for row in csvreader:
            if ( (i == output1)):
                if float(row[7]) > 0:
                    fEvent1.append(float(row[0]))
                    wav = float(row[7])
                    fwlen1.append(wav)
                    fT1.append(float(row[8]))
                    fX.append(float(row[1]))
                    fY.append(float(row[2]))
                    fZ.append(float(row[3]))
                    fmomX = float(row[4])
                    fmomY = float(row[5])
                    fmomZ = float(row[6])
                    theta.append(Xmom[int(row[9])])
                    mag = math.sqrt(np.square(fmomX) + np.square(fmomY) + np.square(fmomZ))
                    fmomMag.append(mag)
                    E = ((constant.h * constant.c)/(wav*10e-9))/(constant.e)
                    fE1.append(E)



plt.hist(fE1, bins = 100)
plt.title("Frequency of photons detected VS photon energy")
plt.xlabel("Photon energy (MeV)")
plt.ylabel("Frequency")
plt.grid()
plt.show()


plt.hist(fwlen1, bins = 100)
plt.title("Frequency of wavelengths detected")
plt.xlabel("Photon wavelengths (nm)")
plt.ylabel("Frequency")
plt.grid()
plt.show()

plt.hist(fT1, bins = 100)
plt.title("Frequency of photon arrival times")
plt.xlabel("Time (ns)")
plt.ylabel("Frequency")
plt.grid()
plt.show()

num = []


for mom in Xmom:
    num.append(theta.count(mom))

plt.plot(Xmom,num)
plt.title("Varying photon entry angle into WLS plate")
plt.xlabel("X Momentum Direction")
plt.ylabel("Detection frequency by PMT")
plt.grid()
plt.show()

trap_eff = []

for i in num:
    trap_eff.append(i/1000)


plt.plot(Xmom,trap_eff)
plt.title("Detection efficiency for different entry angles")
plt.xlabel("X Momentum Direction")
plt.ylabel("Detection efficiency")
plt.grid()
plt.show()
