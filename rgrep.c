#include <stdio.h>
#define MAXSIZE 4096

/**
 * You can use this recommended helper function 
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
int matches_leading(char *partial_line, char *pattern) {
  // Implement if desire 

  return 0;
}

/*
Determines the modifiers for the current pattern cursor position
returns:
0 - none
1 - +
2 - ?
3 - +?
*/
int howAreWeLookingFor(char *pattern, int patternCursor)
{
	char x = pattern[patternCursor + 1];
	char y = pattern[patternCursor + 2];

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

int basicMatch(char *line, int lineCursor, char whatToLookFor, int initial)
{
	//Handle period
	if (whatToLookFor == '.')
	{
		if (line[lineCursor] != 0)
			return ++lineCursor;
		else
			return -1;
	}

	/**If this isn't the first character of the pattern we're looking for, only check the current lineCursor position*/
	if (initial == 0)
	{
		if (line[lineCursor] == whatToLookFor)
			return ++lineCursor;
		else
			return -1;
	}

	while (line[lineCursor] != 0)
	{
		if (line[lineCursor] == whatToLookFor)
			return ++lineCursor; //Return the next line it should be at
		else
			lineCursor++;
	}
	return -1; //Did not find anything
}

int repeatMatch(char *line, int lineCursor, char whatToLookFor, int initial)
{
	int lastKnownLine = lineCursor;
	lineCursor = basicMatch(line, lineCursor, whatToLookFor, initial);
	if (lineCursor == -1)
		return -1; //No match

	if (whatToLookFor == '.')
		whatToLookFor = line[lineCursor - 1];

	while (lineCursor != -1)
	{
		lastKnownLine = lineCursor;
		lineCursor = basicMatch(line, lineCursor, whatToLookFor, 0);
	}
	return lastKnownLine;
}

int questionMatch(char *line, char *pattern, int lineCursor, int patternCursor, char whatToLookFor, int initial)
{
	int lastKnownLine = lineCursor;
	lineCursor = basicMatch(line, lineCursor, whatToLookFor, initial);
	if (lineCursor == -1)
		return lastKnownLine; //doesn't exist, but doesn't matter either
	return lineCursor;
}

int theMatcher(char *line, char *pattern, int lineCursor, int patternCursor, int initial)
{
	/**If we're at the end of the pattern and we are matching, return now
		Otherwise, reset patternCursor*/
	int match = 0; //Are we currently matching (used when we hit end of line)
	while(1)
	{
		if (!pattern[patternCursor])
		{
			if (match)
				return 1;
			else
				return -1;
		}
		

		//determine what we're looking for from pattern (separate method that returns necessary stuff)
		if (pattern[patternCursor] == '\\')
		{
			patternCursor++; //Skip to next character
		}

		char thingWeAreLookingFor = pattern[patternCursor];
		int howAreWeLookingForThing = howAreWeLookingFor(pattern, patternCursor);

		/**If we're at end of line, but haven't finished matching the pattern yet, then no this is not a match*/
		if (line[lineCursor] == 0 && howAreWeLookingForThing < 2)
				return 0;
			

		//call other methods depending on its result
		//. = basically insta-match, unless null
		int result;
		switch (howAreWeLookingForThing)
		{
			case 0:
				result = basicMatch(line, lineCursor, thingWeAreLookingFor, initial);
				patternCursor++; //Move on to next character in pattern
				break;
			case 1:
				result = repeatMatch(line, lineCursor, thingWeAreLookingFor, initial);
				patternCursor = patternCursor + 2; //Jump over the +
				if (thingWeAreLookingFor == '.') //Handle period
					thingWeAreLookingFor = line[lineCursor];
				if (result != -1) //Attempt to consider cases like a+a via decrementing a copy of lineCursor
				{
					int preliminaryLineCursor = result;
					while (line[--preliminaryLineCursor] == thingWeAreLookingFor && preliminaryLineCursor > lineCursor)
					{
						int preliminaryResult = theMatcher(line, pattern, preliminaryLineCursor, patternCursor, 0);
						if (preliminaryResult == 1)
						{
							return 1;
						}
					}	
				}
				break;
			case 2:
				//First see if we can match omitting this character
				if (theMatcher(line, pattern, lineCursor, patternCursor + 2, 0) == 1)
					return 1;
				result = questionMatch(line, pattern, lineCursor, patternCursor, thingWeAreLookingFor, initial);
				patternCursor = patternCursor + 2; //Jump over the ?
				break;
			case 3:
				//First see if we can match omitting this entirely
				if (theMatcher(line, pattern, lineCursor, patternCursor + 3, 0) == 1)
					return 1;
				result = repeatMatch(line, lineCursor, thingWeAreLookingFor, initial);
				patternCursor = patternCursor + 3;
				if (thingWeAreLookingFor == '.') //Handle period
					thingWeAreLookingFor = line[lineCursor];
				if (result != -1) //Attempt to consider cases like a+a via decrementing a copy of lineCursor
				{
					int preliminaryLineCursor = result;
					while (line[--preliminaryLineCursor] == thingWeAreLookingFor && preliminaryLineCursor > lineCursor)
					{
						int preliminaryResult = theMatcher(line, pattern, preliminaryLineCursor, patternCursor, 0);
						if (preliminaryResult == 1)
						{
							return 1;
						}
					}	
				}
				break;
		}
		if (result != -1)
		{
			lineCursor = result;
			match = 1;
			initial = 0;
		}

		/**If one part doesn't match - reset pattern "cursor" position.
		Also reset line "cursor" position + amount of attempts
		*/
		else
			return -1;
	}
		//? = return same value/position if not found
		//+ = if found, keep check if next character also matches. Continue process, and return appropriate position
		//+? = same as ?, but if found, apply + (call ? method, if it doesn't return same position, call +)
}
/**
 * You may assume that all strings are properly null terminated 
 * and will not overrun the buffer set by MAXSIZE 
 *
 * Implementation of the rgrep matcher function
Student note: main calls rgreg_matches for _each line._
 */
int rgrep_matches(char *line, char *pattern) {

	/**First we're gonna split up our pattern	*/
	
	/**We search the line for each part of the pattern
	We maintain line "cursor" position as we iterate through each section of the pattern (result must be contiguous, obviously)
	*/

	int lineCursor = 0;
	int patternCursor = 0;
	int initial = 1; //Is this the first character we're looking for?
	int	matching = theMatcher(line, pattern, lineCursor, patternCursor, initial);
	while (matching == -1)
	{
		patternCursor = 0;
		lineCursor++;
		matching = theMatcher(line, pattern, lineCursor, patternCursor, initial);
	}

    return matching;
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
