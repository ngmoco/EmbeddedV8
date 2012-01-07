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
//  ScriptBinder.h
//  EmbeddedV8

#ifndef SCRIPTING_SCRIPT_BINDER_H
#define SCRIPTING_SCRIPT_BINDER_H

/////////////////////////////////////////////////////////////////////////////
// Includes
#include <v8.h>
#include "ScriptEnv.h"

/////////////////////////////////////////////////////////////////////////////
// Macros

#define BindToClassScriptObject(name, env) { char _named_ []= #name; ScriptedClass< name, _named_ >.Register(env); }

/////////////////////////////////////////////////////////////////////////////
// Functions
void BindFunctionToScriptObject(v8::Persistent<v8::Context> context, v8::Handle<v8::Object> obj, v8::Handle<v8::Value> name, v8::InvocationCallback id);

v8::Handle<v8::Value> OnCreate(const v8::Arguments& args);


template <typename T>
v8::Handle<v8::Object> BindClassToNewObject(T* instance)
{
    v8::HandleScope scope;

    v8::Persistent<v8::ObjectTemplate> class_template;
    v8::Handle<v8::ObjectTemplate> rt = v8::ObjectTemplate::New();

    rt->SetInternalFieldCount(1);

    class_template = v8::Persistent<v8::ObjectTemplate>::New(rt);

    v8::Handle<v8::Object> result = class_template->NewInstance();
    v8::Handle<v8::External> class_ptr;

    class_ptr = v8::External::New(static_cast<T*>(instance));

    result->SetInternalField(0, class_ptr);
    return scope.Close(result);
}

template<typename T>
v8::Handle<v8::Object> BindToScriptObject(  v8::Persistent<v8::Context> context,
                                            T* instance,
                                            v8::Handle<v8::Value> exposedName,
                                            v8::PropertyAttribute props)
{
    v8::HandleScope scope;

    v8::Context::Scope context_scope(context);

    v8::Handle<v8::Object> obj = BindClassToNewObject<T>(instance);
    context->Global()->Set(exposedName, obj, props);

    return scope.Close(obj);
}

void BindFunctionToScriptObject(v8::Persistent<v8::Context> context, v8::Handle<v8::Object> obj, v8::Handle<v8::Value> name, v8::InvocationCallback id)
{
    v8::HandleScope scope;
    v8::Context::Scope context_scope(context);
    
    v8::Handle<v8::FunctionTemplate> func;
    func = v8::FunctionTemplate::New(id);
    obj->Set(name, func->GetFunction());
}

template<typename T>
T* GetBoundClassInstance(v8::Handle<v8::Object> obj)
{
    v8::Handle<v8::External> fields = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<T*>(fields->Value());
}

#endif // end of include guard SCRIPTING_SCRIPT_BINDER_H
