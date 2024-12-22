#pragma once

#include <string>
#include <chrono>
#include <map>
#include <vector>
#include <boost/any.hpp>

class Persistance_Base;

struct ClassifiedTableData
{
public:
	std::string myTableName;
	std::map<std::string, boost::any> myTableAttributes;
};

class Classified_Base
{
protected:
	std::string myPriceStr;	//save full price string
	int myPrice;	//saves price as an int. if no price given = -1
	std::string myCity;
	std::string myContactNo;
	std::string myDetails;
	std::string myUrl;
	std::chrono::year_month_day myDate;

public:
	Classified_Base(std::string aPriceStr, std::string aCity, std::string aContactNo, std::string details, std::string aUrl, std::chrono::year_month_day aDate, int aPrice = -1) :
		myPriceStr(aPriceStr), myCity(aCity), myContactNo(aContactNo), myDetails(details), myUrl(aUrl), myDate(aDate), myPrice(aPrice) {};
	~Classified_Base() {};

	std::chrono::year_month_day Date() { return myDate; };
	std::string PriceStr() const { return myPriceStr; };
	int Price() const { return myPrice; };
	std::string DateStr() const;
	std::string ContactNo() const { return myContactNo; };
	std::string Details() const { return myDetails; };

	virtual std::vector<ClassifiedTableData> GetProperties() = 0;

	virtual std::string write(Persistance_Base* aDatasource) const = 0;	//visitor pattern
};

