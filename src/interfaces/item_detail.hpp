#pragma once
#include <pch.h>

#include "fwddec.hpp"

namespace gaboot
{
    struct item_detail
    {
        int64_t m_id;
        double m_price;
        uint64_t m_quantity;
        std::string m_name;

        void from_json(Json::Value const& json)
        {
            auto& data = json["product"];

            for (auto& product : data)
            {
                this->m_id = product["id"].asInt64();
                this->m_name = product["name"].asString();
                this->m_price = product["price"].asDouble();
                this->m_quantity = product["quantity"].asInt64();

                items.push_back(this);
            }
        }

        nlohmann::json to_json()
        {
            nlohmann::json json;

            for (auto& item : items)
            {
                json.push_back({
                    {"id", item->m_id},
                    {"name", item->m_name},
                    {"price", item->m_price},
                    {"quantity", item->m_quantity}
                });
            }

            return json;
        }
    private:
        std::vector<item_detail*> items;
    };
}