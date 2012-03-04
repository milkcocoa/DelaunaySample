//
//  main.m
//  Delaunay
//
//  Created by Shu MASUDA on 09/08/09.
//  Copyright Shu MASUDA 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "DelaunayAppDelegate.h"


int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        @try
        {
            return UIApplicationMain(argc, argv, nil, NSStringFromClass([DelaunayAppDelegate class]));
        }
        @catch (NSException *exception)
        {
            NSLog(@"%@", [exception callStackSymbols]);

            @throw exception;
        }
    }
}
