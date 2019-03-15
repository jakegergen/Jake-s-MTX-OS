/****************** Algorithm of login *******************/
// login.c : Upon entry, argv[0]=login, argv[1]=/dev/ttyX
#include "ucode.c"
int in, out, err;

char *login_info[7];
// login_info[0] username
// login_info[1] password
// login_info[2] gid
// login_info[3] uid
// login_info[4] fullname
//login_info [5] HOMEDIR
// login_info[6] program

int makeint(char c){
  return ((int)c - 48);
}
char readchar(int fd){
  int t;
  char c[1];
  t = read(fd,c,1);
  if(t == 0){
    return (0);
  }
  return c[0];
}
int readaline(char buf[],int fd){
  char t = readchar(fd);
  char* tmp = buf;
  int s = 0;
  while(t!=-1){
    if(!t){return 0;}
    if(t == '\n'){
      break;
    }
      *tmp = t;
      tmp++;
      t = readchar(fd);
      s++;
      
  }
   
    *tmp = 0;
    return s;//return number of chars read
}

//Tokenizelogin: Populates login_info with user account information
// login_info[0] username
// login_info[1] password
// login_info[2] gid
// login_info[3] uid
// login_info[4] fullname
//login_info [5] HOMEDIR
// login_info[6] program
//root:12345:0:0:super user:/:sh
//kcw:abcde:1:1:kcw user:/user/kcw:sh

void tokenizelogin(char* userline){
  char *s = userline;//start
   char *e = userline;//end
  int i;
  for(i = 0;i<7;i++)
    {
      login_info[i] = s;//point to the start of the word
      while((*e != ':' && (*e!=0)))
	{
	  e++;//move end to the end of the word indicated by ':'
	}
      
      *e = 0;//replace end of word with null
      e++;     //move e from the null byte we added to the first char of the next segment after ':'
      s= e;//shift s to the start of the next word
      
    }
}
   
  



main(int argc, char *argv[])
{
  char uname[128],password[128];
  int fd;
  char line[64];
  int uid;
  int gid;
  int i;
  close(in);
  close(out);
  in = open(argv[1],O_RDONLY);
  out = open(argv[1],O_WRONLY);
  err = open(argv[1],O_WRONLY);
  settty(argv[1]);
  fd = open("/etc/passwd",O_RDONLY);
  while(1){
    printf("\nEnter login:");
    gets(uname);
    printf("Password:"); gets(password);
    while(readaline(line,fd) != 0){//for each account we have saved in file
       tokenizelogin(line);
       if(strcmp(login_info[0],uname) == 0){
	 if(strcmp(login_info[1],password) == 0){
	   // change uid, gid to user's uid, gid; // chuid()
	   //(8).
	   //change cwd to user's home DIR // chdir()
	   //close opened /etc/passwd file // close()
	   //exec to program in user account // exec()
	 
	   //valid account
	   printf("Account Foud\nPC name: %s\n",login_info[4]);
	   gid = makeint(login_info[2]);
	   uid = makeint(login_info[3]);
	   printf("DEBUG: gid: %d uid: %d\n",gid,uid);
	   chdir(login_info[5]);
	   exec(login_info[6]);
	 }
       }
    }
    close(fd);
    fd = open("/etc/passwd",O_RDONLY);

    printf("No Accounts found try again.");
  }

}


