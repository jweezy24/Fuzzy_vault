#include "./headers/universe.h"

int* B[256]; 


int** machine1(){
    char* message = "Jackson Wayne West";
    int** set_of_pointers = malloc(sizeof(int*)* getlen(message));
    
    //polynomial eval
    // f(x) = x^3 + x + 1
    for (int i =0; i < getlen(message); i++){
        int* entry = malloc(sizeof(int)*2);
        int letter = message[i];
        entry[0] = letter;
        entry[1] = f(letter);
        set_of_pointers[i] = entry;
    }   

    return set_of_pointers;

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
    create_B();
    int** points = machine1();
    unlock(points);
    return 0;
}