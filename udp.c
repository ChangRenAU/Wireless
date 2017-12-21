//
//  udp.c
//  wireless project2
//
//  Created by Cecilia Ren on 3/27/17.
//  Copyright © 2017 Cecilia Ren. All rights reserved.
//

#include "udp.h"
#include "topo.h"
#include "nodesEntry.h"

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

int udp_socket;
struct nodes_entry* addr_nodes;

int udp_init(){
    struct sockaddr_in addr = {0};
    struct topo_node_t* local_node = topo_get_local_node();
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(atoi(local_node->real_port));
    
    udp_socket = socket(AF_INET,SOCK_DGRAM,0);
    bind(udp_socket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
    
    topo_free_node(local_node);
    //create space to store the nodes info of destination nodes.
    addr_nodes = nodes_create();
    return 0;
}

int udp_cleanup(){
    nodes_destroy(addr_nodes);
    close(udp_socket);
    return 0;
}

ssize_t udp_send(uint16_t destination, void* data, uint32_t data_size){
    struct addrinfo hints = {0}, *addr;
    struct topo_node_t* node;
    //Get destination addr node info
    addr = nodes_get(addr_nodes,destination);
    
    if(addr == NULL){
        node = topo_get_node(destination);
        
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        
        getaddrinfo(node->real_address,node->real_port,&hints,&addr);
        
        nodes_set(addr_nodes,destination,addr);
        
        topo_free_node(node);
    }
    
    return sendto(udp_socket,data,data_size,0,addr->ai_addr,addr->ai_addrlen);
}

ssize_t udp_recv(uint16_t* source, void* buffer, uint32_t buffer_size){
    memset(buffer,0,buffer_size);
    return recvfrom(udp_socket,buffer,buffer_size,0,NULL,NULL);
}
