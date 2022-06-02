

#include "stat_reader.h"

#include <iomanip>
#include "iostream"
#include <limits>

using namespace std;

void StatReader::GetQueries(std::istream &iStream, ostream &out, TransportCatalogue &catalogue)
{
    std::deque <Requst> busses;
    int count;
    iStream >> count;
    string input;
    for (int i= 0; i <= count; i++)
    {
        getline(iStream, input);
        if (input.empty())
            continue;
        size_t head;
        deque <Requst> * requsts;
        if (input.front() == 'B')
        {
            head = sizeof"Bus";
            requsts = &busses;
        }
        else
            continue;

        string_view startRequest;
        requsts->push_back({std::move(input), startRequest});
        requsts->back().start = requsts->back().text;
        requsts->back().start.remove_prefix(head);
    }

    for(auto &value: busses)
    {
        BusInfo answer;
        if (catalogue.GetBusInfo(value, answer))
        {
            //Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length
            out << "Bus " << answer.name << ": " << answer.countStops
                          << " stops on route, " << answer.countUnicStops
                          << " unique stops, "<< std::setprecision(6) << answer.distance << " route length"<< endl;
        }
        else
        {
            out << "Bus "<< answer.name <<": not found" << endl;
        }


    }

}
