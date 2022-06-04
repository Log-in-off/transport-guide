#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <map>
#include <set>
#include <string>

#include "geo.h"
struct Stop {
    std::string name;
    Coordinates coordinates;
    std::set <std::string_view> buses;
};

struct Requst
{
    std::string text;
    std::string_view start;
};

struct BusInput {
    std::string name;
    std::string request;
    std::vector <std::string_view> stops;
};

struct Bus {
    std::string name;
    std::vector <Stop*> stops;
    bool cycle;
};

struct BusInfo {
    std::string name;
    uint32_t countStops;
    uint32_t countUnicStops;
    uint32_t distance;
    double_t curvature;

    BusInfo():name(""), countStops(0), countUnicStops(0), distance(0), curvature(0) {}
};

struct StopInfo {
    std::string name;
    std::vector <std::string_view> buses;
};
/*
struct Duration
{
    std::pair <Stop*, Stop*> dur;

};

bool operator==(const Duration &lh, const Duration &rh )
{
    return lh.dur == rh.dur;
}
*/

struct DurationHasher {
    size_t operator() (const std::pair <Stop*, Stop*>& f) const {
        std::hash<Stop*> dHasher;
        return (dHasher(f.first) << 32) | dHasher(f.second);
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
    std::unordered_map<std::pair <Stop*, Stop*>, int, DurationHasher> tableDurations_;
};
