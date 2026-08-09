#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "Structure.h"
#include "Macro.h"
#include "FileIO.h"
#include "JSON.h"

int main(int argc, char** argv)
{
	int i;
	int size;
	int arrayLength;
	char* document;
	char* fileName;

	JSON targetJson;

	/*document = ReadJSON(argv[1], &size);
	fileName = GetPureFileName(argv[1]);*/

	document = ReadJSON("E:\\Programming\\Languages\\Programming\\C\\JsonParser\\Main\\Data.json", &size);
	fileName = GetPureFileName("E:\\Programming\\Languages\\Programming\\C\\JsonParser\\Main\\Data.json");

	memset(&targetJson, 0, sizeof(JSON));
	ParseJSON(document, size, &targetJson);
	printf("Product name : %s\n", GetStringToken(&targetJson, "Title"));
	printf("Made company : %s\n", GetStringToken(&targetJson, "Company"));
	printf("Product price : %d\\\n", (int)GetNumberToken(&targetJson, "Price"));
	printf("Product ingredient : %s\n", GetStringToken(&targetJson, "Ingredient"));

	arrayLength = GetArrayLength(&targetJson, "Ingredient");

	for (i = 1; i < arrayLength; i++)
	{
		printf("\t%s\n", GetArrayStringToken(&targetJson, "Ingredient", i));
	}

	FreeJSON(&targetJson);

	SafeDelete(document);

	return 0;
}