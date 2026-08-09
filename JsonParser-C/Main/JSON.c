#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "Constant.h"
#include "Macro.h"
#include "Structure.h"
#include "FileIO.h"
#include "JSON.h"

char* ReadJSON(IN char* fileName, OUT int* jsonSize)
{
	int size;
	char* buffer;
	
	FILE* file;

	ByRuvendixOpenFile(&file, fileName, "rt");
	fseek(file, 0, SEEK_END);

	size = ftell(file);

	fseek(file, 0, SEEK_SET);

	buffer = (char*)malloc(size);

	memset(buffer, 0, size);

	if (fread(buffer, size, 1, file) < 1)
	{
		*jsonSize = 0;

		SafeDelete(buffer);
	}

	*jsonSize = size;

	ByRuvendixCloseFile(file);

	return buffer;
}

void ParseJSON(IN char* document, int size, OUT PJSON json)
{
	int tokenIndex = 0;
	int position = 0;
	int stringLength = 0;

	if (document[position] != '{')
	{
		return;
	}

	position++;

	while (position < size)
	{
		switch (document[position])
		{
		case '"':
			if (CaseString(document, &position, &tokenIndex, &json) == Failure)
			{
				break;
			}

			break;
		case '[':
			if (CaseArray(document, &position, &tokenIndex, &json) == Failure)
			{
				break;
			}

			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '-':
			if (CaseNumber(document, &position, &tokenIndex, &json) == Failure)
			{
				break;
			}

			break;
		}

		position++;
	}
}

ReturnType CaseString(IN char* document, OUT int* position, OUT int* tokenIndex, OUT PPJSON json)
{
	int stringLength;
	char* begin;
	char* end;

	begin = document + *position + 1;

	end = strchr(begin, '"');

	if (end == NULL)
	{
		return Failure;
	}

	stringLength = end - begin;

	(*json)->tokens[*tokenIndex].type = String;
	(*json)->tokens[*tokenIndex].tokenString = (char*)malloc(stringLength + 1);

	memset((*json)->tokens[*tokenIndex].tokenString, 0, stringLength + 1);
	memcpy((*json)->tokens[*tokenIndex].tokenString, begin, stringLength);

	(*tokenIndex)++;

	*position += stringLength;

	return Success;
}

ReturnType CaseArray(IN char* document, OUT int* position, OUT int* tokenIndex, OUT PPJSON json)
{
	(*position)++;

	while (document[*position] != ']')
	{
		if (document[*position] == '"')
		{
			(*json)->tokens[*tokenIndex].isArray = 1;

			if (CaseString(document, position, tokenIndex, json) == Failure)
			{
				(*json)->tokens[*tokenIndex].isArray = 0;

				return Failure;
			}
		}

		(*position)++;
	}

	return Success;
}

ReturnType CaseNumber(IN char* document, OUT int* position, OUT int* tokenIndex, OUT PPJSON json)
{
	int stringLength;
	char* begin;
	char* end;
	char* buffer;

	begin = document + *position;

	end = strchr(begin, ',');

	if (end == NULL)
	{
		end = strchr(begin, '}');

		if (end == NULL)
		{
			return Failure;
		}
	}

	stringLength = end - begin;

	(*json)->tokens[*tokenIndex].type = Number;

	buffer = (char*)malloc(stringLength + 1);

	memset(buffer, 0, stringLength + 1);
	memcpy(buffer, begin, stringLength);

	(*json)->tokens[*tokenIndex].tokenNumber = atof(buffer);

	SafeDelete(buffer);

	(*tokenIndex)++;

	*position += stringLength + 1;

	return Success;
}

char* GetStringToken(IN PJSON json, IN char* key)
{
	int i;

	for (i = 0; i < MAX_TOKEN; i++)
	{
		if (json->tokens[i].type == String && strcmp(json->tokens[i].tokenString, key) == 0)
		{
			if (json->tokens[i + 1].type == String)
			{
				return json->tokens[i + 1].tokenString;
			}
		}
	}

	return NULL;
}

char* GetArrayStringToken(IN PJSON json, IN char* key, int index)
{
	int i;

	for (i = 0; i < MAX_TOKEN; i++)
	{
		if (json->tokens[i].type == String && strcmp(json->tokens[i].tokenString, key) == 0)
		{
			if (json->tokens[i + 1].type == String && json->tokens[i + 1].isArray == 1)
			{
				return json->tokens[i + 1 + index].tokenString;
			}
		}
	}

	return NULL;
}

int GetArrayLength(IN PJSON json, char* key)
{
	int i;
	int count = 0;

	for (i = 0; i < MAX_TOKEN; i++)
	{
		if (json->tokens[i].type == String && strcmp(json->tokens[i].tokenString, key) == 0)
		{
			while (json->tokens[i + 1 + count].isArray == 1)
			{
				count++;
			}

			return count;
		}
	}

	return 0;
}

double GetNumberToken(IN PJSON json, IN char* key)
{
	int i;

	for (i = 0; i < MAX_TOKEN; i++)
	{
		if (json->tokens[i].type == String && strcmp(json->tokens[i].tokenString, key) == 0)
		{
			if (json->tokens[i + 1].type == Number)
			{
				return json->tokens[i + 1].tokenNumber;
			}
		}
	}

	return 0;
}

void FreeJSON(PJSON json)
{
	int i;

	for (i = 0; i < MAX_TOKEN; i++)
	{
		if (json->tokens[i].type == String)
		{
			SafeDelete(json->tokens[i].tokenString);
		}
	}
}