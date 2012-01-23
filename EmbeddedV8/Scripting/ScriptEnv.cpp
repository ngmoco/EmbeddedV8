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

/////////////////////////////////////////////////////////////////////////////
//  ScriptEnv.cpp
//  EmbeddedV8

#include "ScriptEnv.h"
#define JS_DEBUGGING

#ifdef JS_DEBUGGING
#include <v8-debug.h>
#endif

#include <iostream>
#include <fstream>
#include <iterator>
#include <stdlib.h>

/////////////////////////////////////////////////////////////////////////////
// Namespace

/////////////////////////////////////////////////////////////////////////////
// Utility class
// Setup for loging debug information to the system    
const char* ToCString(const v8::String::Utf8Value& value);
v8::Handle<v8::Value> LogD(const v8::Arguments& args);
const char* ToCString(const v8::String::Utf8Value& value)
{
    return *value ? *value : "<string conversion failed>";
}    
v8::Handle<v8::Value> LogD(const v8::Arguments& args)
{
    v8::String::Utf8Value str(args[0]);
    const char* cstr = ToCString(str);
    printf("<JS Debug>: %s\n", cstr);
    
    return v8::Undefined();
}


#ifdef JS_DEBUGGING
v8::Persistent<v8::Context> debug_message_context;
void DispatchDebugMessages();

void DispatchDebugMessages()
{
    v8::Context::Scope scope(debug_message_context);
    v8::Debug::ProcessDebugMessages();
}

const int32_t port_number = 9222;
const bool wait_for_connection = false;
const bool support_callback = true;
#endif


/////////////////////////////////////////////////////////////////////////////
// class ScriptEnv

ScriptEnv::ScriptEnv()
: mContextReady(false)
{
    // Setup the global scope and attach the log statement 
    v8::HandleScope handle_scope;
    
    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
    mGlobalObjDef = v8::Persistent<v8::ObjectTemplate>::New(global);
    
    AttachToGlobalDef("logD", LogD);
}

ScriptEnv::~ScriptEnv()
{
    // Dispose the persistent context.
    mContext.Dispose();
}

void ScriptEnv::StartContext()
{
    v8::HandleScope handle_scope;
    mContext = v8::Context::New(NULL, mGlobalObjDef);
    
    v8::Context::Scope context_scope(mContext);
    
#ifdef JS_DEBUGGING
    debug_message_context = v8::Persistent<v8::Context>::New(mContext);
    
    if (support_callback)
    {
        v8::Debug::SetDebugMessageDispatchHandler(DispatchDebugMessages, true);
    }
    
    if (port_number != -1)
    {
        v8::Debug::EnableAgent("NrgCore", port_number, wait_for_connection);
    }
#endif
}

v8::Handle<v8::Value> ScriptEnv::ExecuteScriptFile(const char* filePath)
{
    FILE *fp;
    long len;
    char *buf;
    fp=fopen(filePath,"rb");
    fseek(fp,0,SEEK_END); //go to end
    len=ftell(fp); //get position at end (length)
    fseek(fp,0,SEEK_SET); //go to beg.
    buf=(char *)malloc(len); //malloc buffer
    fread(buf,len,1,fp); //read into buffer
    fclose(fp);
    
    return ExecuteScript(buf, filePath, true);
}

void ScriptEnv::AttachToGlobalDef(const char* name, v8::Handle<v8::Value> (*func)(const v8::Arguments& args))
{
    v8::HandleScope current_scope;
    mGlobalObjDef->Set(v8::String::New(name), v8::FunctionTemplate::New(func));
}

v8::Handle<v8::Value> ScriptEnv::ExecuteScript(const char* data, const char* name, const bool releaseData)
{
    v8::HandleScope current_scope;
    v8::Context::Scope context_scope(mContext);
    // Create a new context.
    
    v8::Handle<v8::String> source = v8::String::New(data);
    
    // Compile the source code.
    v8::Handle<v8::Script> script = v8::Script::Compile(source, v8::String::New(name));
    
    // free up the resources
    if(releaseData)
    {
        free((void*)data);
    }
    
    // Run it
    v8::Handle<v8::Value> val = script->Run();
    return current_scope.Close(val);
}

v8::Handle<v8::Function> ScriptEnv::FindScriptFunc(const char* function)
{
    v8::HandleScope current_scope;
    v8::Context::Scope context_scope(mContext);
    
    v8::Handle<v8::String> func_name = v8::String::New(function); 
    v8::Handle<v8::Value> func = mContext->Global()->Get(func_name); 
    
    // It is a function; cast it to a Function 
    v8::Handle<v8::Function> func_fun = v8::Handle<v8::Function>::Cast(func);
    
    return  current_scope.Close(func_fun);
}

v8::Handle<v8::Value> ScriptEnv::CallScriptFunc(v8::Handle<v8::Function> handle, const int argc, v8::Handle<v8::Value>* argv)
{
    v8::HandleScope current_scope;
    v8::Context::Scope context_scope(mContext);
    
    v8::Handle<v8::Value> val = handle->Call(mContext->Global(), argc, argv);
    
    return current_scope.Close(val);
}

void ScriptEnv::ForceGC()
{
    v8::HandleScope current_scope;
    v8::Context::Scope context_scope(mContext);
    
    while(!v8::V8::IdleNotification()) {};
}
