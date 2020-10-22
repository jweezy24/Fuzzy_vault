


#include "fuzzy.c"

poly* m_arbitrary(int n, int k, int t, int* iter, char* bits, int tracker){
    poly* M = create_poly(n);
    char* tmp = malloc(sizeof(char)*k);
    int pos_holder = 0;
    int bits_pos = k*iter[tracker];
    int count = 0;


    for(int i =0; i <= n*k; i++){
        if(count%k == 0 && count > 0){
            unsigned int tmp_h = (unsigned int)str_int(tmp);
            M->coeffs[pos_holder] = tmp_h;
            pos_holder++;
            count = 0;
            i--;
            iter[tracker] += 1;
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

// Bit sequences that were ZIA
char vk1[] = {'0','0','0','0','0','0','0','0','0','0','1','1','0','0','1','1','0','0','1','1','0','0','0','1','0','0','0','1','0','0','0','1','0','1','1','0','1','0','0','1','0','0','0','0','0','0','0','0','0','0','1','0','1','0','1','1','1','0','0','0','0','0','1','0','1','1','1','0','0','0','1','1','1','1','1','1','1','1','1','1','0','0','0','0','0','1','0','0','0','0','1','1','1','0','0','1','1','1','0','0','0','1','0','1','1','0','0','0','1','0','0','0','0','1','0','1','0','0','0','1','1','1','1','1','1','1','1','1','1','1','0','1','1','1','1','0','1','0','1','1','1','1','0','1','0','0','1','1','0','1','0','0','0','0','0','1','0','0','1','0','0','1','0','1','1','1','0','1','1','0','1','0','1','1','1','1','0','1','1','1','1','0','0','0','0','0','0','1','0','1','0','1','1','1','0','1','1','1','1','1','0','1','0','1','0','1','0','0','0','1','1','0','1','1','0','0','1','1','1','0','0','1','1','0','1','1','0','1','0','0','1','1','0','1','0','0','1','0','1','0','0','0','1','1','0','1','1','0','0','0','0','0','0','0','0','0'};
char vk2[] = {'0','0','0','0','0','0','0','0','0','0','1','1','0','0','1','1','0','0','1','1','0','0','0','1','0','0','0','1','0','0','0','1','0','1','1','0','1','0','0','1','0','0','0','0','0','0','0','0','0','0','1','0','1','0','1','1','1','0','0','0','0','0','1','0','1','1','1','0','0','0','1','1','1','1','1','1','1','1','1','1','0','0','0','0','0','1','0','0','0','0','1','1','1','0','0','1','1','1','0','0','0','1','0','1','1','0','0','0','1','0','0','0','0','1','0','1','0','0','0','1','1','1','1','1','1','1','1','1','1','1','0','1','1','1','1','0','1','0','1','1','1','1','0','1','0','0','1','1','0','1','0','0','0','0','0','1','0','0','1','0','0','1','0','1','1','1','0','1','1','0','1','0','1','1','1','1','0','1','1','1','1','0','0','0','0','0','0','1','0','1','0','1','1','1','0','1','1','1','1','1','0','1','0','1','0','1','0','0','0','1','1','0','1','1','0','0','1','1','1','0','0','1','1','0','1','1','0','1','0','0','1','1','0','1','0','0','1','0','1','0','0','0','1','1','0','1','1','0','0','0','0','0','0','0','0','0'};



int main(int argc, char **argv) {
    setup_tables();
    int n = 30;
    int k = 9;
    //int t = ((n-k)/2);
    int t = ((n-k)/2);
    int r = 128;
    double time_spent;
//    extern char vk2[];
    int total = 0;
    int noise_size = 8; // Num bits per coeff

    int stream_count_layers[2] = {0,0};

    poly* g_x = g(t);
    poly* M = m_random_message(k);
    poly* n_1 = m_arbitrary(k, noise_size, t, stream_count_layers, vk1, 0);
    poly* n_2 = m_arbitrary(k, noise_size, t, stream_count_layers, vk2, 1);
    poly* C2 = gf_mult_poly(n_1, g_x);
    poly* C = gf_mult_poly(g_x, n_2);
//    print_poly(C);
    
    //FUZZY VAULT DECODING
    int* R = lock(k,t,r,C);
    poly* res =  unlock(R, g_x, C2, k, t, r);
    if(res) {
        print_poly(res);
        print_poly(n_1);
    }

    return 0;
}
