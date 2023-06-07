#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstdio>

using namepsace std;

void findfile(char *path, char *find)
{
DIR *dir;
if((dir=opendir(path))!=NULL){
struct dirent *entry;
while((entry = readdir(dir))!=NULL){
struct stat st;
char filename[512];snprintf(filename, sizeof(filename), "%s/%s", path, entry->d_name);
lstat(filename, &st);
if(S_ISDIR(st.st_mode)){
findfile(filename, find);
}
else if (S_ISLINK(st.st_mode)){
if(strcmp(entry->d_name, find)){
}
}
}
}
closedir(dir);
}
int main()
{
findfile();

return 0;
}
