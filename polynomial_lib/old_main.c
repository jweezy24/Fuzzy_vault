int main(){
    setup_tables();
    int n = NW-1;
    int k = 9;
    //int d = n-k+1;
    int t = (n-k)/2;

    // printf("Generator = %d\n", generator);


    time_t dd;
    srand((unsigned) time(&dd));

    poly* g_x = g(t);

    poly* M = m_(n, k , t);

    poly* x_2t = create_poly(2*t+1);
    x_2t->coeffs[2*t] = 1;

    //M = gf_mult_poly(M,x_2t);

    poly* Q = gf_div_poly(M, g_x, 0);
    poly* CK = gf_div_poly(M, g_x, 1);

    //poly* C = gf_poly_add(M,CK);
    poly* C = gf_mult_poly(M,g_x);

    // printf("M(x) = ");
    // print_poly(M);

    // printf("g(x) = ");
    // print_poly(g_x);


    int errors = 0;
    for(int i = 0; i < C->size; i++){
        //5% chance of error
        //printf("%d\n", rand());
        if(rand()%200 > 100 &&  errors <= t){
            unsigned int val = rand()%(int)pow(2,pow_2);
            if( bin_num(val) + errors <= t){
                C->coeffs[i] = (unsigned int)C->coeffs[i] ^ val;
                printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", i, val);
                errors+=bin_num(val);
            }
        }
    }

    printf("THERE ARE %d BIT ERRORS\n", errors);

    // int a = 1;
    // int a_2 = 216;
    // int b = 8;
    // int b_2 = 2;
    // int c = 9;
    // int c_2 = 149;
    // int d = 41;
    // int d_2 = 24;
    // int e = 120;
    // int e_2 = 64;

    // C->coeffs[a] = (unsigned int)C->coeffs[a] ^ a_2;
    // printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", a, a_2);
    // C->coeffs[b] = (unsigned int)C->coeffs[b] ^ b_2;
    // printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", b, b_2);
    // C->coeffs[c] = (unsigned int)C->coeffs[c] ^ c_2;
    // printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", c , c_2);
    // C->coeffs[d] = (unsigned int)C->coeffs[d] ^ d_2;
    // printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", d, d_2);
    // C->coeffs[e] = (unsigned int)C->coeffs[e] ^ e_2;
    // printf("ERROR PUT IN COEFF %d ERROR OFF BY %d\n", e , e_2);

    // printf("Q(x) = ");
    // print_poly(C);

    // printf("CK(x) = ");
    // print_poly(C);



    synd* S = syndome_calculator_division(C,  g_x, t);

    //poly* sig = euclid_alg(S, t);

    if(S->synds == 1 && S->p->coeffs[0] == 0){
        printf("No message issue\n");
        exit(1);
    }

    printf("Syndromes = %d\n", S->synds);
    
    poly* sig = berlecamp_table(S->p, S->synds);

    // printf("Syndromes = %d\n", S->synds);
    // print_poly(sig);

    if(sig != 0){
        //printf("Sigma = ");
        //print_poly(sig);

        poly* s_r = sigma_r(sig);
        printf("Sigma_r = ");
        print_poly(s_r);

        poly* roots = roots_of_poly(s_r,t,n);
        //printf("roots are = ");
        //print_poly(roots);

        poly* errors = error_correction(roots,  S->p);

        reassemble_message(errors, roots, C);

        poly* M2 = gf_div_poly(C, g_x,0);

        print_poly(M);
        printf("Original: ");
        display_message(M);
        print_poly(M2);
        printf("Corrected:");
        display_message(M2);

        int check = 1;
        for(int i = 0; i < M->size; i++){
            if(M->coeffs[i] != M2->coeffs[i]){
                check = 0;
            }
        }

        if(check){
            printf("STREAMS MATCH\n");
        }


    }

    return 0;
}