import csv
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
import math
import numpy as np

option1 = "WLSWithNoReflectiveSkin"
option2 = "WLSAdded2"

outputs = ['data/' + option1 + '/output0_nt_Photons_t0.csv','data/' + option1 + '/output0_nt_Photons_t1.csv','data/' + option2 + '/output0_nt_Photons_t0.csv','data/' + option2 + '/output0_nt_Photons_t1.csv']


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
fmomMag=[]
wav =[]



for i in outputs:

    with open(i) as file:
        csvreader = csv.reader(file)
        count = 0
        for row in csvreader:
            if count > 12:

                if ( (i == 'data/' + option1 + '/output0_nt_Photons_t0.csv') or (i == 'data/' + option1 + '/output0_nt_Photons_t1.csv')):
                    fEvent1.append(float(row[0]))
                    fwlen1.append(float(row[7]))
                if ( (i == 'data/' + option2 + '/output0_nt_Photons_t0.csv') or (i == 'data/' + option2 + '/output0_nt_Photons_t1.csv')):
                    fEvent2.append(float(row[0]))
                    fwlen2.append(float(row[7]))
                fX.append(float(row[1]))
                fY.append(float(row[2]))
                fZ.append(float(row[3]))
                fmomX = float(row[4])
                fmomY = float(row[5])
                fmomZ = float(row[6])
                mag = math.sqrt(np.square(fmomX) + np.square(fmomY) + np.square(fmomZ))
                fmomMag.append(mag)
            count += 1

plt.hist(fEvent2, bins=50, label=option2)
plt.hist(fEvent1, bins = 50, label=option1)
plt.title("Number of photons for each event (" + option1 + " vs " + option2 + ")")
plt.xlabel("Event(s)")
plt.ylabel("Frequency of photons")
plt.legend(loc='upper right')
plt.show()
fZ = np.array(fZ)
my_cmap = plt.get_cmap('hot')
fig = plt.figure()
ax = plt.axes(projection ='3d')
surf = ax.scatter3D(fX, fY, fZ,cmap = my_cmap,edgecolor ='none')
fig.colorbar(surf, ax = ax,shrink = 0.4, aspect = 5)
ax.set_title('Surface plot ('+ option2 + ')')
ax.set_xlabel("X-position (cm)")
ax.set_ylabel("Y-position (cm)")
ax.set_zlabel("Z-position (cm)")
plt.show()
plt.hist(fmomMag, bins=50)
plt.title("Photon momentum magnitude ("+ option1 + ")")
plt.xlabel("Momentum (eV)")
plt.ylabel("Frequeny")
plt.show()
plt.hist(fwlen1, bins=50, label=option1)
plt.hist(fwlen2, bins=50, label=option2)
plt.title("Photon wavelength ((" + option1 + " vs " + option2 + ")")
plt.xlabel("Wavelength (nm)")
plt.ylabel("Frequency")
plt.legend(loc='upper right')
plt.show()

