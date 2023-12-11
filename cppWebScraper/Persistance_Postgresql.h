#pragma once
#include "Persistance_Database.h"

#include <pqxx/pqxx> 

class Persistance_Postgresql :
    public Persistance_Database
{
protected:
    std::unique_ptr <pqxx::connection> myDB;
    bool readConfigData(const std::string aConfigFileName, std::map<std::string, std::string>& dataSourceProperties) override;
    bool openDataSource(const std::map<std::string, std::string>& dataSourceProperties) override;
    bool closeDataSource() override;

public:
    bool IsOpen() override;
    Persistance_Postgresql(const std::string aConfigFileName);
    ~Persistance_Postgresql();
};

