#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <iostream>

struct student
{
    student(int id):id_(id), name_("unknown"){}
    student(int id, const std::string& name):id_(id), name_(name){}
    /* data */
    int id_;
    std::string name_;
};

int main()
{
    int fd = open("temp_mmap", O_RDONLY, 0664);
    
    off_t size = sizeof(struct student);

    struct student* stu_ptr = static_cast<struct student*>
    (mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0)); // 权限要小于open时
    if(stu_ptr == MAP_FAILED)
    {
        return -1;
    }

    for(int i = 0;i < 10;++i)
    {
       std::cout << "id = "  << stu_ptr->id_
                << ", name = " << stu_ptr->name_
                << std::endl;
        sleep(1);
    }

    munmap(stu_ptr, size);
    close(fd);
    return 0;
}