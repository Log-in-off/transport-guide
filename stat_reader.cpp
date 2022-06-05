

#include "stat_reader.h"

#include <iomanip>
#include "iostream"
#include <limits>

using namespace std;

namespace TG
{

namespace stat
{
void StatReader::GetQueries(std::istream &iStream, ostream &out, catalogue::TransportCatalogue &catalogue)
{
    std::deque <catalogue::Requst> busses;
    std::deque <catalogue::Requst> stops;
    int count;
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

        catalogue::Requst req{"", input};
        req.start.remove_prefix(head);

        if (requsts == &stops)
        {
            catalogue::StopInfo answer;
            if (catalogue.FindStop(req, answer))
            {
                if (answer.buses.size())
                {
                    out << "Stop "<< answer.name <<": buses";
                    for (auto bus:answer.buses)
                        out << " "<< bus;
                    out << endl;
                }
                else
                {
                    out << "Stop "<< answer.name <<": no buses" << endl;
                }

            }
            else
            {
                //Stop Samara: not found
                out << "Stop "<< answer.name <<": not found" << endl;
            }

        }
        else if (requsts == &busses)
        {
            catalogue::BusInfo answer;
            if (catalogue.GetBusInfo(req, answer))
            {
                //Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.36124 curvature
                out << "Bus " << answer.name << ": " << answer.countStops
                              << " stops on route, " << answer.countUnicStops
                              << " unique stops, " << answer.distance << " route length, "
                              << std::setprecision(6) << answer.curvature << " curvature"
                              << endl;
            }
            else
            {
                out << "Bus "<< answer.name <<": not found" << endl;
            }
        }
    }

}

}
}

