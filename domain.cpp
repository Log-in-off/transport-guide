#include "domain.h"

namespace TG
{
namespace domain
{

Requst::Requst():text(""), start(text) {}
Requst::Requst(std::string textInput):text(textInput), start(text) {}
Requst::Requst(std::string text, std::string_view start):text(text), start(start) {}

BusInput::BusInput():name(""), request(""), stops({}){}
BusInput::BusInput(std::string name, std::string request, std::vector <std::string_view> stops):name(name), request(request), stops(stops){}

Bus::Bus():name(""), stops({}), countUnicStops(0), distance(0), curvature(0), isRoundtrip(false){}
Bus::Bus(std::string name, std::vector <Stop*> stops, uint32_t countUnicStops, uint32_t distance, double_t curvature, bool isRoundtrip)
               :name(name), stops(stops), countUnicStops(countUnicStops), distance(distance), curvature(curvature), isRoundtrip(isRoundtrip){}

BusInfo::BusInfo():name(""), countStops(0), countUnicStops(0), distance(0), curvature(0) {}
BusInfo::BusInfo(std::string name, uint32_t countStops, uint32_t countUnicStops, uint32_t distance, double_t curvature)
                   :name(name), countStops(countStops), countUnicStops(countUnicStops), distance(distance), curvature(curvature) {}

StopInfo::StopInfo():name(""), buses({}){}
StopInfo::StopInfo(std::string name, std::vector <std::string_view> buses):name(name), buses(buses){}

}
}
