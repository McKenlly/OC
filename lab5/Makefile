CC = g++
CFLAGS = -std=c++11 -Wall -Werror -Wno-sign-compare -Wno-unused-result

PROG = lab5 
 
 so: 
	$(CC) $(CFLAGS) -fPIC -c usr/vector.cpp
	$(CC) $(CFLAGS) -fPIC -c usr/TVectorItem.cpp
	$(CC) $(CFLAGS) -fPIC -c usr/TIterator.cpp
	$(CC) -shared -o libvector.so   *.o
	rm *.o
stat:
	$(CC) $(CFLAGS) -L. mainRun.cpp -lvector -o main
dyn:
	g++ -rdynamic -o lmao main.cpp -ldl
