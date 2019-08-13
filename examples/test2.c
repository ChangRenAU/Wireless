//
//  test1.c
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Usage: test1 <node-address>\n");
        exit(1);
    }
    
    uint16_t address = atoi(argv[1]), from;
    char* message = "Hello, world!";
    char buffer[64];
    
    topo_init("examples/node2.txt",address,NULL);
    
    switch(address){
        case 19:
            net_recv(&from,buffer,64);
            printf("Message from %d: %s\n",from,buffer);
            net_send(42,message,strlen(message)+1);
            net_send(23,message,strlen(message)+1);
            break;
            
        case 23:
            net_recv(&from,buffer,64);
            printf("Message from %d: %s\n",from,buffer);
            break;
            
        case 42:
            net_send(19,message,strlen(message)+1);
            net_recv(&from,buffer,64);
            printf("Message from %d: %s\n",from,buffer);
            break;
    }
    
    net_cleanup();
    topo_cleanup();
    return 0;
}
