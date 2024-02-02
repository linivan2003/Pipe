#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	
	if (argc < 2) {
        fprintf(stderr, "Usage: %s <command1> <command2> ... <commandN>\n", argv[0]);
        exit(EINVAL);
    }

	int num_cmds = argc - 1; //num of command line arguments
    int pipes[num_cmds - 1][2]; // num of pipes corresponds to number of 

	for (int i = 0; i < num_cmds - 1; i++) {
    	if (pipe(pipes[i]) == -1) {
            perror("pipe");
            return -1;
        }
    }

	for (int i = 0; i < num_cmds; i++) {
        pid_t child_pid = fork();
		if (child_pid == -1) {
            perror("fork");
            return -1;
        }
		if (child_pid == 0){
			if (i == 0){   //child process first command 
				close(pipes[i][0]);
				dup2(pipes[i][1], STDOUT_FILENO);
				close(pipes[i][1]);
				execlp(argv[1], argv[1], NULL);
				perror("execlp");
				return(-1);
			} 
			else if (i == num_cmds-1){  //last process child 
				execlp(argv[num_cmds],argv[num_cmds],NULL);
				perror("execlp");
				return(-1);
			}
			else{  //middle procceses child 
				close(pipes[i][0]);
			 	dup2(pipes[i][1],STDOUT_FILENO);
			 	close(pipes[i][1]);
			 	execlp(argv[i+1],argv[i+1],NULL);
				perror("execlp");
				return(-1);
			}
		}
		else{
 		//parent process
		close(pipes[i][1]);
		dup2(pipes[i][0], STDIN_FILENO);
		close(pipes[i][0]);
		int status;
    	waitpid(child_pid, &status, 0); // Wait for the specific child process

    	 if (status != 0) {
        printf("Child process exited with non-zero status: %d\n", status);
        return (-1);
    		}
		}
	}
	  // Close all pipes in the parent
    for (int i = 0; i < num_cmds - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
	return 0;
}
