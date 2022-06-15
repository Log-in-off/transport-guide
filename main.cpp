#include <iostream>
#include <sstream>

using namespace std;

#include "input_reader.h"
#include "stat_reader.h"
//#include "tests.h"
//#include "test_svg.h"
//#include "test_json.h"
//#include "test_map_renderer.h"
#include "json_reader.h"

int main()
{

    TG::catalogue::TransportCatalogue catalogue;
    TG::ReaderJSON reader;
    TG::RH::RequestHandler transport(catalogue);
    TG::renderer::MapRenderer map;
    reader.GetQueries(cin, cout, transport, &map);
    map.RenderMap(transport, cout);

    return 0;
}
