#include "../fuzzy.c"
#include "./headers/testing_stuff.h"

int main(){
    setup_tables();
    int n = 60;
    int k = 25;
    int t = (n-k)/2;
    int r = 50;


    percent = per*10;

    //int streams = (int)(29478361/8);
    int runs = 10000;
    int count_c = 0;
    stream_count_layers[0] = 0;
    stream_count_layers[1] = 0;


    poly* g_x = g(t);

    poly* M = m_(n, k , t, stream_count_layers[0]);
    
    int errors = 0;

    poly* x_2t = create_poly(2*t+1);
    x_2t->coeffs[2*t] = 1;

    points = 100;

    for(int l = 0; l < M->size; l++){
        //5% chance of error
        //printf("%d\n", rand());
        for(int j = 0; j < pow_2; j++){
            int tmp_num = rand()%100;
            //printf("Random = %d\n", tmp_num);
            if( tmp_num > 100-percent){
                unsigned int val = (int)pow(2,j); 
                //bin(val);
                //printf("\n");
                M->coeffs[l] = (unsigned int)M->coeffs[l] ^ val;
                // printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", i, val); 
                errors+=bin_num(val);
                //printf("Errors = %d\n", errors);
                
            }
        }
    }

    poly* C = gf_mult_poly(M,g_x);

}