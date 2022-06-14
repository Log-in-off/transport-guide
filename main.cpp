#include <iostream>
#include <sstream>

using namespace std;

#include "input_reader.h"
#include "stat_reader.h"
#include "tests.h"
#include "test_svg.h"
#include "test_json.h"

int main()
{
    testTask3();
    testTaskJsonN();
    return 0;
    testJSON();
    testMain();
    testTaskJson();

    TG::input::InputReader reader;
    TG::catalogue::TransportCatalogue catalogue;
    reader.GetQueries(cin, catalogue);

    TG::stat::StatReader statReader;
    statReader.GetQueries(cin, cout, catalogue);

    return 0;
}
