#include <string.h>
#include <sys/stat.h>

#include "file.h"

int File::GetFileSize(const char* file_name, size_t &file_size, std::string &errmsg) 
{
	if (file_name == NULL) 
    {
		return -1;
	}

	struct stat statbuf;

#ifdef _MSC_VER
    constexpr size_t errmsg_size = 1024;
    char msg[errmsg_size] = {0};
#endif

    int ret = stat(file_name, &statbuf);
	if (ret == -1)
	{
#ifdef _MSC_VER
        strerror_s(msg, errmsg_size, errno);
        errmsg = msg;
#else
        errmsg = strerror(errno);
#endif
        return errno;
	}

	// 获取文件大小
	file_size = statbuf.st_size;
    return 0;
}
int File::ReadTheWholeFile(const char *file_name, std::string &file_out,std::string &errmsg)
{
        file_out.clear();
    size_t file_size = 0;
    int ret = GetFileSize(file_name, file_size, errmsg);
    if (ret != 0)
        return ret;

    char *buffer = new (std::nothrow) char[file_size];
    if (buffer == NULL)
        return -2;

     ret = ReadTheWholeFile(file_name, buffer, file_size, errmsg);
    if (ret != 0)
    {
        goto end;
    }

    file_out.assign(buffer, file_size);
end:
    if (buffer != NULL)
    {
        delete[] buffer;
        buffer = NULL;
    }
    return ret;
}
int File::ReadTheWholeFile(const char *file_name, char *buffer, size_t &buffer_size, std::string &errmsg)
{
    size_t file_size = 0;
    int ret = GetFileSize(file_name, file_size, errmsg);
    if (ret != 0)
        return ret;

 char msg[128] = {0};
     ret = 0;
    if (buffer_size < file_size)
    {
        snprintf(msg, sizeof(msg), "buffer size not enough,file_name:%s,buffer size:%u,file_size:%u",
                 file_name, buffer_size, file_size);
        errmsg = msg;
        return -1;
    }

    if(buffer_size == 0) return 0;

    size_t fread_ret = 0;  
    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        snprintf(msg, sizeof(msg), "fopen return null,file_name:%s,errno:%d", file_name, errno);
        errmsg = msg;
        ret = -2;
        goto end;
    }

    fread_ret = fread(buffer, 1, file_size, fp);
    if (fread_ret != file_size)
    {
        snprintf(msg, sizeof(msg), "fread ret:%u,file_name:%s,buffer size:%u,file_size:%u",
                 fread_ret, file_name, buffer_size, file_size);
        errmsg = msg;

        ret = -3;
        goto end;
    }

    buffer_size = file_size;

end:
    fclose(fp);
    return 0;
}

int File::WriteTheWholeFile(const char *file_name, const char *buffer, size_t buffer_size, std::string &errmsg)
{
    if (file_name == NULL || buffer == NULL)
        return -1;

    size_t fwrite_ret = 0;
    char msg[128] = {0};
    FILE *fp = fopen(file_name, "wb");
    if (fp == NULL)
    {
        snprintf(msg, sizeof(msg), "fopen return null,file_name:%s,errno:%d,errmsg:%s",
                 file_name, errno, strerror(errno));
        errmsg = msg;
        return -2;
    }

    int ret = 0;
    // 清空
    if (buffer_size == 0)
    {
        goto end;
    }

    fwrite_ret = fwrite(buffer, 1, buffer_size, fp);
    if (fwrite_ret != buffer_size)
    {
        snprintf(msg, sizeof(msg), "fwrite error,file_name:%s", file_name);
        errmsg = msg;
        ret = -1;
        goto end;
    }

    fflush(fp);

end:
    fclose(fp);
    return ret;
}