from scipy.io import loadmat
import os

#x = loadmat('./together.mat')
x = loadmat('../../../AEP_Prototype/after_correction_256.mat')


os.system(f"rm vk1.*")

os.system(f"rm vk2.*")


ave1 = 0
ave2 = 0
count = 0
stream1 = ''
stream2 = ''
tmp1 = []
tmp2 = []
for row in x['after_correction_256']:
    count+=1
    stream1 += str(row[0])
    stream2 += str(row[1])

print(count)

with open('./vk1.c', 'a+') as f:
    f.write(f'char vk1[] = ')
    f.write('{')
    for pos in range(0,len(stream1)):
        bit = stream1[pos]
        if pos < len(stream1)-1:
            f.write(f"'{bit}',")
        else:
            f.write(f"'{bit}'")
            f.write("};")

with open('./vk2.c', 'a+') as f:
    f.write(f'char vk2[] = ')
    f.write('{')
    for pos in range(0,len(stream2)):
        bit = stream2[pos]
        if pos < len(stream2)-1:
            f.write(f"'{bit}',")
        else:
            f.write(f"'{bit}'")
            f.write("};")

os.system(f"gcc -c ./vk1.c -o ./vk1.o")
os.system(f"gcc -c ./vk2.c -o ./vk2.o")