/*Name: Yen Pham
CS3530
Project 3 - Simulate TCP 3-way handshake and closing a TCP connection in the application layer 
using a client-server architecture.
*/

#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h> 
#include <time.h> 
#include <math.h>
#include "TCP_segment.h"
#define PORT 11485

void server_three_way_handshake(struct sockaddr_in client_addr, int socket_fd)
{
	//The server responds to the request by creating a connection granted TCP segment.
	struct tcpheader *TCP_segment = malloc(sizeof(struct tcpheader));
	int rec_bytes = recv(socket_fd, TCP_segment, sizeof(struct tcpheader), 0);
	//printf("Receive bytes: %d\n", rec_bytes);
	int header_length;
	FILE *fp;
	fp = fopen("server.out", "w"); //write mode

	if (rec_bytes > 0)
	{
		printf("\nServer received SYN signal from client and is creating TCP connection\n");
		//Print the values to console
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

		fprintf(fp, "\nServer received SYN signal from client and is creating TCP connection\n");
		//Print values to file server.out
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

		TCP_segment->srcport = PORT;
		TCP_segment->destport = ntohs(client_addr.sin_port);
		srand(time(NULL));
		TCP_segment->acknum = TCP_segment->seqnum + 1; //Assign acknowledgement number equal to initial client sequence number + 1
		TCP_segment->seqnum = rand()% (int) (pow(2,32)); //Assign a random initial server sequence number
		header_length = (16+16+32+32+4+6+6+16+16+16+32)/32;
		TCP_segment->offset = header_length; //24 bytes = 192 bits, 192/32=6
		TCP_segment->reserved = 0;
		TCP_segment->urg = 0;
		TCP_segment->ack = 1; //Set ACK bit to 1
		TCP_segment->psh = 0;
		TCP_segment->rst = 0;
		TCP_segment->syn = 1; //Set SYN bit to 1
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

		int length = send(socket_fd, TCP_segment, 24, 0);
		if (length < 0)
		{
			printf("Fail to send SYN ACK signal to client\n");
		}
		else
		{
			//Print the values to console
			printf("\nServer has sent SYN ACK signal to client succesfully\n");
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

			//Print values to file server.out
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

	memset(TCP_segment, 0, sizeof(struct tcpheader));	//reallocate memory
	//Once server receives ACK signal from client, TCP connection is created
	rec_bytes = recv(socket_fd, TCP_segment, sizeof(struct tcpheader), 0);
	if (rec_bytes > 0)
	{
		printf("\nServer received ACK signal from client. TCP connection is now created\n");
		//Print the values to console
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

		fprintf(fp, "\nServer received ACK signal from client. TCP connection is now created\n");
		//Print values to file server.out
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
	fclose(fp);
	free(TCP_segment);
}

void server_close_TCP_connection(struct sockaddr_in client_addr, int socket_fd)
{
	int length;
	int header_length;
	bool sent = false;
	unsigned int temp_acknum;
	//The server responds back with an acknowledgment TCP segment
	struct tcpheader *TCP_segment = malloc(sizeof(struct tcpheader)); //Create & allocate memory for an acknowledgement TCP segment
	int rec_bytes = recv(socket_fd, TCP_segment, sizeof(struct tcpheader), 0);
	FILE *fp;
	fp = fopen("server.out", "a"); // append mode

	if (rec_bytes > 0)
	{
		printf("\nServer received FIN signal from client to close TCP connection\n");
		//Print the values to console
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

		fprintf(fp, "\nServer received FIN signal from client to close TCP connection\n");
		//Print the values to console
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

		TCP_segment->srcport = PORT;
		TCP_segment->destport = ntohs(client_addr.sin_port);
		TCP_segment->acknum = TCP_segment->seqnum + 1; //acknowledgement number equal to client sequence number + 1
		temp_acknum = TCP_segment->acknum;
		TCP_segment->seqnum = 512; //Assign 512 as server sequence number
		header_length = (16+16+32+32+4+6+6+16+16+16+32)/32;
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

		length = send(socket_fd, TCP_segment, 24, 0);
		if (length < 0)
		{
			printf("Fail to send ACK signal to client\n");
		}
		else
		{
			sent = true;
			//Print the values
			printf("\nACK signal has been sent to client succesfully\n");
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

			//Print values to file server.out
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

	if (sent)
	{
		//The server again sends another close acknowledgement TCP segment.
		memset(TCP_segment, 0, sizeof(struct tcpheader)); //reallocate memory
		TCP_segment->srcport = PORT;
		TCP_segment->destport = ntohs(client_addr.sin_port);
		TCP_segment->acknum = temp_acknum; //acknowledgement number equal to client sequence number + 1
		TCP_segment->seqnum = 512; //Assign again 512 as server sequence number
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
		unsigned short int checksum_arr1[12];
		unsigned int sum=0, checksum, wrap;

		memcpy(checksum_arr1, TCP_segment, 24); //Copying 24 bytes

		for (int i=0;i<12;i++)
		{
			//printf("0x%04X\n", checkksum_arr1[i]);
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
		length = send(socket_fd, TCP_segment, 24, 0);
		if (length < 0)
		{
			printf("Fail to send FIN signal to client\n");
		}
		else
		{
			//Print the values
			printf("\nFIN signal has been sent to client succesfully\n");
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

			//Print values to file server.out
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
	memset(TCP_segment, 0, sizeof(struct tcpheader));	//reallocate memory
	rec_bytes = recv(socket_fd, TCP_segment, sizeof(struct tcpheader), 0);
	if (rec_bytes > 0)
	{
		printf("\nServer received ACK signal from client.\n");
		//Print the values to console
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
		printf("TCP connection is closed\nGood bye!\n");

		fprintf(fp, "\nServer received ACK signal from client.\n");
		//Print the values to file
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
		fprintf(fp, "TCP connection is closed\nGood bye!\n");
	}
	fclose(fp);
	free(TCP_segment);
}

int main()
{
	int listen_fd, conn_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd == -1)
	{
		perror("Cannot listen to socket\n");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	//to make sure it can use that some port later too
	int on = 1; 
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	if (bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)))
	{
		perror("Bind error\n");
		exit(EXIT_FAILURE);
	}

	if (listen(listen_fd, 10) == -1)
	{
		perror("Listen error\n");
		exit(EXIT_FAILURE);
	}

	int length = sizeof(client_addr);
	conn_fd = accept (listen_fd, (struct sockaddr*) &client_addr, &length);

	printf("Server port: %d\n", PORT);
	printf("Client port: %d\n\n", ntohs(client_addr.sin_port));

	server_three_way_handshake(client_addr, conn_fd);
	printf("\n---------------------------------------------------\n");
	server_close_TCP_connection(client_addr, conn_fd);
	printf("\n---------------------------------------------------\n");

	return 0;
}