file_path = "./bits.log"
import random

def get_binary_string_list(total):
    count = 0
    string = ''
    amount = total
    strs = []

    with open(file_path, 'r') as f:
        for line in f:
            if len(string) == 4:
                strs.append(string)
                string = ''
                count = 0
                amount -= 1
            if len(string) > 4:
                string = ''
                count = 0
            else:
                if '1' in line or '0' in line:
                    string += line.strip()
                    count += 1
            if amount == 0:
                break
            
    return strs

def create_G():
    p_1 = ['0', '1','1','1']
    p_2 = ['1','0', '1','1']
    p_3 = ['1', '1','0','1']
    d_1 = ['1', '0','0','0']
    d_2 = ['0', '1','0','0']
    d_3 = ['0', '0','1','0']
    d_4 = ['0', '0','0','1']
    G = []
    G.append(p_1)
    G.append(p_2)
    G.append(p_3)
    G.append(d_1)
    G.append(d_2)
    G.append(d_3)
    G.append(d_4)
    return G

def create_H():
    r1 = ['0', '0', '1']
    r2 = ['0', '1', '0']
    r3 = ['1', '0', '0']
    r4 = ['1', '1', '0']
    r5 = ['1', '0', '1']
    r6 = ['0', '1', '1']
    r7 = ['1', '1', '1']
    H = []
    H.append(r1)
    H.append(r2)
    H.append(r3)
    H.append(r4)
    H.append(r5)
    H.append(r6)
    H.append(r7)
    return H

#AxB
def mat_mult_encoding(a,b):
    ret = ''
    for row in b:
        sum = 0
        for i in range(0,len(row)):
            sum += (int(a[i]) * int(row[i]))
        ret += str(sum%2)
    return ret

def mat_mult_decoding(a,b):
    ret = []
    iter = 0
    for iter in range(0,3):
        row = []
        for i in range(0, len(b)):
            row.append(b[i][iter])
        sum = 0
        for j in range(0,len(row)):
            sum += (int(a[j]) * int(row[j]))
        ret.append(str(sum%2))
    return ret



def hammingE(binary, G):
    encoding = mat_mult_encoding(binary, G)
    return encoding

def hammingD(binary, H):
    code = mat_mult_decoding(binary, H)
    return code

def correct(code, row, H):
    corrected = ''
    count = 0
    bit_flip = -1
    for row2 in H:
        if row == row2:
            bit_flip = count 
        count += 1
    count = 0
    for i in code:
        val = i
        if bit_flip == count:
            if i == '1':
                val = '0'
            else:
                val = '1'
        corrected+=val
        count+=1
    return corrected



def main():
    total = 10000
    binarys = get_binary_string_list(total)
    G = create_G()
    H = create_H()
    for per in range(0,101):
        bad = 0
        for binary in binarys:
            orig = binary
            print(orig)
            code = hammingE(orig, G)
            new_code = ''
            for i in code:
                chance = int(random.random()*100)%100
                val = i
                if chance < per:
                    if i == '0':
                        val = '1'
                    else:
                        val = '0';
                new_code += val

            check = hammingD(new_code, H)
            
            if check != '000':
                new_code = correct(new_code, check, H)
            
            if new_code[3:] != orig:   
                bad+=1
        with open("./src/tests/data/results_noise_hamming.txt", 'a') as f:
            f.write(f"Ratio {bad}/{total}\t Percent {per}\n")

if "__name__" == "__main__":
    main()
main()