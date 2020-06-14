#include <pthread.h> 
#include "../galois.c"
#include "../gauss.c"



//threads
void *machine1();
void *machine2();
void start_threads();

//utils
int sequence_of_bits(int seq_number);

//fuzzy vault specific methods
int** lock(int k, int t, int r, poly* g);
poly* create_secret_poly(int k);
int** create_B(int k, int t, int r, poly* g);
poly* unlock(int** R, poly* g, int k, int t, int r);
poly* Q_to_poly(int k, int t, int r, int** Q);
poly* RSDecode(int t, poly* C, poly* g);


//matrix methods
mat* create_matrix(int rows, int cols);
void print_matrix(mat* mat);