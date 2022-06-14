#pragma once

#include "request_handler.h"

namespace TG
{
namespace  renderer
{
struct ParamsMap
{
    int a;
};

class MapRenderer
{
public:
    void SetParamMap(ParamsMap &params);
    void RenderMap(RH::RequestHandler &transport, std::ostream &output);
};

}
}

/*
 * В этом файле вы можете разместить код, отвечающий за визуализацию карты маршрутов в формате SVG.
 * Визуализация маршрутов вам понадобится во второй части итогового проекта.
 * Пока можете оставить файл пустым.
 */
