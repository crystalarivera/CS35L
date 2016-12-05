/*
  Notes on use:
  compile with gcc -g sfrobu.c -o sfrobu -std=c99
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

int frobcmp(char const *a, char const *b);
void errorReport(const char* msg);
int frobcmpptrs(const void* a, const void* b);

int comparisons = 0;

int main ()
{
  /* setup to check file type */
  struct stat fileStat;
  if(fstat(STDIN_FILENO,&fileStat) < 0) //Error with fstat
    errorReport("Error with fstat");
  int isReg = 0;

  /* setup to read file */ 
  //input file is a line of words 
  //storage as we read the input from stdin
  char* word = (char*)malloc(sizeof(char));
  char** words = (char**)malloc(sizeof(char*));
  if (word == NULL) errorReport("Error with memory allocation command, malloc.");
  if (words == NULL) errorReport("Error with memory allocation command, malloc.");
  char* input;
  int lastWord = 0;

  //keeps track of stats from stdin
  int letterCount = 0;
  int wordCount = 0;
  int inputCount = 0;
  int fileSize = 0;

  /* check file type and allocate initial memory if it is a regular file */
  if(S_ISREG(fileStat.st_mode))
    { 
      isReg = 1;
      fileSize = fileStat.st_size + 1;
      input = (char*)malloc((fileSize) * sizeof(char));
      if (input == NULL) errorReport("Error with memory allocation command, malloc.");
      if (read(STDIN_FILENO, input, fileStat.st_size) <= 0)
	{
	  free(input);
	  errorReport("Error reading from file.");
	}
      input[fileSize - 1] = ' '; //Always set the end to a space
    }

  //used to read stdin 
  char curr;
  int curry = 0;
  if(!isReg)
    {
      curry = read(STDIN_FILENO, &curr, 1); 
      if (curry <= 0) 
	errorReport("Error reading file");
    }
  else 
    {
      if (inputCount != fileSize)
	{
	  curr = input[inputCount];
	  inputCount++;
	}
    }
  char next;
  int nexty = 0;
  if(!isReg)
    {
      nexty = read(STDIN_FILENO, &next, 1); 
      if (nexty <= 0) 
	errorReport("Error reading file");
    }
  else 
    {
      if (inputCount != fileSize)
	{
	  next = input[inputCount];
	  inputCount++;
	}
    }

  //growing word list accommodated by dynamic memory
  //memory is reallocated each time something is stored
  while(curry > 0 || !(fileSize == inputCount && curr == ' ' && lastWord != 1)) //(!isReg || isReg)
    {
      //store char
      word[letterCount] = curr;
      char* tempword = realloc(word, (letterCount+2)*sizeof(char));
      if (tempword == NULL) 
	{
	  for(size_t i = 0; i < wordCount; i++)
	    free(words[i]);
	  free(words);
	  errorReport ("Memory allocation error");
	}
      word = tempword;

      //at the end of the word, store it
      if(curr == ' ')
	{
	  words[wordCount] = word; //add complete word to words list
	  char** tempwords = realloc(words, (wordCount+2)*sizeof(char*));
	  if (tempwords == NULL)
	    {
	      for(size_t i = 0; i < wordCount; i++)
		free(words[i]);
	      free(words);
	      errorReport("Memory allocation error");
	    }
	  words = tempwords;
	  wordCount++;

	  //reset word stats
	  word = NULL;
	  word = (char*)malloc(sizeof(char));
	  letterCount = -1; //-1 since letterCount is incremented later
	  lastWord = 0;
	}

      //ignore extra spaces
      if (curr == ' ' && next == ' ' && inputCount != fileSize)
	{
	  while (curr == ' ')
	    {
	      if (!isReg)
		{
		  curry = read(STDIN_FILENO, &curr, 1);
		  if (curry <= 0)
		    {
		      for (size_t i = 0; i < wordCount; i++)
			free(words[i]);
		      free(words);
		      errorReport("Error reading file");
		    }
		}
	      else
		{
		  if (inputCount != fileSize)
		    {
		      curr = input[inputCount];
		      inputCount++;
		    }
		}
	    }
	  if (!isReg)
	    {
	      nexty = read(STDIN_FILENO, &next, 1);
	      if (nexty <= 0)
		{
		  for (size_t i = 0; i < wordCount; i++)
		    free(words[i]);
		  free(words);
		  errorReport("Error reading file");
		}
	    }
	  else
	    {
	      if (inputCount != fileSize)
		{
		  next = input[inputCount];
		  inputCount++;
		}
	    }
	}
      else //or increment just get the next character
	{
	  //does not read last word if there are not multiple traiting spaces
	  //condition added to fix this problem
	  if (curr != ' ' && next == ' ' && inputCount == fileSize)
	    lastWord = 1;
	  curr = next;
	  if (!isReg)
	    {
	      curry = nexty;
	      nexty = read(STDIN_FILENO, &next, 1);
	      if (nexty <= 0)
		{
		  for (size_t i = 0; i < wordCount; i++)
		    free(words[i]);
		  free(words);
		  errorReport("Error reading file");
		}
	    }
	  else
	    {
	      if (inputCount != fileSize)
		{
		  next = input[inputCount];
		  inputCount++;
		}
	    }

	}
      letterCount++;
    }

  /* sort frobnicated words */
  qsort (words, wordCount, sizeof(char*), frobcmpptrs);

  /* print sorted frob words to stdout */
  for(size_t i = 0; i < wordCount; i++)
    {
      for(size_t j = 0; ;j++)
	{
	  int writeCheck = write(STDOUT_FILENO, &(words[i][j]), 1);
	  //EOF error checking
	  if (writeCheck <= 0) 
	    {
	      for(size_t i = 0; i < wordCount; i++)
		free(words[i]);
	      free(words);
	      errorReport ("Error writing to stdout");
	    }
	  if(words[i][j] == ' ')
	    break;
	}	
    }	

  fprintf(stderr, "Comparisons: %d\n", comparisons);

  /* free dynamic memory */
  for(size_t i = 0; i < wordCount; i++)
    free(words[i]);
  free(words);
  if(isReg) free(input);

  return 0;
}

/* Compares two unfrobnicated words */
/* Returns negative, zero, positive if A < B, A == B, A > B respectively */
int frobcmp(char const *a, char const *b)
{
  /* all frobnicating does is XOR with 00101010 (aka 42) */
  for ( ; ; a++, b++)    
    {
      if(*a == ' ' && *b == ' ') { return 0; }
      else if (*a == ' ' || ((*a^42) < (*b^42))) { return -1; }
      else if (*b == ' ' || ((*a^42) > (*b^42))) { return 1; }
    }
  return 0;
}

/* Report error */
// acts as a boolean by returning true or false
void errorReport(const char* msg)
{
  fprintf(stderr, "%s Error: %d\n", msg, errno);
  exit(1);
}

//allows us to use frobcmp, but with pointers to pointers
int frobcmpptrs(const void* a, const void* b)
{
  const char* c = *(const char**)a;
  const char* d = *(const char**)b;
  comparisons++;
  return frobcmp(c, d);
}
