#include "ucode.c"
int dbg = 1;

int is_special(char *c){//  0 |   1 <   2 >   3 <<   4 >>

  if(*c == '<'){
    return 1;
  }
  if(*c == '>'){
    if(*(c+1) == '>'){
      return 3;}
    return 2;
  }
  return -1;
}




int do_command(char *cmdline)
{
  char *cp = cmdline;
  int val = is_special(cp);
  int fd;
  int pid;
  char buf[3];
  
  while(val == -1){
    cp++;
    if(!*(cp)){//no io redirects
      printf("No i/o-%s-",cmdline);
  	 exec(cmdline);
  	 exit(-1);
      }
    val = is_special(cp);
  }
  
  cp--;//cp is now at space right before redirect symbol
  *cp = 0;//add null char so we can exec(cmdline)
  while(*cp != ' '){ cp++; }
  //move cp from null terminated cmd, to the first space after redirect symbol which is one before file name
  
  *cp++;//cp is now on first char of file name
 

  switch(val){
    
  case 1: //<
    close(0);//close std in fd
     fd = open(cp,O_RDONLY);
          if(fd < 0){
       printf("%s not found\n\r",cp);
       exit(-1);
     }
    
   
	  //dup2(fd,0);//replace stdin with the fd of the in file, if its nto already there
    break;
    
  case 2://>
    printf("FILE NAME..:%s",cp);
     gets(buf);
     close(1);
     fd = open(cp, O_WRONLY | O_CREAT);
     if(fd < 0){
       printf("%s not found",cp);
       exit(-1);
     }
    
     // dup2(fd,0);//move fd into stdout slot, if its not already there
    break;
    
  case 3://>>
     close(1);
     fd = open(cp, O_WRONLY|O_CREAT|O_APPEND);
   
    if(fd < 0){
       printf("%s not found",cp);
       exit(-1);
     }
    
    //dup2(fd,1);
    break;
  }
  exec(cmdline);
  
}

do_pipe(char *cmdline, int *pd)
{
 
  int haspipe;
  int lpd[2];
  char *head;
  char *tail;
  char buf[3];

  int pid;
  int status;

  
  head = cmdline;
  tail = cmdline;
  haspipe = scan(head,&tail);

  
  //printf("head:%s tail:%s",head,tail);
  
  if(pd){//
    close(pd[0]);
    dup2(pd[1],1);
    //close(pd[1]); //Books says I want this but why would I do this?
  }
 

  if(haspipe)
    {  
      pipe(lpd);
      pid = fork();
      
      if(pid){
	close(lpd[1]);//close write end
	dup2(lpd[0],0);//replace my read end with pipe read end
	//close(lpd[0]); //Books says I want this but why would I do this?
	//parent is reader of pipe
        do_command(tail);
      }
      else{
	//pass pipe to my child to write into
        do_pipe(head,lpd);
      }
    }
  else{
    do_command(head);
      }
  
}



int scan( char* h, char** t)
{
  char *head = h;
  char *tail = (char *)* t;
  while(*tail != 0){
    tail++;
  }

  while(*tail != '|'){
    if(tail == head){
      return 0;
    }
    tail--;
  }
 *(tail-1) = 0;//add null before |
  tail++;
 *(tail-1) = 0;//add null on |
  *tail= 0;//add null char after '|'
  tail++;
  (*t) = tail;
  
  return 1;

}

char* getpath (char *cmdline){
  char *cp = cmdline;
  while(*cp != ' ')
    {
      cp++;
    }
  cp++;
  return cp;
}

int main(){
  char cwd[128];
  int status;
  char cmdline[124]; //supplied by the user e.x: cd a/b/c
  char buf[124];
  char *path;
  char stall[64];
  char *newpath[256]; //buffer for relative and nonrelative pathname
   int pid, pd[2];
   int fd;

  while(1){
    
    getcwd(buf);
    strcpy(cwd,buf);
    printf("Jake Sh:");
    gets(cmdline);
    strcpy(buf,cmdline);
    //FirstCmd(buf,cmd);//cmd will be a null terminated string of first cmd
    /* //Trivial */
    
    if(!strncmp(cmdline,"cd",2)) // cd /a cd a
      {
    	path = getpath(cmdline);
	if(dbg)
	  printf("DEBUG path: %s\n any key to continue",path);
	gets(&stall);
      }
    	if(path[0] != '/'){
    	  strcpy(newpath,cwd);//from cwd
    	  strcat(newpath,path);//add the relative path name
    	  printf("newpath: %s\n",newpath);
	  chdir(newpath);
    	}
    	else{
    	  strcpy(newpath,path); //nonrelative pathname
    	chdir(newpath);//change cwd
      }
    if(!strcmp(cmdline,"logout"))
    	exit(0);
    
    pid = fork();
    if(pid){
      wait(&status);
      continue;
    } 
    else{
      do_pipe(buf,0);
    }
    
    }
    
}


