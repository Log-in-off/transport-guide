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

    Requst();
    Requst(std::string text, std::string_view start);
};

struct BusInput {
    std::string name;
    std::string request;
    std::vector <std::string_view> stops;

    BusInput();
    BusInput(std::string name, std::string request, std::vector <std::string_view> stops);
};

struct Bus
{
    std::string name;
    std::vector <Stop*> stops;
    uint32_t countUnicStops;
    uint32_t distance;
    double_t curvature;

    Bus();
    Bus(std::string name, std::vector <Stop*> stops, uint32_t countUnicStops, uint32_t distance, double_t curvature);
};

struct BusInfo {
    std::string name;
    uint32_t countStops;
    uint32_t countUnicStops;
    uint32_t distance;
    double_t curvature;

    BusInfo();
    BusInfo(std::string name, uint32_t countStops, uint32_t countUnicStops, uint32_t distance, double_t curvature);
};

struct StopInfo {
    std::string name;
    std::vector <std::string_view> buses;

    StopInfo();
    StopInfo(std::string name, std::vector <std::string_view> buses);
};

struct DurationHasher {
    size_t operator() (const std::pair <const Stop*, const Stop*>& f) const;
};

class TransportCatalogue
{
public:
    void AddStop(const Requst &requst);
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
