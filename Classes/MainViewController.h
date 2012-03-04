//
//  MainViewController.h
//  Delaunay
//
//  Created by Shu MASUDA on 09/08/10.
//  Copyright 2009 - 2010 Shu MASUDA. All rights reserved.
//

#import <UIKit/UIKit.h>


@protocol DelaunayTessalationDelegate<NSObject>
- (void)respondToTriangleCreation:(id)sender;
@end


@class DelaunayView;

@interface MainViewController : UIViewController<DelaunayTessalationDelegate>
{
    IBOutlet UIButton       *calcButton;
    IBOutlet DelaunayView   *delaunayView;
    IBOutlet UISwitch       *showCircumcirclesSwitch;
}

@property (nonatomic, retain) IBOutlet UIButton     *calcButton;
@property (nonatomic, retain) IBOutlet DelaunayView *delaunayView;
@property (nonatomic, retain) IBOutlet UISwitch     *showCircumcirclesSwitch;

- (IBAction)respondToCalcButtonTouchUpInside:(id)sender;

@end
