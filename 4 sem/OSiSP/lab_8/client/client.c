#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

char current_dir[BUFFER_SIZE];

int create_connection(char* , long );
int send_requests_from_file(int , char* );

int handle_request(int socket, char *request) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t n;

    // Отправляем запрос серверу
    n = send(socket, request, strlen(request), 0);
    if (n == -1) {
        perror("send");
        exit(1);
    }

    // Получаем ответ от сервера
    n = recv(socket, buffer, BUFFER_SIZE, 0);
    if (n <= 0) {
        printf("Server closed connection.\n");
    }

    if (strcmp(buffer, "BYE") == 0) {
        // проверяем, является ли запрос командой "выход"
        printf("%s\n", buffer);
        return 1;
    } else if (strncmp(buffer, "CD", 2) == 0) {
        // проверяем получилось ли поменять каталог
        strcpy(current_dir, buffer + 3);
    } else {
        // Выводим ответ на экран
        printf("%s\n", buffer);
    }
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./clnt <server-ip> <server-port>\n");
        exit(1);
    }
    char* server_ip = argv[1];
    long server_port = strtol(argv[2], NULL, 10);

    int client_socket = create_connection(server_ip, server_port);


    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    // Получаем сразу info от сервера
    recv(client_socket, buffer, BUFFER_SIZE, 0);
    printf("%s\n", buffer);
    memset(buffer, 0, BUFFER_SIZE);

    strcpy(current_dir, "");
    while (1) {
        printf("%s> ", current_dir);
        if(!fgets(buffer, BUFFER_SIZE, stdin))
            break;
        buffer[strcspn(buffer, "\n")] = '\0'; // убираем символ перевода строки в конце строки

        // Если запрос начинается с символа "@", читаем команды из файла
        if (buffer[0] == '@') {
            if(!send_requests_from_file(client_socket, buffer + 1))
                continue;
        } else {
            if(handle_request(client_socket, buffer) == 1)
                return 0;
        }
    }

    close(client_socket);
    return 0;
}


int create_connection(char* server_ip, long server_port){
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }
    return client_socket;
}

int send_requests_from_file(int client_socket, char* buffer){
    FILE *fp = fopen(buffer, "r");
    if (fp == NULL) {
        perror("fopen");
        return 0;
    }
    memset(buffer, 0, BUFFER_SIZE);
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Удаляем символ новой строки из запроса

        if(handle_request(client_socket, buffer) == 1) {
            fclose(fp);
            return 1;
        }
    }
    return 0;
}
