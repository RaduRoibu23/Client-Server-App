# ROIBU RADU GHEORGHE 325CB
CPP=g++
CPPFLAGS=-I. -Wall -Wextra -std=c++11

client: client.cpp requests.cpp helpers.cpp
	$(CPP) -o client client.cpp requests.cpp helpers.cpp $(CPPFLAGS)

run: client
	@./client

clean:
	rm -f *.o client

