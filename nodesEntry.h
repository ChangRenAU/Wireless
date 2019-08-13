//
//  nodesEntry.h
//  wireless project2
//
//  Created by Cecilia Ren on 3/28/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#ifndef nodesEntry_h
#define nodesEntry_h

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

struct nodes_entry;
void* nodes_get(struct nodes_entry* nodes, uint32_t key);
int nodes_set(struct nodes_entry* nodes, uint32_t key, void* value);
struct nodes_entry* nodes_create();
uint32_t nodes_num(struct nodes_entry* nodes);
int nodes_destroy(struct nodes_entry* nodes);

#endif /* nodesEntry_h */
