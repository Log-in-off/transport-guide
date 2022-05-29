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