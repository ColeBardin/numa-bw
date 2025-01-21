#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define USAGE fprintf(stderr, "Usage: %s [-h] [-n numas] [-c cpn] [-m mem]\n", argv[0])

int main(int argc, char **argv)
{
    int flags;
    int opt;
    int numas = 0;
    int cpn = 0;
    int mem = 0;
    
    while((opt = getopt(argc, argv, "hn:c:m:")) != -1)
    {
        switch(opt)
        {
            case 'h':
                USAGE;
                puts("  -h        help, prints out the menu");
                puts("  -n numas  number of numa nodes to use. 0/default, use all available nodes");
                puts("  -c cpn    number of cores per numa node to use. 0/default, will use maximum number of cores per numa node");
                puts("  -m mem    total amount of memory to allocate in bytes. 0/default, will utilize all available memory");
            case 'n':
                numas = atoi(optarg);    
                if(numas < 0)
                {
                    fprintf(stderr, "ERROR: failed to parse n flag argument: %s\n", optarg); 
                    USAGE;
                    return EXIT_FAILURE;
                }
                break;
            case 'c':
                cpn= atoi(optarg);    
                if(cpn < 0)
                {
                    fprintf(stderr, "ERROR: failed to parse c flag argument: %s\n", optarg); 
                    USAGE;
                    return EXIT_FAILURE;
                }
                break;
            case 'm':
                mem = atoi(optarg);    
                if(mem < 0)
                {
                    fprintf(stderr, "ERROR: failed to parse m flag argument: %s\n", optarg); 
                    USAGE;
                    return EXIT_FAILURE;
                }
                break;
            default:
                fprintf(stderr, "ERROR: unrecognized argument: %c\n", opt);
                USAGE;
                return EXIT_FAILURE;
        }
    }

    printf("numas: %d\n", numas);
    printf("cpn: %d\n", cpn);
    printf("mem: %d\n", mem);
    return 0;
}

