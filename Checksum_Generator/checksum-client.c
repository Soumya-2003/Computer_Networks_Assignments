#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

void calculateChecksum(char *data, int segmentLength){
    int dataLength = strlen(data);

    int requiredZeros = segmentLength - (dataLength % segmentLength);
    if(requiredZeros == segmentLength) requiredZeros = 0;

    char dataword[requiredZeros + dataLength + segmentLength];

    if (requiredZeros && requiredZeros != segmentLength){
        char leadingZeros[requiredZeros];
        int i;
        for(i = 0; i < requiredZeros; i++){
            leadingZeros[i] = '0';
        }
        leadingZeros[i] = '\0';
        strcpy(dataword, leadingZeros);

        strcat(dataword, data);
        dataword[requiredZeros + dataLength] = '\0';
    }
    else{
        strcpy(dataword, data);
    }

    char data1[segmentLength], data2[segmentLength];

    //TODO: prepare data1
    for(int j = 0; j < segmentLength; j++){
        data1[j] = '0';
    }

    for(int i = 0; i < strlen(dataword); i += segmentLength){
        int carry = 0;
        for(int j = 0; j < segmentLength; j++){
            data2[j] = dataword[i+j];
        }
        for(int k = segmentLength-1; k >= 0; k--){
            data1[k] = (((data1[k] - 48) + (data2[k] - 48) + carry) % 2) + 48;
            carry = ((data1[k] - 48) + (data2[k] - 48) + carry) / 2;
        }
        if(carry){
            for(int j = 0; j < segmentLength-1; j++){
                data2[j] = '0';
            }
            data2[segmentLength-1] = '1';
            carry = 0;
            for(int k = segmentLength-1; k >= 0; k--){
                data1[k] = (((data1[k] - 48) + (data2[k] - 48) + carry) % 2) + 48;
                carry = ((data1[k] - 48) + (data2[k] - 48) + carry) / 2;
            }
        }
    }

    data1[segmentLength] = '\0';

    printf("Checksum: %s", data1);
    
    strcat(dataword, data1);
    strcpy(data, dataword);
    
}

int main(){
    int socketDescriptor;
	struct sockaddr_in serverAddress;
	char data[50], transCode[50];
    int segmentLength;
	
	socketDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8000);
	serverAddress.sin_addr.s_addr = inet_addr("172.17.0.1");
	
	connect(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	
	printf("Enter the data: ");
    scanf("%s", data);

    printf("Enter the segment length: ");
    scanf("%d", &segmentLength);
	
	calculateChecksum(data, segmentLength);
    strcpy(transCode, data);
	
	printf("\nTransmitted Code: %s", transCode);
	send(socketDescriptor, transCode, sizeof(transCode), 0);
	
	close(socketDescriptor);
}