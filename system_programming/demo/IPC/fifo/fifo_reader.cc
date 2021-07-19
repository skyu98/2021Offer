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

    char content[4096];
    int fifo = open(argv[1], O_RDONLY, 0664);
    for(int i = 0;i < 10;++i)
    {
        read(fifo, content, sizeof(content));
        std::cout << content << std::endl;
    }
    
    return 0;
}