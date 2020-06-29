#include "./headers/fuzzy.h"

unsigned int irrs[5] = {0b10001001, 0b10001111, 0b10011101, 0b10111111, 0b11001011};


//we will use bits as the data we want to encode.
int* lock(int k, int t, int r, poly* p){
    int* X = malloc(sizeof(int)*(NW));
    int* R = malloc(sizeof(int)*(NW));

    for(int i = 0; i < NW; i++){
        X[i] = 0;
        R[i] = -1;
    }

    //printf("THE SECRET IS = ");
    //print_poly(p);
    int rotation = 0;
    int raised = 1;
    printf("Stream_count Start1 = %d\n", stream_count_layers[1]);
    stream_count_layers[1] = stream_count_layers[0];
    int range[3] = {0,0,0};
    int stream_count = stream_count_layers[0];
    int zeros = 0;
    
    for(int i = 0; i < points;){
        int num = gf_pow(sequence_of_bits(stream_count,percent), raised%NW);
        // int num = gf_pow(generator, i);
        //printf("in_range = %d\n", in_range);
        if (num != -1 && R[num] == -1 && X[num] == 0){
            //printf("NUM = %d\n", num);
            int in_range = check_range(range, num);
            if(in_range){
                X[num] = 1;
                R[num] = eval_poly(p, num);
                i++;
            }
        }
        raised +=1;
        rotation+=1;
        stream_count+=1;
    }
    stream_count_layers[0] = stream_count;
    printf("Stream_count END1 = %d\n", stream_count_layers[0]);

    int len = points;
    int check = 1;

    time_t dd;
    srand((unsigned) time(&dd));

    for(int i = points; i < NW;i++){
        if(R[i] == -1){
            int val = rand()%NW;
            R[i] = val;
        }
    }
    return R;

}

poly* create_secret_poly(int k){
    poly* p = create_poly(k);
    int num =0;

    for(int i = 0; i < k; i++){
        num = sequence_of_bits(i+1, percent);
        p->coeffs[i] = num;
    }    
    return p;

}

int* create_B(int k, int t, int r, poly* p){
    int* R = malloc(sizeof(int)*(NW));
    int* X = malloc(sizeof(int)*(NW));

    for(int i =0; i < NW; i++){
        R[i] = -1;
        X[i] = 0;
    }

    int rotation = 0;
    int raised = 1;
    
    int stream_count = stream_count_layers[1];

    printf("Stream_count Start2 = %d\n", stream_count_layers[1]);

    int range[3] = {0,0,0};
    int zeros = 0;

    for(int i = 0; i < points;){
        int num = gf_pow( sequence_of_bits(stream_count,0), raised%NW);

        if (num != -1 && R[num] == -1 && X[num] == 0){
            //printf("NUM = %d\n", num);
            int in_range = check_range(range, num);
            if(in_range){
                X[num] = 1;
                R[num] = eval_poly(p, num);
                i++;
            }
        }
        //printf("Num=%d\n", num);
        raised+=1;
        rotation+=1;
        stream_count+=1;

    }

    stream_count_layers[1] = stream_count;
    printf("Stream_count END2 = %d\n", stream_count_layers[1]);
    printf("Range = %d\t %d\t %d\n", range[0], range[1], range[2]);

    return R;

}

int check_range(int* range, int num){
    int min = 10000000;
    int min_pos = 0;
    for(int i = 0; i < 3; i++){
        if(min > range[i]){
            min_pos = i+1;
            min = range[i];
        }
    }
    for(int i = 0; i < 3; i++){
        if(min > range[i]){
            min_pos = i+1;
            min = range[i];
        }
    }

    int ret = 0;
    //printf("%d\t %d\t %d\n",num,min_pos,range[1]);
    switch(min_pos){
        case 3:
            //printf("EVALS = %d\n", (int)(NW*(3-min_pos))/3);
            if(num >= (int)(NW*(3-min_pos))/3){
                ret = 1;
                range[min_pos-1] += 1;
            }
            break;
        case 2:
            if(num <= (int)(NW*(min_pos))/3 && num >= (int)(NW*(3-min_pos))/3) {
                    ret = 1;
                    range[min_pos-1] += 1;
                }
            break;
        case 1:
            //printf("EVALS = %d\n", (int)(NW*(3-min_pos))/3);
            if(num >= 0 && num <= (int)(NW*(min_pos))/3){
                    ret = 1;
                    range[min_pos-1] += 1;
                }
            break;
    }

    return ret;

}

int is_close(int a, int b){
    if(a == 0 && b == 0){
        return 1;
    }
    if(a==0 || b == 0){
        return 0;
    }
    printf("a/b=%f\n", (float)(a/b));
    if( (float) (a/b) > .8){
        return 1;
    }else{
        return 0;
    }
}


poly* unlock(int* R, poly* g, poly* C2, int k, int t, int r){
    int* B = create_B(k,t,r,C2);
    int** Q = malloc(sizeof(int*)*(NW));
    int q_size = 0;

    for(int i = 0; i < NW; i++){
        int* tmp_row = malloc(sizeof(int)*2);
        tmp_row[0] = i;
        // for(int j = 0; j < points; j++){
        if(B[i] != -1 && is_close(B[i], R[i])){
           ///*if(B[i] != R[i])*/ printf("WRONG MAP B[i] = %d \t R[i] = %d\n", B[i], R[i]);
            tmp_row[1] = R[i];
            Q[q_size] = tmp_row;
            q_size+=1;
        }
            
        //} 
    }

    printf("Q size = %d\n",q_size);
    poly* C = Q_to_poly(q_size, k, t,r,Q);
    if(C==0) return 0;
    //print_poly(C);
    //printf("\n NEW C(x) = \n");
    //print_poly(C);
    poly* new = RSDecode(t, C, C2, g);
    //if(new == 0) return C;
    return new;
}

poly* Q_to_poly(int points, int k, int t, int r, int** Q){
    
    mat* matrix =0;
    //printf("Qsize = %d\n", points);
    if(points >= 100){
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

poly* RSDecode(int t, poly* C, poly* C2, poly* g){

    // // poly* p = gf_div_poly(C, g, 0);

    // // poly* p2 = gf_div_poly(C2, g, 0);

    poly* div = gf_div_poly(C,g, 0);
    poly* div2 = gf_div_poly(C2,g, 0);

    poly* re = gf_div_poly(C,g, 1);
    if(re->size > 1 && re->coeffs[0] != 0){
        return 0;
    }

    poly* sum = gf_poly_add(div,div2);

    int errors = 0;
    for(int i = 0; i < sum->size; i++){
        errors+=bin_num(sum->coeffs[i]);
    }

    printf("FOUND %d ERRORS IN MESSAGE\n", errors);

    poly* corrected = gf_poly_add(sum,div);

    poly* corrected_p = gf_mult_poly(corrected, g);

    if(errors > 50){
        return 0;
    }else{
        return corrected_p;
    }

    synd* S = syndome_calculator_division(C,  g, t);


    if( poly_eq(C2, C)){
        return C;
    }

    poly* sig = berlecamp_table(S->p, S->synds);

    if(sig != 0){
        printf("Sigma = ");
        print_poly(sig);

        poly* s_r = sigma_r(sig);
        //printf("Sigma_r = ");
        //print_poly(s_r);

        poly* roots = roots_of_poly(s_r,t,NW-1);
        //printf("roots are = ");
        //print_poly(roots);

        poly* errors = error_correction(roots,  S->p);
        printf("errors = ");
        print_poly(errors);
        reassemble_message(errors, roots, C);
        printf("Corrected C = ");
        print_poly(C);

        //poly* M2 = gf_div_poly(p, g,0);

        return C;
    }else{
        printf("SIG WAS 0\n");
        return C;
    }

}

int sequence_of_bits(int seq_number, int flip){
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
            int rand_val = rand()%100; 
            if(rand_val > 100-flip){
                if(bits[bits_pos] == '1') tmp[count] = '0';
                if(bits[bits_pos] == '0') tmp[count] = '1';
                count+=1;
                bits_pos+=1;
            }else{
                tmp[count] = bits[bits_pos];
                count+=1;
                bits_pos+=1;
            }

        }
        
    }
    return -1;
}