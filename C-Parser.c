#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

void parser();
void INThandler(int);

void pipecmd(char **tokens, int position)
{
  int arg1,arg2;
  if(position==4){
  arg1 =2;
  arg2 =3;
  }

  else if(position==3){
  arg1=1;
  arg2=2;
  }
   else if(position==5){
     arg1=3;
      arg2=3;
  }

  char *args_2[arg1];
  char *args_1[arg2];
  int pipefd[2];
  int pid;
  int i =0;
  if(position==4){
    //args_2[0]=tokens[0];
    args_2[0]=tokens[0];
    args_2[1]=NULL;
  // = {"cat", "scores", NULL};
  args_1[0] = tokens[2];
  args_1[1] = tokens[3];
  args_1[2] = NULL;
  }
  else if(position==3){
    //args_2[0]=tokens[0];
    args_2[0]=tokens[0];
    args_2[1]=NULL;
  // = {"cat", "scores", NULL};
  args_1[0] = tokens[2];
  args_1[1] = NULL;
  }
   else if(position==5){
    //args_2[0]=tokens[0];
    args_2[0]=tokens[0];
    args_2[1]=tokens[1];
    args_2[2]=NULL;
  // = {"cat", "scores", NULL};
  args_1[0] = tokens[3];
  args_1[1] = tokens[4];
  args_1[2] = NULL;
  }
  // make a pipe (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd);

  pid = fork();

  if (pid == 0)
    {
     

      dup2(pipefd[0], 0);

      // close unused hald of pipe

      close(pipefd[1]);

      // execute 

      execvp(args_1[0], args_1);
    }
  else
    {

      dup2(pipefd[1], 1);

      // close unused unput half of pipe

      close(pipefd[0]);

      // execute 

      execvp(args_2[0], args_2);
    }
}


void append(char **tokens, char *file, int cmdtype){
    int in, out;
  char *args[3];
    pid_t  pid;
     int    status;
    args[0]=tokens[0];
    args[1]=tokens[1];
    args[2]=NULL;
    if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     } 
     if(cmdtype ==0  &&   pid==0){  //>> 
      // open input and output files
    in = open(file, O_RDWR|O_APPEND|O_CREAT,S_IRWXO);
      // replace standard input with input file
    dup2(in, STDOUT_FILENO);
    close(in);
    execvp(args[0], args);
    perror(args[0]);
    printf("*** Could not execute '%s'\n", args[0]);
    exit(1);  // forked child process exits non-zero
  }
  if(cmdtype ==1 &&  pid==0){	//2>>
      // open input and output files
    in = open(file, O_RDWR|O_APPEND|O_CREAT,S_IRWXO);
      // replace standard input with input file
    dup2(in, STDERR_FILENO);
    close(in);
    execvp(args[0], args);
    perror(args[0]);
    printf("*** Could not execute '%s'\n", args[0]);
    exit(1);  // forked child process exits non-zero
  }

}


void redirect(char **tokens, char *file, int cmdtype){
  int in, out;
  char *args[3];
    pid_t  pid;
     int    status;
    args[0]=tokens[0];
    args[1]=tokens[1];
    args[2]=NULL;
	
     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     } 

  if(cmdtype ==0&&pid==0){//<
      // open input and output files
    in = open(file, O_RDONLY);
      // replace standard input with input file
    dup2(in, 0); 
    close(in);
    execvp(args[0], args);
    perror(args[0]);
    printf("*** Could not execute '%s'\n", args[0]);
    exit(1);  // forked child process exits non-zero
  }
else if(cmdtype==1&&pid==0){	//>
  out = open(file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
  // replace standard output with output file
  dup2(out, 1);
  // close unused file descriptors
  close(out);
  // execute grep
  execvp(args[0], args);
  perror(args[0]);
    printf("*** Could not execute '%s'\n", args[0]);
    exit(1);  // forked child process exits non-zero
  }
else if(cmdtype==2  &&  pid==0){	//2>
  out = open(file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
  // replace standard output with output file
  dup2(out, STDERR_FILENO);
  // close unused file descriptors
  close(out);
  // execute grep
  execvp(args[0], args);
  perror(args[0]);
    printf("*** Could not execute '%s'\n", args[0]);
    exit(1);  // forked child process exits non-zero
  }
  else if(cmdtype==3  &&  pid==0){	// &>
  out = open(file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
  // replace standard output with output file
  dup2(out, STDERR_FILENO);
  dup2(out, STDOUT_FILENO);
  // close unused file descriptors
  close(out);
  // execute grep
  execvp(args[0], args);
  perror(args[0]);
    printf("*** Could not execute '%s'\n", args[0]);
    exit(1);  // forked child process exits non-zero
  }
  else {                                  /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;
     }

}


static int
forkexec( int argc, char **argv ){ // for single args no special chars
  int pid;

  switch( pid=fork() ) {
  case -1:  /* ERROR */
    perror("fork");
    return 1;
  case 0:   /* CHILD */
    execvp(argv[0],argv);
    perror(argv[0]);
    printf("*** Could not execute '%s'\n", argv[0]);
    exit(1);  // forked child process exits non-zero
  default:{ /* PARENT */
    int status;
    wait( &status );
    return (status == 0) ? 0 : 1;
      }
  }
  /*NOTREACHED*/
}

int main(){
    signal(SIGINT, INThandler);
    parser();
        return 0;
}


void  INThandler(int sig)
{
     char  c;

     signal(sig, SIG_IGN);
     printf("Did you hit Ctrl-C?\n"
            "End Parser? [y/n] ");
     c = getchar();
     if (c == 'y' || c == 'Y')
          exit(0);
     else
          signal(SIGINT, INThandler);
     getchar(); // Get new line character
}


void parser(){

    int i,j;
    char str[128];
    int tokenCheck=0;
    int position=0;
    int column=0;
    int spaceCount=0;
    int match=0;
    char key[] = {'<','|','>','&'};
    char token[128][128];
    int specialLocation;
    
    
    while (fgets(str, 50, stdin)) // go until ctrl c
    {
      specialLocation=0;
        fflush(stdout);
        spaceCount=0;//checking to see if there was a space before it
        match=0;//used to check whether to add another position if a space came after it(due to how i have positions being added) 0 for a space, 1 for a token
        column=0;
        position=0;
        tokenCheck=0;//used to check if the a space is coming after a token: 1 if its coming afte a token. 0 if coming after a special token
        for( i=0; i<strlen(str);i++){      //Checking string that was typed in
            for( j=0; j<strlen(key);j++){   //checking the key that is stored
                
                
                
                if(str[i] ==' '&& str[i]!='\n'){
                    if(i!=strlen(str)-1){//if i is not the last item in the array
                        if(spaceCount==0 && tokenCheck!=0){
                            position++;//go to a new position
                            column=0;//reset columns
                            spaceCount=1;
                            match=0;
                        }
                    }
                }
                
                
                else if(str[i]==key[j]){//checks to see if its a match
                    if(str[i-1]!=' '&& match==0){
                        if(i!=0){
                            position++;//goes to the next position of the array
                        }
                    }
                    column=0;
                    token[position][column]=str[i];
                    if(i!=(strlen(str)-2)){
                      //if(str[i+1]!=' ')
                        position++;
                    }
                    match=1;
                    tokenCheck=0;
                    break;
                }
                
                
                else if(j==(3) && str[i]!='\n'){//stores the token if the token is not one of the special characters
                    if(str[i]!=' '){
                        token[position][column]=str[i];
                        column++;//goes to the next column
                        spaceCount=0;
                        tokenCheck=1;
                        match=0;
                    }
                }
                
            }
        }
    char** tokens = (char**)malloc(position+1 * sizeof(char*));
        token[position+1][0]='\0';
         for( i =0; i<position+1; i++){
            tokens[i]=token[i];
         }
       //INSERT COMMAND FUNCTION HERE

            for(i =0; i<position+1; i++){
             for(j=0; j<strlen(key);j++){
            if(*tokens[i]==key[j]){
                printf("Special Token: %s\n",tokens[i]);
                specialLocation=i;
                break;
            }else if(j==(3)&& *token[i]!='\n'){
                printf("Token: %s\n",tokens[i]);
            }


        }
    }

if(specialLocation>=0 && *tokens[specialLocation]=='|')
{    

     int status;
  int process = fork();
  if(process == 0) {
    pipecmd(tokens, position+1);
    exit(0);
  } else if(process == -1){
    perror("forking failed\n");
    exit(1);
  } else {
    while ((process = wait(&status)) != -1) 
      ;
  }   

}
else if(specialLocation>=0 && *tokens[specialLocation]=='>')
      {
        
        if(specialLocation>=1 && *tokens[specialLocation-1] == '2') // 2>
          {
                redirect(tokens,tokens[specialLocation+1], 2);
          }
        else if(specialLocation>=1 && *tokens[specialLocation-1] == '1') // 1>
          {
              redirect(tokens,tokens[specialLocation+1], 1);
          }
        else if(specialLocation>=1 && *tokens[specialLocation-1] == '&') // &>
          {
                append(tokens,tokens[specialLocation+1], 3);
          }
        else if(specialLocation>=1 && *tokens[specialLocation-1] == '>') 
          {
         if(specialLocation>=2 && *tokens[specialLocation-2] == '2') // 2>>
            {
                    append(tokens,tokens[specialLocation+1], 1);
              }
                else // >>
              {
                  append(tokens,tokens[specialLocation+1], 0);
              }
      
          }
          else{//>

            redirect(tokens,tokens[specialLocation+1], 1);

          }
      
      }
      else if(*tokens[specialLocation]=='<'){ // <

             redirect(tokens,tokens[specialLocation+1], 0);
        }

        

              if(specialLocation==0){
                  int status;
                if( (status=forkexec(position,tokens)) != 0 ){
                      printf("*** Command %s returned %d.\n",
                    tokens[0], status);
                }
            }      
                  for(i=0; i<sizeof(str);i++){
                      str[i]='\0';
                      
                  }
                  memset(token, '\0', sizeof(token[0][0]) * 128 * 128);
                  //free(tokens);
      }

}
