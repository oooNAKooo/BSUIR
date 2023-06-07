#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[], char *envp[]){
    printf("CHILD process %s started with pid %d and ppid %d\n", argv[0], getpid(), getppid());
    FILE* env_file =fopen(argv[1], "r");
    if(env_file==NULL){
        perror("fopen");
        exit(EXIT_FAILURE);}
    char *line = NULL;
    size_t len=0;
    while(getline(&line,&len, env_file)!=-1){
        line[strcspn(line,"\n")]='\0';
        char *value=getenv(line);
        printf("%s=%s\n",line,value!=NULL?value:"(NULL)");}
        free(line);
    fclose(env_file);
    exit(EXIT_SUCCESS);
}
