#include <stdio.h>
#include <string.h>

int main()
{
   FILE *fp;
   int totalBytes;
   char line[80];
   char ch;
   int i = 0;

   fp = fopen("input.txt", "r+");
   if (!fp) {
      printf("unable to open input file");
   }

   fseek(fp, 0, SEEK_END);
   memset(line, 0, 80);
   totalBytes = ftell(fp);

   ch = fgetc(fp);
   while(totalBytes > 0) {
      while (ch == EOF || ch == '\n') {
         fseek(fp, -2, SEEK_CUR);
         totalBytes--;
         ch = fgetc(fp);
      }

      while (ch != '\n' && totalBytes > 0) {
         line[i++] = ch;
         fseek(fp, -2, SEEK_CUR);
         totalBytes--;
         ch = fgetc(fp);
      }

      if (*line) {
         printf("%s\n", line);
         memset(line, 0, 80);
         i = 0;
      }
   }
   fclose(fp);
   return 0;
}
