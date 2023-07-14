#define DMALLOC_DISABLE 1
#include "dmalloc.hh"
#include <cassert>
#include <cstring>

struct dmalloc_stats g_stats = {
    .nactive = 0,         // # active allocations
    .active_size = 0,     // # bytes in active allocations
    .ntotal = 0,          // # total allocations
    .total_size = 0,      // # bytes in total allocations
    .nfail = 0,           // # failed allocation attempts
    .fail_size = 0,
    .heap_min = 0xFFFFFFFF, //Highest possible uintptr_t
    .heap_max = 0, //Lowest possible uintptr_t
};

//Ptr returned by dmalloc -> to record of allocation
std::unordered_map<void*,alloc_record> g_freed_set;


/**
 * dmalloc(sz,file,line)
 *      malloc() wrapper. Dynamically allocate the requested amount `sz` of memory and 
 *      return a pointer to it 
 * 
 * @arg size_t sz : the amount of memory requested 
 * @arg const char *file : a string containing the filename from which dmalloc was called 
 * @arg long line : the line number from which dmalloc was called 
 * 
 * @return a pointer to the heap where the memory was reserved
 */
void* dmalloc(size_t sz, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // Your code here.

    if(sz >= 0xFFFFFFFF){
        g_stats.nfail++;
        g_stats.fail_size+=sz;
        return nullptr;
    }

    size_t real_size = sizeof(alloc_header) + sz + sizeof(int);

    void * result = base_malloc(real_size);
    if(!result){
        g_stats.nfail++;
        g_stats.fail_size+=sz;
        return nullptr;
    }

    //Allocation succeeded if we get here
    alloc_header * header = (alloc_header *)result;

    //Add header then return payload
    header->size = sz;
    header->free = false;
    header->mem_start = (size_t)(header + 1);
    header->header_marker = MARKER;


    int * marker_p = (int*)(header->mem_start + sz);
    *marker_p = MARKER;

    result = (void*)header->mem_start;

    struct alloc_record record{
        .file = file,
        .line = line,
        .header = header,
        .freed = false, 
    };

    g_freed_set[result] = record;

    //Update global struct here
    g_stats.nactive++;
    g_stats.active_size+=sz;
    g_stats.ntotal++;
    g_stats.total_size+=sz;

    //Update heap min/max
    uintptr_t start = (uintptr_t)result;
    uintptr_t end = start + sz;
    if(start < g_stats.heap_min) g_stats.heap_min = start;
    if(end > g_stats.heap_max) g_stats.heap_max = end;

    return result;
}

/**
 * dfree(ptr, file, line)
 *      free() wrapper. Release the block of heap memory pointed to by `ptr`. This should 
 *      be a pointer that was previously allocated on the heap. If `ptr` is a nullptr do nothing. 
 * 
 * @arg void *ptr : a pointer to the heap 
 * @arg const char *file : a string containing the filename from which dfree was called 
 * @arg long line : the line number from which dfree was called 
 */
void dfree(void* ptr, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // Your code here.
    if(!ptr) return;

    


    //Pointer outside heap
    if((size_t)ptr < g_stats.heap_min || (size_t)ptr > g_stats.heap_max){
        fprintf(stderr,"MEMORY BUG: %s:%ld: invalid free of pointer %p, not in heap\n",file,line,ptr);
        abort();
    }

    alloc_header* real_ptr = ((alloc_header*)ptr) - 1;


    //Check for invalid free address (not beginning of block)
    if(real_ptr->mem_start != (size_t)ptr){
        fprintf(stderr,"MEMORY BUG: %s:%ld: invalid free of pointer %p, not allocated\n",file,line,ptr);

        //Check if ptr is inside allocated block
        for(auto& [block_p,record]: g_freed_set){
            alloc_header* header = record.header;
            if((size_t)ptr > header->mem_start && (size_t)ptr < (header->mem_start + header->size)){
                fprintf(stderr,"%s:%ld: %p is %ld bytes inside a %lld byte region allocated here\n",
                record.file,record.line,ptr,(size_t)ptr - header->mem_start,header->size);
            }
        }

        abort();
    }

    //Check for double-free
    if(real_ptr->free || g_freed_set[ptr].freed){
        fprintf(stderr,"MEMORY BUG: invalid free of pointer %p, double free\n",ptr);
        abort();
    }



    //Check for boundary writing error at end of memory
    int * start_marker_p = &(real_ptr->header_marker);
    int * end_marker_p = (int*)(real_ptr->mem_start + real_ptr->size);
    //Check end of memory first
    if(*end_marker_p != (int)MARKER || *start_marker_p != (int)MARKER){
        fprintf(stderr,"MEMORY BUG: detected wild write during free of pointer %p\n",ptr);
        abort();
    }


    real_ptr->free = true;
    g_freed_set[ptr].freed = true;
    g_stats.nactive--;
    g_stats.active_size-= real_ptr->size;

    //Ensure we free alloc metadata too

    base_free((void*)real_ptr);
}

/**
 * dcalloc(nmemb, sz, file, line)
 *      calloc() wrapper. Dynamically allocate enough memory to store an array of `nmemb` 
 *      number of elements with wach element being `sz` bytes. The memory should be initialized 
 *      to zero  
 * 
 * @arg size_t nmemb : the number of items that space is requested for
 * @arg size_t sz : the size in bytes of the items that space is requested for
 * @arg const char *file : a string containing the filename from which dcalloc was called 
 * @arg long line : the line number from which dcalloc was called 
 * 
 * @return a pointer to the heap where the memory was reserved
 */
void* dcalloc(size_t nmemb, size_t sz, const char* file, long line) {
    // Your code here (to fix test014).

    size_t total_size = nmemb*sz;

    if(sz !=0 &&  total_size / sz != nmemb){
        g_stats.nfail++;
        g_stats.fail_size+= sz*nmemb;
        return nullptr;
    }

    void* ptr = dmalloc(nmemb * sz, file, line);
    if (ptr) {
        memset(ptr, 0, nmemb * sz);
    }
    return ptr;
}

/**
 * get_statistics(stats)
 *      fill a dmalloc_stats pointer with the current memory statistics  
 * 
 * @arg dmalloc_stats *stats : a pointer to the the dmalloc_stats struct we want to fill
 */
void get_statistics(dmalloc_stats* stats) {
    // Stub: set all statistics to enormous numbers
    //memset(stats, 255, sizeof(dmalloc_stats));
    memcpy(stats,&g_stats,sizeof(dmalloc_stats));
}

/**
 * print_statistics()
 *      print the current memory statistics to stdout       
 */
void print_statistics() {
    dmalloc_stats stats;
    get_statistics(&stats);

    printf("alloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("alloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}

/**  
 * print_leak_report()
 *      Print a report of all currently-active allocated blocks of dynamic
 *      memory.
 */
void print_leak_report() {
    // Your code here.
    for(auto& [ptr,record]: g_freed_set){
        alloc_header* header = record.header;
        if(!header->free){
            printf("LEAK CHECK: %s:%ld: allocated object %p with size %lld\n",
            record.file,record.line,ptr,header->size);
        }
    }
}
