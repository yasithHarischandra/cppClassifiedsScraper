#pragma once

#include <string>
class Persistance_Base
{
protected:
	//stores whether connected to data source successfully
	bool myOpenStatus;

	const std::string myConfigFileName;

	const std::string myDataSourceType;

	virtual bool openDataSource() = 0;

	virtual bool closeDataSource() = 0;

	virtual bool readConfigData() = 0;

public:
	Persistance_Base(const std::string aConfigFileName, std::string aDataSourceType);
	//check if data source is already open
	bool IsOpen();
};

