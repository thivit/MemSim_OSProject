#ifndef MEMSIM_H
#define MEMSIM_H

// page structure
typedef struct 
{
    int pageNo;
    int modified;
} page;

// modes of page replacement
enum repl 
{
    random,
    fifo,
    lru,
    clock
};

const int pageoffset = 12;
// shared variables declared in mem.c
extern int numFrames;
extern page* frameTable;

// main functions
int createMMU(int frames);
int checkInMemory(int page_number);
int allocateFrame(int page_number);

// policy selectors
page selectVictimFIFO(int page_number);
page selectVictimLRU(int page_number);
page selectVictimRand(int page_number);
page selectVictimClock(int page_number);

#endif