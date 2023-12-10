#pragma once

#include <string>
#include <chrono>
#include <map>
#include <vector>
#include <boost/any.hpp>

struct ClassifiedTableData
{
public:
	std::string myTableName;
	std::map<std::string, boost::any> myTableAttributes;
};

class Classified_Base
{
protected:
	std::string myPrice;
	std::string myCity;
	std::string myContactNo;
	std::string myDetails;
	std::string myUrl;
	std::chrono::year_month_day myDate;

public:
	Classified_Base(std::string aPrice, std::string aCity, std::string aContactNo, std::string details, std::string aUrl, std::chrono::year_month_day aDate) :
		myPrice(aPrice), myCity(aCity), myContactNo(aContactNo), myDetails(details), myUrl(aUrl), myDate(aDate) {};
	~Classified_Base() {};

	std::chrono::year_month_day GetDate() { return myDate; };

	virtual std::vector<ClassifiedTableData> GetProperties() = 0;
};

