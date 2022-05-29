#include "transport_catalogue.h"

#include <iomanip>
#include "iostream"
#include <limits>
using namespace std;


void TransportCatalogue::AddStop(Requst &stop)
{
    size_t fName = stop.start.find_first_of(':');

    size_t fSplitCoordinates = stop.start.find_first_of(',');
    string_view name = stop.start.substr(0, fName);
    string nameS {name};


    // перевод в double  stod(string);
    //double lat = ::atof(stop.start.substr(fName + 1, stop.start.size() - fName).data());
    //double lng = ::atof(stop.start.substr(fSplitCoordinates + 1, stop.start.size() - fSplitCoordinates).data());
    //string_view latv = stop.start.substr(fName +2 , stop.start.size() - fName);
    //string_view lngv = stop.start.substr(fSplitCoordinates + 2, stop.start.size() - fSplitCoordinates);



    double lat = ::stod(stop.start.substr(fName + 2, stop.start.size() - fSplitCoordinates-1).data());
    double lng = ::stod(stop.start.substr(fSplitCoordinates + 2, stop.start.size() - fSplitCoordinates).data());

    //double lat = ::atof(stop.start.substr(fName + 2, stop.start.size() - fSplitCoordinates-1).data());
    //double lng = ::atof(stop.start.substr(fSplitCoordinates + 2, stop.start.size() - fSplitCoordinates).data());
    string_view latv = stop.start.substr(fName +2 , stop.start.size() - fSplitCoordinates-1);
    string_view lngv = stop.start.substr(fSplitCoordinates + 2, stop.start.size() - fSplitCoordinates);
    cout << latv << "|" << lngv << endl;
    cout <<std::setprecision(6) << lat << "|" << lng << endl;
    stops_.push_back({move(nameS), {lat, lng}});
    stopnameToStops_.insert({stops_.back().name, &stops_.back()});

}

void TransportCatalogue::AddBus(Requst &Requst)
{
    return;

}
