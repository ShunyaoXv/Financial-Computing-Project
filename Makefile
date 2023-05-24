CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11 -lcurl -w -pthread

plot.exe:  main.o Engine.o multi.o Menu.o gnuplot_linux.o preprocess.o fetchData.o StockData.o tradingDayManager.o Vector.o configure.o Calculation.o
	$(CC) $(CFLAGS) -o plot.exe main.o Engine.o multi.o Menu.o gnuplot_linux.o preprocess.o fetchData.o StockData.o tradingDayManager.o Vector.o configure.o Calculation.o

main.o: Engine.h main.cpp
	$(CC) $(CFLAGS) -c main.cpp
	
multi.o: configure.h StockData.h tradingDayManager.h multi.cpp
	$(CC) $(CFLAGS) -c multi.cpp
	
Engine.o: Vector.h StockData.h gnuplot_linux.h Menu.h Engine.cpp
	$(CC) $(CFLAGS) -c Engine.cpp

Menu.o: configure.h Vector.h tradingDayManager.h StockData.h fetchData.h preprocess.h gnuplot_linux.h Menu.h Calculation.h multi.h Menu.cpp 
	$(CC) $(CFLAGS) -c Menu.cpp

gnuplot_linux.o: configure.h gnuplot_linux.h Vector.h gnuplot_linux.cpp
	$(CC) $(CFLAGS) -c gnuplot_linux.cpp
	
preprocess.o: configure.h Vector.h tradingDayManager.h StockData.h preprocess.h preprocess.cpp
	$(CC) $(CFLAGS) -c preprocess.cpp

fetchData.o: configure.h Vector.h tradingDayManager.h StockData.h fetchData.h multi.h fetchData.cpp
	$(CC) $(CFLAGS) -c fetchData.cpp

StockData.o: configure.h Vector.h tradingDayManager.h StockData.h StockData.cpp
	$(CC) $(CFLAGS) -c StockData.cpp

tradingDayManager.o: configure.h tradingDayManager.h tradingDayManager.cpp
	$(CC) $(CFLAGS) -c tradingDayManager.cpp

Vector.o: configure.h Vector.h Vector.cpp
	$(CC) $(CFLAGS) -c Vector.cpp
	
configure.o: configure.h configure.cpp
	$(CC) $(CFLAGS) -c configure.cpp
	
Calculation.o: StockData.h Vector.h Calculation.h Calculation.cpp 
	$(CC) $(CFLAGS) -c Calculation.cpp

clean:
	rm -rf plot.exe *.o