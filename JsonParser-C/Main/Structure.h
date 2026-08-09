// Creation Date: 2023.02.28. Tue, 22:34:10
// Modified Date: 2023.02.28. Tue, 22:36:50
#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__

#include "Constant.h"

typedef struct _Token
{
	TokenType type;

	union
	{
		char* tokenString;
		double tokenNumber;
	};

	int isArray;
} Token, *PToken;

typedef struct _JSON
{
	Token tokens[MAX_TOKEN];
} JSON, *PJSON, **PPJSON;

#endif