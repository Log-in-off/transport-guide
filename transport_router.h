#pragma once
#include <iostream>
#include <optional>
#include <map>
#include <string_view>

#include "request_handler.h"
#include "router.h"
#include "graph.h"

namespace TG
{

namespace TR
{
struct RoutingSettings
{
    int bus_wait_time;
    int bus_velocity;
};

class TransportRouter
{
public:
    void SetSettings(RoutingSettings settings);
    void MakeRoutigGraph(RH::RequestHandler &transport);
private:
    std::optional<RoutingSettings> settings_;
    std::optional<std::map <const std::string_view, const domain::Bus *>> base_;
    std::optional<std::map <const std::string_view, const domain::Stop *>> stops_;

};

}

}
