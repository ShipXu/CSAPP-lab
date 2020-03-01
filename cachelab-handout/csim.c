#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cachelab.h"

struct MEM_PARAM {
    bool visable;
    int s;
    int e;
    int b;
    char* tracefile;
};

struct TRACE_PARAM {
    char operation;
    size_t address;
    int size;
};

typedef struct MEM_PARAM mem_param_t;
typedef struct TRACE_PARAM trace_param_t;

void print_mem_param(mem_param_t mem_param) {
    printf("MEM_PARAM \n");
    printf("visible : %d\n", mem_param.visable);
    printf("s : %d\n", mem_param.s);
    printf("e : %d\n", mem_param.e);
    printf("b : %d\n", mem_param.b);
    printf("tracefile : %s\n", mem_param.tracefile);
}

void print_trace_param(trace_param_t trace_param) {
    printf("TRACE_PARAM \n");
    printf("operation : %c\n", trace_param.operation);
    printf("address : %lx\n", trace_param.address);
    printf("size : %ld\n", trace_param.size);
}

void print_help() {
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n"
            "Options:\n"
            "-h         Print this help message.\n"
            "-v         Optional verbose flag.\n"
            "-s <num>   Number of set index bits.\n"
            "-E <num>   Number of lines per set.\n"
            "-b <num>   Number of block offset bits.\n"
            "-t <file>  Trace file.\n"
            "\n"
            "Examples:\n"
            "linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n"
            "linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
};

struct MEM_PARAM process_args(int argc, char* argv[]) {
    mem_param_t mem_param;

    if (argc > 10) {
        printf("too many arguments\n");
        exit(1);
    }
    else if (argc < 9) {
        printf("too few arguments\n");
        exit(2);
    }

    if (strchr(argv[1], 'v') != NULL) {
        mem_param.visable = true;
    };

    int cmd_opt;

    while (1) {
        cmd_opt = getopt(argc, argv, "hv::s:E:b:t:");

        if (cmd_opt == -1)
            break; 

        if (cmd_opt == '?') {
            printf("invaid arguments\n");
            exit(3);
        }

        /* Lets parse */
        switch (cmd_opt) {
            case 'h':
                print_help();
                exit(0);
                break;
            case 'v':
                mem_param.visable = true;
                break;
            case 's':
                mem_param.s = atoi(optarg);
                break;
            case 'E':
                mem_param.e = atoi(optarg);
                break;
            case 'b':
                mem_param.b = atoi(optarg);
                break;
            case 't':
                mem_param.tracefile = optarg;
                break;

            /* Error handle: Mainly missing arg or illegal option */
            case '?':
                printf("invaid arguments\n");
                break;
            default:
                printf("invaid arguments\n");
                break;
        }
    }

    print_mem_param(mem_param);
    return mem_param;
}

bool check_operation(char operation) {
    return operation == 'I' || operation == 'M' || operation == 'L' || operation == 'S';
}

void init_cache() {
    
}
// tag s size

int main(int argc, char* argv[])
{
    mem_param_t mem_param = process_args(argc, argv);
    trace_param_t trace_param;
    // cache_matrix;

    FILE* fp = fopen(mem_param.tracefile, "r");
    while (fscanf(fp, "%c %lx,%d", &trace_param.operation, &trace_param.address, &trace_param.size) != EOF) {
        if (check_operation(trace_param.operation)) {
            // ;
        }
    }

    printSummary(0, 0, 0);
    return 0;
}