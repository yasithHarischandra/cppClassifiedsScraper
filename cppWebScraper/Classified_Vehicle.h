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
	int myEngineCapacity;
	std::string myType;
	std::string myStartType;
	std::string myOptions;
	bool myHasLeasing;

public:
	Classified_Vehicle(std::string aPrice, std::string aCity, std::string aContactNo, std::string details, std::string aUrl, std::chrono::year_month_day aDate,
		std::string aMake, std::string aModel, int aYoM, int aMileage, std::string aTransmission, std::string aFuelType, std::string aType, std::string aStartType, 
		std::string options, int anEngineCapacity, int aPriceInt, bool hasLeasing);

	std::vector<ClassifiedTableData> GetProperties() override;

	std::string write(Persistance_Base* aDatasource) const override;

	std::string City() const { return myCity; };
	std::string Maker() const { return myMake; };
	std::string Model() const { return myModel; };
	std::string Type() const { return myType; };
	std::string Url() const { return myUrl; };
	int Year() const { return myYoM; };
	int Mileage() const { return myMileage; };
	std::string Transmission() const { return myTransmission; };
	std::string FuelType() const { return myFuelType; };
	int EngineCapacity() const { return myEngineCapacity; };
	std::string StartType() const { return myStartType; };
	std::string Options() const { return myOptions; };
	bool HasLeasing() const { return myHasLeasing; };

};

