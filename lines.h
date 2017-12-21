//
//  lines.c
//  wireless project2
//
//  Created by Cecilia Ren on 3/30/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//
#ifndef lines_h
#define lines_h

#include <stdint.h>

struct line;

struct line* line_create();
int line_destroy(struct line* line);

unsigned int line_push(struct line* line, void* value);
void* line_pop(struct line* line);
unsigned int line_len(struct line* line);

#endif /* lines_h */
