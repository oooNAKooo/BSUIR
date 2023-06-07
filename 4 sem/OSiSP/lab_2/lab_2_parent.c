#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CHILDREN 100

int main(int argc, char *argv[], char *envp[]) {
    char** sorted_envp = (char**) malloc(sizeof(char*) * (argc + 1));
    memcpy(sorted_envp, envp, sizeof(char*) * argc); 
    sorted_envp[argc] = NULL;
    qsort(sorted_envp, argc, sizeof(char*), (int(*)(const void*, const void*)) strcmp);

    int num_children = 0;
    char child_num[3];
    while (1) {
        printf("Enter command (+, *, &, q): ");
        fflush(stdout);

        char c;
        scanf("%c", &c);
        while (getchar() != '\n'); // очистка буфера ввода
        if (c == '+') {
            if (num_children >= MAX_CHILDREN) {
                fprintf(stderr, "Maximum number of children reached\n");
                continue;
            }

            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                sprintf(child_num, "%02d", num_children++);
                char child_name[20];
                sprintf(child_name, "child_%s", child_num);
                char path[100];
                strcpy(path, child_name);
                strcat(path, "env.txt");
                char *child_argv[] = {child_name, path, NULL};
                execve(child_name, child_argv, sorted_envp);
                perror("execve");
                exit(EXIT_FAILURE);
            }
        } else if (c == '*' || c == '&') {
            if (num_children >= MAX_CHILDREN) {
                fprintf(stderr, "Maximum number of children reached\n");
                continue;
            }

            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                sprintf(child_num, "%02d", num_children++);
                char child_name[20];
                sprintf(child_name, "child_%s", child_num);
                char path[100];
                strcpy(path, child_name);
                strcat(path, "env.txt");
                char *child_argv[] = {child_name, path, NULL};
                execve(child_name, child_argv, c == '*' ? sorted_envp : environ);
                perror("execve");
                exit(EXIT_FAILURE);
            }
        } else if (c == 'q') {
            break;
        } else if (c != '\n') {
            printf("Invalid command\n");
        }
    }

    free(sorted_envp);
    exit(EXIT_SUCCESS);
}
