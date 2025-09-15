# compiler mode
CC = gcc
# compilation flags
FLAGS = -Wall -g
# object files 
OBJS = memsim.o mmu.o fifo.o lru.o rand.o clock.o

memsim: $(OBJS)
	$(CC) $(FLAGS) -o memsim $(OBJS)

# compilation rules
memsim.o: memsim.c memsim.h
mmu.o: mmu.c memsim.h
fifo.o: fifo.c memsim.h
lru.o: lru.c memsim.h
rand.o: rand.c memsim.h
clock.o: clock.c memsim.h

clean:
	rm -f *.o memsim