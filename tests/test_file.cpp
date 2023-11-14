#include <CppUTest/CommandLineTestRunner.h>
#include <thread>
#include <unistd.h>

#include "file.h"

TEST_GROUP(File)
{
    void setup()
    {

    } 
    void teardown()
    {
        
    }
};

TEST(File, TestFileNotExist)
{
    const char *file_name = "./tes2dfadt_fidf1ale2";
    size_t file_size = 0;
    std::string errmsg;
    int ret = File::GetFileSize(file_name, file_size,errmsg);
   CHECK(ret == ENOENT);
}

TEST(File, WriteFileAndReadFile)
{
    const char *file_name = "./tes2dfadt_fidfale2tes2dfadt_fidfale2";
    std::string file = "<xml>test jkdfa\ndfa</xml>";
    std::string errmsg;
    int ret = File::WriteTheWholeFile(file_name, file.data(), file.size(),errmsg);
   CHECK(ret == 0);

 std::string file_out;
   ret = File::ReadTheWholeFile(file_name, file_out,errmsg);
   CHECK(ret == 0);
   CHECK(file_out.size() == file.size());

   ret = remove(file_name);
   CHECK(ret == 0);
}

TEST(File, WriteFileTwiceAndReadFile)
{
    const char *file_name = "./tes2dfadt_tes2dfadt_fidfale2fidftes2dfadt_fidfale2dfadale2";
    std::string file = "<xml>test jkdfa\ndfa</xml>";
    std::string errmsg;
    int ret = File::WriteTheWholeFile(file_name, file.data(), file.size(),errmsg);
   CHECK(ret == 0);

    std::string file2 = "<xml>test jkdfa\ndffffffffffffffffffffffffffffffffffffa</xml>";
     ret = File::WriteTheWholeFile(file_name, file2.data(), file2.size(),errmsg);
   CHECK(ret == 0);

 std::string file_out;
   ret = File::ReadTheWholeFile(file_name, file_out,errmsg);
   CHECK(ret == 0);
   CHECK(file_out.size() == file2.size());

   ret = remove(file_name);
   CHECK(ret == 0);
}

TEST(File, TestClearFile)
{
    const char *file_name = "./TestClearFile";
    std::string file = "<xml>test jkdfa\ndfa</xml>";
    std::string errmsg;
    // 写入文件
    int ret = File::WriteTheWholeFile(file_name, file.data(), file.size(),errmsg);
   CHECK(ret == 0);

    // 清空
    size_t file_size = 0;
    ret = File::WriteTheWholeFile(file_name, "", file_size,errmsg);
   CHECK(ret == 0);

    file_size = -1;
     ret = File::GetFileSize(file_name, file_size, errmsg);
    CHECK(ret == 0);
    CHECK(file_size == 0);

   std::string file_out;
   ret = File::ReadTheWholeFile(file_name, file_out,errmsg);
   CHECK(ret == 0);
   CHECK(file_out.size() == 0);

   ret = remove(file_name);
   CHECK(ret == 0);
}