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
//
//  ScriptClassWrapper.h
//  EmbeddedV8

#ifndef EmbeddedV8_ScriptClassWrapper_h
#define EmbeddedV8_ScriptClassWrapper_h

#include <v8.h>

template< typename T >
class ScriptedClass
{
public:

    static void Register(ScriptEnv env, const char* N)
    {
        // Pull the context out
        env.AttachToGlobalDef(v8::String::New(N), v8::FunctionTemplate::New(ScriptedClass<T>::OnCreate));
        v8::Persistent<v8::ObjectTemplate> class_template;
        v8::Handle<v8::ObjectTemplate> rt = v8::ObjectTemplate::New();

        rt->SetInternalFieldCount(1);

        sClassTemplate = v8::Persistent<v8::ObjectTemplate>::New(rt);
        
        T::BindScriptMethods(sClassTemplate);
    }

    static v8::Persistent<v8::ObjectTemplate> sClassTemplate;
private:
   
    static v8::Handle<v8::Value> OnCreate(const v8::Arguments& args)
    {
        v8::HandleScope scope;
        if(!args.IsConstructCall())
        {
            return v8::Undefined();
        }


        
        v8::Handle<v8::Object> result = sClassTemplate->NewInstance();
        //v8::Handle<v8::External> class_ptr;
        T* instance = new T(args);
        v8::Persistent<v8::External> class_ptr = v8::Persistent<v8::External>::New(v8::External::New(instance));
        class_ptr.MakeWeak(NULL, ScriptedClass<T>::OnReleased);
        
        result->SetInternalField(0, class_ptr);

        return scope.Close(result);
    }
    
    static void OnReleased(v8::Persistent<v8::Value> object, void* params)
    {
        v8::Persistent<v8::External> ref = v8::Persistent<v8::External>::Cast(object);
        T* instance = static_cast<T*>(ref->Value());
        delete instance;
    }
    
    
};

template<typename T>
v8::Persistent<v8::ObjectTemplate> ScriptedClass<T>::sClassTemplate;


#endif
