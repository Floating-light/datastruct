sundry: sundrycode.cpp
	g++ -o sundry.exe sundrycode.cpp
	./sundry.exe

test: LVector.cpp LVector.hpp
	g++ -o vectest.exe LVector.cpp LVectorTest.cpp
	./vectest.exe
clean: 
	rm *.exe