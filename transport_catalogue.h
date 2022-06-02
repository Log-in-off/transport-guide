#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <map>
#include <string>

#include "geo.h"
struct Stop {
    std::string name;
    Coordinates coordinates;
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
    double_t distance;

    BusInfo():name(""), countStops(0), countUnicStops(0), distance(0) {}
};



class TransportCatalogue
{
public:
    void AddStop(Requst &requst);
    //void FindStop();
    void AddBus(Requst &requst);
    //void FindBus();
    bool GetBusInfo(Requst &requst, BusInfo &answer);
private:
    std::deque <Stop> stops_;
    std::unordered_map <std::string_view, Stop *> stopnameToStops_;
    std::deque <Bus> buses;
    std::unordered_map <std::string_view, Bus *> busNameToBus_;
};
