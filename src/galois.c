#include "./headers/galois.h"


extern char bits[];



poly* create_poly(int size){
    poly* p = malloc(sizeof(poly));
    p->coeffs = malloc(sizeof(unsigned int)*size);
    p->size = size;
    for(int i =0; i < size; i++){
        p->coeffs[i] = 0;   
    }
    return p;
}


poly* dup_poly(poly* a){
    poly* ret = malloc(sizeof(poly));
    ret->size = a->size;
    ret->coeffs = malloc(sizeof(int)*a->size);
    for(int i = 0; i < a->size; i++){
        ret->coeffs[i] = a->coeffs[i];
    }
    return ret;
}

void free_poly(poly* p){
    free(p->coeffs);
    free(p);
}

void resize_poly(poly* p){
    int size = 0;
    for(int i = p->size-1; i >= 0; i--){
        if(p->coeffs[i] != 0){
            size = i;
            break;
        }
    }
    p->coeffs = realloc(p->coeffs, (size+1)*sizeof(int));
    p->size = size+1;

}

int deg(poly* p){
    for(int i = p->size-1; i >= 0; i--){
        if(p->coeffs[i] != 0){
            return i;
        }
    }
    return 0;
}

void print_poly(poly* p){

    for(int i = 0; i < p->size; i++){
        printf("%d*x^%d +\t", p->coeffs[i], i);
    }
    printf("\n");

}


int setup_tables(){
    unsigned int x_to_w=0; 
    unsigned int b=0; 
    unsigned int log = 0;

    x_to_w =  NW;
    gflog = malloc(sizeof(unsigned int) * (x_to_w));
    gfilog = malloc(sizeof(unsigned int) * (x_to_w));

    b = 1;
    for(log = 0; log < x_to_w-1; log++){
        gflog[b] = (unsigned int)log;
        gfilog[log] = (unsigned int) b;
        b = b << 1;
        if(b & (x_to_w)){
            // printf("log: %d\n", log);
            // bin(b);
            // printf("\n\n");
            // bin(prim_poly);
            // printf("\n\n");
            // bin(x_to_w);
            // printf("\n\n");
            // bin(b ^ prim_poly);
            // printf("\n");
            b = (b ^ prim_poly) ;
            //bin(b);
        }

    }

    // for(int i = 0; i < NW; i++){
    //    printf("%d\t",gflog[i]);
    // }

    // printf("\n");

    // for(int i = 0; i < NW; i++){
    //    printf("%d\t",gfilog[i]);
    // }

    // printf("\n");

    // exit(1);

    return 0;

}


int gf_mult(int a, int b){
    int sum_log;

    if(a == 0 || b == 0) return 0;
    sum_log = (int)gflog[a] + (int)gflog[b];
    if(sum_log >= NW-1){
        //printf("%d\n", sum_log); 
        sum_log -= NW-1;
    }
    //printf("%d\n", sum_log);
    return gfilog[sum_log];
}

poly* gf_mult_poly(poly* a, poly* b){

    if(a->size == 1 && b->size== 1){
        poly* ret = create_poly(2);
        ret->coeffs[0] = gf_mult(a->coeffs[0], b->coeffs[0]);
        return ret;
    }



    int new_poly_size = a->size + b->size - 1;

    poly* new_poly = create_poly(new_poly_size);

    if(a->size == 1){
        for(int j = b->size-1; j >= 0; j--){
            //printf(" j = %d a size = %d b size = %d\n",j,a->size, b->size);
            new_poly->coeffs[j] ^= (unsigned int)gf_mult(a->coeffs[0], b->coeffs[j]);
        }
    }

    if(b->size == 1){
        for(int j = a->size-1; j >= 0; j--){
            //printf(" j = %d a size = %d b size = %d\n",j,a->size, b->size);
            new_poly->coeffs[j] ^= (unsigned int)gf_mult(a->coeffs[j], b->coeffs[0]);
        }
    }


    for(int i = a->size-1; i >= 0; i--){
        for(int j = b->size-1; j >= 0; j--){
            //printf("i = %d, j = %d a size = %d b size = %d\n", i,j,a->size, b->size);
            new_poly->coeffs[i+j] ^= (unsigned int)gf_mult(a->coeffs[i], b->coeffs[j]);
        }
    }

    resize_poly(new_poly);

    return new_poly;
}

int gf_div(int a, int b){
    int diff_log;

    if(a == 0) return 0;
    if(b == 0) return 0;
    diff_log = (int)gflog[a] - (int)gflog[b];
    if(diff_log < 0) diff_log += NW-1;
    return gfilog[diff_log];
}

poly* gf_poly_add(poly* a, poly* b){
    int max = 0;
    int min = 0;
    if(a->size > b->size){
        max = a->size;
        min = b->size;
    }else{
        max = b->size;
        min = a->size;
    }

    poly* p = create_poly(max);

    for(int i =0; i < max; i++){
        if(i >= min){
            if(min == b->size){
                p->coeffs[i] = a->coeffs[i];
            }else{
                p->coeffs[i] = b->coeffs[i];
            }
        }else{
            p->coeffs[i] = a->coeffs[i] ^ b->coeffs[i];
        }
    }
    return p;
}

// a/b if remainder = 1 return remainder else return the divsion
poly* gf_div_poly(poly* a, poly* b, int remainder){
    
    poly* zero = create_poly(2);
    zero->coeffs[0] = 0;
    if(a->size == 0 || b->size == 0) return zero;

    if(a->size <= 2){
        int div = a->coeffs[0];
        for(int i = 0; i < b->size; i++){
            b->coeffs[i] = gf_div(div, b->coeffs[i]);
        }
        return b;
    }
    
    if(b->size <= 2){
        for(int i = 0; i < a->size; i++){
            a->coeffs[i] = gf_div(a->coeffs[i], b->coeffs[0]); 
        }
        return a;
    }

    int pos = 0;

    //printf("%d\n", range);

    poly* divisor = dup_poly(b);

    poly* dividend = dup_poly(a);

    poly* q = create_poly(a->size);

    int i = a->size-1;

    while(i >= b->size-1){
        int num =  dividend->coeffs[i];
        int test = 0;
        int coeff = 0;
        //int tmp = 0;

        for(int j = 1; j < p; j++){
            test = gf_mult(j,divisor->coeffs[b->size-1]);
            int test2 = test^num;
            if(test2 == 0){
                coeff = j;
                break;
            }
        }

        //bin(coeff);
        //printf("Coeff = %d a=%d\t b=%d\t div_location=%d\t pos=%d \t i=%d\t b->size-1=%d\n", coeff, dividend[i], divisor[b->size-1],div_location,pos,i,a->size-1);
        //print_arr(dividend, a->size); 
        //print_arr(divisor, b->size); 
        q->coeffs[(i-b->size+1)] = coeff;
        // int tmp =0;

        if(coeff != 0){
            int tmp_pos= 0;
            for(int j = b->size-1; j >= 0;j--){
                if (divisor->coeffs[j] != 0){
                    //printf("a=%d  b=%d b/a=%d range=%d\n", dividend[j], divisor[j], gf_div(coeff, divisor[j]), range);
                    //printf("product = %d\n", gf_mult(divisor[j], coeff));
                    //tmp = gf_mult(divisor->coeffs[j], coeff);
                    dividend->coeffs[i-tmp_pos] ^= gf_mult(divisor->coeffs[j], coeff);
                    //printf("x^%d + x^%d \n", i-tmp_pos, j);
                }
                tmp_pos++;
            }
            //print_arr(dividend, a->size); 
        }
        pos++;
        i--;

    }
    //printf("QUOTIENT IN DIV FUNC.\n");
    //print_poly(q);
    resize_poly(q);
    resize_poly(dividend);

    if(remainder == 1){
        //printf("POSITON1\n");
        //printf("a->size=%d \t b->size=%d \t set=%d \t range=%d\n", a->size, b->size, sep, range);
        free_poly(divisor);
        free_poly(zero); 
        return dividend;
    }else{
        //printf("POSITION\n");
        free_poly(zero);
        free_poly(divisor);
        free_poly(dividend);
        return q;
    }

}

poly* g(int t){

        
    poly* tmp_1 = create_poly(2);
    tmp_1->coeffs[0] = generator;
    tmp_1->coeffs[1] = 1;
    for(int i = 2; i <= 2*t; i++){
        poly* tmp_2 = create_poly(2);
        tmp_2->coeffs[0] = gf_pow(generator, i);
        tmp_2->coeffs[1] = 1;
        poly* holder = gf_mult_poly(tmp_1, tmp_2);
        free(tmp_1);
        tmp_1 = create_poly(holder->size);
        for(int j =0; j < holder->size; j++){
            tmp_1->coeffs[j] = holder->coeffs[j];
        }
        free(tmp_2);
    }
    //printf("%d\n", size);
    return tmp_1;
}


poly* m_(int n, int k, int t, int bits_start, int tracker){
    poly* M = create_poly(n);
    char* tmp = malloc(sizeof(char)*pow_2);
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
            if(tracker == 0) {
                stream_count_layers[0]+=1;
            }else {
                stream_count_layers[1]+=1;
            } 
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

int eval_poly(poly* p, int x){
    int ret = p->coeffs[0];
    for(int i = 1; i < p->size; i++ ){
        ret ^= gf_mult(p->coeffs[i], gf_pow(x, i));
       
    }
    return ret;
} 

int gf_pow(unsigned int a, int b){
    unsigned int ret = 1;
    if(a == 0){
        return 0;
    }
    for (int i =0; i < b; i++){
        ret = (unsigned int) gf_mult(ret, a);
    }
    //printf("ret=%d\t a=%d\t b=%d\n", ret, a, b);
    //sleep(1);
    return ret;
}

void bin(unsigned n) { 
    unsigned i; 
    for (i = 1 << 16; i > 0; i = i / 2) 
        (n & i)? printf("1"): printf("0");
   //printf("\n"); 
} 

unsigned int str_int(char* str){
    int tmp_num = 0;
    unsigned int num = 0;
    int size = pow_2-1;

    for (int i =0; i <= size; i++){
        if(str[i] == '0'){
            tmp_num = 0;
        }else{
            tmp_num = 1;
        }
        num += (unsigned int)tmp_num*((int)pow(2, size-(i)));   
    }

    return num;
}

synd* syndome_calculator_division(poly* C, poly* g, int t){

    poly* s = gf_div_poly(C, g,1);
    poly* S = create_poly(2*t);
    synd* sy = malloc(sizeof(synd));
    printf("s = ");
    print_poly(s);
    int iter = 1;
    sy->p = S;
    sy->synds = 0;
    for(int i = 1; i <= 2*t; i++){
        S->coeffs[i-1] = eval_poly(s, gf_pow(generator, i));
        for(int j = 0; j < i; j++){
            if(S->coeffs[i-1] == S->coeffs[j] && i-1 != j){
                iter = 0;
            }
        }
        if(iter == 1){
            sy->synds += 1;
        }
        iter = 1; 
    }

    //printf("Syndromes are = ");
    //print_poly(sy->p);

    //printf("amount of syndromes = %d\n", sy->synds);

    resize_poly(S);

    return sy;



}



poly* sigma_r(poly* s){
    int pos = 0;
    int size = s->size;
    poly* sigma_r = create_poly(size);
    for(int i =size-1; i >=0; i--){
        sigma_r->coeffs[pos] = s->coeffs[i];
        pos++;
        
    }
    return sigma_r;
}

poly* roots_of_poly(poly* sigma_r, int t, int n){
    poly* zeros_ = create_poly(sigma_r->size);
    int zeros = 0;
    unsigned int num = 0;

    for(int i=0; i < NW-1; i++){
        num = (unsigned int) eval_poly(sigma_r, gf_pow(generator, i));
        //eval_poly(sigma_r, len, gf_pow(generator, i));
        // printf("%d\n", num);
        if(num == 0){
            zeros_->coeffs[zeros] = i;
            zeros++;
            //printf("ZERO AT %d\n", i);
        }
        //printf("%d\n", zeros);
    }


    // for(int i=1; i < NW; i++){
    //     num = (unsigned int) eval_poly(sigma_r, len, i);
    //     if(num == 0){
    //         zeros_[zeros] = i;
    //         zeros_ = realloc(zeros_, sizeof(int)*(zeros+1));
    //         zeros++;
    //         printf("%d\n", i);
    //     }
    //     //printf("%d\n", zeros);
    // }

    resize_poly(zeros_);

    return zeros_;
}


poly* berlecamp_table(poly* S, int t){

    poly* C = create_poly(2);
    C->coeffs[0]  = 1;
    poly* B = create_poly(2);
    B->coeffs[0] = 1;

    

    int m = 1;

    int L = 0;

    int b = 1;

    int n, d;

    
    if(t <= (S->size)-2){
        t = S->size;
    }
    //printf("t=%d\n", t);

    for(n = 0; n < t; n++){
        d = S->coeffs[n];
        for(int i =1; i <= L; i++){
    
            d ^= gf_mult(C->coeffs[i], S->coeffs[n-i]);
        }
        
        if (d == 0){
            m = m+1;
        }else if(2*L <= n+1){
            poly* T = C;
            int coeff = gf_mult(d,gf_inverse(b));

            poly* tmp = create_poly(m+1);
            tmp->coeffs[m] = coeff;

            

            C = gf_poly_add(C, gf_mult_poly(tmp, B));
            L = n + 1 - L;
            B = T;
            b = d;
            m = 1;

        }else{
            int coeff = gf_mult(d,gf_inverse(b));;
            poly* tmp = create_poly(m+1);
            tmp->coeffs[m] = coeff;
            C = gf_poly_add(C, gf_mult_poly(tmp, B));
            m=m+1;
        }
        
    }

    //printf("L = %d\n", L);

    if(L == 0){
        return 0;
    }

    return C;




}

poly* inverse_poly(poly* p){
    for(int i = 0; i < p->size; i++){
        p->coeffs[i] = gf_inverse(p->coeffs[i]);
    }
    return p;
}

poly* error_correction(poly* roots, poly* S){
    
    mat* matrix = create_matrix(S->size, roots->size+1);
    //print_poly(roots);
    //init matrix for error locations
    for(int i = 0; i < S->size; i++){
        int* tmp = malloc(sizeof(int)* (roots->size+1));
        for(int j =0; j < roots->size+1; j++){
            if(j < roots->size){
                tmp[j] = gf_pow(gf_pow(generator, roots->coeffs[j]), i+1);
            }else{
                tmp[j] = S->coeffs[i];
            }
        }
        matrix->matrix[i] = tmp;    
    }
    
    poly* errors = gauss_elim(matrix);
    

    //printf("Error correction at each coeffiecnt = \n");
    //print_poly(errors);



    return errors;

}

void print_mat(int** mat, int row, int col){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            printf("%d \t", mat[i][j]);
        }
        printf("\n");
    }
}

int find_coeff_row_reduction(int a, int b){
    int tmp = 0;
    int res = 0;
    if(b == 0){
        return 0;
    }if(a == 0){
        return 1;
    }
    for(int i = 1; i < NW; i++){
        tmp = gf_mult(a, i);
        res = tmp ^ b;
        if(res == 0){
            return i;
        }
    }

    return NW;
}

void reassemble_message(poly* errors, poly* locations, poly* M){
    int location = 0;
    int error = 0;

    for(int i =0; i < M->size; i++){
        for(int j = 0; j < errors->size; j++){
            location = locations->coeffs[j];
            error = errors->coeffs[j];
            if(location == i){
                M->coeffs[location] ^= error;
                break;
            }
        }
    }

}

void display_message(poly* M){
    for(int i =0; i < M->size; i++){
        bin(M->coeffs[i]);
    }
    printf("\n");
}

int bin_num(unsigned n) { 
    unsigned i;
    int count = 0; 
    for (i = 1 << 16; i > 0; i = i / 2) 
        (n & i)? count+=1: 0;
    return count;
   //printf("\n"); 
} 

int gf_inverse(int num){

    for(int i = 1; i < NW; i++){
        if(gf_mult(num,i) == 1){
            return i;
        }
    }

    return 0;

}
