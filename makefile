OOPTS = -g -std=c99 -Wall -Wextra -c
LOPTS = -g -std=c99 -Wall -Wextra
DAOBJS = da.o da-test2.o integer.o
CDAOBJS = cda.o test-cda3.o integer.o
SOBJS = stack.o da.o stack-test.o integer.o
QOBJS = queue.o cda.o test-queue.o integer.o
MOBJS = amaze.o maze.o cell.o stack.o queue.o da.o cda.o
COBJS = cell.o da.o

all : da stack cda queue maze amaze

da : $(DAOBJS)
	gcc $(LOPTS) $(DAOBJS) -o da
cda : $(CDAOBJS)
	gcc $(LOPTS) $(CDAOBJS) -o cda
stack : $(SOBJS)
	gcc $(LOPTS) $(SOBJS) -o stack
queue : $(QOBJS)
	gcc $(LOPTS) $(QOBJS) -o queue
maze : $(MOBJS)
	gcc $(LOPTS) $(MOBJS) -o maze
amaze : $(MOBJS)
	gcc $(LOPTS) $(MOBJS) -o amaze
integer.o : integer.c integer.h
	gcc $(OOPTS) integer.c
da.o : da.c da.h
	gcc $(OOPTS) da.c
stack.o : stack.c stack.h da.h
	gcc $(OOPTS) stack.c
cda.o : cda.c cda.h
	gcc $(OOPTS) cda.c
queue.o : queue.c queue.h cda.h
	gcc $(OOPTS) queue.c
cell.o : cell.c cell.h da.h
	gcc $(OOPTS) cell.c
maze.o : maze.c maze.h cell.c cell.h stack.h amaze.c
	gcc $(OOPTS) maze.c
amaze.o : maze.c maze.h cell.c cell.h stack.h
	gcc $(OOPTS) amaze.c
#da-test2.o : da-test2.c da.h
	#gcc $(OOPTS) da-test2.c
#test-cda3.o : test-cda3.c cda.h
	#gcc $(OOPTS) test-cda3.c
#stack-test.o : stack-test.c stack.h da.h
	#gcc $(OOPTS) stack-test.c
#test-queue.o : test-queue.c queue.h cda.h
	#gcc $(OOPTS) test-queue.c
test : maze #da stack cda queue
	#./da
	#./cda
	#./stack
	#./queue
	./amaze -r 2777 -c 83 16 ready.maze
valgrind : maze #da stack cda queue
	#valgrind --leak-check=full ./da
	#valgrind --leak-check=full ./cda
	#valgrind --leak-check=full ./stack
	#valgrind --leak-check=full ./queue
	valgrind --leak-check=full ./maze
clean :
	rm -f $(DAOBJS) $(SOBJS) $(CDAOBJS) $(QOBJS) $(COBJS) $(MOBJS) \
	da stack cda queue maze
