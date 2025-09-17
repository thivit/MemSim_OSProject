#include "memsim.h"
#include <stdlib.h>

static int* referenceBits = NULL;
static int clockHand = 0;

page selectVictimCLOCK(int page_number)
{
    page victim;

    // initialize reference bits
    if(referenceBits == NULL)
    {
        referenceBits = malloc(numFrames * sizeof(int)); 
        for(int i = 0; i < numFrames; i++) referenceBits[i] = 0;
    }

    // replacement loop
    while(1)
    {
        // victim found
        if(referenceBits[clockHand] == 0)
        {
            // save old victim's info
            victim = frameTable[clockHand];

            // replace with new page
            load_into_frame(clockHand, page_number);

            // mark new page as recently used
            referenceBits[clockHand] = 1;

            // tick clock foward
            clockHand = (clockHand + 1) % numFrames;

            return victim;
        }
        // second chance
        else
        {
            // clear the bit
            referenceBits[clockHand] = 0;

            // tick clock foward
            clockHand = (clockHand + 1) % numFrames;
        }
    }
}