#include "./headers/gauss.h"

poly* gauss_elim(mat* matrix){
    int* variables = malloc(sizeof(int)*matrix->cols-1);
    int start_row = determine_left_most_col(matrix);
    check_for_defined_variables(matrix, variables);
    //print_matrix(matrix);
    if (start_row != 0){
        row_swap(matrix, 0, start_row);
    }
    row_echelon(matrix);
    //print_matrix(matrix);
    poly* p = translate_zeros(matrix);
    free(variables);
    free_matrix(matrix);
    //print_poly(p);
    return p;

}

void free_matrix(mat* matrix){
    for(int i = 0; i < matrix->cols; i++){
        free(matrix->matrix[i]);
    }
    free(matrix);
}

mat* create_matrix(int rows, int cols){
    mat* ret = malloc(sizeof(mat));
    ret->matrix = malloc(sizeof(int*)*rows);
    ret->rows = rows;
    ret->cols = cols;
    return ret;
}

int poly_eq(poly* p1, poly* p2){
    resize_poly(p1);
    resize_poly(p2);
    if(p1->size != p2->size){
        return 0;
    }
    for(int i = 0; i < p1->size; i++){
        if(p1->coeffs[i] != p2->coeffs[i]){
            return 0;
        }
    }
    return 1;
}


void print_matrix(mat* matrix){
    for(int i = 0; i < matrix->rows; i++){
        for(int j = 0; j < matrix->cols; j++){
            printf("%d \t", matrix->matrix[i][j]);
        }
        printf("\n");
    }
}

int determine_left_most_col(mat* matrix){
    int row = matrix->cols; 
    for(int i = 0; i < matrix->rows; i++){
        for(int j = 0; j < matrix->cols; j++){
            if(matrix->matrix[i][j] != 0){
                if(i < row){
                    row = i;
                }
            }
        }
    }
    return row;
}

void row_swap(mat* matrix, int row_old, int row_new){
    int tmp_row_1 = matrix->matrix[row_old];
    int tmp_row_2 = matrix->matrix[row_new];
    matrix->matrix[row_old] = tmp_row_2;
    matrix->matrix[row_new] = tmp_row_1;
}

void scale_row(int* row, int size, int scale){
    for(int i = 0; i < size; i++){
        row[i] = gf_mult(row[i],scale);
    }
}

int* copy_row(int* row, int size){
    int* new_row = malloc(sizeof(int)*size);
    for(int j = 0; j < size; j++){
        new_row[j] = row[j];
    }
    return new_row;
}


void zero_row_move(mat* matrix, int row){

    for(int i = row; i < matrix->rows-1; i++){
        int* tmp_row = matrix->matrix[i+1];
        int* tmp_row_z = matrix->matrix[i];
        matrix->matrix[i] = tmp_row;
        matrix->matrix[i+1] = tmp_row_z;
    }
}

int is_zero_row(mat* matrix, int row){

    int zero_count = 0;
    //printf("Coeff = %d\n", coeff );
    for(int k = 0; k < matrix->cols; k++){
        if(matrix->matrix[row][k] == 0){
            zero_count+=1;
        }
    }

    if (zero_count >= matrix->cols-1){
        return 1;
    }
    return 0;
}


void check_for_defined_variables(mat* matrix, int* vars){
    int non_zero_count = 0;
    int tmp_pos = 0;
    int coeff = 0;
    int S_i = 0;

    if(matrix->cols == 2){
        int a = 0;
        int b = 0;
        for(int i = 0; i < matrix->rows; i++){
            if(i == 0){
                a=matrix->matrix[i][0];
                b=matrix->matrix[i][1];
                int tmp = gf_inverse(gf_div(a,b));
                matrix->matrix[i][0] = 1;
                matrix->matrix[i][1] = tmp;  
            }else{
                matrix->matrix[i][0] = 0;
                matrix->matrix[i][1] = 0;  
            }
        }
        // printf("A=%d\tB=%d\n", a,b);
        // printf("A/B=%d\n", gf_div(a,b));
    }else{


        for(int i = 0; i < matrix->rows; i++){
            non_zero_count = 0;
            for(int j = 0; j < matrix->cols-1; j++){
                if(matrix->matrix[i][j] != 0){
                    non_zero_count +=1;
                    tmp_pos = j;
                    coeff = matrix->matrix[i][j];
                    S_i = matrix->matrix[i][matrix->cols-1];
                    coeff = gf_div(S_i, coeff);
                }
            }
            if(non_zero_count == 1){
                vars[i] = coeff;
                for(int j = 0; j < matrix->rows; j++){
                    int tmp_val = gf_mult(matrix->matrix[j][tmp_pos], coeff);
                    matrix->matrix[j][tmp_pos] = 0;
                    matrix->matrix[j][matrix->cols-1] = tmp_val;
                }   
            }
        }
    }

}


void row_echelon(mat* matrix){
    int row_pos = 0;
    for(int i = 0; i < matrix->cols-1 && row_pos < matrix->rows; i++){
        int* tmp_row = malloc(sizeof(int)* matrix->cols);

        for(int j = 0; j < matrix->cols; j++){
            tmp_row[j] = matrix->matrix[row_pos][j];
        }

        if(is_zero_row(matrix, i)==1){
                zero_row_move(matrix,i);
                if(is_zero_row(matrix,i) == 0){
                    if(i != 0){
                        i-=1;
                    }
                }
            }
        
        int coeff = gf_inverse(tmp_row[i]);

        scale_row(tmp_row, matrix->cols, coeff);

        matrix->matrix[row_pos] = tmp_row;

        row_pos+=1;
        
        for(int j = i+1; j < matrix->rows; j++){

            //matrix->matrix[i] = tmp_row;
            int* tmp_row2 = copy_row(tmp_row, matrix->cols);

            
            scale_row(tmp_row2, matrix->cols, matrix->matrix[j][i]);

            for(int k = 0; k < matrix->cols; k++){
                matrix->matrix[j][k] = tmp_row2[k] ^ matrix->matrix[j][k];
            }

            if(is_zero_row(matrix, j)==1){
                zero_row_move(matrix,j);
                if(is_zero_row(matrix,j) == 0){
                    j-=1;
                }
            }

            free(tmp_row2);

            //printf("Coeff = %d\n", coeff );
            //print_matrix(matrix);
        
        }
        //print_matrix(matrix);
    }
}


poly* translate_zeros (mat* matrix){

    poly* errors = create_poly(matrix->cols-1);

    for(int i = 0; i < errors->size; i++){
        errors->coeffs[i] = -1;
    }

    int distance_to_travel = 0;

    for(int i=matrix->rows-1; i >= 0; i--){
        int* tmp_row = matrix->matrix[i];
        int S_i = tmp_row[matrix->cols-1];
        //printf("BEFORE S_I = %d\n", tmp_row[matrix->cols]);
        
        if(is_zero_row(matrix, i) == 0 || (i == 0 && matrix->cols == 2)){
            
            for(int j = errors->size-1; j >= 0; j--){
                //printf("LOOP S_I = %d\n", S_i);
                if(errors->coeffs[j] == -1 && !(i == 0 && matrix->cols == 2)){
                    errors->coeffs[j] = S_i;
                    break;
                }else if(errors->coeffs[j] != -1){
                    S_i ^= gf_mult(errors->coeffs[j], tmp_row[j]);
                }else if(errors->coeffs[j] == -1 && (i == 0 && matrix->cols == 2)){
                    errors->coeffs[j] = matrix->matrix[0][1];
                    break;
                }
            }
            
       }
    }

    return errors;
}