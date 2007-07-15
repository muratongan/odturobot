function Vector(xx,yy)
{
	this.x=xx;
	this.y=yy;
};

var Vectormath=
{
	vmul:function(v1,v2)
	{
		return new Vector(v1.x*v2.x-v1.y*v2.y,v1.x*v2.y+v2.x*v1.y);
	},
	add:function(v1,v2)
	{
		return new Vector(v1.x+v2.x,v1.y+v2.y);
	},
	sub:function(v1,v2)
	{
		return new Vector(v1.x-v2.x,v1.y-v2.y);
	},
	neg:function(v)
	{
		return new Vector(-v.x,-v.y);
	},
	cmul:function(c,v)
	{
		return new Vector(c*v.x,c*v.y);
	}
};

var Sonarposition=
[
	new Vector(-14,6),
	new Vector(-12,10),
	new Vector(-6.5,13.5),
	new Vector(-3,15),
	new Vector(3,15),
	new Vector(6.5,13.5),
	new Vector(12,10),
	new Vector(14,6),
	new Vector(14,-11.5),
	new Vector(12,-7.5),
	new Vector(6.5,-4),
	new Vector(3,-2.5),
	new Vector(-3,-2.5),
	new Vector(-6.5,-4),
	new Vector(-12,-7.5),
	new Vector(-14,-11.5)
];

var Sonarnormal=
[
	new Vector(-1,0),
	new Vector(-0.7682,0.6402),
	new Vector(-0.4338,0.9010),
	new Vector(-0.1961,0.9805),
	new Vector(0.1961,0.9805),
	new Vector(0.4338,0.9010),
	new Vector(0.7682,0.6402),
	new Vector(1,0),
	new Vector(1,0),
	new Vector(0.7682,0.6402),
	new Vector(0.4338,0.9010),
	new Vector(0.1961,0.9805),
	new Vector(-0.1961,0.9805),
	new Vector(-0.4338,0.9010),
	new Vector(-0.7682,0.6402),
	new Vector(-1,0)
];

function B1()
{
	var behaviour=this;
	var interval_id;
	var toarbitrator;
	var rot=0;
	var trans=0;
	var weight=1;
	this.sonar=[5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000];
	var stopcallback=function(ref)
	{
		clearInterval(interval_id);
	};
	var datacallback=function(datastring,ref)
	{
		var v0;
		var v1;
		var minindex=7;
		var frontindex;
		var cindex;
		try
		{
			eval(datastring);
		}
		catch(err)
		{
			alert(err);
		}
		for(cindex=6;cindex>3;--cindex)
		{
			if(behaviour.sonar[minindex]>behaviour.sonar[cindex])
				minindex=cindex;
		}
		frontindex=minindex-1;
		v0=Vectormath.add(Vectormath.cmul(behaviour.sonar[minindex],Sonarnormal[minindex]),Sonarposition[minindex]);
		v1=Vectormath.sub(Vectormath.add(Vectormath.cmul(behaviour.sonar[frontindex],Sonarnormal[frontindex]),Sonarposition[frontindex]),v0);		rot=Math.max(0,Math.atan2(-v1.x,v1.y)*0.5);
		weight=0.5/behaviour.sonar[minindex];
		trans=0;
		if((minindex==7)||(rot<0.1))
		{
			weight=1;
		}
	};
	var timely=function()
	{
		toarbitrator.connection.send("arbitrator.wspeed("+trans+","+rot+","+weight+"); arbitrator.request(\"sonar\");");
	};
	this.kill=function()
	{
		toarbitrator.connection.close();
	};
	toarbitrator=new Client(datacallback,stopcallback);
	interval_id=setInterval(timely,100);
};

function B2()
{
	var behaviour=this;
	var interval_id;
	var toarbitrator;
	var trans=0;
	var rot=0;
	var weight=1;
	this.sonar=[5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000];
	var stopcallback=function(ref)
	{
		clearInterval(interval_id);
	};
	var datacallback=function(datastring,ref)
	{
		var v0;
		var v1;
		var minindex=0;
		var frontindex;
		var cindex;
		try
		{
			eval(datastring);
		}
		catch(err)
		{
			alert(err);
		}
		for(cindex=1;cindex<4;++cindex)
		{
			if(behaviour.sonar[minindex]>behaviour.sonar[cindex])
				minindex=cindex;
		}
		frontindex=minindex+1;
		v0=Vectormath.add(Vectormath.cmul(behaviour.sonar[minindex],Sonarnormal[minindex]),Sonarposition[minindex]);
		v1=Vectormath.sub(Vectormath.add(Vectormath.cmul(behaviour.sonar[frontindex],Sonarnormal[frontindex]),Sonarposition[frontindex]),v0);		rot=Math.min(0,Math.atan2(-v1.x,v1.y)*0.5);
		weight=0.5/behaviour.sonar[minindex];
		trans=0;
		if((minindex==0)||(rot>-0.1))
		{
			weight=1;
		}
	};
	var timely=function()
	{
		toarbitrator.connection.send("arbitrator.wspeed("+trans+","+rot+","+weight+"); arbitrator.request(\"sonar\");");
	};
	this.kill=function()
	{
		toarbitrator.connection.close();
	};
	toarbitrator=new Client(datacallback,stopcallback);
	interval_id=setInterval(timely,100);
};

