#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <pthread.h>
#include <numa.h>
#include <limits.h>

#define V_MAJOR 0
#define V_MINOR 0
#define V_BUG   1
#define USAGE fprintf(stderr, "Usage: %s [-h] [-n numas] [-c cpn] [-m mem]\n", argv[0])
#define B_TO_MB(x) x / 1000000
#define B_TO_GB(x) x / 1000000000

int main(int argc, char **argv)
{
    int i;
    int flags;
    int opt;
    int numas = 0;
    int sys_numas = 0;
    int cpn = 0;
    int sys_cores = 0;
    int max_cpn = 0;
    uint64_t mem = 0;
    uint64_t mem_per_node = 0;
    long long free64;
    int min_numa_index = 0;

    if(numa_available() == -1)
    {
        fputs("ERROR: numa library not available", stderr);
        return EXIT_FAILURE;
    }
    sys_numas = numa_num_configured_nodes();
    sys_cores = numa_num_configured_cpus();
    max_cpn = sys_cores / sys_numas;

    while((opt = getopt(argc, argv, "hn:c:m:")) != -1)
    {
        switch(opt)
        {
            case 'h':
                USAGE;
                puts("  -h          prints out help menu");
                puts("  -n <numas>  number of numa nodes to use. 0/default, use all available nodes");
                puts("  -c <cpn>    number of cores per numa node to use. 0/default, will use maximum number of cores per numa node");
                puts("  -m <mem>    total amount of memory to allocate in bytes. 0/default, will utilize all available memory");
                return EXIT_SUCCESS;
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
                mem = strtoull(optarg, NULL, 10);    
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

    if(numas > sys_numas)
    {
        fprintf(stderr, "ERROR: cannot use %d numa nodes, max is %d\n", numas, sys_numas);
        return EXIT_FAILURE;
    } 
    else if(numas == 0) numas = sys_numas;

    if(cpn > max_cpn)
    {
        fprintf(stderr, "ERROR: cannot use %d cpn, max is %d\n", cpn, max_cpn);
        return EXIT_FAILURE;
    } 
    else if(cpn == 0) cpn = max_cpn;

    mem_per_node = ULLONG_MAX;
    for(i = 0; i < numas; i++)
    {
        numa_node_size64(i, &free64);
        if(free64 < mem_per_node)
        {
            mem_per_node = free64;
            min_numa_index = i;
        }
    }
    if(mem / numas > mem_per_node)
    {
        fprintf(stderr, "ERROR: requested amount of memory cannot be evenly divided to the nodes\n\trequested %zu MB, divided equally to %d numa nodes = %zu MB/node\n\tnuma node %d has %zu MB available\n\tmax working memory %zu MB\n", B_TO_MB(mem), numas, B_TO_MB(mem / numas), min_numa_index, B_TO_MB(mem_per_node), B_TO_MB(mem_per_node * numas));
        return EXIT_FAILURE;
    }
    else if(mem == 0) mem = mem_per_node * numas;

    printf("~~~~~~~~~~~~~ NUMA BW V%d.%d.%d ~~~~~~~~~~~~~\n", V_MAJOR, V_MINOR, V_BUG);
    printf("Numas Nodes:       %d out of %d Nodes\n", numas, sys_numas);
    printf("Cores per Node:    %d out of %d Cores/Node\n", cpn, max_cpn);
    printf("Total Cores:       %d out of %d Cores\n", cpn * numas, sys_cores);
    printf("Memory Per Node:   %zu MB/Node\n", B_TO_MB(mem_per_node));
    printf("Total Memory:      %zu MB\n", B_TO_MB(mem));
    return 0;
}

