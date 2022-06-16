#pragma once
#include <iostream>
#include <vector>
#include <set>

#include "geo.h"
namespace TG
{
namespace domain
{

struct Stop
{
    std::string name;
    geo::Coordinates coordinates;
    std::set <std::string_view> buses;
};

struct Request
{
    std::string text;
    std::string_view start;

    Request();
    Request(const std::string &textInput);
    Request(const std::string &text, std::string_view start);
};

struct BusInput {
    std::string name;
    std::string request;
    std::vector <std::string_view> stops;

    BusInput();
    BusInput(const std::string &name, std::string request, std::vector <std::string_view> stops);
};

struct Bus
{
    std::string name;
    std::vector <Stop*> stops;
    uint32_t countUnicStops;
    uint32_t distance;
    double_t curvature;
    bool isRoundtrip;

    Bus();
    Bus(const std::string & name, const std::vector <Stop*> & stops, uint32_t countUnicStops, uint32_t distance, double_t curvature, bool isRoundtrip);
};

struct BusInfo {
    std::string name;
    uint32_t countStops;
    uint32_t countUnicStops;
    uint32_t distance;
    double_t curvature;

    BusInfo();
    BusInfo(const std::string & name, uint32_t countStops, uint32_t countUnicStops, uint32_t distance, double_t curvature);
};

struct StopInfo {
    std::string name;
    std::vector <std::string_view> buses;

    StopInfo();
    StopInfo(const std::string & name, const std::vector <std::string_view> & buses);
};

}
}
