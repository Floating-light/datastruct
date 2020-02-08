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
RvalueReferences.o: ./primer/chapter13/6MovingObjects/RvalueReferences.cpp
	g++ -g -o RvalueReferences.o -c ./primer/chapter13/6MovingObjects/RvalueReferences.cpp
primer: FORCE
	g++ -g ./primer/chapter14/overloadingConversion.cpp -o main.exe
#data struct 
mooc: 
	g++ -g ./DSAMooc/Graph/TestMain.cpp -o main.exe
#leet code
leetcodeMain: 
	g++ -g ./leetcode/JumpGame/jumpGame.cpp -o main.exe
clean: 
	find . -name "*.exe" -exec rm -rf {} \;
	find . -name "*.o" -exec rm -rf {} \;

FORCE:
