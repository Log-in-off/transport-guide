#include "input_reader.h"

#include "iostream"

using namespace std;

namespace TG
{
namespace input
{

void InputReader::GetQueries(std::istream &iStream, catalogue::TransportCatalogue &catalogue)
{
    std::deque <domain::Request> stops;
    std::deque <domain::Request> buses;
    int count = 0;
    iStream >> count;
    for (int i= 0; i <= count; i++)
    {
        GetRequests(iStream, stops, buses);
    }

    for(auto &value: stops)
    {
        catalogue.AddStop(value);
    }

    for(auto &value: stops)
    {
        catalogue.AddDurationsBetweenStop(value);
    }

    for(auto &value: buses)
    {
        catalogue.AddBus(value);
    }
}

void InputReader::GetRequests(std::istream &iStream,  std::deque<domain::Request> &stops, std::deque<domain::Request> &buses)
{
    string input;
    getline(iStream, input);
    if (input.empty())
        return ;

    const string headBus = "Bus ";
    const string headStop = "Stop ";
    size_t head;
    deque <domain::Request> * requsts;
    size_t findHeadBus = input.find(headBus);
    size_t findHeadStop = input.find(headStop);
    if (findHeadBus == 0)
    {
        head = headBus.size();
        requsts = &buses;
    }
    else if (findHeadStop == 0)
    {
        head = headStop.size();
        requsts = &stops;
    }
    else
        return;

    string_view startRequest;
    requsts->push_back({std::move(input), startRequest});
    requsts->back().start = requsts->back().text;
    requsts->back().start.remove_prefix(head);
}

}

}
