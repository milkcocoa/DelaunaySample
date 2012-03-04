//
//  delaunay.hpp
//  Delaunay
//
//  Created by Shu MASUDA on 09/08/09.
//  Copyright 2009 - 2010 Shu MASUDA. All rights reserved.
//
//

#ifndef DELAUNAY_DELAUNAY_HPP
#define DELAUNAY_DELAUNAY_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <numeric>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/signals2.hpp>

namespace
{
    template <typename V>
    struct edge
    {
        typedef V            vertex_type;

        edge()
        {
        }

        edge(vertex_type& v1,
             vertex_type& v2)
        {
            m_v[0] = v1;
            m_v[1] = v2;
        }

        vertex_type& operator[](unsigned int index)
        {
            assert(index < 2);
            return m_v[index];
        }

        vertex_type const& operator[](unsigned int index) const
        {
            assert(index < 2);
            return m_v[index];
        }

        bool operator==(edge<V> const& e) const
        {
            return ((this->m_v[0] == e[0] && this->m_v[1] == e[1]) ||
                    (this->m_v[0] == e[1] && this->m_v[1] == e[0]));
        }

    private:
        vertex_type        m_v[2];
    };
}


template <typename V, typename T>
struct delaunay
{
    typedef V                                   vertex_type;
    typedef T                                   triangle_type;
    typedef edge<vertex_type>                   edge_type;
    typedef typename vertex_type::value_type    value_type;

    void operator()(std::vector<vertex_type>&   vertices,
                    std::vector<triangle_type>& triangles)
    {
        assert(vertices.size() > 3);

        triangle_type t = create_super_triangle(vertices);

        triangles.push_back(t);
        for (int i = 0; i < 3; ++i)
        {
            vertices.push_back(t[i]);
        }

        compute_triangles(vertices, triangles);
        remove_super_vertices(t, triangles);
    }

    boost::signals2::signal<void (delaunay<V, T> const&)>    triangle_created;

private:
    triangle_type create_super_triangle(std::vector<vertex_type>& vertices)
    {
        typedef typename std::vector<vertex_type>::const_iterator        iterator;

        std::pair<iterator, iterator> minmax_x =
            std::minmax_element(vertices.begin(),
                                vertices.end(),
                                boost::bind(&vertex_type::x, _1) < boost::bind(&vertex_type::x, _2));

        std::pair<iterator, iterator> minmax_y =
            std::minmax_element(vertices.begin(),
                                vertices.end(),
                                boost::bind(&vertex_type::y, _1) < boost::bind(&vertex_type::y, _2));

        vertex_type max_vertex(minmax_x.second->x(), minmax_y.second->y());
        vertex_type min_vertex(minmax_x.first->x(), minmax_y.first->y());
        vertex_type middle_vertex   = (max_vertex + min_vertex) / 2.0;
        vertex_type diff_vertex     = max_vertex - min_vertex;
        value_type  max_diff        = std::max(max_vertex.x(), max_vertex.y());

        triangle_type result;
        result[0].set_x(middle_vertex.x() - 20 * max_diff);
        result[0].set_y(middle_vertex.y() - max_diff);

        result[1].set_x(middle_vertex.x());
        result[1].set_y(middle_vertex.y() + 20 * max_diff);

        result[2].set_x(middle_vertex.x() + 20 * max_diff);
        result[2].set_y(middle_vertex.y() - max_diff);

        NSLog(@"Vertex max: (%f, %f)", max_vertex.x(), max_vertex.y());
        NSLog(@"Vertex min: (%f, %f)", min_vertex.x(), min_vertex.y());
        NSLog(@"Vertex difference: (%f, %f)", diff_vertex.x(), diff_vertex.y());
        NSLog(@"Vertex middle: (%f, %f)", middle_vertex.x(), middle_vertex.y());

        return result;
    }

    void compute_triangles(std::vector<vertex_type>&    vertices,
                           std::vector<triangle_type>&  triangles)
    {
        value_type eps = std::numeric_limits<value_type>::epsilon();

        for (vertex_type& v : vertices)
        {
            std::vector<edge_type>    edges;
            std::vector<edge_type>    erased_edges;

            auto t = triangles.begin();
            while (t != triangles.end())
            {
                bool inside = is_inside_circumcircle(v, *t, eps);

                if (inside)
                {
                    edge_type    triangle_edges[3];

                    for (int i = 0; i < 3; ++i)
                    {
                        if ((*t)[i].x() < (*t)[(i + 1) % 3].x())
                        {
                            triangle_edges[i][0] = (*t)[i];
                            triangle_edges[i][1] = (*t)[(i + 1) % 3];
                        }
                        else
                        {
                            triangle_edges[i][0] = (*t)[(i + 1) % 3];
                            triangle_edges[i][1] = (*t)[i];
                        }

                        NSLog(@"Triangle edge: (%f, %f), (%f, %f)", triangle_edges[i][0].x(), triangle_edges[i][0].y(), triangle_edges[i][1].x(), triangle_edges[i][1].y());
                        edges.push_back(triangle_edges[i]);
                    }

                    t = triangles.erase(t);

                    // eraseの戻り値は次のiteratorであるので ++t してしまうと行き過ぎ。
                    continue;
                }

                ++t;
            }

            if (edges.size())
            {
                for (auto i = edges.begin(); i != edges.end(); ++i)
                {
                    for (auto j = i + 1; j != edges.end(); ++j)
                    {
                        if (*i == *j)
                        {
                            NSLog(@"Being erased edge: (%f, %f), (%f, %f)", (*i)[0].x(), (*i)[0].y(), (*i)[1].x(), (*i)[1].y());
                            erased_edges.push_back(*i);
                        }
                    }
                }
            }

            for (edge_type& e : edges)
            {
                if (std::find(erased_edges.begin(), erased_edges.end(), e) != erased_edges.end())
                {
                    NSLog(@"Edge: (%f, %f), (%f, %f)", e[0].x(), e[0].y(), e[1].x(), e[1].y());
                    continue;
                }

                vertex_type v1 = e[0];
                vertex_type v2 = e[1];

                NSLog(@"Created triangle: (%f, %f), (%f, %f), (%f, %f)",
                      v1.x(), v1.y(),
                      v2.x(), v2.y(),
                      v.x(), v.y());

                triangle_type triangle(v1, v2, v);
                triangles.push_back(triangle);

                triangle_created(*this);
            }
        }
    }

    void remove_super_vertices(triangle_type&               super,
                               std::vector<triangle_type>&  triangles)
    {
        for (int i = 0; i < 3; ++i)
        {
            triangles.erase(std::remove_if(triangles.begin(),
                                           triangles.end(),
                                           boost::bind(&triangle_type::contain_vertex, _1, super[i])),
                            triangles.end());
        }
    }
};


#endif  // DELAUNAY_DELAUNAY_HPP
