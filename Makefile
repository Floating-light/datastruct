sundry: sundrycode.cpp
	g++ -o sundry.exe sundrycode.cpp
	./sundry.exe

test: LVector.hpp LVectorTest.cpp
	g++ -o LVector.o -c LVector.cpp
	g++ -o LVectorTest.o -c LVectorTest.cpp
	g++ -o vectest.exe LVectorTest.o LVector.o
	./vectest.exe
clean: 
	rm *.exe *.o 