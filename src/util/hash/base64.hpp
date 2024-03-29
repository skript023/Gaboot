#pragma once

#include <pch.h>

namespace gaboot::base64
{
    inline std::string encode(const std::string& input) 
    {
        const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        std::string encoded;
        int i = 0;
        int j = 0;
        uint8_t char_array_3[3];
        uint8_t char_array_4[4];

        for (char c : input) 
        {
            char_array_3[i++] = c;

            if (i == 3) 
            {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (i = 0; i < 4; i++) 
                {
                    encoded += base64_chars[char_array_4[i]];
                }

                i = 0;
            }
        }

        if (i > 0) 
        {
            for (j = i; j < 3; j++) 
            {
                char_array_3[j] = '\0';
            }

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

            for (j = 0; j < i + 1; j++) 
            {
                encoded += base64_chars[char_array_4[j]];
            }

            while (i++ < 3) 
            {
                encoded += '=';
            }
        }

        return encoded;
    }

    inline std::string decode(const std::string& encoded)
    {
        const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        std::string decoded;
        int i = 0;
        int j = 0;
        int in_len = encoded.size();
        uint8_t char_array_4[4];
        uint8_t char_array_3[3];

        while (in_len-- && (encoded[j] != '=') && (isalnum(encoded[j]) || (encoded[j] == '+') || (encoded[j] == '/'))) 
        {
            char_array_4[i++] = encoded[j++];

            if (i == 4) 
            {
                for (i = 0; i < 4; i++) 
                {
                    char_array_4[i] = base64_chars.find(char_array_4[i]);
                }

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; i < 3; i++) 
                {
                    decoded += char_array_3[i];
                }
                i = 0;
            }
        }

        if (i > 0)
        {
            for (j = i; j < 4; j++) 
            {
                char_array_4[j] = 0;
            }

            for (i = 0; i < 4; i++) 
            {
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

            for (j = 0; j < i - 1; j++) 
            {
                decoded += char_array_3[j];
            }
        }

        return decoded;
    }
}