import os
import sys
import matplotlib.pyplot as plt

def parse_data_plot():
    legened = []
    p=[]
    for path, directories, files in os.walk('./src/tests/data'):
        for file_ in files:
            if 'results' in file_ and path == "./src/tests/data" and "noise" not in file_:
                x_axis = []
                y_axis = []
                filepath = f"{path}/{file_}"
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
                        p.append((percent, percent_fail))
                 
                p.sort(key = lambda x: x[0])  
                for x,y in p:
                    print(x)
                    print(y)
                    x_axis.append(x)
                    y_axis.append(y)
                plt.plot(x_axis,y_axis)
                plt.legend(legened)
    
    
    plt.ylabel('Percent Chance of a Failed Key')
    plt.xlabel('Percent Chance of Noise Interference')
    plt.savefig("../Fuzzy_paper/paper/figures/noise_to_key_accuracy.pdf")
    plt.clf()



def parse_data_bar():
    legened = []
    y_axis = []
    for path, directories, files in os.walk('./src/tests/data'):
        for file_ in files:
            if 'results' in file_ and path == './src/tests/data':
                x_axis = []
                filepath = f"./src/tests/data/{file_}"
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
                        if count == 10:
                            y_axis.append(percent_fail)
                        count+=1
    
    plt.bar(legened,y_axis)
    plt.ylabel('Percent Chance of a Failed Key at 20% Noise')
    plt.xlabel('Datasets')
    plt.savefig("../Fuzzy_paper/paper/figures/DatasetComparisons.pdf")


def noise_size_difference():
    legened = []
    p=[]
    last_arr = 0
    for path, directories, files in os.walk('./src/tests/data'):
        for file_ in files:
            if 'results_noise' in file_ and path == "./src/tests/data":
                x_axis = []
                y_axis = []
                filepath = f"{path}/{file_}"

                with open(filepath, 'r') as f:
                    for line in f:
                        
                        l_lst = line.split("\t")
                        if last_arr != int(l_lst[3].split(" ")[2].strip()):
                            array_iter = int(l_lst[3].split(" ")[2].strip())
                            legened.append(array_iter)
                            if last_arr != 0:
                                plt.plot(x_axis, y_axis)
                                x_axis = []
                                y_axis = []
                            last_arr = array_iter
                            
                        percent_fail = float(eval(l_lst[1].split(" ")[1].strip()))
                        percent = int(l_lst[2].strip())
                        x_axis.append(percent)
                        y_axis.append(percent_fail)
                    plt.plot(x_axis, y_axis)
                    x_axis = []
                    y_axis = []

    plt.legend(legened)
    plt.ylabel('Percent Chance of a Failed Key')
    plt.xlabel('Percent Chance of Noise Interference')
    plt.savefig("../Fuzzy_paper/paper/figures/noise_size_to_key_len.pdf")
    plt.clf()


def main():
    #parse_data_plot()
    #parse_data_bar()
    noise_size_difference()

main()