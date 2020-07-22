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
    
    for(int i = 0; i < NW;){
        int x = sequence_of_bits(stream_count,percent);
        //int x = gf_pow(irrs[rotation%5],sequence_of_bits(stream_count,percent));
        //int card = cardinality(x);
        int num = x;
        //printf("Cardinality of %d = %d\n", x, card);
        //int num = gf_pow(generator, raised%NW);
        //int num = gf_pow(generator, i);
        int y = eval_poly(p, num);
        //printf("in_range = %d\n", in_range);
        //printf("NUM = %d\n", num);
        if (num != -1 && R[num] == -1 && X[num] == 0){ //&& card >= points){
            //int in_range = check_range(range, num);
            //if(in_range){
                X[num] = 1;
                R[num] = y;
                i++;
            //}
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

    // for(int i = 0; i < NW;i++){
    //     if(R[i] == -1){
    //         int val = rand()%NW;
    //         R[i] = val;
    //     }
    // }
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

    for(int i = 0; i < NW-2;){
        int x = sequence_of_bits(stream_count,0);
        //int x = gf_pow(irrs[rotation%5],sequence_of_bits(stream_count,0));
        //int card = cardinality(x);
        int num = x;
        //printf("Cardinality of %d = %d\n", x, card);
        //int num = gf_pow(generator, raised%NW);
        //int num = gf_pow(generator, i);
        int y = eval_poly(p, x);
        //printf("Cardinality of %d = %d\n", x, card);

        if (num != -1 && R[num] == -1 && X[num] == 0){
            int in_range = check_range(range, num);
            if(in_range){
                X[num] = 1;
                R[num] = y;
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
    int max = 1;
    int min = 1;
    
    if(a == 0 && b == 0){
        return 1;
    }
    if(a==0 || b == 0){
        return 0;
    }
    if(a > b) {max = a; min = b;}
    if(a <= b) {max = b; min = a;}
    printf("a/b=%f\n", (float)min/(float)max);
    if( (float)min/(float)max > .8){
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
        if(B[i] != -1 && is_close(R[i],B[i])){
            ///*if(B[i] != R[i])*/ printf("B[i] = %d \t R[i] = %d at i = %d \n", B[i], R[i], i);
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
    poly* new = RSDecode(t, C, g, 0);
    //if(new == 0) return C;
    return new;
}


poly* Q_to_poly(int points_2, int k, int t, int r, int** Q){
    
    mat* matrix =0;
    printf("Inside dis funct big boy.\n");
    if(points_2 >= points){
        matrix = create_matrix(points, points+1);
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
                //printf("y = %d\n", y);
                row[j] = y;
            } 
        }
        matrix->matrix[i] = row;
    }

    poly* p = gauss_elim(matrix);
    //print_matrix(matrix);
    return p;
}

poly* RSDecode(int t, poly* C, poly* noise, poly* g){

    // // // poly* p = gf_div_poly(C, g, 0);

    // // // poly* p2 = gf_div_poly(C2, g, 0);

    // poly* div = gf_div_poly(C,g, 0);
    // poly* div2 = gf_div_poly(C2,g, 0);

    // // poly* re = gf_div_poly(C, g, 1);
    // // if(re->size > 1 && re->coeffs[0] != 0){
    // //     printf("C = ");
    // //     print_poly(C);
    // //     return 0;
    // // }

    // poly* sum = gf_poly_add(div,div2);

    // int errors = 0;
    // for(int i = 0; i < sum->size; i++){
    //     errors+=bin_num(sum->coeffs[i]);
    // }

    // printf("FOUND %d ERRORS IN MESSAGE\n", errors);

    // poly* corrected = gf_poly_add(sum,div);

    // poly* corrected_p = gf_mult_poly(corrected, g);

    // if(errors > points/2){
    //     return 0;
    // }else{
    //     return corrected_p;
    // }

    C = gf_poly_add(C, noise);

    //printf("\nPRE FIX = ");
    //print_poly(bad);
    synd* S = syndome_calculator_division(C, g, t);
    //printf("PAST THE S DEF\n");

    poly* sig = berlecamp_table(S->p, S->synds);
    if(sig != 0){
        // printf("Sigma = ");
        // print_poly(sig);
        poly* s_r = sigma_r(sig);
        // printf("Sigma_r = ");
        // print_poly(s_r);

        poly* roots = roots_of_poly(s_r,t,NW-1);
        // printf("roots are = ");
        // print_poly(roots);

        poly* errors = error_correction(roots,  S->p);
        // printf("errors are = ");
        // print_poly(errors);

        reassemble_message(errors, roots, C);
    

        poly* M2 = gf_div_poly(C, g, 0);
        free_poly(sig);
        free_synd(S);
        free_poly(roots);
        free_poly(errors);
        //poly* message = gf_div_poly(C, M2, 0);
        return M2;
    }else{
        //printf("SIG WAS 0\n");
        poly* message = gf_div_poly(C, g, 0);
        return message;
    }

}

int sequence_of_bits(int seq_number, int flip){
    char* tmp = malloc(sizeof(char)*pow_2);
    int bits_pos = seq_number*(pow_2-1);
    int count = 0;
    
    for(int i = seq_number*(pow_2-1); i <= seq_number*pow_2; i++){
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

int cardinality(int x){
    int num = x;
    int count =0;
    if(x == 0) return 0;
    while(num != 1){
        count+=1;
        num = gf_mult(num,x);
    }
    return count;
}

poly* m_random_message(int size){
    poly* ret = create_poly(size);
    for(int i = 0; i < size; i++){
        ret->coeffs[i] = abs(randombytes_random())%256;
    }
    return ret;
}