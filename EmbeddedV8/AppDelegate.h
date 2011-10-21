////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2011 Chris Jimison
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without 
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
// the Software, and to permit persons to whom the Software is furnished to do so, subject to the following 
// conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AppDelegate.h
//  EmbeddedV8
//

#import <Cocoa/Cocoa.h>
#include <v8.h>
#include "Scripting/ScriptEnv.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    ScriptEnv* scriptEnv;
    v8::Persistent<v8::Function> onGameStart;
    v8::Persistent<v8::Function> onGameStop;
    v8::Persistent<v8::Function> onGameUpdate;
    v8::Persistent<v8::Function> onAddEntity;
    v8::Persistent<v8::Function> onRemoveEntites;
    v8::Persistent<v8::Function> onBoundsSet;
    
    BOOL createNew;
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSToolbarItem* boarder;
@property (assign) IBOutlet NSToolbarItem* physics;

-(void) renderGame:(NSRect)drawRect;

@end
