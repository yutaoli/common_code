#ifndef CC_ENCRYPT_H
#define CC_ENCRYPT_H

#include <string>

namespace cc_encrypt
{
    class CcEncrypt
    {
        public:
        static std::string Bin2Hex(unsigned char *s, int len);
        static std::string Sha1(const std::string &data);
    };
}
#endif