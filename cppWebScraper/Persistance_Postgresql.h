#pragma once
#include "Persistance_Database.h"
#include "Classified_Vehicle.h"

#include <pqxx/pqxx> 

class Persistance_Postgresql :
    public Persistance_Database
{
protected:
    std::unique_ptr <pqxx::connection> myDB;
    bool readConfigData(const std::string aConfigFileName, std::map<std::string, std::string>& dataSourceProperties) override;
    bool openDataSource(const std::map<std::string, std::string>& dataSourceProperties) override;
    bool closeDataSource() override;

    //database functions

    //table name should be in the format "schema.table"
    int insertRecordGetPK(const std::string anAtributeName, const std::string anAtributeValue, const std::string aTableName, const std::string aPK);
    int numRecordsAvailable(const std::string anAtributeName, const std::string anAtributeValue, const std::string aTableName);
    
public:
    std::string WriteData(const Classified_Vehicle& anAd) override;
    bool IsOpen() override;
    bool NewestClassifiedDate(std::chrono::year_month_day& aDate) override;
    Persistance_Postgresql(const std::string aConfigFileName);
    ~Persistance_Postgresql();
};

