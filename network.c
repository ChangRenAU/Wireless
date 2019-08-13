//
//  network.c
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#include "network.h"
#include "cache_tb.h"
#include "topo.h"
#include "flooding.h"
#include "udp.h"
#include "lines.h"

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

uint16_t local_address;

struct line* net_recv_line;

pthread_t net_run_thread;

void net_hton(struct net_packet_t* packet){
    packet->source = htons(packet->source);
    packet->destination = htons(packet->destination);
    packet->prev_hop = htons(packet->prev_hop);
    packet->seq = htons(packet->seq);
}

void net_ntoh(struct net_packet_t* packet){
    packet->source = ntohs(packet->source);
    packet->destination = ntohs(packet->destination);
    packet->prev_hop = ntohs(packet->prev_hop);
    packet->seq = ntohs(packet->seq);
}

struct net_packet_t* net__copy_packet(struct net_packet_t* in){
    struct net_packet_t* out = (struct net_packet_t*)malloc(sizeof(struct net_packet_t));
    memcpy(out,in,sizeof(struct net_packet_t));
    return out;
}

void* net__run(){
    struct net_packet_t packet = {0};
    
    while(1){
        udp_recv(NULL,&packet,sizeof(struct net_packet_t));
        net_ntoh(&packet);
        
//        if(packet.route_control[0] != 0){
//            route_control_packet(&packet);
//            continue;
//        }
        
        if(!seq_check(packet.source,packet.seq)){
            continue;
        }
        
        if(packet.destination == local_address){

            line_push(net_recv_line,net__copy_packet(&packet));
            continue;
        }
        
        route_dispatch_packet(&packet);
    }
}

int net_init(){
    udp_init();
    
    net_recv_line = line_create();
    
    struct topo_node_t* node = topo_get_local_node();
    local_address = node->address;
    topo_free_node(node);
    
    cache_init(topo_get_num_nodes());
    route_init();

    pthread_create(&net_run_thread,NULL,net__run,NULL);
    
    return 0;
}

int net_cleanup(){
    pthread_cancel(net_run_thread);
    pthread_join(net_run_thread,NULL);
    
    route_cleanup();
    cache_cleanup();
    
    line_destroy(net_recv_line);
    
    udp_cleanup();
    
    return 0;
}

int net_send(uint16_t destination, void* data, uint32_t data_size){
    static uint16_t seq = 0;
    struct net_packet_t packet = {0};
    
    packet.source = local_address;
    packet.destination = destination;
    packet.seq = seq++;
    
    if(data_size > NET_MAX_PAYLOAD){

        return -1;
    }
    
    packet.size = data_size + NET_HEADER_SIZE;
    memcpy(&packet.payload,data,data_size);
    
    route_dispatch_packet(&packet);
    return data_size;
}

int net_recv(uint16_t* source, void* buffer, uint32_t buffer_size){
    int size;
    struct net_packet_t* packet;

    packet = line_pop(net_recv_line);
    
    if(source != NULL){
        *source = packet->source;
    }
    
    size = packet->size - NET_HEADER_SIZE;
    
    memcpy(buffer,packet->payload,size);
    
    free(packet);
    
    return size;
}
