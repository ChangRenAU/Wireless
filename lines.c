//
//  lines.h
//  wireless project2
//
//  Created by Cecilia Ren on 3/30/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#include "lines.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

struct line_node {
    void* value;
    struct line_node* next;
};

struct line {
    unsigned int len;
    
    struct line_node* front;
    struct line_node* back;
    
    pthread_mutex_t lock;
    pthread_cond_t avail;
};

struct line_node* line_create_node(void* value){
    struct line_node* node = (struct line_node*)malloc(sizeof(struct line_node));
    
    node->value = value;
    node->next = NULL;
    
    return node;
}

int line_destroy_node(struct line_node* node){
    if(node != NULL){
        line_destroy_node(node->next);
        free(node);
    }
    return 0;
}

struct line* line_create(){
    struct line* line = (struct line*)malloc(sizeof(struct line));
    memset(line,0,sizeof(struct line));
    
    pthread_mutex_init(&line->lock,NULL);
    pthread_cond_init(&line->avail,NULL);
    
    return line;
}

int line_destroy(struct line* line){
    line_destroy_node(line->front);
    
    pthread_mutex_destroy(&line->lock);
    pthread_cond_destroy(&line->avail);
    
    free(line);
    return 0;
}

unsigned int line_push(struct line* line, void* value){
    unsigned int len;
    struct line_node* node = line_create_node(value);
    
    pthread_mutex_lock(&line->lock);
    
    if(line->len == 0){
        line->front = node;
        line->back = node;
    } else {
        line->back->next = node;
        line->back = node;
    }
    
    len = ++line->len;
    
    pthread_cond_signal(&line->avail);
    pthread_mutex_unlock(&line->lock);
    
    return len;
}

void* line_pop(struct line* line){
    struct line_node* node;
    void* value = NULL;
    
    pthread_mutex_lock(&line->lock);
    
    while((node = line->front) == NULL){
        pthread_cond_wait(&line->avail,&line->lock);
    }
    
    value = node->value;
    line->front = node->next;
    line->len--;
    
    pthread_mutex_unlock(&line->lock);
    
    free(node);
    return value;
}

unsigned int line_len(struct line* line){
    unsigned int len;
    
    pthread_mutex_lock(&line->lock);
    len = line->len;
    pthread_mutex_unlock(&line->lock);
    
    return len;
}



