
scraper.out: main.o
	g++ main.o -o scraper

main.o: main.cpp
	g++ -c main.cpp

clean: 
	rm *.o scraper