#include "./headers/fuzzy.h"

int points =0;

//we will use bits as the data we want to encode.
int** lock(int k, int t, int r, poly* p){
    int* X = malloc(sizeof(int)*(NW));
    int** R = malloc(sizeof(int*)*(NW));

    //printf("THE SECRET IS = ");
    //print_poly(p);

    for(int i = 1; i < points+1; i++){
        int* tmp_list = malloc(sizeof(int)*2);
        int num = gf_pow(generator, i);
        if (num != -1 && eval_poly(p, num) != 0){
            //printf("x = %d\n", num);
            tmp_list[0] = num;
            tmp_list[1] = eval_poly(p, num);
        }
        X[i-1] = num;
        R[i-1] = tmp_list;
    }

    int len = points;
    int check = 1;

    time_t dd;
    srand((unsigned) time(&dd));

    for(int i = points; i < NW;){
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

int** create_B(int k, int t, int r, poly* p){
    int** R = malloc(sizeof(int*)*(points));

    for(int i = 1; i < points+1; i++){
        int* tmp_list = malloc(sizeof(int)*2);
        int num = gf_pow(generator, i);
        if (num != -1){
            tmp_list[0] = num;
            tmp_list[1] = eval_poly(p, num);
        }else{
            printf("Sequence of bits returned a bad value.\n");
            exit(1);
        }
        R[i-1] = tmp_list;
    }
    return R;

}


int sequence_of_bits(int seq_number){
    char* tmp = malloc(sizeof(char)*pow_2);
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

poly* unlock(int** R, poly* g, poly* C2, int k, int t, int r){
    int** B = create_B(k,t,r,C2);
    int** Q = malloc(sizeof(int*)*(NW));
    int q_size = 0;

    for(int i = 0; i < points; i++){
        int* tmp_row = R[i];
        for(int j = 0; j < points; j++){
            if(tmp_row[0] == B[j][0]){
                Q[q_size] = tmp_row;
                q_size+=1;
            }
            
        } 
    }

    //printf("Q size = %d\n",q_size);
    poly* C = Q_to_poly(q_size, k, t,r,Q);
    //printf("\n NEW C(x) = \n");
    //print_poly(C);
    RSDecode(t, C, g);
    return C;
}

poly* Q_to_poly(int points, int k, int t, int r, int** Q){
    
    mat* matrix =0;
    if(points > 100){
        matrix = create_matrix(100, 101);
    }else{
        printf("NOT ENOUGH POINTS TO RECREATE MESSAGE.\n");
        return 0;
    }

    for(int i = 0; i < matrix->rows; i++){
        int* row = malloc(sizeof(int)* (matrix->cols));
        int x = Q[i][0];
        int y = Q[i][1];

        for(int j = 0; j < matrix->cols; j++){
            if(j != matrix->cols-1){
                row[j] = gf_pow(x, j);
            }else{
                row[j] = y;
            } 
        }
        matrix->matrix[i] = row;
    }

    poly* p = gauss_elim(matrix);


    //print_matrix(matrix);
    return p;
}

poly* RSDecode(int t, poly* C, poly* g){
    synd* S = syndome_calculator_division(C,  g, t);

    poly* sig = berlecamp_table(S->p, S->synds);

    if(sig != 0){
        //printf("Sigma = ");
        //print_poly(sig);

        poly* s_r = sigma_r(sig);
        //printf("Sigma_r = ");
        //print_poly(s_r);

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
