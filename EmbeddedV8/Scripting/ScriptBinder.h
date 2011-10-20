/////////////////////////////////////////////////////////////////////////////
/**
 *  v8ClassHelper.h
 *  Helper for wrapping Class objects to allow V8 to make native callbacks
 *
 *  Created by Chris Jimison
 *
 */

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
