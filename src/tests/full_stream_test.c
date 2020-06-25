#include "../fuzzy.c"
#include "./headers/testing_stuff.h"

int main(){
    setup_tables();
    int n = 100;
    int k = 25;
    int t = (n-k)/2;
    //int d = n-k+1;
    int r = 50;

    for(int per = 0; per <= 10; per++){

        int percent = per*10;

        //int streams = (int)(29478361/8);
        int streams = 10000;
        int count_c = 0;

        for(int i = 0; i < streams; i++){

            poly* g_x = g(t);

            poly* M = m_(n, k , t, i);
            
            int errors = 0;

            poly* x_2t = create_poly(2*t+1);
            x_2t->coeffs[2*t] = 1;

            poly* C = gf_mult_poly(M,g_x);
            poly* C2 = gf_mult_poly(M,g_x);

            points = 200;

            for(int i = 0; i < C->size; i++){
                //5% chance of error
                //printf("%d\n", rand());
                if((rand()%100) > 100-percent){
                    unsigned int val = (rand()%(int)pow(2,pow_2)) + 1;
                    if(val >= (int)pow(2,pow_2) ) val = 1;
                    C->coeffs[i] = (unsigned int)C->coeffs[i] ^ val;
                    // printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", i, val);
                    errors+=bin_num(val);
                    
                }
            }



            int** R = lock(k,t,r,C);

            //printf("THERE ARE %d BIT ERRORS\n", errors);

            poly*  res = unlock(R, g_x, C2, k,t,r);
            if(res == 0) exit(1);
            // printf("\nNEW POLY = ");
            // print_poly(res);
            // printf("\nOLD POLY = ");
            // print_poly(C);

            int correct = poly_eq(res, C);
            if (correct == 0) count_c +=1;
        }

        //log_st("");

        array_c* number2 = int_to_str(streams);
        array_c* number = int_to_str(count_c);
        array_c* frac = make_fraction(number, number2);
        log_final_line(frac, percent);

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

void log_final_line(array_c* frac, int percent){
    FILE *fp;

   fp = fopen("./results.txt", "a");
   fprintf(fp, "Ratio\t %s Percent\t %d\n", frac->buf,percent);

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