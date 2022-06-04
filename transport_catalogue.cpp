#include "transport_catalogue.h"

#include <iomanip>
#include "iostream"
#include <limits>
#include <set>
using namespace std;


void TransportCatalogue::AddStop(Requst &requst)
{
    size_t fName = requst.start.find_first_of(':');

    size_t fSplitCoordinates = requst.start.find_first_of(',', fName);
    string_view name = requst.start.substr(0, fName);
    auto fMainStop = stopnameToStops_.find(name);
    if ( fMainStop == stopnameToStops_.end())
    {
        //первый проход добавления. добавляем остановки без заполнения хеша расстояний
        string nameS {name};
        const double lat = ::stod(requst.start.substr(fName + 2, requst.start.size() - fSplitCoordinates-1).data());
        const double lng = ::stod(requst.start.substr(fSplitCoordinates + 2, requst.start.size() - fSplitCoordinates).data());
        stops_.push_back({move(nameS), {lat, lng}, {}});
        stopnameToStops_.insert({stops_.back().name, &stops_.back()});
    }
    else
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

            // проблемное место
            std::pair <Stop*, Stop*> obj = {fMainStop->second, fStop->second}; // формируем ключ
            DurationHasher hash; // объявляем хешер для проверки
            cout << nameStation <<" " << duration << " hash " << hash(obj) <<endl;

            // добавляем в два одинаковых объекта. tableDurations и tableDurations_
            // в одном случае падает, в другом нет.
            std::unordered_map<std::pair <Stop*, Stop*>, int, DurationHasher> tableDurations;
            tableDurations[obj] = duration;
            this->tableDurations_[obj] = duration;

        }
    }
}

bool TransportCatalogue::FindStop(Requst &requst, StopInfo &answer)
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

void TransportCatalogue::AddBus(Requst & requst)
{
    size_t fName = requst.start.find_first_of(':');

    string_view name = requst.start.substr(0, fName);
    string nameS {name};
    vector <string_view> stops;
    size_t fStartStops = fName+1;
    bool not_end_request = true;
    bool cycle = false;
    string spliter = ">";

    if (string::npos == requst.start.find_first_of(">", fStartStops))
    {
        cycle = true;
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

    buses_.push_back({move(nameS), {}, cycle});
    for (string_view value : stops)
    {
        auto f = stopnameToStops_.find(value);
        buses_.back().stops.push_back(f->second);
        f->second->buses.insert(buses_.back().name);
    }

    if (cycle)
    {
        for (int i = stops.size() - 2; i >= 0 ; i-- )
            buses_.back().stops.push_back(buses_.back().stops.at(i));
    }


    busNameToBus_.insert({buses_.back().name, &buses_.back()});

    //std::cout << requst.start;
    //std::cout <<nameS << endl;
    //for (auto value : stops)
    //    std::cout <<value << endl;
    //std::cout << endl;
}

bool TransportCatalogue::GetBusInfo(Requst &requst,  BusInfo &answer)
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


    Stop *firstStop = nullptr;
    set <string_view> uinicStops;
    double_t calcDist = 0;
    for (auto value : fBus->second->stops)
    {
        uinicStops.insert(value->name);
        if (firstStop)
        {
            calcDist += ComputeDistance(firstStop->coordinates, value->coordinates);
            auto fDist = tableDurations_.find({value, firstStop});
            if (fDist == tableDurations_.end())
                fDist = tableDurations_.find({firstStop, value});
            answer.distance += fDist->second;
        }
        firstStop = value;
    }

    answer.curvature = static_cast<double_t>( answer.distance) /calcDist;
    answer.countStops = fBus->second->stops.size();
    answer.countUnicStops = uinicStops.size();

    return true;
}
