/***********************************************************************
name:
	assignment4 -- acts as a pipe using ":" to seperate programs.
description:	
	See CS 360 Processes and Exec/Pipes lecture for helpful tips.
***********************************************************************/

/* Includes and definitions */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <assert.h>

/**********************************************************************
./assignment4 <arg1> : <arg2>

    Where: <arg1> and <arg2> are optional parameters that specify the programs
    to be run. If <arg1> is specified but <arg2> is not, then <arg1> should be
    run as though there was not a colon. Same for if <arg2> is specified but
    <arg1> is not.
**********************************************************************/

int main(int argc, char *argv[]){
	if (argc < 2) {  // when argument is not given
	    fprintf(stdout, "Usage: ./assignment4 <arg1> : <arg2>\n");
	    fprintf(stdout, "Where: <arg1> and <arg2> are optional parameters\n");
	    return -1;
	}

	char *left[5];
	char *lName = NULL;
	char *rName = NULL;
	char *right[5];
	bool stillLeft = true;
	int l, r;
	l= r = 0;

	// read from arguments to divide into pieces
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], ":") == 0) {
            stillLeft = false;
            if (i + 1 == argc)  // no <arg2> provided
                break;
            ++i;
        }

        if (stillLeft) {  // <argv1>
            if (lName == NULL) {
                lName = argv[i];
                left[l++] = argv[i];
            }
            else
                left[l++] = argv[i];
        }
        else { // <argv2>
            if (rName == NULL) {
                rName = argv[i];
                right[r++] = argv[i];
            }
            else
                right[r++] = argv[i];
        }
    }
    left[l++] = right[r++] = NULL;  // to terminate char*

    int fd[2], reader, writer;
    assert (pipe (fd) >= 0);
	reader = fd[0];
	writer = fd[1];

	if (fork()) {  // parent
        close(writer);

        if (rName != NULL && lName != NULL) {  // <argv1> and <argv2> provided
            dup2(reader, STDIN_FILENO);
            dup2(writer, STDOUT_FILENO);
            close(writer);
            wait(0);  // wait for child to finish

            if (execvp(rName, right) < 0) {
                printf("%s\n", strerror(errno));
            }
            close(reader);
        }

        if (rName == NULL) {  // only <argv1> provided
            if (execvp(lName, left) < 0) {
                printf("%s\n", strerror(errno));
                return (-errno);
            }
        }

        if (lName == NULL) {  // only <argv2> provided
            if (execvp(rName, right) < 0) {
                printf("%s\n", strerror(errno));
                return (-errno);
            }
        }
        close(reader);
        close(writer);
	}
	else {
	    // child process, run the <argv1> and redirect the output to the stdin
	    // for parent to run the <argv2>
	    close(reader);
	    dup2(writer, STDOUT_FILENO);
	    if (execvp(lName, left)< 0) {
            printf("%s\n", strerror(errno));
	        return (-errno);
	    }
	}
    return 0;
}