#pragma once

#include <iostream>
#include <deque>
#include <string>

#include "transport_catalogue.h"

class StatReader
{
public:
    void GetQueries(std::istream & iStream, std::ostream &out , TransportCatalogue &catalogue);

private:

};
