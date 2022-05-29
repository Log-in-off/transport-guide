#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <map>
#include <string>

#include <geo.h>
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
};



class TransportCatalogue
{
public:
    void AddStop(Requst &Requst);
    //void FindStop();
    void AddBus(Requst &Requst);
    //void FindBus();
    void GetBusInfo(const std::string &namebus);
private:
    std::deque <Stop> stops_;
    std::unordered_map <std::string_view, Stop *> stopnameToStops_;
    std::deque <Bus> buses;
    std::unordered_map <std::string_view, Bus *> busNameToBus_;
};
