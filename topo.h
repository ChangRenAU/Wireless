//
//  topo.h
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#ifndef topo_h
#define topo_h

#include <stdint.h>

struct topo_coord_t {
    int16_t x;
    int16_t y;
};

struct topo_node_t {
    uint16_t address;
    char real_address[64];
    char real_port[8];
    struct topo_coord_t loc;
    uint16_t num_links;
    uint16_t* links;
    
};
int topo_init(const char* file, uint16_t local_address);
int topo_cleanup();

struct topo_node_t* topo_get_local_node();
struct topo_node_t* topo_get_node(uint16_t address);
unsigned int topo_get_num_nodes();

uint32_t topo_drop_rate(uint16_t remote_node);

struct topo_node_t* topo_alloc_node();
struct topo_node_t* topo_copy_node(struct topo_node_t* node);
int topo_free_node(struct topo_node_t* node);


#endif /* topo_h */
