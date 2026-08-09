#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#define IN
#define OUT

#define MAX_TOKEN 20

typedef enum _ReturnTrpe
{
	Success,
	Failure
} ReturnType;

typedef enum _TokenType
{
	String,
	Number
} TokenType;

#endif