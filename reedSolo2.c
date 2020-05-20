#include "./headers/reedSolo2.h"

char* mesg = "Hello";
long p = pow(2,16);
int symbols = 256;
unsigned int NW = 1 << 16;
unsigned int *gflog, *gfilog;
extern char bits[];
unsigned int prim_poly = (unsigned int) 0b10001000000001011;
//unsigned int prim_poly = 0b;

int* g(int t){
    int generator = 0b1011;
    int* tmp_1 = malloc(sizeof(int)*2);
    tmp_1[0] = generator;
    tmp_1[1] = 1;
    int size = 2;
    for(int i = 1; i < 2*t; i++){
        int* tmp_2 = malloc(sizeof(int)*2);
        tmp_2[0] = gf_pow(generator, i+1);
        tmp_2[1] = 1;
        int* holder = gf_mult_poly(tmp_1, tmp_2, size, 2);
        size = 2+size-1;
        tmp_1 = realloc(tmp_1, size*sizeof(int));
        for(int j =0; j < size; j++){
            tmp_1[j] = holder[j];
        }
    }
    return tmp_1;
}

//encoding chars
int* m_charp(int n, int k, int t){
    int * M = malloc(sizeof(int)*n);
    int pos_holder = n-1;
    int count = 0;
    
    for(int i =0; mesg[i] != 0; i++){
        int letter = mesg[i];
        count = 0;

        M[pos_holder] = letter;
        pos_holder-=1;
        
    }

    for(int i = pos_holder; i >=0; i--){
        M[i] = 0;
    }
    return M;
}

//encoding bits
int* m_(int n, int k, int t){
    int * M = malloc(sizeof(int)*n);
    char* tmp = malloc(sizeof(char)*9);
    int pos_holder = n-1;
    int bits_pos = 0;
    int count = 0;
    
    for(int i =0; i <= k*8; i++){
        if(count%8 == 0 && count > 0){
            unsigned int tmp_h = (unsigned int)str_int(tmp);
            M[pos_holder] = tmp_h;
            pos_holder--;
            count = 0;
            i--; 
        }else{
            tmp[count] = bits[bits_pos];
            count+=1;
            bits_pos+=1;
        }
        
    }

    for(int i = pos_holder; i >=0; i--){
        M[i] = 0;
    }
    return M;
}

int* syndome_calculator_collective_sums(int* C, int len_c, int t){
    int* Synd = malloc(sizeof(int)* 2*t);
    int generator = 0b1011;
    for(int i =0; i < 2*t; i++){
        int S_i = 0;
        int alpha = gf_pow(generator, i);
        for(int j = 0; j< len_c; j++){
            S_i ^= gf_mult(C[j], gf_pow(alpha, j));
        }
        Synd[i] = S_i;
    }
    return Synd;
}

int* syndome_calculator_division(int* C, int* g, int len_c, int len_g){

    return gf_div_poly(C, g, len_c, len_g, 1);

}



int* encode_message(int n, int k, int t){
    int* g_x = g(t); 
    int* M = m_(n, k, t);

    int* Q = gf_div_poly(M, g_x, n, n-k,0);
    int* CK = gf_div_poly(M, g_x, n, n-k,1);

    int* C = gf_poly_add(M,CK, n, n-k);

    //int* S_i_d = syndome_calculator_division(C, g_x, n, n-k);

    //print_arr(C, n);

    int** sigmas = berlecamp_table(C, t, n);

    int* s = sigmas[2*t+1];
    int size = sigmas[2*t+2][2*t+1];

    int* s_r = sigma_r(s, size);

    print_arr(s, size);

    print_arr(s_r, size);

    //int* sig = error_locating_poly(S_i_d,  t, n-k);

    return C;
}


int* error_locating_poly(int* S, int t, int n){

    int* init = malloc(sizeof(int)*2*t);
    int** rem_S = malloc(sizeof(int*)*2*t);
    int remainders = 1;

    for(int i =0; i <= 2*t-1; i++){
        if(i == 2*t-1){
            init[i] = 1;
        }else{
            init[i] = 0;
        }
    }
    
    int checked = 1;
    while(checked && remainders < 2*t){
        checked = 0;
        
        init = gf_div_poly(S, init, n, 2*t,1);
        
        print_arr(init, n);

        for(int i=0; i < 2*t; i++){
            if(init[i] != 0){
                checked = 1;
            }
        }

        if(checked){
            rem_S[remainders-1] = init;
            remainders++;
        }else{
            printf("HERE\n");
        }

        // for( int i =0; i < remainders; i++){
        //     print_arr(rem_S[i], 2*t);
        // }

    }

    return 0;

}

int** berlecamp_table(int* R, int t, int n){

    int** b_table = malloc(sizeof(int*)* (2*t+2));
    int** sigmas = malloc(sizeof(int*)*(2*t+3));
    int* sigma_sizes = malloc(sizeof(int)*(2*t+2));


    int r1[5] = {-1, 1, 1, 0, 1};
    int tmp = calculate_S(R, 1, n);
    int r2[5] = {0, 1, 1, tmp, 0};
    
    int* simga_r1 = malloc(sizeof(int));
    int* simga_r2 = malloc(sizeof(int));

    for(int i = 0; i < 1; i++){
        if(i == 0){
            simga_r1[i] = 1;
            simga_r2[i] = 1;
        }else{
            simga_r1[i] = 1;
            simga_r2[i] = 1;
        }
    }
    sigmas[0] = simga_r1;
    sigmas[1] = simga_r2;

    sigma_sizes[0] = 1;
    sigma_sizes[1] = 1;

    b_table[0] = r1;
    b_table[1] = r2;

    for(int i = 2; i < 2*t+2; i++){
        int* row_before = b_table[i-1];
        int* curr_row = malloc(sizeof(int)*5);
        
        int mu = i;
        
        int d_mu_b = calculate_S(R, i-1, n);
        int d_mu = calculate_S(R, i, n);
        
        int* sigma_mu_b = sigmas[i-1];
        int* sigma_mu = malloc(sizeof(int)*2*t);

        int h_mu_b = row_before[4];
        int h_mu = 0;

        if(d_mu != 0){
            int max_h = h_mu_b;
            int max_h_pos = 0;
            int h_sums = 0;
            int* sigma_row;
            for(int j = i-1; j >= 0; j--){
                int* row = b_table[j];
                int h_mu_j = row[4];
                h_sums = h_mu_j ^ abs(i-1) ^ abs(j-1);
                if (h_sums > max_h){
                    max_h = h_sums;
                    max_h_pos = j;
                    sigma_row = sigmas[j];
                }
            }
            int d_row = b_table[max_h_pos][4];
            int d_row_inv = gf_inverse(d_row);
            unsigned int coeff = gf_mult(d_mu_b, d_row_inv);
            int pow = i ^ abs(max_h_pos-1);
            int* new_x = x_pow(pow);
            int* new_sig = gf_mult_poly(new_x, sigma_row, pow, sigma_sizes[max_h_pos]);
            int new_size = pow+2*t - 1;
            sigma_sizes[i] = new_size;
            int* final_sig =  gf_poly_add(sigma_mu_b , new_sig, sigma_sizes[max_h_pos], new_size);
            //print_arr(final_sig, new_size);
            sigmas[i] = final_sig;
            curr_row[0] = abs(i-2);
            curr_row[1] = 1;
            if(mu-1 > 0){
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(final_sig, new_size, 1);
            }else{
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(final_sig, new_size, calculate_S(R, mu-1, n));
            }
            curr_row[3] = max_h;
            curr_row[4] = mu ^ max_h;
            b_table[i] = curr_row;
        }else{
            sigmas[i] = sigma_mu_b;
            sigma_sizes[i] = sigma_sizes[i-1];
            h_mu = h_mu_b;
            curr_row[0] = i-2;
            curr_row[1] = 1;
            if(mu-1 > 0){
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(sigma_mu_b, sigma_sizes[i-1], 1);
            }else{
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(sigma_mu_b, sigma_sizes[i-1], calculate_S(R, mu-1, n));
            }
            curr_row[3] = h_mu;
            curr_row[4] = mu ^ h_mu; 
        }

    }

    // for(int i = 0; i < 2*t+2; i++){
    //    printf("HERE\n");
    //    print_arr(sigmas[i], sigma_sizes[i]);
    // }

    // for(int i =0; i < 2*t+2; i++){
    //     free(b_table[i]);
    // }

    free(b_table);
    sigmas[2*t+2] = sigma_sizes;

    return sigmas;

}

int* sigma_r(int* s, int len){
    int pos = 0;
    int* sigma_r = malloc(sizeof(int)*len);
    for(int i = len-1; i >=0; i--){
        sigma_r[pos] = s[i];
        pos++;
    }
    return sigma_r;
}

int* roots_of_poly(int* sigma_r, int len){
    int* zeros_ = malloc(sizeof(int));
    int zeros = 0;
    unsigned int num = 0;
    int generator = 0b1011;
    for(int i=0; i < len; i++){
        num = unsigned int eval_poly(sigma_r, len, gf_pow(generator, i));
        if(num == 0){
            zeros_[zeros] = gf_pow(generator,i);
            zeros = realloc(zeros_, sizeof(int)*(zeros+1));
            zeros
        } 
    }
}


int calculate_S(int* R, int num, int n){
    int generator = 0b1011;
    int ret = 0;
    for(int i = 0; i < n; i++){
        ret ^= gf_mult(R[i], gf_pow(gf_pow(generator,num), i));
    }
    return ret;
}



void print_arr(int* a, int len ){
    for(int i = 0; i < len; i++){
        printf("%u*x^%d + \t", a[i], i);
    }
    printf("\n");
}

int parity_check(int* C, int* g, int len){
    for(int i =0; i < len; i++){
        if(C[i] != g[i]){
            printf("C: %d\t G: %d\t i: %d\n", C[i], g[i], i);
            return 0;
        }
    }
    return 1;
}

int* x_pow(int pow){
    if(pow == 0){
        int* poly_ = malloc(sizeof(int));
        poly_[0] = 1;
        return poly_;
    }
    
    int* poly  = malloc(sizeof(int)*(pow+1));
    
    for(int i = 0; i < pow+1; i++){
        if(i == pow){
            poly[i] = 1;
        }else{
            poly[i] = 0;
        }
    }

    return poly;
}

int eval_poly(int* poly, int len, int x){
    int num = 0;
    for(int i =0; i <= len; i++){
        num ^= gf_mult(poly[i], gf_pow(x, i));
    }
    return num;
}

void decode(int* C, int k, int n){
    int* mesg = malloc(sizeof(int)*k);
    int mesg_pos = 0;
    for(int i =n-1; i>=n-k; i--){
        mesg[mesg_pos] = C[i];
        mesg_pos+=1;
    }
    printf("Message: ");
    for(int j = 0; j < k; j++){
        bin(mesg[j]);
    }
    printf("\n");
    printf("Message: ");
    for(int j = 0; j < k*8; j++){
        printf("%d", bits[j]-'0');
    }
    printf("\n");
}


void main(){
    setup_tables();

    int n = 10;
    int k = 3;
    int d = n-k+1;
    int t = (n-k)/2;

    //local data to show encoding works
    int* C = encode_message(n,k,t);



    //int check = parity_check(C,C,n);

    //if(parity_check){
    //decode(C,k,n);
    //}

    //printf("Parity Check: %d\n", check);

    //Doesn't work
    //int* S_i_s = syndome_calculator_collective_sums(C, n, t);
    
    //print_arr(S_i_s, 2*t);


    encode_message(n,k,t);
}






int setup_tables(){
    unsigned int x_to_w=0; 
    unsigned int b=0; 
    unsigned int log = 0;

    x_to_w =  p+1;
    gflog = malloc(sizeof(unsigned int) * x_to_w);
    gfilog = malloc(sizeof(unsigned int) * x_to_w);

    b = 1;
    for(log = 0; log < x_to_w-1; log++){
        gflog[b] = (unsigned int)log;
        gfilog[log] = (unsigned int) b;
        b = b << 1;
        if(b & x_to_w){
            //printf("log: %d\n", log);
            //bin(b);
            //bin(prim_poly);
            //bin(b ^ prim_poly);
            b = (b ^ prim_poly) ;
            //bin(b);
        }

    }

    return 0;

}


int gf_mult(int a, int b){
    int sum_log;

    if(a == 0 || b == 0) return 0;
    sum_log = gflog[a] ^ gflog[b];
    if(sum_log >= NW-1){ 
        sum_log -= NW-1;
    }
    return gfilog[sum_log];
}

int* gf_mult_poly(int* a, int* b, int a_len, int b_len){

    int new_poly_size = a_len+b_len -1;

    int* new_poly = malloc(sizeof(int)*new_poly_size);
    
    for(int i =0; i < new_poly_size; i++){
        new_poly[i]=0;
    }

    for(int i = a_len-1; i >= 0; i--){
        for(int j = b_len-1; j >= 0; j--){
            new_poly[i+j] ^= gf_mult(a[i], b[j]);
            
        }
    }

    return new_poly;
}

int gf_div(int a, int b){
    int diff_log;

    if(a == 0) return 0;
    if(b == 0) return -1;
    diff_log = gflog[a] - gflog[b];
    if(diff_log < 0) diff_log += NW-1;
    return gfilog[diff_log];
}

int* gf_poly_add(int* a, int* b, int len, int len2){
    int max = 0;
    int check = 0;
    int min = 0;
    if(len > len2){
        max = len;
        min = len2;
    }else{
        max = len2;
        min = len;
    }

    int* poly = malloc(sizeof(int)*max+1);

    for(int i =0; i <= max; i++){
        if(i >= min){
            if(min == len2){
                poly[i] = a[i] ^ 0;
            }else{
                poly[i] = 0 ^ b[i];
            }
        }else{
            poly[i] = a[i] ^ b[i];
        }
    }
    return poly;
}

// a/b if remainder = 1 return remainder else return the divsion
int* gf_div_poly(int* a, int* b, int a_len, int b_len, int remainder){
   
    // if(a_len < b_len){
    //     if(remainder){
    //         return b;
    //     }else{
    //         return 0;
    //     }
    // }

    int range = a_len - b_len - 1;

    int* divisor = malloc(sizeof(int)*b_len);

    int* dividend = malloc(sizeof(int)*a_len);

    for(int i =0; i < b_len; i++){
        divisor[i] = b[i];
 
    }
    for(int i =0; i < a_len; i++){
        dividend[i] = a[i];
 
    }

    int* r = malloc(sizeof(int)*(a_len));

    for(int i = a_len-1; i >= range; i--){
        int coeff = gf_div(dividend[i], divisor[i]);

        if(coeff != 0){
            for(int j = b_len-2; j >= 0; j--){
                if (divisor[j] != 0){
                    //printf("a=%d  b=%d b/a=%d range=%d\n", coeff, divisor[j], gf_div(coeff, divisor[j]), range);
                    dividend[i-(b_len-1-j)] ^= gf_mult(divisor[j], coeff);
                }
            }
        }

    }



    int sep = b_len - 1;

    for(int i = 0; i <= sep; i++){
        r[i] = dividend[i];
        dividend[i] = 0;
    }

    if(remainder == 1){
        return r;
    }else{
        return dividend;
    }

}

int gf_pow(int a, int b){
    unsigned int ret = 1;
    for (int i =0; i < b; i++){
        ret = (unsigned int) gf_mult(ret, a);
    }
    return ret;
}

void bin(unsigned n) 
{ 
    unsigned i; 
    for (i = 1 << 7; i > 0; i = i / 2) 
        (n & i)? printf("1"): printf("0");
   //printf("\n"); 
} 

unsigned int str_int(char* str){
    char c = 0;
    int tmp_num = 0;
    unsigned int num = 0;
    int size = 7;

    for (int i =0; i <= size; i++){
        if(str[i] == '0'){
            tmp_num = 0;
        }else{
            tmp_num = 1;
        }
        num += (unsigned int)tmp_num*((int)pow(2, size-(i)));   
    }

    return num;
}

int gf_inverse(int num){

    for(int i = 1; i < p; i++){
        if(gf_mult(num,i) == 1){
            return i;
        }
    }

}
