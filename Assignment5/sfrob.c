/*
Notes on use:
compile with gcc -g sfrob.c -o sfrob -std=c99
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int frobcmp(char const *a, char const *b);
void errorReport(const char* msg);
int frobcmpptrs(const void* a, const void* b);

int main ()
{
  /* read file */
 
  //input file is a line of words 
  //storage as we read the input from stdin
  char* word = (char*)malloc(sizeof(char));
  char** words = (char**)malloc(sizeof(char*));

  //keeps track of stats from stdin
  int letterCount = 0;
  int wordCount = 0;

  //used to read stdin 
  char curr = getchar(); 
  if (ferror(stdin)) 
    {
      for(size_t i = 0; i < wordCount; i++)
	free(words[i]);
      free(words);
      errorReport("Error reading file");
    }
  char next = getchar();
  if (ferror(stdin)) 
    {
      for(size_t i = 0; i < wordCount; i++)
	free(words[i]);
      free(words);
      errorReport("Error reading file");
    }

  //growing word list accommodated by dynamic memory
  //memory is reallocated each time something is stored
  while(curr != EOF)
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

      //at the end of the word
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
      	}

      //completed reading stdin
      if(next == EOF && curr == ' ')
	{
	  break;
	}

      //ignore extra spaces
      else if (curr == ' ' && next == ' ')
	{
	  while(curr == ' ')
	    {
	      curr = getchar();
	      if(ferror(stdin))
		{
		  for(size_t i = 0; i < wordCount; i++)
		    free(words[i]);
		  free(words);
		  errorReport ("Error reading file");
		}
	    }
	  next = getchar();
	  if(ferror(stdin))
	    {
	      for(size_t i = 0; i < wordCount; i++)
		free(words[i]);
	      free(words);
	      errorReport ("Error reading file");
	    }
	  letterCount++;
	  continue;
	}
      
      else if(next == EOF) //Add a space at the end if there isn't already one
	{
	  curr = ' ';
	  letterCount++;
	  continue;
	}

      //increment our letter counter and get the next character
      curr = next;
      next = getchar();
      if (ferror(stdin)) 
	{
	  for(size_t i = 0; i < wordCount; i++)
	    free(words[i]);
	  free(words);
	  errorReport("Error reading file");
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
	  //EOF error checking
	  if (putchar(words[i][j]) == EOF) 
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

 /* free dynamic memory */
  for(size_t i = 0; i < wordCount; i++)
    free(words[i]);
  free(words);

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
  return frobcmp(c, d);
}
