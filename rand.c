#include "memsim.h"
#include <stdio.h>
#include <stdlib.h>  

page selectVictimRAND(int page_number)
{
    tick++;
    int victim = rand() % numFrames;
    page out = frameTable[victim];
    load_into_frame(victim, page_number);
    return out;
}