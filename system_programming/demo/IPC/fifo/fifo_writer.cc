#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		std::cout << "Need the name of fifo!" << std::endl;
		return -1;
	}

    std::string content = "fifo test";
    int fifo = open(argv[1], O_WRONLY, 0664);
    for(int i = 0;i < 10;++i)
    {
        std::string tmp = content + std::to_string(i);
        write(fifo, tmp.data(), tmp.size());
        sleep(1);
    }
    close(fifo);

    return 0;
}