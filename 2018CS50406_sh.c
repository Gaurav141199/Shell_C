#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/stat.h>


#define clear() printf("\033[H\033[J")

// Greeting shell during startup
void init_shell()
{
	clear();
	printf("\033[0;35m");
	printf("\n\n\n******************"
		"************************");
	printf("\n\n\t    ****MY SHELL****");
	printf("\n\n    -Smile because you would love it-");
	printf("\n\n\n*******************"
		"***********************");
		printf("\033[0m");
	char* username = getenv("USER");
	printf("\033[0;36m");
	printf("\n\n\nUSER is: @%s", username);
	printf("\n");
	printf("\033[0m");
}

void parsecmd(char cmd[], char* argsv[]){
	char* token;
	token = strtok(cmd, " ");
	int i=0;
	while(token!=NULL){
		argsv[i]=token;
		token = strtok(NULL, " ");
		i++;
	}
	argsv[i]=NULL;
}


void printDir()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("\033[1;33m");
  printf("Dir: %s", cwd);
  printf("\033[0m");

}

void changedirectory(char word[])
{
	// for()
	// using the command
	// printf("%s\n",word );
	if(chdir(word)==0){
		printDir();
		printf("\033[0;34m");
    printf("%s","/shell>> ");
    printf("\033[0m");
		printf("\033[1;32m");
    printf("Directory changed\n");
    printf("\033[0m");

	}
	else{
		printDir();
		printf("\033[0;34m");
    printf("%s","/shell>> ");
    printf("\033[0m");
		printf("\033[1;31m");
    printf("Directory not FOUND\n");
    printf("\033[0m");

	}
}

void makedirectory(char word[]){
	if(mkdir(word,0777)==0){
		printDir();
		printf("\033[0;34m");
    printf("%s","/shell>> ");
    printf("\033[0m");
		printf("\033[1;32m");
		printf("Directory %s made\n", word);
    printf("\033[0m");
	}
	else{
		printDir();
		printf("\033[0;34m");
    printf("%s","/shell>> ");
    printf("\033[0m");
		printf("\033[1;31m");
		printf("ERROR\n");
		printf("\033[0m");
	}
}

void removedirectory(char word[]){
	if(rmdir(word)==0){
		printDir();
		printf("\033[0;34m");
    printf("%s","/shell>> ");
    printf("\033[0m");
		printf("\033[1;32m");
		printf("Directory %s deleted.\n", word);
    printf("\033[0m");
	}
	else{
		printDir();
		printf("\033[0;34m");
    printf("%s","/shell>> ");
    printf("\033[0m");
		printf("\033[1;31m");
		printf("ERROR\n");
		printf("\033[0m");
	}
}

void execArgs(char* parsed[])
{
	// Forking a child
	pid_t pid = fork();

	if (pid == -1) {
		printDir();
		printf("\033[0;34m");
    printf("%s","/shell>> ");
    printf("\033[0m");
		printf("Failed forking child..\n");
		return ;
	} else if (pid == 0) {
		if (execvp(parsed[0], parsed) < 0) {
			// if (execvp(parsed[0], parsed) < 0) {
			printDir();
			printf("\033[0;34m");
	    printf("%s","/shell>> ");
	    printf("\033[0m");
			printf("\033[1;31m");
			printf("Could not execute command3..\n");
			printf("\033[0m");
		}
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return ;
	}
}

// Function where the piped system commands is executed
void execArgsPiped(char** parsed)
{
	// 0 is read end, 1 is write end
	int pipefd[2];
	pid_t p1, p2;

	if (pipe(pipefd) < 0) {
		printDir();
		printf("\033[0;34m");
    printf("%s","/shell>> ");
    printf("\033[0m");
		printf("\033[1;31m");
		printf("Pipe could not be initialized\n");
		printf("\033[0m");
		return;
	}
	p1 = fork();
	if (p1 < 0) {
		printf("\033[1;31m");
		printf("\nCould not fork\n");
		printf("\033[0m");

		return;
	}

	if (p1 == 0) {
		// Child 1 executing..
		// It only needs to write at the write end
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		if (execvp(parsed[0], parsed) < 0) {
			printDir();
			printf("\033[0;34m");
	    printf("%s","/shell>> ");
	    printf("\033[0m");
			printf("\033[1;31m");
			printf("Could not execute command 1..\n");
			printf("\033[0m");

			exit(0);
		}
	} else {
		// Parent executing
		p2 = fork();

		if (p2 < 0) {
			printDir();
			printf("\033[0;34m");
	    printf("%s","/shell>> ");
	    printf("\033[0m");
			printf("\033[1;31m");
			printf("Could not fork\n");
			printf("\033[0m");
			return;
		}

		// Child 2 executing..
		// It only needs to read at the read end
		if (p2 == 0) {
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			if (execvp(parsed[2], parsed) < 0) {
				printDir();
				printf("\033[0;34m");
		    printf("%s","/shell>> ");
		    printf("\033[0m");
				printf("\033[1;31m");
				printf("Could not execute command 2..\n");
				printf("\033[0m");
				exit(0);
			}
		} else {
			// parent executing, waiting for two children
			wait(NULL);
			wait(NULL);
		}
	}
}



void check(char *words[1000]){

	if(strcmp("cd",words[0])==0){
		changedirectory(words[1]);
	}
	else if(strcmp("pwd",words[0])==0){
		printDir();
		printf("\n");
	}
	else if(strcmp("mkdir",words[0])==0){
		makedirectory(words[1]);
	}
	else if(strcmp("rmdir",words[0])==0){
		removedirectory(words[1]);
	}
	else if (strcmp("exit",words[0])==0){
		printf("");
	}
	else if (strcmp("clear",words[0])==0){
		clear();
	}
	else if(words[1]!=NULL){

	if(strcmp(">",words[1])==0){
            // printf("test");
						pid_t pid = fork();
						// printf("step1" );

						if (pid == -1) {
							printDir();
							printf("\033[0;34m");
					    printf("%s","/shell>> ");
					    printf("\033[0m");
							printf("\033[1;31m");
							printf("\nFailed forking child..\n");
							printf("\033[0m");

							return;
						}
						else if (pid == 0) {
							// printf("step2");
							FILE * fPtr;

				    fPtr = freopen(words[2], "w+",stdout );
								// if(fPtr == NULL)
							 // {
								// 	 /* File not created hence exit */
								// 	 printDir();
			 					// 	printf("%s","/shell>> ");
								// 	 printf("Unable to create file.\n");
								// 	 exit(EXIT_FAILURE);
							 // }

								 	int res = execvp(words[0], words);
									fclose(fPtr);
								  if (res < 0) {

									// printf("%s\n","hue1" );
				 						// if (execvp(parsed[0], parsed) < 0) {

										// printf("%s\n","test" );
										printDir();
										printf("\033[0;34m");
								    printf("%s","/shell>> ");
								    printf("\033[0m");
										printf("\033[1;31m");
									  printf("Could not execute command1..\n");
										printf("\033[0m");

				 			 }
							 exit(0);
					}

					else {
						// printf("%s\n","hue" );
							// printf("step3");
						wait(&pid);

						return;
					}
				}

				else if(strcmp("<",words[1])==0){

					// printf("%s\n","step1" );
					pid_t pid = fork();

					if (pid == -1) {
						printDir();
						printf("\033[0;34m");
				    printf("%s","/shell>> ");
				    printf("\033[0m");
						printf("\033[1;31m");

						printf("Failed forking child..\n");
						printf("\033[0m");
						return;
					} else if (pid == 0) {
						// printf("%s\n","step0" );
						FILE * fPtr;

					fPtr = freopen(words[2], "r",stdin );
							if(fPtr == NULL)
						 {
								 /* File not created hence exit */
								 printDir();
						 		printf("\033[0;34m");
						     printf("%s","/shell>> ");
						     printf("\033[0m");
								 printf("\033[1;31m");

		 						 printf("Unable to create file.\n");
		 						 printf("\033[0m");

								 exit(EXIT_FAILURE);
						 }
						 else{
								int res = execvp(words[0], words);
								fclose(fPtr);
								if (res < 0) {
									// printf("%s\n","hue1" );
									// if (execvp(parsed[0], parsed) < 0) {
									printDir();
									printf("\033[0;34m");
							    printf("%s","/shell>> ");
							    printf("\033[0m");
									printf("\033[1;31m");

 		 						 printf("Could not execute command2..\n");
 		 						 printf("\033[0m");

						 }
							}
				}

				else {
					// printf("%s\n","hue" );e
					wait(NULL);
					return;
				}
			}


			else if (strcmp("|",words[1])==0){
				execArgsPiped(words);
			}
		}
		else{
			execArgs(words);
		}
	}



int main(int argc, char** argv){
	// load the configuration
	char input[1024];
	int status =0;
	init_shell();
	int k = 1;
	int g;
	while(k==1){
		printDir();
		printf("\033[0;34m");
		printf("%s","/shell>> " );
		printf("\033[0m");
		// fgets(input, 1024, stdin);

		gets(input);
		// char words[1000][1000];
		char *arr[1000];
		// printf("%s\n",input);
		// char fword[200];
		parsecmd(input,arr);
		// int h;
		// int g=0;
		// int ctr=0;
		// for(int i=0;i<=strlen(input);i++){
		// 	if(input[i]==' '|| input[i]=="\0"){
		// 		words[ctr][g]='\0';
		// 		ctr++;
		// 		g=0;
		// 	}
		// 	else {
		// 		words[ctr][g]=input[i];
		// 		g++;
		// 	}
		// }

		if(strcmp("exit",arr[0])==0){
			printf("\033[1;32m");
			printf("\n*******************BYE*******************\n\n");
			printf("\033[0m");
			return 0;
		}
		// char*
		// execArgs(arr);
		check(arr);
	}
	return 0;
}
