#include "map_renderer.h"

namespace TG
{
namespace renderer
{

svg::Polyline CreateBusLine(const SphereProjector &proj, const std::vector <domain::Stop*> &stops) {
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

    std::map <const std::string_view, const domain::Bus *> base = transport.GetBuses();
    std::map <const std::string_view, const domain::Stop *> stops = transport.GetUsedStop();

    const SphereProjector proj {stops.begin(), stops.end(), settings->width, settings->height, settings->padding};

     svg::Document doc;
     size_t select_color = 0;
     for (const auto & bus:base)
     {
         if (bus.second->stops.empty())
             continue;

         doc.Add(CreateBusLine(proj, bus.second->stops).SetStrokeWidth(settings->line_width)
                                                       .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                                                       .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                                                       .SetFillColor("none")
                                                       .SetStrokeColor(settings->color_palette.at(select_color)));
         if (++select_color >= settings->color_palette.size())
             select_color = 0;
     }

     select_color = 0;
     for (const auto & bus:base)
     {
         if (bus.second->stops.empty())
             continue;

         MakeLableBus(doc, proj, bus.first, bus.second->stops.front()->coordinates, select_color);

        if (!bus.second->isRoundtrip && !(bus.second->stops.front()->name == bus.second->stops.at(bus.second->stops.size()/2)->name))
            MakeLableBus(doc, proj, bus.first, bus.second->stops.at(bus.second->stops.size()/2)->coordinates, select_color);

        if (++select_color >= settings->color_palette.size())
            select_color = 0;
     }

     for (const auto & stop: stops)
     {
         doc.Add(svg::Circle().SetCenter(proj(stop.second->coordinates))
                              .SetRadius(settings->stop_radius)
                              .SetFillColor("white"));
     }
     for (const auto & stop: stops)
     {
         MakeLableStop(doc, proj, stop.second->name, stop.second->coordinates);
     }

     doc.Render(output);
}

void MapRenderer::MakeLableBus( svg::Document &doc, const SphereProjector &proj, std::string_view nameBus, geo::Coordinates point, size_t numberColor)
{
    doc.Add(svg::Text()
                 .SetStrokeWidth(settings->underlayer_width)
                 .SetFillColor(settings->underlayer_color)
                 .SetStrokeColor(settings->underlayer_color)
                 .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                 .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                 .SetFontFamily("Verdana")
                 .SetPosition(proj(point))
                 .SetOffset({settings->bus_label_offset.x, settings->bus_label_offset.y})
                 .SetFontSize(settings->bus_label_font_size)
                 .SetFontWeight("bold")
                 .SetData(std::string{nameBus}));

    doc.Add(svg::Text()
                .SetFontFamily("Verdana")
                .SetPosition(proj(point))
                .SetOffset({settings->bus_label_offset.x, settings->bus_label_offset.y})
                .SetFontSize(settings->bus_label_font_size)
                .SetFontWeight("bold")
                .SetData(std::string{nameBus})
            .SetFillColor(settings->color_palette.at(numberColor)));
}

void MapRenderer::MakeLableStop(svg::Document &doc, const SphereProjector &proj, std::string_view nameStop, geo::Coordinates point)
{
    doc.Add(svg::Text()
                 .SetStrokeWidth(settings->underlayer_width)
                 .SetFillColor(settings->underlayer_color)
                 .SetStrokeColor(settings->underlayer_color)
                 .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                 .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
                 .SetFontFamily("Verdana")
                 .SetPosition(proj(point))
                 .SetOffset({settings->stop_label_offset.x, settings->stop_label_offset.y})
                 .SetFontSize(settings->stop_label_font_size)
                 .SetData(std::string{nameStop}));

    doc.Add(svg::Text()
                .SetFontFamily("Verdana")
                .SetPosition(proj(point))
                .SetOffset({settings->stop_label_offset.x, settings->stop_label_offset.y})
                .SetFontSize(settings->stop_label_font_size)
                .SetData(std::string{nameStop})
                .SetFillColor("black"));

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
