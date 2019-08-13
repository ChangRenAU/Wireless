//
//  cache_tb.c
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#include "cache_tb.h"

#include <stdlib.h>
#include <string.h>

struct cache_entry {
    uint16_t addr;
    uint16_t seq;
};

unsigned int cache_cap;
unsigned int cache_num;

struct cache_entry* cache_table;

uint16_t cache_thr_high = 65520;;
uint16_t cache_thr_low = 16;

int cache__compare(const void* a, const void* b){
    return ((const struct cache_entry*)a)->addr - ((const struct cache_entry*)b)->addr;
}

int cache_init(unsigned int size){
    cache_cap = size;
    cache_num = 0;
    
    cache_table = (struct cache_entry*)malloc(cache_cap*sizeof(struct cache_entry));
    memset(cache_table,0,cache_cap*sizeof(struct cache_entry));
    
    return 0;
}

int cache_cleanup(){
    free(cache_table);
    return 0;
}

int seq_check(uint16_t addr, uint16_t seq){
    struct cache_entry key = {.addr = addr}, *match;
    
    if(cache_num == 0){
        cache_table[0].addr = addr;
        cache_table[0].seq = seq;
        cache_num++;
        return 1;
    }
    
    match = bsearch(&key,cache_table,cache_num,sizeof(struct cache_entry),cache__compare);
    
    if(match == NULL){
        if(cache_num == cache_cap){
            cache_cap <<= 1;
            cache_table = (struct cache_entry*)realloc(cache_table,cache_cap*sizeof(struct cache_entry));
        }
        
        cache_table[cache_num].addr = addr;
        cache_table[cache_num].seq = seq;
        cache_num++;
        
        //sort the cache table
        qsort(cache_table,cache_num,sizeof(struct cache_entry),cache__compare);
        return 1;
    }
    
    if(match->seq < seq || (match->seq > cache_thr_high && seq < cache_thr_low)){
        match->seq = seq;
        return 1;
    }
    
    return 0;
}
