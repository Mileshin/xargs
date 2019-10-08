#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <err.h>

int read_line(char** line, const int fd) {
   char ch;
   ssize_t rb;
   char* tmp = malloc(sizeof(char));
   char* buf = malloc(sizeof(char));
   int result = EOF;
   
   tmp[0] = 0, buf[0] = 0;
   
   while ((rb = read(fd, &ch, 1)) && ch != '\n') {
      /*tmp = malloc((strlen(buf) + 1) * sizeof(char));
      strcpy(tmp, buf);
      tmp[strlen(buf)] = 0;
      if (strlen(buf) > 1) {
         free(buf);
      }
      buf = malloc((strlen(buf) + 1) * sizeof(char));
      strcpy(buf, tmp);
      buf[strlen(tmp)] = ch, buf[strlen(tmp) + 1] = 0;
      free(tmp);*/
      buf = realloc(buf, (strlen(buf) + 1) * sizeof(char));
      buf[strlen(buf)] = ch, buf[strlen(buf) + 1] = 0;

   }
   
   if (ch == '\n') {
      result = !EOF;
   }
   
   if (rb < 0) {
      return -1;
   }

   *line = buf;
   return result;
}


int main(int argc, char* argv[]) {
   char* cmd = NULL;
   char* buf = NULL;
   int code = !EOF;
   
   if (argc > 1) {
      cmd = argv[1];
   }
   
   
   while ((code = read_line(&buf, STDIN_FILENO)) != EOF) {
       if (code < 0) {
          err(1, "%s", strerror(errno));
       }
   
       if (cmd != NULL) {
         char* buf2 = malloc((strlen(cmd) 
         + strlen(buf) + 1) * sizeof(char));
   
         sprintf(buf2, "%s %s", cmd, buf);
         if (system(buf2) < 0) {
            err(1, "%s", strerror(errno));
         }
       } else if (write(STDOUT_FILENO, buf, strlen(buf)) < 0) {
                 err(1, "%s", strerror(errno));
              }
   
   }
   if (cmd == NULL) {
      if (write(STDOUT_FILENO, "\n", 1) < 0) {
         err(1, "%s", strerror(errno));
      }
   }
   return EXIT_SUCCESS;
}
