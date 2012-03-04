//
//  DelaunayView.h
//  Delaunay
//
//  Created by Shu MASUDA on 09/08/10.
//  Copyright 2009 - 2010 Shu MASUDA. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "point.hpp"
#include "triangle.hpp"

#include <vector>

@interface DelaunayView : UIView
{
    std::vector< triangle< point<float> > >     *triangles;
    std::vector< point<float> >                 *vertices;
    BOOL                                        showCircumcircles;
}

@property (nonatomic, assign) BOOL  showCircumcircles;

- (void)setTriangles:(std::vector< triangle< point<float> > >&)aTriangles;
- (void)setVertices:(std::vector< point<float> >&)aVertices;

@end
