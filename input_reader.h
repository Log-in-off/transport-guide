#pragma once

#include <iostream>
#include <deque>
#include <string>

#include "transport_catalogue.h"

class InputReader
{
public:
    void GetQueries(std::istream & iStream, TransportCatalogue &catalogue);

private:

};
