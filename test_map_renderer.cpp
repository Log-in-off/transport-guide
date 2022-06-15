#include "test_map_renderer.h"

#include <iostream>
#include <fstream>
#include "json_reader.h"
#include "map_renderer.h"

#include <iomanip>
void testRender1()
{
     using namespace std;
     ifstream ifs("input1.json");
     if( ifs.is_open())
     {
         TG::catalogue::TransportCatalogue catalogue;
         TG::ReaderJSON reader;
         TG::RH::RequestHandler transport(catalogue);
         TG::renderer::MapRenderer map;
         reader.GetQueries(ifs, cout, transport, &map);
         map.RenderMap(transport, cout);
     }
}

void testRender()
{
     using namespace std;
     ifstream ifs("input.json");
     if( ifs.is_open())
     {
         TG::catalogue::TransportCatalogue catalogue;
         TG::ReaderJSON reader;
         TG::RH::RequestHandler transport(catalogue);
         TG::renderer::MapRenderer map;
         reader.GetQueries(ifs, cout, transport, &map);
         map.RenderMap(transport, cout);
     }
}

void testRender2()
{
     using namespace std;
     ifstream ifs("input2.json");
     if( ifs.is_open())
     {
         TG::catalogue::TransportCatalogue catalogue;
         TG::ReaderJSON reader;
         TG::RH::RequestHandler transport(catalogue);
         TG::renderer::MapRenderer map;
         reader.GetQueries(ifs, cout, transport, &map);
         map.RenderMap(transport, cout);
     }
}
