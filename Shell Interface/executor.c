/* Jessica Shi, jshi2021, 117976373 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <sysexits.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "command.h"
#include "executor.h"

/* static void print_tree(struct tree *t);*/
static int traverse_tree(struct tree *t, int input, int output);

int execute(struct tree *t) {
   
   /* If tree isn't null, traverse it. Otherwise, return 0 */
   if(t != NULL){
       return traverse_tree(t, -1, -1);
   }
   return 0;
}

/* Helper method to traverse tree recursively */
static int traverse_tree(struct tree *t, int input, int output){
    pid_t pid;
    int status_waiting, status;

    /* Start by handling nodes with conjunction NONE */
    if (t->conjunction == NONE){
       
        /* If command is exit, program exits */
        if(strcmp(t->argv[0], "exit") == 0){
            exit(0);
        } 
        
        /* If command is cd, change directory with chdir() */
        else if(strcmp(t->argv[0], "cd") == 0){
            if(t->argv[1] != NULL){
                chdir(t->argv[1]);
            }
        }

        /* If other commands are called */
        else{
            if((pid = fork()) < 0){
                perror("fork");
            }
            else{
                /* Executes command with child process */
                if(pid == 0){

                    /* Handles and checks input redirection*/
                    if(t->input != NULL){
                        if((input = open(t->input, O_RDONLY)) == -1){
                           perror("input");
                        }
                        if(dup2(input, STDIN_FILENO) < 0){
                           perror("dup2 input");
                        }
                        close(input);
                    }

                    /* Handles and checks output redirection*/
       	       	    if(t->output != NULL){
       	       	       	if((output = open(t->input, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1){
                           perror("output");
                        }
       	       	       	if(dup2(output, STDOUT_FILENO) < 0){
      	       	       	   perror("dup2	output");
       	       	       	}
       	       	       	close(output);
       	       	    }

                    /* Executes the command */
                    execvp(t->argv[0], t->argv);
                    
                    /* If failed, message will print */
                    fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
                    exit(-1);
                }

                /* Parent process waits, then returns status*/
                else{
                    wait(&status_waiting);
                    return status_waiting;
                }
            }
        }
    }
  
    /* &&, redirection and executes left and right nodes */
    else if(t->conjunction == AND){
        
        /* Checks and deals with input/output redirection */
        if(t->input != NULL){
            if((input = open(t->input, O_RDONLY)) == -1){
                exit(-1);
            }        
        }
        
        if(t->output != NULL){
            if((output = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1){
                exit(-1);
            }
        }

        /* Executes left node, then right node if status is right */
        status = traverse_tree(t->left, input, output);
        if(status == 0){
            traverse_tree(t->right, input, output);
        }
    }    
 
    /* Handles redirection and creates a child process to execute command */
    else if(t->conjunction == PIPE){
        int pipefd[1];

        /* Checks if a left child has a redirect and pipe at same time */
        if(t->left != NULL){
           if(t->left->output){
               printf("Ambiguous output redirect.\n");
           }
        }

        /* Checks if a right child has redirect and pipe at same time */
        if(t->right != NULL){
           if(t->right->input){
               printf("Ambigous input redirect.\n");
           }
        }

        /* Checks and deals with input/output redirection */
	if(t->input != NULL){
            if((input = open(t->input, O_RDONLY)) == -1){
                exit(-1);
	    }
       	}

       	if(t->output != NULL){
            if((output = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1){
                exit(-1);
            }
        }

        /* Create the pipe */
        if(pipe(pipefd) < 0){
            perror("pipe");
        }
 
        /* Use fork to create a child process */
        if((pid = fork()) < 0){
            perror("fork");
        }
        else{
      
            /* Child process executes the left node */
            if(pid == 0){
               close(pipefd[0]); /* Closes the read end*/
               if(dup2(pipefd[1], STDOUT_FILENO) < 0){
                   perror("dup2 output");
               }

               traverse_tree(t->left, input, pipefd[1]);
               close(pipefd[1]);
               exit(0);
            }
 
            /* Parent process executes the right node */
            else{
               close(pipefd[1]); /*Closes the write end*/
               if(dup2(pipefd[0], STDIN_FILENO) < 0){
                   perror("dup2");
               }

               traverse_tree(t->right, pipefd[0], output);
               close(pipefd[0]);
               wait(NULL); /*For child*/
            }
        }
    }

    /* Subshell, handles redirection and creates subshell */
    else if(t->conjunction == SUBSHELL){
        /* Checks and deals with input/output redirection */
        if(t->input != NULL){
            if((input = open(t->input, O_RDONLY)) == -1){
                exit(-1);
            }
        }

	if(t->output != NULL){
            if((output = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1){
                exit(-1);
            }
	}

        /* Creates a fork */
        if((pid = fork()) < 0){
           perror("fork");
        }
        else{
           /* Child executes left node */
           if(pid == 0){
               traverse_tree(t->left, input, output);
               exit(0);
           }
          
           /* Parent */
           else{
               wait(NULL);
           }
        }
    }
    return 0;
}

/*static void print_tree(struct tree *t) {*/
  /* if (t != NULL) {*/
    /*  print_tree(t->left);*/

     /* if (t->conjunction == NONE) {*/
       /*  printf("NONE: %s, ", t->argv[0]);*/
     /* } else {*/
       /*  printf("%s, ", conj[t->conjunction]);*/
     /* }*/
     /* printf("IR: %s, ", t->input);*/
     /* printf("OR: %s\n", t->output);*/

     /* print_tree(t->right);*/
  /* }*/
/*}*/

