
typedef struct matrix{
    int** matrix;
    int rows;
    int cols;
}mat;

void gauss_elim(mat* matrix);
int determine_left_most_col(mat* matrix);
void row_swap(mat* matrix, int row_old, int row_new);
void scale_row(int* row, int size, int scale);
void row_echelon(mat* matrix);
void scale_row(int* row, int size, int scale);
int* copy_row(int* row, int size);
void zero_row_move(mat* matrix, int row);
int is_zero_row(mat* matrix, int row);
void check_for_defined_variables(mat* matrix, int* vars);
poly* translate_zeros (mat* matrix);