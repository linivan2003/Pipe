#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	printf("number of command-line arguments: %d\n",argc);
	//print each command line argument
	for  (int i = 0; i<argc;i++){
		printf("Argument %d: &s\n",i,argv[i]);
	}
	int pipefd[2];
	pipe(pipefd);
	pid_t child_pid = fork();
	if (child_pid == 0){
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		execlp(argv[1], argv[1], NULL);


	} else{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		execlp(argv[2], argv[2], NULL);
	}
	printf("This line should not be reached.\n");

	return 0;
}
