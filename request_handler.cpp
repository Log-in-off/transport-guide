#include "request_handler.h"
#include <algorithm>
namespace TG
{
namespace RH
{

RequestHandler::RequestHandler(catalogue::TransportCatalogue &db):db_(db) {}

void RequestHandler::AddAllStops(const std::deque<catalogue::Requst> &stops)
{
    std::for_each(stops.begin(), stops.end(), [&] (auto &req) {db_.AddStop(req);});
    std::for_each(stops.begin(), stops.end(), [&] (auto &req) {db_.AddDurationsBetweenStop(req);});
}

void RequestHandler::AddAllBuses(const std::deque<catalogue::Requst> &buses)
{
    std::for_each(buses.begin(), buses.end(), [&] (auto &req) {db_.AddBus(req);});
}

bool RequestHandler::GetBusesByStop(const catalogue::Requst &requst, catalogue::StopInfo &answer) const
{
    return db_.FindStop(requst, answer);
}

std::map <const std::string_view, const catalogue::Bus *> RequestHandler::GetBusAndStops()
{
    return db_.GetBusesInfo();
}

bool RequestHandler::GetBusStat(const catalogue::Requst &requst, catalogue::BusInfo &answer) const
{
    return db_.FindBus(requst, answer);
}

}
}
