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


svg::Polyline CreateBusLine(const SphereProjector &proj, const std::vector <catalogue::Stop*> &stops) {
    svg::Polyline polyline;
    for (const auto & stop: stops)
    {
        polyline.AddPoint(proj(stop->coordinates));
    }

    return polyline;
}

void MapRenderer::SetParamMap(ParamsMap &params)
{
    settings = std::move(params);
}

void MapRenderer::RenderMap(RH::RequestHandler &transport,  std::ostream& output)
{
    if (!settings)
        return;

    std::map <const std::string_view, const catalogue::Bus *> base = transport.GetBusAndStops();
    std::unordered_set<const geo::Coordinates*> pointOnMap;
    for (const auto & bus:base)
    {
        for (const auto & stop:bus.second->stops)
        {
            pointOnMap.insert(&stop->coordinates);
        }
    }
    const SphereProjector proj {pointOnMap.begin(), pointOnMap.end(), settings->width, settings->height, settings->padding};

     svg::Document doc;
     size_t select_color = 0;
     for (const auto & bus:base)
     {

         doc.Add(CreateBusLine(proj, bus.second->stops).SetStrokeWidth(settings->line_width)
                                                       .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                                                       .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                                                       .SetFillColor("none")
                                                       .SetStrokeColor(settings->color_palette.at(select_color)));
         if (++select_color >= settings->color_palette.size())
             select_color = 0;
     }
     doc.Render(output);
}

bool IsZero(double value) {
    return std::abs(value) < EPSILON;
}

svg::Point SphereProjector::operator()(geo::Coordinates coords) const {
    return {
        (coords.lng - min_lon_) * zoom_coeff_ + padding_,
                (max_lat_ - coords.lat) * zoom_coeff_ + padding_
    };
}

}
}
