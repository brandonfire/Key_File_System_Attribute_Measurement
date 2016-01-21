#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <linux/futex.h>
#include <fcntl.h>
#include <sys/stat.h>
#define BLOCKSIZE 4096
unsigned long long rdtsc()
{
unsigned low_end, high_end;
unsigned long long result;
__asm__("rdtsc\r\n" : "=d" (high_end), "=a" (low_end));
result = high_end;
result = (result<<32) | low_end;
return	result;
}
int main(int argc, char **argv)
{
    unsigned long long start,end,timeu;
    struct timeval start1, end1;
    cpu_set_t my_set;
    CPU_ZERO ( &my_set );
    CPU_SET(3, &my_set);
    sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
    gettimeofday(&start1,NULL);
    start = rdtsc();
    sleep(1);
    end = rdtsc();
    gettimeofday(&end1,NULL);
    timeu=(end1.tv_sec * 1000000 + end1.tv_usec)- (start1.tv_sec * 1000000 + start1.tv_usec);
    printf(" 1 second: %u cycles, %u us by timer gettimeofday()\n",end-start,timeu);
    gettimeofday(&start1,NULL);
    start = rdtsc();
    sleep(2);
    end = rdtsc();
    gettimeofday(&end1,NULL);
    timeu=(end1.tv_sec * 1000000 + end1.tv_usec)- (start1.tv_sec * 1000000 + start1.tv_usec);
    printf(" 2 seconds: %u cycles %u us by timer gettimeofday()\n",end-start,timeu);
    int readsize;
    int readloops=10;
    for(readsize=1;readsize<33554433;readsize=readsize*2){
        FILE *fp=fopen("myfile1", "w");
        fseek(fp, 0, SEEK_SET);
        int size;
        for(size=0;size<readsize;size++)
            fputc('a', fp);
        fclose(fp);
        FILE *f = fopen("myfile1", "rb");
        char *string = malloc(readsize+1);
        for(size=0;size<readloops;size++){

            fseek(f,0,SEEK_SET);
            start = rdtsc();
            fread(string, readsize, 1, f);
            end = rdtsc();
            if(size==0)
                printf("Initial  read %d bytes time cost: %u cycles \n",readsize+1, (end-start));
            else if(size==readloops-1)
                printf("Repeated read %d bytes time cost: %u cycles \n",readsize+1, (end-start));
    }

    free(string);
    fclose(f);
}
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int fd = open("myfile_ip",O_CREAT | O_WRONLY | O_TRUNC, mode);
        int writeloops;
        char *buffer = malloc(BLOCKSIZE);
        int i;
        for(i=0;i<4095;i++){
            *buffer++ = 'd';
        }
        *buffer='\0';
        buffer = buffer-4095;
        for(writeloops=0; writeloops<30;writeloops++){
            start = rdtsc();
            write(fd,buffer,BLOCKSIZE);
            fsync(fd);
            end = rdtsc();
            printf("%dth block write time cost: %u cycles \n", writeloops+1, end-start);
        }
        close(fd);
    
    return	0;

}
