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
//  main.mm
//  EmbeddedV8
//

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include
#import <Cocoa/Cocoa.h>
#include <v8.h>
#include "Scripting/ScriptEnv.h"
#include "Scripting/ScriptBinder.h"
#include "Scripting/ScriptClassWrapper.h"
#include "Game/EntityWrapper.h"

using namespace game;

int main(int argc, char *argv[])
{
    // Setup the environment
    Open_Script_Scope;
    ScriptEnv env;

    // Reigster any globals
    ScriptedClass< EntityWrapper >::Register(env, "Entity");

    env.StartContext();
    
    NSString* file = [[NSBundle mainBundle] pathForResource:@"EntityFactory" ofType:@"js"];
    NSString* data = [NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil];
    env.ExecuteScript( [data cStringUsingEncoding:NSUTF8StringEncoding]  );
    
    file = [[NSBundle mainBundle] pathForResource:@"Main" ofType:@"js"];
    data = [NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil];
    env.ExecuteScript( [data cStringUsingEncoding:NSUTF8StringEncoding]  );
    
    
    //v8::Handle<v8::Value> result = env.ExecuteScript("var x = new Entity(); x.pos = [10, 20]; x.LogData();");
    // Convert the result to an ASCII string and print it.
    //printf("%f\n", (*result)->NumberValue());
    //env.ForceGC();

    return NSApplicationMain(argc, (const char **)argv);
}
