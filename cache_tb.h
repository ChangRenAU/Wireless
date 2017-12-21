//
//  cache_tb.h
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#ifndef cache_tb_h
#define cache_tb_h

#include <stdint.h>

int cache_init(unsigned int size);
int cache_cleanup();

int seq_check(uint16_t addr, uint16_t seq);

#endif /* cache_tb_h */
