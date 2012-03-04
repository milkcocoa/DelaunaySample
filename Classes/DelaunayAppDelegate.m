//
//  DelaunayAppDelegate.m
//  Delaunay
//
//  Created by Shu MASUDA on 09/08/09.
//  Copyright Shu MASUDA 2009. All rights reserved.
//

#import "DelaunayAppDelegate.h"

#import "MainViewController.h"

@implementation DelaunayAppDelegate

@synthesize mainViewController;
@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application
{
    mainViewController = [[MainViewController alloc] initWithNibName:@"MainViewController" bundle:nil];
    [window setRootViewController:mainViewController];

    // Override point for customization after application launch
    [window makeKeyAndVisible];
}


@end
