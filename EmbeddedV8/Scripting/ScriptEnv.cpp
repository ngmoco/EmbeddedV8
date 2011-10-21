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
//  ScriptEnv.cpp
//  EmbeddedV8

#include "ScriptEnv.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <string.h>
    
v8::Handle<v8::Value> NewCallback(const v8::Arguments& args);    
    
ScriptEnv::ScriptEnv()
: mContextReady(false)
{
    v8::HandleScope handle_scope;
    // Create a new context.
    
    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();
    mGlobalObjDef = v8::Persistent<v8::ObjectTemplate>::New(global);
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
}


uint32_t ScriptEnv::LoadScriptFromFile(const char* filePath)
{
    v8::HandleScope current_scope;
    v8::Context::Scope context_scope(mContext);
    
    // Using stream lets load the file from the file system
    std::ifstream inFile( filePath );
    if( !inFile )
    {
        //cerr << "Couldn´t open input file" << endl;
        //return false;
    }

    // create reader objects
    std::istream_iterator<std::string> dataBegin( inFile );
    std::istream_iterator<std::string> dataEnd;
 
    std::string data;
    

    while( dataBegin != dataEnd )
    {
        data += *dataBegin;
        dataBegin++;
    }
    
    v8::Handle<v8::String> source = v8::String::New( data.c_str() );
    
    v8::Handle<v8::Script> script = v8::Script::Compile(source);
    uint32_t idx = static_cast<uint32_t>(mScripts.size());
    
    mScripts.push_back(script);
    
    return idx;
}

void ScriptEnv::AttachToGlobalDef(v8::Handle<v8::String> name, v8::Handle<v8::FunctionTemplate> tmp)
{ 
    mGlobalObjDef->Set(name, tmp);
}

v8::Handle<v8::Value> NewCallback(const v8::Arguments& args)
{
    v8::Handle<v8::Value> val;
    
    return val;
}

v8::Handle<v8::Value> ScriptEnv::ExecuteScript(const char* data, uint32_t* idx)
{
    v8::HandleScope current_scope;
    // Create a new context.
    
    v8::Context::Scope context_scope(mContext);
    
    v8::Handle<v8::String> source = v8::String::New(data);

    // Compile the source code.
    v8::Handle<v8::Script> script = v8::Script::Compile(source);
    
    if(idx)
    {
        *idx = static_cast<uint32_t>(mScripts.size());
    }
    mScripts.push_back(script);
    v8::Handle<v8::Value> val = script->Run();

    return current_scope.Close(val);
}

v8::Handle<v8::Value> ScriptEnv::ExecuteScript(const uint32_t scriptId)
{
    v8::HandleScope current_scope;
    v8::Context::Scope context_scope(mContext);
        
    ScriptMap::iterator iter = mScripts.begin() + scriptId;
    v8::Handle<v8::Value> val;
    if(iter != mScripts.end())
    {
        val = (*iter)->Run();
    }
    
    return current_scope.Close(val);
}

v8::Handle<v8::Value> ScriptEnv::ExecuteScript()
{
    v8::HandleScope current_scope;
    
    ScriptMap::iterator iter = mScripts.begin();
    ScriptMap::iterator iterEnd = mScripts.end();
    
    v8::Handle<v8::Value> val;
    for(; iter != iterEnd; ++iter)
    {
        val = (*iter)->Run();
    }
    
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
    while(!v8::V8::IdleNotification()) {};
}
        