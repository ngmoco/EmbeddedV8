var EntityFactory = function()
{
    this.entities = [];
    this.lastUpdate = new Date();
}

EntityFactory.prototype.Update = function(delta)
{
    var len = this.entities.length;
    for(var idx = 0; idx < len; ++idx)
    {
        if(this.entities[idx].Update)
        {
            this.entities[idx].Update(delta);
        }
    }
}