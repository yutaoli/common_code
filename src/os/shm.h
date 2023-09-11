#ifndef SHM_H
#define SHM_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#define kShmnameSize 128
//T *g_mq_stat_shmp;

template <typename T>
class Shm
{
	public:
		Shm()
		{
			fd_ = -1;
			shmp_ = NULL;
		}
		virtual ~Shm()
		{
			// DO NOTHING
		}
	public:

		/* posix demo:https://man7.org/linux/man-pages/man3/shm_open.3.html */
		int ShmRInit(const char *shmname)
		{
			char *shmpath = GetShmPath(shmname);
			fd_ = shm_open(shmpath, O_RDONLY, S_IRUSR | S_IWUSR);
			if(fd_ == -1)
			{
				//printf("shm_open fail,errno:%d,msg:%s,shmpath:%s",errno,strerror(errno),shmpath);
				return -1;
			}
			//printf("fd:%d\n",fd);

			shmp_ = (T *)mmap(NULL, sizeof(T), PROT_READ, MAP_SHARED, fd_, 0);
			if(shmp_ == MAP_FAILED)
			{
				//printf("mmap fail,errno:%d,msg:%s,shmpath:%s",errno,strerror(errno),shmpath);
				return -2;
			}

			return 0;
		}

		int ShmWInit(const char *shmname)
		{
			char *shmpath = GetShmPath(shmname);
			fd_ = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
			if(fd_ == -1)
			{
				if(errno != EEXIST)
				{
					//printf("shm_open fail,errno:%d,msg:%s,shmpath:%s",errno,strerror(errno),shmpath);
					return -1;
				}

				//printf("file%s exist\n", shmpath);
				fd_ = shm_open(shmpath, O_RDWR, S_IRUSR | S_IWUSR);
				if(fd_  == -1)
				{
					// printf("shm_open fail,errno:%d,msg:%s,shmpath:%s",errno,strerror(errno),shmpath);
					return -2;
				}
			}
			//printf("fd:%d\n",fd);

			if(ftruncate(fd_, sizeof(T)) == -1)
			{
				//printf("ftruncate fail,errno:%d,msg:%s",errno,strerror(errno));
				return -3;
			}

			shmp_ = (T *)mmap(NULL, sizeof(T), PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
			if(shmp_ == MAP_FAILED)
			{
				//printf("mmap fail,errno:%d,msg:%s",errno,strerror(errno));
				return -4;
			}

			return 0;
		}

		int ShmFini(const char *shmname)
		{
			if(fd_ != -1)
			{
				close(fd_);
				fd_ = -1;
			}

			if(shmp_ != NULL)
			{
				int ret = munmap(shmp_, sizeof(T));
				if(ret != 0)
				{
					//printf("UnMapping errno:%d,msg:%s\n",errno,strerror(errno));
					return ret;
				}
				shmp_ = NULL;
			}

			char *shmpath = GetShmPath(shmname);
			return shm_unlink(shmpath);
		}
		T* GetPtrW(){ return shmp_; }
		const T* GetPtrR(){ return shmp_; }

	private:
		char * GetShmPath(const char *shmname)
		{
			static char shmpath[kShmnameSize];
			snprintf(shmpath, kShmnameSize, "/%s_%s", getlogin(),shmname);
			return shmpath;
		}

	private:
		int fd_;
		T *shmp_;
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
