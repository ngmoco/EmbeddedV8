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
//  ScriptUtils.h
//  EmbeddedV8

#ifndef EmbeddedV8_ScriptUtils_h
#define EmbeddedV8_ScriptUtils_h

#include <v8.h>

// Assume that the value is an external and needs to unwrap
// the object
template<typename T>
T CastTo(v8::Handle<v8::Value> val)
{
    v8::Local<v8::Object> obj = v8::Object::Cast(*val);
    v8::Handle<v8::External> fields = v8::Handle<v8::External>::Cast(obj->GetInternalField(0));
    return static_cast<T>(fields->Value());
}


template<>
int32_t CastTo<int32_t>(v8::Handle<v8::Value> val)
{
    return val->Int32Value();
}

template<>
uint32_t CastTo<uint32_t>(v8::Handle<v8::Value> val)
{
    return val->Uint32Value();
}

template<>
char* CastTo<char*>(v8::Handle<v8::Value> val)
{
    // This look WAY more unsafe then it really is.
    // When you convert to ascii you are making a
    // copy of the string on the v8 heap.
    // and the V8 GC will not be called before
    // this method returns so you should be ok.  Assuming
    // the if the user wants to keep this string around
    // they make a copy of it.
    v8::String::AsciiValue ascii(val);
    return *ascii;
}

template<>
float CastTo<float>(v8::Handle<v8::Value> val)
{
    return val->NumberValue();
}

template<> 
double CastTo<double>(v8::Handle<v8::Value> val)
{
    return val->NumberValue();
}

template<> 
bool CastTo<bool>(v8::Handle<v8::Value> val)
{
    return val->BooleanValue();
}

template<> 
int64_t CastTo<int64_t>(v8::Handle<v8::Value> val)
{
    return val->IntegerValue();
}


template<typename T>
v8::Handle<v8::Value> Wrap(const T* val)
{
    return v8::External::New(val);
}

template<>
v8::Handle<v8::Value> Wrap(const char* val)
{
    return v8::String::New(val);
}


template<typename T>
v8::Handle<v8::Value> Wrap(T val);

template<>
v8::Handle<v8::Value> Wrap(int32_t val)
{
    return v8::Number::New(val);
}

template<>
v8::Handle<v8::Value> Wrap(uint32_t val)
{
    return v8::Number::New(val);
}

template<>
v8::Handle<v8::Value> Wrap(float val)
{
    return v8::Number::New(val);
}

template<>
v8::Handle<v8::Value> Wrap(double val)
{
    return v8::Number::New(val);
}



#endif
