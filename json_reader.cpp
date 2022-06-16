#include "json_reader.h"

#include <sstream>


/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */

namespace TG
{
void ReaderJSON::GetQueries(std::istream &iStream, std::ostream& output, RH::RequestHandler &transport, renderer::MapRenderer *map)
{
    json::Document doc = json::Load(iStream);
    if (doc.GetRoot().IsMap())
    {
        //шлем stops и buses в реквест хендлер
        FillingCatalogue(doc, transport);

        if (map)
            GetSettingsForMap(doc, map);

        GetRequstsToCatalogue(doc, transport, output, map);
    }
}

void ReaderJSON::FillingCatalogue(json::Document &doc, RH::RequestHandler &transport)
{
    std::deque <catalogue::Requst> stops;
    std::deque <catalogue::Requst> buses;
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
}

void ReaderJSON::GetRequstsToCatalogue(json::Document &doc, RH::RequestHandler &transport, std::ostream &output, renderer::MapRenderer *map)
{
    auto findStat = doc.GetRoot().AsMap().find("stat_requests");
    if (findStat != doc.GetRoot().AsMap().end() && findStat->second.IsArray() && !findStat->second.AsArray().empty())
    {
        json::Array out;
        for (const auto &value : findStat->second.AsArray())
        {
            json::Dict newNode{{"request_id", value.AsMap().at("id").AsInt()}};

            if (tagStop == value.AsMap().at("type").AsString())
            {
                catalogue::Requst req{"", value.AsMap().at("name").AsString()};
                MakeAnswerStopInfo(transport, req, newNode);
            }
            else if (tagBus == value.AsMap().at("type").AsString())
            {
                catalogue::Requst req{"", value.AsMap().at("name").AsString()};
                MakeAnswerBusInfo(transport, req, newNode);
            }
            else if (tagMap == value.AsMap().at("type").AsString())
            {
                MakeAnswerMap(transport, newNode, map);
            }
            out.push_back(newNode);
        }
        json::Print(json::Document{out}, output);
    }
}

void ReaderJSON::GetSettingsForMap(json::Document &doc, renderer::MapRenderer *map)
{
    auto findStat = doc.GetRoot().AsMap().find("render_settings");
    if (findStat != doc.GetRoot().AsMap().end() && (!doc.GetRoot().AsMap().empty()))
    {
        renderer::ParamsMap param;
        auto inputParam = findStat->second.AsMap();
        param.width = inputParam.at("width").AsDouble();
        param.height = inputParam.at("height").AsDouble();
        param.padding = inputParam.at("padding").AsDouble();
        param.line_width = inputParam.at("line_width").AsDouble();
        param.stop_radius = inputParam.at("stop_radius").AsDouble();
        param.bus_label_font_size = inputParam.at("bus_label_font_size").AsInt();
        param.bus_label_offset.x = inputParam.at("bus_label_offset").AsArray().at(0).AsDouble();
        param.bus_label_offset.y = inputParam.at("bus_label_offset").AsArray().at(1).AsDouble();
        param.stop_label_font_size = inputParam.at("stop_label_font_size").AsInt();
        param.stop_label_offset.x = inputParam.at("stop_label_offset").AsArray().at(0).AsDouble();
        param.stop_label_offset.y = inputParam.at("stop_label_offset").AsArray().at(1).AsDouble();
        param.underlayer_width = inputParam.at("underlayer_width").AsDouble();

        param.underlayer_color = GetColor(inputParam.at("underlayer_color"));
        for (const auto &value : inputParam.at("color_palette").AsArray())
        {
            param.color_palette.push_back(GetColor(value));
        }

        map->SetParamMap(param);
    }
}

svg::Color ReaderJSON::GetColor(const json::Node &node)
{
    if (node.IsString())
        return svg::Color(node.AsString());

    uint8_t red = static_cast<uint8_t>(node.AsArray().at(0).AsInt());
    uint8_t green = static_cast<uint8_t>(node.AsArray().at(1).AsInt());
    uint8_t blue = static_cast<uint8_t>(node.AsArray().at(2).AsInt());

    if (node.AsArray().size() == 3 )
        return svg::Color(svg::Rgb{red, green, blue});

    double opacity = node.AsArray().at(3).AsDouble();
    return svg::Color(svg::Rgba{red, green, blue, opacity});
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

void ReaderJSON::MakeAnswerMap(RH::RequestHandler &transport, json::Dict &node, renderer::MapRenderer *map)
{
    std::stringstream ss;
    map->RenderMap(transport, ss);
    node.emplace("map", ss.str());
}


}
