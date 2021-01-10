#include <stdio.h>

struct tcpheader
{
	unsigned short int srcport; //16-bit source port
	unsigned short int destport; //16-bit destination port
	unsigned int seqnum; //32-bit sequence number
	unsigned int acknum; //32-bit acknowledgement number
	unsigned short int offset:4, //4-bit data offset or header length
		reserved:6, //6-bit reserved section
		//6-bit flags
		urg:1,
		ack:1,
		psh:1,
		rst:1,
		syn:1,
		fin:1;
	unsigned short int window;//16-bit receive window for flow control
	unsigned short int checksum;//6-bit checksum
	unsigned short int pointer;//16-bit urgent data pointer
	unsigned int option;//32-bit Options
};