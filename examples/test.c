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



int main(int argc, char* argv[]) {
    
    if(argc != 2) {
        printf("Usage: test <node-address>\n");
        exit(1);
    }
    
    uint16_t address = atoi(argv[1]), from;
    char* message = "hello beauty.";
    char buffer[64];
    
    //local node address
    topo_init("examples/node1.txt",address);
    net_init();
    
    switch(address){
        case 12:
            while(1){
                net_recv(&from,buffer,64);
                printf("Message from %d: %s\n",from,buffer);
            }
            break;
            
        case 1:
            while(1){
                net_send(12,message,strlen(message)+1);
                printf("Message send successfully. %s\n",message);
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
