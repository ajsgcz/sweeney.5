#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include "header.h"

#define MAX_INTERVAL 1000 // Define the maximum interval in milliseconds
#define REQUEST_TYPE 1 // Message type for resource requests
#define RELEASE_TYPE 2 // Message type for resource releases


int generateRandomInterval() {
    return rand() % (MAX_INTERVAL + 1); // Add 1 to include MAX_INTERVAL
}
// Function to request resources
void requestResources() {
    // Implement logic to request resources
     printf("Requesting resources...\n");
    // Request resources here...
}

void sendRequestMessage(int msqid, int resourceType) {
    Message msg;
    msg.type = REQUEST_TYPE;
    msg.resourceType = resourceType;
    // Set other message parameters if needed

    // Send the message
    if (msgsnd(msqid, &msg, sizeof(Message), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
}

// Function to release resources
void releaseResources() {
    // Implement logic to release resources
     printf("Releasing resources...\n");
    // Release resources here...
}

int main(int argc, char *argv[]) {
  int msqid = msgget(MESSAGE_QUEUE_KEY, 0666);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    int interval = 0;
    char *logfile = NULL;

    // Parse command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "i:f:")) != -1) {
        switch (opt) {
            case 'i':
                interval = atoi(optarg); // Convert argument to integer
                break;
            case 'f':
                logfile = optarg; // Set logfile path
                break;
            default:
                fprintf(stderr, "Usage: %s [-i interval] [-f logfile]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Validate command line arguments
    if (interval <= 0 || logfile == NULL) {
        fprintf(stderr, "Interval must be a positive integer and logfile must be specified\n");
        exit(EXIT_FAILURE);
    }

    // Output parsed command line arguments
    printf("Interval: %d\n", interval);
    printf("Logfile: %s\n", logfile);

   // Seed the random number generator with the current time
    srand(time(NULL));

    // Generate a random number between 0 and RAND_MAX
    int random_number = rand();

    // Print the random number
    printf("Random number: %d\n", random_number);
    // Main loop
    while (1) {
       // Generate a random time interval between 0 and MAX_INTERVAL milliseconds
    int random_interval = rand() % (MAX_INTERVAL + 1); // Add 1 to include MAX_INTERVAL

    // Print the random time interval
    printf("Random time interval: %d milliseconds\n", random_interval);
        // Request or release resources based on random time
         int interval = generateRandomInterval();

        // Sleep for the random time interval
        usleep(interval * 1000); // Convert milliseconds to microseconds

        // Generate a random number to decide whether to request or release resources
        int action = rand() % 2; // 0 for request, 1 for release

        // Perform the corresponding action
        if (action == 0) {
            requestResources(); // Request resources
        } else {
            releaseResources(); // Release resources
        }
    }
        // Sleep for random time interval
        int sleep_time = rand() % BOUND;
        usleep(sleep_time);

	sendRequestMessage(msqid, resourceType);
    }
    
    return 0;
}
