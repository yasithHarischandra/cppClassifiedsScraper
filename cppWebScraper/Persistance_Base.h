#pragma once

#include "Classified_Vehicle.h"
#include <string>
#include <map>

class Persistance_Base
{
protected:
	//stores whether connected to data source successfully
	bool myOpenStatus;

	virtual bool openDataSource(const std::map<std::string, std::string>& dataSourceProperties) = 0;

	virtual bool closeDataSource() = 0;

	virtual bool readConfigData(const std::string aConfigFileName, std::map<std::string, std::string>& dataSourceProperties) = 0;

	

public:
	Persistance_Base();
	//check if data source is already open
	virtual bool IsOpen();

	virtual std::string WriteData(const Classified_Vehicle& anAd) = 0;
};

