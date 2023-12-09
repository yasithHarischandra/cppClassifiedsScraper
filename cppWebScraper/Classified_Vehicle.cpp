#include "Classified_Vehicle.h"

Classified_Vehicle::Classified_Vehicle(std::string aPrice, std::string aCity, std::string aContactNo, std::string details, std::string aUrl, std::chrono::year_month_day aDate, std::string aMake, std::string aModel, 
	int aYoM, int aMileage, std::string aTransmission, std::string aFuelType, std::string aType, std::string aStartType, std::string options)
	: Classified_Base(aPrice, aCity, aContactNo, details, aUrl, aDate), myMake(aMake), myModel(aModel), myYoM(aYoM), myMileage(aMileage), myTransmission(aTransmission), myFuelType(aFuelType), 
	myType(aType), myStartType(aStartType), myOptions(options)
{
}

std::vector<ClassifiedTableData> Classified_Vehicle::GetProperties()
{
	return std::vector<ClassifiedTableData>();
}
