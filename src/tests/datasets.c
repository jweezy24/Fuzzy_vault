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

char* file_path = "./src/tests/data/results_acc.txt";


int main(){
    pthread_t tid[9];
    int rc;
    for(int i = 0; i < 9; i++){
        args *stuff = init_args(i);
        rc = pthread_create(&tid[i], NULL, run, (void*)stuff);
        pthread_join(tid[i], NULL);
        if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       } 
    }
    pthread_exit(NULL);
    return 0;
}

args* init_args(int iter){
    args* ret = malloc(sizeof(args));
    switch(iter){
        case 0:
            ret->bits = acc;
            ret->size = size_of_acc;
            ret->file_path = "./src/tests/data/results_acc.txt";
            break;
        case 1:
            ret->bits = bar;
            ret->size = size_of_bar;
            ret->file_path = "./src/tests/data/results_bar.txt";
            break;
        case 2:
            ret->bits = ble;
            ret->size = size_of_ble;
            ret->file_path = "./src/tests/data/results_ble.txt";
            break;
        case 3:
            ret->bits = gry;
            ret->size = size_of_gry;
            ret->file_path = "./src/tests/data/results_gry.txt";
            break;
        case 4:
            ret->bits = hum;
            ret->size = size_of_hum;
            ret->file_path = "./src/tests/data/results_hum.txt";
            break;
        case 5:
            ret->bits = lux;
            ret->size = size_of_lux;
            ret->file_path = "./src/tests/data/results_lux.txt";
            break;
        case 6:
            ret->bits = mag;
            ret->size = size_of_mag;
            ret->file_path = "./src/tests/data/results_mag.txt";
            break;
        case 7:
            ret->bits = tmp;
            ret->size = size_of_tmp;
            ret->file_path = "./src/tests/data/results_tmp.txt";
            break;
        case 8:
            ret->bits = wifi;
            ret->size = size_of_wifi;
            ret->file_path = "./src/tests/data/results_wifi.txt";
            break;
    }
    return ret;
}

void *run(void* shit){
    setup_tables();
    int stream_count_layers_local[2] = {0,0};
    args* stuff = (args*)shit;
    int n = 100;
    int k = 25;
    int t = (n-k)/2;
    int r = 50;
    char* bits_acc = convert_acc_to_bits(stuff->bits, stuff->size);

    for(int per = 0; per <= 100; per++){

        percent = per;

        //int streams = (int)(29478361/8);
        
        int noise_size = 3; 
        int runs = stuff->size/(n*noise_size*10);
        int count_c = 0;
        stream_count_layers_local[0] = 0;
        stream_count_layers_local[1] = 0;
        poly* g_x = g(t);

        for(int i = 0; i < runs; i++){

            poly* n_1 = m_arbitrary(n, noise_size, t, stream_count_layers_local, bits_acc, 0);
            poly* n_2 = m_arbitrary(n, noise_size, t, stream_count_layers_local, bits_acc, 1);
            // resize_poly(n_1);
            // resize_poly(n_2);            
            poly* M = m_random_message(k);

            
            int errors = 0;

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

            //printf("THERE ARE %d BIT ERRORS\n", errors);


            poly*  res = RSDecode(t, C, n_1, g_x);
            if(res == 0){
                //printf("***************FAILURE****************\n"); 
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
            //if(correct) {printf("***************CORRECT****************\n"); }
            if (correct == 0) {
                count_c +=1; 
                //printf("***************FAILURE****************\n");
                // printf("\nC = ");
                // print_poly(C);
                // printf("\nCorrected Poly = ");
                // print_poly(res);
                // printf("\nC2 = ");
                // print_poly(C2);
                }
            //printf("Stream_count A = %d\n", stream_count_layers[0]);
            //printf("Stream_count B = %d\n", stream_count_layers[1]);
            if(stream_count_layers_local[0] > stream_count_layers_local[1]) stream_count_layers_local[1] = stream_count_layers_local[0];
            if(stream_count_layers_local[1] >= stream_count_layers_local[0]) stream_count_layers_local[0] = stream_count_layers_local[1];
            }
            free_poly(n_1);
            free_poly(n_2);
            free_poly(M);
            free_poly(C);
            free_poly(C2);
            free_poly(res);
        }

        //log_st("");

        array_c* number2 = int_to_str(runs);
        array_c* number = int_to_str(count_c);
        array_c* frac = make_fraction(number, number2);
        log_final_line(frac, percent,stuff->file_path);

        free(number->buf);
        free(number2->buf);
        free(frac->buf);

        free(number);
        free(number2);
        free(frac);
        free(g_x);
        
    }
    
    pthread_exit(NULL); 

}

char* convert_acc_to_bits(float* arr, int size){
    //printf("SIZE = %d", (int)(size_of_acc/10));
    int samples_per_bit = 10;
    char* bits_acc = malloc(sizeof(char) * (int)(size/10));
    float average = 0;
    int pos = 0;

    for(int i =0; i < size; i++){
        average += arr[i];
        if(i != 0 && (i%samples_per_bit == 0)){
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

poly* m_arbitrary(int n, int k, int t, int* iter, char* bits, int tracker){
    poly* M = create_poly(n);
    char* tmp = malloc(sizeof(char)*k);
    int bits_start = iter[tracker];
    int pos_holder = 0;
    int bits_pos = k*bits_start;
    int count = 0;


    for(int i =0; i <= n*k; i++){
        if(count%k == 0 && count > 0){
            unsigned int tmp_h = (unsigned int)str_int(tmp);
            M->coeffs[pos_holder] = tmp_h;
            pos_holder++;
            count = 0;
            i--;
            iter[tracker] += 1;
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

void log_final_line(array_c* frac, int percent, char* file_path){
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



