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

	pid = fork();
	if(pid > 0)  // 父进程
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);  // wc本来从STDIN读，现在从管道读端读
		execlp("wc", "wc", "-l", NULL);
	}
	else if(pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO); // ls本来向STDOUT写，现在往管道写端写
		execlp("ls", "ls", NULL);
	}

	return 0;
}