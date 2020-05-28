#include "./headers/galois.h"


int setup_tables(){
    unsigned int x_to_w=0; 
    unsigned int b=0; 
    unsigned int log = 0;

    x_to_w =  NW;
    gflog = malloc(sizeof(unsigned int) * x_to_w);
    gfilog = malloc(sizeof(unsigned int) * x_to_w);

    b = 1;
    for(log = 0; log < x_to_w-1; log++){
        gflog[b] = (unsigned int)log;
        gfilog[log] = (unsigned int) b;
        b = b << 1;
        if(b & (x_to_w)){
            b = (b ^ prim_poly) ;
        }

    }

    //for(int i = 0; i < NW; i++){
    //    printf("%d\t",gflog[i]);
    //}


    return 0;

}

poly* create_poly(int* coeff, int len){
    poly* tmp = malloc(sizeof(poly));

    tmp->coeffs = malloc(sizeof(int)*len);
    tmp->size = len;

    for(int i = 0; i < len; i++){
        tmp->coeffs[i] = coeff[i];
    }

    return tmp;

}

void print_poly(poly* p){
    int size = p->size;

    for(int i = 0; i < size; i++){
        printf("%d*x^%d + \t", p->coeffs[i], i);
    }
    printf("\n");
}


int gf_mult(int a, int b){
    int sum_log;

    if(a == 0 || b == 0) return 0;
    sum_log = (int)gflog[a] + (int)gflog[b];
    if(sum_log >= NW-1){ 
        sum_log -= NW-1;
    }
    //printf("%d\n", sum_log);
    return gfilog[sum_log];
}

int gf_div(int a, int b){
    int diff_log;

    if(a == 0) return 0;
    if(b == 0) return 0;
    diff_log = (int)gflog[a] - (int)gflog[b];
    if(diff_log < 0) diff_log += NW-1;
    return gfilog[diff_log];
}


void bin(unsigned n) 
{ 
    unsigned i; 
    for (i = 1 << 16; i > 0; i = i / 2) 
        (n & i)? printf("1"): printf("0");
   //printf("\n"); 
}

int deg(int* poly, int size){
    for(int i =size-1; i >= 0; i--){
        if(poly[i] != 0){
            return i;
        }
    }
    return 0;
}

int* gf_div_poly(int* a, int* b, int a_len, int b_len, int remainder){

    int range = abs( a_len - b_len - 1);
    int pos = 0;

    printf("%d\n", range);

    int* divisor = malloc(sizeof(int)*b_len);

    int* dividend = malloc(sizeof(int)*a_len);

    for(int i =0; i < b_len; i++){
        divisor[i] = b[i];
 
    }
    for(int i =0; i < a_len; i++){
        dividend[i] = a[i];
 
    }

    int* r = malloc(sizeof(int)*(a_len));
    int* q = malloc(sizeof(int)*a_len);

    int div_location = b_len-1;
    int a_pos = a_len-1;
    int i = deg(dividend,a_len);

    while(i >= b_len-1){
        int coeff = gf_div(dividend[i], divisor[b_len-1]);
        //printf("Coeff = %d a=%d\t b=%d\t div_location=%d\t pos=%d \t i=%d\t b_len-1=%d\n", coeff, dividend[i], divisor[b_len-1],div_location,pos,i,b_len-1);
        //print_arr(dividend, a_len); 
        //print_arr(divisor, b_len); 
        q[(i-b_len+1)] = coeff;


        if(coeff != 0){
            int tmp_pos= 0;
            for(int j = b_len-1; j >= 0;j--){
                if (divisor[j] != 0){
                    //printf("a=%d  b=%d b/a=%d range=%d\n", dividend[j], divisor[j], gf_div(coeff, divisor[j]), range);
                    //printf("product = %d\n", gf_mult(divisor[j], coeff));
                    dividend[i-tmp_pos] ^= gf_mult(divisor[j], coeff);
                    //printf("x^%d + x^%d \n", i-tmp_pos, j);
                }
                tmp_pos++;
            }
            //print_arr(dividend, a_len); 
            div_location--;
        }
        pos++;
        i = deg(dividend,a_len);

    }
    //print_arr(q, a_len);



    int sep = b_len - 1;

    if(remainder == 1){
        //printf("POSITON1\n");
        //printf("a_len=%d \t b_len=%d \t set=%d \t range=%d\n", a_len, b_len, sep, range);
        free(r);
        free(divisor); 
        return dividend;
    }else{
        //printf("POSITION\n");
        free(r);
        free(divisor);
        free(dividend);
        return q;
    }



}

void print_arr(int* a, int len ){
    for(int i = 0; i < len; i++){
        printf("%u*x^%d + \t", a[i], i);
    }
    printf("\n");
}


//simple division test
int run_t1(){

    int test_p[3] = {1,0,1};
    int* test_4 = gf_div_poly(test_p, test_p, 3,3,0);
    int* test_5 = gf_div_poly(test_p, test_p, 3,3,1);
    
    
    print_arr(test_4,3);
    print_arr(test_5,3);

    return 1;
}

//medium difficulty
int run_t2(){

    int test_p[3] = {1,0,1};
    int test_p2[4] = {1,1,0,1};  


    int* test_6 = gf_div_poly(test_p2, test_p, 4,3,0);
    int* test_7 = gf_div_poly(test_p2, test_p, 4,3,1);
    
    
    print_arr(test_6, 4);
    print_arr(test_7, 4);

    return 1;
}

//complex difficulety
int run_t3(){

    int test_p2[6] = {1,0,1,0,0,1};
    int test_p[4] =  {1,1,0,1};  


    int* test_6 = gf_div_poly(test_p2, test_p, 6,4,0);
    int* test_7 = gf_div_poly(test_p2, test_p, 6,4,1);
    
    
    print_arr(test_6, 6);
    print_arr(test_7, 6);

    return 1;
}


int main(){
    setup_tables();
    int test = 0b101;


    int test_2 = gf_mult(test,test);

    int test_3 = gf_div(test, test);

    // bin(test_2);
    // printf("\n");

    // bin(test_3);
    // printf("\n");

    run_t1();
    printf("\n");
    run_t2();
    printf("\n");
    run_t3();




    return 0;
}