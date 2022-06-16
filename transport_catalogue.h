#pragma once

#include <vector>
#include <deque>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include <cmath>

#include "geo.h"
#include "domain.h"

namespace TG
{
namespace catalogue
{

struct DurationHasher {
    size_t operator() (const std::pair <const domain::Stop*, const domain::Stop*>& f) const;
};

class TransportCatalogue
{
public:
    void AddStop(const domain::Request &requst);
    void AddDurationsBetweenStop(const domain::Request &requst);
    bool FindStop(const domain::Request &requst, domain::StopInfo &answer) const;
    void AddBus(const domain::Request &requst);
    std::map <const std::string_view, const domain::Bus *> GetBusesInfo() const;
    std::map <const std::string_view, const domain::Stop *> GetUsedStopInfo() const;
    bool FindBus(const domain::Request &requst, domain::BusInfo &answer) const;

private:
    std::deque <domain::Stop> stops_;
    std::unordered_map <std::string_view, domain::Stop *> stopnameToStops_;
    std::deque <domain::Bus> buses_;
    std::unordered_map <std::string_view, domain::Bus *> busNameToBus_;
    std::unordered_map<std::pair <const domain::Stop*, const domain::Stop*>, int, DurationHasher> tableDurations_;
};
}
}
