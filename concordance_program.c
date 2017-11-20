#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

FILE* processArgs(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	FILE* fp = processArgs(argc, argv);
	fseek(fp, 0, SEEK_END);
	size_t SIZE = ftell(fp);
	char str[SIZE];
	fseek(fp, 0, SEEK_SET);
	fread(str, 1, SIZE, fp);

	// Separate tokens
	char* token;
	char** tokHEAD = malloc(sizeof(char*));
	char** tokens = tokHEAD;
	int numOfTokens = 0;
	token = strtok(str, " ");
	
	while (token != NULL) 
	{
		// Remove first and last character is non alphanumeric
		int len = strlen(token);
		if (!isalnum(token[len - 1]))
		{
			token[len - 1] = '\0';
			len -= 1;
		}
		
		if (!isalnum(*token))
		{
			int i;
			for (i = 0; i < len; i++)
				token[i] = token[i + 1];
		}

		// printf("%s\n", token);

		tokHEAD = realloc(tokHEAD, sizeof(char*) * ++numOfTokens);
		*tokens++ = token;
		token = strtok(NULL, " ");
	}

	// Sort tokens
	int i, j;
	for (i = 0; i < numOfTokens - 1; i++)
	{
		for (j = i + 1; j < numOfTokens; j++)
		{
			size_t tokSIZE1 = strlen(tokHEAD[i]), tokSIZE2 = strlen(tokHEAD[j]);
			char temp1[tokSIZE1], temp2[tokSIZE2];
			int k;
			for (k = 0; k < tokSIZE1; k++)
				temp1[k] = tolower(tokHEAD[i][k]);
			
			for(k = 0; k < tokSIZE2; k++)
				temp2[k] = tolower(tokHEAD[j][k]);

			if (strcmp(temp1, temp2) > 0)
			{
				char* temp = tokHEAD[i];
				tokHEAD[i] = tokHEAD[j];
				tokHEAD[j] = temp;
			}
		}
	}

	// Get word count
	int wordCount[numOfTokens], index = 0;
	for(i = 0; i < numOfTokens; i++)
		wordCount[i] = 0;
	for (i = 0; i < numOfTokens; i++)
	{
		if (i > 0 && strcmp(tokHEAD[i], tokHEAD[i - 1]) == 0)
				wordCount[i++] = 0;
		else
		{
			wordCount[i]++;
			for (j = i + 1; j < numOfTokens; j++)
				if (strcmp(tokHEAD[i], tokHEAD[j]) == 0)
					wordCount[i]++;
		}
	}

	// Print to file
	FILE* outFile = fopen(argv[3], "w+");
	if (outFile == NULL)
	{
		perror("Opening file failed");
	    exit(1);
	}


	for (i = 0; i < numOfTokens; i++)
		if (wordCount[i] > 0)
		{
			printf("%s:%*s%d\n", tokHEAD[i], 20, wordCount[i]);
			fprintf(outFile, "%s:%*s%d\n", 20, tokHEAD[i], wordCount[i]);
		}

	free(tokHEAD);
	close(fp);

	return 0;
}

FILE* processArgs(int argc, char* argv[])
{
	FILE* fp;
	if (argc == 4)
	{
		if ((fp = fopen(argv[2], "r")) == NULL)
		{
			perror("Opening file failed");
		    exit(1);
		}
	}
	else
	{
		fprintf(stderr, "%s\n", "Incorrect number of arguments.");
		exit(1);
	}

	return fp;
}
