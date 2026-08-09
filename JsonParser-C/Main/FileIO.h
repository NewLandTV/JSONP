// Creation Date: 2023.02.28. Tue, 22:40:25
// Modified Date: 2023.02.28. Tue, 22:42:04
#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#include <stdio.h>
#include "Constant.h"

char* GetPureFileName(IN char* fileName);
void ByRuvendixOpenFile(OUT FILE** file, IN char** fileName, IN char* mode);
void ByRuvendixCloseFile(IN FILE* file);

#endif