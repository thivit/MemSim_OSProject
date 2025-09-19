#include "memsim.h"
#include <stdlib.h>

/*
* LRU tracks when each frame was last accessed using tick counter from mmu.c
* Keeps track of current frames access time (tick)
* When deciding which frame to replace, just find the frame with smallest tick value
*/

static int* access_time = NULL;

// Called by MMU whenever a page is accessed (hit)
void set_reference_bit_lru(int frame_no)
{
    if (access_time == NULL)
    {
        access_time = malloc(numFrames * sizeof(int));
        for (int i = 0; i < numFrames; i++) access_time[i] = 0;
    }
    access_time[frame_no] = tick++;
}

page selectVictimLRU(int page_number)
{
    page victim;
    
    // Init
    if (access_time == NULL)
    {
        access_time = malloc(numFrames * sizeof(int));
        for (int i = 0; i < numFrames; i++) access_time[i] = 0;
    }
    
    // Find the frame with the smallest access time (LRU)
    int victim_frame = 0;
    for (int i = 1; i < numFrames; i++)
    {
        if (access_time[i] < access_time[victim_frame])
        {
            victim_frame = i;
        }
    }
    
    victim = frameTable[victim_frame];
    
    // Replace with new page
    load_into_frame(victim_frame, page_number);
    
    return victim;
}