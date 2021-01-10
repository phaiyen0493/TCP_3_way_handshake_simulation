/*Name: Yen Pham
CS3530
Project 3 - Simulate TCP 3-way handshake and closing a TCP connection in the application layer 
using a client-server architecture.
*/

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <signal.h>
#include <wchar.h>
#include <time.h> 
#include <math.h>
#include "TCP_segment.h"
#define PORT 11485

void client_three_way_handshake(int socket_fd)
{
	struct tcpheader *TCP_segment; //Create a connection request TCP segment
	TCP_segment = malloc(sizeof(struct tcpheader));
	FILE *fp;
	fp = fopen("client.out", "w"); //write mode

	//Get client port number
	struct sockaddr_in local_addr;
	int addr_size = sizeof(local_addr);
	getsockname(socket_fd, (struct sockaddr *) &local_addr, &addr_size);

	printf("Server port: %d\n", PORT);
	printf("Client port: %d\n", ntohs(local_addr.sin_port));
	
	TCP_segment->srcport = ntohs(local_addr.sin_port);
	TCP_segment->destport = PORT;

	//srand(time(NULL));
	TCP_segment->seqnum = rand()% (int) (pow(2,32));
	unsigned int temp_seqnum = TCP_segment->seqnum;
	TCP_segment->acknum = 0;
	int header_length = (16+16+32+32+4+6+6+16+16+16+32)/32;
	TCP_segment->offset = header_length; //24 bytes = 192 bits, 192/32=6
	TCP_segment->reserved = 0;
	TCP_segment->urg = 0;
	TCP_segment->ack = 0;
	TCP_segment->psh = 0;
	TCP_segment->rst = 0;
	TCP_segment->syn = 1; //Set the SYN bit to 1
	TCP_segment->fin = 0;
	TCP_segment->window = 0;
	TCP_segment->checksum = 0;
	TCP_segment->pointer = 0;
	TCP_segment->option = 0;

	//Calculate checksum
	unsigned short int checksum_arr[12];
	unsigned int sum=0, checksum, wrap;

	memcpy(checksum_arr, TCP_segment, 24); //Copying 24 bytes

	for (int i=0;i<12;i++)
	{
		//printf("0x%04X\n", checkksum_arr[i]);
		sum = sum + checksum_arr[i];
	}

	wrap = sum >> 16;// Wrap around once  
	sum = sum & 0x0000FFFF;   
	sum = wrap + sum;  
	wrap = sum >> 16;// Wrap around once more  
	sum = sum & 0x0000FFFF;  
	checksum = wrap + sum;  
	//printf("\nSum Value: 0x%04X\n", checksum);  /* XOR the sum for checksum */  
	//printf("\nChecksum Value: 0x%04X\n", (0xFFFF^checksum));

	TCP_segment->checksum = checksum;
	int length = send(socket_fd, TCP_segment, 24, 0);//send message to server
	if (length <= 0)
	{
		printf("Fail to send SYN signal to request connection\n");
	}
	else
	{
		//Print values
		printf("\nClient has succesfully sent SYN signal to request connection\n");
		printf("TCP source port: %d\n", TCP_segment->srcport);
		printf("TCP destination port: %d\n", TCP_segment->destport);
		printf("TCP sequence number: %d\n", TCP_segment->seqnum);
		printf("TCP ack number: %d\n", TCP_segment->acknum);
		printf("TCP offset/ header length: %d\n", TCP_segment->offset);
		printf("TCP URG bit value: %d\n", TCP_segment->urg);
		printf("TCP ACK bit value: %d\n", TCP_segment->ack);
		printf("TCP PSH bit value: %d\n", TCP_segment->psh);
		printf("TCP RST bit value: %d\n", TCP_segment->rst);
		printf("TCP SYN bit value: %d\n", TCP_segment->syn);
		printf("TCP FIN bit value: %d\n", TCP_segment->fin);
		printf("TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);

		//Print values to file client.out
		fprintf(fp, "\nServer has sent SYN ACK signal to client succesfully\n");
		fprintf(fp, "TCP source port: %d\n", TCP_segment->srcport);
		fprintf(fp, "TCP destination port: %d\n", TCP_segment->destport);
		fprintf(fp, "TCP sequence number: %d\n", TCP_segment->seqnum);
		fprintf(fp, "TCP ack number: %d\n", TCP_segment->acknum);
		fprintf(fp, "TCP offset/ header length: %d\n", TCP_segment->offset);
		fprintf(fp, "TCP URG bit value: %d\n", TCP_segment->urg);
		fprintf(fp, "TCP ACK bit value: %d\n", TCP_segment->ack);
		fprintf(fp, "TCP PSH bit value: %d\n", TCP_segment->psh);
		fprintf(fp, "TCP RST bit value: %d\n", TCP_segment->rst);
		fprintf(fp, "TCP SYN bit value: %d\n", TCP_segment->syn);
		fprintf(fp, "TCP FIN bit value: %d\n", TCP_segment->fin);
		fprintf(fp, "TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);
	}

	//Receive ACK SYN message from server
	memset(TCP_segment, 0, sizeof(struct tcpheader));	//reallocate memory
	int rec_bytes = recv(socket_fd, TCP_segment, sizeof(struct tcpheader), 0);
	if (rec_bytes > 0)
	{
		printf("\nClient received SYN ACK signal from server\n");
		//Print values
		printf("TCP source port: %d\n", TCP_segment->srcport);
		printf("TCP destination port: %d\n", TCP_segment->destport);
		printf("TCP sequence number: %d\n", TCP_segment->seqnum);
		printf("TCP ack number: %d\n", TCP_segment->acknum);
		printf("TCP offset/ header length: %d\n", TCP_segment->offset);
		printf("TCP URG bit value: %d\n", TCP_segment->urg);
		printf("TCP ACK bit value: %d\n", TCP_segment->ack);
		printf("TCP PSH bit value: %d\n", TCP_segment->psh);
		printf("TCP RST bit value: %d\n", TCP_segment->rst);
		printf("TCP SYN bit value: %d\n", TCP_segment->syn);
		printf("TCP FIN bit value: %d\n", TCP_segment->fin);
		printf("TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);

		fprintf(fp, "\nClient received SYN ACK signal from server\n");
		fprintf(fp, "TCP source port: %d\n", TCP_segment->srcport);
		fprintf(fp, "TCP destination port: %d\n", TCP_segment->destport);
		fprintf(fp, "TCP sequence number: %d\n", TCP_segment->seqnum);
		fprintf(fp, "TCP ack number: %d\n", TCP_segment->acknum);
		fprintf(fp, "TCP offset/ header length: %d\n", TCP_segment->offset);
		fprintf(fp, "TCP URG bit value: %d\n", TCP_segment->urg);
		fprintf(fp, "TCP ACK bit value: %d\n", TCP_segment->ack);
		fprintf(fp, "TCP PSH bit value: %d\n", TCP_segment->psh);
		fprintf(fp, "TCP RST bit value: %d\n", TCP_segment->rst);
		fprintf(fp, "TCP SYN bit value: %d\n", TCP_segment->syn);
		fprintf(fp, "TCP FIN bit value: %d\n", TCP_segment->fin);
		fprintf(fp, "TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);

		//The client responds back with an acknowledgement TCP segment.
		TCP_segment->srcport = ntohs(local_addr.sin_port);
		TCP_segment->destport = PORT;
		TCP_segment->acknum = TCP_segment->seqnum + 1; //Acknowledgement number equal to initial server sequence number + 1
		TCP_segment->seqnum = temp_seqnum + 1; //Assign a sequence number as initial client sequence number + 1
		TCP_segment->offset = header_length; //24 bytes = 192 bits, 192/32=6
		TCP_segment->reserved = 0;
		TCP_segment->urg = 0;
		TCP_segment->ack = 1; //Set the ACK bit to 1
		TCP_segment->psh = 0;
		TCP_segment->rst = 0;
		TCP_segment->syn = 0;
		TCP_segment->fin = 0;
		TCP_segment->window = 0;
		TCP_segment->checksum = 0;
		TCP_segment->pointer = 0;
		TCP_segment->option = 0;

		unsigned short int checksum_arr1[12];
		sum=0; checksum = 0; wrap = 0;
		memcpy(checksum_arr1, TCP_segment, 24); //Copying 24 bytes
		for (int i=0;i<12;i++)
		{
			//printf("0x%04X\n", checkksum_arr[i]);
			sum = sum + checksum_arr1[i];
		}

		wrap = sum >> 16;// Wrap around once  
		sum = sum & 0x0000FFFF;   
		sum = wrap + sum;  
		wrap = sum >> 16;// Wrap around once more  
		sum = sum & 0x0000FFFF;  
		checksum = wrap + sum;  
		//printf("\nSum Value: 0x%04X\n", checksum);  /* XOR the sum for checksum */  
		//printf("\nChecksum Value: 0x%04X\n", (0xFFFF^checksum));

		TCP_segment->checksum = checksum;
		length = send(socket_fd, TCP_segment, 24, 0);//send message to server
		if (length <= 0)
		{
			printf("Fail to send ACK signal to request connection\n");
		}
		else
		{
			printf("\nClient has succesfully sent ACK signal to sever\n");
			printf("TCP source port: %d\n", TCP_segment->srcport);
			printf("TCP destination port: %d\n", TCP_segment->destport);
			printf("TCP sequence number: %d\n", TCP_segment->seqnum);
			printf("TCP ack number: %d\n", TCP_segment->acknum);
			printf("TCP offset/ header length: %d\n", TCP_segment->offset);
			printf("TCP URG bit value: %d\n", TCP_segment->urg);
			printf("TCP ACK bit value: %d\n", TCP_segment->ack);
			printf("TCP PSH bit value: %d\n", TCP_segment->psh);
			printf("TCP RST bit value: %d\n", TCP_segment->rst);
			printf("TCP SYN bit value: %d\n", TCP_segment->syn);
			printf("TCP FIN bit value: %d\n", TCP_segment->fin);
			printf("TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);

			//Print values to file client.out
			fprintf(fp, "\nServer has sent SYN ACK signal to client succesfully\n");
			fprintf(fp, "TCP source port: %d\n", TCP_segment->srcport);
			fprintf(fp, "TCP destination port: %d\n", TCP_segment->destport);
			fprintf(fp, "TCP sequence number: %d\n", TCP_segment->seqnum);
			fprintf(fp, "TCP ack number: %d\n", TCP_segment->acknum);
			fprintf(fp, "TCP offset/ header length: %d\n", TCP_segment->offset);
			fprintf(fp, "TCP URG bit value: %d\n", TCP_segment->urg);
			fprintf(fp, "TCP ACK bit value: %d\n", TCP_segment->ack);
			fprintf(fp, "TCP PSH bit value: %d\n", TCP_segment->psh);
			fprintf(fp, "TCP RST bit value: %d\n", TCP_segment->rst);
			fprintf(fp, "TCP SYN bit value: %d\n", TCP_segment->syn);
			fprintf(fp, "TCP FIN bit value: %d\n", TCP_segment->fin);
			fprintf(fp, "TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);
		}
	}

	fclose(fp);
	free(TCP_segment);
}

void client_close_TCP_connection(int socket_fd)
{
	FILE *fp;
	fp = fopen("client.out", "a"); //append mode

	//Create a close request TCP segment
	struct tcpheader *TCP_segment;
	TCP_segment = malloc(sizeof(struct tcpheader));

	//Get client port number
	struct sockaddr_in local_addr;
	int addr_size = sizeof(local_addr);
	getsockname(socket_fd, (struct sockaddr *) &local_addr, &addr_size);

	//printf("client port: %d\n", ntohs(local_addr.sin_port));
	
	TCP_segment->srcport = ntohs(local_addr.sin_port);
	TCP_segment->destport = PORT;

	TCP_segment->seqnum = 1024; //Assign a 1024 as client sequence number
	unsigned int temp_seqnum = TCP_segment->seqnum;
	TCP_segment->acknum = 512; //Assign acknowledgement number as 512
	int header_length = (16+16+32+32+4+6+6+16+16+16+32)/32;
	TCP_segment->offset = header_length; //24 bytes = 192 bits, 192/32=6
	TCP_segment->reserved = 0;
	TCP_segment->urg = 0;
	TCP_segment->ack = 0;
	TCP_segment->psh = 0;
	TCP_segment->rst = 0;
	TCP_segment->syn = 0;
	TCP_segment->fin = 1; //Set the FIN bit to 1
	TCP_segment->window = 0;
	TCP_segment->checksum = 0;
	TCP_segment->pointer = 0;
	TCP_segment->option = 0;

	//Calculate checksum
	unsigned short int checksum_arr[12];
	unsigned int sum=0, checksum, wrap;

	memcpy(checksum_arr, TCP_segment, 24); //Copying 24 bytes

	for (int i=0;i<12;i++)
	{
		//printf("0x%04X\n", checkksum_arr[i]);
		sum = sum + checksum_arr[i];
	}

	wrap = sum >> 16;// Wrap around once  
	sum = sum & 0x0000FFFF;   
	sum = wrap + sum;  
	wrap = sum >> 16;// Wrap around once more  
	sum = sum & 0x0000FFFF;  
	checksum = wrap + sum;  
	//printf("\nSum Value: 0x%04X\n", checksum);  /* XOR the sum for checksum */  
	//printf("\nChecksum Value: 0x%04X\n", (0xFFFF^checksum));

	TCP_segment->checksum = checksum;
	int length = send(socket_fd, TCP_segment, 24, 0);//send message to server
	if (length <= 0)
	{
		printf("Client failed to send FIN signal to close TCP connection\n");
	}
	else
	{
		printf("\nClient has succesfully sent FIN signal to server to close TCP connection\n");
		printf("TCP source port: %d\n", TCP_segment->srcport);
		printf("TCP destination port: %d\n", TCP_segment->destport);
		printf("TCP sequence number: %d\n", TCP_segment->seqnum);
		printf("TCP ack number: %d\n", TCP_segment->acknum);
		printf("TCP offset/ header length: %d\n", TCP_segment->offset);
		printf("TCP URG bit value: %d\n", TCP_segment->urg);
		printf("TCP ACK bit value: %d\n", TCP_segment->ack);
		printf("TCP PSH bit value: %d\n", TCP_segment->psh);
		printf("TCP RST bit value: %d\n", TCP_segment->rst);
		printf("TCP SYN bit value: %d\n", TCP_segment->syn);
		printf("TCP FIN bit value: %d\n", TCP_segment->fin);
		printf("TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);

		//Print values to file client.out
		fprintf(fp, "\nServer has sent SYN ACK signal to client succesfully\n");
		fprintf(fp, "TCP source port: %d\n", TCP_segment->srcport);
		fprintf(fp, "TCP destination port: %d\n", TCP_segment->destport);
		fprintf(fp, "TCP sequence number: %d\n", TCP_segment->seqnum);
		fprintf(fp, "TCP ack number: %d\n", TCP_segment->acknum);
		fprintf(fp, "TCP offset/ header length: %d\n", TCP_segment->offset);
		fprintf(fp, "TCP URG bit value: %d\n", TCP_segment->urg);
		fprintf(fp, "TCP ACK bit value: %d\n", TCP_segment->ack);
		fprintf(fp, "TCP PSH bit value: %d\n", TCP_segment->psh);
		fprintf(fp, "TCP RST bit value: %d\n", TCP_segment->rst);
		fprintf(fp, "TCP SYN bit value: %d\n", TCP_segment->syn);
		fprintf(fp, "TCP FIN bit value: %d\n", TCP_segment->fin);
		fprintf(fp, "TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);
	}

	//Receive ACK message from server 
	memset(TCP_segment, 0, sizeof(struct tcpheader));	//reallocate memory
	int rec_bytes = recv(socket_fd, TCP_segment, sizeof(struct tcpheader), 0);
	
	if (rec_bytes > 0)
	{
		//Print values to console
		printf("\nClient received ACK signal from server\n");
		printf("TCP source port: %d\n", TCP_segment->srcport);
		printf("TCP destination port: %d\n", TCP_segment->destport);
		printf("TCP sequence number: %d\n", TCP_segment->seqnum);
		printf("TCP ack number: %d\n", TCP_segment->acknum);
		printf("TCP offset/ header length: %d\n", TCP_segment->offset);
		printf("TCP URG bit value: %d\n", TCP_segment->urg);
		printf("TCP ACK bit value: %d\n", TCP_segment->ack);
		printf("TCP PSH bit value: %d\n", TCP_segment->psh);
		printf("TCP RST bit value: %d\n", TCP_segment->rst);
		printf("TCP SYN bit value: %d\n", TCP_segment->syn);
		printf("TCP FIN bit value: %d\n", TCP_segment->fin);
		printf("TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);

		//Print values to file client.out
		fprintf(fp, "\nClient received ACK signal from server\n");
		fprintf(fp, "TCP source port: %d\n", TCP_segment->srcport);
		fprintf(fp, "TCP destination port: %d\n", TCP_segment->destport);
		fprintf(fp, "TCP sequence number: %d\n", TCP_segment->seqnum);
		fprintf(fp, "TCP ack number: %d\n", TCP_segment->acknum);
		fprintf(fp, "TCP offset/ header length: %d\n", TCP_segment->offset);
		fprintf(fp, "TCP URG bit value: %d\n", TCP_segment->urg);
		fprintf(fp, "TCP ACK bit value: %d\n", TCP_segment->ack);
		fprintf(fp, "TCP PSH bit value: %d\n", TCP_segment->psh);
		fprintf(fp, "TCP RST bit value: %d\n", TCP_segment->rst);
		fprintf(fp, "TCP SYN bit value: %d\n", TCP_segment->syn);
		fprintf(fp, "TCP FIN bit value: %d\n", TCP_segment->fin);
		fprintf(fp, "TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);
	}

	//Receive FIN message from server. The client responds back with an acknowledgement TCP segment.
	memset(TCP_segment, 0, sizeof(struct tcpheader));	//reallocate memory
	rec_bytes = recv(socket_fd, TCP_segment, sizeof(struct tcpheader), 0);
	if (rec_bytes > 0)
	{
		printf("\nClient received FIN signal from server\n");
		//Print values
		printf("TCP source port: %d\n", TCP_segment->srcport);
		printf("TCP destination port: %d\n", TCP_segment->destport);
		printf("TCP sequence number: %d\n", TCP_segment->seqnum);
		printf("TCP ack number: %d\n", TCP_segment->acknum);
		printf("TCP offset/ header length: %d\n", TCP_segment->offset);
		printf("TCP URG bit value: %d\n", TCP_segment->urg);
		printf("TCP ACK bit value: %d\n", TCP_segment->ack);
		printf("TCP PSH bit value: %d\n", TCP_segment->psh);
		printf("TCP RST bit value: %d\n", TCP_segment->rst);
		printf("TCP SYN bit value: %d\n", TCP_segment->syn);
		printf("TCP FIN bit value: %d\n", TCP_segment->fin);
		printf("TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);

		fprintf(fp, "\nClient received FIN signal from server\n");
		//Print values to file client.out
		fprintf(fp, "TCP source port: %d\n", TCP_segment->srcport);
		fprintf(fp, "TCP destination port: %d\n", TCP_segment->destport);
		fprintf(fp, "TCP sequence number: %d\n", TCP_segment->seqnum);
		fprintf(fp, "TCP ack number: %d\n", TCP_segment->acknum);
		fprintf(fp, "TCP offset/ header length: %d\n", TCP_segment->offset);
		fprintf(fp, "TCP URG bit value: %d\n", TCP_segment->urg);
		fprintf(fp, "TCP ACK bit value: %d\n", TCP_segment->ack);
		fprintf(fp, "TCP PSH bit value: %d\n", TCP_segment->psh);
		fprintf(fp, "TCP RST bit value: %d\n", TCP_segment->rst);
		fprintf(fp, "TCP SYN bit value: %d\n", TCP_segment->syn);
		fprintf(fp, "TCP FIN bit value: %d\n", TCP_segment->fin);
		fprintf(fp, "TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);

		TCP_segment->srcport = ntohs(local_addr.sin_port);
		TCP_segment->destport = PORT;
		TCP_segment->acknum = TCP_segment->seqnum + 1; //Acknowledgement number equal to server sequence number + 1
		TCP_segment->seqnum = temp_seqnum + 1; //Assign a sequence number as client sequence number + 1
		TCP_segment->offset = header_length; //24 bytes = 192 bits, 192/32=6
		TCP_segment->reserved = 0;
		TCP_segment->urg = 0;
		TCP_segment->ack = 1; //Set the ACK bit to 1
		TCP_segment->psh = 0;
		TCP_segment->rst = 0;
		TCP_segment->syn = 0;
		TCP_segment->fin = 0;
		TCP_segment->window = 0;
		TCP_segment->checksum = 0;
		TCP_segment->pointer = 0;
		TCP_segment->option = 0;

		//Calculate checksum
		unsigned short int checksum_arr1[12];
		sum=0; checksum = 0; wrap = 0;

		memcpy(checksum_arr1, TCP_segment, 24); //Copying 24 bytes

		for (int i=0;i<12;i++)
		{
			//printf("0x%04X\n", checkksum_arr[i]);
			sum = sum + checksum_arr1[i];
		}

		wrap = sum >> 16;// Wrap around once  
		sum = sum & 0x0000FFFF;   
		sum = wrap + sum;  
		wrap = sum >> 16;// Wrap around once more  
		sum = sum & 0x0000FFFF;  
		checksum = wrap + sum;  
		//printf("\nSum Value: 0x%04X\n", checksum);  /* XOR the sum for checksum */  
		//printf("\nChecksum Value: 0x%04X\n", (0xFFFF^checksum));

		TCP_segment->checksum = checksum;
		length = send(socket_fd, TCP_segment, 24, 0);//send message to server
		if (length <= 0)
		{
			printf("Client failed to send ACK signal to close TCP connection\n");
		}
		else
		{
			//Print values
			printf("\nClient has succesfully sent ACK signal to server to close TCP connection\n");
			printf("TCP source port: %d\n", TCP_segment->srcport);
			printf("TCP destination port: %d\n", TCP_segment->destport);
			printf("TCP sequence number: %d\n", TCP_segment->seqnum);
			printf("TCP ack number: %d\n", TCP_segment->acknum);
			printf("TCP offset/ header length: %d\n", TCP_segment->offset);
			printf("TCP URG bit value: %d\n", TCP_segment->urg);
			printf("TCP ACK bit value: %d\n", TCP_segment->ack);
			printf("TCP PSH bit value: %d\n", TCP_segment->psh);
			printf("TCP RST bit value: %d\n", TCP_segment->rst);
			printf("TCP SYN bit value: %d\n", TCP_segment->syn);
			printf("TCP FIN bit value: %d\n", TCP_segment->fin);
			printf("TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);

			//Print values to file client.out
			fprintf(fp, "\nServer has sent SYN ACK signal to client succesfully\n");
			fprintf(fp, "TCP source port: %d\n", TCP_segment->srcport);
			fprintf(fp, "TCP destination port: %d\n", TCP_segment->destport);
			fprintf(fp, "TCP sequence number: %d\n", TCP_segment->seqnum);
			fprintf(fp, "TCP ack number: %d\n", TCP_segment->acknum);
			fprintf(fp, "TCP offset/ header length: %d\n", TCP_segment->offset);
			fprintf(fp, "TCP URG bit value: %d\n", TCP_segment->urg);
			fprintf(fp, "TCP ACK bit value: %d\n", TCP_segment->ack);
			fprintf(fp, "TCP PSH bit value: %d\n", TCP_segment->psh);
			fprintf(fp, "TCP RST bit value: %d\n", TCP_segment->rst);
			fprintf(fp, "TCP SYN bit value: %d\n", TCP_segment->syn);
			fprintf(fp, "TCP FIN bit value: %d\n", TCP_segment->fin);
			fprintf(fp, "TCP check sum in decimal: %d and hexadecimal: %x\n", TCP_segment->checksum, TCP_segment->checksum);
		}
	}
	fclose(fp);
	free(TCP_segment);

}

int main()
{
	int length;
	int socket_fd;
	struct sockaddr_in server_addr;
	socket_fd = socket(AF_INET, SOCK_STREAM,0);
	server_addr.sin_port = htons(PORT);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("129.120.151.94");

	int sk_option = 1;
	//display if failed to send
	if(setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY, (char *) &sk_option, sizeof(sk_option)) < 0 ) 
	{ 
		perror("ERROR: setsockopt failed"); 
		return EXIT_FAILURE; 
	} 
  
	//connect the server
	if ((connect (socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))) == -1)
	{
		printf("Connection to socket failed \n");
		exit(EXIT_FAILURE);
	}

	client_three_way_handshake(socket_fd);
	printf("\n---------------------------------------------------\n");
	client_close_TCP_connection(socket_fd);
	printf("\n---------------------------------------------------\n");

	return 0;
}