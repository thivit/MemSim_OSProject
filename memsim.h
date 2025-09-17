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
    RAND,
    FIFO,
    LRU,
    CLOCK
};

extern unsigned long long tick;
extern const int pageoffset;
extern int numFrames;           // number of frames (defined in memsim.c)
extern page* frameTable;        // frame table      (defined in mmu.c)

// main functions
int createMMU(int frames);
int checkInMemory(int page_number);
int allocateFrame(int page_number);

// policy selectors
page selectVictimFIFO(int page_number);
page selectVictimLRU(int page_number);
page selectVictimRAND(int page_number);
page selectVictimCLOCK(int page_number);

#endif