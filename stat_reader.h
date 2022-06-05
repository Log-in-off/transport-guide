#pragma once

#include <iostream>
#include <deque>
#include <string>

#include "transport_catalogue.h"

namespace TG
{
namespace stat
{

class StatReader
{
public:
    void GetQueries(std::istream & iStream, std::ostream &out , catalogue::TransportCatalogue &catalogue);
};

}
}

