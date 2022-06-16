#pragma once
#include <iostream>
#include <deque>

#include "transport_catalogue.h"
#include "request_handler.h"
#include "json.h"
#include "map_renderer.h"

namespace TG {

class ReaderJSON {
public:
    void GetQueries(std::istream & iStream, std::ostream &output, RH::RequestHandler &transport, renderer::MapRenderer *map = nullptr);
private:
    const std::string tagBus = "Bus";
    const std::string tagStop = "Stop";
    const std::string tagMap = "Map";
    void FillingCatalogue(json::Document &doc, RH::RequestHandler &transport);
    void GetRequstsToCatalogue(json::Document &doc, RH::RequestHandler &transport, std::ostream& output, renderer::MapRenderer *map);
    void GetSettingsForMap(json::Document &doc, renderer::MapRenderer *map);
    svg::Color GetColor(const json::Node &node);
    void MakeRequestStop(std::deque <domain::Requst> &stops, const json::Node &node);
    void MakeRequestBus(std::deque <domain::Requst> &stops, const json::Node &node);
    void MakeAnswerStopInfo(RH::RequestHandler &transport, domain::Requst &req, json::Dict &node);
    void MakeAnswerBusInfo(RH::RequestHandler &transport, domain::Requst &req, json::Dict &node);
    void MakeAnswerMap(RH::RequestHandler &transport, json::Dict &node, renderer::MapRenderer *map);
};

}
