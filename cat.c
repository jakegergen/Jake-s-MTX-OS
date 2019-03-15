#include "ucode.c"


int main (int argc, char *argv[]) // cat f
{
    char buf[1024];
    int readSize = 1024;
    char ttybuf[64];
    char c;
    STAT file;
    int fd, i, j;
    int size;
    char inputLine[1024];
    int ttyfd;
    char bufer[64];
    char n = '\n';
    char r = '\r';
   
  printf("Jake Cat in actions\n\r");
  gets(bufer);
   
    
    if(argc == 1) //If we only have one argument we get stuff from stdin
    {
        fd = 0;
        gettty(ttybuf);//read into buf
        //open(tty, 1);
       open(ttybuf, 0); //open buf for write
        while(read(fd, &c, 1) > 0) //read file 1 byte at a time
        {
           
    	  write(1, &c, 1);//write char to fd[1]: out

            //If the character is a new line
            if(c == '\n')
            {
                
                //write(ttyfd, &n, 1);
		write(2, &r, 1);
            }
            //If the character is a carriage return
            else if(c == '\r')
            {
                //write a new line
	       write(ttyfd, &n, 1);
                //Write a carriage return so we dont increase file size
	       write(ttyfd, &r, 1);
	       // write(1, &n, 1);
	       //  write(2, &r, 1);
            }
        }
    }
    
    else if(argc >= 2) //Case 2, files passed in as input files
    {
        for(i = 1; i < argc; i++)//For every file given by user
        {
            fd = open(argv[i], 0); //Open file i for read

            if(fd == -1) // bad file name given
            {
    	      printf("cat: Bad file given.\n"); //error out
                exit(-1);
            }

            //printf("DEBUG: FD = %d\n", fd);

            stat(argv[i], &file);
    	    //save into stat struct file
	      
            size = file.st_size; //Get the size of the file in Bytes

            while(size > 0) //while unread bytes
            {
                //if the file size is smaller than our current read size, adjust so dont over read
                if(size < 1024)
                {
    		  //this ensure we never read too far, into no where land
                    readSize = size;
                }

               
                read(fd, buf, readSize); //read file

                //We need to loop through our buffer to print it out character by character
                for( j = 0; j < readSize; j++)
                {
                    c = buf[j];

                    //First write the character
                    //write(ttyfd, &c, 1);
		    write(1, &c, 1);

                    //If the character is a new line
                    if(c == '\n')
                    {
                        //Write a carriage return
                        //write(ttyfd, &r, 1);
			write(2, &r, 1);
                    }
                    //If the character is a carriage return
                    else if(c == '\r')
                    {
                        //write a new line
                        write(1, &n, 1);
                        //Write a carriage return
                        write(2, &r, 1);
                    }
                }

                //adjust size to compensate for how many bytes were read
                size -= readSize;

            }

            close(fd);
        }

        exit(0);

    }
    else //If we get here then the user probably made a mistake
    {
        printf("usage: cat <filename1> <filename2> ... etc\n");
        exit(-1);
    }
}
