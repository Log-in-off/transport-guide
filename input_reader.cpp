#include "input_reader.h"

#include "iostream"

using namespace std;

namespace TG
{
namespace input
{

void InputReader::GetQueries(std::istream &iStream, catalogue::TransportCatalogue &catalogue)
{
    std::deque <catalogue::Requst> stops;
    std::deque <catalogue::Requst> busses;
    int count = 0;
    iStream >> count;
    string input;
    for (int i= 0; i <= count; i++)
    {
        getline(iStream, input);
        if (input.empty())
            continue;
        size_t head;
        deque <catalogue::Requst> * requsts;
        if (input.front() == 'B')
        {
            head = sizeof"Bus";
            requsts = &busses;
        }
        else if (input.front() == 'S')
        {
            head = sizeof"Stop";
            requsts = &stops;
        }
        else
            continue;

        string_view startRequest;
        requsts->push_back({std::move(input), startRequest});
        requsts->back().start = requsts->back().text;
        requsts->back().start.remove_prefix(head);
    }

    for (int i = 0; i < 2; i++)
        for(auto &value: stops)
        {
            catalogue.AddStop(value);
        }


    for(auto &value: busses)
    {
        catalogue.AddBus(value);
    }
}

}

}
