import csv
import matplotlib.pyplot as plt
#from matplotlib.colors import LogNorm
import numpy as np



output = 'data/output0_nt_'

data = ['Photons.csv', 'Scoring.csv']

for i in data:
    i = output + i
    fEvent = []
    fX = []
    fY =[]
    fZ =[]
    fwlen=[]
    fEdep=[]
    #fT=[]

    with open(i) as file:
        csvreader = csv.reader(file)
        count = 0
        for row in csvreader:
            if i == 'data/output0_nt_Photons.csv':
                if count > 9:
                    fEvent.append(float(row[0]))
                    fX.append(float(row[1]))
                    fY.append(float(row[2]))
                    fZ.append(float(row[3]))
                    fwlen.append(float(row[4]))
                    #fT.append(float(row[5]))
            if i == 'data/output0_nt_Scoring.csv':
                if count > 5:
                    fEdep.append(float(row[0]))
            count += 1

    if i == 'data/output0_nt_Scoring.csv':
        plt.hist(fEdep, bins=50)
        plt.show()
    else:
        fZ = np.array(fZ)
        my_cmap = plt.get_cmap('hot')
        fig = plt.figure()
        ax = plt.axes(projection ='3d')
        surf = ax.scatter3D(fX, fY, fZ,
                       cmap = my_cmap,
                       edgecolor ='none')
        fig.colorbar(surf, ax = ax,
             shrink = 0.4, aspect = 5)
        ax.set_title('Surface plot')
        plt.show()
        plt.show()
        plt.hist(fX, bins = 100)
        plt.show()
        plt.hist(fY, bins = 100)
        plt.show()
        plt.hist(fZ, bins = 100)
        plt.show()
        plt.hist(fEvent, bins = 50)
        plt.show()

        if i =='data/output0_nt_Photons.csv':
            plt.hist(fwlen, bins = 50)
            plt.show()
            #plt.hist(fT, bins = 50)
            #plt.show()



