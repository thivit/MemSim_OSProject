#include <stdlib.h>
#include "memsim.h"

const int pageoffset = 12;

unsigned long long tick = 0;


int createMMU(int frames)
{
    frameTable = malloc(frames * sizeof(page));

    // check if malloc fails
    if(!frameTable) return -1;

    // initialise frames
    for(int i = 0; i < frames; i++)
    {
        frameTable[i].pageNo = -1;
        frameTable[i].modified = 0;
    }

    return 0;
}

int checkInMemory(int page_number)
{
    for(int i = 0; i < numFrames; i++)
    {
        if(frameTable[i].pageNo == page_number) 
        {
            if (set_reference_bit != NULL) 
                set_reference_bit(i);
            return i;
        }
    }

    // if page not found
    return -1;
}

int allocateFrame(int page_number)
{
    for(int i = 0; i < numFrames; i++)
    {
        // find freee slot
        if(frameTable[i].pageNo == -1)
        {
            frameTable[i].pageNo = page_number;
            frameTable[i].modified = 0;
            if (set_reference_bit != NULL)
                set_reference_bit(i);
            return i;
        }
    }

    // no free slot
    return -1;
}   

void load_into_frame(int frame_no, int page_number)
{
    frameTable[frame_no].pageNo = page_number;
    frameTable[frame_no].modified = 0;
}