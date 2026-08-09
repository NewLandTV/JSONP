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