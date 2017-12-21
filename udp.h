//
//  udp.h
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#ifndef udp_h
#define udp_h

#include <stdint.h>
#include <unistd.h>

int udp_init();
int udp_cleanup();

ssize_t udp_send(uint16_t destination, void* data, uint32_t data_size);
ssize_t udp_recv(uint16_t* source, void* buffer, uint32_t buffer_size);

#endif /* udp_h */
