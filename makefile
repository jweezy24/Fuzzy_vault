build_universe:
	gcc universe.c -lgsl -lgslcblas -lm -o universe.o simulated_input.o

reed:
	gcc reedSolo.c -g -lgsl -lgslcblas -lm -o reed.o 

reed2:
	gcc reedSolo2.c -g -lm -o reed2.o simulated_input.o