#include "../fuzzy.c"
#include "./headers/testing_stuff.h"

int main(){
    setup_tables();
    int n = start_size;
    int k = start_message;
    //int t = ((n-k)/2);
    int t = ((n-k)/2);
    int r = 128;
    char* file_path = "./src/tests/data/results_noise_burst.txt";
    poly** g_s = malloc(sizeof(poly*)*8);
    poly** noise_good = malloc(sizeof(poly*)*8);
    poly** noise_bad = malloc(sizeof(poly*)*8);


    for(int n_size = 1; n_size < pow_2+1; n_size++){
        for(int per = 1; per <= 100; per++){

            poly* n_1;
            poly* n_2;
            
            poly* g_x = g(t);
            percent = per;
            int noise_size = n_size;
            int count_c = 0;
            //int streams = (int)(29478361/(n*noise_size));
            int streams = (int)(1000);
            stream_count_layers[0] = 0;
            stream_count_layers[1] = 0;

            for(int i = 0; i< streams; i++){

                poly* g_x = g(t);
                poly* n_1 = m_(n, noise_size, t, stream_count_layers[0], 0);
                poly* n_2 = m_(n, noise_size, t, stream_count_layers[1], 1);
                resize_poly(n_1);
                resize_poly(n_2);            
                poly* M = m_random_message(k);
                poly* C2;

                C2 = gf_mult_poly(M, g_x);

                int errors = 0;

                //FUZZY VAULT
                //poly* C2 = gf_mult_poly(n_1,g_x);
                
                //OURS
                //poly* C2 = gf_mult_poly(M, g_s[7]);

                points = 128;
                
                for(int l = 0; l < n_2->size; l++){
                    //5% chance of error
                    //printf("%d\n", rand());
                    //for(int j = 0; j < noise_size; j++){
                    int tmp_num = abs(randombytes_random())%100;
                    //printf("Random = %d\n", tmp_num);
                    if( tmp_num >= 100-percent){
                        unsigned int val = abs(randombytes_random())%256;
                        //bin(val);
                        //printf("\n");
                        n_2->coeffs[l] = (unsigned int)n_2->coeffs[l] ^ val;
                        //printf("ERROR PUT IN LAYER %d ERROR AT %d WITH VAL %u\n", layer, l, val); 
                        errors+=1;
                        //printf("Errors = %d\n", errors);
                        
                    }
                    //}
                }

                //FUZZY VAULT
                //poly* C = gf_mult_poly(g_x, n_2);
                
                poly* C;
                C = gf_mult_poly(M, g_x);
                C = gf_poly_add(C, n_2);

                printf("C = ");
                print_poly(C);



                //int* R = lock(k,t,r,C);

                //printf("THERE ARE %d BIT ERRORS\n", errors);
                
                //FUZZY VAULT DECODING
                //poly* res =  unlock(R, g_x, C2, k, t, r);


                poly*  res = RSDecode(t, C, n_1, g_x);
                if(res == 0){
                    printf("***************FAILURE****************\n"); 
                    // printf("\nC = ");
                    // print_poly(C);
                    // printf("\nCorrected Poly = 0\n");
                    // printf("\nC2 = ");
                    // print_poly(C2);
                    // exit(0);
                    count_c+=1;
                }else{
                // printf("\nC = ");
                // print_poly(C);
                // printf("\nCorrected Poly = ");
                // print_poly(res);
                // printf("\nC2 = ");
                // print_poly(C2);

                int correct = poly_eq(M, res);
                    if(correct) {
                        printf("***************CORRECT****************\n");
                        // printf("C = ");
                        // print_poly(C); 
                        // // printf("\nn_1 = ");
                        // // print_poly(n_1);
                        // // printf("\nn_2 = ");
                        // // print_poly(n_2);
                        // printf("\nCorrected Poly = ");
                        // print_poly(res);
                        // printf("\nMessage = ");
                        // print_poly(M); 
                        //exit(1);
                    }else{
                        count_c +=1; 
                        printf("***************FAILURE****************\n");
                        // printf("G = ");
                        // print_poly(g_x);
                        // printf("C2 = ");
                        // print_poly(C2);
                        // printf("C = ");
                        // print_poly(C);
                        // // printf("\nn_1 = ");
                        // // print_poly(n_1);
                        // // printf("\nn_2 = ");
                        // // print_poly(n_2);
                        // printf("\nCorrected Poly = ");
                        // print_poly(res);
                        // printf("\nMessage = ");
                        // print_poly(M); 
                        // printf("THERE ARE %d BIT ERRORS\n", errors);
                        // printf("Stream_count A = %d\n", stream_count_layers[0]);
                        // printf("Stream_count B = %d\n", stream_count_layers[1]);/*exit(1);*/
                    
                        // exit(1);
                    }
                }
                if(stream_count_layers[0] > stream_count_layers[1]) stream_count_layers[1] = stream_count_layers[0];
                if(stream_count_layers[1] >= stream_count_layers[0]) stream_count_layers[0] = stream_count_layers[1];
                //free_poly(n_1);
                //free_poly(n_2);
                free_poly(M);
                free_poly(C);
                free_poly(C2);
                if(res != 0) free_poly(res);
            }

            //log_st("");

            array_c* number2 = int_to_str(streams);
            array_c* number = int_to_str(count_c);
            array_c* frac = make_fraction(number, number2);
            log_final_line_noise(frac, percent, noise_size, file_path);
            free(number->buf);
            free(number2->buf);
            free(frac->buf);
            free_poly(g_x);

            free(number);
            free(number2);
            free(frac);
            

        }
    }
    
    return 0; 

}


void log_results(int stream_num, int correct){
   FILE *fp;

   fp = fopen("./results.txt", "a");
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

void log_final_line_noise(array_c* frac, int percent, int noise_size, char* file_path){
    FILE *fp;

   fp = fopen(file_path, "a");
   fprintf(fp, "Ratio\t %s Percent\t %d\t Noise %d\n", frac->buf,percent, noise_size);

   fclose(fp);
}

void log_st(array_c* stream_num){
    FILE *fp;

    fp = fopen("./results.txt", "a");
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