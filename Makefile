sundry: sundrycode.cpp
	g++ -o sundry.exe sundrycode.cpp
	./sundry.exe

test: LVector.hpp LVectorTest.cpp
	g++ -o LVector.o -c LVector.cpp
	g++ -o LVectorTest.o -c LVectorTest.cpp
	g++ -o vectest.exe LVectorTest.o LVector.o
	./vectest.exe
CopyControl:
	g++ -g -o copycontrol.exe ./primer/chapter13/CopyControl/copycontrol.cpp
	./copycontrol.exe
leetcode: 
	g++ -g -o leetcode.exe ./leetcode/isMatch/isMatch.cpp
	./leetcode.exe
clean: 
	rm *.exe *.o 