#pragma once

#include <pch.h>
#include <openssl/sha.h>
#include <cstring>

namespace gaboot::sha256
{
    inline std::string encode(const std::string str) 
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        std::string hashedString = "";

        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.length());
        SHA256_Final(hash, &sha256);

        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) 
        {
            char hex[3];
            sprintf(hex, "%02x", hash[i]);
            hashedString += hex;
        }

        return hashedString;
    }
}