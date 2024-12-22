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

bool Persistance_Postgresql::openDataSource(const std::map<std::string, std::string>& dataSourceProperties)
{
	std::string connectionStr{ "dbname = " + dataSourceProperties.at("database") + " user = " + dataSourceProperties.at("user") +
		" password = " + dataSourceProperties.at("password") + " host = " + dataSourceProperties.at("host") + " port = " + dataSourceProperties.at("port") };
	try
	{
		myDB = std::make_unique<pqxx::connection>(connectionStr);
	}
	catch (const std::exception& e)
	{
		spdlog::error("Failed to connect to PostgreSQL database. - " + std::string(e.what()));
		return false;
	}
	if (myDB->is_open())
		spdlog::info("Successfully connected to PostgreSQL Database.");
	else
		spdlog::error("Failed to connect to PostgreSQL database.");

	return myDB->is_open();
}

bool Persistance_Postgresql::closeDataSource()
{
	myDB->close();
	return myDB->is_open();
}

int Persistance_Postgresql::insertRecordGetPK(std::string anAtributeName, std::string anAtributeValue, std::string aTableName, std::string aPK)
{
	int primaryKey{ -1 };

	try
	{
		pqxx::work N{ *myDB.get() };
		//check if there is already a city record;
		std::string getIdSql{ "SELECT " + aPK + " FROM " + aTableName + " \
					   WHERE " + anAtributeName + " = '" + N.esc(anAtributeValue) + "'" };
		
		pqxx::result RCityId(N.exec(getIdSql));
		N.commit();

		if (RCityId.empty())
		{
			pqxx::work W{ *myDB.get() };

			std::string insertSql{ "INSERT INTO " + aTableName + "(" + anAtributeName + ") \
							VALUES('" + W.esc(anAtributeValue) + "') \
							ON CONFLICT(" + anAtributeName + ") DO NOTHING \
							RETURNING " + aPK };

			pqxx::result R(W.exec(insertSql));
			W.commit();

			if (R.empty())	//city was not inserted to table because it already exists, now get the city id
			{
				spdlog::error("Failed to read back city number from database.");
				return -1;
			}
			else
			{
				primaryKey = R.begin()[0].as<int>();
			}
		}
		else
		{
			primaryKey = RCityId.begin()[0].as<int>();
		}

	}
	catch (const std::exception& e)
	{
		spdlog::error("Failed to read back city number from database. - " + std::string(e.what()));
		return -1;
	}
	return primaryKey;
}

int Persistance_Postgresql::numRecordsAvailable(const std::string anAtributeName, const std::string anAtributeValue, const std::string aTableName)
{
	int numRecords{ 0 };
	
	try
	{
		pqxx::work N{ *myDB.get() };

		std::string getRecordSql{ "SELECT COUNT(" + anAtributeName + ") FROM " + aTableName + " \
					   WHERE " + anAtributeName + " = '" + N.esc(anAtributeValue) + "'" };

		pqxx::result resultRows(N.exec(getRecordSql));
		//N.commit();

		if (!resultRows.empty())
			numRecords = resultRows.begin()[0].as<int>();

	}
	catch (const std::exception& e)
	{
		spdlog::error("Failed checking for duplicates. - " + std::string(e.what()));
		return false;
	}
	return numRecords;
}

std::string Persistance_Postgresql::WriteData(const Classified_Vehicle& anAd)
{
	if (numRecordsAvailable("vehicle_ad_url", anAd.Url(), "classifieds.vehicle_ad") > 0)
		return "duplicate";

	int cityId{ insertRecordGetPK("city_name_en", anAd.City(), "classifieds.city", "city_id")};
	if (cityId < 0)
	{
		//throw std::invalid_argument("Error writing classified city to database. - " + anAd.City());
		spdlog::error("City - " + anAd.City());
		return "";
	}
		

	int makerId{ insertRecordGetPK("vehicle_maker", anAd.Maker(),"classifieds.vehicle_maker", "vehicle_maker_id") };
	if (makerId < 0)
		throw std::invalid_argument("Error writing vehicle manufacturer to database. - " + anAd.Maker());

	int typeId{ insertRecordGetPK("vehicle_type", anAd.Type(),"classifieds.vehicle_type", "vehicle_type_id") };
	if (typeId < 0)
		throw std::invalid_argument("Error writing vehicle type to database. - " + anAd.Type());

	////Now get ready for writing the record 
	try
	{
		pqxx::work W{ *myDB.get() };

		//SQL string
		std::string insertAdSql =
			"INSERT INTO classifieds.vehicle_ad\
		(\
			vehicle_type_id,\
			vehicle_maker_id,\
			vehicle_model,\
			vehicle_ad_year,\
			vehicle_ad_mileage,\
			vehicle_ad_transmission,\
			vehicle_ad_fuel_type,\
			vehicle_ad_engine_capacity,\
			vehicle_ad_start_type,\
			vehicle_ad_price,\
			vehicle_ad_url,\
			vehicle_ad_date,\
			city_id,\
			vehicle_ad_contactno,\
			vehicle_ad_options,\
			vehicle_ad_details\
		)\
		VALUES\
		("
			+ std::to_string(typeId) + ","
			+ std::to_string(makerId) + ",'"
			+ W.esc(anAd.Model()) + "',"
			+ std::to_string(anAd.Year()) + ","
			+ std::to_string(anAd.Mileage()) + ",'"
			+ W.esc(anAd.Transmission()) + "','"
			+ W.esc(anAd.FuelType()) + "',"
			+ std::to_string(anAd.EngineCapacity()) + ",'"
			+ W.esc(anAd.StartType()) + "','"
			+ W.esc(anAd.PriceStr()) + "','"
			+ W.esc(anAd.Url()) + "','"
			+ W.esc(anAd.DateStr()) + "',"
			+ std::to_string(cityId) + ",'"
			+ W.esc(anAd.ContactNo()) + "','"
			+ W.esc(anAd.Options()) + "','"
			+ W.esc(anAd.Details()) + "'" +
			")\
		ON CONFLICT(vehicle_ad_url) DO NOTHING\
		RETURNING vehicle_ad_id";
		
		W.exec(insertAdSql);
		W.commit();
	}
	catch (const std::exception& e)
	{
		spdlog::error("Failed writing to database. - " + std::string(e.what()));
		spdlog::error("URL - " + anAd.Url());
		return "failure";
	}
	
	return "success";
}

bool Persistance_Postgresql::IsOpen()
{
	if (myDB)
		return myDB->is_open();
	else
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
			return;
		}
	}
	catch (const std::exception& e)
	{
		spdlog::error("Error reading data source configuration file. - " + std::string(e.what()));
		return;
	}

	//Next - Open database connection
	openDataSource(dataSourceProperties);
}

Persistance_Postgresql::~Persistance_Postgresql()
{
	closeDataSource();
}
