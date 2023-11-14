#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <string>

class File
{
public:
static int GetFileSize(const char* file_name, size_t &file_size, std::string &errmsg);

// 读整个文件
static int ReadTheWholeFile(const char *file_name, std::string &file_out,std::string &errmsg);
static int ReadTheWholeFile(const char *file_name, char *buffer, size_t &buffer_size,std::string &errmsg);

// 写整个文件
static int WriteTheWholeFile(const char *file_name, const char *buffer, size_t buffer_size,std::string &errmsg);
};
#endif