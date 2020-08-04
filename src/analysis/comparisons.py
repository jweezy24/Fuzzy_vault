import sys
import os
import time
import matplotlib.pyplot as plt

def parse_data(name):
    file_name = ''
    for path, directories, files in os.walk('./src/tests/data'):
        for file_ in files:
            if 'results' in file_ and path == "./src/tests/data" and "noise" in file_:
                if name in file_:
                    file_name = f"{path}/{file_}"
                    break
    
    with open(file_name,'r') as f:
        x = []
        data = {}
        ratio_y = []
        time_y = []
        GF = ''
        fig, (ax, bx) = plt.subplots(2)

        for line in f:
            p1 = line.split('\t')
            if GF == '':
                GF = p1[5]
            elif GF != p1[5]:
                GF = p1[5]
                ratio_y = []
                time_y = []
                x = []
            ratio_y.append(float(eval(p1[1].split(' ')[1])))
            time_y.append(float(p1[4].split(' ')[2]))
            x.append(int(p1[2]))

        for key in data:
            ax.set_ylabel('Message Correctly Decoded (Percent)')
            bx.set_xlabel('Percent Chance of Symbol Error')
            bx.set_ylabel('Time (Seconds)')
            ax.plot(x, ratio_y, label=)
            ax.legend(loc="upper right")
            bx.plot(x, time_y)
            fig.suptitle(f'{name} Comparison')
        plt.savefig(f"../Fuzzy_paper/paper/figures/{name}_graph.pdf")
           

def main():
    parse_data('OAP')


main()