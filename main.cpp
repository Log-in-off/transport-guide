#include <iostream>
#include <sstream>

using namespace std;

#include "json_reader.h"
#include "map_renderer.h"

//int main() {
//    using namespace std;
//     TG::catalogue::TransportCatalogue catalogue;
//         TG::ReaderJSON reader;
//         TG::RH::RequestHandler transport(catalogue);
//         TG::renderer::MapRenderer map;
//         reader.GetQueries(cin, cout, transport, &map);
//         map.RenderMap(transport, cout);
//}


#include "test_map_renderer.h"
#include "test_json.h"
#include "tests.h"
int main()
{
    testTaskGraph1();
    return 0;
    testRender2_1();
    testRender3_1();
    testJSON();
    testRender();
    testRender1();
    testRender2();
}


/*
#include <iostream>
#include "json_reader.h"

int main() {
    using namespace std;
    TG::catalogue::TransportCatalogue catalogue;
    TG::ReaderJSON reader;
    TG::RH::RequestHandler transport(catalogue);
    TG::renderer::MapRenderer map;
    reader.GetQueries(cin, cout, transport, &map);
}
*/
