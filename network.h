//
//  network.h
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#ifndef network_h
#define network_h

#include <stdint.h>

#define NET_MAX_PAYLOAD 116
#define NET_HEADER_SIZE 12

struct net_packet_t {
    uint16_t source;
    uint16_t destination;
    uint16_t prev_hop;
    uint16_t seq;
    uint8_t size;
//    uint8_t route_control[3];
    uint8_t payload[NET_MAX_PAYLOAD];
} __attribute__((__packed__));
 
int net_init();
int net_cleanup();

int net_send(uint16_t destination, void* data, uint32_t data_size);
int net_recv(uint16_t* source, void* buffer, uint32_t buffer_size);

void net_hton(struct net_packet_t* packet);
void net_ntoh(struct net_packet_t* packet);


#endif /* network_h */
