#include "Persistance_Base.h"

Persistance_Base::Persistance_Base() : myOpenStatus(false)
{
}

bool Persistance_Base::IsOpen()
{
    return myOpenStatus;
}
