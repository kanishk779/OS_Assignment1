// print in reverse in another file sys call
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define BUF_SIZE (1024 * 1024 * 8)


void strrev(char *str, size_t len) {
  for (size_t i = 0; i < len / 2; i++) {
    char c = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = c;
  }
}
char *convertToString(int num) {
  int temp = num;
  char *str = (char *)malloc(20);
  int ind = 0;
  while (temp != 0) {
    int digit = temp % 10;
    temp /= 10;
    str[ind] = digit + '0';
    ind++;
  }
  strrev(str, ind);
  return str;
}
char ch = '\r';
int main(int argc, char *argv[]) {

  int fd1 = open(argv[1], O_RDONLY, 0);
  if (fd1 == -1) {
    perror("failed to open first file\n");
    exit(1);
  }
  mkdir("Assignment", 0700);
  chdir("Assignment");
  int fd2 = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if (fd2 == -1) {
    perror("failed to open second file \n");
    exit(1);
  }

  char *buf;
  buf = (char *)malloc(BUF_SIZE);
  size_t progress = 0;
  struct stat stat_buf;
  fstat(fd1, &stat_buf);

  size_t statsz = stat_buf.st_size;
  size_t filesz = lseek(fd1, 0, SEEK_END); // filesz will be used to calculate
                                           // the progress of copying the data
					 
  char mesg[100];

  sprintf(mesg, "File size %lu\n", filesz);
  write(1, mesg, strlen(mesg));
  sprintf(mesg, "BUF_SIZE %d\n", BUF_SIZE);
  write(1, mesg, strlen(mesg));

  char *per;

  while (filesz >= BUF_SIZE) {
    lseek(fd1, -BUF_SIZE, SEEK_CUR);
    size_t byteRead = read(fd1, buf, BUF_SIZE);
    strrev(buf, byteRead);
    size_t byteWritten = write(fd2, buf, byteRead);
    progress += byteWritten;
    int percentage = (progress * 100) / statsz;
    per = convertToString(percentage);
    write(1, &ch, 1);
    write(1, per, 5);
    filesz -= (byteWritten);
    lseek(fd1, -byteWritten, SEEK_CUR);
  }
  // After coming out of the loop we know that the file size is smaller than the
  // buffer size
  lseek(fd1, -filesz, SEEK_CUR);
  size_t byteRead = read(fd1, buf, filesz);
  strrev(buf, byteRead);
  size_t byteWritten = write(fd2, buf, byteRead);
  progress += byteWritten;
  int percentage = (progress * 100) / statsz;
  per = convertToString(percentage);
  write(1, &ch, 1);
  write(1, per, 5);
  filesz -= byteWritten;

  write(1, "\n", 1);
  free(buf);
  close(fd1);
  close(fd2);
  return 0;
}

