#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define NUM_IT_SERVICES 3
#define RED_LEVEL 10
#define YELLOW_LEVEL 5

// Function declarations
void initITService(int serviceNumber);
void ITService(int serviceNumber);
int calc_threat(int threat);
void handleSNMPRequest(int serviceNumber);
void handleReconfiguration(int serviceNumber);
void handleShutdown(int serviceNumber);

// Global variables
int reconfigured[NUM_IT_SERVICES] = {0};
int threatLevels[NUM_IT_SERVICES] = {1};
time_t lastSNMPRequestTime[NUM_IT_SERVICES] = {0};
pid_t ITServicePIDs[NUM_IT_SERVICES];

// Signal handler for SIGCHLD
void sigchld_handler(int signo) {
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // Handle the termination of IT services
        for (int i = 0; i < NUM_IT_SERVICES; i++) {
            if (pid == ITServicePIDs[i]) {
                if (WIFEXITED(status)) {
                    if (WEXITSTATUS(status) == 0) {
                        // IT service terminated successfully
                        printf("Job well done IT specialist %d. Prepare for new attacks!\n", i + 1);
                    } else if (WEXITSTATUS(status) == 1) {
                        // IT service compromised
                        printf("IT service %d compromised, we are going out of business!\n", i + 1);
                    } else {
                        // IT service crashed
                        printf("Call HR, we need a new cybersecurity expert for service %d.\n", i + 1);
                    }
                }
            }
        }
    }
}

// Signal handler for SIGALRM
void sigalrm_handler(int signo) {
    // Handle alarm signal
    for (int i = 0; i < NUM_IT_SERVICES; i++) {
        if (!reconfigured[i]) {
            threatLevels[i] = calc_threat(threatLevels[i]);
            // Check if SNMP request can be sent
            if (time(NULL) - lastSNMPRequestTime[i] >= 5) {
                // Print threat level color
                if (threatLevels[i] >= RED_LEVEL) {
                    printf("IT service %d threat level: Red\n", i + 1);
                } else if (threatLevels[i] >= YELLOW_LEVEL) {
                    printf("IT service %d threat level: Yellow\n", i + 1);
                } else {
                    printf("IT service %d threat level: Green\n", i + 1);
                }
            } else {
                // Print message about load being too high
                printf("IT service %d load too high. Threat is increased.\n", i + 1);
            }
        }
    }
    // Set a new alarm for one second
    alarm(1);
}

// Signal handler for other signals (e.g., SIGUSR1, SIGUSR2)
void custom_signal_handler(int signo) {
    // Handle custom signals
    // Determine which IT service the signal is for
    int serviceNumber = signo - SIGUSR1;

    // Ensure the service number is within bounds
    if (serviceNumber >= 0 && serviceNumber < NUM_IT_SERVICES) {
        // Process the signal based on the service type
        if (signo == SIGUSR1) {
            // SNMP request signal
            handleSNMPRequest(serviceNumber);
        } else if (signo == SIGUSR2) {
            // Reconfiguration signal
            handleReconfiguration(serviceNumber);
        }
    }
}

// Function to initialize an IT service
void initITService(int serviceNumber) {
    // Fork the process for the IT service
    pid_t pid = fork();

    if (pid == 0) {
        // Child process (IT service)
        ITService(serviceNumber);
    } else if (pid > 0) {
        // Parent process
        ITServicePIDs[serviceNumber] = pid;
    } else {
        // Forking error
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
}

// Main function
int main(int argc, char *argv[]) {
    // Check command-line arguments
    if (argc != NUM_IT_SERVICES + 2) {
        fprintf(stderr, "Usage: %s COC_terminal IT_service1 IT_service2 IT_service3\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialize IT services
    for (int i = 0; i < NUM_IT_SERVICES; i++) {
        initITService(i);
    }

    // Set signal handlers for SIGCHLD, SIGALRM, and custom signals
    signal(SIGCHLD, sigchld_handler);
    signal(SIGALRM, sigalrm_handler);
    signal(SIGUSR1, custom_signal_handler);
    signal(SIGUSR2, custom_signal_handler);

    // Set an initial alarm for one second
    alarm(1);

    // Main loop to handle user input
    while (1) {
        char command[10];
        int serviceNumber;

        // Read user input from COC terminal
        printf("Enter command (sn, rn, kn): ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            // Handle EOF or error
            perror("Error reading input");
            break;
        }

        // Process commands
        if (sscanf(command, "sn %d", &serviceNumber) == 1) {
            // SNMP request command
            if (serviceNumber >= 1 && serviceNumber <= NUM_IT_SERVICES) {
                // Send SNMP request signal to the corresponding IT service
                kill(ITServicePIDs[serviceNumber - 1], SIGUSR1);
                lastSNMPRequestTime[serviceNumber - 1] = time(NULL);
            } else {
                printf("Invalid IT service number\n");
            }
        } else if (sscanf(command, "rn %d", &serviceNumber) == 1) {
            // Reconfiguration command
            if (serviceNumber >= 1 && serviceNumber <= NUM_IT_SERVICES) {
                // Send reconfiguration signal to the corresponding IT service
                kill(ITServicePIDs[serviceNumber - 1], SIGUSR2);
            } else {
                printf("Invalid IT service number\n");
            }
        } else if (sscanf(command, "kn %d", &serviceNumber) == 1) {
            // Shutdown command
            if (serviceNumber >= 1 && serviceNumber <= NUM_IT_SERVICES) {
                // Instruct IT service to shutdown
                handleShutdown(serviceNumber - 1);
            } else {
                printf("Invalid IT service number\n");
            }
        } else {
            // Invalid command
            printf("Invalid command\n");
        }
    }

    // Clean up and exit
    return 0;
}

// Function to handle SNMP request
void handleSNMPRequest(int serviceNumber) {
    if (time(NULL) - lastSNMPRequestTime[serviceNumber] < 5) {
        printf("Load too high. Threat is increased.\n");
        threatLevels[serviceNumber]++;
    } else {
        if (!reconfigured[serviceNumber]) {
            // Print threat level color
            if (threatLevels[serviceNumber] >= RED_LEVEL) {
                printf("IT service %d threat level: Red\n", serviceNumber + 1);
            } else if (threatLevels[serviceNumber] >= YELLOW_LEVEL) {
                printf("IT service %d threat level: Yellow\n", serviceNumber + 1);
            } else {
                printf("IT service %d threat level: Green\n", serviceNumber + 1);
            }
            lastSNMPRequestTime[serviceNumber] = time(NULL);
        }
    }
}

// Function to handle reconfiguration
void handleReconfiguration(int serviceNumber) {
    if (!reconfigured[serviceNumber]) {
        if (threatLevels[serviceNumber] < RED_LEVEL) {
            printf("Threat level is not critical. You are fired!\n");
            exit(EXIT_FAILURE);
        } else {
            reconfigured[serviceNumber] = 1;
            printf("Reconfiguring system to thwart attack – this may take a few seconds.\n");
        }
    } else {
        printf("Cannot reconfigure more than once. You are fired!\n");
        exit(EXIT_FAILURE);
    }
}

// Function to handle IT service shutdown
void handleShutdown(int serviceNumber) {
    // Terminate the IT service
    printf("Terminated IT service %d\n", serviceNumber + 1);
    kill(ITServicePIDs[serviceNumber], SIGTERM);
}
