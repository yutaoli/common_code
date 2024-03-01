
#include <openssl/sha.h>
#include <string.h>

#include "cc_encrypt.h"

namespace cc_encrypt
{
    std::string CcEncrypt::Bin2Hex(unsigned char *s, int len)
    {
        char ch[64];

        unsigned int offset = 0;
        for (int i = 0; i < len; i++, offset = offset + 2)
        {
            snprintf(ch + offset, 3, "%02x", s[i]);
        }
        return ch;
    }
    std::string CcEncrypt::Sha1(const std::string &data)
    {
        // 等价于 liyt@T640:~/data/project/jbh_shell/test$ echo -n  'Hello, world!'|sha1sum
        size_t length = strlen(data.c_str());

        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1((const unsigned char *)data.c_str(), length, hash);
        return Bin2Hex(hash, SHA_DIGEST_LENGTH).c_str();
    }
}