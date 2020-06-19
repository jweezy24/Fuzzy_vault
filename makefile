galois:
	cd src && gcc galois.c -Wall -g3 -lm -o galois.o ../simulated_input.o

fuzzy:
	cd src && gcc fuzzy.c -Wall -g3 -lm -lpthread -o fuzzy.o ../simulated_input.o

run:
	./src/fuzzy.o