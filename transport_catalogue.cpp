#include "transport_catalogue.h"

#include <iomanip>
#include "iostream"
#include <limits>
using namespace std;


void TransportCatalogue::AddStop(Requst &requst)
{
    size_t fName = requst.start.find_first_of(':');

    size_t fSplitCoordinates = requst.start.find_first_of(',');
    string_view name = requst.start.substr(0, fName);
    string nameS {name};


    // перевод в double  stod(string);
    //double lat = ::atof(stop.start.substr(fName + 1, stop.start.size() - fName).data());
    //double lng = ::atof(stop.start.substr(fSplitCoordinates + 1, stop.start.size() - fSplitCoordinates).data());
    //string_view latv = stop.start.substr(fName +2 , stop.start.size() - fName);
    //string_view lngv = stop.start.substr(fSplitCoordinates + 2, stop.start.size() - fSplitCoordinates);



    double lat = ::stod(requst.start.substr(fName + 2, requst.start.size() - fSplitCoordinates-1).data());
    double lng = ::stod(requst.start.substr(fSplitCoordinates + 2, requst.start.size() - fSplitCoordinates).data());

    //double lat = ::atof(stop.start.substr(fName + 2, stop.start.size() - fSplitCoordinates-1).data());
    //double lng = ::atof(stop.start.substr(fSplitCoordinates + 2, stop.start.size() - fSplitCoordinates).data());
    /*
    string_view latv = requst.start.substr(fName +2 , requst.start.size() - fSplitCoordinates-1);
    string_view lngv = requst.start.substr(fSplitCoordinates + 2, requst.start.size() - fSplitCoordinates);
    cout << nameS << endl;
    cout << latv << "|" << lngv << endl;
    cout <<std::setprecision(6) << lat << "|" << lng << endl;
    */
    stops_.push_back({move(nameS), {lat, lng}});
    stopnameToStops_.insert({stops_.back().name, &stops_.back()});

}

void TransportCatalogue::AddBus(Requst & requst)
{
    size_t fName = requst.start.find_first_of(':');

    //size_t fSplitCoordinates = requst.start.find_first_of(',');
    string_view name = requst.start.substr(0, fName);
    string nameS {name};
    vector <string_view> stops;
    size_t fStartStops = fName+1;
    bool work = true;
    bool cycle = false;
    string spliter = ">";

    if (string::npos == requst.start.find_first_of(">", fStartStops))
    {
        cycle = true;
        spliter = "-";
    }

    while (work) {
        size_t fSplit = requst.start.find_first_of(spliter, fStartStops);

        if (fSplit == string::npos)
        {
            work = false;
            fSplit = requst.start.size();
        }

        size_t fEndStop = fSplit - 1;
        size_t fStartStop = requst.start.find_first_not_of(" ", fStartStops);

        while (requst.start.at(fEndStop) == ' ')
            fEndStop--;

        stops.push_back(requst.start.substr(fStartStop, fEndStop - fStartStop + 1));
        fStartStops = fSplit + 1;
    }

    if (cycle)
    {
        for (int i = stops.size() - 2; i >= 0 ;i-- )
            stops.push_back(stops.at(i));
    }

    Bus newBus;
    newBus.name = move(nameS);
    for (string_view value : stops)
    {
        auto f = stopnameToStops_.find(value);
        newBus.stops.push_back(f->second);
    }

    buses.push_back(move(newBus));
    busNameToBus_.insert({buses.back().name, &buses.back()});

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


    auto fBus = busNameToBus_.find(nameBus);
    if (fBus == busNameToBus_.end())
        return false;

    answer.name = nameBus;
    return true;


}
