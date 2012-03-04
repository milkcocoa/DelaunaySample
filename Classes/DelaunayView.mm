//
//  DelaunayView.mm
//  Delaunay
//
//  Created by Shu MASUDA on 09/08/10.
//  Copyright 2009 - 2010 Shu MASUDA. All rights reserved.
//

#import "DelaunayView.h"

#include <algorithm>
#include <cmath>
#include <limits>


template <typename T>
T radians(T degrees)
{
    return degrees * M_PI / 180;
}


template <typename T>
void draw_circumcircle(CGContextRef context, T const& triangle)
{
    typedef T                                       triangle_type;
    typedef typename triangle_type::vertex_type     vertex_type;
    typedef typename vertex_type::value_type        value_type;

    value_type x, y, eps, r;

    eps = std::numeric_limits<value_type>::epsilon();
    if (circumcircle_center(triangle, x, y, eps))
    {
        r  = std::sqrt(std::pow(triangle[0].x() - x, 2) + std::pow(triangle[0].y() - y, 2));
        NSLog(@"Circumcircle: %f, %f, %f", x, y, r);

        CGContextSaveGState(context);
        CGContextTranslateCTM(context, x, y);

        CGContextAddArc(context, 0, 0, r, 0, radians(360.0f), 0);
        CGContextStrokePath(context);

        CGContextRestoreGState(context);
    }
}


template <typename T>
void draw_triangle(CGContextRef context, T const& triangle)
{
    NSLog(@"draw_triangle: (%f, %f), (%f, %f), (%f, %f)",
          triangle[0].x(), triangle[0].y(),
          triangle[1].x(), triangle[1].y(),
          triangle[2].x(), triangle[2].y());

    for (int i = 0; i < 3; ++i)
    {
        CGContextMoveToPoint(context, triangle[i].x(), triangle[i].y());
        CGContextAddLineToPoint(context, triangle[(i + 1) % 3].x(), triangle[(i + 1) % 3].y());
        CGContextStrokePath(context);
    }
}


template <typename V>
void draw_vertex(CGContextRef context, V const& vertex)
{
    NSLog(@"draw_vertex: (%f, %f)", vertex.x(), vertex.y());

    CGContextSaveGState(context);
    CGContextTranslateCTM(context, vertex.x(), vertex.y());

    CGContextAddArc(context, 0, 0, 5, 0, radians(360.0f), 0);
    CGContextStrokePath(context);
    CGContextFillEllipseInRect(context, CGRectMake(-5, -5, 10, 10));

    CGContextRestoreGState(context);
}


#pragma mark - DelaunayView () Interface


@interface DelaunayView ()

#pragma mark - Private Methods

- (void)_drawCircumcircles:(CGContextRef)context;
- (void)_drawTriangles:(CGContextRef)context;
- (void)_drawVertices:(CGContextRef)context;

@end


#pragma mark - DelaunayView Implementation


@implementation DelaunayView

@synthesize showCircumcircles;


- (void)_drawCircumcircles:(CGContextRef)context
{
    typedef point<float>            vertex_type;
    typedef triangle<vertex_type>   triangle_type;

    if (!triangles)
    {
        return;
    }

    for (auto triangle : *triangles)
    {
        draw_circumcircle(context, triangle);
    }
}


- (void)_drawTriangles:(CGContextRef)context
{
    typedef point<float>            vertex_type;
    typedef triangle<vertex_type>   triangle_type;

    if (!triangles)
    {
        return;
    }

    for (auto triangle : *triangles)
    {
        draw_triangle(context, triangle);
    }
}


- (void)_drawVertices:(CGContextRef)context
{
    typedef point<float>    vertex_type;

    if (!vertices)
    {
        return;
    }

    for (auto vertex : *vertices)
    {
        draw_vertex(context, vertex);
    }
}


- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();

    CGContextSetRGBFillColor(context, 0.0f, 0.0f, 0.0f, 1.0f);
    CGContextSetLineWidth(context, 2.0f);

    [self _drawVertices:context];
    [self _drawTriangles:context];

    if (showCircumcircles)
    {
        [self _drawCircumcircles:context];
    }
}


- (void)dealloc
{
    delete triangles;
    delete vertices;
}


- (void)setTriangles:(std::vector<triangle<point<float>>>&)aTriangles
{
    if (triangles != &aTriangles)
    {
        delete triangles;
        triangles = &aTriangles;
    }

    [self setNeedsDisplay];
}


- (void)setVertices:(std::vector<point<float>>&)aVertices
{
    if (vertices != &aVertices)
    {
        delete vertices;
        vertices = &aVertices;
    }

    [self setNeedsDisplay];
}


@end
