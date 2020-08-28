import os
import sys
import matplotlib.pyplot as plt


def analize_file(file_name,nickname,hamming=False):
    dictionary = {}
    if not hamming:
        with open(f"./src/tests/data/{file_name}", "r") as f:
            for line in f:
                data = line.split('\t')
                failures = data[1].split(' ')[1]
                runs = failures.split('/')[1]
                message_size = data[5].split(' ')[2].split(',')[0]
                noise_size = data[3].split(' ')[2]
                simulated_error = data[2].split(' ')[1]
                time  = data[4].split(' ')[2]
                val = (failures,runs,noise_size,time,simulated_error)
                if message_size not in dictionary.keys():
                    entry = {message_size: [val]}
                    dictionary.update(entry)
                else:
                    tmp = dictionary[message_size]
                    tmp.append(val)
                    dictionary.update({message_size: tmp})
        
        fig, (ax, bx) = plt.subplots(2)
        x_axis_1 = []
        y_axis_1 = []
        x_axis_2 = []
        y_axis_2 = []
        for key in dictionary.keys():
            for item in dictionary[key]:
                percent_fail = eval(item[0])
                sim = int(item[4])
                runs = int(item[1])
                bits = int(item[2])
                total_time = float(item[3])
                x_axis_1.append(sim)
                x_axis_2.append(sim)
                y_axis_1.append(percent_fail*100)
                y_axis_2.append(float(total_time/runs))
            
            fig.suptitle(f"Message size Comparison")
            ax.set_ylabel('Message failure as percent')
            bx.set_xlabel('Bits per coefficent')
            bx.set_ylabel('Time Per decoding (seconds)')
            ax.plot(x_axis_1, y_axis_1,label=key)
            bx.plot(x_axis_2, y_axis_2,label=key)
            x_axis_1 = []
            y_axis_1 = []
            x_axis_2 = []
            y_axis_2 = []

        fig.savefig(f"../Fuzzy_paper/paper/figures/message_size_graph_{nickname}.pdf")
        fig.clf()
    else:
        with open(f"./src/tests/data/{file_name}", "r") as f:
            for line in f:
                data = line.split('\t')
                failures = data[0].split(' ')[1]
                runs = failures.split('/')[1]
                message_size = data[2].split(' ')[6].replace('(',"").replace(',',"")
                noise_size = data[2].split(' ')[2]
                simulated_error = data[1].split(' ')[2]
                time  = data[2].split(' ')[4]
                val = (failures,runs,noise_size,time,simulated_error)
                if message_size not in dictionary.keys():
                    entry = {message_size: [val]}
                    dictionary.update(entry)
                else:
                    tmp = dictionary[message_size]
                    tmp.append(val)
                    dictionary.update({message_size: tmp})
        
        fig, (ax, bx) = plt.subplots(2)
        x_axis_1 = []
        y_axis_1 = []
        x_axis_2 = []
        y_axis_2 = []
        for key in dictionary.keys():
            for item in dictionary[key]:
                percent_fail = eval(item[0])
                sim = int(item[4])
                runs = int(item[1])
                bits = int(item[2])
                total_time = float(item[3])
                x_axis_1.append(sim)
                x_axis_2.append(sim)
                y_axis_1.append(percent_fail*100)
                y_axis_2.append(float(total_time/runs))
            
            fig.suptitle(f"Message size Comparison")
            ax.set_ylabel('Message failure as percent')
            bx.set_xlabel('Bits per coefficent')
            bx.set_ylabel('Time Per decoding (seconds)')
            ax.plot(x_axis_1, y_axis_1,label=key)
            bx.plot(x_axis_2, y_axis_2,label=key)
            x_axis_1 = []
            y_axis_1 = []
            x_axis_2 = []
            y_axis_2 = []

        fig.savefig(f"../Fuzzy_paper/paper/figures/message_size_graph_{nickname}.pdf")
        fig.clf()


if __name__ == "__main__":
    #analize_file("results_noise_OAP.txt","OAP")
    #analize_file("results_noise_new.txt","NEW")
    analize_file("results_noise_hamming.txt","hamming",hamming=True)