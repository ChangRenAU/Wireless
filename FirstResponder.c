#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<errno.h>
#include<netdb.h>
#include<stdarg.h>
#include<string.h>
#include<time.h>

#define SERVER_PORT 10120
#define BUFFER_SIZE 512
#define LOSS_RATE 0.2
#define MESSAGE_MAX_SIZE 25
#define random(x) (rand()%x)
#define pi 3.14



typedef struct packet{
    char mes1[BUFFER_SIZE];
    char mes2[BUFFER_SIZE];
    char mes3[BUFFER_SIZE];
}Packet;




typedef struct frame{
    int seq_no;
    Packet data;
}Frame;




int gremlin()
{
    srand(time(NULL));
    int bound = 100 - LOSS_RATE*100;
    if(rand()%100 >= bound)
    {
        return 0; //packet loss
    }
    else
    {
        return 1;
    }
}




void heartrate(char*mes1,char*buf1,int x1)
{
	int y1,b1;
	float a1;
	if(x1<=1200)
	{
		a1=0.1,b1=80;
		y1=a1*x1+b1;
		sprintf(mes1,"%d",y1);
	}
	if(x1>1200)
	{
		a1=(-1)/3.0,b1=600;
		y1=a1*x1+b1;
		sprintf(mes1,"%d",y1);
	}
	strncpy(buf1,mes1,strlen(mes1)>BUFFER_SIZE?BUFFER_SIZE:strlen(mes1));//store message in buffer
}




void location(char*mes2,char*buf2, int x2)
{

	int y2,z2;
	if (x2<=60)
	{
		z2=1;
	}
	else if(x2<=120)
	{
		z2=2;
		x2=x2-60;
	}
	else if(x2<=180)
	{
		z2=3;
		x2=x2-120;
	}
	else if(x2<=240)
	{
		z2=4;
		x2=x2-180;
	}
	else if(x2<=300)
	{
		z2=5;
		x2=x2-240;
	}
	else
	{
		z2=6;
		x2=x2-300;
		if(x2>60)
		{
			x2=10;
		}
	}
	

		y2=30*sin(pi*x2/60);

	char g1[25],g2[25],g3[25];
	bzero(g1, 25);
	bzero(g2, 25);
	bzero(g3, 25);
	sprintf(g1,"%d",x2);//change number to string
	sprintf(g2,"%d",y2);
	sprintf(g3,"%d",z2);
	sprintf(mes2,"%s,%s,%s",g1,g2,g3);
	strncpy(buf2,mes2,strlen(mes2)>BUFFER_SIZE?BUFFER_SIZE:strlen(mes2));
}





void oxygen(char*mes3,char*buf3,int x3)
{
	int b3;
	int sign=5;
	float y3,a3;
	a3=(-1)/1800.0, b3=1;
	y3=a3*x3+b3;
	y3=y3*100;
	gcvt(y3,sign,mes3);
	strncpy(buf3,mes3,strlen(mes3)>BUFFER_SIZE?BUFFER_SIZE:strlen(mes3));	
}






int main()
{
    /*Ip address of Fire chief*/
    struct sockaddr_in server_addr;//The address of dealing with network conn
    socklen_t server_addr_length = sizeof(server_addr);
    memset((char *)&server_addr,0,  server_addr_length);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("131.204.14.206");//Ip address of fire Chief
    server_addr.sin_port = htons(SERVER_PORT);
    
    /* Create socket */
    int client_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_socket_fd < 0)
    {
        perror("Create Socket Failed:");
        exit(1);
    }
    
    /* Input message into frame */
    //HeartRate and Oxygen-level Function

    Frame send_frame;
    int lossPacket = 0;
    int frame_id = 0;//Local sequence number of frame
    int recv_result;
    char ACK[MESSAGE_MAX_SIZE] = "";//Received ACK
    char exptACK[MESSAGE_MAX_SIZE] = "";//Expected sequence number of ACK
    
    char mes1[MESSAGE_MAX_SIZE],mes2[MESSAGE_MAX_SIZE],mes3[MESSAGE_MAX_SIZE];
    bzero(mes1, MESSAGE_MAX_SIZE);
    bzero(mes2, MESSAGE_MAX_SIZE);
    bzero(mes3, MESSAGE_MAX_SIZE);

    int i;
    for(i=0;i<=1800;i++)    
    {	
	//sent data
	bzero(send_frame.data.mes1, BUFFER_SIZE);
        bzero(send_frame.data.mes2, BUFFER_SIZE);
        bzero(send_frame.data.mes3, BUFFER_SIZE);
	
		heartrate(mes1,send_frame.data.mes1,i);
		location(mes2,send_frame.data.mes2,i);
		oxygen(mes3,send_frame.data.mes3,i);
        
        send_frame.seq_no = frame_id;
        
   
        while(1)
        {
            lossPacket = gremlin();//Determine if the frame loss
            struct timeval timeout={1,0};//1s
            setsockopt(client_socket_fd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
            
            if(lossPacket == 1)
            {
                if((sendto(client_socket_fd, &send_frame, sizeof(send_frame),0,(struct sockaddr*)&server_addr,server_addr_length)) < 0)
                {
                    perror("Send Data Failed:\n");
                    exit(1);
                }
                else
                {
                    printf("Frame %d sent.\n",frame_id); 
                    recv_result = recvfrom(client_socket_fd,ACK,sizeof(ACK),0,(struct sockaddr*)&server_addr,&server_addr_length);//Get ACK from Fire Chief
                    sprintf(exptACK,"%d",frame_id);
                    if(recv_result > 0 && (strcmp(ACK,exptACK)) == 0)//Check if the sequence number match.
                    {
                        printf("Ack %s received\n", ACK);
						printf("Frame %d sent successfully.Waiting for next frame...\n",frame_id);
                        break;
                    }
                    else
                    {
                        printf("Frame %d time expired\n", frame_id);
			sleep(1);
                    }

                }
            }
            else
            {
                printf("Frame loss, wait for re-transmitting.\n");
		sleep(1);
                continue;//Retransmitting
            }
        }
        frame_id++;//The sequence number of next frame
        sleep(1);
    }
    
    
    close(client_socket_fd);
    printf("Data Sent.\n");
    
    return 0;
} 
