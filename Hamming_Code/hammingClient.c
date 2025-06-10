#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void generateHammingCode(char *cdata) {
    int size = strlen(cdata);
    int data[size];

    for (int i = 0; i < size; i++) {
        data[i] = cdata[size - 1 - i] - '0';  // Reverse order and convert char to int
    }

    int parityBitCount = 0;
    while ((1 << parityBitCount) < size + parityBitCount + 1) {
        parityBitCount++;
    }

    int totalBits = size + parityBitCount;
    int hammingCode[totalBits];

    // Insert data and placeholder for parity bits
    for (int i = 0, j = 0; i < totalBits; i++) {
        if ((i + 1) & i)
            hammingCode[i] = data[j++];
        else
            hammingCode[i] = 0;
    }

    // Calculate parity bits
    for (int i = 0; i < parityBitCount; i++) {
        int position = 1 << i;
        int parity = 0;
        for (int j = position - 1; j < totalBits; j += 2 * position) {
            for (int k = 0; k < position && (j + k) < totalBits; k++) {
                parity ^= hammingCode[j + k];
            }
        }
        hammingCode[position - 1] = parity;
    }

    // Convert to string in reverse order
    for (int i = 0; i < totalBits; i++) {
        cdata[i] = hammingCode[totalBits - 1 - i] + '0';
    }
    cdata[totalBits] = '\0';

    printf("Redundant bits: %d\n", parityBitCount);
    printf("Hamming Code: %s\n", cdata);
}

int main() {
    int socketDescriptor;
    struct sockaddr_in serverAddress;
    char cdata[20];

    socketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8002);
    serverAddress.sin_addr.s_addr = inet_addr("192.168.1.4");

    connect(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    printf("Enter the data: ");
    scanf("%s", cdata);

    generateHammingCode(cdata);

    printf("\nTransmitted Code: %s", cdata);
    send(socketDescriptor, cdata, sizeof(cdata), 0);

    close(socketDescriptor);

    return 0;
}
