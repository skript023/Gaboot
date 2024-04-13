/**
 *
 *  Orders.h
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
class MasterCustomers;
class OrderDetails;

class Orders
{
  public:
    struct Cols
    {
        static const std::string _id;
        static const std::string _name;
        static const std::string _customer_id;
        static const std::string _total_price;
        static const std::string _discount;
        static const std::string _grand_total;
        static const std::string _total_item;
        static const std::string _status;
        static const std::string _expired;
        static const std::string _created_at;
        static const std::string _updated_at;
    };

    static const int primaryKeyNumber;
    static const std::string tableName;
    static const bool hasPrimaryKey;
    static const std::string primaryKeyName;
    using PrimaryKeyType = std::string;
    const PrimaryKeyType &getPrimaryKey() const;

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column names,
     * otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select all
     * columns by an asterisk), please set the offset to -1.
     */
    explicit Orders(const drogon::orm::Row &r, const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit Orders(const Json::Value &pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    Orders(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);

    Orders() = default;

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
    const std::string &getValueOfId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getId() const noexcept;
    ///Set the value of the column id
    void setId(const std::string &pId) noexcept;
    void setId(std::string &&pId) noexcept;

    /**  For column name  */
    ///Get the value of the column name, returns the default value if the column is null
    const std::string &getValueOfName() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getName() const noexcept;
    ///Set the value of the column name
    void setName(const std::string &pName) noexcept;
    void setName(std::string &&pName) noexcept;
    void setNameToNull() noexcept;

    /**  For column customer_id  */
    ///Get the value of the column customer_id, returns the default value if the column is null
    const std::string &getValueOfCustomerId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getCustomerId() const noexcept;
    ///Set the value of the column customer_id
    void setCustomerId(const std::string &pCustomerId) noexcept;
    void setCustomerId(std::string &&pCustomerId) noexcept;

    /**  For column total_price  */
    ///Get the value of the column total_price, returns the default value if the column is null
    const double &getValueOfTotalPrice() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<double> &getTotalPrice() const noexcept;
    ///Set the value of the column total_price
    void setTotalPrice(const double &pTotalPrice) noexcept;
    void setTotalPriceToNull() noexcept;

    /**  For column discount  */
    ///Get the value of the column discount, returns the default value if the column is null
    const double &getValueOfDiscount() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<double> &getDiscount() const noexcept;
    ///Set the value of the column discount
    void setDiscount(const double &pDiscount) noexcept;
    void setDiscountToNull() noexcept;

    /**  For column grand_total  */
    ///Get the value of the column grand_total, returns the default value if the column is null
    const double &getValueOfGrandTotal() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<double> &getGrandTotal() const noexcept;
    ///Set the value of the column grand_total
    void setGrandTotal(const double &pGrandTotal) noexcept;
    void setGrandTotalToNull() noexcept;

    /**  For column total_item  */
    ///Get the value of the column total_item, returns the default value if the column is null
    const int32_t &getValueOfTotalItem() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getTotalItem() const noexcept;
    ///Set the value of the column total_item
    void setTotalItem(const int32_t &pTotalItem) noexcept;
    void setTotalItemToNull() noexcept;

    /**  For column status  */
    ///Get the value of the column status, returns the default value if the column is null
    const std::string &getValueOfStatus() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getStatus() const noexcept;
    ///Set the value of the column status
    void setStatus(const std::string &pStatus) noexcept;
    void setStatus(std::string &&pStatus) noexcept;
    void setStatusToNull() noexcept;

    /**  For column expired  */
    ///Get the value of the column expired, returns the default value if the column is null
    const std::string &getValueOfExpired() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getExpired() const noexcept;
    ///Set the value of the column expired
    void setExpired(const std::string &pExpired) noexcept;
    void setExpired(std::string &&pExpired) noexcept;
    void setExpiredToNull() noexcept;

    /**  For column created_at  */
    ///Get the value of the column created_at, returns the default value if the column is null
    const ::trantor::Date &getValueOfCreatedAt() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getCreatedAt() const noexcept;
    ///Set the value of the column created_at
    void setCreatedAt(const ::trantor::Date &pCreatedAt) noexcept;

    /**  For column updated_at  */
    ///Get the value of the column updated_at, returns the default value if the column is null
    const ::trantor::Date &getValueOfUpdatedAt() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getUpdatedAt() const noexcept;
    ///Set the value of the column updated_at
    void setUpdatedAt(const ::trantor::Date &pUpdatedAt) noexcept;


    static size_t getColumnNumber() noexcept {  return 11;  }
    static const std::string &getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;
    /// Relationship interfaces
    MasterCustomers getMaster_customers(const drogon::orm::DbClientPtr &clientPtr) const;
    void getMaster_customers(const drogon::orm::DbClientPtr &clientPtr,
                             const std::function<void(MasterCustomers)> &rcb,
                             const drogon::orm::ExceptionCallback &ecb) const;
    std::vector<OrderDetails> getOrder_details(const drogon::orm::DbClientPtr &clientPtr) const;
    void getOrder_details(const drogon::orm::DbClientPtr &clientPtr,
                          const std::function<void(std::vector<OrderDetails>)> &rcb,
                          const drogon::orm::ExceptionCallback &ecb) const;
  private:
    friend drogon::orm::Mapper<Orders>;
    friend drogon::orm::BaseBuilder<Orders, true, true>;
    friend drogon::orm::BaseBuilder<Orders, true, false>;
    friend drogon::orm::BaseBuilder<Orders, false, true>;
    friend drogon::orm::BaseBuilder<Orders, false, false>;
#ifdef __cpp_impl_coroutine
    friend drogon::orm::CoroMapper<Orders>;
#endif
    static const std::vector<std::string> &insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;
    ///For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<std::string> id_;
    std::shared_ptr<std::string> name_;
    std::shared_ptr<std::string> customerId_;
    std::shared_ptr<double> totalPrice_;
    std::shared_ptr<double> discount_;
    std::shared_ptr<double> grandTotal_;
    std::shared_ptr<int32_t> totalItem_;
    std::shared_ptr<std::string> status_;
    std::shared_ptr<std::string> expired_;
    std::shared_ptr<::trantor::Date> createdAt_;
    std::shared_ptr<::trantor::Date> updatedAt_;
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
    bool dirtyFlag_[11]={ false };
  public:
    static const std::string &sqlForFindingByPrimaryKey()
    {
        static const std::string sql="select * from " + tableName + " where id = $1";
        return sql;
    }

    static const std::string &sqlForDeletingByPrimaryKey()
    {
        static const std::string sql="delete from " + tableName + " where id = $1";
        return sql;
    }
    std::string sqlForInserting(bool &needSelection) const
    {
        std::string sql="insert into " + tableName + " (";
        size_t parametersCount = 0;
        needSelection = false;
        sql += "id,";
        ++parametersCount;
        if(!dirtyFlag_[0])
        {
            needSelection=true;
        }
        if(dirtyFlag_[1])
        {
            sql += "name,";
            ++parametersCount;
        }
        if(dirtyFlag_[2])
        {
            sql += "customer_id,";
            ++parametersCount;
        }
        if(dirtyFlag_[3])
        {
            sql += "total_price,";
            ++parametersCount;
        }
        sql += "discount,";
        ++parametersCount;
        if(!dirtyFlag_[4])
        {
            needSelection=true;
        }
        sql += "grand_total,";
        ++parametersCount;
        if(!dirtyFlag_[5])
        {
            needSelection=true;
        }
        sql += "total_item,";
        ++parametersCount;
        if(!dirtyFlag_[6])
        {
            needSelection=true;
        }
        if(dirtyFlag_[7])
        {
            sql += "status,";
            ++parametersCount;
        }
        if(dirtyFlag_[8])
        {
            sql += "expired,";
            ++parametersCount;
        }
        sql += "created_at,";
        ++parametersCount;
        if(!dirtyFlag_[9])
        {
            needSelection=true;
        }
        sql += "updated_at,";
        ++parametersCount;
        if(!dirtyFlag_[10])
        {
            needSelection=true;
        }
        if(parametersCount > 0)
        {
            sql[sql.length()-1]=')';
            sql += " values (";
        }
        else
            sql += ") values (";

        int placeholder=1;
        char placeholderStr[64];
        size_t n=0;
        if(dirtyFlag_[0])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        else
        {
            sql +="default,";
        }
        if(dirtyFlag_[1])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[2])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[3])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[4])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        else
        {
            sql +="default,";
        }
        if(dirtyFlag_[5])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        else
        {
            sql +="default,";
        }
        if(dirtyFlag_[6])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        else
        {
            sql +="default,";
        }
        if(dirtyFlag_[7])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[8])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        if(dirtyFlag_[9])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        else
        {
            sql +="default,";
        }
        if(dirtyFlag_[10])
        {
            n = snprintf(placeholderStr,sizeof(placeholderStr),"$%d,",placeholder++);
            sql.append(placeholderStr, n);
        }
        else
        {
            sql +="default,";
        }
        if(parametersCount > 0)
        {
            sql.resize(sql.length() - 1);
        }
        if(needSelection)
        {
            sql.append(") returning *");
        }
        else
        {
            sql.append(1, ')');
        }
        LOG_TRACE << sql;
        return sql;
    }
};
} // namespace gaboot
} // namespace drogon_model
