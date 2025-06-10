#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void generateHammingCode(char hamstr[]) {
    int len = strlen(hamstr);
    int parityBitCount = 0;

    // Determine number of parity bits
    while ((1 << parityBitCount) < len + 1) {
        parityBitCount++;
    }

    int hamming[len];
    for (int i = 0; i < len; i++) {
        hamming[i] = hamstr[len - 1 - i] - '0';  // Reverse and convert to int
    }

    int errorPosition = 0;

    // Calculate parity bits
    for (int i = 0; i < parityBitCount; i++) {
        int position = 1 << i;
        int parity = 0;

        for (int j = position - 1; j < len; j += 2 * position) {
            for (int k = 0; k < position && (j + k) < len; k++) {
                parity ^= hamming[j + k];
            }
        }

        if (parity != 0) {
            errorPosition += position;
        }
    }

    // Output results
    if (errorPosition == 0) {
        printf("Actual Data Received (No Error)\n");
    } else {
        printf("Error at position: %d\n", errorPosition);
        hamming[errorPosition - 1] ^= 1;

        printf("Corrected Data: ");
        for (int i = len - 1; i >= 0; i--) {
            printf("%d", hamming[i]);
        }
        printf("\n");
    }
}

int main() {
    int serverSocket, clientSocket, clientAddressLength;
    struct sockaddr_in serverAddress, clientAddress;
    char cdata[20];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8002);
    serverAddress.sin_addr.s_addr = inet_addr("192.168.1.4");

    // Bind and listen
    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 1);

    // Accept connection
    clientAddressLength = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

    // Receive data
    recv(clientSocket, cdata, sizeof(cdata), 0);
    printf("Client says: %s\n", cdata);

    // User enters the received code for checking
    printf("Enter a data to check: ");
    scanf("%s", cdata);

    generateHammingCode(cdata);

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
