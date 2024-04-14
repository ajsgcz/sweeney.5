#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>
#include "header.h"

int terminate = 0;
int num_processes = DEFAULT_NUM_PROCESSES;
int interval = DEFAULT_INTERVAL;
char* logfile = NULL;
int clock_shmid = -1;
int resources_shmid = -1;
int process_shmid = -1;

// Function to handle signals
void signalHandler(int signum) {
	 if (signum == SIGINT) {
        terminate = 1; // Set flag to indicate termination
    }
    // Implement signal handling logic
}

int createSharedMemory(size_t size) {
    int shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666); // Create shared memory segment
    if (shmid == -1) {
        perror("shmget"); // Print error message if creation fails
        exit(EXIT_FAILURE);
    }
    return shmid;
}

void initializeClock(Clock *clock_ptr) {
    // Initialize clock values
    clock_ptr->seconds = 0;
    clock_ptr->nanoseconds = 0;
}

void initializeResourceTable(ResourceDescriptor *resource_table) {
    // Initialize each resource descriptor in the table
    for (int i = 0; i < NUM_RESOURCES; i++) {
        resource_table[i].totalInstances = RESOURCE_INSTANCES;
        resource_table[i].availableInstances = RESOURCE_INSTANCES;
        // Initialize allocated and requested arrays if needed
        // memset(resource_table[i].allocated, 0, sizeof(resource_table[i].allocated));
        // memset(resource_table[i].requested, 0, sizeof(resource_table[i].requested));
    }
}

// Function to fork a child process
void forkChildProcess() {
    pid_t pid = fork(); // Fork a child process
    if (pid == -1) {
        perror("fork"); // Print error message if fork fails
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process code...
	    printf("Child process with PID %d\n", getpid());
        exit(EXIT_SUCCESS);
    } else {
        // Parent process code...
	    
    printf("Parent process with PID %d\n", getpid());
    
    
    }
}

void checkChildProcesses() {
    pid_t wpid;
    int status;

    // Non-blocking waitpid
    while ((wpid = waitpid(-1, &status, WNOHANG)) > 0) {
        // Child process has terminated
        // Free up its resources
        
        printf("Child process with PID %d has terminated.\n", wpid);
    }
    if (wpid == -1 && errno != ECHILD) {
        perror("waitpid"); // Print error message if waitpid fails
    }
}

int shouldLaunchChildProcess() {
    // Check if the maximum number of child processes has been reached
    if (childrenInSystem >= MAX_CHILD_PROCESSES) {
        return 0; // Do not launch a new process
    }
    
    // Add additional conditions to determine if a new process should be launched
    
    return 1; // Launch a new process
}

void launchChildProcess() {
    if (shouldLaunchChildProcess()) {
        // Fork a new child process
        forkChildProcess();
        childrenInSystem++;
        printf("Launched new child process. Total children in system: %d\n", childrenInSystem);
    } else {
        printf("Maximum number of child processes reached. Not launching new process.\n");
    }
}

void grantResourceRequests(ResourceDescriptor *resource_table, int num_processes) {
    // Loop through each process and check if it has an outstanding resource request
    for (int i = 0; i < num_processes; i++) {
	    if (process_has_outstanding_request(resource_table, i)) {
            grant_resource_request(resource_table, i);
        }

	    void checkMessagesFromChildProcesses(int msqid, ResourceDescriptor *resource_table) {
    // Declare message buffer
    Message msg;

    // Receive messages from child processes
    while (msgrcv(msqid, &msg, sizeof(Message), 0, IPC_NOWAIT) != -1) {
        // Process the received message
        switch (msg.type) {
            case REQUEST:
                // Handle resource request
                handleResourceRequest(resource_table, msg);
                break;
            case RELEASE:
                // Handle resource release
                handleResourceRelease(resource_table, msg);
                break;
            default:
                // Invalid message type
                fprintf(stderr, "Invalid message type received\n");
                break;
        }
    }
}

	    // Function to handle resource request from a child process
void handleResourceRequest(ResourceDescriptor *resource_table, Message msg) {
    // Implement logic to grant resource request if possible
   
    if (resource_available(resource_table, msg.resourceType)) {
        // Grant resource request
        grant_resource(resource_table, msg);
    } else {
        // Resource not available, notify child process to wait
         notify_child_to_wait(msg.processId);
    }
    
}

	    void handleResourceRelease(ResourceDescriptor *resource_table, Message msg) {
    // Implement logic to release resource

    release_resource(resource_table, msg);
    
}

	    void outputResourceTable(ResourceDescriptor *resource_table) {
    // Print resource table
    printf("Resource Table:\n");
    printf("Resource\tTotal\tAvailable\n");
    for (int i = 0; i < NUM_RESOURCES; i++) {
        printf("%d\t\t%d\t%d\n", i, resource_table[i].totalInstances, resource_table[i].availableInstances);
    }
}

// Function to output process table
void outputProcessTable(int num_processes) {
    // Print process table
    printf("\nProcess Table:\n");
    printf("Process ID\n");
    for (int i = 0; i < num_processes; i++) {
        printf("%d\n", i);
    }
}

	    void checkAndResolveDeadlock(ResourceDescriptor *resource_table, int num_processes) {
    // Implement deadlock detection algorithm
   
    if (detect_deadlock(resource_table, num_processes)) {
        // Resolve deadlock
        resolve_deadlock(resource_table, num_processes);
    }
    
}


void printUsage() {
    printf("Usage: oss [-h] [-n proc] [-s simul] [-i intervalInMsToLaunchChildren] [-f logfile]\n");
}

int main(int argc, char *argv[]) {
    // Install signal handler
    signal(SIGINT, signalHandler);
    // Create message queue
    int msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    // Parse command line arguments
	 int opt;
    while ((opt = getopt(argc, argv, "hn:s:i:f:")) != -1) {
        switch (opt) {
            case 'h':
                // Print usage information and exit
                printUsage();
                return 0;
            case 'n':
                // Parse number of processes
                num_processes = atoi(optarg);
                break;
            case 's':
                // Parse simulation time
                // (Not implemented in this example)
                break;
            case 'i':
                // Parse interval to launch children
                interval = atoi(optarg);
                break;
            case 'f':
                // Parse logfile
                logfile = optarg;
                break;
            case '?':
                // Handle invalid option
                printUsage();
                return 1;
        }
    }
    
    // Create shared memory for clock, resource table, process table, etc.
	clock_shmid = createSharedMemory(sizeof(Clock));
    
    // Attach shared memory for clock
    Clock *clock_ptr = (Clock *)shmat(clock_shmid, NULL, 0);
    if (clock_ptr == (void *)-1) {
        perror("shmat"); // Print error message if attachment fails
        exit(EXIT_FAILURE);
    }
    
    // Initialize resources and data structures
    clock_shmid = createSharedMemory(sizeof(Clock));
    Clock *clock_ptr = (Clock *)shmat(clock_shmid, NULL, 0);
    if (clock_ptr == (void *)-1) {
        perror("shmat"); // Print error message if attachment fails
        exit(EXIT_FAILURE);
    }
    initializeClock(clock_ptr);
    
    // Create shared memory for resource table
    resources_shmid = createSharedMemory(NUM_RESOURCES * sizeof(ResourceDescriptor));
    ResourceDescriptor *resource_table = (ResourceDescriptor *)shmat(resources_shmid, NULL, 0);
    if (resource_table == (void *)-1) {
        perror("shmat"); // Print error message if attachment fails
        exit(EXIT_FAILURE);
    }
    initializeResourceTable(resource_table);
    
    // Fork child processes
	for (int i = 0; i < num_processes; i++) {
        forkChildProcess();
    }
	 
    // Wait for all child processes to terminate
    int status;
    pid_t wpid;
    while ((wpid = wait(&status)) > 0);
    
    return 0;
}
    

    
    // Main loop
   
	while (!terminate && (stillChildrenToLaunch || childrenInSystem)) {
       // Non-blocking waitpid to check if child process has terminated
        checkChildProcesses();
		// Launch a child process if conditions are met
        launchChildProcess();
		// Grant outstanding resource requests
        grantResourceRequests(resource_table, num_processes);
		// Check for messages from child processes
        checkMessagesFromChildProcesses(msqid, resource_table);
		// Check for deadlock every second
        checkAndResolveDeadlock(resource_table, num_processes);
		        // Output resource table every half a second
        outputResourceTable(resource_table);
        
        // Output process table every half a second
        outputProcessTable(num_processes);
        
        // Sleep for half a second
        usleep(500000); // 500,000 microseconds = 0.5 seconds
    }
	    // Sleep for one second
        sleep(1);
    }
    
    // Cleanup shared memory and resources
    cleanup();
    
    return 0;
}
