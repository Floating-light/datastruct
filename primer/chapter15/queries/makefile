TextQuery.o: TextQuery.h TextQuery.cpp QueryResult.h
	g++ -g -c TextQuery.cpp
Query.o: Query.hpp Query.cpp
	g++ -g -c Query.cpp
WordQuery.o: WordQuery.hpp TextQuery.h
	g++ -c WordQuery.hpp
NotQuery.o: NotQuery.hpp Query_base.hpp TextQuery.h Query.hpp
	g++ -c NotQuery.hpp
OrQuery.o: OrQuery.hpp
	g++ -c OrQuery.hpp
AndQuery.o: AndQuery.hpp
	g++ -c AndQuery.hpp
newQueryMain.o: newQueryMain.cpp
	g++ -g -c newQueryMain.cpp
Main: TextQuery.o Query.o newQueryMain.o
	g++ -g TextQuery.o Query.o newQueryMain.o -o main.exe

Real: TextQuery.o newQueryMain.o
	g++ -o TextQuery.o newQueryMain.o
clean: 
	find . -name "*.exe" -exec rm -rf {} \;
	find . -name "*.o" -exec rm -rf {} \;

