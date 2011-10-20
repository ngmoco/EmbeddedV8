

function OnGameStart()
{

}

function OnGameStop()
{

}

var last = null;
var Factory = new EntityFactory();

function OnGameUpdate()
{
    // Calculating Delta time
    var now = new Date;
    var delta = 0;
    
    if(last)
    {
        delta = now - last;
    }
    
    Factory.Update(delta);
    
    last = now;
}