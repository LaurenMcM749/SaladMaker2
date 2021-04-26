
saladmaker: chef.c saladmaker.c
	gcc chef.c -o chef -pthread
	gcc saladmaker.c -o saladmaker -pthread

saladmaker.o: chef.c saladmaker.h 
	gcc -c chef.c
	gcc -c saladmaker.c

clean:
	rm *.o myhie
