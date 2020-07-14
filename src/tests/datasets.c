#include "../fuzzy.c"
#include "./headers/testing_stuff.h"
#include <time.h>

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

char* file_path = "./src/tests/data/results_hum.txt";




int main(){
    setup_tables();
    int n = 60;
    int k = 25;
    int t = (n-k)/2;
    int r = 50;
    char* bits_acc = convert_acc_to_bits(hum, size_of_hum);

    for(int per = 0; per <= 100; per++){

        percent = per;

        //int streams = (int)(29478361/8);
        
        int noise_size = 3; 
        int runs = 10000;
        int count_c = 0;
        stream_count_layers[0] = 0;
        stream_count_layers[1] = 0;

        for(int i = 0; i < runs; i++){

            poly* g_x = g(t);
            poly* n_1 = m_arbitrary(n, noise_size, t, stream_count_layers[0], bits_acc, 0);
            poly* n_2 = m_arbitrary(n, noise_size, t, stream_count_layers[1], bits_acc, 1);
            resize_poly(n_1);
            resize_poly(n_2);            
            poly* M = m_random_message(k);

            
            int errors = 0;

            poly* x_2t = create_poly(2*t+1);
            x_2t->coeffs[2*t] = 1;

            poly* C2 = gf_mult_poly(M,g_x);

            points = 100;

            for(int l = 0; l < n_2->size; l++){
                //5% chance of error
                //printf("%d\n", rand());
                for(int j = 0; j < noise_size; j++){
                    int tmp_num = abs(randombytes_random())%100;
                    //printf("Random = %d\n", tmp_num);
                    if( tmp_num >= 100-percent){
                        unsigned int val = (int)pow(2,j);
                        //bin(val);
                        //printf("\n");
                        n_2->coeffs[l] = (unsigned int)n_2->coeffs[l] ^ val;
                        //printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", l, val); 
                        errors+=bin_num(val);
                        //printf("Errors = %d\n", errors);
                        
                    }
                }
            }

            poly* C = gf_poly_add(C2, n_2);

            // printf("C = ");
            // print_poly(C);

            // printf("C2 = ");
            // print_poly(C2);


            //int* R = lock(k,t,r,C);

            // printf("THERE ARE %d BIT ERRORS\n", errors);


            poly*  res = RSDecode(t, C, n_1, g_x);
            if(res == 0){
                printf("***************FAILURE****************\n"); 
                // printf("\nC = ");
                // print_poly(C);
                // printf("\nCorrected Poly = 0\n");
                // printf("\nC2 = ");
                // print_poly(C2);
                //exit(0);
                count_c+=1;
            }else{
            // printf("\nC = ");
            // print_poly(C);
            // printf("\nCorrected Poly = ");
            // print_poly(res);
            // printf("\nC2 = ");
            // print_poly(C2);

            int correct = poly_eq(M, res);
            if(correct) {printf("***************CORRECT****************\n"); }
            if (correct == 0) {
                count_c +=1; 
                printf("***************FAILURE****************\n");
                // printf("\nC = ");
                // print_poly(C);
                // printf("\nCorrected Poly = ");
                // print_poly(res);
                // printf("\nC2 = ");
                // print_poly(C2);
                }
            printf("Stream_count A = %d\n", stream_count_layers[0]);
            printf("Stream_count B = %d\n", stream_count_layers[1]);
            if(stream_count_layers[0] > stream_count_layers[1]) stream_count_layers[1] = stream_count_layers[0];
            if(stream_count_layers[1] >= stream_count_layers[0]) stream_count_layers[0] = stream_count_layers[1];
            }
        }

        //log_st("");

        array_c* number2 = int_to_str(runs);
        array_c* number = int_to_str(count_c);
        array_c* frac = make_fraction(number, number2);
        log_final_line(frac, percent);
        

    }
    
    return 0; 

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

poly* m_arbitrary(int n, int k, int t, int bits_start, char* bits, int tracker){
    poly* M = create_poly(n);
    char* tmp = malloc(sizeof(char)*k);
    int pos_holder = 0;
    int bits_pos = pow_2*bits_start;
    int count = 0;


    for(int i =0; i <= n*k; i++){
        if(count%k == 0 && count > 0){
            unsigned int tmp_h = (unsigned int)str_int(tmp);
            M->coeffs[pos_holder] = tmp_h;
            pos_holder++;
            count = 0;
            i--;
            if(tracker == 0) {
                stream_count_layers[0]+=1;
            }else {
                stream_count_layers[1]+=1;
            } 
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



