int** berlecamp_table(int* R, int t, int n){

    int** b_table = malloc(sizeof(int*)* (2*t+2));
    int** sigmas = malloc(sizeof(int*)*(2*t+3));
    int* sigma_sizes = malloc(sizeof(int)*(2*t+2));


    int r1[5] = {-1, 1, 1, 0, 1};
    int tmp = calculate_S(R, 1, n);
    int r2[5] = {0, 1, 1, tmp, 0};
    
    int* simga_r1 = malloc(sizeof(int));
    int* simga_r2 = malloc(sizeof(int));

    for(int i = 0; i < 1; i++){
        if(i == 0){
            simga_r1[i] = 1;
            simga_r2[i] = 1;
        }else{
            simga_r1[i] = 1;
            simga_r2[i] = 1;
        }
    }
    sigmas[0] = simga_r1;
    sigmas[1] = simga_r2;

    sigma_sizes[0] = 1;
    sigma_sizes[1] = 1;

    b_table[0] = r1;
    b_table[1] = r2;

    for(int i = 2; i < 2*t+2; i++){
        int* row_before = b_table[i-1];
        int* curr_row = malloc(sizeof(int)*5);
        
        int mu = i;
        
        int d_mu_b = calculate_S(R, i-1, n);
        int d_mu = calculate_S(R, i, n);
        
        int* sigma_mu_b = sigmas[i-1];
        //int* sigma_mu = malloc(sizeof(int)*2*t);

        int h_mu_b = row_before[4];
        int h_mu = 0;

        if(d_mu != 0){
            int max_h = h_mu_b;
            int max_h_pos = 0;
            int h_sums = 0;
            int* sigma_row;
            for(int j = i-1; j >= 0; j--){
                int* row = b_table[j];
                int h_mu_j = row[4];
                h_sums = h_mu_j ^ abs(i-1) ^ abs(j-1);
                if (h_sums > max_h){
                    max_h = h_sums;
                    max_h_pos = j;
                    sigma_row = sigmas[j];
                }
            }
            //int d_row = b_table[max_h_pos][4];
            //int d_row_inv = gf_inverse(d_row);
            //unsigned int coeff = gf_mult(d_mu_b, d_row_inv);
            int pow = i ^ abs(max_h_pos-1);
            int* new_x = x_pow(pow);
            int* new_sig = gf_mult_poly(new_x, sigma_row, pow, sigma_sizes[max_h_pos]);
            int new_size = pow+2*t - 1;
            sigma_sizes[i] = new_size;
            int* final_sig =  gf_poly_add(sigma_mu_b , new_sig, sigma_sizes[max_h_pos], new_size);
            //print_arr(final_sig, new_size);
            sigmas[i] = final_sig;
            curr_row[0] = abs(i-2);
            curr_row[1] = 1;
            if(mu-1 > 0){
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(final_sig, new_size, 1);
            }else{
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(final_sig, new_size, calculate_S(R, mu-1, n));
            }
            curr_row[3] = max_h;
            curr_row[4] = mu ^ max_h;
            b_table[i] = curr_row;
        }else{
            sigmas[i] = sigma_mu_b;
            sigma_sizes[i] = sigma_sizes[i-1];
            h_mu = h_mu_b;
            curr_row[0] = i-2;
            curr_row[1] = 1;
            if(mu-1 > 0){
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(sigma_mu_b, sigma_sizes[i-1], 1);
            }else{
                curr_row[2] = calculate_S(R, mu+1, n) ^ eval_poly(sigma_mu_b, sigma_sizes[i-1], calculate_S(R, mu-1, n));
            }
            curr_row[3] = h_mu;
            curr_row[4] = mu ^ h_mu; 
        }

    }

    // for(int i = 0; i < 2*t+2; i++){
    //    printf("HERE\n");
    //    print_arr(sigmas[i], sigma_sizes[i]);
    // }

    // for(int i =0; i < 2*t+2; i++){
    //     free(b_table[i]);
    // }

    free(b_table);
    sigmas[2*t+2] = sigma_sizes;

    return sigmas;

}
