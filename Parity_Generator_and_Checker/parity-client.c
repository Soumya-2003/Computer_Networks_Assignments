#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Declare socket descriptors and address structures
    int socketDescriptor;
    struct sockaddr_in serverAddress;
    char inputBits[50];       // Stores the user-input bit sequence
    char oddParityBits[50];   // Stores the bit sequence with odd parity

    // Create a TCP socket
    socketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Configure the server address
    serverAddress.sin_family = AF_INET;            // IPv4 protocol
    serverAddress.sin_port = htons(8000);          // Port number 8000 (converted to network byte order)
    serverAddress.sin_addr.s_addr = inet_addr("172.17.0.1"); // Server IP address

    // Connect to the server
    connect(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    // Get user input for the bit sequence
    printf("Enter the bit sequence: ");
    scanf("%s", inputBits);

    // Count the number of '1' bits in the input sequence
    int i = 0, bitCount = 0; 
    while (inputBits[i] != '\0') {
        if (inputBits[i] == '1') {
            bitCount++;
        }
        i++;
    }

    // Copy the original input to another variable for odd parity processing
    strcpy(oddParityBits, inputBits);
    printf("%s", oddParityBits);

    // Determine and append parity bit
    if (bitCount % 2 == 0) {
        // Even number of '1's, append '0' for even parity, '1' for odd parity
        strcat(oddParityBits, "1");   // Odd parity codeword
        strcat(inputBits, "0");       // Even parity codeword
        printf("The Even parity bit is: 0\n");
        printf("The Even parity code word is: %s\n", inputBits);
        printf("The Odd parity bit is: 1\n");
        printf("The Odd parity code word is: %s\n", oddParityBits);
    } else {
        // Odd number of '1's, append '1' for even parity, '0' for odd parity
        strcat(oddParityBits, "0");   // Odd parity codeword
        strcat(inputBits, "1");       // Even parity codeword
        printf("The Even parity bit is: 1\n");
        printf("The Even parity code word is: %s\n", inputBits);
        printf("The Odd parity bit is: 0\n");
        printf("The Odd parity code word is: %s\n", oddParityBits);
    }

    // Send the even parity codeword to the server
    send(socketDescriptor, inputBits, sizeof(inputBits), 0);

    // Close the socket
    close(socketDescriptor);

    return 0;
}