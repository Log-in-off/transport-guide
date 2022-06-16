#pragma once

#include <iostream>
#include <deque>
#include <string>

#include "transport_catalogue.h"

namespace TG
{
namespace input
{

class InputReader
{
public:
    void GetQueries(std::istream & iStream, catalogue::TransportCatalogue &catalogue);
private:
    void GetRequests(std::istream &iStream, std::deque<domain::Requst> &stops, std::deque<domain::Requst> &buses);
};

}
}
