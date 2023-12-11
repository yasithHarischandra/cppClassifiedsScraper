#pragma once
#include "Persistance_Database.h"

class Persistance_Postgresql :
    public Persistance_Database
{
protected:
    bool readConfigData(const std::string aConfigFileName, std::map<std::string, std::string>& dataSourceProperties) override;
    bool openDataSource() override;
    bool closeDataSource() override;

public:
    Persistance_Postgresql(const std::string aConfigFileName);
};

