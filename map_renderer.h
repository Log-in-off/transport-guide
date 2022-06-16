#pragma once

#include "request_handler.h"
#include "geo.h"
#include "svg.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <vector>

namespace TG
{
namespace  renderer
{

inline const double EPSILON = 1e-6;
bool IsZero(double value);

class SphereProjector {
public:
    // points_begin и points_end задают начало и конец интервала элементов geo::Coordinates
    template <typename PointInputIt>
    SphereProjector(PointInputIt points_begin, PointInputIt points_end,
                    double max_width, double max_height, double padding);

    // Проецирует широту и долготу в координаты внутри SVG-изображения
    svg::Point operator()(geo::Coordinates coords) const;

private:
    double padding_;
    double min_lon_ = 0;
    double max_lat_ = 0;
    double zoom_coeff_ = 0;
};

template <typename PointInputIt>
SphereProjector::SphereProjector(PointInputIt points_begin, PointInputIt points_end,
                double max_width, double max_height, double padding)
    : padding_(padding) //
{
    // Если точки поверхности сферы не заданы, вычислять нечего
    if (points_begin == points_end) {
        return;
    }

    // Находим точки с минимальной и максимальной долготой
    const auto [left_it, right_it] = std::minmax_element(
        points_begin, points_end,
        [](auto lhs, auto rhs) { return lhs.second->coordinates.lng < rhs.second->coordinates.lng; });
    min_lon_ = (*left_it).second->coordinates.lng;
    const double max_lon = (*right_it).second->coordinates.lng;

    // Находим точки с минимальной и максимальной широтой
    const auto [bottom_it, top_it] = std::minmax_element(
        points_begin, points_end,
        [](auto lhs, auto rhs) { return lhs.second->coordinates.lat < rhs.second->coordinates.lat; });
    const double min_lat = (*bottom_it).second->coordinates.lat;
    max_lat_ = (*top_it).second->coordinates.lat;

    // Вычисляем коэффициент масштабирования вдоль координаты x
    std::optional<double> width_zoom;
    if (!IsZero(max_lon - min_lon_)) {
        width_zoom = (max_width - 2 * padding) / (max_lon - min_lon_);
    }

    // Вычисляем коэффициент масштабирования вдоль координаты y
    std::optional<double> height_zoom;
    if (!IsZero(max_lat_ - min_lat)) {
        height_zoom = (max_height - 2 * padding) / (max_lat_ - min_lat);
    }

    if (width_zoom && height_zoom) {
        // Коэффициенты масштабирования по ширине и высоте ненулевые,
        // берём минимальный из них
        zoom_coeff_ = std::min(*width_zoom, *height_zoom);
    } else if (width_zoom) {
        // Коэффициент масштабирования по ширине ненулевой, используем его
        zoom_coeff_ = *width_zoom;
    } else if (height_zoom) {
        // Коэффициент масштабирования по высоте ненулевой, используем его
        zoom_coeff_ = *height_zoom;
    }
}

struct CoordinatePoint
{
    double x;
    double y;
};

struct ParamsMap
{
    double width;
    double height;
    double padding;
    double line_width;
    double stop_radius;
    int bus_label_font_size;
    CoordinatePoint bus_label_offset;
    int stop_label_font_size;
    CoordinatePoint stop_label_offset;
    double underlayer_width;
    svg::Color underlayer_color;
    std::vector<svg::Color> color_palette;
};


class MapRenderer
{
public:
    void SetParamMap(ParamsMap &params);
    void RenderMap(RH::RequestHandler &transport, std::ostream &output);
private:
    void MakeLabelBus(svg::Document &doc, const SphereProjector &proj, std::string_view nameBus, geo::Coordinates point, size_t numberColor);
    void MakeLabelStop(svg::Document &doc, const SphereProjector &proj, std::string_view nameStop, geo::Coordinates point);
    std::optional<ParamsMap> settings;

};

}
}
