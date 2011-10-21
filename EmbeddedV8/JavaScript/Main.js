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
//  Main.js
//  EmbeddedV8

var last = null;
var Factory = new EntityFactory();

function OnAddEntity()
{
    var count = Math.floor(Math.random()*100) + 1;
    for(var idx = 0; idx < count; ++idx)
    {
        Factory.CreateEntity();
    }
}

function OnRemoveEntites()
{
    Factory.ClearAllEntities();
}

function OnGameStart()
{
    return 0;
}

function OnGameStop()
{
    
}

function OnGameUpdate()
{
    // Calculating Delta time
    var now = new Date();
    var delta = 0;
    
    if(last)
    {
        delta = now.getTime() - last.getTime();
    }
    
    Factory.Update(delta);
    
    last = now;
    return Factory.entities;
}

