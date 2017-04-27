#ifndef SHAPES_H
#define SHAPES_H

#ifdef TEST1
template<typename T, typename U>
void intersect(T &left, U &right)
{
    std::cout << "Intersecting "
              << typeid(T).name() << " and "
              << typeid(U).name()
              << std::endl;
}
#endif

struct shape;
struct point;
struct rectangle;
struct circle;

#ifdef TEST3

#include "dispatcher.h"


void intersect(shape &sh1, shape &sh2)
{
    double_dispatcher<shape>::call(sh1, sh2);
}

void shape_init_intersect_dd()
{
    std::function<void (point&, point&)> f1 = [](point& s, point& s2)
    {
        std::cout << "point point" << std::endl;
    };

    std::function<void(point&, rectangle&)> f2 = [](point& s, rectangle& s2)
    {
        std::cout << "point rectangle" << std::endl;
    };

    std::function<void(point&, circle&)> f3 = [](point& s, circle& s2)
    {
        std::cout << "point circle" << std::endl;
    };

    std::function<void(rectangle&, circle&)> f4 = [](rectangle& s, circle& s2)
    {
        std::cout << "rectangle circle" << std::endl;
    };

    std::function<void(rectangle&, rectangle&)> f5 = [](rectangle& s, rectangle& s2)
    {
        std::cout << "rectangle rectangle" << std::endl;
    };

    std::function<void(circle&, circle&)> f6 = [](circle& s, circle& s2)
    {
        std::cout << "circle circle" << std::endl;
    };

    double_dispatcher<shape>::reg(f1);
    double_dispatcher<shape>::reg(f2);
    double_dispatcher<shape>::reg(f3);
    double_dispatcher<shape>::reg(f4);
    double_dispatcher<shape>::reg(f5);
    double_dispatcher<shape>::reg(f6);
}
#endif

struct shape
{
    shape(double x, double y)
        : x_(x)
        , y_(y)
    {}

#ifdef TEST1
    virtual void intersect_impl(shape&     sh) = 0;
    virtual void intersect_impl(point&     p)  = 0;
    virtual void intersect_impl(rectangle& r)  = 0;
    virtual void intersect_impl(circle&    c)  = 0;
#endif
#ifdef TEST3
    virtual void foo() {}
#endif
protected:
    double x_;
    double y_;
};

#ifdef TEST1
void intersect(shape& sh1, shape &sh2)
{
    sh1.intersect_impl(sh2);
}
#endif

struct point : shape
{
    point(double x, double y)
        : shape(x, y)
    {}

#ifdef TEST1
    virtual void intersect_impl(shape& sh) override
    {
        sh.intersect_impl(*this);
    }

    virtual void intersect_impl(point& p) override
    {
        intersect<point, point>(*this, p);
    }

    virtual void intersect_impl(rectangle& r) override
    {
        intersect<point, rectangle>(*this, r);
    }

    virtual void intersect_impl(circle& c) override
    {
        intersect<point, circle>(*this, c);
    }
#endif
};

struct rectangle : shape
{
    rectangle(double left_x, double top_y,
              double width, double height)
        : shape(left_x, top_y)
        , width_(width)
        , height_(height)
    {}

#ifdef TEST1
    virtual void intersect_impl(shape& sh) override
    {
        sh.intersect_impl(*this);
    }

    virtual void intersect_impl(point& p) override
    {
        intersect<rectangle, point>(*this, p);
    }

    virtual void intersect_impl(rectangle& r) override
    {
        intersect<rectangle, rectangle>(*this, r);
    }

    virtual void intersect_impl(circle& c) override
    {
        intersect<rectangle, circle>(*this, c);
    }
#endif
private:
    double width_;
    double height_;
};

struct circle : shape
{
    circle(double center_x, double center_y, double radius)
        : shape(center_x, center_y)
    {}

#ifdef TEST1
    virtual void intersect_impl(shape& sh) override
    {
        sh.intersect_impl(*this);
    }

    virtual void intersect_impl(point& p) override
    {
        intersect<circle, point>(*this, p);
    }

    virtual void intersect_impl(rectangle& r) override
    {
        intersect<circle, rectangle>(*this, r);
    }

    virtual void intersect_impl(circle& c) override
    {
        intersect<circle, circle>(*this, c);
    }
#endif
private:
    double radius_;
};

#endif // SHAPES_H
