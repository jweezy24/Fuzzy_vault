#include "./headers/universe.h"
#include "polynomial_fittings.c"

int* B[256];
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
            x_ax[pos] = pos;
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

    polynomialfit(points, k, (double*)x, (double*)y, (double*)polynomial);
    
    for(int i=0; i < 15; i++){
        printf("%f\n", polynomial[i]);
    }
    //polynomial eval
    // f(x) = x^3 + x + 1
    for (int i =0; i < getlen(message); i++){
        int* entry = malloc(sizeof(int)*2);
        int letter = message[i];
        entry[0] = letter;
        entry[1] = f(letter);
    }   


}

void create_B(){
    int total_chars = 256;
    for(int i = 0; i < total_chars; i++){
        int* entry = malloc(sizeof(int)*2);
        entry[0] = i;
        entry[1] = f(i);
        B[i] = entry;
    }
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

void unlock(int** points){
    int len = getlen2(points);
    int mesg_pos = 0;
    char* mesg = malloc(sizeof(len));
    for(int i = 0; i < len; i++){
        int x_i = points[i][0];
        int y_i = points[i][1];
        if(is_proper_element(x_i, y_i) == 1){
            mesg[mesg_pos] = x_i;
            mesg_pos+=1;
        }
    }
    printf("%s", mesg);
}

int is_proper_element(int x, int y){
    int len = getlen2(B);
    int is_element = 0;
    for(int i = 0; i < len && is_element == 0; i++){
        int val_b_x = B[i][0];
        int val_b_y = B[i][1];
        if(val_b_x == x && val_b_y == y){
            is_element = 1;
        }
    }
    return is_element;
}

int main(){
    double** data_2 = create_domain_range();
    machine1(15, data_2);
    unlock((int**)data_2);
    return 0;
}