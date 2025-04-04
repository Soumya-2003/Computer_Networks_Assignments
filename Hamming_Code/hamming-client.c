#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<math.h>

void generateHammingCode(char *cdata){
    int size = strlen(cdata);
    int data[size];
    for(int i = 0; i<size; i++){
        data[i] = cdata[i] - 48;
    }

    int paritybit = 0;
    while((1<<paritybit) < size + paritybit + 1)
        paritybit++;
    int hammingLength = size + paritybit;
    int hamming[hammingLength];
    int revdata[size];
    for (int i = 0; i < size; i++){
        revdata[i] = data[size-1-i];
    }
    int j = 0;
    for (int i = 0; i < hammingLength; i++){
        if((i+1) & i)   hamming[i] = revdata[j++];
        else    hamming[i] = 0;
    }

    for(int i = 0; i < paritybit; i++){
        int position = 1<<i;
        int parity = 0;
        for(int j = position-1; j < hammingLength; j+=2*position){
            for(int k = 0; k < position && (j+k)<hammingLength; k++){
                parity ^= hamming[j+k];
            }       
        }
        hamming[position-1] = parity;
    }
    int revcode[hammingLength];
    for(int i = 0; i < hammingLength; i++){
        revcode[i] = hamming[hammingLength-1-i];
    }

    //Convert revcode to string
    // char hammingCode[20];
    for(int i = 0; i < hammingLength; i++){
        cdata[i] = revcode[i] + 48;
    }

    printf("Redundant bits: %d\n", paritybit);
    printf("Hamming Code: %s\n", cdata);
        
}

int main()
{
    int socketDescriptor;
	struct sockaddr_in serverAddress;
    char cdata[20];
	
	socketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8000);
	serverAddress.sin_addr.s_addr = inet_addr("172.17.0.1");
	
	connect(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	
	printf("Enter the data: ");
    scanf("%s", cdata);

    generateHammingCode(cdata);

	printf("\nTransmitted Code: %s", cdata);
	send(socketDescriptor, cdata, sizeof(cdata), 0);
	
	close(socketDescriptor);    

    return 0;
}
