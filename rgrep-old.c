#include <stdio.h>
#define MAXSIZE 4096

/**
 * You can use this recommended helper function 
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
int matches_leading(char *line, char *pattern, int lineIndex)
{
	char p = pattern[0];
	char l = line[lineIndex];
	
	if (p == '.') 
		return 1;
		
	if (p == l && p != '\\')
		return 1;
	
	if (p == '\\' && pattern[1] == l)
		return 1;

  return 0;
}

/*
Determines the modifiers for the specified character index
returns:
0 - none
1 - +
2 - ?
3 - +?
*/

int hasModifiers(char *pattern, int patternIndex)
{
	char x = pattern[patternIndex + 1];
	char y = pattern[patternIndex + 2];
	
	if (x == '+')
	{
		if (y == '\?')
			return 3;
		return 1;
	}
	
	if (x == '\?')
	{
		if (y == '+')
			return 3;
		return 2;
	}
	
	//No modifiers if we get to here
	return 0;
}

/*
int patternSplicer(char *line, char *pattern, int lineIndex, int patternIndex)
{
	if (!line[lineIndex])
		return -999;
		
	int escaped = 0;
	int i = 1;
	int j = 0;
	//Check if pattern contains an escape
	if (pattern[patternIndex] == '\\')
	{
		escaped = 1;
		patternIndex++;
	}
	
	//If escaped .
	if (pattern[patternIndex] == '.' && escaped)
	{
		int modified = hasModifiers(pattern, patternIndex);
		switch (modified)
		{
			case 0:
				return 1;
			case 2:
				if (line[lineIndex] == '.')
					return 1;
				else
					return 0;
			case 1:
				while (i)
				{
					if (line[lineIndex] == '.')
					{
						lineIndex++;
						j++
					}
				}
				return j;
			case 3:
				if (line[lineIndex] == '.')
				{
					while (i)
					{
						if (line[lineIndex] == '.')
						{
							lineIndex++;
							j++
						}
					}
				return j;
				}
				else
					return 0;
			
		}
	}
	return -99;
}
*/

int checkNext(char *line, char *pattern, int i, int j)
{
	while (line[i] != pattern[j])
	{
		i++;
	}
	return i;
}

int directMatch(char *line, char *pattern, int i, int j)
{
	//If the character of the line array is matching the character of the pattern array,
	if (line[i] == pattern[j])
	{
		return 1;
	}
	return 0;
}

int runSomethingElse(char *line, char *pattern, int i, int j)
{	
	
	
	if (pattern[j] == 0)
	{
		return 1;
	}
	
	if (line[i] == 0)
		return 0;
	/*
	while (line[i] != 0)
	{
		int match = matches_leading(line, pattern, i);
		if (match)
			break;
		i++;
	}
	*/


	/*
	Handle backslash
	*/
	if (pattern[j] == '\\')
	{
		if (directMatch(line, pattern, i, (j + 1)))
		{
			//TODO: check for modifiers
			j++;
			j++;
			i++;
			return runSomethingElse(line, pattern, i, j);
		}
	}
	
	int modifier = hasModifiers(pattern, j);
	
	/*
	Handle le period wildcard
	*/
	if (pattern[j] == '.')
	{
		switch (modifier)
		{
			case 3:
			case 1:
				j++;
				i = checkNext(line, pattern, i, j);
				return runSomethingElse(line, pattern, i, j);
			case 2:
				j++;
				j++;
				i = checkNext(line, pattern, i, j);
				return runSomethingElse(line, pattern, i, j);
		}
	}
	
	/*
	apply cases for modifiers
	*/
	
	switch (modifier)
	{
		// +
		case 1:
			i = checkNext(line, pattern, i, j);
			break;		
		// ?
		case 2:
			break;		
		// +?
		case 3:
			break;
	}
	
	if (directMatch(line, pattern, i, j))
	{
		i++;
		j++;
	}
	
	if (pattern[j] != 0)
	{
		if (line[i] == pattern[j])
			return runSomethingElse(line, pattern, (i + 1), (j + 1));
	}
	
	
	return runSomethingElse(line, pattern, (i + 1), 0);
}

/**
 * You may assume that all strings are properly null terminated 
 * and will not overrun the buffer set by MAXSIZE 
 *
 * Implementation of the rgrep matcher function
 */
int rgrep_matches(char *line, char *pattern) 
{
    //int lineSize = sizeof(line)/sizeof(line[0]);
	//int patternSize = sizeof(pattern)/sizeof(pattern[0]);
    int i = 0;
	//int initialMatch = 1;
	int z = 0;
	while (pattern[z] != 0)
	{
		char y = pattern[z];
		if (y == '.' || y == '+' || y == '\?' || y == '\\')
		{
			int ohno = runSomethingElse(line, pattern, 0, 0);
			return ohno;
		}
		z++;
	}
	
	
	//Basic (dumb) checker
    while (1)
	{
	    //If the character of the line array is matching the character of the pattern array,
		if (line[i] == pattern[0])
		{
		    int a = i; //copy i so we don't alter it
			int b = 0; //we'll need original value of i if this fails
		    int match = 1;
			while (match)
			{
			    //If we're at the end of the line while match == 1...
				if (pattern[b] == 0)
				    return 1; //Huzzah
					
				//If we aren't matching,
				if (line[a] != pattern[b])
				{
					match = 0;
				}
				a++;
			    b++;
			}
		}
		
		//move to next character in line (failed match, move on)
		i++;
		
		
		//If we're at the end of the line
		if (line[i] == 0)
		{
			//No match!
			return 0;
		}
	}

    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PATTERN>\n", argv[0]);
        return 2;
    }

    /* we're not going to worry about long lines */
    char buf[MAXSIZE];

    while (!feof(stdin) && !ferror(stdin)) {
        if (!fgets(buf, sizeof(buf), stdin)) {
            break;
        }
        if (rgrep_matches(buf, argv[1])) {
            fputs(buf, stdout);
            fflush(stdout);
        }
    }

    if (ferror(stdin)) {
        perror(argv[0]);
        return 1;
    }

    return 0;
}
