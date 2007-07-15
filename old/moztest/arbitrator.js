function Arbitrator(watchdogport)
{
	var towatchdog;
	var tobehaviours;
	var arbitrator=this;
	this.currentconnection=null;
	this.translation=0;
	this.rotation=0;
	this.weight=1;
	this.sonar=[5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000];
	var watchdoghandler=function(datastring,conn)
	{
		try
		{
			eval(datastring);
		}
		catch(err)
		{
			alert(err);
		}
		conn.send("watchdog.setspeed("+arbitrator.translation+","+arbitrator.rotation+");");
		arbitrator.weight=Math.max(1,Math.pow(arbitrator.weight,0.125));
	};
	var watchdogclosehandler=function(conn)
	{	
		arbitrator.rotation=0;
		arbitrator.translate=0;
		arbitrator.weight=1;
		alert("watchdog dead");
		tobehaviours.stop();
	};
	var behaviourhandler=function(datastring,conn)
	{
		arbitrator.currentconnection=conn;
		try
		{
			eval(datastring);
		}
		catch(err)
		{
			alert(err);
		}
	};
	var noconnhandler=function()
	{
		arbitrator.translation=0;
		arbitrator.rotation=0;
		arbitrator.weight=1;	
	};
	towatchdog=new Client(watchdoghandler,watchdogclosehandler,watchdogport);
	tobehaviours=new Server(behaviourhandler,noconnhandler);
};

Arbitrator.prototype.stop=function()
{
	this.translation=0;
	this.rotation=0;
	this.weight=8;
};

Arbitrator.prototype.request=function(devicename)
{
	if(!this.currentconnection.send("behaviour."+devicename+"="+this[devicename].toSource()+";"))
		alert("behaviour dead");
};

Arbitrator.prototype.wspeed=function(t,r,w)
{
	var dweight=(this.weight-w)/2;
	if(dweight>0)
	{
		this.translation=(1-dweight/this.weight)*this.translation;
	}
	else
	{	
		this.translation=(dweight/w+1)*t;
		this.rotation=r;
	}
	this.weight+=Math.max(w,this.weight);
};
