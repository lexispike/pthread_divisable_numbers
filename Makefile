# compile Q2.cpp
Q2: Q2.cpp
	g++ -lpthread -std=c++11 Q2.cpp -o Q2

# run Q2
run_Q2: Q2
	./Q2

# clean the directory
clean:
	rm -f Q2
