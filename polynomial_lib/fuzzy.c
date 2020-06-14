#include "./headers/fuzzy.h"


int main(){
    setup_tables();
    int n = NW-1;
    int k = 9;
    //int d = n-k+1;
    int t = (n-k)/2;
    int r = t + k;

    // printf("Generator = %d\n", generator);


    time_t dd;
    srand((unsigned) time(&dd));

    poly* g_x = g(t);

    poly* M = m_(n, t , t);

    poly* x_2t = create_poly(2*t+1);
    x_2t->coeffs[2*t] = 1;

    //M = gf_mult_poly(M,x_2t);

    //poly* Q = gf_div_poly(M, g_x, 0);
    //poly* CK = gf_div_poly(M, g_x, 1);

    //poly* C = gf_poly_add(M,CK);
    poly* C = gf_mult_poly(M,g_x);


    int** R = lock(k,t,r,M);

    unlock(R, M, k,t,r);

    //print_poly(res);
    
    
    return 0; 

}

//we will use bits as the data we want to encode.
int** lock(int k, int t, int r, poly* p){
    int* X = malloc(sizeof(int)*(r));
    int** R = malloc(sizeof(int*)*(r));

    printf("THE SECRET IS = ");
    print_poly(p);

    for(int i = 0; i < k; i++){
        int* tmp_list = malloc(sizeof(int)*2);
        int num = gf_pow(generator, i);
        if (num != -1){
            tmp_list[0] = num;
            printf("eval_poly = %d\n",eval_poly(p, num));
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

mat* create_matrix(int rows, int cols){
    mat* ret = malloc(sizeof(mat));
    ret->matrix = malloc(sizeof(int*)*rows);
    ret->rows = rows;
    ret->cols = cols;
    return ret;
}

int** create_B(int k, int t, int r, poly* p){
    int** R = malloc(sizeof(int*)*(r+t));

    for(int i = 0; i < r+t; i++){
        int* tmp_list = malloc(sizeof(int)*2);
        int num = gf_pow(generator, i);
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

poly* unlock(int** R, poly* p, int k, int t, int r){
    int** B = create_B(k,t,r,p);
    int** Q = malloc(sizeof(int*)*(r));
    int q_size = 0;

    for(int i = 0; i < r; i++){
        int* tmp_row = R[i];
        for(int j = 0; j < r; j++){
            if(tmp_row[0] == B[j][0] && tmp_row[1] == B[j][1] && rand()%100 < 95){
                Q[q_size] = tmp_row;
                q_size+=1;
            }
            
        } 
    }

    poly* C = Q_to_poly(q_size,t,r,Q);
    //RSDecode(t, C, g);
    return C;
}

poly* Q_to_poly(int k, int t, int r, int** Q){
    mat* matrix = create_matrix(k, t+1);


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

   gauss_elim(matrix);


    print_matrix(matrix);
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




void print_matrix(mat* matrix){
    for(int i = 0; i < matrix->rows; i++){
        for(int j = 0; j < matrix->cols; j++){
            printf("%d \t", matrix->matrix[i][j]);
        }
        printf("\n");
    }
}