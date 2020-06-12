#include "./galois.c"
#include "./headers/fuzzy.h"


int main(){
    setup_tables();
    int n = NW-1;
    int k = 9;
    int t = (n-k)/2;
    int r = 12;

    printf("t=%d\tr=%d\n", t,r);

    poly* g_x = g(t);

    int** R = lock(k,t,r,g_x);

    poly* res = unlock(R, g_x, k,t,r);

    //print_poly(res);
    
    
    return 0; 

}

//we will use bits as the data we want to encode.
int** lock(int k, int t, int r, poly* g){
    int* X = malloc(sizeof(int)*(r));
    int** R = malloc(sizeof(int*)*(r));
    poly* secret = create_secret_poly(t);
    poly* p = gf_mult_poly(secret,g);

    printf("THE SECRET IS = ");
    print_poly(secret);

    for(int i = 0; i < k; i++){
        int* tmp_list = malloc(sizeof(int)*2);
        int num = sequence_of_bits(i+1);
        if (num != -1){
            tmp_list[0] = num;
            tmp_list[1] = eval_poly(p, num);
        }else{
            printf("Sequence of bits returned a bad value.\n");
            exit(1);
        }
        X[i] = num;
        R[i] = tmp_list;
    }

    int len = t;
    int check = 1;

    time_t dd;
    srand((unsigned) time(&dd));

    for(int i = k; i < r;){
        int* tmp_list = malloc(sizeof(int)*2);
        int val = rand()%NW;
        for(int j =0; j < len && check == 1; j++){
            if(val == X[j]){
                check = 0;
            }
        }
        if(check == 1){
            len+=1;
            int check2 = 1;
            int val2 = rand()%NW;
            int real_val = eval_poly(p, val);
            while(check2){
                if(val2 != real_val){
                    tmp_list[0] = val;
                    tmp_list[1] = val2;
                    X[i] = val;
                    R[i] = tmp_list;
                    i++;
                    check2 = 0;
                }else{
                    val2 = rand()%NW;
                }
            }
        }
        check = 1;
    }

    return R;

}

poly* create_secret_poly(int k){
    poly* p = create_poly(k);
    int num =0;

    for(int i = 0; i < k; i++){
        num = sequence_of_bits(i+1);
        p->coeffs[i] = num;
    }    
    return p;

}

int** create_B(int k, int t, int r, poly* g){
    int** R = malloc(sizeof(int*)*(r));
    poly* secret = create_secret_poly(t);
    poly* p = gf_mult_poly(g,secret);
    for(int i = 0; i < r; i++){
        int* tmp_list = malloc(sizeof(int)*2);
        int num = sequence_of_bits(i+1);
        if (num != -1){
            tmp_list[0] = num;
            tmp_list[1] = eval_poly(p, num);
        }else{
            printf("Sequence of bits returned a bad value.\n");
            exit(1);
        }
        R[i] = tmp_list;
    }
    return R;

}


int sequence_of_bits(int seq_number){
    char* tmp = malloc(sizeof(char)*pow_2);
    int pos_holder = 0;
    int bits_pos = 0;
    int count = 0;
    
    for(int i = 0; i <= seq_number*pow_2; i++){
        if(count%pow_2 == 0 && count > 0){
            if( i == seq_number*pow_2){
                unsigned int tmp_h = (unsigned int)str_int(tmp);
                return tmp_h;
            }
            count = 0;
            i--; 
        }else{
            tmp[count] = bits[bits_pos];
            count+=1;
            bits_pos+=1;
        }
        
    }
    return -1;
}

poly* unlock(int** R, poly* g, int k, int t, int r){
    int** B = create_B(k,t,r,g);
    int** Q = malloc(sizeof(int*)*r);

    for(int i = 0; i < r; i++){
        for(int j = 0; j < r; j++){
            if(R[i][0] == B[j][0] && R[i][1] == B[j][1]){
                Q[i] = R[i];
                break;
            }
        } 
    }

    poly* C = Q_to_poly(k,t,r,Q);
    //RSDecode(t, C, g);
    return 0;
}

poly* Q_to_poly(int k, int t, int r, int** Q){
    int** matrix = malloc(sizeof(int*)*(r-t));

    for(int i = 0; i < r-t; i++){
        int* row = malloc(sizeof(int)* (2*t+1));
        int x = Q[i][0];
        int y = Q[i][1];

        for(int j = 0; j < 2*t+1; j++){
            if(j != 2*t){
                row[j] = gf_pow(x, j);
            }else{
                row[j] = y;
            } 
        }
        matrix[i] = row;
    }

    for(int i = 0; i < r-t; i++){
        int* tmp_row = malloc(sizeof(int)* 2*t+1);

        for(int j = 0; j < 2*t+1; j++){
            tmp_row[j] = matrix[i][j];
        }
        
        for(int j = i+1; j < 2*t+1; j++){
            int coeff = find_coeff_row_reduction(tmp_row[i], matrix[j][i]);

            if(coeff == NW){
                printf("ERROR A = %d \t B = %d\n", tmp_row[i], matrix[j][i]);
                exit(1);
            }
            
            //printf("j=%d\n", j);
            for(int k = 0; k < 2*t+1; k++){
                tmp_row[k] = gf_mult(tmp_row[k], coeff);
            }

            for(int k = 0; k < 2*t+1; k++){
                matrix[j][k] = tmp_row[k] ^ matrix[j][k];
            }

            //printf("Coeff = %d\n", coeff );
        

        }
        print_mat(matrix,  r-t, 2*t+1);
        printf("\n");
    }

    poly* errors = create_poly(2*t);
    for(int i=r-t-1; i >= 0; i--){
        int* tmp_row = matrix[i];
        if(tmp_row[2*t] != 0){
            int S_i = tmp_row[2*t];
            for(int j = errors->size-1; j >= 0; j--){
                if(errors->coeffs[j] == 0){
                    int tmp_co =0;
                    int tmp_err = 0;
                    for(int k = j; k < 2*t+1; k++){
                        if(i < j){
                            tmp_co = gf_mult(errors->coeffs[k], tmp_row[k]);
                            S_i ^= tmp_co;
                        }else{
                            int check = 1;
                            for(int l = 1; l < NW  && check == 1; l++){
                                tmp_err = gf_mult(tmp_row[k], l);
                                if(tmp_err == S_i){
                                    check =0;
                                    errors->coeffs[j] = l;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    print_poly(errors);
    print_mat(matrix, 2*t, 2*t+1);
    return 0;
}

poly* RSDecode(int t, poly* C, poly* g){
    synd* S = syndome_calculator_division(C,  g, t);

    poly* sig = berlecamp_table(S->p, S->synds);

    if(sig != 0){
        //printf("Sigma = ");
        //print_poly(sig);

        poly* s_r = sigma_r(sig);
        printf("Sigma_r = ");
        print_poly(s_r);

        poly* roots = roots_of_poly(s_r,t,NW-1);
        //printf("roots are = ");
        //print_poly(roots);

        poly* errors = error_correction(roots,  S->p);

        reassemble_message(errors, roots, C);

        poly* M2 = gf_div_poly(C, g,0);

        return M2;
    }

    return 0;

}