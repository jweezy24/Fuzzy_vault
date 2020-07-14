import os
import sys
import matplotlib.pyplot as plt

def parse_data_plot():
    legened = []
    for path, directories, files in os.walk('./'):
        for file_ in files:
            if 'results' in file_ and path == "./":
                x_axis = []
                y_axis = []
                filepath = f"./{file_}"
                if "_" in filepath:
                    label = file_.split('_')[1].split('.')[0]
                else:
                    label = "Electricity"

                legened.append(label)
                with open(filepath, 'r') as f:
                    for line in f:
                        l_lst = line.split("\t")
                        percent_fail = float(eval(l_lst[1].split(" ")[1].strip()))
                        percent = int(l_lst[2].strip())
                        x_axis.append(percent)
                        y_axis.append(percent_fail)
                        plt.plot(x_axis,y_axis)
                        plt.legend(legened)
    
    
    plt.ylabel('Percent Chance of a Failed Key')
    plt.xlabel('Percent Chance of Noise Interference')
    plt.savefig("../Fuzzy_paper/paper/figures/noise_to_key_accuracy.pdf")
    plt.clf()



def parse_data_bar():
    legened = []
    y_axis = []
    for path, directories, files in os.walk('./'):
        for file_ in files:
            if 'results' in file_ and path == "./":
                x_axis = []
                filepath = f"./{file_}"
                if "_" in filepath:
                    label = file_.split('_')[1].split('.')[0]
                else:
                    label = "Electricity"

                legened.append(label)
                with open(filepath, 'r') as f:
                    count = 0
                    for line in f:
                        l_lst = line.split("\t")
                        percent_fail = float(eval(l_lst[1].split(" ")[1].strip()))
                        percent = int(l_lst[2].strip())
                        if count == 2:
                            y_axis.append(percent_fail)
                        count+=1
    
    plt.bar(legened,y_axis)
    plt.ylabel('Percent Chance of a Failed Key at 20% Noise')
    plt.xlabel('Datasets')
    plt.savefig("../Fuzzy_paper/paper/figures/DatasetComparisons.pdf")


def main():
    parse_data_plot()
    parse_data_bar()

main()