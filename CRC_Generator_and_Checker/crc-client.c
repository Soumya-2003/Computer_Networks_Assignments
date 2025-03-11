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
	char temp[dataLength + divisorLength];
	strcpy(temp, data);
	for(int i = 0; i < divisorLength-1; i++){
		temp[dataLength+i] = '0';
	}
	for(int i = 0; i < dataLength; i++){
		if(temp[i] == '1'){
			xor(&temp[i], divisor, divisorLength);
		}
	}
	strncpy(remainder, &temp[dataLength], divisorLength-1);
}

int main(){
	int socketDescriptor;
	struct sockaddr_in serverAddress;
	char data[20], divisor[20], remainder[20], transmittedCode[20];
	
	socketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8000);
	serverAddress.sin_addr.s_addr = inet_addr("172.17.0.1");
	
	connect(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	
	printf("Input data: ");
	gets(data);
	
	printf("Input divisor: ");
	gets(divisor);
	
	crcGenerator(data, divisor, remainder);
	strcpy(transmittedCode, data);
	strcat(transmittedCode, remainder);
	
	printf("\nChecksum: %s", remainder);
	printf("\nTransmitted Code: %s", transmittedCode);
	send(socketDescriptor, transmittedCode, sizeof(transmittedCode), 0);
	
	close(socketDescriptor);
}
