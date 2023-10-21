#pragma once
#include <pch.h>

namespace gaboot::master_customer_schema
{
    static nlohmann::json customer_schema = R"(
    {
        "$schema": "http://json-schema.org/draft-07/schema#",
        "title": "Customer Schema",
        "properties": {
            "firstname": {
                "description": "First Name",
                "type": "string",
                "minLength": 3,
                "maxLength": 127,
                "pattern": "^[A-Za-z]+$"
            },
            "lastname": {
                "description": "Last Name",
                "type": "string",
                "minLength": 3,
                "maxLength": 127,
                "pattern": "^[A-Za-z]+$"
            },
            "username": {
                "description": "Last Name",
                "type": "string",
                "minLength": 3,
                "maxLength": 127,
                "pattern": "^[A-Za-z0-9]+$"
            },
            "email": {
                "description": "Last Name",
                "type": "string",
                "pattern": "^\\S+@\\S+\\.\\S+$",
                "minLength": 5,
                "maxLength": 127
            },
            "phoneNumber": {
                "description": "Last Name",
                "type": ["string", "null"]
            },
            "addressDetail": {
                "description": "Last Name",
                "type": ["string", "null"]
            },
            "latitude": {
                "description": "Last Name",
                "type": ["string", "null"]
            },
            "longitude": {
                "description": "Last Name",
                "type": ["string", "null"]
            },
            "password": {
                "description": "Last Name",
                "type": "string"
            },
            "token": {
                "description": "Last Name",
                "type": ["string", "null"]
            },
            "isActive": {
                "description": "Last Name",
                "type": "number"
            },
            "imagePath": {
                "description": "Image Location",
                "type": ["string", "null"]
            },
            "thumbnailPath": {
                "description": "Thumbnail Location",
                "type": ["string", "null"]
            },
            "createdAt": {
                "description": "Created date",
                "type": "string"
            },
            "updatedAt": {
                "description": "Updated date",
                "type": "string"
            }
        },
        "required": [
            "firstname",
            "lastname",
            "username",
            "email",
            "password"
        ],
        "type": "object"
    })"_json;
}