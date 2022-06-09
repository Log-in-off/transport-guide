#include "svg.h"
#include <sstream>

namespace svg {

using namespace std::literals;

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);

    context.out << std::endl;
}

// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    //<circle cx="600" cy="200" r="100"
    //        fill="red" stroke="blue" stroke-width="10"  />
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\" "sv;
    out << "/>"sv;
}

Polyline &Polyline::AddPoint(Point point)
{
    points_.push_back(point);
    return *this;
}

void Polyline::RenderObject(const RenderContext &context) const
{
   // <polyline fill="none" stroke="blue" stroke-width="10"
   //             points="50,375
   //                     150,375 150,325 250,325 250,375
   //                     350,375 350,250 450,250 450,375
   //                     550,375 550,175 650,175 650,375
   //                     750,375 750,100 850,100 850,375
   //                     950,375 950,25 1050,25 1050,375
   //                     1150,375" />
    auto& out = context.out;
    out << "<polyline points=\"";
    bool next = false;
    for (const auto & point: points_)
    {
        if (next)
            out << " ";
        next = true;
        out << point.x << "," << point.y;
    }
    out << "\" "sv;
    out << "/>"sv;
}

void Text::RenderObject(const RenderContext &context) const
{
    //<text x="250" y="180" dy="100"
    //        font-family="Verdana" font-size="64" fill="blue" >
    //    Hello, out there!
    //  </text>
    if (data_.empty())
        return;

    auto& out = context.out;
    out << "<text x=\""sv << pos_.x << "\" y=\"" << pos_.y << "\"";
    out << " dx=\""sv << offset_.x << "\" dy=\"" << offset_.y << "\"";
    out << " font-size=\"" << size_ << "\"";
    if (!font_family_.empty())
        out << " font-family=\"" << font_family_ << "\"";
    if (!font_weight_.empty())
        out << " font-weight=\"" << font_weight_ << "\"";
    out << ">" << data_<< "</text>";
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

void Document::Render(std::ostream &out) const
{
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv;
    out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv;

    for (const auto & obj : objects_)
    {
        std::stringstream buf;
        RenderContext render(buf);
        obj.get()->Render(render);
        std::cout << buf.str() << std::endl;
    }

    out << "</svg>"sv;


}



}  // namespace svg
