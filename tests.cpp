/*
void TestRouteName()
{
    std::vector<std::string> routes{"Bus A: B - C - D",
                                   "Bus A B CD: C - D - B"};

    std::vector<std::string> stops{"Stop B: 55.632761, 37.333324",
                                   "Stop C: 52.632761, 32.333324",
                                   "Stop D: 15.632761, 17.333324",};

    InputReader reader;

    reader.GetQueries(std::move(stops), std::move(routes));

    TransportCatalogue catalogue(reader.GetRoutes(), reader.GetStops());

    assert(catalogue.GetRoute("A") != nullptr);
    assert(catalogue.GetRoute("A")->name_ == "A");

    assert(catalogue.GetRoute("A B CD") != nullptr);
    assert(catalogue.GetRoute("A B CD")->name_ == "A B CD");
}

void TestStopsName()
{
    std::vector<std::string> routes{"Bus A: BC A DE - CB D - DEFG - G"};

    std::vector<std::string> stops{"Stop BC A DE: 55.632761, 37.333324",
                                   "Stop CB D: 52.632761, 32.333324",
                                   "Stop DEFG: 15.632761, 17.333324",
                                   "Stop G: 11.632761, 11.333324"};

    InputReader reader;

    reader.GetQueries(std::move(stops), std::move(routes));

    TransportCatalogue catalogue(reader.GetRoutes(), reader.GetStops());

    assert(catalogue.GetRoute("A") != nullptr);

    std::vector<std::string> stop_names{"BC A DE",
                                        "CB D",
                                        "DEFG",
                                        "G",
                                        "DEFG",
                                        "CB D",
                                        "BC A DE"};

    for(auto i = 0; i < stop_names.size(); i++)
    {
        assert(catalogue.GetRoute("A")->route_[i]->name_ == stop_names[i]);
    }
}

void TestAddRouteBeforeStops()
{
    std::stringstream ss("");
    ss << "Bus 256 123-21: Biryulyovo Zapadnoye > Biryusinka > Biryulyovo Zapadnoye\n";
    ss << "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n";
    ss << "Bus 752: Tolstopaltsevo - Marushkino\n";
    ss << "Stop Tolstopaltsevo: 55.611087, 37.208290\n";
    ss << "Stop Marushkino: 55.595884, 37.209755\n";
    ss << "Stop Rasskazovka: 55.632761, 37.333324\n";
    ss << "Stop Biryulyovo Zapadnoye: 55.574371, 37.651700\n";
    ss << "Stop Biryusinka: 55.581065, 37.648390\n";

    std::vector<std::string> stops_strings;
    std::vector<std::string> bus_strings;

    std::string str;
    //std::getline(ss, str);

    for(int i = 0; i < 8; i++)
    {
        std::getline(ss, str);

        if(ToLower(str.substr(0, str.find(' '))) == "bus")
        {
            bus_strings.push_back(str);
        }
        else
        {
            stops_strings.push_back(str);
        }
    }

    InputReader reader;

    reader.GetQueries(std::move(stops_strings), std::move(bus_strings));

    auto routes = reader.GetRoutes();

    assert(routes.find({"256 123-21", {0, 0}})->route_.size() == 3);
    assert(routes.find({"750", {0, 0}})->route_.size() == 5);
    assert(routes.find({"752", {0, 0}})->route_.size() == 3);
}

*/
#include <iostream>
#include <fstream>
#include <sstream>

#include "tests.h"
#include "input_reader.h"
#include "stat_reader.h"
#include "transport_router.h"

void testTask3(void)
{
    using namespace std;
    std::stringstream ss("");
     std::stringstream stat("");
    ss << "13\n";
    ss << "Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino\n";
    ss << "Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka, 100m to Marushkino\n";
    ss << "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n";
    ss << "Bus 750: Tolstopaltsevo - Marushkino - Marushkino - Rasskazovka\n";
    ss << "Stop Rasskazovka: 55.632761, 37.333324, 9500m to Marushkino\n";
    ss << "Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam\n";
    ss << "Stop Biryusinka: 55.581065, 37.64839, 750m to Universam\n";
    ss << "Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya\n";
    ss << "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya\n";
    ss << "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye\n";
    ss << "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n";
    ss << "Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n";
    ss << "Stop Prazhskaya: 55.611678, 37.603831\n";

    stat << "6\n";
    stat << "Bus 256\n";
    stat << "Bus 750\n";
    stat << "Bus 751\n";
    stat << "Stop Samara\n";
    stat << "Stop Prazhskaya\n";
    stat << "Stop Biryulyovo Zapadnoye \n";

    TG::input::InputReader reader;
    TG::stat::StatReader statReader;
    TG::catalogue::TransportCatalogue catalogue;
    reader.GetQueries(ss, catalogue);
    statReader.GetQueries(stat, cout, catalogue);
}

#include "json_reader.h"
void testTaskJson(void)
{
    using namespace std;
    ifstream ifs("input.txt");
    if( ifs.is_open())
    {

        TG::catalogue::TransportCatalogue catalogue;
        TG::ReaderJSON reader;
        TG::RH::RequestHandler handler(catalogue);
        reader.GetQueries(ifs, cout, handler);
    }

}
void testTaskJsonN(void)
{
    using namespace std;
    ifstream ifs("test3.txt");
    if( ifs.is_open())
    {

        TG::catalogue::TransportCatalogue catalogue;
        TG::ReaderJSON reader;
        TG::RH::RequestHandler handler(catalogue);
        reader.GetQueries(ifs, cout, handler);
    }
}

void testMain(void)
{
    using namespace std;
    TG::catalogue::TransportCatalogue catalogue;
    TG::ReaderJSON reader;
    TG::RH::RequestHandler handler(catalogue);
    reader.GetQueries(cin, cout, handler);
}

void testTaskGraph1(void)
{
    using namespace std;
    ifstream ifs("graph1.json");
    if( ifs.is_open())
    {
        TG::catalogue::TransportCatalogue catalogue;
        TG::ReaderJSON reader;
        TG::RH::RequestHandler transport(catalogue);
        TG::renderer::MapRenderer map;
        TG::TR::TransportRouter router;
        reader.GetQueries(ifs, cout, transport, &map, &router);
    }
}
