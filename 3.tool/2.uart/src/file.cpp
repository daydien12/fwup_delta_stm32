#include "file.h"

uint32_t crc32(unsigned long crc, const unsigned char *data, size_t length);

Files::Files(const char *name_file, std::ios_base::openmode __mode)
{
  in_file.open(name_file, __mode);
  if (!in_file.is_open())
  {
    std::cerr << "Error in open file: " << name_file << std::endl;
  }
}

Files::~Files()
{
  in_file.close();
}

uint32_t Files::Files_GetSizeFile(void)
{
  in_file.seekg(0, std::ios::end);
  return in_file.tellg();
}

uint8_t Files::Files_GetArrFile(const uint32_t size, const uint32_t offset, uint8_t *arr)
{
  uint8_t data = 0, count_read_data = 0;
  if (!in_file.is_open())
  {
    return 0;
  }
  in_file.seekg(offset);
  for (count_read_data = 0; count_read_data < size; count_read_data++)
  {
    in_file.read((char *)&data, sizeof(uint8_t));
    arr[count_read_data] = data;
  }
  return count_read_data;
}

uint32_t Files::Files_CalculatorChecksum(void)
{
  uint8_t data = 0;
  uint32_t check_sum = 0xFFFFFFFFUL;
  uint32_t length_file = 0;
  length_file = Files_GetSizeFile();
  if (!in_file.is_open())
  {
    return 0;
  }
  in_file.seekg(0);
  for (int i = 0; i < length_file; i++)
  {
    in_file.read((char *)&data, sizeof(uint8_t));
    check_sum ^= data;
    for (int j = 0; j < 8; j++)
    {
      check_sum = (check_sum >> 1) ^ ((check_sum & 1) ? POLYNOMIAL : 0);
    }
  }
  return (check_sum ^ 0xFFFFFFFFUL);
}

void Files::Files_Clear(void)
{
  in_file.close();
}
