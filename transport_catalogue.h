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
    Requst(std::string textInput);
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
    bool isRoundtrip;

    Bus();
    Bus(std::string name, std::vector <Stop*> stops, uint32_t countUnicStops, uint32_t distance, double_t curvature, bool isRoundtrip);
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
    void AddDurationsBetweenStop(const Requst &requst);
    bool FindStop(const Requst &requst, StopInfo &answer) const;
    void AddBus(const Requst &requst);
    std::map <const std::string_view, const Bus *> GetBusesInfo() const;
    std::map <const std::string_view, const Stop *> GetUsedStopInfo() const;
    bool FindBus(const Requst &requst, BusInfo &answer) const;


private:
    std::deque <Stop> stops_;
    std::unordered_map <std::string_view, Stop *> stopnameToStops_;
    std::deque <Bus> buses_;
    std::unordered_map <std::string_view, Bus *> busNameToBus_;
    std::unordered_map<std::pair <const Stop*, const Stop*>, int, DurationHasher> tableDurations_;
};
}
}
