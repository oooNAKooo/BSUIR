#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //для работы с файлом и ссылками
#include <sys/types.h> 
#include <sys/stat.h> //для struct stat
#include <dirent.h> //DIR*, STRUCT dirent, closedir
#include <getopt.h> //работа

void dirwalk(char *dir, int l_opt, int d_opt, int f_opt, int s_opt)
{
DIR *dp; //работа с директориями
struct dirent *entry; //для перехода к след.элу
struct stat statbuf; //для st_mode
if ((dp=opendir(dir))==NULL)
{
fprintf(stderr, "Error: cannot open dir '%s'\n", dir); //stderr - поток
return;
}
chdir(dir); //смена каталога
while((entry=readdir(dp))!=NULL)
{
lstat(entry->d_name,&statbuf); //работа с buf/statbuf
if (S_ISDIR(statbuf.st_mode)){ // S_ISDIR - это каталог? st_mode для прав доступа
if(strcmp(".",entry->d_name)==0||strcmp("..",entry->d_name)==0){
continue;
}
if(d_opt){
printf("%s/%s\n",dir,entry->d_name);
}
dirwalk(entry->d_name, l_opt, d_opt, f_opt, s_opt);
}else if (S_ISLNK(statbuf.st_mode)){
if(l_opt){
printf("%s/%s",dir,entry->d_name);
}
}else {
if(f_opt){
printf("%s/%s\n", dir, entry->d_name);
}
}
if(s_opt){
printf("%s/%s\n", dir, entry->d_name);
}
chdir("..");
closedir(dp);
}

void print_usage(char *program_name){
printf("Usage: %s[dir] [options]\n", program_name);
printf("Options:\n");
printf("\t-l -- only symbolic links (-type l)\n");
printf("\t-s -- sort output according to LC_COLLATE\n");
printf("\t-d -- only directories (-type d)\n");
printf("\t-f -- only files (-type f)\n");
}

int main(int argc, char *argv[]){
int l_opt=0;
int d_opt=0;
int f_opt=0;
int s_opt=0;

char *dir=".";

int opt;

while((opt=getopt(argc, argv, "ldfs"))!=-1){
switch(opt){
case 'l': l_opt=1;break;
case 'd': d_opt=1;break;
case 's': s_opt=1;break;
case 'f': f_opt=1;break;
default: print_usage(argv[0]);exit(EXIT_FAILURE);break;
}
}
if(optind<argc){
dir=argv[optind];
}
dirwalk(dir,l_opt,d_opt,f_opt,s_opt);
return 0;
}
