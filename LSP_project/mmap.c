#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd;
    char *file = NULL;
    struct stat sb;
    char buf[80] ={0x00,};
    int flag = PROT_WRITE | PROT_READ;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: input\n");
        exit(1);
    }

    if ((fd = open(argv[1], O_RDWR|O_CREAT)) < 0)
    {
        perror("File Open Error");
        exit(1);
    }

    if (fstat(fd, &sb) < 0)
    {
        perror("fstat error");
        exit(1);
    }

    file = (char *)malloc(40);

    // mmap를 이용해서 열린 파일을 메모리에 대응시킨다.
    // file은 대응된 주소를 가리키고, file을 이용해서 필요한 작업을 
    // 하면 된다. 
    if ((file =
        (char *) mmap(0, 40, flag, MAP_SHARED, fd, 0)) == -1)
    {
        perror("mmap error");
        exit(1);
    }
    printf("%s\n", file);
    memset(file, 0x00, 40);
    memset(file, "TEST", 40);
    close(fd);
}
