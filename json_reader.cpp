#include "json_reader.h"

#include <sstream>


/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

namespace TG
{
void ReaderJSON::GetQueries(std::istream &iStream, std::ostream& output, RH::RequestHandler &transport)
{
    using namespace std::literals;
    std::deque <catalogue::Requst> stops;
    std::deque <catalogue::Requst> buses;
    json::Document doc = json::Load(iStream);
    if (doc.GetRoot().IsMap())
    {
        auto findBaseReq = doc.GetRoot().AsMap().find("base_requests");
        if (findBaseReq !=  doc.GetRoot().AsMap().end() && findBaseReq->second.IsArray())
        {
            for (const auto & value : findBaseReq->second.AsArray())
            {
                auto findType = value.AsMap().find("type");
                if (findType != value.AsMap().end())
                {
                    if (findType->second.AsString() == tagStop)
                        MakeRequestStop(stops, value);
                    else if (findType->second.AsString() == tagBus)
                        MakeRequestBus(buses, value);
                }
            }
        }
        transport.AddAllStops(stops);
        transport.AddAllBuses(buses);
        //шлем stops и buses в реквест хендлер
        auto findStat = doc.GetRoot().AsMap().find("stat_requests");
        if (findStat != doc.GetRoot().AsMap().end() && findStat->second.IsArray())
        {
            json::Array out;
            for (const auto &value : findStat->second.AsArray())
            {
                catalogue::Requst req{"", value.AsMap().at("name").AsString()};
                json::Dict newNode{{"request_id", value.AsMap().at("id").AsInt()}};

                if (tagStop == value.AsMap().at("type").AsString())
                {
                    MakeAnswerStopInfo(transport, req, newNode);
                }
                else if (tagBus == value.AsMap().at("type").AsString())
                {
                    MakeAnswerBusInfo(transport, req, newNode);
                }
                out.push_back(newNode);
            }
            json::Print(json::Document{out}, output);
        }
    }
}

void ReaderJSON::MakeRequestStop(std::deque<catalogue::Requst> &stops, const json::Node &node)
{
    std::stringstream ss;
    ss << node.AsMap().at("name").AsString() << ": ";
    json::PrintNode(node.AsMap().at("latitude"), ss);
    ss << ", ";
    json::PrintNode(node.AsMap().at("longitude"), ss);

    auto findDistances = node.AsMap().find("road_distances");
    if (findDistances != node.AsMap().end())
    {
        for (const auto & value :findDistances->second.AsMap())
        {
            ss << ", " << value.second.AsInt() << "m to " << value.first;
        }
    }
    stops.emplace_back((ss.str()));
}

void ReaderJSON::MakeRequestBus(std::deque <catalogue::Requst> &stops, const json::Node &node)
{
    std::stringstream ss;
    ss << node.AsMap().at("name").AsString() << ": ";
    bool next =false;
    std::string spliter = " > ";
    auto findRoundTrip = node.AsMap().find("is_roundtrip");
    if (findRoundTrip != node.AsMap().end())
    {
        if (findRoundTrip->second.AsBool() == false)
            spliter = " - ";
    }
    auto findStops = node.AsMap().find("stops");
    if (findStops != node.AsMap().end())
    {
        for (const auto & value : findStops->second.AsArray())
        {
            if (next)
                ss << spliter;
            next = true;
            ss << value.AsString();
        }
    }
    stops.emplace_back((ss.str()));
}

void ReaderJSON::MakeAnswerStopInfo(RH::RequestHandler &transport, catalogue::Requst &req, json::Dict &node)
{
    catalogue::StopInfo answer;
    if(transport.GetBusesByStop(req, answer))
    {
        json::Array buses;
        for(auto bus : answer.buses)
            buses.push_back(std::string{bus});
        node.emplace("buses", buses);
    }
    else
    {
        node.emplace("error_message", std::string{"not found"});
    }
}

void ReaderJSON::MakeAnswerBusInfo(RH::RequestHandler &transport, catalogue::Requst &req, json::Dict &node)
{
    catalogue::BusInfo answer;
    if (transport.GetBusStat(req, answer))
    {
        node.emplace("curvature", answer.curvature);
        node.emplace("route_length", static_cast<int>(answer.distance));
        node.emplace("stop_count", static_cast<int>(answer.countStops));
        node.emplace("unique_stop_count", static_cast<int>(answer.countUnicStops));
    }
    else
    {
        node.emplace("error_message", std::string{"not found"});
    }
}


}
