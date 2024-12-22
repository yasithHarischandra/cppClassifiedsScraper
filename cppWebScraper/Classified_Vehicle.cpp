#include "Classified_Vehicle.h"
#include "Persistance_Base.h"

Classified_Vehicle::Classified_Vehicle(std::string aPrice, std::string aCity, std::string aContactNo, std::string details, std::string aUrl, std::chrono::year_month_day aDate, std::string aMake, std::string aModel, 
	int aYoM, int aMileage, std::string aTransmission, std::string aFuelType, std::string aType, std::string aStartType, std::string options, int anEngineCapacity, int aPriceInt, bool hasLeasing)
	: Classified_Base(aPrice, aCity, aContactNo, details, aUrl, aDate, aPriceInt), myMake(aMake), myModel(aModel), myYoM(aYoM), myMileage(aMileage), myTransmission(aTransmission), myFuelType(aFuelType), 
	myType(aType), myStartType(aStartType), myOptions(options), myEngineCapacity(anEngineCapacity), myHasLeasing(hasLeasing)
{
}

std::vector<ClassifiedTableData> Classified_Vehicle::GetProperties()
{
	return std::vector<ClassifiedTableData>();
}

std::string Classified_Vehicle::write(Persistance_Base* aDatasource) const
{
	return aDatasource->WriteData(*this);
}


