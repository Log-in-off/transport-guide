#include "input_reader.h"

#include "iostream"

using namespace std;

void InputReader::GetQueries(std::istream &iStream, TransportCatalogue &catalogue)
{
    std::deque <Requst> stops;
    std::deque <Requst> busses;
    int count;
    iStream >> count;
    string input;
    for (int i= 0; i < count; i++)
    {
        getline(iStream, input);
        //cout << input << endl;
        if (input.empty())
            continue;
        size_t head;
        deque <Requst> * requsts;
        if (input.front() == 'B')
        {
            head = sizeof"Bus";
            requsts = &busses;

            //startRequest = input
        }
        else if (input.front() == 'S')
        {
            head = sizeof"Stop";
            requsts = &stops;
        }
        else
            continue;

        string_view startRequest;
        requsts->push_back({std::move(input), startRequest});
        requsts->back().start = requsts->back().text;
        requsts->back().start.remove_prefix(head);
    }

    for(auto &value: stops)
    {
        catalogue.AddStop(value);
    }

    for(auto &value: busses)
    {
        catalogue.AddBus(value);
    }


}
