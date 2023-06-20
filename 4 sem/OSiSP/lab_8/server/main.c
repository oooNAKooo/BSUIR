#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

#define ERROR_INFO_FILE "Error: opening info file"
#define INFO_FILE "server_info.txt"

pthread_mutex_t mutex; // To control thread_counter.

typedef struct client_thread_args client_thread_args;
struct client_thread_args {
    int sock;
    char *root_dir;
};

int create_socket(long);

void *client_handler(void *);

int send_file(int, char *);

char *get_file_fullname(char *, char *);

void proceed_cd_command(int, char **, char *, char *);

void proceed_list_command(int, char *, char *);

int compareFilenames(const char *, const char *);

void sortFilenameList(char **, int);


int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: ./srvr <port> <root_dir>\n");
        return -1;
    }
    long port = strtol(argv[1], NULL, 10);
    if (port <= 0 || port > 65535) {
        printf("Invalid port number: %ld\n", port);
        return -1;
    }

    if (chdir(argv[2]) < 0) {
        perror("chdir");
        return -1;
    }
    char *root_dir = (char *) calloc(BUFFER_SIZE, sizeof(char));
    if (!getcwd(root_dir, BUFFER_SIZE)) {
        perror("getcwd");
        return -1;
    }


    int server_socket = create_socket(port);
    if (server_socket < 0) {
        fprintf(stderr, "error create socket\n");
        return -1;
    }
    printf("Server started 127.0.0.1:%ld\n", port);
    printf("With root directory %s\n", root_dir);


    pthread_mutex_init(&mutex, NULL);
    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_len);
        if (client_socket == -1) {
            perror("Failed to accept client connection");
            continue;
        }
        printf("Accepted new client connection\n");

        pthread_t thread;
        struct client_thread_args args = {client_socket, root_dir};
        if (pthread_create(&thread, NULL, client_handler, &args) != 0) {
            perror("Failed to create client thread");
            close(client_socket);
            break;
        }
        pthread_detach(thread);
    }

    pthread_mutex_destroy(&mutex);
    free(root_dir);
    close(server_socket);
    return 0;
}

int create_socket(long port) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create server socket");
        return -1;
    }

    const int yes = 1;
    // Helps with "Address already in use" error
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes))) {
        perror("setsockopt");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind server socket");
        return -1;
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Failed to listen on server socket");
        return -1;
    }

    return server_socket;
}

void *client_handler(void *arg) {
    struct client_thread_args data = *(client_thread_args *) arg;

    char *current_dir = (char *) calloc(BUFFER_SIZE, sizeof(char));
    strcpy(current_dir, data.root_dir);

    char *info_file = get_file_fullname(data.root_dir, INFO_FILE);

    if (send_file(data.sock, info_file)) {
        send(data.sock, ERROR_INFO_FILE, strlen(ERROR_INFO_FILE), 0);
        goto EXIT;
    }
    while (1) {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        ssize_t n = recv(data.sock, buffer, BUFFER_SIZE, 0);
        if (n <= 0) {
            break;
        }
        if (strncmp(buffer, "ECHO", 4) == 0) {
            send(data.sock, buffer + 5, n - 5, 0);
        } else if (strcmp(buffer, "QUIT") == 0) {
            // обработка QUIT запроса
            send(data.sock, "BYE", 3, 0);
            break;
        } else if (strcmp(buffer, "INFO") == 0) {
            // обработка INFO запроса
            send_file(data.sock, info_file);
        } else if (strncmp(buffer, "CD ", 3) == 0) {
            // обработка CD запроса
            proceed_cd_command(data.sock, &current_dir, data.root_dir, buffer);
        } else if (strcmp(buffer, "LIST") == 0) {
            proceed_list_command(data.sock, current_dir, data.root_dir);
        } else {
            // неизвестный запрос
            char *message = "Unknown command";
            send(data.sock, message, strlen(message), 0);
        }
    }

    EXIT:
    close(data.sock);
    free(info_file);
    free(current_dir);
    shutdown(data.sock, SHUT_RDWR);
    pthread_exit(NULL);
}

int send_file(int socket, char *filename) {
    FILE *info_file = fopen(filename, "r");
    if (info_file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // отправляем содержимое файла на клиент
    char line[BUFFER_SIZE];
    while (fgets(line, BUFFER_SIZE, info_file) != NULL) {
        send(socket, line, strlen(line), 0);
    }
    fclose(info_file);
    return 0;
}

char *get_file_fullname(char *current_dir, char *filename) {
    char *full_name = (char *) calloc(BUFFER_SIZE, sizeof(char));
    strcpy(full_name, current_dir);
    strcat(full_name, "/");
    strcat(full_name, filename);
    return full_name;
}

void proceed_cd_command(int client_socket, char **current_dir, char *root_dir, char *buffer) {
    char temp_name[BUFFER_SIZE];
    memset(temp_name, 0, BUFFER_SIZE);
    strcpy(temp_name, buffer + 3);

    memset(buffer, 0, BUFFER_SIZE);

    pthread_mutex_lock(&mutex);

    if (chdir(*current_dir) == -1) {
        perror("chdir");
        return;
    }
    if (chdir(temp_name) < 0) {
        // отправляем ошибку клиенту
        strcpy(buffer, "Error: Unable to change directory.");
    } else {
        memset(temp_name, 0, BUFFER_SIZE);
        if (!getcwd(temp_name, BUFFER_SIZE * sizeof(char))) {
            perror("getcwd");
            return;
        }
        if (strlen(temp_name) < strlen(root_dir)) {
            // вышли за рамки установленного сервером каталога
            strcpy(buffer, "Error: Going beyond the server's root directory");
        } else {
            // отправляем подтверждение клиенту
            memset(*current_dir, 0, BUFFER_SIZE);
            if (!getcwd(*current_dir, BUFFER_SIZE * sizeof(char))) {
                perror("getcwd");
                return;
            }
            strcpy(buffer, "CD ");
            strcat(buffer, *current_dir + strlen(root_dir) + 1);
        }
    }
    if (chdir(root_dir) == -1) {
        perror("chdir");
        return;
    }
    pthread_mutex_unlock(&mutex);
    send(client_socket, buffer, strlen(buffer), 0);
}

void proceed_list_command(int client_socket, char *current_dir, char *root_dir) {
    char *file_name = (char *) calloc(BUFFER_SIZE, sizeof(char));
    int file_count = 0;
    char *fileNameBuffer = (char *) calloc(BUFFER_SIZE * 2, sizeof(char));

    // открыть текущий каталог
    DIR *dir = opendir(current_dir);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    pthread_mutex_lock(&mutex);

    if (chdir(current_dir) == -1) {
        perror("chdir");
        return;
    }

    struct dirent *ent;
    // перебрать все файлы в каталоге
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            // пропустить ссылки на текущий и родительский каталоги
            continue;
        }
        memset(file_name, 0, BUFFER_SIZE);
        if (ent->d_type == DT_DIR) {
            // это каталог
            sprintf(file_name, "%s/\n", ent->d_name);
        } else if (ent->d_type == DT_REG) {
            // это обычный файл
            sprintf(file_name, "%s\n", ent->d_name);
        } else if (ent->d_type == DT_LNK) {
            // это символическая ссылка
            char link_target[BUFFER_SIZE];
            memset(link_target, 0, BUFFER_SIZE);
            ssize_t len = readlink(ent->d_name, link_target, BUFFER_SIZE - 1);
            if (len != -1) {
                link_target[len] = '\0';

                // Определяем тип стрелки
                char arrow_type[6];
                memset(arrow_type, 0, 6);
                struct stat st;
                if (lstat(link_target, &st) == 0 && S_ISREG(st.st_mode)) {
                    strcpy(arrow_type, "-->");
                } else {
                    strcpy(arrow_type, "-->>");
                }
                strcat(file_name, ent->d_name);
                strcat(file_name, " ");
                strcat(file_name, arrow_type);
                strcat(file_name, " ");
                strcat(file_name, link_target);
                strcat(file_name, "\n");
            } else {
                perror("readlink");
                strcat(file_name, ent->d_name);
                strcat(file_name, "\n");
            }
        } else {
            // неизвестный тип файла
            continue;
        }
        ++file_count;
        strcat(fileNameBuffer, file_name);
    }
    fileNameBuffer[strlen(fileNameBuffer)] = '\0';

    char **fileNames = (char **) malloc(sizeof(char *) * file_count);
    for (int i = 0; i < file_count; ++i) fileNames[i] = (char *) malloc(sizeof(char) * 100);

    size_t length = strlen(fileNameBuffer);
    int k = 0;
    for (size_t i = 0, j = 0; i < length; ++i, ++k) {
        if (fileNameBuffer[i] == '\n') {
            fileNames[j][k] = '\0';
            ++j;
            k = -1;
            if (i + 1 == length) break;
            continue;
        }
        fileNames[j][k] = fileNameBuffer[i];
    }
    sortFilenameList(fileNames, file_count);

    char *message = (char *) calloc(BUFFER_SIZE * 2, sizeof(char));

    for (int i = 0; i < file_count; ++i) {
        strcat(message, fileNames[i]);
        strcat(message, "\n");
    }
    if (strlen(message))
        message[strlen(message) - 1] = '\0';
    else
        strcat(message, " ");


    if (chdir(root_dir) == -1) {
        perror("chdir");
        return;
    }
    pthread_mutex_unlock(&mutex);


    send(client_socket, message, strlen(message), 0);

    for (int i = 0; i < file_count; ++i) free(fileNames[i]);
    free(fileNames);
    free(file_name);
    free(fileNameBuffer);
    free(message);

    // закрыть каталог
    closedir(dir);
}

void sortFilenameList(char **list, int length) {
    char *current;
    for (int i = 1; i < length; ++i) {
        current = list[i];
        int j = i;
        while (j > 0) {
            if (compareFilenames(current, list[j - 1]) < 0) {
                list[j] = list[j - 1];
            } else {
                break;
            }
            --j;
        }
        list[j] = current;
    }
}

// firstly alphabet, secondly len
int compareFilenames(const char *filename1, const char *filename2) {
    int i = 0;
    while (filename1[i] != '\n' || filename2[i] != '\n') {
        if (filename1[i] == '\n') {
            // Name 1 is shorter
            return -1;
        }

        if (filename2[i] == '\n') {
            // Name 2 is shorter
            return 1;
        }

        int cmp = filename1[i] - filename2[i];

        if (cmp != 0) {
            return cmp;
        }

        ++i;
    }
    return 0;
}

