#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#include <stdio.h>
#include "Constant.h"

char* GetPureFileName(IN char* fileName);
void ByRuvendixOpenFile(OUT FILE** file, IN char** fileName, IN char* mode);
void ByRuvendixCloseFile(IN FILE* file);

#endif