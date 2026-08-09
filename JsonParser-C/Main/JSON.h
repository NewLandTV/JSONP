// Creation Date: 2023.02.28. Tue, 22:46:54
// Modified Date: 2023.02.28. Tue, 22:53:23
#ifndef __JSON_H__
#define __JSON_H__

#include "Constant.h"
#include "Structure.h"

char* ReadJSON(IN char* fileName, OUT int* jsonSize);
void ParseJSON(IN char* document, int size, OUT PJSON json);
ReturnType CaseString(IN char* document, OUT int* position, OUT int* tokenIndex, OUT PPJSON json);
ReturnType CaseArray(IN char* document, OUT int* position, OUT int* tokenIndex, OUT PPJSON json);
ReturnType CaseNumber(IN char* document, OUT int* position, OUT int* tokenIndex, OUT PPJSON json);
char* GetStringToken(IN PJSON json, IN char* key);
char* GetArrayStringToken(IN PJSON json, IN char* key, int index);
int GetArrayLength(IN PJSON json, char* key);
double GetNumberToken(IN PJSON json, IN char* key);
void FreeJSON(PJSON json);

#endif