#pragma once
#include "Classified_Base.h"
class Classified_Vehicle :
    public Classified_Base
{
protected:
	std::string myMake;
	std::string myModel;
	int myYoM;
	int myMileage;
	std::string myTransmission;
	std::string myFuelType;
	std::string myType;
	std::string myStartType;
	std::string myOptions;

public:
	Classified_Vehicle(std::string aPrice, std::string aCity, std::string aContactNo, std::string details, std::string aUrl, std::chrono::year_month_day aDate,
		std::string aMake, std::string aModel, int aYoM, int aMileage, std::string aTransmission, std::string aFuelType, std::string aType, std::string aStartType, 
		std::string options);

	std::vector<ClassifiedTableData> GetProperties() override;
};

