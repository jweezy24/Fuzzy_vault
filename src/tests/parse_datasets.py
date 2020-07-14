import os
import sys
#import threading
from concurrent.futures import ThreadPoolExecutor

def parse_car_data(filename,outputfile,var_name):
    data = []
    for i in range(1, 13):
        filename.replace("Sensor-01", f"Sensor-0{i}")
        with open(filename,"r") as f:
            for line in f:
                raws = line.split(' ')
                try:
                    data.append(float(raws[0]))
                except Exception as e:
                    print(e)
        filename.replace(f"Sensor-0{i}", "Sensor-01")
    print(f"Size of {filename} = {len(data)}")
    
    with open(outputfile, "a+") as f:
        f.write(f'float {var_name}[] = ')
        f.write('{')
        for pos in range(0,len(data)):
            if(pos < len(data)-1):
                f.write(f" {data[pos]}, ")
            else:
                f.write(f"{data[pos]}")
                f.write("};\n")

def build():
    objects = []
    for (root,dirs,files) in os.walk('./data', topdown=True):
        for file_ in files:
            if '.c' in file_:
                os.system(f"gcc -c ./data/{file_} -o ./data/{file_.split('.')[0]}.o")
                objects.append(f"./data/{file_.split('.')[0]}.o")
    command = 'ld -r '
    for file_ in objects:
        command += f"{file_} "
    command += " -o final.o"
    os.system(command)
    

def main():
    threads = []
    os.system("cd ../../ && make clean")
    with ThreadPoolExecutor(max_workers=10) as executor: 
        executor.submit(parse_car_data, "../../datasets/ZIADatasets/CarDataset/Sensor-01/ble/ble.txt", "./data/ble.c", "ble")
        executor.submit(parse_car_data, "../../datasets/ZIADatasets/CarDataset/Sensor-01/sensors/accData", "./data/acc.c", "acc")
        executor.submit(parse_car_data, "../../datasets/ZIADatasets/CarDataset/Sensor-01/sensors/barData", "./data/bar.c", "bar")
        executor.submit(parse_car_data, "../../datasets/ZIADatasets/CarDataset/Sensor-01/sensors/gyrData", "./data/gry.c", "gry")
        executor.submit(parse_car_data, "../../datasets/ZIADatasets/CarDataset/Sensor-01/sensors/humData", "./data/hum.c", "hum")
        executor.submit(parse_car_data, "../../datasets/ZIADatasets/CarDataset/Sensor-01/sensors/luxData", "./data/lux.c", "lux")
        executor.submit(parse_car_data, "../../datasets/ZIADatasets/CarDataset/Sensor-01/sensors/magData", "./data/mag.c", "mag")
        executor.submit(parse_car_data, "../../datasets/ZIADatasets/CarDataset/Sensor-01/sensors/tmpData", "./data/tmp.c", "tmp")
        executor.submit(parse_car_data, "../../datasets/ZIADatasets/CarDataset/Sensor-01/wifi/wifi.txt", "./data/wifi.c", "wifi")
    build()

main()