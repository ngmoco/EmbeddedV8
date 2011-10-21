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
//  AppDelegate.m
//  EmbeddedV8

#import "AppDelegate.h"
#include <v8.h>
#include "Scripting/ScriptEnv.h"
#include "Scripting/ScriptBinder.h"
#include "Scripting/ScriptClassWrapper.h"
#include "Game/EntityWrapper.h"
#include "Scripting/ScriptUtils.h"

using namespace game;

@implementation AppDelegate

@synthesize window = _window, boarder, physics;

- (void) alertDidEnd:(NSAlert *)alert returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo
{

}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Setup the environment
    Open_Script_Scope;
    scriptEnv = new ScriptEnv();

    // Reigster any classes to the global
    ScriptedClass< EntityWrapper >::Register(*scriptEnv, "Entity");
    scriptEnv->StartContext();

    NSDictionary* plistDict = [[NSDictionary alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"ScriptManifest" ofType:@"plist"]];
    NSArray* code = [plistDict objectForKey:@"code"];

    for(NSString* name in code)
    {
        // Load up some scripts
        NSString* file = [[NSBundle mainBundle] pathForResource:name ofType:@"js"];
        NSString* data = [NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil];
        scriptEnv->ExecuteScript( [data cStringUsingEncoding:NSUTF8StringEncoding]  );
    }
    
    onGameStart =       v8::Persistent<v8::Function>::New(scriptEnv->FindScriptFunc("OnGameStart"));
    onGameStop =        v8::Persistent<v8::Function>::New(scriptEnv->FindScriptFunc("OnGameStop"));
    onGameUpdate =      v8::Persistent<v8::Function>::New(scriptEnv->FindScriptFunc("OnGameUpdate"));
    onAddEntity =       v8::Persistent<v8::Function>::New(scriptEnv->FindScriptFunc("OnAddEntity"));
    onBoundsSet =       v8::Persistent<v8::Function>::New(scriptEnv->FindScriptFunc("SetGameBounds"));
    onRemoveEntites =   v8::Persistent<v8::Function>::New(scriptEnv->FindScriptFunc("OnRemoveEntites"));
    
    int32_t error = CastTo<int32_t>(scriptEnv->CallScriptFunc(onGameStart));
    
    if(error)
    {
        NSAlert *alert = [[NSAlert alloc] init];
        [alert setMessageText:@"Could not load the JS GAME code"];
        [alert setInformativeText:@"Error on starting the game Javascript code"];
        [alert addButtonWithTitle:@"Exit"];
        [alert beginSheetModalForWindow:_window modalDelegate:self didEndSelector:@selector(alertDidEnd:returnCode:contextInfo:) contextInfo:nil];
        alert = nil;
    }
}

-(IBAction) onAddEntity:(id) sender
{
    Open_Script_Scope;
    scriptEnv->CallScriptFunc(onAddEntity);
}

-(IBAction) onRemoveAll:(id) sender
{
    Open_Script_Scope;
    scriptEnv->CallScriptFunc(onRemoveEntites);
}

-(IBAction) onSetBoard:(id) sender
{

}

-(IBAction) onSetPhysics:(id) sender
{

}

-(IBAction) onForceGC:(id) sender
{
    if(scriptEnv)
    {
        scriptEnv->ForceGC();
    }
}

-(void) renderGame:(NSRect)drawRect
{
    if(scriptEnv)
    { 
        Open_Script_Scope;
        
        v8::Handle<v8::Value> argv[2] = { Wrap<int32_t>(drawRect.size.width), Wrap<int32_t>(drawRect.size.width) }; 
        scriptEnv->CallScriptFunc(onBoundsSet, 2, argv);
        
        // Get list of all game entities active
        v8::Handle<v8::Value> result = scriptEnv->CallScriptFunc(onGameUpdate);
        v8::Local<v8::Array> array = v8::Array::Cast(*result);
        
        uint32_t len = array->Length();
        for(uint32_t idx = 0; idx < len; ++idx)
        {
            Entity* entity = CastTo<Entity*>(array->Get(idx));
            
            // Now lets draw the entity at the given location
            [[NSGraphicsContext currentContext] setImageInterpolation: NSImageInterpolationHigh];

            NSSize viewSize  = drawRect.size;
            NSSize imageSize = { entity->GetWidth(), entity->GetHeight() };

            NSPoint viewCenter;
            viewCenter.x = viewSize.width  * 0.50 + entity->GetPosition()[0];
            viewCenter.y = viewSize.height * 0.50 + entity->GetPosition()[1];

            NSPoint imageOrigin = viewCenter;
            imageOrigin.x -= imageSize.width  * 0.50;
            imageOrigin.y -= imageSize.height * 0.50;

            NSRect destRect;
            destRect.origin = imageOrigin;
            destRect.size = imageSize;
            
            NSImage * image = [[NSImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForImageResource:@"javascript"]];
            
            [image drawInRect: destRect
                fromRect: NSZeroRect
                operation: NSCompositeSourceOver
                fraction: 1.0];
        }
    }
}

@end
