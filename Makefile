EXECUTABLES = navigation

all : $(EXECUTABLES)

buildStructure.o : buildStructure.c
	gcc -c buildStructure.c
	
minPriorityQueue.o : minPriorityQueue.c
	gcc -c minPriorityQueue.c
	
dijkstra.o : dijkstra.c
	gcc -c dijkstra.c

partialSearch.o : partialSearch.c
	gcc -c partialSearch.c
	
menu.o : menu.c
	gcc -c menu.c

navigation : menu.o buildStructure.o minPriorityQueue.o dijkstra.o partialSearch.o
	gcc -o navigation menu.o buildStructure.o minPriorityQueue.o dijkstra.o partialSearch.o

clean :
	-rm *.o
	-rm navigation
