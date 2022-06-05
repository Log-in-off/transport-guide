

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
    int count;
    iStream >> count;
    string input;
    for (int i= 0; i <= count; i++)
    {
        getline(iStream, input);
        if (input.empty())
            continue;        

        typeRequset type;
        catalogue::Requst req{"", input};
        if (!GetReques(input,type, req))
            continue;

        if (type == getStop)
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
                    out << "Stop "<< answer.name <<": no buses" << endl;
            }
            else
                out << "Stop "<< answer.name <<": not found" << endl;
        }
        else if (type == getBus)
        {
            catalogue::BusInfo answer;
            if (catalogue.GetBusInfo(req, answer))
            {
                out << "Bus " << answer.name << ": " << answer.countStops
                              << " stops on route, " << answer.countUnicStops
                              << " unique stops, " << answer.distance << " route length, "
                              << std::setprecision(6) << answer.curvature << " curvature"
                              << endl;
            }
            else
                out << "Bus "<< answer.name <<": not found" << endl;
        }
    }
}

bool StatReader::GetReques(const std::string & input, typeRequset &type, catalogue::Requst &request)
{
    if (input.empty())
        return false;

    const string headBus = "Bus ";
    const string headStop = "Stop ";
    size_t head;

    size_t findHeadBus = input.find(headBus);
    size_t findHeadStop = input.find(headStop);
    if (findHeadBus == 0)
    {
        head = headBus.size();
        type = getBus;
    }
    else if (findHeadStop == 0)
    {
        head = headStop.size();
        type = getStop;
    }
    else
        return false;

    request.start.remove_prefix(head);
    return true;
}

}
}

