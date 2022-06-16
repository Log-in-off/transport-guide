#pragma once
#include <optional>
#include <unordered_set>
#include "transport_catalogue.h"

namespace TG
{
namespace RH
{

class RequestHandler {
public:
    RequestHandler(catalogue::TransportCatalogue& db);
    void AddAllStops(const std::deque <domain::Requst> &stops);
    void AddAllBuses(const std::deque <domain::Requst> &buses);
    bool GetBusStat(const domain::Requst &requst, domain::BusInfo &answer) const;
    bool GetBusesByStop(const domain::Requst &requst, domain::StopInfo &answer) const;

    std::map <const std::string_view, const domain::Bus *> GetBuses();
    std::map<const std::string_view, const domain::Stop *> GetUsedStop();

private:
    catalogue::TransportCatalogue& db_;
};

}
}
