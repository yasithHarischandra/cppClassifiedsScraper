/*  This is a web scraper written in C++ to extract classifieds selling vehicles and save them to a database
*   Initially data will be read from riyasewana.com and will be saved  to a postgres database.
*   Later it may be extended to other classifieds sites as well
*   Written by - Yasith Harischandra
*   This file contains main function
*/

#include <iostream>
#include <memory>
#include "Scraper_Riyasewana.h"
#include "Persistance_Postgresql.h"

int main()
{
    //std::cout << "Hello World!\n";
    std::unique_ptr<Persistance_Base> aDataSource = std::make_unique<Persistance_Postgresql>("database.ini");
    if (!aDataSource->IsOpen())
        return -1;
    auto aWebScraper = std::make_unique<Scraper_Riyasewana>(aDataSource.get());

    aWebScraper->ReadSiteFrontToBack();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
