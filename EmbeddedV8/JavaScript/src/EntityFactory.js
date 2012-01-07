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
//  EntityFactory.js
//  EmbeddedV8

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Provide 
goog.provide('embedded.EntityFactory');

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Globals
var bounds_enabled = false;
var bounds_x = 0;
var bounds_y = 0;
var velocity = 10 ; // Pixels per millisecond

function SetGameBounds(x, y)
{
    bounds_x = x;
    bounds_y = y;
}
goog.exportSymbol('SetGameBounds', SetGameBounds);

/**
 * @constructor
 * @this {embedded.Entity.Factory}
 * NOTE that this comment is needed for the closure compiler to 
 * not issue warnings.
 */
embedded.EntityFactory = function()
{
    this.entities = [];
    this.lastUpdate = new Date();
};

embedded.EntityFactory.prototype.Update = function(delta)
{
    var ents = [];
    var len = this.entities.length;
    for(var idx = 0; idx < len; ++idx)
    {
        var pos = this.entities[idx].pos;
        var dist = this.entities[idx].velocity * delta;
        pos = [ pos[0] + dist * this.entities[idx].cosAngle, 
                pos[1] + dist * this.entities[idx].sinAngle];
        if(pos[0] > bounds_x || pos[0] < -bounds_x)
        {
            // remove
            this.entities[idx] = null;
        }
        else if(pos[1] > bounds_y || pos[1] < -bounds_y)
        {
            // remove
            this.entities[idx] = null;
        }
        else 
        {
            this.entities[idx].pos = pos;
            ents.push(this.entities[idx]);
        }
    }
    this.entities = ents;
};

embedded.EntityFactory.prototype.CreateEntity = function()
{
    var ent = new Entity();
    ent.width = Math.floor(Math.random()*33) + 32;
    ent.height = ent.width;
    ent.angle = Math.random() * Math.PI * 2;
    ent.cosAngle = Math.cos(ent.angle);
    ent.sinAngle = Math.sin(ent.angle);
    ent.velocity = (velocity * Math.floor(Math.random() * 10) + 1) / 1000;
    
    this.entities.push(ent);
    return ent;
};

embedded.EntityFactory.prototype.ClearAllEntities = function()
{
    this.entities = [];
};
