file_path = "./bits.log"
import random

def get_binary_string_list(total):
    count = 0
    string = ''
    amount = total
    strs = []

    with open(file_path, 'r') as f:
        for line in f:
            if count < total:
                if len(line) <= 2 and ('1' in line or '0' in line):
                    strs.append(line.strip())
                else:
                    for i in line:
                        if i == '0' or i == '1':
                            strs.append(i)
                count+=1
            else:
                break

            
    return strs

def create_G():
    p_1 = ['0','1','1']
    G = []
    G.append(p_1)
    return G

def create_H():
    r1 = ['1','0','1']
    H = []
    H.append(r1)
    return H

#AxB
def mat_mult_encoding(a,b):
    ret = ''
    sum = 0
    for i in range(0,len(a[0])):
        ret += str((int(b) * int(a[0][i])))
    return ret

def mat_mult_decoding(a,b):
    ret = ''
    sum = 0
    for i in range(0,len(a[0])):
        for j in range(0,3):
            sum += (int(b[j]) * int(a[0][i]))
        ret+= str(sum%2)
        sum = 0 
        #print(ret)
    return ret



def hammingE(binary, G):
    encoding = mat_mult_encoding(G, binary)
    return encoding

def hammingD(binary, H):
    code = mat_mult_decoding(H, binary)
    return code

def correct(code, row, H):
    #print(code)
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
    total = 1000
    binarys = get_binary_string_list(total)
    G = create_G()
    H = create_H()
    for per in range(0,101):
        bad = 0
        for binary in binarys:
            orig = binary
            code = hammingE(orig, G)
            new_code = ''
            for i in code:
                chance = int(random.random()*100)%100
                val = i
                if chance < per:
                    if i == '0':
                        val = '1'
                    else:
                        val = '0'
                new_code += val

            check = hammingD(new_code, H)
            #print(check)
            #print(code)
            if check != '000':
                new_code = correct(new_code, check, H)
                #print(new_code)
            if new_code != orig:   
                bad+=1
        with open("./src/tests/data/results_noise_hamming_31.txt", 'a') as f:
            f.write(f"Ratio {bad}/{total}\t Percent {per}\n")

if "__name__" == "__main__":
    main()
main()