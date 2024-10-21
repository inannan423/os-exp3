#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

struct czh_rusage
{
    struct timeval ru_utime;
    struct timeval ru_stime;
    long ru_majflt;
    long ru_minflt;
    long ru_nswap;
};

int main()
{
    pid_t pid = getpid();

    // 完成自动生成一串数字，并写入 txt 文件
    FILE *fp = fopen("pid.txt", "w");
    if (fp == NULL)
    {
        perror("fopen failed");
        return -1;
    }
    fprintf(fp, "%d", pid);
    fclose(fp);

    printf("pid: %d\n", pid);

    struct czh_rusage usage;

    int ret = syscall(548, pid, &usage);
    if (ret == -1)
    {
        perror("syscall get_process_usage failed");
        return -1;
    }

    printf("User time: %ld.%06ld seconds\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("System time: %ld.%06ld seconds\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
    printf("Major faults: %ld\n", usage.ru_majflt);
    printf("Minor faults: %ld\n", usage.ru_minflt);
    printf("Swaps: %ld\n", usage.ru_nswap);

    return 0;
}