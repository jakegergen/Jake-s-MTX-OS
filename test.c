#include <sys/types.h>
#include <stdio.h>
#include <string.h>
char readchar(int fd){
  int t;
  char c[1];
  t = read(fd,c,1);
  if(t == 0){
    return ((char)-1);
  }
  return c[0];
}
int readaline(char buf[],int fd){
  char t = readchar(fd);
  char* tmp = buf;
  int s = 0;
  while(t!=-1){
    if(t == '\n'){
      break;
    }
      *tmp = t;
      tmp++;
      t = readchar(fd);
      s++;
  }
    *tmp = 0;
    return s;
}

//root:12345:0:0:super user:/:sh
//kcw:abcde:1:1:kcw user:/user/kcw:sh
char *login_info[7];
void tokenizelogin(char* userline){
  char *s = userline;//start
   char *e = userline;//end
  int i;
  for(i = 0;i<7;i++)
    {
      login_info[i] = s;//point to the start of the word
      while((*e != ':'))
	{
	  e++;//move end to the end of the word indicated by ':'
	}    
      *e = 0;//replace end of word with null
      e++;     //move e from the null byte we added to the first char of the next segment after ':'
      s= e;//shift s to the start of the next word
      printf("DEBUG %s \n",login_info[i]);
    }
}
  
int is_special(char *c){//  0 |   1 <   2 >   3 <<   4 >>
  if(*c == '|'){
    return 0;}
  if(*c == '<'){
    if(*(c+1) == '<'){
      return 3;}
    return 1;
  }
  if(*c == '>'){
    if(*(c+1) == '>'){
      return 4;}
    return 2;
  }

 
  return -1;
}
int parsecmdline(char *cmdline, char *cmdlist[],int spclist[]){
  int val = 0;
  char *s=cmdline;
  char *e=cmdline;
  int clindex = 0; //cmdlist index
  int  slindex = 0;//spc list index
  while(*e != 0){
    val = is_special(e); 
    if(val!=-1){
      // str
      spclist[slindex] = val;
      printf("%d\n",spclist[0]);
       
      e--;//move end pointer to the space right behind the spec char 
      *e = 0;//that space is now null 
       cmdlist[clindex] = s; 
       
      e++;//move e back to orignal position on special char 
      if(is_special(e+1) != -1){ 
     	//if the next char is special too, indicating an << or >>
     	e++;//on to second special char 
      } 
      e++;//move e from special char to the first empty space 
      e++;//move e from first smpty space to start of next cmd 
      s = e;
     
      clindex++; 
      slindex++;
      e--;//ensures e==s after the next e++;
    } 
    e++; 
  }
  //e is now at the end of the string
  cmdlist[clindex] = s;
}

void memreset(char *cmdlist[],char cmdline[124], int spclist[]){
  int i;
  for(i = 0; i<124;i++){
    if(i<32){ spclist[i] = -1; }
    if(i<16){ cmdlist[i] = 0; }
    cmdline[i] = 0;
  }
}


int main(){
  char *cmd = "cd /abcd | ls";
  char buf[124];
  strcpy(buf,cmd);
  char *cmdlist[16];
  int spclist[32];

  parsecmdline(buf,cmdlist,spclist);
  
  printf("%s",cmdlist[0]);
  memreset(cmdlist,buf,spclist);
   printf("%s",cmdlist[0]);
}


