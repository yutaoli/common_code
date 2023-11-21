#ifndef SHM_H
#define SHM_H

#ifdef _MSC_VER
#include <windows.h>
#include <wtsapi32.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define kShmnameSize 128

#ifdef _MSC_VER
#define kFilenameSize 128
#endif
//T *g_mq_stat_shmp;

template <typename T>
class Shm
{
public:
	Shm()
	{
#ifdef _MSC_VER
		fd_ = 0;
		file_fd_ = 0;
#else
		fd_ = -1;
#endif
		shmp_ = NULL;
	}
	virtual ~Shm()
	{
		// DO NOTHING
	}
public:

	/* posix demo:https://man7.org/linux/man-pages/man3/shm_open.3.html */
	int ShmRInit(const char* shmname)
	{
#ifdef _MSC_VER
		DWORD ret = 0;
		char* filepath = GetFilePath(shmname);
		char* shmpath = GetShmPath(shmname);
		file_fd_ = CreateFileA(
			filepath, // file name
			GENERIC_READ | GENERIC_WRITE, // access type
			FILE_SHARE_READ | FILE_SHARE_WRITE, // other processes can't share
			NULL, // security attr
			OPEN_EXISTING, // open only if file exists
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (file_fd_ == INVALID_HANDLE_VALUE)
		{
			ret = GetLastError();
			if (ret == ERROR_FILE_NOT_FOUND)
			{
				printf("CreateFileA ERROR_FILE_NOT_FOUND ,GetLastError:%u,shmpath:%s,filepath:%s\n",
					ret, shmpath, filepath);
				return -1;
			}
			else if(ret != ERROR_SHARING_VIOLATION)
			{
				printf("CreateFileA GetLastError:%u,shmpath:%s,filepath:%s\n",
					ret, shmpath, filepath);
				return -2;
			}
		}

		if (ret == ERROR_SHARING_VIOLATION)
		{
			fd_ = OpenFileMapping(FILE_MAP_READ, TRUE, shmpath);
			if (fd_ == NULL)
			{
				printf("OpenFileMapping fail,GetLastError:%u,shmpath:%s,filepath:%s\n",
					GetLastError(), shmpath, filepath);
				return -3;
			}
		}
		else
		{
			fd_ = CreateFileMapping(file_fd_, NULL, PAGE_READWRITE, 0, (DWORD)sizeof(T), shmpath);
			if (fd_ == NULL)
			{
				printf("CreateFileMapping fail,GetLastError:%u,shmpath:%s,filepath:%s\n",
					GetLastError(), shmpath, filepath);
				return -1;
			}
			/*ret = GetLastError();
			if (ret == ERROR_ALREADY_EXISTS)
			{
				printf("CreateFileMapping ERROR_ALREADY_EXISTS,GetLastError:%u,shmpath:%s,filepath:%s continue\n",
					ret, shmpath, filepath);
			}*/
		}

		shmp_ = (T*)MapViewOfFile(fd_, FILE_MAP_READ, 0, 0, sizeof(T));
		if (shmp_ == NULL)
		{
			printf("MapViewOfFile ret:%u,shmpath:%s", GetLastError(), shmpath);
			return -2;
		}
		return 0;
#else
		char* shmpath = GetShmPath(shmname);
		fd_ = shm_open(shmpath, O_RDONLY, S_IRUSR | S_IWUSR);
		if (fd_ == -1)
		{
			printf("shm_open fail,errno:%d,msg:%s,shmpath:%s", errno, strerror(errno), shmpath);
			return -1;
		}
		//printf("fd:%d\n",fd);

		shmp_ = (T*)mmap(NULL, sizeof(T), PROT_READ, MAP_SHARED, fd_, 0);
		if (shmp_ == MAP_FAILED)
		{
			printf("mmap fail,errno:%d,msg:%s,shmpath:%s", errno, strerror(errno), shmpath);
			return -2;
		}
#endif
		return 0;
	}

	int ShmWInit(const char* shmname)
	{
#ifdef _MSC_VER
		DWORD ret = 0;
		char* filepath = GetFilePath(shmname);
		char* shmpath = GetShmPath(shmname);
		file_fd_ = CreateFileA(
			filepath, // file name
			GENERIC_READ | GENERIC_WRITE, // access type
			FILE_SHARE_READ | FILE_SHARE_WRITE, // other processes can't share
			NULL, // security attr
			CREATE_NEW, // 
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (file_fd_ == INVALID_HANDLE_VALUE)
		{
			ret = GetLastError();
			if(ret != ERROR_SHARING_VIOLATION
				&& ret != ERROR_FILE_EXISTS)
			{
				printf("CreateFileA GetLastError:%u,shmpath:%s,filepath:%s\n",
					ret, shmpath, filepath);
				return -1;
			}
		}

		if (ret == ERROR_SHARING_VIOLATION)
		{
			fd_ = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, TRUE, shmpath);
			if (fd_ == NULL)
			{
				printf("OpenFileMapping fail,GetLastError:%u,shmpath:%s,filepath:%s\n",
					GetLastError(), shmpath, filepath);
				return -3;
			}
		}
		else
		{
			fd_ = CreateFileMapping(file_fd_, NULL, PAGE_READWRITE, 0, (DWORD)sizeof(T), shmpath);
			if (fd_ == NULL)
			{
				printf("CreateFileMapping fail,GetLastError:%u,shmpath:%s,filepath:%s\n",
					GetLastError(), shmpath, filepath);
				return -1;
			}
			/*ret = GetLastError();
			if (ret == ERROR_ALREADY_EXISTS)
			{
				printf("CreateFileMapping ERROR_ALREADY_EXISTS,GetLastError:%u,shmpath:%s,filepath:%s continue\n",
					ret, shmpath, filepath);
			}*/
		}

		shmp_ = (T*)MapViewOfFile(fd_, FILE_MAP_READ| FILE_MAP_WRITE, 0, 0, sizeof(T));
		if (shmp_ == NULL)
		{
			printf("MapViewOfFile ret:%u,shmpath:%s,filepath:%s\n", GetLastError(), shmpath, filepath);
			return -2;
		}
		memset(shmp_, 0, sizeof(T));
		return 0;
#else
		char* shmpath = GetShmPath(shmname);
		fd_ = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
		if (fd_ == -1)
		{
			if (errno != EEXIST)
			{
				printf("shm_open fail,errno:%d,msg:%s,shmpath:%s", errno, strerror(errno), shmpath);
				return -1;
			}

			//printf("file%s exist\n", shmpath);
			fd_ = shm_open(shmpath, O_RDWR, S_IRUSR | S_IWUSR);
			if (fd_ == -1)
			{
				printf("shm_open fail,errno:%d,msg:%s,shmpath:%s", errno, strerror(errno), shmpath);
				return -2;
			}
		}
		//printf("fd:%d\n",fd);

		if (ftruncate(fd_, sizeof(T)) == -1)
		{
			//printf("ftruncate fail,errno:%d,msg:%s",errno,strerror(errno));
			return -3;
		}

		shmp_ = (T*)mmap(NULL, sizeof(T), PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
		if (shmp_ == MAP_FAILED)
		{
			//printf("mmap fail,errno:%d,msg:%s",errno,strerror(errno));
			return -4;
		}
#endif
		return 0;
	}

	int ShmFini(const char* shmname)
	{
#ifdef _MSC_VER
		if (shmp_ != NULL)
		{
			if (false == UnmapViewOfFile(shmp_))
			{
				printf("UnmapViewOfFile error,shmname:%s\n", shmname);
				return -1;
			}
			shmp_ = NULL;
		}
		if (fd_)
		{
			CloseHandle(fd_);
			fd_ = 0;
		}
		if (file_fd_)
		{
			CloseHandle(file_fd_);
			file_fd_ = 0;
		}
		return 0;
#else
		if (fd_ != -1)
		{
			close(fd_);
			fd_ = -1;
		}

		if (shmp_ != NULL)
		{
			int ret = munmap(shmp_, sizeof(T));
			if (ret != 0)
			{
				//printf("UnMapping errno:%d,msg:%s\n",errno,strerror(errno));
				return ret;
			}
			shmp_ = NULL;
		}

		char* shmpath = GetShmPath(shmname);
		return shm_unlink(shmpath);
#endif
	}
	T* GetPtrW() { return shmp_; }
	const T* GetPtrR() { return shmp_; }

private:
#ifdef _MSC_VER
	char* GetFilePath(const char* shmname)
	{
		static char filepath[kFilenameSize];

		char* usernameBuffer = nullptr;
		DWORD infoSize = 0;
		WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, WTS_CURRENT_SESSION, WTS_INFO_CLASS::WTSUserName, &usernameBuffer, &infoSize);
		std::string username(usernameBuffer);
		WTSFreeMemory(usernameBuffer);
		snprintf(filepath, kFilenameSize, "%s_%s", username.c_str(), shmname);
		return filepath;
	}
#endif
	char* GetShmPath(const char* shmname)
	{
		static char shmpath[kShmnameSize];
#ifdef _MSC_VER

		snprintf(shmpath, kShmnameSize, "%s", GetFilePath(shmname));
#else
		snprintf(shmpath, kShmnameSize, "/%s_%s", getlogin(), shmname);
#endif
		return shmpath;
	}

private:
#ifdef _MSC_VER
	HANDLE fd_;
	HANDLE file_fd_;
#else
	int fd_;
#endif

	T* shmp_;
};

#endif

/*
write:
Shm<Stat> stat;
stat.ShmWInit("ClientTestStat");
Stat * ptr = GetPtrW();

read:
Shm<Stat> stat;
stat.ShmRInit("ClientTestStat");
const Stat * ptr = GetPtrR();
*/
