galois:
	cd src && gcc galois.c -Wall -g3 -lm -o galois.o ../simulated_input.o

fuzzy:
	cd src/tests && gcc full_stream_test.c -Wall -g3 -lm -lsodium -lpthread -o tests.o ../../simulated_input.o ../analysis/vk1.o ../analysis/vk2.o
	./src/tests/tests.o

onion:
	cd src_onion/tests && gcc full_stream_test.c -Wall -g3 -lm -lsodium -lpthread -o tests.o ../../simulated_input.o
	./src_onion/tests/tests.o

debug:
	gdb ./src/tests/tests.o

graphs:
	python3 ./src/analysis/analize_correct_keys.py

all_tests:
	gcc ./src/tests/datasets.c -g3 -lm -lsodium -lpthread  -o datasets.o ./src/tests/final.o  simulated_input.o
	./datasets.o
	cd src/tests && gcc full_stream_test.c -Wall -g3 -lm -lsodium -lpthread -o tests.o ../../simulated_input.o
	./src/tests/tests.o

FP:
	cd src/tests && gcc false_positives.c -Wall -g3 -lm -lsodium -o fp.o ../../simulated_input.o final.o
	cd src/tests && ./fp.o

clean:
	rm ./src/tests/data/*

kill_all:
	kill -9 $(ps -aux | grep datasets | awk '{print $2}') 

hamming:
	python3 ./src/tests/hamming_distance_alg.py

comparisons:
	python3 ./src/analysis/comparisons.py