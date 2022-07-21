#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define STEPSIZE 2
#define LINEWIDTH 80
#define MAX_FILENAME_SIZE 10

int reverse(char *wordBegin, char *wordEnd)
{
   char temp;
   while(wordBegin < wordEnd) {
      temp = *wordBegin;
      *wordBegin = *wordEnd;
      *wordEnd = temp;
      wordBegin++;
      wordEnd--;
   }
   return;
}

int reverseLine(char *str) {
   char *charIterator = str;
   char *wordStart = str;

   while(*charIterator) {
      charIterator++;
      if (*charIterator == ' ' || *charIterator == '\0') {
         reverse(wordStart, charIterator-1);
         if (*charIterator == ' ') {
            wordStart = charIterator + 1;
         }
      }
   }
   reverse(str, charIterator - 1);
}

int writeToScratchFile(char *lines[STEPSIZE],
                       char *scratchFileName,
                       int numLines)
{
   FILE *scratchfp;
   scratchfp = fopen(scratchFileName, "w");
   if (!scratchfp) {
      printf("unable to open scratch file");
      return;
   }

   while(numLines--) {
      fprintf(scratchfp, lines[numLines]);
      memset(lines[numLines], 0, sizeof(char) * LINEWIDTH);
   }
   fclose(scratchfp);
}


int combineScratchToOutput(int numScratchFiles)
{
   FILE *outfp;
   FILE *scratchfp;
   char scratchFileName[MAX_FILENAME_SIZE];
   char line[LINEWIDTH];

   outfp = fopen("output.txt", "w");
   if (!outfp) {
      printf("unable to open output file");
   }
   while(numScratchFiles--) {
      memset(scratchFileName, 0, sizeof(char) * MAX_FILENAME_SIZE);
      sprintf(scratchFileName, "%d.txt", numScratchFiles);
      scratchfp = fopen(scratchFileName, "r");
      if (!scratchfp) {
         printf("unable to open scratch file");
         exit(0);
      }
      while (fgets(line, LINEWIDTH, scratchfp)) {
         fprintf(outfp, line);
         memset(line, 0, sizeof(char) * LINEWIDTH);
      }
      fclose(scratchfp);
   }
   fclose(outfp);
}



int main()
{
   FILE *inpfp, *scratchfp;
   char *lines[STEPSIZE];
   char *line;
   int i, len;
   int numLines = 0;
   int iterations = 0;
   char outputFileName[MAX_FILENAME_SIZE];

   line = (char *)malloc(sizeof(char) * LINEWIDTH);
   if (!line) {
      printf("memory allocation failure");
      return;
   }
   memset(line, 0, sizeof(char) * LINEWIDTH);

   for (i = 0;i < STEPSIZE; i++) {
      lines[i] = (char *)malloc(sizeof(char) * LINEWIDTH);
      if (!lines[i]) {
         printf("memory allocation failure");
         return;
      }
      memset(lines[i], 0, sizeof(char) * LINEWIDTH);
   }

   inpfp = fopen("input.txt", "r");
   if (!inpfp) {
      printf("unable to open input file");
      return;
   }
  
   while (fgets(line, LINEWIDTH, inpfp)) {
      len = strlen(line);
      line[len-1] = '\0';
      reverseLine(line);
      line[len-1] = '\n';
      line[len] = '\0';
      memset(lines[numLines], 0, sizeof(char) * LINEWIDTH);
      strncpy(lines[numLines++], line, len);
      memset(line, 0, sizeof(char) * LINEWIDTH);

      if (numLines == STEPSIZE) {
          memset(outputFileName, 0, sizeof(char) * MAX_FILENAME_SIZE);
          sprintf(outputFileName, "%d.txt", iterations++);
          writeToScratchFile(lines, outputFileName, numLines);
          numLines = 0;
      }
   }

   if (numLines > 0) {
      memset(outputFileName, 0, sizeof(char) * MAX_FILENAME_SIZE);
      sprintf(outputFileName, "%d.txt", iterations++);
      writeToScratchFile(lines, outputFileName, numLines);
   }

   fclose(inpfp);

   if (iterations > 0) {
      combineScratchToOutput(iterations);
   }

   // no memory leaks!!
   free(line);
   for(i = 0;i < STEPSIZE; i++) {
      free(lines[i]);
   }
   return;
}


