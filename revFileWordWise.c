#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define STEPSIZE 2
#define LINEWIDTH 80
#define MAX_FILENAME_SIZE 10

/*********************************************
 * reverse
 * - Given a word, this function will reverse
 *   the word inplace.
 *
 * input:- start & end ptrs of the word
 *********************************************/
void reverse(char *wordBegin, char *wordEnd)
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

/*****************************************************
 * reverseLine
 * - This function achieves word wise reversal.
 *   Given a line, this function will form a word
 *   and will reverse the word within the line string.
 *   After reversal of each word, it will reverse the
 *   complete line.
 *
 * input:- char* pointing to the line string
 *****************************************************/
void reverseLine(char *str) {
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

/*************************************************************
 * writeToScratchFile
 * - This function forms the intermediate scratch files.
 *   Lets say the input file had 100 lines and our stepsize
 *   is 10 lines. So we will reverse 10 lines in a word-wise
 *   manner and send the array of pointers to this function.
 *   Given the lines, this function will print the lines from
 *   the last. The resultant scratch file will be merged to
 *   form a big output file, containing the output in required
 *   format.
 *
 * input:- collection of lines, scratchfileNum, number of lines
 *         in the collection
 **************************************************************/
void writeToScratchFile(char *lines[STEPSIZE],
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


/*************************************************************
 * combineScratchToOutput
 * - This function merges the scratch files formed in descending
 *   order. This means the file containing the last chunk, will
 *   be merged first.
 *
 * input:- number of scratch files to merge.
 **************************************************************/
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


/*******************************************************************
 * main
 * - This function follows the following logic.
 *   1. Given a file with N lines.
 *   2. Say,We cannot read N lines at once, but can read 10 lines.
 *   3. So we will prepare scratch (N/10)+1 files with 10 lines each.
 *   4. In the end we will merge the 10 scratch files.
 *
 *   Scratch files:-
 *   1. Lets say we have total 100 lines.
 *   2. So we will prepare 10 scratch files with 10 lines each.
 *   3. Here each file will contain lines in:-
 *      3.1 descending order, i.e. line 10th will come before 9th,
 *          9th before 8th and so on.
 *      3.2 Also each line will be reversed word wise.
 *   4. Intermediate output will be like 0.txt, 1.txt....9.txt  
 *  
 *   Combining to form the output:-
 *   1. Once we have files from 0.txt to 9.txt containing lines
 *      reversed in a word-wise manner, we start merging with last
 *      file onwards.
 *   2. So contents of 9.txt will come first, followed by 8.txt
 *      and so on.
 *   3. output.txt is our required file.
 *   
 * input:- None
 **************************************************************/
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


