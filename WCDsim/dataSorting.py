import csv
import numpy as np
import os.path


outputs = np.arange(0,33,1)
output_t = np.arange(0,2,1)



with open('data/data.csv', 'w') as file2:
    writer = csv.writer(file2)

    for j in output_t:
        for i in outputs:
            file = "/output" + str(outputs[i]) + "_nt_Photons_t" + str(output_t[j]) + ".csv"
            path = "data" + file
            if os.path.isfile(path) == True:
                print(file)
                with open(path) as file1:
                    csvreader = csv.reader(file1)
                    count = 0
                    for row in csvreader:
                        if count > 12:
                            fEvent = float(row[0])
                            fX = float(row[1])
                            fY = float(row[2])
                            fZ = float(row[3])
                            fmomX = float(row[4])
                            fmomY = float(row[5])
                            fmomZ = float(row[6])
                            fwlen = float(row[7])
                            fT = float(row[8])
                            writer.writerow([fEvent,fX,fY,fZ,fmomX,fmomY,fmomZ,fwlen,fT,i])
                        count += 1
            else:
                continue
