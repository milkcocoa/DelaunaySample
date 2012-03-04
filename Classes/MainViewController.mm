//
//  MainViewController.mm
//  Delaunay
//
//  Created by Shu MASUDA on 09/08/10.
//  Copyright 2009 - 2010 Shu MASUDA. All rights reserved.
//

#import "MainViewController.h"

#include <random>

#import "DelaunayView.h"

#include "delaunay.hpp"
#include "point.hpp"
#include "triangle.hpp"


template <typename T>
struct respond_to_triangle_creation_adaptor
{
    respond_to_triangle_creation_adaptor(id<DelaunayTessalationDelegate> delegate)
    : m_delegate(delegate)
    {
    }

    void operator()(T const& sender)
    {
        if ([m_delegate respondsToSelector:@selector(respondToTriangleCreation:)])
        {
            [m_delegate respondToTriangleCreation:nil];
        }
    }

    id<DelaunayTessalationDelegate>    m_delegate;
};


@interface MainViewController(Private)
- (std::vector< point<float> >*)generateVerticesWithHeight:(float)height width:(float)width;
@end


@implementation MainViewController

@synthesize calcButton;
@synthesize delaunayView;
@synthesize showCircumcirclesSwitch;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (std::vector< point<float> >*)generateVerticesWithHeight:(float)height width:(float)width;
{
    using namespace std;

    typedef point<float> vertex_type;

    mt19937 engine(static_cast<unsigned long>(std::time(0)));
    uniform_int_distribution<>  dst_x(10, width - 20);
    uniform_int_distribution<>  dst_y(40, height - 20);

    auto vertices = new std::vector<vertex_type>;

    for (int i = 0; i < 10; ++i)
    {
        vertex_type v(dst_x(engine), dst_y(engine));
        NSLog(@"Vertex: %f, %f", v.x(), v.y());
        vertices->push_back(v);
    }

    return vertices;
}


- (IBAction)respondToCalcButtonTouchUpInside:(id)sender
{
    using namespace std;

    typedef point<float>            vertex_type;
    typedef triangle<vertex_type>   triangle_type;

    delaunay<vertex_type, triangle_type>    delaunay_tessellation;

    [delaunayView setShowCircumcircles:[[self showCircumcirclesSwitch] isOn]];

    auto vertices = [self generateVerticesWithHeight:delaunayView.frame.size.height
                                               width:delaunayView.frame.size.width];
    auto triangles = new std::vector<triangle_type>;

    [delaunayView setVertices:*vertices];
    [delaunayView setTriangles:*triangles];

    respond_to_triangle_creation_adaptor< delaunay<vertex_type, triangle_type> > triangle_created(self);
    delaunay_tessellation.triangle_created.connect(triangle_created);

    delaunay_tessellation(*vertices, *triangles);
}


#pragma mark - DelaunayTessalationDelegate


- (void)respondToTriangleCreation:(id)sender
{
    NSLog(@"A triangle is created.");
}


@end
