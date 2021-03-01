#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
void sys_err(const char *str){
	perror(str);
	exit(1);
}

int main(int argc, char* argv[])
{
	int fd[2];
	pid_t pid;

	int ret = pipe(fd);
	if(ret == -1)
	{
		sys_err("pipe error");
	}

    int idx = 0;
    for(idx = 0;idx < 2;++idx)
    {
        pid = fork();
        if(pid == 0) // 子进程
        {   
            break;
        }
    }

    if(idx == 2) // 父进程
    {
        // 父进程必须关闭管道两端，否则数据无法单向流动
        close(fd[0]);  
        close(fd[1]);

        pid_t wait_ret;
        while((wait_ret = waitpid(-1, NULL, WNOHANG)) != -1)
        {
            if(wait_ret == 0)
            {
                sleep(1);
            }
        }
    }
    else if(idx == 0) // 子进程1
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);  // wc本来从STDIN读，现在从管道读端读
        execlp("wc", "wc", "-l", NULL);
    }
    else if(idx == 1) // 子进程2    
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO); // ls本来向STDOUT写，现在往管道写端写
        execlp("ls", "ls", NULL);
    }

	return 0;
}