// udp_listener.c
#include "udp_listener.h"

#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "screenGameplay.h"

#define RX_BUFFER_LEN 1024
#define TX_BUFFER_LEN 1024
#define PORT 12345

static int* runFlag;
static pthread_t udpThread;


// Prepare response accordingly
static void reply(char* messageRx, char* messageTx)
{
	// CHEAT: change max score
	if (strncmp(messageRx, "maxScore", sizeof("maxScore") - 1) == 0) {
		// Split the string by ' '
		char* text = strtok(messageRx, " ");

		// Get the value
		text = strtok(NULL, " ");
		// Convert to integer
		int newScore = atoi(text);

		// Validate
		gameScreen_setMaxScore(newScore);
	}
	// Stop the program
	else if (strncmp(messageRx, "stop", sizeof("stop") - 1) == 0) {
		*runFlag = 0;
	}
}

// UDP listener
void* startListening(void* args)
{
	// Set up UDP (mostly similar to Dr. Brian Fraser's code since there's only 1 way to do this)
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);
	
	// Create and bind to socket
	int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
	bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
	
	while (*runFlag) {
		// Get the data
		struct sockaddr_in sinRemote;
		unsigned int sin_len = sizeof(sinRemote);
		char messageRx[RX_BUFFER_LEN];	
		
		int bytesRx = recvfrom(socketDescriptor,
			messageRx, RX_BUFFER_LEN, 0,
			(struct sockaddr *) &sinRemote, &sin_len);
		
		int terminateIdx = (bytesRx < RX_BUFFER_LEN) ? bytesRx : RX_BUFFER_LEN - 1;
		messageRx[terminateIdx] = 0;
		
		// Extract the meaning
		char messageTx[TX_BUFFER_LEN] = "";
		reply(messageRx, messageTx);
		
		// Reply
		sin_len = sizeof(sinRemote);
		sendto (socketDescriptor,
			messageTx, strlen(messageTx),
			0,
			(struct sockaddr *) &sinRemote, sin_len);
	}
	
	// Close
	close(socketDescriptor);

	return NULL;
}


// Start listening using UDP
void UDP_init(int* flag)
{
	runFlag = flag;
	pthread_create(&udpThread, NULL, &startListening, NULL);
}