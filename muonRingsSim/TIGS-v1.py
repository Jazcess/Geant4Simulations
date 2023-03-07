import numpy as np
import pandas as pd

import math as mt
from scipy import constants
import matplotlib
import matplotlib.pyplot as plt


# from graphs import *
import csv


def macro(w):
    global X_w
    global Y_w
    global Z_w

    X_w = float(input("Enter the X Coordinates for the world: "))
    Y_w = float(input("Enter the Y coordinates for the world: "))
    Z_w = float(input("Enter the Z coordinates for the world: "))

    lines = []  # Creates an empty list lines
    target_line = None
    target_line_index = None  # A variable 'target_line_index' is set to none
    with open("set.mac", "r") as file:  # Opens the file in read mode and reads all the lines
        lines = file.readlines()
    for i, line in enumerate(lines):  # A loop iterates over all the lines in the files and stores them in the 'lines' list
        if "0.1" in line:  # If the target string (in this case 0.1) is found in the line, the line is stored in a variable named 'target_line'
            target_line = line
            target_line_index = i  # This stores the line number in a variable 'target_line_index'
            break
    if target_line is None:  # If the target line has not been found, a ValueError is raised with the message: 'Target line 1 not found in the file'
        raise ValueError("Target line 1 not found in the file")
    updated_line = line.replace("0.1", str(X_w))
    lines[target_line_index] = updated_line

    target_line = None
    target_line_index = None
    with open("set.mac", "r") as file:
        lines = file.readlines()
    for i, line in enumerate(lines):
        if "0.1" in line:
            target_line = line
            target_line_index = i
            break
    if target_line is None:
        raise ValueError("Target line 2 not found in the file")
    updated_line = line.replace("0.1", str(Y_w))
    lines[target_line_index] = updated_line

    target_line = None
    target_line_index = None
    with open("set.mac", "r") as file:
        lines = file.readlines()
    for i, line in enumerate(lines):
        if "0.1" in line:
            target_line = line
            target_line_index = i
            break
    if target_line is None:
        raise ValueError("Target line 3 not found in the file")
    updated_line = line.replace("0.1", str(Z_w))
    lines[target_line_index] = updated_line

    with open("set.mac", "w") as file:
        file.writelines(lines)


def mac(a, b, c):
    # Create a list of target values
    target_values = [a, b, c]
    # Read the existing .mac file
    lines = []
    with open("set.mac", "r") as file:
        lines = file.readlines()
    # Replace each target line with the corresponding target value
    target_line = "0.05"
    for i, line in enumerate(lines):
        if target_line in line:
            updated_line = line.replace(target_line, str(target_values[i % 3]))
            lines[i] = updated_line
    # Write the updated lines back to the .mac file
    with open("set.mac", "w") as file:
        file.writelines(lines)





while (
    True
):  # running a while loop that is True to make it an infinte loop until broken using "break" command

    print("WELCOME TO YOUR TIGS-V1.0! ENTER AN OPTION TO CONTINUE:")
    print(" ")
    options = """
1. Enter the Parametersfor a New World
2. Calculate the characteristic impedance for the data inputed
3. Calculate the uncertainty in character impedance from data inputed
4. Append data to database
5. Add data to MySQL Database

6. Exit Database



Select An Option To Continue: """

    try:

        option = int(
            input(options)
        )  # user provides machine with a number from the above options
        if option not in range(1, 7):
            raise ValueError  # this raises a Value Error that is excepted at the end of the code and telling the user that the value is not in range
        if option == 1:

            z_w = float(input("Enter Z coords:"))
            y_w = float(input("Enter Y coords:"))
            x_w = float(input("Enter X coords:"))
            mac(x_w, y_w, z_w)



        elif option == 2:  # start of our control flow statements to make the program work in an option statement that is given by the user
            z = float(input("Enter Z coords:"))
            y = float(input("Enter Y coords:"))
            x = float(input("Enter X coords:"))
            mac(x, y, z)


        elif option == 7:
            print("Entering Data...")

            print("Done.")
            break

    except ValueError:
        print("Invalid Option, Try Again \n")
        continue







