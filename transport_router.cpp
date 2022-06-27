#include "transport_router.h"

namespace TG
{
namespace TR
{

void TransportRouter::SetSettings(RoutingSettings settings)
{
    settings_ = std::move(settings);
}

void TransportRouter::MakeRoutigGraph(RH::RequestHandler &transport)
{
    base_ = transport.GetBuses();
    stops_ = transport.GetUsedStop();
    graph_ = std::make_unique<graph::DirectedWeightedGraph<Weighted>>(stops_->size()*2);

}



}
}
