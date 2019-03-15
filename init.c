#include "ucode.c"
int console;
int t1;
int t2;
int parent()
// P1's code
{
  int pid, status;
  while(1){
    printf("Jake INIT : wait for ZOMBIE child\n");
    pid = wait(&status);
    if (pid==console){
      // if console login process died
      printf("INIT: forks a new console login\n");
      console = fork(); // fork another one
      if (console)
	continue;
      else
	exec("login /dev/tty0"); // new console login process
    }
    printf("INIT: I just buried an orphan child proc %d\n", pid);
  }
}
int main(){
  int in, out;
  in = open("/dev/tty0", O_RDONLY); // file descriptor 0
  out = open("/dev/tty0", O_WRONLY); // for display to console
  printf("INIT : fork a login proc on console\n");
  console = fork();
  
  if (console){ // parent
    printf("Jake init\n");
    t1 = fork();
    if(t1){
      printf("jake init on t1\n");
      t2 = fork();
      if(t2){

	parent();
      }
      else{
	exec("login /dev/ttyS1");
      }
    }
    else{
      exec("login /dev/ttyS0");
    }
  }
  else{
    exec("login /dev/tty0");
  }
  
   
}
