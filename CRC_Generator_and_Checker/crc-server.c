#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

void xor(char *dividend, char *divisor, int divisorLength){
	for(int i = 0; i < divisorLength; i++){
		dividend[i] = (dividend[i] == divisor[i] ? '0':'1');
	}
}

void crcGenerator(char *data, char *divisor, char *remainder){
	int dataLength = strlen(data);
	int divisorLength = strlen(divisor);
	
	char temp[dataLength + 1];
	strcpy(temp, data);
	
	for(int i = 0; i < dataLength; i++){
		if(temp[i] == '1'){
			xor(&temp[i], divisor, divisorLength);
		}
	}
	strncpy(remainder, &temp[dataLength], divisorLength-1);
	remainder[divisorLength-1] = '\0';
}

int main(){
	int serverSocket, clientSocket, clientAddressLength;
	struct sockaddr_in serverAddress, clientAddress;
	char data[20], divisor[20], remainder[20], transmittedCode[20];
	
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8000);
	serverAddress.sin_addr.s_addr = inet_addr("172.17.0.1");
	
	bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	listen(serverSocket, 1);
	
	clientAddressLength = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    
    recv(clientSocket, transmittedCode, sizeof(transmittedCode), 0);
    printf("Client says: %s\n", transmittedCode);
    
    
    printf("Input Divisor: ");
    gets(divisor);
	
	crcGenerator(transmittedCode, divisor, remainder);
	
	printf("\nChecksum: %s\n", remainder);
	
	int flag = 1;
	for(int i = 0; remainder[i] != '\0'; i++){
		if(remainder[i] != '0'){
			printf("Rejecting");
			flag = 0;
			break;
		}
	}
	if(flag){
		printf("Accepting...");
	}
	
	
	close(clientSocket);
    close(serverSocket);

    return 0;
}
