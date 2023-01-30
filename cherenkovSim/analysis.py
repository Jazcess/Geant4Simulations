import csv
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm




output = 'data/output0_nt_Photons.csv'


fEvent = []
fX = []
fY =[]
fZ =[]
fwlen=[]


with open(output) as file:
    csvreader = csv.reader(file)
    count = 0
    for row in csvreader:
        if count > 9:
            fEvent.append(float(row[0]))
            fX.append(float(row[1]))
            fY.append(float(row[2]))
            fZ.append(float(row[3]))
            fwlen.append(float(row[4]))
        count += 1


plt.hist2d(fX,fY, bins=50, norm=LogNorm())
plt.show()
plt.hist(fX, bins = 100)
plt.show()
plt.hist(fY, bins = 100)
plt.show()
plt.hist(fZ, bins = 100)
plt.show()
plt.hist(fEvent, bins = 50)
plt.show()
plt.hist(fwlen, bins = 50)
plt.show()

