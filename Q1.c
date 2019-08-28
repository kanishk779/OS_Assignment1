// print in reverse in another file sys call
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define BUF_SIZE (1024 * 1024 * 8)
#define true 1
#define false 0

// A function to reverse the string in-place
void strrev(char *str, size_t len) 
{
    for (size_t i = 0; i < len / 2; i++)
    {
        char c = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = c;
    }
}

// A function to convert an integer to a string
char *convertToString(int num) 
{
    int temp = num;
    char *str = (char *)malloc(20);
    int ind = 0;
    while (temp != 0) 
    {
        int digit = temp % 10;
        temp /= 10;
        str[ind] = digit + '0';
        ind++;
    }
    // str stores the the reversed integer i.e if num=156,
    // str stores 651. So reverse the string str
    strrev(str, ind);
    return str;
}
// This is used to goto the start of a line while printing the progress.
char ch = '\r';
int main(int argc, char *argv[]) 
{
    // open the first file
    int fd1 = open(argv[1], O_RDONLY, 0);
    if (fd1 == -1) {
        perror("failed to open first file\n");
        exit(1);
    }
    // create the directory Assignment
    mkdir("Assignment", 0700);
    // move to that directory
    chdir("Assignment");
    // open the second file
    int fd2 = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd2 == -1) {
        perror("failed to open second file \n");
        exit(1);
    }

    // create buffer for storing the data.
    char *buf;
    buf = (char *)malloc(BUF_SIZE);
    size_t progress = 0;

    // used for storing info related to the file described by fd1
    struct stat stat_buf;
    fstat(fd1, &stat_buf);

    size_t statsz = stat_buf.st_size;
    // filesz will be used to calculate progress
    size_t filesz = lseek(fd1, 0, SEEK_END); 
    					 
    char mesg[100];

    sprintf(mesg, "File size %lu\n", filesz);
    write(1, mesg, strlen(mesg));
    sprintf(mesg, "BUF_SIZE %d\n", BUF_SIZE);
    write(1, mesg, strlen(mesg));

    // stores the string percentage.
    char *per;

    while(1)
    {
        if(filesz >= BUF_SIZE)
        {
            lseek(fd1, -BUF_SIZE, SEEK_CUR);
            size_t byteRead = read(fd1, buf, BUF_SIZE);
        }
        else
        {
            // This the last chunk of data that is reversed
            lseek(fd1, -filesz, SEEK_CUR);
            size_t byteRead = read(fd1, buf, filesz);
            is_file_over = true;
        }
        strrev(buf, byteRead);
        size_t byteWritten = write(fd2, buf, byteRead);
        // update the progress
        progress += byteWritten;
        int percentage = (progress * 100) / statsz;
        per = convertToString(percentage);
        // move the cursor to the start of the line
        write(1, &ch, 1);
        // write the progress
        write(1, per, 4);
        filesz -= (byteWritten);
        if(is_file_over)
            break;
        // if the file is over no need of seeking to beginning of the file
        lseek(fd1, -byteWritten, SEEK_CUR);
    }

    write(1, "\n", 1);

    // some neccessary clean-up
    free(buf);
    close(fd1);
    close(fd2);
    return 0;
}

