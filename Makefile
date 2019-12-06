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
StrVec.o: ./primer/chapter13/5MgrDynamicMemory/StrVec.cpp \
          ./primer/chapter13/5MgrDynamicMemory/StrVec.hpp
	g++ -g -o StrVec.o -c ./primer/chapter13/5MgrDynamicMemory/StrVec.cpp
main.o: ./primer/chapter13/5MgrDynamicMemory/main.cpp
	g++ -g -o main.o -c ./primer/chapter13/5MgrDynamicMemory/main.cpp
Dyn: StrVec.o main.o
	g++ -o main.exe StrVec.o main.o
#data struct 
#leet code
leetcodeMain: 
	g++ -g ./leetcode/group_stack/timeOfFunction/exclusiveTime.cpp -o main.exe
clean: 
	rm *.exe *.o 