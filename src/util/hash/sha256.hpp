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

        // Use SHA256_Init_ex to avoid deprecation warning
        EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL); // Initialize the digest context with SHA-256
        EVP_DigestUpdate(mdctx, str.c_str(), str.length()); // Update the digest context with the data
        EVP_DigestFinal_ex(mdctx, hash, NULL); // Finalize the digest calculation

        // Convert hash to string
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            char hex[3];
            sprintf(hex, "%02x", hash[i]);
            hashedString += hex;
        }

        EVP_MD_CTX_free(mdctx);

        return hashedString;
    }
}