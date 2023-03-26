output: main.o flightticket.o addition.o Iterator.o flighthashtable.o
	g++ main.o flightticket.o addition.o Iterator.o flighthashtable.o -o output
main.o: main.cpp
	g++ -c main.cpp
flightticket.o: flightticket.cpp flightticket.h
	g++ -c flightticket.cpp
Iterator.o: Iterator.cpp Iterator.h
	g++ -c Iterator.cpp
flighthashtable.o: flighthashtable.cpp flighthashtable.h
	g++ -c flighthashtable.cpp
addition.o: addition.cpp
	g++ -c addition.cpp

