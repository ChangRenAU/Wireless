//
//  test1.c
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.

#include "network.h"
#include "topo.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>



int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: test <node-address>\n");
        exit(1);
    }
    
    uint16_t address = atoi(argv[1]), from;
    char* message = "Hello, beauty!";
    char buffer[64];
    
    topo_init("examples/node3.txt",address);
    
    switch(address){
        case 19:
            while(1){
                net_recv(&from,buffer,64);
                printf("Message from %d: %s\n",from,buffer);
            }
            break;
            
        case 42:
            while(1){
                net_send(19,message,strlen(message)+1);
                sleep(1);
            }
            break;
            
        default:
            net_recv(&from,buffer,64);
            printf("Message from %d: %s\n",from,buffer);
            break;
    }
    net_cleanup();
    topo_cleanup();
    return 0;
}
