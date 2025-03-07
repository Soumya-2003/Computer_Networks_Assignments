#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(){
    int sd, cad1;
    struct sockaddr_in sad, cad;
    char str[50];

    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sad.sin_family = AF_INET;
    sad.sin_port = htons(8000);
    sad.sin_addr.s_addr = inet_addr("192.168.0.111");

    connect(sd, (struct sockaddr *)&sad, sizeof(sad));

    printf("Enter your Message: ");
    gets(str);
    send(sd, str, sizeof(str), 0);

    close(sd);
}