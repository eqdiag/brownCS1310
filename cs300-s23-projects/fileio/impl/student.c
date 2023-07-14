#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdarg.h>

#include "../io300.h"



/*
    student.c
    Fill in the following stencils
*/

/*
    When starting, you might want to change this for testing on small files.
*/
#ifndef CACHE_SIZE
#define CACHE_SIZE 4096
#endif

#if(CACHE_SIZE < 4)
#error "internal cache size should not be below 4."
#error "if you changed this during testing, that is fine."
#error "when handing in, make sure it is reset to the provided value"
#error "if this is not done, the autograder will not run"
#endif

/*
   This macro enables/disables the dbg() function. Use it to silence your
   debugging info.
   Use the dbg() function instead of printf debugging if you don't want to
   hunt down 30 printfs when you want to hand in
*/
#define DEBUG_PRINT 0
#define DEBUG_STATISTICS 0

//CURRENT VERSION
//Using a kind of "last accessed block cache"
//Cache is just a single buffer of size CACHE_SIZE
//We keep track of the current CACHE_SIZE block from the file and the index into that block
//Also a dirty bit is used to determine if any new writes have been made to that block
//A read from the file occurs when the read is outside of the cache block
//A write occurs when we move outside the cache block and the current block is dirty

struct io300_file {
    /* read,write,seek all take a file descriptor as a parameter */
    int fd;
    /* this will serve as our cache */
    //char *cache;
    char * cache;


    // TODO: Your properties go here

    size_t start; //Beginning location of loaded cache block
    size_t cur; //Next read/write location in cache block
    size_t end; //Last readable location in loaded cache block
    int dirty; //Tracks if writes have been made to current cache (should we write back to disk)
    //[start,end) is the range of the cache that is valid and can be written back to disk

    /* Used for debugging, keep track of which io300_file is which */
    char *description;
    /* To tell if we are getting the performance we are expecting */
    struct io300_statistics {
        int read_calls;
        int write_calls;
        int seeks;
    } stats;
};

/*
    Assert the properties that you would like your file to have at all times.
    Call this function frequently (like at the beginning of each function) to
    catch logical errors early on in development.
*/
static void check_invariants(struct io300_file *f) {

    assert(f != NULL);
    assert(f->cache != NULL);
    assert(f->fd >= 0);

    // TODO: Add more invariants
    assert(f->start <= f->end);
    assert(f->start <= f->cur);
    assert(f->cur <= f->end);
    assert((f->end - f->start) <= CACHE_SIZE);
}

/*
    Wrapper around printf that provides information about the
    given file. You can silence this function with the DEBUG_PRINT macro.
*/
static void dbg(struct io300_file *f, char *fmt, ...) {
    (void)f; (void)fmt;
#if(DEBUG_PRINT == 1)
    static char buff[300];
    size_t const size = sizeof(buff);
    int n = snprintf(
        buff,
        size,
        // TODO: Add the fields you want to print when debugging
        "{desc: %s,start: %ld,cur: %ld,end: %ld,dirty: %d} -- ",
        f->description,
        f->start,
        f->cur,
        f->end,
        f->dirty
    );
    int const bytes_left = size - n;
    va_list args;
    va_start(args, fmt);
    vsnprintf(&buff[n], bytes_left, fmt, args);
    va_end(args);
    printf("%s", buff);
#endif
}



struct io300_file *io300_open(const char *const path, char *description) {
    if (path == NULL) {
        fprintf(stderr, "error: null file path\n");
        return NULL;
    }

    int const fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        fprintf(stderr, "error: could not open file: `%s`: %s\n", path, strerror(errno));
        return NULL;
    }

    struct io300_file *const ret = malloc(sizeof(*ret));
    if (ret == NULL) {
        fprintf(stderr, "error: could not allocate io300_file\n");
        return NULL;
    }

    ret->fd = fd;
    ret->cache = malloc(CACHE_SIZE);
    if (ret->cache == NULL) {
        fprintf(stderr, "error: could not allocate file cache\n");
        close(ret->fd);
        free(ret);
        return NULL;
    }
    ret->description = description;
    // TODO: Initialize your file
    ret->start = 0;
    ret->cur = 0;
    ret->end = 0;
    ret->dirty = 0;
    ret->stats.read_calls = 0;
    ret->stats.write_calls = 0;
    ret->stats.seeks = 0;

    

    check_invariants(ret);
    dbg(ret, "Just finished initializing file from path: %s\n", path);
    return ret;
}

int io300_seek(struct io300_file *const f, off_t const pos) {
    check_invariants(f);
    f->stats.seeks++;

    // TODO: Implement this
    //If position is in current cache block, just update cur
    if((f->start <= (size_t)pos) && ((size_t)pos <= f->end)){
        f->cur = pos;
        return f->cur;
    }
    //Othwerwise, need to flush cache
    int success = io300_flush(f);
    if(success == -1) return -1;

    //Then load new block starting at pos
    success = lseek(f->fd,pos,SEEK_SET);
    if(success == -1) return -1;
    ssize_t bytes_read = read(f->fd,f->cache,CACHE_SIZE);
    if(bytes_read == -1) return -1;
    f->start = pos;
    f->cur = f->start;
    f->end = f->start+bytes_read;
    
    return f->start;
}

int io300_close(struct io300_file *const f) {
    check_invariants(f);

#if(DEBUG_STATISTICS == 1)
    printf("stats: {desc: %s, read_calls: %d, write_calls: %d, seeks: %d}\n",
            f->description, f->stats.read_calls, f->stats.write_calls, f->stats.seeks);
#endif
    // TODO: Implement this
    int success = io300_flush(f);
    if(success == -1) return -1;
    if(close(f->fd) == -1) return -1;

    free(f->cache);
    free(f);
    return 0;
}

off_t io300_filesize(struct io300_file *const f) {
    check_invariants(f);
    struct stat s;
    int const r = fstat(f->fd, &s);
    if (r >= 0 && S_ISREG(s.st_mode)) {
        dbg(f,"Filesize?: %d\n",s.st_size);
        return s.st_size;
    } else {
        return -1;
    }
}

int io300_readc(struct io300_file *const f) {
    check_invariants(f);
    // TODO: Implement this
    int character;
    //Because little-endian, get last byte of int
    int success = io300_read(f,(char*)&character,1);
    if(success <= 0) return -1; //ERROR OR EOF
    dbg(f,"READ CHAR: %c\n",(char)character);
    return character;
}
int io300_writec(struct io300_file *f, int ch) {
    check_invariants(f);
    // TODO: Implement this
    char const c = (char)ch;
    int success = io300_write(f,&c,1);
    if(success <= 0) return -1;
    dbg(f,"WROTE CHAR: %c\n",(char)c);
    return (int)c;   
}

ssize_t io300_read(struct io300_file *const f, char *const buff, size_t const sz) {
    check_invariants(f);
    f->stats.read_calls++;
    // TODO: Implement this
    if(!sz) return 0;
    dbg(f,"READ CALL of sz = %ld\n",sz);

    
    size_t total_read = 0;
    while(total_read != sz){
        //How many bytes left to read in cache
        size_t amt_cached = f->end - f->cur;
        if(!amt_cached){

            //If nothing left to read in cache, flush it 
            dbg(f,"FLUSHING CACHE\n");
            int success = io300_flush(f);
            if(success == -1) return -1;
            
            //Refill cache from file
            ssize_t bytes_read = read(f->fd,f->cache,CACHE_SIZE);
            if(bytes_read == -1) return -1;
            if(bytes_read == 0) return total_read; //Disk says can't read anymore right now
            //Update part of cache that is valid
            f->end += bytes_read;
        }else{
            //Fill buff with some cache bytes
            size_t amt_left = sz - total_read;
            size_t cpy_size = amt_left <= amt_cached ? amt_left : amt_cached;
            memcpy(buff + total_read,f->cache + (f->cur - f->start),cpy_size);
            f->cur += cpy_size;
            total_read += cpy_size;
            dbg(f,"READ %d bytes from cache\n",cpy_size);
        }
    }
    return total_read;
}
ssize_t io300_write(struct io300_file *const f, const char *buff, size_t const sz) {
    check_invariants(f);
    f->stats.write_calls++;
    // TODO: Implement this
    if(!sz) return 0;
    dbg(f,"WRITE CALL of sz = %ld\n",sz);

    size_t total_written = 0;
    while(total_written != sz){
        //Bytes left in the cache before we flush it
        size_t bytes_left = CACHE_SIZE - (f->cur - f->start);
        dbg(f,"WRITE bytes left in cache = %ld\n",bytes_left);
        if(!bytes_left){
            //Flush cache
            dbg(f,"FLUSHING CACHE\n");
            int success = io300_flush(f);
            if(success == -1) return -1;

            ssize_t bytes_read = read(f->fd,f->cache,CACHE_SIZE);
            if(bytes_read == -1) return -1;
            f->end += bytes_read;
        }else{
            size_t write_sz = sz - total_written;
            size_t cpy_sz = write_sz <= bytes_left ? write_sz : bytes_left;
            memcpy(f->cache + (f->cur - f->start),buff + total_written,cpy_sz);
            //Update cur AND end (writing increases valid read range)
            f->cur += cpy_sz;
            if(f->cur > f->end) f->end += cpy_sz;
            total_written += cpy_sz;
            //Make sure marked as dirty so flush actually sends changes to disk
            f->dirty = 1;
            dbg(f,"WROTE %d bytes to cache\n",cpy_sz);
        }
    }
    return total_written;
}

int io300_flush(struct io300_file *const f) {
    check_invariants(f);
    // TODO: Implement this
    //Flush the cache, fail if all bytes aren't flushed
    //SUCCESS = 1,FAIL = -1

    //Only flush cache if it was written to
    if(f->dirty){
        size_t flush_bytes = f->end - f->start;
        if(!flush_bytes) return 1; //Don't do anything if there's nothing to flush
        off_t offset = lseek(f->fd,f->start,SEEK_SET); //Move file head to start of cache block in file
        if((size_t)offset != f->start) return -1; //Offset should be at start location
        ssize_t num_bytes = write(f->fd,f->cache,flush_bytes);
        if((size_t)num_bytes != flush_bytes) return -1; //Flush should be all or nothing
    }
    //Cache is now empty so set state appropriately
    f->start = f->end;
    f->cur = f->end;
    f->dirty = 0; //Reset dirty bit
    return 1;
}