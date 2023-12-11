#include "Persistance_Postgresql.h"

#include <spdlog/spdlog.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

bool Persistance_Postgresql::readConfigData(const std::string aConfigFileName, std::map<std::string, std::string>& dataSourceProperties)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(aConfigFileName, pt);

	dataSourceProperties["host"] = pt.get<std::string>("postgresql.host");
	dataSourceProperties["port"] = pt.get<std::string>("postgresql.port");
	dataSourceProperties["database"] = pt.get<std::string>("postgresql.database");
	dataSourceProperties["user"] = pt.get<std::string>("postgresql.user");
	dataSourceProperties["password"] = pt.get<std::string>("postgresql.password");

	return true;
}

bool Persistance_Postgresql::openDataSource()
{
	return false;
}

bool Persistance_Postgresql::closeDataSource()
{
	return false;
}

Persistance_Postgresql::Persistance_Postgresql(const std::string aConfigFileName) :
	Persistance_Database()
{
	//First step - read properties from ini file
	std::map<std::string, std::string> dataSourceProperties;
	try
	{
		if (!readConfigData(aConfigFileName, dataSourceProperties))
		{
			spdlog::error("Error reading data source configuration file.");
			myOpenStatus = false;
			return;
		}
	}
	catch (const std::exception& e)
	{
		spdlog::error("Error reading data source configuration file. - " + std::string(e.what()));
		myOpenStatus = false;
		return;
	}


}
