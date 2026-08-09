// Creation Date: 2023.02.28. Tue, 22:30:33
// Modified Date: 2023.02.28. Tue, 22:32:28
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