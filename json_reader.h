#pragma once
#include <iostream>
#include <deque>

#include "transport_catalogue.h"
#include "request_handler.h"
#include "json.h"
#include "map_renderer.h"
/*
 * Здесь можно разместить код наполнения транспортного справочника данными из JSON,
 * а также код обработки запросов к базе и формирование массива ответов в формате JSON
 */
namespace TG {

class ReaderJSON {
public:
    void GetQueries(std::istream & iStream, std::ostream &output, RH::RequestHandler &transport, renderer::MapRenderer *map = nullptr);
private:
    const std::string tagBus = "Bus";
    const std::string tagStop = "Stop";
    void MakeRequestStop(std::deque <catalogue::Requst> &stops, const json::Node &node);
    void MakeRequestBus(std::deque <catalogue::Requst> &stops, const json::Node &node);
    void MakeAnswerStopInfo(RH::RequestHandler &transport, catalogue::Requst &req, json::Dict &node);
    void MakeAnswerBusInfo(RH::RequestHandler &transport, catalogue::Requst &req, json::Dict &node);
};

}
