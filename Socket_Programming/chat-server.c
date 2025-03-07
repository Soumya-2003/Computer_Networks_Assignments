#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

int main(){
    int sd, cd, cad1;
    struct sockaddr_in sad, cad;
    char str[50];

    sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sad.sin_family = AF_INET;
    sad.sin_port = htons(8000);
    sad.sin_addr.s_addr = inet_addr("192.168.0.111");

    bind(sd, (struct sockaddr *)&sad, sizeof(sad));
    listen(sd, 1);

    cad1 = sizeof(cad);
    cd = accept(sd, (struct sockaddr *)&cad, &cad1);
    do
    {
        printf("Enter the string: ");
        fflush(stdin);
        scanf("%s", str);
        send(cd, str, sizeof(str), 0);
        recv(cd, str, sizeof(str), 0);
        printf("\nReceived data is: %s\n", str);
    } while (strcmp(str,"bye") != 0);
    
    close(cd);
    close(sd);
}