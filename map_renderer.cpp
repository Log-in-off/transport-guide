#include "map_renderer.h"

/*
 * В этом файле вы можете разместить код, отвечающий за визуализацию карты маршрутов в формате SVG.
 * Визуализация маршрутов вам понадобится во второй части итогового проекта.
 * Пока можете оставить файл пустым.
 */
namespace TG
{
namespace renderer
{

void MapRenderer::SetParamMap(ParamsMap &params)
{

}

void MapRenderer::RenderMap(RH::RequestHandler &transport,  std::ostream& output)
{
    transport.GetBusAndStops();

}

}
}
