#include <iostream>
#include <sstream>

using namespace std;

#include "input_reader.h"
#include "stat_reader.h"
#include "tests.h"
#include "test_svg.h"

int main()
{
    //testSVG();
    //testPicture();
    TestColor();
    return 0;

    testTask3();
    return 0;

    TG::input::InputReader reader;
    TG::catalogue::TransportCatalogue catalogue;
    reader.GetQueries(cin, catalogue);

    TG::stat::StatReader statReader;
    statReader.GetQueries(cin, cout, catalogue);

    return 0;
}
