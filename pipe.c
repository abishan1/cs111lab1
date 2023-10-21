#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>

int main (int argc, char *argv[]) {
  // File descriptors for pipes
  int fd[2]; // fd[0] for read end (output), fd[1] for write end (input)
  int fd1[2];
  bool newpipe = true;

  if (argc <= 1) { // terminate program if there are no commands
	errno = EINVAL;
    return EINVAL;
  }

  if (argc == 2) { // handle one command (following ./pipe.c)
    if (execlp(argv[1], argv[1], NULL) == -1) {
      perror("execlp");
      return errno;
    }
    return 0;
  }

  for (int i = 1; i < argc; i++) { // handle more than one command with for loop
    if (i != argc - 1) {
      int pipe_res;

	  if (!newpipe) {
		pipe_res = pipe(fd1);
	  } else {
		pipe_res = pipe(fd);
	  }

      if (pipe_res == -1) {
        perror("pipe");
        return errno;
      }
      newpipe = !newpipe; // alternates between FDs fd and fd1
    }

    int pid = fork();
    if (pid == 0) { // handle child process behavior
      if (i != argc - 1 && i != 1) { // handle the middle commands
        if (newpipe) { 	// use the first fd
			dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            dup2(fd1[1], STDOUT_FILENO);
            close(fd1[1]);
        } else { 		// use the second fd
            dup2(fd1[0], STDIN_FILENO);
            close(fd1[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
        }
      } 
	  
	  else { // handle first or last command
        if (i == argc - 1) {
			if (newpipe) {
				dup2(fd1[0], STDIN_FILENO);
         	} else {
            	dup2(fd[0], STDIN_FILENO);
            }
        } else if (i == 1) {
			if (!newpipe) {
				dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            } else {
				dup2(fd1[1], STDOUT_FILENO);
            	close(fd1[1]);
            }
        }
        close(newpipe ? fd1[0] : fd[0]);
      }

      if (execlp(argv[i], argv[i], NULL) == -1) {
        perror("execlp");
        return errno;
      }
    }
	
	else if (pid != 0) { // handle parent process behavior
      int wstatus;
      wait(&wstatus);

      if (!WIFEXITED(wstatus)) {
        return ECHILD;
      }
	  
	  if (WEXITSTATUS(wstatus) != 0) {
        return WEXITSTATUS(wstatus);
      }

      if (i == argc - 1) {
        close(newpipe ? fd1[0] : fd[0]);
      } else if (i == 1) {
        close(newpipe ? fd1[1] : fd[1]);
      } else {
        close(newpipe ? fd[0] : fd1[0]);
        close(newpipe ? fd1[1] : fd[1]);
      }
    } 
	else {
      perror("fork");
      return 1;
    }
  }
}
