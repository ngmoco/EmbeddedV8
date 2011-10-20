//
//  ScriptEnv.h
//  V8Game
//
//  Created by Chris Jimison on 10/18/11.
//  Copyright (c) 2011 ngmoco:). All rights reserved.
//

#ifndef V8Game_ScriptEnv_h
#define V8Game_ScriptEnv_h

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
    
    void ForceGC();
    
private:
    
    typedef std::vector< v8::Handle<v8::Script> > ScriptMap;
    ScriptMap mScripts;
    
    v8::Persistent<v8::ObjectTemplate> mGlobalObjDef;
    v8::Persistent<v8::Context> mContext;
    bool mContextReady;
    
}; // end of class ScriptEnv    


#endif
