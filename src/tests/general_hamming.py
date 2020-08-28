import os, sys
import random
import time
file_path = "./bits.log"

def get_binary_string_list(total,size):
    count = 0
    string = ''
    amount = total
    strs = []

    with open(file_path, 'r') as f:
        for line in f:
            if amount > 0:
                if len(string) == size:
                    strs.append(string)
                    string = ''
                    count = 0
                    amount -= 1
                if len(string) > size:
                    string = ''
                    count = 0
                else:
                    if '1' in line or '0' in line:
                        string += line.strip()
                        count += 1
                if amount == 0:
                    break
    return strs

def generate_I(num):
    I = []
    for i in range(0,num):
        row = []
        for j in range(0, num):
            if i == j:
                row.append(1)
            else:
                row.append(0)
        I.append(row)
    return I

def generate_P(ham, num):
    size = ham-num
    P = []
    for i in range(0,size):
        row = []
        for j in range(0, num):
            if i == j:
                row.append(0)
            else:
                row.append(1)
        P.append(row)
    return P

def create_G(ham,num):
    I = generate_I(num)
    P = generate_P(ham, num)
    G = []
    for row in I:
        G.append(row)
    
    for row in P:
        G.append(row)

    return G


def mat_mult_encoding(a,b):
    ret = ''
    for row in b:
        sum = 0
        for i in range(0,len(row)):
            sum += (int(a[i]) * int(row[i]))
        ret += str(sum%2)
    return ret



def find_all_hamming_codes(num, G):
    nums = (2**num) - 1
    codes = []
    for i in range(0, nums):
        binary = format(i, f'0{num}b')
        code = mat_mult_encoding(binary, G)
        count = 0
        for j in codes:
            if j == code:
                break
            else:
                count+=1
        if count >= len(codes):
            codes.append(code)
            count = 0
    return codes

def map_code_to_given_stream(stream, codes, min):
    num = int(stream, 2)
    min = 2**min -1
    pos = 0
    for i in codes:
        tmp = num ^ int(i, 2)
        if tmp < min:
            min = tmp
            pos = i
    
    return pos
    



def print_matrix(mat):
    for i in mat:
        string = ''
        for j in i:
            string += f"{j}\t"
        print(string)



def main(n1, n2):
    total = 10000
    binarys = get_binary_string_list(total,n1)
    G = create_G(n1,n2)
    codes = find_all_hamming_codes(n2, G)
    for per in range(0,101):
        bad = 0
        start = time.time()
        for binary in binarys:
            orig = binary
            new_code = ''
            for i in orig:
                chance = int(random.random()*100)%100
                val = i
                if chance < per:
                    if i == '0':
                        val = '1'
                    else:
                        val = '0'
                new_code += val
    
            c1 = map_code_to_given_stream(new_code, codes, n1)
            c2 = map_code_to_given_stream(orig, codes, n1)
            
            if c1 != c2:   
                bad+=1
        end = time.time()
        seconds = end - start 
        with open(f"./src/tests/data/results_noise_hamming_{n1}{n2}.txt", 'a') as f:
            f.write(f"Ratio {bad}/{total}\t Percent {per}\t Noise: {n2} Time: {seconds} GF: {n1,n2} \n")


main(31,26)