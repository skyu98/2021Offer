#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string>

struct student {
    student(int id):id_(id), name_("unknown"){}
    student(int id, const std::string& name):id_(id), name_(name){}
    /* data */
    int id_;
    std::string name_;
};

int main()
{
    student ysy(0, "Yishiyu");
    int fd = open("temp_mmap", O_RDWR|O_CREAT|O_TRUNC, 0664);
    
    unlink("temp_mmap");
    off_t size = sizeof(ysy);
    ftruncate(fd, size);

    struct student* stu_ptr = static_cast<struct student*>
    (mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
    if(stu_ptr == MAP_FAILED)
    {
        return -1;
    }

    for(int i = 0;i < 10;++i)
    {
        memcpy(stu_ptr, &ysy, size);
        ++ysy.id_;
        sleep(1);
    }

    munmap(stu_ptr, size);
    close(fd);
    return 0;
}