#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define BUF_SIZE (1024 * 1024)
/*
1. First argument is the newfile name.
2. Second argument is the oldfile name.
3. Third argument is the directory created in the previous C program.
*/

int writeThis(char *str) {
  int len = strlen(str);
  int written = write(1, str, len);
  return written;
}

void printFilePermission(int fileMode) {
  writeThis("-----------\n");
  /* Check owner permissions */
  if ((fileMode & S_IRUSR) && (fileMode & S_IREAD))
    writeThis("owner has Read permissions \n");
  else
    writeThis("owner does not have Read permissions\n");
  if ((fileMode & S_IWUSR) && (fileMode & S_IWRITE))
    writeThis("owner has Write permissions\n");
  else
    writeThis("owner does not have Write permissions \n");
  if ((fileMode & S_IXUSR) && (fileMode & S_IEXEC))
    writeThis("owner has execute permissions\n");
  else
    writeThis("owner does not have execute permission\n");
  /* Check group permissions */
  if ((fileMode & S_IRGRP) && (fileMode & S_IREAD))
    writeThis("group has Read permissions\n");
  else
    writeThis("group does not have Read permissions\n");
  if ((fileMode & S_IWGRP) && (fileMode & S_IWRITE))
    writeThis("group has Write permissions \n");
  else
    writeThis("group does not have write permissions\n");
  if ((fileMode & S_IXGRP) && (fileMode & S_IEXEC))
    writeThis("group has execute permission\n");
  else
    writeThis("group does not have execute permissions\n");
  /* check other user permissions */
  if ((fileMode & S_IROTH) && (fileMode & S_IREAD))
    writeThis("other has Read permissions\n");
  else
    writeThis("other does not have Read permissions\n");
  if ((fileMode & S_IWOTH) && (fileMode & S_IWRITE))
    writeThis("other has write permissions \n");
  else
    writeThis("other does not have write permissions\n");
  if ((fileMode & S_IXOTH) && (fileMode & S_IEXEC))
    writeThis("other has execute permissions\n");
  else
    writeThis("other does not have execute permissions\n");
}

bool checkIfEqual(char *buf_fd1, char *buf_fd2, size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (buf_fd1[i] != buf_fd2[len - i - 1])
      return false;
  }
  return true;
}
int main(int argc, char *argv[]) {
  char *info = "We are currently in the old directory\n";
  writeThis(info);

  int fd1 = open(argv[2], O_RDONLY, 0);
  if (fd1 == -1) {
    perror("failed to open first file\n");
    exit(1);
  }
  int fdDir = open(argv[3], O_RDONLY, 0);
  struct stat stat_old_file;
  fstat(fd1, &stat_old_file);
  int oldFile_mode = stat_old_file.st_mode;
  writeThis("The file permissions for the old file is:-\n");
  printFilePermission(oldFile_mode);
  struct stat stat_dir;
  fstat(fdDir, &stat_dir);
  int dirMode = stat_dir.st_mode;
  char info2[] =
      "The file permissions for the directory given in the arguments are :-\n";
  write(1, info2, strlen(info2));
  printFilePermission(dirMode);
  write(1, "\n", 1);
  char info3[] = "Now we have changed the directory to get access to the "
                 "reversed file, it's permissions are :-\n";
  write(1, info3, strlen(info3));
  chdir(argv[3]);
  int fd2 = open(argv[1], O_RDONLY, 0);
  if (fd2 == -1) {
    perror("failed to open the second file");
    exit(1);
  }
  struct stat stat_buf;
  fstat(fd2, &stat_buf);
  int filemode = stat_buf.st_mode;
  printFilePermission(filemode);
  write(1, "\n", 1);
  /*--------------------------------------------------------------------*/
  char info4[] = "Comparing the files character to check if the are reverse of "
                 "each other\n";
  write(1, info4, strlen(info4));

  char info5[] = "They are reverse of each other \n";
  char info6[] = "They are not reverse of each other \n";
  bool Equal = true;
  char *buf_fd1 = (char *)malloc(BUF_SIZE);
  char *buf_fd2 = (char *)malloc(BUF_SIZE);
  size_t bytesRead;
  bool loop = true;

  lseek(fd2, 0, SEEK_END);
  while (loop) {
    bytesRead = read(fd1, buf_fd1, BUF_SIZE);
    if (lseek(fd2, -BUF_SIZE, SEEK_CUR) >= 0) {
      read(fd2, buf_fd2,bytesRead);
      lseek(fd2, -bytesRead, SEEK_CUR);
      if (!checkIfEqual(buf_fd1, buf_fd2, bytesRead)) {
        Equal = false;
        break;
      }
    } else {
      lseek(fd2, 0, SEEK_SET);
      read(fd2, buf_fd2, bytesRead);
      lseek(fd2, 0, SEEK_SET);
      if (!checkIfEqual(buf_fd1, buf_fd2, bytesRead)) {
        Equal = false;
        break;
      }
      loop = false;
    }
  }

  if (Equal)
    write(1, info5, strlen(info5));
  else
    write(1, info6, strlen(info6));
  return 0;
}

