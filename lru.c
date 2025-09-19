#include "memsim.h"

page selectVictimLRU(int page_number)
{
    tick++;
    int victim = 0;
    for (int i=1;i<numFrames;i++) {
        if (lastAccessTimes[i] < lastAccessTimes[victim]) victim =i;
    }
    page out = frameTable[victim];
    frameTable[victim].pageNo = page_number;
    frameTable[victim].modified = 0;

    loadTimes[vicctim] = lastAccessTimes[victim] = tick;
    clock_on_access(victim);
    return out;
}