#pragma once
#include <pch.h>

#include "fwddec.hpp"

namespace gaboot
{
    struct item_detail
    {
        int64_t id;
        double price;
        uint64_t quantity;
        std::string name;

        void from_json(Json::Value const& json)
        {
            auto& data = json["product"];

            for (auto& product : data)
            {
                this->id = product["id"].asInt64();
                this->name = product["name"].asString();
                this->price = product["price"].asDouble();
                this->quantity = product["quantity"].asInt64();

                items.push_back(this);
            }
        }

        nlohmann::json to_json()
        {
            nlohmann::json json;

            for (auto& item : items)
            {
                json.push_back({
                    {"id", item->id},
                    {"name", item->name},
                    {"price", item->price},
                    {"quantity", item->quantity}
                });
            }

            return json;
        }
    private:
        std::vector<item_detail*> items;
    };
}