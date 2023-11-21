/**
 *
 *  Payments.h
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

class Payments
{
  public:
    struct Cols
    {
        static const std::string _id;
        static const std::string _name;
        static const std::string _transactionId;
        static const std::string _merchantId;
        static const std::string _paymentType;
        static const std::string _description;
        static const std::string _transactionTime;
        static const std::string _transactionStatus;
        static const std::string _fraudStatus;
        static const std::string _bank;
        static const std::string _vaNumber;
        static const std::string _currency;
        static const std::string _expiry_time;
        static const std::string _createdAt;
        static const std::string _updatedAt;
    };

    const static int primaryKeyNumber;
    const static std::string tableName;
    const static bool hasPrimaryKey;
    const static std::string primaryKeyName;
    using PrimaryKeyType = int32_t;
    const PrimaryKeyType &getPrimaryKey() const;

    /**
     * @brief constructor
     * @param r One row of records in the SQL query result.
     * @param indexOffset Set the offset to -1 to access all columns by column names,
     * otherwise access all columns by offsets.
     * @note If the SQL is not a style of 'select * from table_name ...' (select all
     * columns by an asterisk), please set the offset to -1.
     */
    explicit Payments(const drogon::orm::Row &r, const ssize_t indexOffset = 0) noexcept;

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     */
    explicit Payments(const Json::Value &pJson) noexcept(false);

    /**
     * @brief constructor
     * @param pJson The json object to construct a new instance.
     * @param pMasqueradingVector The aliases of table columns.
     */
    Payments(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false);

    Payments() = default;

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
    const int32_t &getValueOfId() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<int32_t> &getId() const noexcept;
    ///Set the value of the column id
    void setId(const int32_t &pId) noexcept;

    /**  For column name  */
    ///Get the value of the column name, returns the default value if the column is null
    const std::string &getValueOfName() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getName() const noexcept;
    ///Set the value of the column name
    void setName(const std::string &pName) noexcept;
    void setName(std::string &&pName) noexcept;
    void setNameToNull() noexcept;

    /**  For column transactionId  */
    ///Get the value of the column transactionId, returns the default value if the column is null
    const std::string &getValueOfTransactionid() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getTransactionid() const noexcept;
    ///Set the value of the column transactionId
    void setTransactionid(const std::string &pTransactionid) noexcept;
    void setTransactionid(std::string &&pTransactionid) noexcept;
    void setTransactionidToNull() noexcept;

    /**  For column merchantId  */
    ///Get the value of the column merchantId, returns the default value if the column is null
    const std::string &getValueOfMerchantid() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getMerchantid() const noexcept;
    ///Set the value of the column merchantId
    void setMerchantid(const std::string &pMerchantid) noexcept;
    void setMerchantid(std::string &&pMerchantid) noexcept;
    void setMerchantidToNull() noexcept;

    /**  For column paymentType  */
    ///Get the value of the column paymentType, returns the default value if the column is null
    const std::string &getValueOfPaymenttype() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getPaymenttype() const noexcept;
    ///Set the value of the column paymentType
    void setPaymenttype(const std::string &pPaymenttype) noexcept;
    void setPaymenttype(std::string &&pPaymenttype) noexcept;
    void setPaymenttypeToNull() noexcept;

    /**  For column description  */
    ///Get the value of the column description, returns the default value if the column is null
    const std::string &getValueOfDescription() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getDescription() const noexcept;
    ///Set the value of the column description
    void setDescription(const std::string &pDescription) noexcept;
    void setDescription(std::string &&pDescription) noexcept;
    void setDescriptionToNull() noexcept;

    /**  For column transactionTime  */
    ///Get the value of the column transactionTime, returns the default value if the column is null
    const std::string &getValueOfTransactiontime() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getTransactiontime() const noexcept;
    ///Set the value of the column transactionTime
    void setTransactiontime(const std::string &pTransactiontime) noexcept;
    void setTransactiontime(std::string &&pTransactiontime) noexcept;
    void setTransactiontimeToNull() noexcept;

    /**  For column transactionStatus  */
    ///Get the value of the column transactionStatus, returns the default value if the column is null
    const std::string &getValueOfTransactionstatus() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getTransactionstatus() const noexcept;
    ///Set the value of the column transactionStatus
    void setTransactionstatus(const std::string &pTransactionstatus) noexcept;
    void setTransactionstatus(std::string &&pTransactionstatus) noexcept;
    void setTransactionstatusToNull() noexcept;

    /**  For column fraudStatus  */
    ///Get the value of the column fraudStatus, returns the default value if the column is null
    const std::string &getValueOfFraudstatus() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getFraudstatus() const noexcept;
    ///Set the value of the column fraudStatus
    void setFraudstatus(const std::string &pFraudstatus) noexcept;
    void setFraudstatus(std::string &&pFraudstatus) noexcept;
    void setFraudstatusToNull() noexcept;

    /**  For column bank  */
    ///Get the value of the column bank, returns the default value if the column is null
    const std::string &getValueOfBank() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getBank() const noexcept;
    ///Set the value of the column bank
    void setBank(const std::string &pBank) noexcept;
    void setBank(std::string &&pBank) noexcept;
    void setBankToNull() noexcept;

    /**  For column vaNumber  */
    ///Get the value of the column vaNumber, returns the default value if the column is null
    const std::string &getValueOfVanumber() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getVanumber() const noexcept;
    ///Set the value of the column vaNumber
    void setVanumber(const std::string &pVanumber) noexcept;
    void setVanumber(std::string &&pVanumber) noexcept;
    void setVanumberToNull() noexcept;

    /**  For column currency  */
    ///Get the value of the column currency, returns the default value if the column is null
    const std::string &getValueOfCurrency() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getCurrency() const noexcept;
    ///Set the value of the column currency
    void setCurrency(const std::string &pCurrency) noexcept;
    void setCurrency(std::string &&pCurrency) noexcept;
    void setCurrencyToNull() noexcept;

    /**  For column expiry_time  */
    ///Get the value of the column expiry_time, returns the default value if the column is null
    const std::string &getValueOfExpiryTime() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<std::string> &getExpiryTime() const noexcept;
    ///Set the value of the column expiry_time
    void setExpiryTime(const std::string &pExpiryTime) noexcept;
    void setExpiryTime(std::string &&pExpiryTime) noexcept;
    void setExpiryTimeToNull() noexcept;

    /**  For column createdAt  */
    ///Get the value of the column createdAt, returns the default value if the column is null
    const ::trantor::Date &getValueOfCreatedat() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getCreatedat() const noexcept;
    ///Set the value of the column createdAt
    void setCreatedat(const ::trantor::Date &pCreatedat) noexcept;

    /**  For column updatedAt  */
    ///Get the value of the column updatedAt, returns the default value if the column is null
    const ::trantor::Date &getValueOfUpdatedat() const noexcept;
    ///Return a shared_ptr object pointing to the column const value, or an empty shared_ptr object if the column is null
    const std::shared_ptr<::trantor::Date> &getUpdatedat() const noexcept;
    ///Set the value of the column updatedAt
    void setUpdatedat(const ::trantor::Date &pUpdatedat) noexcept;


    static size_t getColumnNumber() noexcept {  return 15;  }
    static const std::string &getColumnName(size_t index) noexcept(false);

    Json::Value toJson() const;
    Json::Value toMasqueradedJson(const std::vector<std::string> &pMasqueradingVector) const;
    /// Relationship interfaces
  private:
    friend drogon::orm::Mapper<Payments>;
    friend drogon::orm::BaseBuilder<Payments, true, true>;
    friend drogon::orm::BaseBuilder<Payments, true, false>;
    friend drogon::orm::BaseBuilder<Payments, false, true>;
    friend drogon::orm::BaseBuilder<Payments, false, false>;
#ifdef __cpp_impl_coroutine
    friend drogon::orm::CoroMapper<Payments>;
#endif
    static const std::vector<std::string> &insertColumns() noexcept;
    void outputArgs(drogon::orm::internal::SqlBinder &binder) const;
    const std::vector<std::string> updateColumns() const;
    void updateArgs(drogon::orm::internal::SqlBinder &binder) const;
    ///For mysql or sqlite3
    void updateId(const uint64_t id);
    std::shared_ptr<int32_t> id_;
    std::shared_ptr<std::string> name_;
    std::shared_ptr<std::string> transactionid_;
    std::shared_ptr<std::string> merchantid_;
    std::shared_ptr<std::string> paymenttype_;
    std::shared_ptr<std::string> description_;
    std::shared_ptr<std::string> transactiontime_;
    std::shared_ptr<std::string> transactionstatus_;
    std::shared_ptr<std::string> fraudstatus_;
    std::shared_ptr<std::string> bank_;
    std::shared_ptr<std::string> vanumber_;
    std::shared_ptr<std::string> currency_;
    std::shared_ptr<std::string> expiryTime_;
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
    bool dirtyFlag_[15]={ false };
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
            sql += "name,";
            ++parametersCount;
        }
        if(dirtyFlag_[2])
        {
            sql += "transactionId,";
            ++parametersCount;
        }
        if(dirtyFlag_[3])
        {
            sql += "merchantId,";
            ++parametersCount;
        }
        if(dirtyFlag_[4])
        {
            sql += "paymentType,";
            ++parametersCount;
        }
        if(dirtyFlag_[5])
        {
            sql += "description,";
            ++parametersCount;
        }
        if(dirtyFlag_[6])
        {
            sql += "transactionTime,";
            ++parametersCount;
        }
        if(dirtyFlag_[7])
        {
            sql += "transactionStatus,";
            ++parametersCount;
        }
        if(dirtyFlag_[8])
        {
            sql += "fraudStatus,";
            ++parametersCount;
        }
        if(dirtyFlag_[9])
        {
            sql += "bank,";
            ++parametersCount;
        }
        if(dirtyFlag_[10])
        {
            sql += "vaNumber,";
            ++parametersCount;
        }
        if(dirtyFlag_[11])
        {
            sql += "currency,";
            ++parametersCount;
        }
        if(dirtyFlag_[12])
        {
            sql += "expiry_time,";
            ++parametersCount;
        }
        if(dirtyFlag_[13])
        {
            sql += "createdAt,";
            ++parametersCount;
        }
        if(dirtyFlag_[14])
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
        if(dirtyFlag_[5])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[6])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[7])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[8])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[9])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[10])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[11])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[12])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[13])
        {
            sql.append("?,");

        }
        if(dirtyFlag_[14])
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
