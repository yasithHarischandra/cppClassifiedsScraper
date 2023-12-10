#pragma once
#include "Persistance_Base.h"

class Persistance_Database :
    public Persistance_Base
{
public:
    Persistance_Database(const std::string aConfigFileName, std::string aDataSourceType);
    Persistance_Database() = delete;
};

