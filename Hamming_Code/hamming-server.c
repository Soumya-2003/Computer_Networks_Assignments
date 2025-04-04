#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<math.h>

void generateHammingCode(char hamstr[]){
    int hamsize = strlen(hamstr);
    int paritybit = 0;
    while((1<<paritybit) < hamsize + paritybit + 1)
        paritybit++;

    int hamming[hamsize];

    for (int i = 0; i < hamsize; i++){
        hamming[i] = hamstr[hamsize-1-i] - 48;
    }
    int error = 0;

    for(int i = 0; i < paritybit; i++){
        int position = 1<<i;
        int parity = 0;
        for(int j = position-1; j < hamsize; j+=2*position){
            for(int k = 0; k < position && (j+k)<hamsize; k++){
                parity ^= hamming[j+k];
            }       
        }
        if(parity != 0){
            error += position;
        }
    }
    if(error == 0){
        printf("Actual Data Received\n");
    }
    else{
        printf("Error at position: %d\n", error);
        hamming[error-1] ^= 1;
        printf("Corrected Data: ");
        for(int i = 0; i < hamsize; i++){
            printf("%d", hamming[hamsize-1-i]);
        } 
        printf("\n");
    }
        
}

int main()
{
    int serverSocket, clientSocket, clientAddressLength;
	struct sockaddr_in serverAddress, clientAddress;
	char cdata[20];
	
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8000);
	serverAddress.sin_addr.s_addr = inet_addr("172.17.0.1");
	
	bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	listen(serverSocket, 1);
	
	clientAddressLength = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    
    recv(clientSocket, cdata, sizeof(cdata), 0);
    printf("Client says: %s\n", cdata);
    
    printf("Enter a data to check: ");
    scanf("%s", cdata);

    generateHammingCode(cdata);
	
	close(clientSocket);
    close(serverSocket);

    return 0;
}
