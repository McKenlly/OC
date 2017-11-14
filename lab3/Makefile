FLAGS= -std=c++11 -pthread -pedantic
#-Werror -Wno-sign-compare -Wno-long-long -lm 
CC=g++
PROJECT=main
FILE=TimSort
all: clear main delete

main: $(FILE).cpp
	$(CC) $(FLAGS) -c $(FILE).cpp
	$(CC) $(FLAGS) -o $(PROJECT) $(FILE).o -lm
	
clear:
	rm -rf $(FILE).o $(PROJECT)
delete:
	rm $(FILE).o
