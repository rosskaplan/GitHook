callDatabase.o: callDatabase.cpp
	g++ callDatabase.cpp -o callDatabase --std=c++11
	./callDatabase -e emailtest -n nametest -d datetest -h hashtest -m testtesttesttesttesttesttesttesttesttest *tag1test* *tag2test* messagetest messagetest2
