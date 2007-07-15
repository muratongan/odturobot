#include <libplayerc++/playerc++.h>
#include <iostream>
using namespace PlayerCc;


int main()
{
	PlayerClient client("localhost",6665);
	Position2dProxy pos(&client,1);
	pos.SetMotorEnable(true);
	while(1){pos.SetSpeed((double)0.5,(double)0);
			usleep(200000);}
	return 0;
}
