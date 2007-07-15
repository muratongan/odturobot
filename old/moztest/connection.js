function Connection(transport,datacallback,stopcallback)
{
	var pump;
	var instream;
	var scrptstream;
	var outstream;
	var that=this;
	var listener=
	{
		onStartRequest:function(request,context)
		{
			dump("connection started\n");
		},
		onStopRequest:function(request,context,status)
		{
			dump("connection stopped\n");
			if(stopcallback)
			{
				stopcallback(that);
			}
		},
		onDataAvailable:function(request,context,inputstream,offset,count)
		{
			if(datacallback)
			{
				datacallback(scrptstream.read(count),that);
			}
		}
	};
	instream=transport.openInputStream(0,0,0);
	scrptstream=Components.classes["@mozilla.org/scriptableinputstream;1"].createInstance(Components.interfaces.nsIScriptableInputStream);
	scrptstream.init(instream);
	pump=Components.classes["@mozilla.org/network/input-stream-pump;1"].createInstance(Components.interfaces.nsIInputStreamPump);
	pump.init(instream,-1,-1,0,0,false);
	pump.asyncRead(listener,null);
	outstream=transport.openOutputStream(0,0,0);
	this.send=function(data)
	{
		if(transport.isAlive())
		{
			outstream.write(data,data.length);
			return true;
		}
		else
		{
			return false;
		}
	};
	this.close=function()
	{
		transport.close(0);
	};
};

function Client()
{
	var datacallback=null;
	var stopcallback=null;
	var transport;
	var host="localhost";
	var port=6112;
	switch(arguments.length)
	{
		case 4:
			host=arguments[3];
		case 3:
			port=arguments[2];
		case 2:
			stopcallback=arguments[1];
		case 1:
			datacallback=arguments[0];
		default:
			break;
	}
	transport=Components.classes["@mozilla.org/network/socket-transport-service;1"].getService(Components.interfaces.nsISocketTransportService).createTransport(null,0,host,port,null);
	this.connection=new Connection(transport,datacallback,stopcallback);
};

function Server()
{
	var socket;
	var datacallback=null;
	var noconncallback=null;
	var port=6112;
	var connections=[];
	var stopcallback=function(ref)
	{
		var count=connections.length;
		var i;
		for(i=0;i<count;++i)
		{
			if(ref==connections[i])
			{
				connections.splice(i,1);
				break;
			}
		}
		if(count==1)
			noconncallback();
		dump("client connection stopped:"+i+"\n");
	};
	var acceptor=
	{
		onSocketAccepted:function(serversocket,transport)
		{
			dump("accepted\n");
			connections.push(new Connection(transport,datacallback,stopcallback));
		},
		onStopListening:function(serversocket,status)
		{
			for each(var connection in connections)
				connection.close();
			dump("stopped\n");
		}
	};
	switch(arguments.length)
	{
		case 3:
			port=arguments[2];
		case 2:
			noconncallback=arguments[1];
		case 1:
			datacallback=arguments[0];
		default:
			break;
	}
	socket=Components.classes["@mozilla.org/network/server-socket;1"].createInstance(Components.interfaces.nsIServerSocket);
	socket.init(port,false,-1);
	socket.asyncListen(acceptor);
	this.stop=function()
	{
		socket.close();
	};
};
