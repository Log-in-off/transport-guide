#include "transport_catalogue.h"

#include <iomanip>
#include "iostream"
#include <limits>
#include <set>
using namespace std;

namespace TG
{
namespace catalogue
{

Requst::Requst():text(""), start(text) {}

Requst::Requst(std::string textInput):text(textInput), start(text) {}
Requst::Requst(std::string text, std::string_view start):text(text), start(start) {}

BusInput::BusInput():name(""), request(""), stops({}){}
BusInput::BusInput(std::string name, std::string request, std::vector <std::string_view> stops):name(name), request(request), stops(stops){}

Bus::Bus():name(""), stops({}), countUnicStops(0), distance(0), curvature(0), isRoundtrip(false){}
Bus::Bus(std::string name, std::vector <Stop*> stops, uint32_t countUnicStops, uint32_t distance, double_t curvature, bool isRoundtrip)
               :name(name), stops(stops), countUnicStops(countUnicStops), distance(distance), curvature(curvature), isRoundtrip(isRoundtrip){}

BusInfo::BusInfo():name(""), countStops(0), countUnicStops(0), distance(0), curvature(0) {}
BusInfo::BusInfo(std::string name, uint32_t countStops, uint32_t countUnicStops, uint32_t distance, double_t curvature)
                   :name(name), countStops(countStops), countUnicStops(countUnicStops), distance(distance), curvature(curvature) {}

StopInfo::StopInfo():name(""), buses({}){}
StopInfo::StopInfo(std::string name, std::vector <std::string_view> buses):name(name), buses(buses){}

size_t DurationHasher::operator() (const std::pair <const Stop*, const Stop*>& f) const {
    //std::hash<Stop*> dHasher;
    //return (dHasher(f.first) << 32) | dHasher(f.second);
    // не знаю какой из хешеров лучше
    return std::hash<const void*>{}(f.first) * 37 + std::hash<const void*>{}(f.second) * pow(37, 3);
}

void TransportCatalogue::AddStop(const Requst &requst)
{
    size_t fName = requst.start.find_first_of(':');
    size_t fSplitCoordinates = requst.start.find_first_of(',', fName);
    string nameS {requst.start.substr(0, fName)};

    const double lat = ::stod(requst.start.substr(fName + 2, requst.start.size() - fSplitCoordinates-1).data());
    const double lng = ::stod(requst.start.substr(fSplitCoordinates + 2, requst.start.size() - fSplitCoordinates).data());
    stops_.push_back({move(nameS), {lat, lng}, {}});
    stopnameToStops_.insert({stops_.back().name, &stops_.back()});
}

void TransportCatalogue::AddDurationsBetweenStop(const Requst &requst)
{
    size_t fName = requst.start.find_first_of(':');
    size_t fSplitCoordinates = requst.start.find_first_of(',', fName);
    string_view name = requst.start.substr(0, fName);

    auto fMainStop = stopnameToStops_.find(name);
    if ( fMainStop != stopnameToStops_.end())
    {
        // заполняем хеш-таблицу расстояний
        bool not_end_request = true;
        size_t fSplitSection = requst.start.find_first_of(',', fSplitCoordinates +1);
        if (fSplitSection == string::npos)
            return;
        while (not_end_request)
        {
            size_t fStartDur = requst.start.find_first_of("0123456789", fSplitSection +1);
            size_t fEndDur = requst.start.find_first_of("m", fStartDur +1);
            size_t fStartStation = fEndDur;
            for (int i = 0; i < 3; i++)
                fStartStation = requst.start.find_first_not_of(" ", fStartStation +1);
            size_t fEndSection = requst.start.find_first_of(",", fStartStation +1);
            if (fEndSection == string::npos)
            {
                fEndSection = requst.start.size();
                not_end_request = false;
            }

            size_t fEndStation = fEndSection-1;
            while (requst.start.at(fEndStation) == ' ')
                fEndStation--;

            string_view nameStation = requst.start.substr(fStartStation, fEndStation-fStartStation+1);
            const int duration = ::stoi(requst.start.substr(fStartDur, fEndDur-fStartDur).data());

            fSplitSection = fEndSection;
            auto fStop = stopnameToStops_.find(nameStation);
            tableDurations_[{fMainStop->second, fStop->second}] = duration;
        }
    }

}

bool TransportCatalogue::FindStop(const Requst &requst, StopInfo &answer) const
{
    size_t fStart = requst.start.find_first_not_of(" ");
    size_t fEnd = requst.start.size() - 1;

    while (requst.start.at(fEnd) == ' ')
        fEnd--;
    string_view nameStop = requst.start.substr(fStart, fEnd - fStart + 1);
    answer.name = nameStop;

    auto fStop = stopnameToStops_.find(nameStop);
    if (fStop == stopnameToStops_.end())
        return false;

    for (auto value : fStop->second->buses)
    {
        answer.buses.push_back(value);
    }

    return true;
}

void TransportCatalogue::AddBus(const Requst & requst)
{
    size_t fName = requst.start.find_first_of(':');
    string_view name = requst.start.substr(0, fName);
    string nameS {name};
    vector <string_view> stops;
    size_t fStartStops = fName+1;
    bool not_end_request = true;
    bool lineTrip = false;
    string spliter = ">";

    if (string::npos == requst.start.find_first_of(">", fStartStops))
    {
        lineTrip = true;
        spliter = "-";
    }

    while (not_end_request) {
        size_t fSplit = requst.start.find_first_of(spliter, fStartStops);

        if (fSplit == string::npos)
        {
            not_end_request = false;
            fSplit = requst.start.size();
        }

        size_t fEndStop = fSplit - 1;
        size_t fStartStop = requst.start.find_first_not_of(" ", fStartStops);

        while (requst.start.at(fEndStop) == ' ')
            fEndStop--;

        stops.push_back(requst.start.substr(fStartStop, fEndStop - fStartStop + 1));
        fStartStops = fSplit + 1;
    }

    Stop *firstStop = nullptr;
    set <string_view> uinicStops;
    uint32_t factDistans = 0;
    double_t geoDist = 0;

    buses_.push_back({move(nameS), {}, 0, 0, 0, !lineTrip});

    Bus & addedBus = buses_.back();
    for (string_view value : stops)
    {
        auto f = stopnameToStops_.find(value);
        addedBus.stops.push_back(f->second);
        f->second->buses.insert(addedBus.name);
        uinicStops.insert(value);
        if (firstStop)
        {
            geoDist += ComputeDistance(firstStop->coordinates, f->second->coordinates);
            auto fDist = tableDurations_.find({firstStop, f->second});
            if (fDist == tableDurations_.end())
                fDist = tableDurations_.find({f->second, firstStop});
            factDistans += fDist->second;
        }
        firstStop = f->second;
    }

    if (lineTrip)
    {
        geoDist *= 2;
        for (int i = stops.size() - 2; i >= 0 ; i-- )
        {
            addedBus.stops.push_back(addedBus.stops.at(i));

            auto fDist = tableDurations_.find({addedBus.stops.at(addedBus.stops.size() - 2), addedBus.stops.at(addedBus.stops.size() - 1)});
            if (fDist == tableDurations_.end())
                fDist = tableDurations_.find({addedBus.stops.at(addedBus.stops.size() - 1), addedBus.stops.at(addedBus.stops.size() - 2)});

            factDistans += fDist->second;
        }
    }


    busNameToBus_.insert({addedBus.name, &addedBus});
    addedBus.countUnicStops = uinicStops.size();
    addedBus.distance = factDistans;
    addedBus.curvature =  static_cast<double_t>(factDistans) /geoDist;
}

std::map <const string_view, const Bus *> TransportCatalogue::GetBusesInfo() const
{
    return {busNameToBus_.begin(), busNameToBus_.end()};
}

std::map<const string_view, const Stop *> TransportCatalogue::GetUsedStopInfo() const
{
    std::map<const string_view, const Stop *> answer;
    for (const auto & stop: stops_)
    {
        if(!stop.buses.empty())
            answer.insert({stop.name, &stop});
    }
    return answer;
}

bool TransportCatalogue::FindBus(const Requst &requst,  BusInfo &answer) const
{
    size_t fStart = requst.start.find_first_not_of(" ");
    size_t fEnd = requst.start.size() - 1;

    while (requst.start.at(fEnd) == ' ')
        fEnd--;
    string_view nameBus = requst.start.substr(fStart, fEnd - fStart + 1);
    answer.name = nameBus;


    auto fBus = busNameToBus_.find(nameBus);
    if (fBus == busNameToBus_.end())
        return false;

    answer.curvature = fBus->second->curvature;
    answer.countStops = fBus->second->stops.size();
    answer.countUnicStops = fBus->second->countUnicStops;
    answer.distance = fBus->second->distance;

    return true;
}



}
}
