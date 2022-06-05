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
    void GetReques(std::istream &iStream, std::deque<catalogue::Requst> &stops, std::deque<catalogue::Requst> &buses);
};

}
}
