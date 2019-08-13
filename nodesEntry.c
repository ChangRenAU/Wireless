//
//  nodesEntry.c
//  wireless project2
//
//  Created by Cecilia Ren on 3/28/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#include "nodesEntry.h"

struct node_entry {
    uint32_t key;
    void* value;
};

struct nodes_entry {
    struct node_entry* entries;
    uint32_t num;
    uint32_t max;
};

int nodes_compare(const void* a, const void* b)
{
    uint32_t c = ((const struct node_entry*)a)->key - ((const struct node_entry*)b)->key;
    return c;
}

struct node_entry* nodes_find(struct nodes_entry* nodes, uint32_t key){
    unsigned int i;
    struct node_entry k = {.key=key};
    
     for(i=0;i<nodes->num;i++){
        if(nodes_compare(&k,&nodes->entries[i]) == 0)
            return &nodes->entries[i];
    }
    return NULL;
}

struct nodes_entry* nodes_create()
{
    struct nodes_entry* nodes = (struct nodes_entry*)malloc(sizeof(struct nodes_entry));
    
    nodes->num = 0;
    nodes->max = 16;
    
    nodes->entries = (struct node_entry*)malloc(nodes->max*sizeof(struct node_entry));
    memset(nodes->entries,0,nodes->max*sizeof(struct node_entry));
    
    return nodes;
}

int nodes_set(struct nodes_entry* nodes, uint32_t key, void* value){
    struct node_entry* entry;
    
    entry = nodes_find(nodes,key);
    
    if(entry == NULL){
        if(nodes->num == nodes->max)
        {
            nodes->max = nodes->max*3;
            nodes->entries = (struct node_entry*)realloc(nodes->entries,nodes->max*sizeof(struct node_entry));
        }
        entry = &nodes->entries[nodes->num++];
        
        entry->key = key;
        entry->value = value;
    }
    return 0;
}

void* nodes_get(struct nodes_entry* nodes, uint32_t key){
    void* value = NULL;
    struct node_entry* entry;
        
    entry = nodes_find(nodes,key);
        
    if(entry != NULL){
        value = entry->value;
    }

    return value;
}

uint32_t nodes_num(struct nodes_entry* nodes){
    return nodes->num;
}

int nodes_destroy(struct nodes_entry* nodes){
    
    free(nodes->entries);
    free(nodes);
    return 0;
}



