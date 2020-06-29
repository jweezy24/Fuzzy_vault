#include "../fuzzy.c"
#include "./headers/testing_stuff.h"
#include "./utils.c"

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

            // printf("Message is %d = ", k);
            // print_poly(M);

            //M = gf_mult_poly(M,x_2t);

            //poly* Q = gf_div_poly(M, g_x, 0);
            //poly* CK = gf_div_poly(M, g_x, 1);

            //poly* C = gf_poly_add(M,CK);
            poly* C = gf_mult_poly(M,g_x);
            poly* C2 = gf_mult_poly(M,g_x);

            points = 200;

            // printf("C(x) is = ");
            // print_poly(C);


            for(int i = 0; i < C->size; i++){
                //5% chance of error
                //printf("%d\n", rand());
                if((rand()%100) > 100-percent){
                    unsigned int val = rand()%(int)pow(2,pow_2);
                    C->coeffs[i] = (unsigned int)C->coeffs[i] ^ val;
                    // printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", i, val);
                    errors+=bin_num(val);
                    
                }
            }



            int** R = lock(k,t,r,C);

            // Adds error to points
            
            // for(int i = 0; i < NW; i++){
            //     //5% chance of error
            //     //printf("%d\n", rand());
            //     if(rand()%200 > 190 &&  errors <= t){
            //     int* tmp_row = R[i];
            //     for(int j = 0; j < 2; j++){
            //             unsigned int val = rand()%(int)pow(2,pow_2);
            //             if( bin_num(val) + errors <= t){
            //                 if(val%2){
            //                     tmp_row[1] = (unsigned int)tmp_row[1] ^ val;
            //                     printf("ERROR PUT IN Y %d ERROR OFF BY %d\n", i, val);
            //                     errors+=bin_num(val);
            //                 }else{
            //                     tmp_row[0] = (unsigned int)tmp_row[0] ^ val;
            //                     printf("ERROR PUT IN X %d ERROR OFF BY %d\n", i, val);
            //                     errors+=bin_num(val);
            //                 }
            //             }
            //         }
            //     }
            // }


            // for(int i = 0; i < C->size; i++){
            //     //5% chance of error
            //     //printf("%d\n", rand());
            //     if(rand()%200 > 100 &&  errors <= (k+t)/2){
            //         unsigned int val = rand()%(int)pow(2,pow_2);
            //         if( bin_num(val) + errors <= (k+t)/2){
            //             C->coeffs[i] = (unsigned int)C->coeffs[i] ^ val;
            //             printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", i, val);
            //             errors+=bin_num(val);
            //         }
            //     }
            // }

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