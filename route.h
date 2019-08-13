//
//  route.h
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#ifndef route_h
#define route_h

#include "network.h"

int route_init();
int route_cleanup();

int route_update_links();
int route_control_packet(struct net_packet_t* packet);

int route_dispatch_packet(struct net_packet_t* packet);

#endif /* route_h */
