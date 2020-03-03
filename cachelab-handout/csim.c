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
    size_t size;
};

struct CACHE_LINE {
    bool valid;
    size_t tag;
    struct CACHE_LINE* last;
    struct CACHE_LINE* next;
};

struct WORK_SET {
    struct CACHE_LINE* head;
    struct CACHE_LINE* tail;
    int length;
};

typedef struct MEM_PARAM mem_param_t;
typedef struct TRACE_PARAM trace_param_t;
typedef struct CACHE_LINE cache_line_t;
typedef struct WORK_SET work_set_t;


void insertWorkSet(work_set_t* work_set, cache_line_t* cache_line) {
    cache_line_t* origin_head = work_set->head;
    work_set->head = cache_line;
    cache_line->next = origin_head;

    if (NULL != origin_head) {
        origin_head->last = cache_line;
    }
    else {
        work_set->tail = cache_line;
    }
}

cache_line_t* popWorkSet(work_set_t* work_set) {
    cache_line_t* pop_element;
    cache_line_t* last_element;
    cache_line_t* next_element;

    if (NULL == work_set->tail)
        return NULL;

    pop_element = work_set->tail;
    last_element = pop_element->last;
    next_element = pop_element->next;
    work_set->tail = last_element;

    if (NULL != last_element) {
        last_element->next = next_element;
    }

    if (NULL != next_element) {
        next_element->last = last_element;
    }

    return pop_element;
}

// cache_line_t* popWorkSet(work_set_t* work_set, cache_line_t* cache_line) {
//     cache_line_t* last_element;
//     cache_line_t* next_element;

//     last_element = cache_line->last;
//     next_element = cache_line->next;

//     if (NULL != last_element) {
//         last_element->next = next_element;
//     }

//     // if (NULL != next_element) {
//     //     next_element->last = last_element;
//     // }

//     return pop_element;
// }
/* 
 * isHit 
 *  0 : miss
 *  1 : Hit
 * -1 : full_cache
 */
int isWorkSetHit(work_set_t* work_set, cache_line_t cache_line) {
    // bool available = false;

    cache_line_t* head_cache = work_set->head;
    cache_line_t* tail_cache = work_set->tail;

    // workset should not be empty when load data
    if (NULL == head_cache) {
        exit(5);
    }

    while (head_cache != tail_cache) {
        // if (!head_cache->valid) {
        //     available = true;
        // }

        // Hit situation
        if (head_cache->valid && head_cache->tag == cache_line.tag)
            return 1;

        head_cache = head_cache->next;
    }

    // if search not hit , we arrive the WorkSet tail
    if (head_cache == tail_cache) {
        if (head_cache->valid && tail_cache->tag == cache_line.tag) {
            return 1;
        }
        else if (head_cache->valid && tail_cache->tag != cache_line.tag) {
            return -1;
        }
    }

    return 0;
}

work_set_t* InitWorkSet(int line_num) {
    if (line_num <= 0)
        exit(6);

    work_set_t* work_set;
    work_set = (work_set_t *)malloc(sizeof(work_set_t));
    memset(work_set, 0, sizeof(work_set_t));

    cache_line_t* head_cache;
    cache_line_t* last_cache;
    cache_line_t* iter_cache;

    head_cache = (cache_line_t *)malloc(sizeof(cache_line_t));
    memset(head_cache, 0, sizeof(cache_line_t));
    last_cache = head_cache;

    work_set->head = head_cache;
    work_set->tail = last_cache;

    for (int i = 1; i < line_num; i++) {
        iter_cache = (cache_line_t *)malloc(sizeof(cache_line_t));
        // last_cache->next = iter_cache;
        // last_cache = last_cache->next;
        insertWorkSet(work_set, iter_cache);
    }

    return work_set;
}

void disConnectCacheLine(work_set_t* work_set, cache_line_t* cache_line) {
    cache_line_t* last_cache = cache_line->last;
    cache_line_t* next_cache = cache_line->next;

    if (NULL != last_cache) {
        last_cache->next = next_cache;
    }

    if (NULL != next_cache) {
        next_cache->last = last_cache;
    }

    if (work_set->head == cache_line) {
        work_set->head = next_cache;
    }

    if (work_set->tail == cache_line) {
        work_set->tail = last_cache;
    }
    // return cache_line;
    // cache_line->valid = true;
    // cache_line->tag = cache_line.tag;
    // insertWorkSet(work_set, head_cache);
}

int loadCache(work_set_t* work_set, size_t tag) {
    bool available = false;

    cache_line_t* head_cache = work_set->head;
    // cache_line_t* tail_cache = work_set->tail;
    cache_line_t* available_cache = NULL;

    // workset should not be empty when load data
    if (NULL == head_cache) {
        exit(5);
    }

    // while (head_cache != tail_cache) {
    while (NULL != head_cache) {
        if (head_cache->valid && (head_cache->tag == tag)) {
            // pop_element
            // head_cache != NULL
            // cache_line_t* last_cache = head_cache->last;
            // cache_line_t* next_cache = head_cache->next;

            // if (NULL != last_cache) {
            //     last_cache->next = next_cache;
            // }

            // if (NULL != next_cache) {
            //     next_cache->last = last_cache;
            // }
            disConnectCacheLine(work_set, head_cache);
            head_cache->valid = true;
            head_cache->tag = tag;
            insertWorkSet(work_set, head_cache);
            return 1;
        }

        if (!head_cache->valid) {
            available = true;
            available_cache = head_cache;
        }

        head_cache = head_cache->next;
    }

    if (available) {
        // cache_line_t* last_cache = available_cache->last;
        // cache_line_t* next_cache = available_cache->next;

        // if (NULL != last_cache) {
        //     last_cache->next = next_cache;
        // }

        // if (NULL != next_cache) {
        //     next_cache->last = last_cache;
        // }
        disConnectCacheLine(work_set, available_cache);
        available_cache->valid = true;
        available_cache->tag = tag;
        insertWorkSet(work_set, available_cache);
        return 0;
    }
    else {
        // eviction
        cache_line_t* pop_cache_line = popWorkSet(work_set);
        pop_cache_line->valid = true;
        insertWorkSet(work_set, pop_cache_line);
        return -1;
    }
}


void print_cache_line(cache_line_t* cache_line) {
    if (NULL == cache_line) {
        return;
    }

    printf("CACHE_LINE : %p ", cache_line);
    printf("valid : %d ", cache_line->valid);
    printf("tag : %lx\n", cache_line->tag);
}

void print_work_set(work_set_t* work_set) {
    if (NULL == work_set) {
        return;
    }

    cache_line_t* travase_cache = work_set->head;
    printf("WORKSET\n");
    while (travase_cache != NULL) {
        print_cache_line(travase_cache);
        travase_cache = travase_cache->next;
    };
}

void print_reverse_work_set(work_set_t* work_set) {
    if (NULL == work_set) {
        return;
    }

    cache_line_t* travase_cache = work_set->tail;
    printf("WORKSET\n");
    while (travase_cache != NULL) {
        print_cache_line(travase_cache);
        travase_cache = travase_cache->last;
    };
}

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

struct MEM_PARAM processArgs(int argc, char* argv[]) {
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
    return operation == 'M' || operation == 'L' || operation == 'S';
}

// void init_cache() {
    
// }



size_t getTag(size_t address, int s, int b) {
    size_t tag;
    tag = address >> (s + b);
    return tag;
}

size_t getSetIndex(size_t address, int s, int b) {
    size_t set_mask;
    size_t set_index;
    set_mask = (1 << s) - 1;
    set_index = (address >> b) & set_mask;
    return set_index;
}

size_t getBlockOffset(size_t address, int s, int b) {
    size_t set_mask;
    size_t block_offset;
    set_mask = (1 << b) - 1;
    block_offset = address & set_mask;
    return block_offset;
}

// void LRU_Cache_Hit(work_set_t work_set, size_t tag) {
//     cache_line_t address = popWorkSet(work_set);

// }

/* 
 * test1 : test the insert function
 */
void test1() {
    printf("---------------------test1---------------------\n");
    // work_set_t* work_set = InitWorkSet(10);
    // print_work_set(work_set);
    work_set_t* work_set = (work_set_t*)malloc(sizeof(work_set_t));
    memset(work_set, 0, sizeof(work_set_t));
    print_work_set(work_set);
    print_reverse_work_set(work_set);

    printf("---------------------\n");
    cache_line_t* new_cache_line = (cache_line_t*)malloc(sizeof(cache_line_t));
    new_cache_line->valid = true;
    new_cache_line->tag = 0xffffffff;
    print_cache_line(new_cache_line);

    printf("---------------------\n");
    insertWorkSet(work_set, new_cache_line);
    print_work_set(work_set);
    print_reverse_work_set(work_set);

    // if we insert the same struct in the list,
    // we can triger a very interesting question
    // insertWorkSet(work_set, new_cache_line);
    // print_work_set(work_set);

    for (int i = 0; i < 10; i++) {
        new_cache_line = (cache_line_t*)malloc(sizeof(cache_line_t));
        new_cache_line->tag = i;
        insertWorkSet(work_set, new_cache_line);
    }

    print_work_set(work_set);
    print_reverse_work_set(work_set);
}

/* 
 * test2 : test the pop function
 */
void test2() {
    printf("---------------------test2 pop function---------------------\n");
    work_set_t* work_set = InitWorkSet(10);
    print_work_set(work_set);
    print_reverse_work_set(work_set);

    printf("---------------------\n");
    print_cache_line(work_set->head);
    print_cache_line(work_set->tail);

    printf("---------------------\n");
    cache_line_t* cache_line = popWorkSet(work_set);
    print_cache_line(cache_line);

    printf("---------------------\n");
    print_work_set(work_set);
    print_reverse_work_set(work_set);
}

/* 
 * test3 : test the insertWorkSet function
 */
void test3() {
    printf("---------------------test3 insertWorkSet function---------------------\n");
    work_set_t* work_set = InitWorkSet(10);
    print_work_set(work_set);
    print_reverse_work_set(work_set);

    printf("---------------------\n");
    cache_line_t* new_cache_line = (cache_line_t*)malloc(sizeof(cache_line_t));
    new_cache_line->valid = true;
    new_cache_line->tag = 0xffffffff;
    print_cache_line(new_cache_line);
    print_work_set(work_set);
    print_reverse_work_set(work_set);

    printf("---------------------\n");
    insertWorkSet(work_set, new_cache_line);
    print_work_set(work_set);
    print_reverse_work_set(work_set);
}

/* 
 * test4 : test the disConnectCacheLine function
 */
void test4() {
    printf("---------------------test4 disConnectCacheLine function---------------------\n");
    work_set_t* work_set = InitWorkSet(10);
    print_work_set(work_set);
    print_reverse_work_set(work_set);

    printf("---------------------\n");
    cache_line_t* new_cache_line = (cache_line_t*)malloc(sizeof(cache_line_t));
    new_cache_line->valid = true;
    new_cache_line->tag = 0xffffffff;
    print_cache_line(new_cache_line);
    print_reverse_work_set(work_set);

    printf("---------------------\n");
    insertWorkSet(work_set, new_cache_line);
    print_work_set(work_set);
    print_reverse_work_set(work_set);

    printf("-----------disconnect----------\n");
    disConnectCacheLine(work_set, new_cache_line);
    print_work_set(work_set);
    print_reverse_work_set(work_set);
    // work_set_t* work_set = InitWorkSet(10);
    // print_work_set(work_set);

    // printf("---------------------\n");
    // cache_line_t* new_cache_line = (cache_line_t*)malloc(sizeof(cache_line_t));
    // new_cache_line->valid = true;
    // new_cache_line->tag = 0xffffffff;
    // print_cache_line(new_cache_line);

    // printf("---------------------\n");
    // insertWorkSet(work_set, new_cache_line);
    // print_work_set(work_set);
}

/* 
 * test5 : test the loadCache function
 */
void test5() {
    printf("---------------------test5 loadCache function---------------------\n");
    work_set_t* work_set = InitWorkSet(10);
    print_work_set(work_set);
    print_reverse_work_set(work_set);

    printf("---------------------\n");
    cache_line_t* new_cache_line = (cache_line_t*)malloc(sizeof(cache_line_t));
    new_cache_line->valid = true;
    new_cache_line->tag = 0xffffffff;
    print_cache_line(new_cache_line);
    print_reverse_work_set(work_set);

    printf("---------------------\n");
    insertWorkSet(work_set, new_cache_line);
    print_work_set(work_set);
    print_reverse_work_set(work_set);

    int isHit;

    printf("----------hit situation---------\n");
    isHit = loadCache(work_set, 0xffffffff);
    printf("is Hit %d\n", isHit);


    printf("----------miss situation--------\n");
    // new_cache_line->tag = 0xaaa;
    isHit = loadCache(work_set, 0xaaa);
    printf("is Hit %d\n", isHit);

    printf("----------------eviction situation----------------\n");
    // warm up
    for (size_t i = 0; i < 10; i++) {
        isHit = loadCache(work_set, i);
        printf("is Hit %d\n", isHit);
    }

    // envict the cache
    isHit = loadCache(work_set, 0x11);
    printf("is Hit %d\n", isHit);
}

int main(int argc, char* argv[])
{
    mem_param_t mem_param = processArgs(argc, argv);
    trace_param_t trace_param;

    // int hit_count;
    // int miss_count
    // int eviction_count;

    // hit_count = 0;
    // miss_count = 0;
    // eviction_count = 0;

    size_t n_work_set = 1 << mem_param.s;
    size_t n_cache_line = 1 << mem_param.b;
    work_set_t** cache = (work_set_t**)malloc(n_work_set * sizeof(work_set_t*));

    int i;
    for (i = 0; i < n_work_set; i++) {
        cache[i] = InitWorkSet(n_cache_line);
    }

    for (i = 0; i < n_work_set; i++) {
        print_work_set(cache[i]);
    }

    bool isHit;

    FILE* fp = fopen(mem_param.tracefile, "r");
    while (fscanf(fp, "%c %lx,%ld", &trace_param.operation, &trace_param.address, &trace_param.size) != EOF) {
        if (check_operation(trace_param.operation)) {
            size_t block_offset = getBlockOffset(trace_param.address, mem_param.s, mem_param.b);
            size_t set_index = getSetIndex(trace_param.address, mem_param.s, mem_param.b);
            size_t tag = getTag(trace_param.address, mem_param.s, mem_param.b);
            printf("line record : %lx %lx %lx\n", tag, set_index, block_offset);
            isHit = loadCache(cache[set_index], tag);
            printf("%c %lx,%ld %d\n", trace_param.operation, trace_param.address, trace_param.size, isHit);
        }
    }

    // delete the space

    // printSummary(hit_count, miss_count, eviction_count);

    // work_set_t work_set;
    // work_set.head = NULL;
    // work_set.tail = NULL;
    
    // // cache_line_t cache_line;
    // // work_set.head = ;
    // // work_set.tail = ;
    // cache_matrix = ()malloc();
    // popWorkSet(work_set);
    // test1();
    // test2();
    // test3();
    // test4();
    // test5();
    return 0;
}