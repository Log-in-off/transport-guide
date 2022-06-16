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
    void AddAllStops(const std::deque <domain::Request> &stops);
    void AddAllBuses(const std::deque <domain::Request> &buses);
    bool GetBusStat(const domain::Request &requst, domain::BusInfo &answer) const;
    bool GetBusesByStop(const domain::Request &requst, domain::StopInfo &answer) const;

    std::map <const std::string_view, const domain::Bus *> GetBuses();
    std::map<const std::string_view, const domain::Stop *> GetUsedStop();

private:
    catalogue::TransportCatalogue& db_;
};

}
}
