CC=g++
CFLAGS=  -g
BIN=sphcache
OBJS=main.o SphMutexLock.o SphEpoller.o SphShmget.o SphCondition.o SphThread.o SphMyThread.o SphThreadPool.o
LIBS= -lpthread
%.o:%.cc
	$(CC) $(CFLAGS) -c $< -o $@  $(LIBS)
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
clean:
	rm -f *.o $(BIN)

