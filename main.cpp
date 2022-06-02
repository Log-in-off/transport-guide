#include <iostream>
#include <sstream>

using namespace std;

#include "input_reader.h"
#include "stat_reader.h"

int main()
{
    cout << "Hello word" << endl;
    std::stringstream ss("");
        ss << "9\n";
        ss << "Bus 256 123-21: Biryulyovo Zapadnoye > Biryusinka > Biryulyovo Zapadnoye\n";
        ss << "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Biryulyovo Zapadnoye\n";
        ss << "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n";
        ss << "Bus 752: Tolstopaltsevo - Marushkino\n";
        ss << "Stop Tolstopaltsevo: -55.611087, 37.208290\n";
        ss << "Stop Marushkino: 55.595884, 37.209755\n";
        ss << "Stop Rasskazovka: 55.632761, 37.333324\n";
        ss << "Stop Biryulyovo Zapadnoye: 55.574371, 37.651700\n";
        ss << "Stop Biryusinka: 55.581065, 37.648390\n";


    InputReader reader;
    TransportCatalogue catalogue;
    reader.GetQueries(ss, catalogue);


    std::stringstream stat("");
    stat << "3\n";
    stat << "Bus 256\n";
    stat << "Bus 750\n";
    stat << "Bus 752\n";
    StatReader statReader;
    statReader.GetQueries(stat, cout, catalogue);
    cout << "End" << endl;

    return 0;
}
