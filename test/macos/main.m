/*
 * test.c (MacOS)
 * 
 * This file is part of the "PicoRenderer" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#import <Cocoa/Cocoa.h>

#include <pico.h>
#include <stdio.h>


// --- global members --- //

PRobject context        = NULL;
PRobject frameBuffer    = NULL;
PRboolean isQuit        = PR_FALSE;

PRuint scrWidth         = 640;
PRuint scrHeight        = 480;


// --- interfaces --- //

@interface AppDelegate : NSObject
{
    BOOL _quit;
}
- (id) initApp;
- (BOOL) isQuit;
@end

@implementation AppDelegate

- (id) initApp
{
    self = [super init];
    _quit = FALSE;
    return (self);
}

- (void) windowWillClose:(id)sender
{
    _quit = TRUE;
}

- (BOOL) isQuit
{
    return (_quit);
}


@end


// --- functions --- //

static void postKeyEvent(void* event)
{
    NSString* s = [(NSEvent*)event characters];
    
    if (s != nil && [s length] > 0)
    {
        unsigned int c = [s characterAtIndex:0];
        
        switch (c)
        {
            case 27: // ESC
                isQuit = PR_TRUE;
                break;
        }
    }
    
    [s release];
}

static void processEvents(NSWindow* wnd)
{
    for (;;)
    {
        NSEvent* event = [wnd
            nextEventMatchingMask:  NSEventMaskAny
            untilDate:              nil
            inMode:                 NSDefaultRunLoopMode
            dequeue:                YES
        ];

        if (event == nil)
            break;

        switch ([event type])
        {
            case NSEventTypeKeyDown:
                postKeyEvent(event);
                break;

            default:
                [NSApp sendEvent:event];
                break;
        }
    }
}

int main(int argc, char* argv[])
{
    // Create window with cocoa framework
    [[NSAutoreleasePool alloc] init];
    [NSApplication sharedApplication];
    [NSApp setDelegate:(id<NSApplicationDelegate>)[[[AppDelegate alloc] initApp] autorelease]];
    //[NSBundle loadNibNamed:@"MainMenu" owner:[NSApp delegate]];
    [NSApp finishLaunching];
    
    NSWindow* window = NULL;
    
    window = [[NSWindow alloc]
        initWithContentRect:    NSMakeRect(0, 0, (CGFloat)scrWidth, (CGFloat)scrHeight)
        styleMask:              (NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable)
        backing:                NSBackingStoreBuffered
        defer:                  NO
    ];

    [window center];
    [window setDelegate:(id<NSWindowDelegate>)[NSApp delegate]];
    [window setAcceptsMouseMovedEvents:YES];
    [window setIsVisible:YES];
    [window makeKeyAndOrderFront:nil];
    [window setTitle:@"pico_renderer test (MacOS)"];

    // Initialize renderer
    prInit();
    
    // Create graphics context
    PRcontextdesc contextDesc;
    contextDesc.window = (void*)window;
    PRobject context = prCreateContext(&contextDesc, scrWidth, scrHeight);
    //prMakeCurrent();
    
    // Create frame buffer
    frameBuffer = prCreateFrameBuffer(scrWidth, scrHeight);
    prBindFrameBuffer(frameBuffer);
    
    prClearColor(0, 0, 0);
    
    // Setup projection matrix
    PRfloat projection[16];
    prBuildPerspectiveProjection(
        projection,                   // output matrix
        (PRfloat)scrWidth/scrHeight,  // aspect ratio
        1.0f,                         // near clipping plane
        100.0f,                       // far clipping plane
        74.0f * PR_DEG2RAD            // field of view (fov) in radians
    );
    prProjectionMatrix(projection);
    
    // World transform
    PRfloat worldMatrix[16];
    PRfloat rotation = 0.0f;
    
    PRobject tex0 = prCreateTexture();
    prTexImage2DFromFile(tex0, "crate.png", PR_TRUE, PR_TRUE);
    prTexEnvi(PR_TEXTURE_LOD_BIAS, 2);
    
    // Main loop
    while (!isQuit)
    {
        // Process events
        processEvents(window);

        if ([(AppDelegate*)[NSApp delegate] isQuit])
            isQuit = PR_TRUE;

        // Setup viewport
        prViewport(0, 0, scrWidth, scrHeight);
        
        // Setup world matrix
        prLoadIdentity(worldMatrix);
        prTranslate(worldMatrix, 0, 0, 4);
        prRotate(worldMatrix, 1, 0, 0, M_PI*0.28);
        prRotate(worldMatrix, 0, 0, 1, rotation);
        prScale(worldMatrix, 2, 2, 2);
        prWorldMatrix(worldMatrix);
        rotation += 0.01f;//0.025f;
        
        // Draw scene
        prClearFrameBuffer(
            frameBuffer,
            0.0f,
            PR_COLOR_BUFFER_BIT | PR_DEPTH_BUFFER_BIT
        );
        
        prColor(255, 255, 0);
        
        prBindTexture(tex0);
        prBegin(PR_TRIANGLES);
        {
            prTexCoord2i(0, 0);
            prVertex2i(-1, 1);
            
            prTexCoord2f(1, 0);
            prVertex2i(1, 1);
            
            prTexCoord2i(1, 1);
            prVertex2i(1, -1);
            
            prTexCoord2i(0, 0);
            prVertex2i(-1, 1);
            
            prTexCoord2i(1, 1);
            prVertex2i(1, -1);
            
            prTexCoord2f(0, 1);
            prVertex2i(-1, -1);
        }
        prEnd();
        
        prPresent(context);
    }
    
    // Clean up
    prDeleteFrameBuffer(frameBuffer);
    prDeleteContext(context);
    
    prRelease();
    
    return 0;
}

