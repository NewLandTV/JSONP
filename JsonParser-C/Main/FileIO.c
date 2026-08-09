// Creation Date: 2023.02.28. Tue, 22:42:29
// Modified Date: 2023.02.28. Tue, 23:33:05
#include <string.h>
#include <stdlib.h>
#include "FileIO.h"

#pragma warning(disable:4996)

char* GetPureFileName(IN char* fileName)
{
	int index = 0;
	char* token[10];

	token[index++] = strtok(fileName, "\\");

	while (token[index - 1] != NULL)
	{
		token[index++] = strtok(NULL, "\\");
	}

	return token[index - 2];
}

void ByRuvendixOpenFile(OUT FILE** file, IN char** fileName, IN char* mode)
{
	*file = fopen(fileName, mode);

	if (*file == NULL)
	{
		perror("Failed to open file!");
		exit(EXIT_FAILURE);
	}
}

void ByRuvendixCloseFile(IN FILE* file)
{
	if (fclose(file) == EOF)
	{
		perror("Failed to close file");
		exit(EXIT_FAILURE);
	}
}