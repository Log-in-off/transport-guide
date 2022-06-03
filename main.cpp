#include <iostream>
#include <sstream>

using namespace std;

#include "input_reader.h"
#include "stat_reader.h"
#include "tests.h"

int main()
{
    firstTest();
    secondTest();
    therdTest();
    return 0;

    InputReader reader;
    TransportCatalogue catalogue;
    reader.GetQueries(cin, catalogue);

    StatReader statReader;
    statReader.GetQueries(cin, cout, catalogue);

    return 0;
}
