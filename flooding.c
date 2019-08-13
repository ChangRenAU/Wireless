//
//  flooding.c
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#include "flooding.h"
#include "network.h"
#include "topo.h"
#include "udp.h"

#include <stdlib.h>
#include <stdio.h>

uint16_t local_address;
uint16_t num_links;
uint16_t* links;

int route_init(){
    struct topo_node_t* node = topo_get_local_node();
    
    local_address = node->address;
    num_links = node->num_links;
    
    links = node->links;
    node->links = NULL;
    
    topo_free_node(node);
    
    return 0;
}

int route_cleanup(){
    free(links);
    
    return 0;
}

int route_update_links(){
    struct topo_node_t* node = topo_get_local_node();
    
    free(links);
    
    num_links = node->num_links;
    
    links = node->links;
    node->links = NULL;
    
    topo_free_node(node);
    
    return 0;
}

int route_control_packet(struct net_packet_t* packet){
    return 0;
}

int route_dispatch_packet(struct net_packet_t* packet){
    int i;
    uint16_t prev_hop = packet->prev_hop, source = packet->source;
    uint8_t size = packet->size;
    packet->prev_hop = local_address;
    
    net_hton(packet);
    
    for(i=0;i<num_links;i++){
        if(links[i] != prev_hop && links[i] != source){
            
            if(topo_drop_rate(links[i]) < 100.0){
                udp_send(links[i],packet,size);
            }
        }
    }
    
    return 0;
}
