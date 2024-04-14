#ifndef HEADER_H
#define HEADER_H

#include <sys/types.h>

// Define constants
#define MESSAGE_QUEUE_KEY 1234
#define NUM_RESOURCES 10

// Define message structure
typedef struct {
    long type; // Message type
    int resourceType; // Resource type
    // Add other message parameters if needed
} Message;

// Function prototypes
int generateRandomInterval();

#endif /* HEADER_H */
