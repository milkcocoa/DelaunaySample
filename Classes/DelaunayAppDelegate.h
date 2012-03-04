//
//  DelaunayAppDelegate.h
//  Delaunay
//
//  Created by Shu MASUDA on 09/08/09.
//  Copyright Shu MASUDA 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MainViewController;

@interface DelaunayAppDelegate : UIResponder
<
    UIApplicationDelegate
>

@property (nonatomic) IBOutlet MainViewController   *mainViewController;
@property (nonatomic) IBOutlet UIWindow             *window;

@end
