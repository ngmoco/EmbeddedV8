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
//  Entity.h
//  EmbeddedV8

#ifndef EmbeddedV8_Entity_h
#define EmbeddedV8_Entity_h

#include <v8.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Namespace
namespace game
{

////////////////////////////////////////////////////////
// Game Entity object that can be rendered in the view
// just be simulated
class Entity
{
public:

    Entity();
    
    virtual ~Entity();
    
    void LogData()
    { 
        printf("Position = ( %f, %f )\nwidth = %f\nheight = %f\n", 
                mPosition[0], mPosition[1], mWidth, mHeight);
    }
    
    const float* GetPosition() const { return mPosition; }
    void SetPosition(const float* pos){ SetPosition(pos[0], pos[1]); }
    void SetPosition(const float x = 0.0f, const float y = 0.0f){ mPosition[0] = x; mPosition[1] = y;}
    
    float GetWidth() const { return mWidth; }
    void SetWidth(const float width = 0.0f) { mWidth = width; }
    
    float GetHeight() const { return mHeight; }
    void SetHeight(const float height = 0.0f) { mHeight = height; }

private:
   
    ///////////////////////////////////////////////////
    // Data Members
    float mPosition[2];
    float mWidth;
    float mHeight;

    ///////////////////////////////////////////////////
    // Disallowed methods
    Entity(const Entity&);
    Entity& operator=(const Entity&);
};


} // end of namespace Game


#endif // end of include guard EmbeddedV8_Entity_h
