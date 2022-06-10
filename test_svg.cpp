#include "test_svg.h"

#include "svg.h"
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES

//using namespace svg;

svg::Polyline CreateStar(svg::Point center, double outer_rad, double inner_rad, int num_rays) {
    svg::Polyline polyline;
    for (int i = 0; i <= num_rays; ++i) {
        double angle = 2 * M_PI * (i % num_rays) / num_rays;
        polyline.AddPoint({center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle)});
        if (i == num_rays) {
            break;
        }
        angle += M_PI / num_rays;
        polyline.AddPoint({center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle)});
    }
    return polyline;
}

// Выводит приветствие, круг и звезду
void DrawPicture() {
    using namespace std::literals;
    svg::Document doc;
    doc.Add(svg::Circle().SetCenter({20, 20}).SetRadius(10));
    doc.Add(svg::Text()
                .SetFontFamily("Verdana"s)
                .SetPosition({35, 20})
                .SetOffset({0, 6})
                .SetFontSize(12)
                .SetFontWeight("bold"s)
                .SetData("Hello C++"s));
    doc.Add(CreateStar({20, 50}, 10, 5, 5));
    doc.Render(std::cout);
}

void TestText()
{
    using namespace std::literals;
    svg::Document doc;
    doc.Add(svg::Text()
                .SetFontFamily("Verdana"s)
                .SetPosition({35, 20})
                .SetOffset({0, 6})
                .SetFontSize(12)
                .SetData("Hello, <UserName>. Would you like some \"M&M\'s\"?"s));

    doc.Render(std::cout);
}


void testSVG(void)
{
    using namespace std::literals;
    using namespace svg;

    DrawPicture();
    TestText();
}

namespace shapes {

class Triangle : public svg::Drawable {
public:
    Triangle(svg::Point p1, svg::Point p2, svg::Point p3)
        : p1_(p1)
        , p2_(p2)
        , p3_(p3) {
    }

    // Реализует метод Draw интерфейса svg::Drawable
    void Draw(svg::ObjectContainer& container) const override {
        container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
    }

private:
    svg::Point p1_, p2_, p3_;
};

class Star : public svg::Drawable {
public:
    Star(svg::Point center, double_t outer_radius, double_t inner_radius, int num_rays)
        : center_(center)
        , outer_radius_(outer_radius)
        , inner_radius_(inner_radius)
        , num_rays_(num_rays) {}

    void Draw(svg::ObjectContainer& container) const override {
        container.Add(::CreateStar(center_, outer_radius_, inner_radius_, num_rays_).SetFillColor("red").SetStrokeColor("black"));
    }

private:
    svg::Point center_;
    double_t outer_radius_;
    double_t inner_radius_;
    int num_rays_;
};
class Snowman : public svg::Drawable{
public:
    Snowman(svg::Point head_center, double_t head_radius)
        : head_center_(head_center)
        , head_radius_(head_radius) {}

    void Draw(svg::ObjectContainer& container) const override {
        const svg::Point foot_center = {head_center_.x, head_center_.y + 5 * head_radius_};
        const svg::Point midle_center = {head_center_.x, head_center_.y + 2 * head_radius_};
        container.Add(svg::Circle().SetCenter(foot_center).SetRadius(2*head_radius_).SetFillColor("rgb(240,240,240)").SetStrokeColor("black"));
        container.Add(svg::Circle().SetCenter(midle_center).SetRadius(1.5*head_radius_).SetFillColor("rgb(240,240,240)").SetStrokeColor("black"));
        container.Add(svg::Circle().SetCenter(head_center_).SetRadius(head_radius_).SetFillColor("rgb(240,240,240)").SetStrokeColor("black"));
    }
private:
    svg::Point head_center_;
    double_t head_radius_;

};

} // namespace shapes




template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}


void testPicture(void)
{
    using namespace svg;
    using namespace shapes;
    using namespace std;

    vector<unique_ptr<svg::Drawable>> picture;

    picture.emplace_back(make_unique<Triangle>(Point{100, 20}, Point{120, 50}, Point{80, 40}));
    // 5-лучевая звезда с центром {50, 20}, длиной лучей 10 и внутренним радиусом 4
    picture.emplace_back(make_unique<Star>(Point{50.0, 20.0}, 10.0, 4.0, 5));
    // Снеговик с "головой" радиусом 10, имеющей центр в точке {30, 20}
    picture.emplace_back(make_unique<Snowman>(Point{30, 20}, 10.0));

    svg::Document doc;
    // Так как документ реализует интерфейс ObjectContainer,
    // его можно передать в DrawPicture в качестве цели для рисования
    DrawPicture(picture, doc);

    // Выводим полученный документ в stdout
    doc.Render(cout);

}
