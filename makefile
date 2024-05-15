CXX = g++
CXXFLAGS = -Wall

p: mytest.cpp pqueue.o 
	$(CXX) $(CXXFLAGS) mytest.cpp pqueue.o -o proj3

d: driver.cpp pqueue.o 
	$(CXX) $(CXXFLAGS) driver.cpp pqueue.o -o proj3

pqueue.o: pqueue.h pqueue.cpp
	$(CXX) $(CXXFLAGS) -c pqueue.cpp

clean:
	rm *.o*
	rm *~ 

v:
	valgrind --leak-check=full --track-origins=yes ./proj3

r:
	./proj3

b:
	gdb ./proj3
