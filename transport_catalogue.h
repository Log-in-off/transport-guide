#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include <cmath>

#include "geo.h"

namespace TG
{
namespace catalogue
{

struct Stop
{
    std::string name;
    geo::Coordinates coordinates;
    std::set <std::string_view> buses;
};

struct Requst
{
   std::string text;
    std::string_view start;

    Requst():text(""), start(text){}
    Requst(std::string text, std::string_view start):text(text), start(start){}
};

struct BusInput {
    std::string name;
    std::string request;
    std::vector <std::string_view> stops;

    BusInput():name(""), request(""), stops({}){}
    BusInput(std::string name, std::string request, std::vector <std::string_view> stops):name(name), request(request), stops(stops){}
};

struct Bus
{
    std::string name;
    std::vector <Stop*> stops;
    uint32_t countUnicStops;
    uint32_t distance;
    double_t curvature;

    Bus():name(""), stops({}), countUnicStops(0), distance(0), curvature(0){}
    Bus(std::string name, std::vector <Stop*> stops, uint32_t countUnicStops, uint32_t distance, double_t curvature)
                   :name(name), stops(stops), countUnicStops(countUnicStops), distance(distance), curvature(curvature){}
};

struct BusInfo {
    std::string name;
    uint32_t countStops;
    uint32_t countUnicStops;
    uint32_t distance;
    double_t curvature;

    BusInfo():name(""), countStops(0), countUnicStops(0), distance(0), curvature(0) {}
    BusInfo(std::string name, uint32_t countStops, uint32_t countUnicStops, uint32_t distance, double_t curvature)
                       :name(name), countStops(countStops), countUnicStops(countUnicStops), distance(distance), curvature(curvature) {}
};

struct StopInfo {
    std::string name;
    std::vector <std::string_view> buses;

    StopInfo():name(""), buses({}){}
    StopInfo(std::string name, std::vector <std::string_view> buses):name(name), buses(buses){}
};

struct DurationHasher {
    size_t operator() (const std::pair <const Stop*, const Stop*>& f) const {
        //std::hash<Stop*> dHasher;
        //return (dHasher(f.first) << 32) | dHasher(f.second);
        // не знаю какой из хешеров лучше
        return std::hash<const void*>{}(f.first) * 37 + std::hash<const void*>{}(f.second) * pow(37, 3);
    }
};

class TransportCatalogue
{
public:
    void AddStop(Requst &requst);
    bool FindStop(Requst &requst, StopInfo &answer);
    void AddBus(Requst &requst);
    //void FindBus();
    bool GetBusInfo(Requst &requst, BusInfo &answer);
private:
    std::deque <Stop> stops_;
    std::unordered_map <std::string_view, Stop *> stopnameToStops_;
    std::deque <Bus> buses_;
    std::unordered_map <std::string_view, Bus *> busNameToBus_;
    std::unordered_map<std::pair <const Stop*, const Stop*>, int, DurationHasher> tableDurations_;
};
}
}
