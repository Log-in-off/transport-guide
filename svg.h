#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <deque>
#include <optional>
#include <variant>

namespace svg {


struct Rgb {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    Rgb(uint8_t redI, uint8_t greenI, uint8_t blueI);
    Rgb();
};

std::ostream & operator<< (std::ostream &out, const Rgb & color);

struct Rgba {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    double opacity;
    Rgba(uint8_t redI, uint8_t greenI, uint8_t blueI, double opacityI);
    Rgba();

};
std::ostream & operator<< (std::ostream &out, const Rgba & color);

std::ostream & operator<< (std::ostream &out, const std::monostate & color);

using Color = std::variant<std::monostate, std::string, Rgb, Rgba>;
inline const Color NoneColor{"none"};

std::ostream & operator<< (std::ostream &out, const Color &color);

enum class StrokeLineCap {
    BUTT,
    ROUND,
    SQUARE,
};

std::ostream & operator<< (std::ostream &out, const StrokeLineCap &obj);


enum class StrokeLineJoin {
    ARCS,
    BEVEL,
    MITER,
    MITER_CLIP,
    ROUND,
};

std::ostream & operator<< (std::ostream &out, const StrokeLineJoin &obj);

template <typename Owner>
class PathProps {
public:
    Owner & SetFillColor(Color newColor)
    {
        fillColor_ = move(newColor);
        return AsOwner();
    }
    Owner & SetStrokeColor(Color newColor)
    {
        strokeColor_ = move(newColor);
        return AsOwner();
    }
    Owner & SetStrokeWidth(double width)
    {
        width_ = width;
        return AsOwner();
    }
    Owner & SetStrokeLineCap(StrokeLineCap line_cap)
    {
        line_cap_ = line_cap;
        return AsOwner();
    }
    Owner & SetStrokeLineJoin(StrokeLineJoin line_join)
    {
        line_join_ = line_join;
        return AsOwner();
    }

protected:
    ~PathProps() = default;
    void RenderAttrs(std::ostream& out) const {
        using namespace std::literals;

        if (!std::holds_alternative<std::monostate>(fillColor_)) {
            out << " fill=\""sv << fillColor_ << "\""sv;
        }
        if (!std::holds_alternative<std::monostate>(strokeColor_)) {
            out << " stroke=\""sv << strokeColor_ << "\""sv;
        }
        if (width_) {
            out << " stroke-width=\""sv << *width_ << "\""sv;
        }
        if (line_cap_)
        {
            out << " stroke-linecap=\""sv << *line_cap_ << "\""sv;
        }
        if (line_join_)
        {
            out << " stroke-linejoin=\""sv << *line_join_ << "\""sv;
        }
    }
private:
    Owner& AsOwner() {
            // static_cast безопасно преобразует *this к Owner&,
            // если класс Owner — наследник PathProps
            return static_cast<Owner&>(*this);
        }
    Color fillColor_ ;
    Color strokeColor_;
    std::optional<double> width_;
    std::optional<StrokeLineCap> line_cap_;
    std::optional<StrokeLineJoin> line_join_;

};

struct Point {
    Point() = default;
    Point(double x, double y)
        : x(x)
        , y(y) {
    }
    double x = 0;
    double y = 0;
};

/*
 * Вспомогательная структура, хранящая контекст для вывода SVG-документа с отступами.
 * Хранит ссылку на поток вывода, текущее значение и шаг отступа при выводе элемента
 */
struct RenderContext {
    RenderContext(std::ostream& out)
        : out(out) {
    }

    RenderContext(std::ostream& out, int indent_step, int indent = 0)
        : out(out)
        , indent_step(indent_step)
        , indent(indent) {
    }

    RenderContext Indented() const {
        return {out, indent_step, indent + indent_step};
    }

    void RenderIndent() const {
        for (int i = 0; i < indent; ++i) {
            out.put(' ');
        }
    }

    std::ostream& out;
    int indent_step = 0;
    int indent = 0;
};

/*
 * Абстрактный базовый класс Object служит для унифицированного хранения
 * конкретных тегов SVG-документа
 * Реализует паттерн "Шаблонный метод" для вывода содержимого тега
 */
class Object {
public:
    void Render(const RenderContext& context) const;

    virtual ~Object() = default;

private:
    virtual void RenderObject(const RenderContext& context) const = 0;
};

/*
 * Класс Circle моделирует элемент <circle> для отображения круга
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
 */
class Circle final : public Object, public PathProps<Circle>  {
public:
    Circle() = default;
    Circle& SetCenter(Point center);
    Circle& SetRadius(double radius);

private:
    void RenderObject(const RenderContext& context) const override;

    Point center_ = {0.0, 0.0};
    double radius_ = 1.0;
};

/*
 * Класс Polyline моделирует элемент <polyline> для отображения ломаных линий
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
 */
class Polyline : public Object, public PathProps<Polyline> {
public:
    Polyline() = default;
    // Добавляет очередную вершину к ломаной линии
    Polyline& AddPoint(Point point);

private:
    void RenderObject(const RenderContext& context) const override;
    std::deque <Point> points_;
};

/*
 * Класс Text моделирует элемент <text> для отображения текста
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
 */
class Text : public Object, public PathProps<Text> {
public:
    Text() = default;
    // Задаёт координаты опорной точки (атрибуты x и y)
    Text& SetPosition(Point pos);

    // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
    Text& SetOffset(Point offset);

    // Задаёт размеры шрифта (атрибут font-size)
    Text& SetFontSize(uint32_t size);

    // Задаёт название шрифта (атрибут font-family)
    Text& SetFontFamily(std::string font_family);

    // Задаёт толщину шрифта (атрибут font-weight)
    Text& SetFontWeight(std::string font_weight);

    // Задаёт текстовое содержимое объекта (отображается внутри тега text)
    Text& SetData(std::string data);
private:
    void RenderObject(const RenderContext& context) const override;

    Point pos_ = {0.0, 0.0};
    Point offset_ = {0.0, 0.0};
    uint32_t size_ = 1;
    std::string font_family_ = "";
    std::string font_weight_ = "";
    std::string data_ = "";

};

class ObjectContainer {
public:
    virtual ~ObjectContainer(){}
    template <typename Obj>
    void Add(Obj obj);

    virtual void AddPtr(std::unique_ptr<Object> &&ptr) = 0;
    // Прочие методы рисования графических примитивов
    // ...
};

template <typename Obj>
void ObjectContainer::Add(Obj obj) {
    AddPtr(std::make_unique<Obj>(std::move(obj)));
}


class Document : public ObjectContainer{
public:

    // Добавляет в svg-документ объект-наследник svg::Object
    void AddPtr(std::unique_ptr<Object> &&ptr);

    // Выводит в ostream svg-представление документа
    void Render(std::ostream& out) const;

private:
    std::deque < std::unique_ptr<Object>> objects_;

};

class Drawable {
public:
    virtual void Draw(ObjectContainer& g) const = 0;
    virtual ~Drawable() {}
};



}  // namespace svg
