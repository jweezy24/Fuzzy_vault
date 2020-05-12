#include "./headers/universe.h"
#include "polynomial_fittings.c"

extern char bits[];
int points = 0;

int stream_len(){
    int pos = 0;
    int val = 1;

    while(val){
        val = bits[pos];
        pos+=1;
    }
    return pos;
}

double** create_domain_range(){
    int len = stream_len();
    int pos = 0;
    int binary = 0;
    double** axies = malloc(sizeof(double*)*2);
    double* x_ax = malloc(sizeof(double));
    double* y_ax = malloc(sizeof(double));
    char* str = malloc(8);


    for(int i = 0; i < len; i++){
        char tmp = bits[i];
        str[binary] = tmp;
        binary+=1;
        if(binary%8 == 0 && binary > 0){
            int number = str_int(str);
            x_ax[pos] = number;
            y_ax[pos] = number;
            pos+=1;
            x_ax = realloc(x_ax, sizeof(double)*(pos+1));
            y_ax = realloc(y_ax, sizeof(double)*(pos+1));
            binary = 0;
        }

    }
    axies[0] = x_ax;
    axies[1] = y_ax;
    points = pos;

    return axies;
}

double** create_domain_range_corruption(){
    int len = stream_len();
    int pos = 0;
    int binary = 0;
    int changes = 0;
    double** axies = malloc(sizeof(double*)*2);
    double* x_ax = malloc(sizeof(double));
    double* y_ax = malloc(sizeof(double));
    char* str = malloc(8);


    for(int i = 0; i < len; i++){
        char tmp = bits[i];
        int drop = rand();
        
        if(drop > 31084 && changes < 0){
         if (tmp == '0'){
             tmp = '1';
         } else{
             tmp = '0';
         }  
         str[binary] = tmp;
         changes+=1;
        }else{ 
            str[binary] = tmp;
        }
        binary+=1;
        if(binary%8 == 0 && binary > 0){
            int number = str_int(str);
            x_ax[pos] = number;
            y_ax[pos] = number;
            pos+=1;
            x_ax = realloc(x_ax, sizeof(double)*(pos+1));
            y_ax = realloc(y_ax, sizeof(double)*(pos+1));
            binary = 0;
        }

    }
    axies[0] = x_ax;
    axies[1] = y_ax;

    return axies;
}

int str_int(char* str){
    char c = 0;
    int tmp_num = 0;
    int num = 0;
    int size = 8;

    for (int i =0; i < size; i++){
        c = str[i];
        tmp_num = c -'0';
        num += tmp_num*pow_jack(10, size-(i+1));   
    }
    return num;
}



void machine1(int k, double** axis){
    char* message = "Jackson Wayne West";
    double* x = axis[0];
    double* y = axis[1];

    double polynomial[k];

    int polynomial_2[k];

    //polynomial eval
    for (int i =0; i < points; i++){
        y[i] = f(x[i]);
    }

    polynomialfit(points, k, (double*)x, (double*)y, (double*)polynomial);

    printf("Old Polynomial:\t");
    for(int j = 0; j < k; j++){
        polynomial_2[j] = (int)(polynomial[j])%prime;
        printf("%dx^(%d) + \t", polynomial_2[j], k-j);
    }
    printf("\n");   


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

int f(int x){
    return (int)(pow_jack(x,3) + x + 1)%prime;
}

int pow_jack(int val, int amount){
    int ret_int = 1;
    for(int i = 0; i < amount; i++){
        ret_int*=val;
    }
    return ret_int;
}

void unlock(int** R, double** B, int k){
    int len = getlen2(R);
    int mesg_pos = 0;
    int pos =0;
    double* x_axis = malloc(sizeof(int)*len);
    double* y_axis = malloc(sizeof(int)*len);
    double polynomial[k];
    int polynomial_2[k];
    char* mesg = malloc(sizeof(len));


    for(int i = 0; i < len; i++){
        double x_i = R[i][0];
        double y_i = R[i][1];
        mesg_pos = is_proper_element(x_i, y_i,len, B);
        if(mesg_pos != -1){
            x_axis[pos] = x_i;
            y_axis[pos] = y_i;
            pos+=1;
        }
    }
    polynomialfit(points, k, (double*)x_axis, (double*)y_axis, (double*)polynomial);

    printf("New Polynomial:\t");
    for(int j = 0; j < k; j++){
        polynomial_2[j] = (int)polynomial[j];
        printf("%dx^(%d) + \t", polynomial_2[j], k-j);
    }
    printf("\n");
}

int is_proper_element(double x, double y, int len, double** B){
    int is_element = -1;
    for(int i = 0; i < len && is_element == 0; i++){
        double val_b_x = B[i][0];
        double val_b_y = B[i][1];
        if(val_b_x == x){
            is_element = i;
        }
    }
    return is_element;
}

int main(){
    printf("%d\n", stream_len());
    double** data_2 = create_domain_range();
    int* R = machine1(3, data_2);
    double** B = create_domain_range_corruption();
    unlock((int**)data_2, data_2,3);
    return 0;
}