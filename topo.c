//
//  nodes.c
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#include "topo.h"
#include "nodesEntry.h"
#include "flooding.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define MAX_LINKS 32

char* config_file;

struct nodes_entry* node_temp;
struct topo_node_t* topo_local_node;
uint16_t topo_local_address;

pthread_t topo_node_thread;

int topo__update_nodes(){
    char links_buf[64], *link;
    FILE* fp;
    struct topo_node_t* node;
    
    fp = fopen(config_file,"r");
    
    while(!feof(fp)){
        node = (struct topo_node_t*)malloc(sizeof(struct topo_node_t));
        
        fscanf(fp,"Node %hu %64[^,], %8s %hd %hd links %64[^\n]\n",
               &node->address,
               &node->real_address,
               &node->real_port,
               &node->loc.x,
               &node->loc.y,
               links_buf);
        
        node->links = (uint16_t*)malloc(MAX_LINKS*sizeof(uint16_t));
        
        //GET links from links_buf
        link = strtok(links_buf," ");
        while(link != NULL){
            node->links[node->num_links++] = atoi(link);
            link = strtok(NULL," ");
        }
        node->links = (uint16_t*)realloc(node->links,node->num_links*sizeof(uint16_t));
        
        nodes_set(node_temp,node->address,node);
    }
    fclose(fp);
    //get locol node info through local address
    topo_local_node = nodes_get(node_temp,topo_local_address);
    
    return 0;
}

int topo__update_loc(){
    //TODO: make nodes become mobile.
    return 0;
}

void* topo__run(){
    bool changeLoc = false;
    while(1){
        if (changeLoc)
            topo__update_loc();
        topo__update_nodes();
        route_update_links();
        changeLoc = true;
        
        sleep(1);
    }
}

int topo_init(const char* file, uint16_t local_address){
    config_file = (char*)malloc(strlen(file)+1);
    strcpy(config_file,file);
    
    topo_local_address = local_address;
    
    node_temp = nodes_create();
    topo__update_nodes();
    
    pthread_create(&topo_node_thread,NULL,topo__run,NULL);
    
    return 0;
}

int topo_cleanup(){

    pthread_cancel(topo_node_thread);
    pthread_join(topo_node_thread,NULL);
    free(config_file);
    nodes_destroy(node_temp);

    return 0;
}

struct topo_node_t* topo_get_local_node(){
    return topo_copy_node(topo_local_node);
}

struct topo_node_t* topo_get_node(uint16_t address){
    //get node info from address
    return topo_copy_node(nodes_get(node_temp,address));
}

unsigned int topo_get_num_nodes(){
    //return the number of nodes
    return nodes_num(node_temp);
}

uint32_t topo_drop_rate(uint16_t remote_node){
    //compute drop rate
    int x, y;
    int d;
    const struct topo_coord_t* a = &topo_local_node->loc;
    //get remote node info through its addr
    const struct topo_coord_t* b = &((struct topo_node_t*)nodes_get(node_temp,remote_node))->loc;
    
    x = a->x - b->x;
    x *= x;
    y = a->y - b->y;
    y *= y;
    x += y;
    //ignore the decimal part
    d = sqrt(x);
    
    return d;
}

struct topo_node_t* topo_alloc_node(){
    struct topo_node_t* node = (struct topo_node_t*)malloc(sizeof(struct topo_node_t));
    memset(node,0,sizeof(struct topo_node_t));
    return node;
}

struct topo_node_t* topo_copy_node(struct topo_node_t* node){
    if(node == NULL){
        return NULL;
    }
    
    struct topo_node_t* result = topo_alloc_node();
    memcpy(result,node,sizeof(struct topo_node_t));
    
    result->links = (uint16_t*)malloc(result->num_links*sizeof(uint16_t));
    memcpy(result->links,node->links,result->num_links*sizeof(uint16_t));
    
    return result;
}

int topo_free_node(struct topo_node_t* node){
    if(node->links != NULL){
        free(node->links);
    }
    free(node);
    return 0;
}
