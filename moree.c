#include "ucode.c"

//This function prints a line until the end of the line which should be 80, because the max dimensions
//of MTX is (24, 80)
void PrintLine(int fd)
{
    int i, nRead;
    char c, n = '\n', r = '\r';

    //Iterate 0-79 columns
    for (i = 0; i < 80; i++)
    {
        //Find how much we need to read, and get the first character
        nRead = read(fd, &c, 1);
        write(1, &c, 1); //Write the character to stdout

        //If the character we just wrote is a \n character
        if (c == '\n')
        {
            write(2, &r, 1); //Write a carriage return to stderr, it puts a new line in the terminal and avoids
                                    //staircase printing
        }
        //If the character we just wrote is a \r character
        else if (c == '\r')
        {
            write(1, &n, 1); //Write a \n character to stdout,
            write(2, &r, 1); //and a carriage return stderr so we don't increase file size and don't get
                                    //staircase printing
        }

        //If we get nRead not equal to 1
        if (nRead != 1)
        {
            exit(0); //Done reading exit
        }

        //If the character was a \n or a \r then just break the loop because that's the end of the line
        if (c == n || c == r)
        {
            break;
        }
    }
}


//This function just calls PrintLine() 24 times because there are 24 rows in a page in MTX
void PrintPage(int fd)
{
    int i;
    printk(" \n");
    
    for (i = 0; i < 24; i++)
    {
        PrintLine(fd);
    }
}


int main(int argc, char *argv[])
{
    char c, userInput, tty[64];
    int fd, read, i, j;
    char bufer[10];
    printf("Jake More\n");
    gets(bufer);
    //If we only have "more" as our argument, then we are probably in a pipe, let's handle it
  
    if (argc == 1)
    {
      // pipe in will be our stdin (0)
        fd = dup(0); //DUPLICATE our read pipe into our file decriptor
        close(0);  //close it so we can replace it with our terminal
        gettty(tty); //read tty syscall()
        open(tty, 0); //Open it for read or in 
    }
    
    else//file input, and we are probably not a pipe
    {
        fd = open(argv[1], 0); //Open the file for reading
    }

   
    if (fd < 0)
    {
       printk("BAD more: cannot bad file %s\n", argv[1]);
        exit(-1);
    }

    
    while (1)
    {
      PrintPage(fd);//
        while (1)
        {
            userInput = getc();
            switch(userInput)
            {
            case '\r': //"enter" == print 1 line
	      
                PrintLine(fd);
                break;
            case ' ': //space == print page
                PrintPage(fd);
                break;
            case 'q': //If we get a 'q' the user wants to end more
	      printk(" \n");
	       printk("more: quit\n\n");
                return 0;
                break;
            default:
                break;
            }
        }
    }
    exit(0);
}
