#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<stdarg.h>
#include<string.h>
#include<ncurses.h>
#include<time.h>

#define SERVER_PORT 10120
#define BUFFER_SIZE 512
#define MESSAGE_MAX_SIZE 32



typedef struct packet{
    char mes1[BUFFER_SIZE];
    char mes2[BUFFER_SIZE];
    char mes3[BUFFER_SIZE];
}Packet;



typedef struct frame{
    int seq_no;
    Packet data;
}Frame;





void heartrate(char*m1,char*buf1)
{
	//Three Display Windows
	strncpy(m1, buf1, MESSAGE_MAX_SIZE);//read message from buffer
	WINDOW*win1; //create window 
	initscr();//open curses mode
	refresh();//refresh the screen
	win1=newwin(10,35,2,4);//build a window and specify it's length,width and the starting position
	box(win1,ACS_VLINE,ACS_HLINE);//draw the window
	mvwprintw(win1,1,1,"HeartRate:%s bmp",m1);//display string at one point
	wrefresh(win1);//refresh 
	delwin(win1);
}






void location(char*m2, char*buf2)
{
	//Location Display
	strncpy(m2, buf2, MESSAGE_MAX_SIZE); 
	WINDOW*win2;
	initscr();//open curses mode
	refresh();//refresh the screen
	win2=newwin(10,35,2,40);
	box(win2,ACS_VLINE,ACS_HLINE);
	mvwprintw(win2,1,1,"Location:[%s]",m2);
	wrefresh(win2);
	delwin(win2);
}






void oxygen(char*m3, char*buf3)
{
	//Oxygen-level Display
	strncpy(m3, buf3,MESSAGE_MAX_SIZE);
	WINDOW*win3;
	initscr();//open curses mode
	refresh();//refresh the screen
	win3=newwin(10,35,12,4);
	box(win3,ACS_VLINE,ACS_HLINE);
	mvwprintw(win3,1,1,"Air-Pack Oxygen Level:%s%%",m3);
	wrefresh(win3);
	delwin(win3);	
}





int main()
{
    Frame recv_frame;//Store received frame.
    char ACK[MESSAGE_MAX_SIZE] = "";
    /* Create UDP socket interface */
    struct sockaddr_in server_addr;
    
//    char buffer1[BUFFER_SIZE], buffer2[BUFFER_SIZE], buffer3[BUFFER_SIZE];
    memset((char *)&server_addr, 0, sizeof(server_addr));
    printf("Waiting for data from First Responder.\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    
    /* Create socket */
    int server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_socket_fd == -1)
    {
        perror("Create Socket Failed:\n");
        exit(1);
    }
    
    /* Bind socket interface */
    if(-1 == (bind(server_socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))))
    {
        perror("Server Bind Failed:\n");
        exit(1);
    }
    
    
    /* Data transmission */
    while(1)
    {
        /* Define an address used to get client address */
        struct sockaddr_in client_addr;
        int recv_len;
        socklen_t client_addr_length = sizeof(client_addr);
        
        
        /* Data receiver */
        bzero(recv_frame.data.mes1, BUFFER_SIZE);
        bzero(recv_frame.data.mes2, BUFFER_SIZE);
        bzero(recv_frame.data.mes3, BUFFER_SIZE);
        
        recv_len=recvfrom(server_socket_fd, &recv_frame, sizeof(recv_frame),0,(struct sockaddr*)&client_addr, &client_addr_length);//Receive data from First Responder.

        if(recv_len == -1)
        {
            perror("Receive Data Failed:\n");
            exit(1);
        }
        else
        {   WINDOW*win4;
	    win4=newwin(10,35,12,40);
	    mvwprintw(win4,1,1,"Receive frame %d successfully.\n", recv_frame.seq_no);
            sprintf(ACK,"%d",recv_frame.seq_no);//Pass sequence number to ACK.
	    wrefresh(win4);
            delwin(win4);
            if (sendto(server_socket_fd, ACK, sizeof(ACK),0,(struct sockaddr*)&client_addr, client_addr_length) < 0)//Send ACK to First Responder.
                perror("Send ACK error.\n");
        
        
            char mes1[MESSAGE_MAX_SIZE],mes2[MESSAGE_MAX_SIZE],mes3[MESSAGE_MAX_SIZE];
            bzero(mes1, MESSAGE_MAX_SIZE);
            bzero(mes2, MESSAGE_MAX_SIZE);
            bzero(mes3, MESSAGE_MAX_SIZE);
        
        
	    //Three Display Windows
	    heartrate(mes1,recv_frame.data.mes1);
	    location(mes2,recv_frame.data.mes2);
	    oxygen(mes3,recv_frame.data.mes3);	
        }
    }
    
    
    getch();
    endwin();
    close(server_socket_fd); 
    return 0; 
} 
