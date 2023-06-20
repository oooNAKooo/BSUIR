#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdbool.h>

#define MAX_THREADS 8192

#pragma region prototypes

struct index_s {
    double time_mark; 
    uint64_t recno; 
};

struct index_hdr_s {
    uint64_t records; 
    struct index_s idx[]; 
};

int memsize; 
int granul; 
int threads; 
char *filename; 

void *file_data; 
size_t file_size; 

struct sort_block {
    int num;
    void *start;
    size_t size;
    bool free;
};

pthread_mutex_t mutex;
pthread_t tid[MAX_THREADS];

struct sort_block *blocks;

size_t block_size;

size_t records;
struct index_s *data;
bool all = false;
bool fl = true;
bool fl2 = true;

void merge_blocks(void* args);

#pragma endregion

#pragma region sort
int cmp_index(const void *a, const void *b) {
    struct index_s *ia = (struct index_s *)a;
    struct index_s *ib = (struct index_s *)b;
    if (ia->time_mark < ib->time_mark) {
        return -1;
    } else if (ia->time_mark > ib->time_mark) {
        return 1;
    } else {
        return 0;
    }
}


void *sort_thread(void *arg) {
    int num = *(int*)arg;
    printf("Thread number: %d\n", num);
    struct sort_block *block = &blocks[num];
    block->free = false;
    printf("Data before in thread %d: %d %d %d %d\n",num, block->start, block->size, block->num, block->free);  
    struct index_s *dt = (struct index_s*) block->start;
    printf("%d %lf \n", dt->recno, dt->time_mark);
    size_t n = block->size / sizeof(struct index_s);
    qsort(dt, n, sizeof(struct index_s), 
          (int (*)(const void *, const void *)) cmp_index);
    printf("Data after in thread %d: %d %d %d %d\n",num, block->start, block->size, block->num, block->free);
    printf("%d %lf \n", dt->recno, dt->time_mark);
    while(true){
        pthread_mutex_lock(&mutex);
        all = true;
        for(int i = 0; i<granul; i++)
        {
            if(blocks[i].free == true)
            {
                all = false;
                block = &blocks[i];
                printf("Thread %d find block %d", num, block->num);
                block->free = false;
                break;
            }
        }
        
        pthread_mutex_unlock(&mutex);

        if(all)
            pthread_exit(NULL);

        dt = (struct index_s*) block->start;
        qsort(dt, n, sizeof(struct index_s), 
          (int (*)(const void *, const void *)) cmp_index);

        printf("Data after in thread %d: %d %d %d %d\n",num, block->start, block->size, block->num, block->free);
        printf("%d %d \n", dt->recno, dt->time_mark);
    } 
}

void init(/*bool start*/)
{
    int res = pthread_mutex_init(&mutex, NULL);
    if (res) {
    fprintf(stderr, "Failed mutex init \n");
    exit(1);
    }

    block_size = memsize / granul;
    printf("Block size: %d", block_size);
    //if(start){
    blocks = (struct sort_block*)malloc(sizeof(struct sort_block)*granul);
    
    struct index_hdr_s *header = (struct index_hdr_s *) file_data;
    printf("\nCount of rcords: %d\n", header->records);
    records = header->records;
    data = header->idx;
    //}
    // else{
    //     data = (struct index_s*)file_data+8; 
    // }   
}
void sort_index() {        
    for (int i = 0; i < granul; i++) {
        blocks[i].start = data+(i * block_size)/sizeof(struct index_s);//fl? (file_data + 8 + (i * block_size)): (file_data + (i * block_size));
        //fl = false;
        //printf("Data in blok i = %d %d\n",i, blocks[i].start);
        struct index_s* dt = (struct index_s*)(blocks[i].start);
        printf("Data details in blok i = %d %d %lf\n", i, dt->recno, dt->time_mark);
        blocks[i].size = block_size;//* sizeof(struct index_s);
        blocks[i].free = true;
        blocks[i].num = i;
    }
    
    for (int i = 0; i < threads; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&tid[i], NULL, sort_thread, arg);
    }
    for (int i = 0; i < threads; i++) {
        pthread_join(tid[i], NULL);
    }

    for (int i = 0; i < threads; i++) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        pthread_create(&tid[i], NULL, merge_blocks, arg);
    }
    for (int i = 0; i < threads; i++) {
        pthread_join(tid[i], NULL);
    }
}

#pragma endregion

#pragma region merge 

void merge_blocks(void* args) {
    int num1, num2;
    bool num_fl = true;
    bool isAll = false;
    while(true)
    {
        pthread_mutex_lock(&mutex);
        printf("thread %d is founding his pair\n", *(int*)args);
        for(int i = 0; i<granul; i++)
        {
            isAll=false;
            if(blocks[i].free == false){
                if(num_fl){
                    printf("He found num1 = %d", i);
                    num1 = blocks[i].num;
                    blocks[i].free = true;
                    num_fl = false;
                }
                else {
                    printf("He found num2 = %d", i);
                    num2 = blocks[i].num;
                    blocks[i].free = true;
                    num_fl = true;
                    isAll = true;
                    break;
                }
            }
        }
        pthread_mutex_unlock(&mutex);
        if(!isAll){
            printf("thread %d found nothing\n", *(int*)args);
            pthread_exit(NULL);
        }
        size_t offset1 = blocks[num1].size;
        size_t offset2 = blocks[num2].size;
        memcpy(blocks[num1].start+offset1, 
                   blocks[num2].start, 
                   offset2);
        printf("Thread %d merge %d to %d \n", *(int*)args, num2, num1);
        blocks[num1].size= offset1 + offset2;
        blocks[num1].free = false;
        if(offset1+offset2 == memsize){
            printf("Thread %d see, that 0 block is over", *(int*)args);
            pthread_exit(NULL);
        }
    }
}

#pragma endregion



int main(int argc, char *argv[]) {
    if (argc != 5) {
    printf("Usage: sort_index memsize granul threads filename\n");
    exit(1);
    }

    memsize = atoi(argv[1]);
    granul = atoi(argv[2]);
    threads = atoi(argv[3]);
    filename = argv[4];

    int fd = open(filename, O_RDWR);
    if (fd < 0) {
    perror("Error opening file");
    exit(1);
    }

    file_size = lseek(fd, 0, SEEK_END);

    int memsize_count = file_size/memsize;
    memsize = file_size;

    //for(int i = 0; i<memsize_count; i++){

        //if(i == 0){
            file_data = mmap(NULL, memsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            init();
        //}
        // else{
        //      
        //      file_data = mmap(NULL, memsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, i*memsize);
        //      init(false);
        // }
        

        if (file_data == MAP_FAILED) {
            perror("Error mapping file");
            close(fd);
            exit(1);
        }

        sort_index();

        if (munmap(file_data, memsize) < 0) {
        perror("Error unmapping file");
        }
    //}
    close(fd);
    printf("Success\n");
    return 0;
}