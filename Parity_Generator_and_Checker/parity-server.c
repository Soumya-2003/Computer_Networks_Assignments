#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int serverSocket, clientSocket, clientAddressLength;
    struct sockaddr_in serverAddress, clientAddress;
    char receivedBits[50];
    char codeword[50];
    char extractedBits[50];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Configure server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8000);
    serverAddress.sin_addr.s_addr = inet_addr("172.17.0.1");

    // Bind socket to address and listen for connections
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 10);

    // Accept client connection
    clientAddressLength = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr*)&serverAddress, &clientAddressLength);

    // Receive data from client
    recv(clientSocket, receivedBits, sizeof(receivedBits), 0);
    printf("Client says: %s\n", receivedBits);

    // Get codeword from user
    printf("Enter the codeword: ");
    fgets(codeword, sizeof(codeword), stdin);

    int i = 0, bitCount = 0;
    char parityChoice;

    // Count the number of '1' bits
    while (codeword[i] != '\0') {
        if (codeword[i] == '1') {
            bitCount++;
        }
        i++;
    }

    // Get parity choice
    printf("Enter your Parity Choice (E for Even, O for Odd): ");
    scanf(" %c", &parityChoice);  // Added space before %c to consume newline

    int j = 0;

    // Validate parity choice and check correctness
    if ((parityChoice == 'E' && bitCount % 2 == 0) || (parityChoice == 'O' && bitCount % 2 != 0)) {
        printf("Accepting... ");
        while (j < strlen(codeword) - 1) {
            extractedBits[j] = codeword[j];
            j++;
        }
        extractedBits[j] = '\0'; // Ensure null termination
        printf("%s\n", extractedBits);
    } else {
        printf("Rejected\n");
    }

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
