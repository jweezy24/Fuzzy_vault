build_universe:
	gcc universe.c -lgsl -lgslcblas -lm -o universe.o simulated_input.o

reed:
	gcc reedSolo.c -g -lgsl -lgslcblas -lm -o reed.o 