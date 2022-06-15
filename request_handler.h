#pragma once
#include <optional>
#include <unordered_set>
#include "transport_catalogue.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * В качестве источника для идей предлагаем взглянуть на нашу версию обработчика запросов.
 * Вы можете реализовать обработку запросов способом, который удобнее вам.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

// Класс RequestHandler играет роль Фасада, упрощающего взаимодействие JSON reader-а
// с другими подсистемами приложения.
// См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)
namespace TG{

namespace RH {

class RequestHandler {
public:
    RequestHandler(catalogue::TransportCatalogue& db);
    // MapRenderer понадобится в следующей части итогового проекта
    //RequestHandler(const TransportCatalogue& db, const renderer::MapRenderer& renderer);
    void AddAllStops(const std::deque <catalogue::Requst> &stops);
    void AddAllBuses(const std::deque <catalogue::Requst> &buses);


    //// Возвращает информацию о маршруте (запрос Bus)
    bool GetBusStat(const catalogue::Requst &requst, catalogue::BusInfo &answer) const;
    //std::optional< catalogue::BusInfo> GetBusStat(const std::string_view& bus_name) const;

    //// Возвращает маршруты, проходящие через
    bool GetBusesByStop(const catalogue::Requst &requst, catalogue::StopInfo &answer) const;
    //const std::unordered_set<catalogue::StopInfo>* GetBusesByStop(const std::string_view& stop_name) const;

    std::map <const std::string_view, const catalogue::Bus *> GetBusAndStops();
    // Этот метод будет нужен в следующей части итогового проекта
    //svg::Document RenderMap() const;

private:
    // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
    catalogue::TransportCatalogue& db_;
    //const renderer::MapRenderer& renderer_;
};

}
}
