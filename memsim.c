#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memsim.h"

int numFrames;
page* frameTable;

int main(int argc, char* argv[])
{
    char* tracename;
    int page_number, frame_no, done;
    int do_line, i;
    int no_events, disk_writes, disk_reads;
    int debugmode;
    enum repl replace;
    int allocated = 0;
    unsigned address;
    char rw;
    page Pvictim;
    FILE* trace;

    // check arguments
    if (argc < 5) 
    {
        printf( "Usage: ./memsim inputfile numberframes replacementmode debugmode \n");
        exit ( -1);
	}
	else 
    {
        tracename = argv[1];	
	    trace = fopen( tracename, "r");
	    if (trace == NULL ) 
        {
            printf( "Cannot open trace file %s \n", tracename);
            exit ( -1);
	    }
	    numFrames = atoi(argv[2]);
        if (numFrames < 1) 
        {
            printf( "Frame number must be at least 1\n");
            exit ( -1);
        }
        if (strcmp(argv[3], "lru\0") == 0)
            replace = LRU;
	    else if (strcmp(argv[3], "rand\0") == 0)
	        replace = RAND;
	    else if (strcmp(argv[3], "clock\0") == 0)
            replace = CLOCK;		 
	    else if (strcmp(argv[3], "fif\0") == 0)
            replace = FIFO;		 
        else 
	    {
            printf( "Replacement algorithm must be rand/fifo/lru/clock  \n");
            exit ( -1);
	    }

        if (strcmp(argv[4], "quiet\0") == 0)
            debugmode = 0;
	    else if (strcmp(argv[4], "debug\0") == 0)
            debugmode = 1;
        else 
	    {
            printf( "Replacement algorithm must be quiet/debug  \n");
            exit ( -1);
	    }
	}
	
    // initialise MMU
	done = createMMU (numFrames);
	if ( done == -1 ) 
    {
		printf( "Cannot create MMU" ) ;
		exit(-1);
    }

	no_events = 0 ;
	disk_writes = 0 ;
	disk_reads = 0 ;

    // process trace file
    do_line = fscanf(trace,"%x %c",&address,&rw);
	while (do_line == 2)
	{
	    page_number =  address >> pageoffset;
		frame_no = checkInMemory( page_number) ;                        /* ask for physical address */

		if ( frame_no == -1 )
		{
            disk_reads++ ;			                                    /* Page fault, need to load it into memory */
            if (debugmode) 
                printf( "Page fault %8d \n", page_number) ;

            if (allocated < numFrames)  			                    /* allocate it to an empty frame */
		    {
                frame_no = allocateFrame(page_number);
		        allocated++;
            }
            else
            {
                switch (replace) 
                {
                    case FIFO:
                        Pvictim = selectVictimFIFO(page_number);
                        break;
                    case LRU:
                        Pvictim = selectVictimLRU(page_number);
                        break;
                    case RAND:
                        Pvictim = selectVictimRAND(page_number);
                        break;
                    case CLOCK:
                        Pvictim = selectVictimCLOCK(page_number);
                        break; 
		        }

                if (Pvictim.modified)                                   /* need to know victim page and modified  */
	 	        {
                    disk_writes++;			    
                    if (debugmode) printf( "Disk write %8d \n", Pvictim.pageNo) ;
		        }
		        else
                {
                    if (debugmode) printf( "Discard    %8d \n", Pvictim.pageNo) ;
		        }

                frame_no = checkInMemory(page_number);
		    }
        }

        // handle access type
		if ( rw == 'R')
        {
		    if (debugmode) printf( "reading    %8d \n", page_number) ;
		}
		else if ( rw == 'W')
        {
            int f = checkInMemory(page_number);
            if (f != -1) frameTable[f].modified = 1;
		    if (debugmode) printf( "writting   %8d \n", page_number) ;
		}
		else 
        {
		    printf( "Badly formatted file. Error on line %d\n", no_events+1); 
		    exit (-1);
		}

		no_events++;
        do_line = fscanf(trace,"%x %c",&address,&rw);
	}

    // print stats
	printf( "total memory frames:  %d\n", numFrames);
	printf( "events in trace:      %d\n", no_events);
	printf( "total disk reads:     %d\n", disk_reads);
	printf( "total disk writes:    %d\n", disk_writes);
	printf( "page fault rate:      %.4f\n", (float) disk_reads/no_events);

    return 0;
}