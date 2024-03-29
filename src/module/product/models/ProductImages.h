/**
 *
 *  ProductImages.h
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#pragma once
#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/SqlBinder.h>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/BaseBuilder.h>
#ifdef __cpp_impl_coroutine
#include <drogon/orm/CoroMapper.h>
#endif
#include <trantor/utils/Date.h>
#include <trantor/utils/Logger.h>
#include <json/json.h>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <tuple>
#include <stdint.h>
#include <iostream>

namespace drogon
{
namespace orm
{
class DbClient;
using DbClientPtr = std::shared_ptr<DbClient>;
}
}
namespace drogon_model
{
namespace gaboot
{
class MasterProducts;

class ProductImages
{
  public:
    struct Cols
    {
        static const std::string _id;
        static const std::string _imagePath;
        static const std::string _thumbnailPath;
        static const std::string _productId;
        static const std::string _isCover;
        static const std::string _createdAt;
        static const std::string _updatedAt;
    };

    const static int primaryKeyNumber;
    const static std::string tableName;
    const static bool hasPrimaryKey;
    const static std::string primaryKeyName;
    using PrimaryKeyType = uint64_t;
    const PrimaryKeyType &getPrimaryKey() const;

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column names,
     * otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select all
     * columns by an asterisk), please set the offset to -1.
     */
    explicit ProductImages(const drogon::orm::Row &r, const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit ProductImages(const Json::Value &pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    ProductImages(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);

    ProductImages() = default;

    void updateByJson(const Json::Value &pJson) noexcept(false);
    void updateByMasqueradedJson(const Json::Value &pJson,
                                 const std::vector<std::string> &pMasqueradingVector) noexcept(false);
    static bool validateJsonForCreation(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForCreation(const Json::Value &,
                                                const std::vector<std::string> &pMasqueradingVector,
                                                    std::string &err);
    static bool validateJsonForUpdate(const Json::Value &pJson, std::string &err);
    static bool validateMasqueradedJsonForUpdate(const Json::Value &,
                                          const std::vector<std::string> &pMasqueradingVector,
                                          std::string &err);
    static bool validJsonOfField(size_t index,
                          const std::string &fieldName,
                          const Json::Value &pJson,
                          std::string &err,
                          bool isForCreation);

    /**  For column id  */
    ///Get the value of the column id, returns the default value if the column is null
    const uint64_t &getValueOfId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<uint64_t> &getId() const noexcept;
    ///Set the value of the column id
    void setId(const uint64_t &pId) noexcept;

    /**  For column imagePath  */
    ///Get the value of the column imagePath, returns the default value if the column is null
    const std::string &getValueOfImagepath() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getImagepath() const noexcept;
    ///Set the value of the column imagePath
    void setImagepath(const std::string &pImagepath) noexcept;
    void setImagepath(std::string &&pImagepath) noexcept;

    /**  For column thumbnailPath  */
    ///Get the value of the column thumbnailPath, returns the default value if the column is null
    const std::string &getValueOfThumbnailpath() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getThumbnailpath() const noexcept;
    ///Set the value of the column thumbnailPath
    void setThumbnailpath(const std::string &pThumbnailpath) noexcept;
    void setThumbnailpath(std::string &&pThumbnailpath) noexcept;

    /**  For column productId  */
    ///Get the value of the column productId, returns the default value if the column is null
    const int32_t &getValueOfProductid() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getProductid() const noexcept;
    ///Set the value of the column productId
    void setProductid(const int32_t &pProductid) noexcept;

    /**  For column isCover  */
    ///Get the value of the column isCover, returns the default value if the column is null
    const int8_t &getValueOfIscover() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int8_t> &getIscover() const noexcept;
    ///Set the value of the column isCover
    void setIscover(const int8_t &pIscover) noexcept;

    /**  For column createdAt  */
    ///Get the value of the column createdAt, returns the default value if the column is null
    const ::trantor::Date &getValueOfCreatedat() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getCreatedat() const noexcept;
    ///Set the value of the column createdAt
    void setCreatedat(const ::trantor::Date &pCreatedat) noexcept;
    void setCreatedatToNull() noexcept;

    /**  For column updatedAt  */
    ///Get the value of the column updatedAt, returns the default value if the column is null
    const ::trantor::Date &getValueOfUpdatedat() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getUpdatedat() const noexcept;
    ///Set the value of the column updatedAt
    void setUpdatedat(const ::trantor::Date &pUpdatedat) noexcept;
    void setUpdatedatToNull() noexcept;


    static size_t getColumnNumber() noexcept {  return 7;  }
    static const std::string &getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;
    /// Relationship interfaces
    std::vector<MasterProducts> getMaster_products(const drogon::orm::DbClientPtr &clientPtr) const;
    void getMaster_products(const drogon::orm::DbClientPtr &clientPtr,
                            const std::function<void(std::vector<MasterProducts>)> &rcb,
                            const drogon::orm::ExceptionCallback &ecb) const;
  private:
    friend drogon::orm::Mapper<ProductImages>;
    friend drogon::orm::BaseBuilder<ProductImages, true, true>;
    friend drogon::orm::BaseBuilder<ProductImages, true, false>;
    friend drogon::orm::BaseBuilder<ProductImages, false, true>;
    friend drogon::orm::BaseBuilder<ProductImages, false, false>;
#ifdef __cpp_impl_coroutine
    friend drogon::orm::CoroMapper<ProductImages>;
#endif
    static const std::vector<std::string> &insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;
    ///For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<uint64_t> id_;
    std::shared_ptr<std::string> imagepath_;
    std::shared_ptr<std::string> thumbnailpath_;
    std::shared_ptr<int32_t> productid_;
    std::shared_ptr<int8_t> iscover_;
    std::shared_ptr<::trantor::Date> createdat_;
    std::shared_ptr<::trantor::Date> updatedat_;
    struct MetaData
    {
        const std::string colName_;
        const std::string colType_;
        const std::string colDatabaseType_;
        const ssize_t colLength_;
        const bool isAutoVal_;
        const bool isPrimaryKey_;
        const bool notNull_;
    };
    static const std::vector<MetaData> metaData_;
    bool dirtyFlag_[7]={ false };
  public:
    static const std::string &sqlForFindingByPrimaryKey()
    {
        static const std::string sql="select * from " + tableName + " where id = ?";
        return sql;
    }

    static const std::string &sqlForDeletingByPrimaryKey()
    {
        static const std::string sql="delete from " + tableName + " where id = ?";
        return sql;
    }
    std::string sqlForInserting(bool &needSelection) const
    {
        std::string sql="insert into " + tableName + " (";
        size_t parametersCount = 0;
        needSelection = false;
            sql += "id,";
            ++parametersCount;
        if(dirtyFlag_[1])
        {
            sql += "imagePath,";
            ++parametersCount;
        }
        if(dirtyFlag_[2])
        {
            sql += "thumbnailPath,";
            ++parametersCount;
        }
        if(dirtyFlag_[3])
        {
            sql += "productId,";
            ++parametersCount;
        }
        sql += "isCover,";
        ++parametersCount;
        if(!dirtyFlag_[4])
        {
            needSelection=true;
        }
        if(dirtyFlag_[5])
        {
            sql += "createdAt,";
            ++parametersCount;
        }
        if(dirtyFlag_[6])
        {
            sql += "updatedAt,";
            ++parametersCount;
        }
        needSelection=true;
        if(parametersCount > 0)
        {
            sql[sql.length()-1]=')';
            sql += " values (";
        }
        else
            sql += ") values (";

        sql +="default,";
        if(dirtyFlag_[1])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[2])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[3])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[4])
        {
            sql.append("?,");

        }
        else
        {
            sql +="default,";
        }
        if(dirtyFlag_[5])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[6])
        {
            sql.append("?,");

        }
        if(parametersCount > 0)
        {
            sql.resize(sql.length() - 1);
        }
        sql.append(1, ')');
        LOG_TRACE << sql;
        return sql;
    }
};
} // namespace gaboot
} // namespace drogon_model
