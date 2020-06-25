import os
import sys
import matplotlib.pyplot as plt

def parse_data(filepath):
    x_axis = []
    y_axis = []
    with open(filepath, 'r') as f:
        for line in f:
            l_lst = line.split("\t")
            percent_fail = float(eval(l_lst[1].split(" ")[1].strip()))
            percent = int(l_lst[2].strip())
            x_axis.append(percent)
            y_axis.append(percent_fail)
    
    
    plt.plot(x_axis,y_axis)
    plt.ylabel('Percent Chance of a Failed Key')
    plt.xlabel('Percent Chance of Noise Interference')
    plt.savefig("../Fuzzy_paper/paper/figures/noise_to_key_accuracy.pdf")


def main():
    f_1 = './results.txt'
    parse_data(f_1)

main()