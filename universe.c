#include "./headers/universe.h"

int* B[256];

int LEN_POLY_1 = 0;
int LEN_POLY_2 = 0;


/*
k = degree of irreducible polynomial
t,r = from t to r the range of uniform random points to add to the set of points variable
mesg = message to encrypt
*/
int** machine1(int k, int t, int r, char* mesg){
    int** set_of_pointers = malloc(sizeof(int*)* (getlen(mesg) + (r-t) ) );

    int* coeff = polynomial_generation(k);
    
    //polynomial eval
    for(int i =0; i < getlen(mesg); i++){
        int* entry = malloc(sizeof(int)*2);
        int letter = mesg[i];
        entry[0] = letter;
        entry[1] = f(k, letter, coeff);
        set_of_pointers[i] = entry;
    }

    for(int i=t; i<r; ){
        int rand_num = rand()%prime;
        int not_random = 0;

        for (int j = 0; j < getlen(mesg) && not_random == 0; j++){
            int comp_val = set_of_pointers[j][0];
            if(comp_val == rand_num){
                not_random = 1;
            }
        }

        if(not_random == 0){
            int random_y = rand()%prime;
            while(f(k, rand_num, coeff) == random_y){
                random_y = rand()%prime;
            }
            i++;
            int* entry = malloc(sizeof(int)*2);
            entry[0] = rand_num;
            entry[1] = random_y;
            set_of_pointers[getlen(mesg) + i] = entry;
        }
    }


    return set_of_pointers;

}


int getlen(char* mesg){
    int count =0;
    while(mesg[count] != 0){
        count +=1;
    }
    return count;
}

int getlen2(int** mesg){
    int count =0;
    while(mesg[count] != 0){
        count +=1;
    }
    return count;
}

int* polynomial_generation(int deg){
    int p = 11;
    int* coeff;
    coeff = malloc(sizeof(int)*deg);
    for(int i = 0; i < deg;){
        int test_num = rand()%prime;
        if(i == 0){
            if(test_num%p != 0){
                coeff[0] = test_num;
                i++;
            }
        }
        if(i > 0 && i < deg-1){
            if(test_num%p == 0){
                coeff[i] = test_num;
                i++;
            }
        }
        if(i == deg-1){
            int p_raised = pow_jack(p,2);
            if(test_num%p_raised != 0){
                coeff[i] = test_num;
                i++;
            }
        }
    }

    return coeff;

}

/*
irreducible polynomial generation
*/
int f(int deg, char x, int* coeff){

    int num = 0;
    for(int i = 0; i < deg; i++){
        num += pow_jack(x, deg-i) * coeff[i];
    }
    return num%prime;
}

int pow_jack(int val, int amount){
    int ret_int = 1;
    for(int i = 0; i < amount; i++){
        ret_int*=val;
    }
    return ret_int;
}

void unlock(int k, int t, int r, int** R){
    int* coeff = polynomial_generation(k);
    int len = getlen2(R);
    for(int i = 0; i < len; i++){
        int x_i = R[i][0];
        int y_i = R[i][1];

        
    }
}


/*
mesg = mesage want to form polynomial from
k = degree of polynomial
*/
// int* Lagrange_Interpolation(char* mesg, int k){
//     int len = getlen(mesg);
//     int** coeff_tmp = malloc(sizeof(int)* k);


// }

int** coeff_calculation_Lagrange(int layer, int k, int** new_k, int** tmp_k, int len, int len_inner){
    int* coeff = malloc(sizeof(int)*k);
    int coeff_tmp = 0;
    int pos = 0;
    if(new_k == 0){
        new_k = malloc(sizeof(int*));
        for(int i =1; i <= k; i++){
            for(int j =i+1; j <= k; j++){
                int* entry = malloc(sizeof(int)*2);
                entry[0] = i;
                entry[1] = j;
                new_k[pos] = entry;
                pos+=1;
                new_k = realloc((int**)new_k, sizeof(int*)*(pos+1));
            }
        }
        if(layer-1 > 1){
            LEN_POLY_1 = pos;
            LEN_POLY_2 = 2;
            return coeff_calculation_Lagrange(layer-1, k, new_k, 0, pos, 2);
            
        }else{
            LEN_POLY_1 = pos;
            LEN_POLY_2 = 2;
            return new_k;
        }
    }else{
        tmp_k = malloc(sizeof(int**));
        int append = 1;
        int shit_they_rap = 0;
        for(int i =1; i<=k; i++){
            for(int j = 0; j < len; j++){
                for(int m = 0; m < len_inner; m++){
                    if(new_k[j] != 0){
                        if(i == new_k[j][m]){
                            append = 0;
                        }
                    }else{
                        append = 0;
                    }
                }
                if(append == 1){
                    int tmp_1 = 0;
                    int tmp_2 = 0;
                    int drop = 1;
                    int* entry_tmp = new_k[j];
                    int add_new_list = 0;
                    int number_in = 0;
                    entry_tmp = realloc(entry_tmp, (len_inner+1) * sizeof(int));
                    entry_tmp[len_inner] = i;
                    if(shit_they_rap > 0){
                        for (int check_2 = 0; check_2 < shit_they_rap; check_2++){
                            int* entry_tmp_2 = tmp_k[check_2];
                            for(int check = 0; check < len_inner+1; check++){
                                tmp_1 = entry_tmp[check];
                            for(int check_3 = 0; check_3 < len_inner+1; check_3++){
                                    tmp_2 = entry_tmp_2[check_3];
                                    if (tmp_1 == tmp_2){
                                        printf("ADD TO NEW LIST LIVE: %d\n", add_new_list);
                                        add_new_list += 1;
                                    }
                                }
                            }
                            if(add_new_list > len_inner){
                                printf("IF STATEMENT: %d\t%d\n", i, add_new_list);
                                printf("DROPPED NUMBER:");
                                for(int check = 0; check < len_inner+1; check++){
                                    tmp_1 = entry_tmp[check];
                                    printf("%d\t", tmp_1);
                                }
                                printf("\n");
                                drop = 0;
                            }
                            add_new_list = 0;
                            tmp_1 = 0;
                            tmp_2 = 0;
                        }
                        if (drop){
                            printf("ADDED NUMBER:");
                                for(int check = 0; check < len_inner+1; check++){
                                    tmp_1 = entry_tmp[check];
                                    printf("%d\t", tmp_1);
                                }
                                printf("\n");
                            tmp_k[pos] = entry_tmp;
                            pos+=1;
                            tmp_k = realloc(tmp_k, sizeof(int*)*(pos+1));
                            shit_they_rap +=1;
                        }
                        drop = 1;
                        
                    }else{
                        printf("ADDED NUMBER:");
                                for(int check = 0; check < len_inner+1; check++){
                                    tmp_1 = entry_tmp[check];
                                    printf("%d\t", tmp_1);
                                }
                                printf("\n");
                        tmp_k[pos] = entry_tmp;
                        pos+=1;
                        tmp_k = realloc(tmp_k, sizeof(int*)*(pos+1));
                        shit_they_rap +=1;
                    }
                }
                append = 1;

            }
        }
        if(layer-1 > 1){
            LEN_POLY_1 = pos;
            LEN_POLY_2 = len_inner+1;
            return coeff_calculation_Lagrange(layer-1, k, tmp_k, 0, pos, len_inner+1);
        }else{
            LEN_POLY_1 = pos;
            LEN_POLY_2 = len_inner+1;
            return tmp_k;
        }

    }

}

int* rs_polynomial_decoder(int a, int b, int c){
    
}

void print_coeffs(int** vals){
    printf("%d\n",LEN_POLY_1);
    printf("%d\n",LEN_POLY_2);
    for(int i = 0; i < LEN_POLY_1; i++){
        for(int j =0; j < LEN_POLY_2 ; j++){
            printf("%d\t", vals[i][j]);
        }
        printf("\n");
    }

}

int main(){
    int k = 15;
    int t = 20;
    int r = 25;
    char* message = "Jackson Wayne West";
    int** new_k = 0;
    new_k = coeff_calculation_Lagrange(3, 4, new_k, 0, 0, 0);

    print_coeffs(new_k);
    //int** points = machine1(k,t,r,message);
    //unlock(k,t,r,points);
    return 0;
}