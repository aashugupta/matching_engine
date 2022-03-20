
testMatchingEngine: Utils.o Order.o PQBook.o MatchingEngine.o TestMatchingEngine.o
	g++ Utils.o Order.o PQBook.o MatchingEngine.o TestMatchingEngine.o -o testMatchingEngine

TestMatchingEngine.o: TestMatchingEngine.cpp
	g++ -c TestMatchingEngine.cpp

Order.o: Order.h Order.cpp
	g++ -c Order.cpp

PQBook.o : PQBook.h PQBook.cpp
	g++ -c PQBook.cpp

MatchingEngine.o : MatchingEngine.h MatchingEngine.cpp
	g++ -c MatchingEngine.cpp

Utils.o : Utils.h Utils.cpp
	g++ -c Utils.cpp

clean:
	rm  -rf *.o *.gch testMatchingEngine

