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
//  ScriptEnv.h
//  EmbeddedV8

#ifndef EmbeddedV8_ScriptEnv_h
#define EmbeddedV8_ScriptEnv_h

#include "v8.h"
#include <stdint.h>
#include <vector>

#define Open_Script_Scope v8::HandleScope _cur_global

class ScriptEnv
{
    
public:
    
    ScriptEnv();
    ~ScriptEnv();
    
    void StartContext();
    void AttachToGlobalDef(v8::Handle<v8::String> name, v8::Handle<v8::FunctionTemplate> tmp);
    uint32_t LoadScriptFromFile(const char* fileName);
    
    v8::Handle<v8::Value> ExecuteScript(const char* data, uint32_t* idx = NULL);
    v8::Handle<v8::Value> ExecuteScript(const uint32_t scriptId);
    v8::Handle<v8::Value> ExecuteScript();
    v8::Handle<v8::Function> FindScriptFunc(const char* function);
    v8::Handle<v8::Value> CallScriptFunc(v8::Handle<v8::Function> handle, const int argc = 0, v8::Handle<v8::Value>* argv = NULL);
    
    void ForceGC();
    
private:
    
    typedef std::vector< v8::Handle<v8::Script> > ScriptMap;
    ScriptMap mScripts;
    
    v8::Persistent<v8::ObjectTemplate> mGlobalObjDef;
    v8::Persistent<v8::Context> mContext;
    bool mContextReady;
    
}; // end of class ScriptEnv    


#endif
