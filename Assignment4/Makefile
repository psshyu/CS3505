a.out:	food.o report.o warehouse.o tester.o
		g++ -o a.out food.o report.o warehouse.o tester.o

food.o:	food.cpp food.h
		g++ -std=c++11 -c food.cpp

report.o:	report.cpp report.h food.h warehouse.h
		g++ -std=c++11 -c report.cpp

warehouse.o:	warehouse.cpp warehouse.h food.h
		g++ -std=c++11 -c warehouse.cpp

tester.o:	tester.cpp report.h
		g++ -std=c++11 -c tester.cpp
		
clean:	
		rm -f *.o a.out