galois:
	cd src && gcc galois.c -Wall -g3 -lm -o galois.o ../simulated_input.o

fuzzy:
	cd src/tests && gcc full_stream_test.c -Wall -g3 -lm -lpthread -o tests.o ../../simulated_input.o

run:
	./src/tests/tests.o

debug:
	gdb ./src/tests/tests.o

graphs:
	python3 ./src/analysis/analize_correct_keys.py