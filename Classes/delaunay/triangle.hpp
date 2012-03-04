//
//  triangle.h
//  Delaunay
//
//  Created by Shu MASUDA on 09/08/09.
//  Copyright 2009 - 2010 Shu MASUDA. All rights reserved.
//

#ifndef DELAUNAY_TRIANGLE_HPP
#define DELAUNAY_TRIANGLE_HPP

#include <cassert>
#include <cmath>

#include <boost/optional.hpp>

template <typename V>
struct triangle
{
    typedef V                                    vertex_type;
    typedef typename vertex_type::value_type    value_type;

    triangle()
    {
    }

    triangle(vertex_type& v1,
             vertex_type& v2,
             vertex_type& v3)
    {
        m_v[0] = v1;
        m_v[1] = v2;
        m_v[2] = v3;
    }

    vertex_type& operator[](unsigned int index)
    {
        assert(index < 3);
        return m_v[index];
    }

    vertex_type const& operator[](unsigned int index) const
    {
        assert(index < 3);
        return m_v[index];
    }

    bool contain_vertex(vertex_type& v)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (m_v[i] == v)
            {
                return true;
            }
        }

        return false;
    }

private:
    vertex_type        m_v[3];
};


template <typename V>
typename triangle<V>::value_type area(triangle<V> const& t)
{
    typedef typename triangle<V>::value_type    value_type;

    value_type x[3];
    value_type y[3];

    for (int i = 0; i < 3; ++i)
    {
        x[i] = t[i].x();
        y[i] = t[i].y();
    }

    // \frac{1}{2}\abs(ad - bc) により求める
    x[1] -= x[0];        y[1] -= y[0];
    x[2] -= x[0];        y[2] -= y[0];

    return 0.5 * std::abs(x[1] * y[2] - x[2] * y[1]);
}


template <typename V>
bool circumcircle_center(triangle<V> const&         t,
                         typename V::value_type&    xc,
                         typename V::value_type&    yc,
                         typename V::value_type     eps)
{
    typedef typename V::value_type  value_type;
    // cf.
    // メッシュ生成のプログラミングTIPS
    // http://ums.futene.net/wiki/MESH/A5E1A5C3A5B7A5E5C0B8C0AEA4CEA5D7A5EDA5B0A5E9A5DFA5F3A5B054495053.html
    //
    // ベクトル的解法のヒント
    // http://robotics.me.es.osaka-u.ac.jp/~masutani/Jouki/Kyouzai/hint-vector2d.html
    //
    // 点p0, p1, p2で与えられる、３角形の外接円の中心qは
    // a = |p1 - p2|, b = |p2 - p0|, c = |p0 - p1| とすると
    // q = {a^2(b^2 + c^2 - a^2)p0 + b^2(c^2 + a^2 - b^2)p1 + c^2(a^2 + b^2 - c^2)p2} / (16S^2)
    // で与えられる。
    // ただし、Sは三角形の面積。

    value_type s = area(t);

    if (s < eps)
    {
        return false;
    }

    s = 16 * s * s;

    value_type a2 = std::pow(t[1].x() - t[2].x(), 2) + std::pow(t[1].y() - t[2].y(), 2);
    value_type b2 = std::pow(t[2].x() - t[0].x(), 2) + std::pow(t[2].y() - t[0].y(), 2);
    value_type c2 = std::pow(t[0].x() - t[1].x(), 2) + std::pow(t[0].y() - t[1].y(), 2);

    value_type ta = a2 * (b2 + c2 - a2);
    value_type tb = b2 * (c2 + a2 - b2);
    value_type tc = c2 * (a2 + b2 - c2);

    xc    = ta * t[0].x();
    xc += tb * t[1].x();
    xc += tc * t[2].x();
    xc /= s;

    yc    = ta * t[0].y();
    yc += tb * t[1].y();
    yc += tc * t[2].y();
    yc /= s;

    return true;
}


template <typename V>
bool is_inside_circumcircle(V const&                v,
                            triangle<V> const&      t,
                            typename V::value_type  eps)
{
    typedef V                                   vertex_type;
    typedef typename vertex_type::value_type    value_type;

    value_type xc, yc;
    if (!circumcircle_center(t, xc, yc, eps))
    {
        return false;
    }

    value_type r2 = std::pow(t[0].x() - xc, 2) + std::pow(t[0].y() - yc, 2);
    value_type d2 = std::pow(v.x() - xc, 2) + std::pow(v.y() - yc, 2);

    return d2 <= r2;
}


#endif  // DELAUNAY_TRIANGLE_HPP
