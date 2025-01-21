#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define USAGE fprintf(stderr, "Usage: %s [-n numas] [-c cpn]\n", argv[0])

int main(int argc, char **argv)
{
    int numas;
    int cpn;
    int flags;
    int opt;
    
    numas = 0;
    cpn = 0;
    while((opt = getopt(argc, argv, "n:c:")) != -1)
    {
        switch(opt)
        {
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
            default:
                fprintf(stderr, "ERROR: unrecognized argument: %c\n", opt);
                USAGE;
                return EXIT_FAILURE;
        }
    }

    printf("numas: %d\ncpn: %d\n", numas, cpn);
    return 0;
}
