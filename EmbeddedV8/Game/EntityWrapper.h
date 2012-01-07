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
//  EntityWrapper.h
//  EmbeddedV8
#ifndef EmbeddedV8_EntityWrapper_h
#define EmbeddedV8_EntityWrapper_h

#include "Entity.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Namespace
namespace game
{

class EntityWrapper : public Entity
{
public:

    EntityWrapper(const v8::Arguments& args)
    : Entity()
    {};
    
    ///////////////////////////////////////////////////
    // Static binding methods
    static void BindScriptMethods(v8::Persistent<v8::ObjectTemplate> obj)
    {
        // Bind the Log statement
        obj->Set("LogData", v8::FunctionTemplate::New(EntityWrapper::Script_LogData));
        
        // Bind the position
        obj->SetAccessor(v8::String::New("pos"), GetPos, SetPos);
        
        // Bind the Direction
        obj->SetAccessor(v8::String::New("width"), GetWidth, SetWidth);
        
        // Bind the Velocity
        obj->SetAccessor(v8::String::New("height"), GetHeight, SetHeight);
        
    }
    
private:
    
    static v8::Handle<v8::Value> Script_LogData(const v8::Arguments& args)
    {
        v8::Handle<v8::External> fields = v8::Handle<v8::External>::Cast(args.Holder()->GetInternalField(0));
        static_cast<Entity*>(fields->Value())->LogData();
        return v8::Undefined();
    }
    
    static v8::Handle<v8::Value> GetPos(v8::Local<v8::String> property, const v8::AccessorInfo &info) 
    {
        v8::Local<v8::Object> self = info.Holder();
        v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        const float* value = static_cast<Entity*>(ptr)->GetPosition();
        v8::Handle<v8::Array> array = v8::Array::New();
        array->Set(0, v8::Number::New(value[0]));
        array->Set(1, v8::Number::New(value[1]));
        
        return array;
    }
  
    static void SetPos(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
    {
        v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(value);
        static_cast<Entity*>(v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0))->Value())->SetPosition(array->Get(0)->NumberValue(), array->Get(1)->NumberValue());
    }
    
    static v8::Handle<v8::Value> GetWidth(v8::Local<v8::String> property, const v8::AccessorInfo &info) 
    {
        return v8::Number::New(static_cast<Entity*>(v8::Local<v8::External>::Cast(
                        info.Holder()->GetInternalField(0))->Value())->GetWidth());

    }
  
    static void SetWidth(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
    {
        v8::Local<v8::Number> num = v8::Local<v8::Number>::Cast(value);
        static_cast<Entity*>(v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0))->Value())->SetWidth(num->NumberValue());
    }
    
    static v8::Handle<v8::Value> GetHeight(v8::Local<v8::String> property, const v8::AccessorInfo &info) 
    {
        return v8::Number::New(static_cast<Entity*>(v8::Local<v8::External>::Cast(
                        info.Holder()->GetInternalField(0))->Value())->GetHeight());
    }
    
    static void SetHeight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
    {
        v8::Local<v8::Number> num = v8::Local<v8::Number>::Cast(value);
        static_cast<Entity*>(v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0))->Value())->SetHeight(num->NumberValue());
    }
};


} // end of namespace game


#endif // end of include guard EmbeddedV8_EntityWrapper_h
