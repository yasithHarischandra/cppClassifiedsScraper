#include "Persistance_Base.h"

Persistance_Base::Persistance_Base(const std::string aConfigFileName, std::string aDataSourceType) :
    myConfigFileName(aConfigFileName), myDataSourceType(aDataSourceType), myOpenStatus(false)
{
}

bool Persistance_Base::IsOpen()
{
    return myOpenStatus;
}
