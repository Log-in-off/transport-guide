#include "request_handler.h"
#include <algorithm>
namespace TG
{
namespace RH
{

RequestHandler::RequestHandler(catalogue::TransportCatalogue &db):db_(db) {}

void RequestHandler::AddAllStops(const std::deque<domain::Request> &stops)
{
    std::for_each(stops.begin(), stops.end(), [&] (auto &req) {db_.AddStop(req);});
    std::for_each(stops.begin(), stops.end(), [&] (auto &req) {db_.AddDurationsBetweenStop(req);});
}

void RequestHandler::AddAllBuses(const std::deque<domain::Request> &buses)
{
    std::for_each(buses.begin(), buses.end(), [&] (auto &req) {db_.AddBus(req);});
}

bool RequestHandler::GetBusesByStop(const domain::Request &requst, domain::StopInfo &answer) const
{
    return db_.FindStop(requst, answer);
}

std::map <const std::string_view, const domain::Bus *> RequestHandler::GetBuses()
{
    return db_.GetBusesInfo();
}

std::map<const std::string_view, const domain::Stop *> RequestHandler::GetUsedStop()
{
    return db_.GetUsedStopInfo();
}

bool RequestHandler::GetBusStat(const domain::Request &requst, domain::BusInfo &answer) const
{
    return db_.FindBus(requst, answer);
}

}
}
