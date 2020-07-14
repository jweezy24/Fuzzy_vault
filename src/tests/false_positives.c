#include "../fuzzy.c"
#include "./headers/testing_stuff.h"

extern float acc[];
extern float bar[];
extern float ble[];
extern float gry[];
extern float hum[];
extern float lux[];
extern float mag[];
extern float tmp[];
extern float wifi[];

int size_of_acc = 1295808;
int size_of_bar = 1255200;
int size_of_tmp = 1272408;
int size_of_gry = 1295808;
int size_of_lux = 1295736;
int size_of_hum = 1272408;
int size_of_mag = 1295808;
int size_of_wifi = 53088;
int size_of_ble = 34752;

char* file_path = "./results_FP_G_acc.txt";

int main(){
    setup_tables();
    int n = 60;
    int k = 25;
    int t = (n-k)/2;
    int r = 50;
    
    stream_count_layers[0] = 0;
    stream_count_layers[1] = 0;


    for(int beg = 0; beg < 1000; beg++){
        int message_seq_start = k*beg;

        poly* M = m_(n, k , t, message_seq_start*k);
        percent = 0;
        poly* g_x = g(t);
        poly* C2 = gf_mult_poly(M,g_x);

        

        int streams = (int)((29478361)/(8*k));
        int runs = streams;
        int count_c = 0;

        for(int i = 0; i < streams/8; i++){

            int start_pos = k*i;

            
            int errors = 0;


            points = 100;

            poly* MM = generate_key_with_wall( k,  n, t, g_x, start_pos);

            // printf("C = ");
            // print_poly(C);

            // printf("C2 = ");
            // print_poly(C2);


            //int* R = lock(k,t,r,C);

            // printf("THERE ARE %d BIT ERRORS\n", errors);


            poly*  res = RSDecode(t, MM, C2, g_x);
            if(res == 0){
                printf("***************FAILURE****************\n"); 
                count_c+=1;
            }else{

            int correct = poly_eq(C2, res);
            if(correct) {printf("***************CORRECT****************\n"); }
            if (correct == 0) {
                count_c +=1; 
                printf("***************FAILURE****************\n");
                }
            }
            free(MM);
            free(res);
        }

        //log_st("");

        array_c* number2 = int_to_str(runs);
        array_c* number = int_to_str(count_c);
        array_c* frac = make_fraction(number, number2);
        log_final_line(frac, message_seq_start);
        free(M);
        free(g_x);
        free(C2);
        }
        

    
    
    return 0; 

}
/* Brute force methods that are purely random*/
poly* generate_key(int n){
    poly* M = create_poly(n);
    char* tmp = malloc(sizeof(char)*pow_2);
    int count = 0;

    for(int i =0; i < n ; i++){
        int coeff =  abs(randombytes_random()) %256;
        M->coeffs[i] = coeff;
        
    }

    // resize_poly(M);    
    free(tmp);
    return M;
}

poly* generate_key_g(int k, poly* g){
    poly* M = create_poly(k);
    char* tmp = malloc(sizeof(char)*pow_2);
    int count = 0;

    for(int i =0; i < k ; i++){
        int coeff = abs(randombytes_random()) %256;
        //printf("R-Val = %d\n", coeff);
        M->coeffs[i] = coeff;
    }

    poly* ret = gf_mult_poly(M, g);
    // resize_poly(M);    
    free(tmp);
    free(M);
    return ret;
}

poly* generate_key_with_wall(int k, int n, int t, poly* g, int start_pos){
    char* tmp = convert_acc_to_bits(acc, size_of_acc);
    poly* M = m_arbitrary(n, k , t, start_pos, bits);

    poly* ret = gf_mult_poly(M, g);
    // resize_poly(M);    
    free(M);
    return ret;
}


poly* m_arbitrary(int n, int k, int t, int bits_start, char* bits){
    poly* M = create_poly(k);
    char* tmp = malloc(sizeof(char)*pow_2);
    int pos_holder = 0;
    int bits_pos = pow_2*bits_start;
    int count = 0;


    for(int i =0; i <= k*pow_2; i++){
        if(count%pow_2 == 0 && count > 0){
            unsigned int tmp_h = (unsigned int)str_int(tmp);
            M->coeffs[pos_holder] = tmp_h;
            pos_holder++;
            count = 0;
            i--;
            stream_count_layers[0]+=1;
            stream_count_layers[1]+=1; 
        }else{
            tmp[count] = bits[bits_pos];
            count+=1;
            bits_pos+=1;
        }
        
    }

    // resize_poly(M);    
    free(tmp);
    return M;
}



char* convert_acc_to_bits(float* arr, int size){
    //printf("SIZE = %d", (int)(size_of_acc/10));
    int samples_per_bit = 10;
    char* bits_acc = malloc(sizeof(char) * (int)(size/10));
    int average = 0;
    int pos = 0;

    for(int i =0; i < size; i++){
        average += arr[i];
        if(i != 0 && i%samples_per_bit == 0){
            if(abs(arr[i-samples_per_bit]) > abs(average/samples_per_bit)){
                bits_acc[pos] = '1';
            }else{
                bits_acc[pos] = '0';
            }
            pos+=1;
            average=0;
        }
    }
    return bits_acc; 
}


void log_results(int stream_num, int correct){
   FILE *fp;

   fp = fopen(file_path, "a");
   if(correct){
        fprintf(fp, "Correct\t Stream=%d\n", stream_num);
    }else{
        fprintf(fp, "Failure\t Stream=%d\n", stream_num);
    }

   fclose(fp);

}

void log_final_line(array_c* frac, int percent){
    FILE *fp;

   fp = fopen(file_path, "a");
   fprintf(fp, "Ratio\t %s Percent\t %d\n", frac->buf,percent);

   fclose(fp);
}

void log_st(array_c* stream_num){
    FILE *fp;

    fp = fopen(file_path, "a");
    fprintf(fp, "%s", stream_num->buf);

    printf("\n");

    fclose(fp);

}

array_c* int_to_str(int num){
    int tmp_count = 0;
    int n = num;
    while(n >= 10){
        tmp_count+=1;
        n = (int)(n/10);
    }
    if(num < 10){
        tmp_count = 0; 
    }

    //printf("Size of array = %d\n", tmp_count);

    array_c* buffer = malloc(sizeof(array_c));

    buffer->buf = (char*)malloc(sizeof(char)*(tmp_count+2));
    buffer->size = tmp_count;

    for(int i = 0; i <= tmp_count; i++){
        int digit = (int)(((float)num)/pow(10,tmp_count - i));
        buffer->buf[i] = (char)('0' + digit);
        num = (int)num - (int)(digit*pow(10,tmp_count - i));
    }
    buffer->buf[tmp_count+1] = '\0';
    return buffer;
}

array_c* make_fraction(array_c* num1, array_c* num2){
    array_c* ret = malloc(sizeof(array_c));
    ret->buf = malloc(sizeof(char)* (num1->size+num2->size+4) );
    ret->size = num1->size+num2->size+1;
    int count = 0; 
    for(int i = 0; i <= num1->size; i++){
        ret->buf[count] = num1->buf[i];
        count+=1;
    }
    ret->buf[count] = '/';
    count+=1; 
    for(int i = 0; i <= num2->size; i++){
        ret->buf[count] = num2->buf[i];
        count+=1;
    }
    ret->buf[count] = '\0';
    return ret;

}


