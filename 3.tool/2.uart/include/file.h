
#ifndef _FILE_H_
#define _FILE_H_

#include "main.h"

#define POLYNOMIAL 0x04C11DB7
class Files
{

public:
    Files(const char *name_file, std::ios_base::openmode __mode);
    ~Files();
    uint32_t Files_GetSizeFile(void);
    uint8_t  Files_GetArrFile(const uint32_t size, const uint32_t offset, uint8_t *arr);
    uint32_t Files_CalculatorChecksum(void);
    void  Files_Clear(void);
private:
    std::ifstream in_file;
    
};

#endif 
