#include "svg.h"
#include <sstream>
#include <string_view>

namespace svg {

using namespace std::literals;

Rgb::Rgb(uint8_t redI, uint8_t greenI, uint8_t blueI):red(redI), green(greenI), blue(blueI){}
Rgb::Rgb():red(0), green(0), blue(0){}

Rgba::Rgba(uint8_t redI, uint8_t greenI, uint8_t blueI, double opacityI):red(redI), green(greenI), blue(blueI), opacity(opacityI){}
Rgba::Rgba():red(0), green(0), blue(0), opacity(1.0){}

std::ostream & operator<< (std::ostream &out, const Rgb & color)
{
    out << "rgb("s << static_cast<int>(color.red) << ","s << static_cast<int>(color.green) << ","s << static_cast<int>(color.blue) << ")"s;
    return out;
}

std::ostream & operator<< (std::ostream &out, const Rgba & color)
{
    out << "rgba("s << static_cast<int>(color.red) << ","s << static_cast<int>(color.green) << ","s << static_cast<int>(color.blue) << ","s<< color.opacity << ")"s;
    return out;
}

std::ostream & operator<< (std::ostream &out, const std::monostate & )
{
    out << NoneColor;
    return out;
}


std::ostream & operator<< (std::ostream &out, const  Color &color)
{
    std::visit( [&out](auto value) {out << value;}, color);
    return out;
}

std::ostream &operator<<(std::ostream &out, const StrokeLineCap &obj)
{
    switch (obj) {
    case StrokeLineCap::BUTT:
        out << "butt"sv;
        break;
    case StrokeLineCap::ROUND:
        out << "round"sv;
        break;
    case StrokeLineCap::SQUARE:
        out << "square"sv;
        break;
    default:
        break;
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const StrokeLineJoin &obj)
{
    switch (obj) {
    case StrokeLineJoin::ARCS:
        out << "arcs"sv;
        break;
    case StrokeLineJoin::BEVEL:
        out << "bevel"sv;
        break;
    case StrokeLineJoin::MITER:
        out << "miter"sv;
        break;
    case StrokeLineJoin::MITER_CLIP:
        out << "miter-clip"sv;
        break;
    case StrokeLineJoin::ROUND:
        out << "round"sv;
        break;
    default:
        break;
    }
    return out;
}

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);

    context.out << std::endl;
}

Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\""sv;
    RenderAttrs(context.out);
    out << "/>"sv;
}

Polyline &Polyline::AddPoint(Point point)
{
    points_.push_back(point);
    return *this;
}

void Polyline::RenderObject(const RenderContext &context) const
{
    auto& out = context.out;
    out << "<polyline points=\""sv;
    bool next = false;
    for (const auto & point: points_)
    {
        if (next)
            out << " "sv;
        next = true;
        out << point.x << ","sv << point.y;
    }
    out << "\""sv;
    RenderAttrs(context.out);
    out << "/>"sv;
}

void Text::RenderObject(const RenderContext &context) const
{
    auto& out = context.out;
    out << "<text"sv;
    RenderAttrs(context.out);

    out << " x=\""sv << pos_.x << "\" y=\""sv << pos_.y << "\""sv;
    out << " dx=\""sv << offset_.x << "\" dy=\""sv << offset_.y << "\""sv;
    out << " font-size=\""sv << size_ << "\""sv;
    if (!font_family_.empty())
        out << " font-family=\""sv << font_family_ << "\""sv;
    if (!font_weight_.empty())
        out << " font-weight=\""sv << font_weight_ << "\""sv;

    out << ">"sv;
    size_t newFind = 0;
    const std::string service_symbols = "\"\'><&";
    size_t lastFind  = newFind;
    while (true) {
        newFind = data_.find_first_of(service_symbols, newFind + 1);
        if (newFind == std::string::npos)
        {
            out << data_.substr(lastFind, data_.size()-lastFind);
            break;
        }

        out << data_.substr(lastFind, newFind-lastFind);
        switch (data_.at(newFind))
        {
            case '\"':
            {
                out << "&quot;"sv;
                break;
            }
            case '\'':
            {
                out << "&apos;"sv;
                break;
            }
            case '<':
            {
                out << "&lt;"sv;
                break;
            }
            case '>':
            {
                out << "&gt;"sv;
                break;
            }
            case '&':
            {
                out << "&amp;"sv;
                break;
            }
            default:
                break;
        }
        lastFind  = newFind + 1;
    }
    out << "</text>"sv;
}

Text &Text::SetPosition(Point pos)
{
    pos_ = pos;
    return *this;
}

Text &Text::SetOffset(Point offset)
{
    offset_ = offset;
    return *this;
}

Text &Text::SetFontSize(uint32_t size)
{
    size_ = size;
    return *this;
}

Text &Text::SetFontFamily(std::string font_family)
{
    font_family_ = font_family;
    return *this;
}

Text &Text::SetFontWeight(std::string font_weight)
{
    font_weight_ = font_weight;
    return *this;
}

Text &Text::SetData(std::string data)
{
    data_ = data;
    return *this;
}

void Document::AddPtr(std::unique_ptr<Object> &&ptr)
{
    objects_.emplace_back(std::move(ptr));
}

void Document::Render(std::ostream &out) const
{
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;
    RenderContext ctx(out, 2, 2);

    for (auto &str : objects_) {
        str->Render(ctx);
    }
    out << "</svg>"sv;
}

RenderContext::RenderContext(std::ostream &out)
    : out(out) {
}

RenderContext::RenderContext(std::ostream &out, int indent_step, int indent)
    : out(out)
    , indent_step(indent_step)
    , indent(indent) {
}

RenderContext RenderContext::Indented() const {
    return {out, indent_step, indent + indent_step};
}

void RenderContext::RenderIndent() const {
    for (int i = 0; i < indent; ++i) {
        out.put(' ');
    }
}



}  // namespace svg
