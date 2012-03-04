//
//  point.hpp
//  Delaunay
//
//  Created by Shu MASUDA on 09/08/09.
//  Copyright 2009 - 2010 Shu MASUDA. All rights reserved.
//

#ifndef DELAUNAY_POINT_HPP
#define DELAUNAY_POINT_HPP

template <typename T>
struct point
{
    typedef T    value_type;

    point()
    {
    }

    point(value_type x, value_type y)
    : m_x(x), m_y(y)
    {
    }

    bool operator==(point<T> const& p) const
    {
        return (m_x == p.x() && m_y == p.y());
    }

    value_type x() const
    {
        return m_x;
    }

    value_type y() const
    {
        return m_y;
    }

    void set_x(value_type const& x)
    {
        m_x = x;
    }

    void set_y(value_type const& y)
    {
        m_y = y;
    }

    point<T>& operator+(point<T> const& rhs)
    {
        m_x += rhs.x();
        m_y += rhs.y();

        return *this;
    }

    point<T>& operator-(point<T> const& rhs)
    {
        m_x -= rhs.x();
        m_y -= rhs.y();

        return *this;
    }

    point<T>& operator/(T const& rhs)
    {
        m_x /= rhs;
        m_y /= rhs;

        return *this;
    }
private:
    value_type    m_x;
    value_type    m_y;
};

#endif  // DELAUNAY_POINT_HPP
