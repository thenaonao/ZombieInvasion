#library "Hud"
#Include "zcommon.acs"
#Import "Const.acs"
#Import "Func.acs"

script 800(void) {
	
	while(GetGameState()!=4){
		
		SetFont("BIGFONT");
		if(GetMapType()==2){
			Acs_execute(802,0);
		}
		
	delay(35);
	}
}

script 801(int minutes) CLIENTSIDE{ //so we dont use bandwidth to send each time the minutes.... even if its use once lol
	SetMinuteToTimer(minutes);
}
script 802() CLIENTSIDE{
	SetFont("BIGFONT");
	int minutes; //IKR this is called each time, created each time in a loop blah blah blah, im a lazy ass
	int secs;
	int truesecs;
	truesecs=GetTimerSeconds();
	minutes=truesecs/60;
	secs=truesecs%60;
	if(secs<10){
		HudMessageBold(s:"Time left: ",d:minutes,s:":0",d:secs,s:" ";HUDMSG_FADEOUT,301,CR_WHITE,0.5,0.92,2.0,1.0);
	}else{
		HudMessageBold(s:"Time left: ",d:minutes,s:":",d:secs,s:" ";HUDMSG_FADEOUT,301,CR_WHITE,0.5,0.92,2.0,1.0);
	}
	addSecondsToTimer(-1);//so we are still in the client side, so it must work lol ( yeah there is a lot of "lol") well /shrug .... I mean its my fault, i didnt account the fact of C/S stuffs, so stuffs getting messier when i tried to patch/fix those issues rofl
	SETFONT("HUDHUM");
	HudMessageBold(s:"A";HUDMSG_PLAIN, 304, CR_UNTRANSLATED, 0.9, 0.65, 1.0);
	SETFONT("HUDZOM");
	HudMessageBold(s:"A";HUDMSG_PLAIN, 305, CR_UNTRANSLATED, 0.9, 0.7, 1.0);
	SetFont("BIGFONT");
	HudMessageBold(s:"\cD",d:GetHumanNumber();HUDMSG_PLAIN,302,CR_UNTRANSLATED,0.93,0.65,1.0);
	HudMessageBold(s:"\cD",d:GetZombieNumber();HUDMSG_PLAIN,303,CR_UNTRANSLATED,0.93,0.7,1.0);

}

script 803(int trueseconds) CLIENTSIDE{ //If there is a desync or somewhat, each minutes, we update from the server, the total seconds
	setSecondsToTimer(trueseconds);
}

script 804 (int a,int b) CLIENTSIDE{
	SetHumanNumber(a);
	SetZombieNumber(b);
}

script 805(int a, int b,int c) CLIENTSIDE{

}
