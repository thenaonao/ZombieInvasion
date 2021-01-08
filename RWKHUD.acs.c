#library "RwkHud"
#Include "zcommon.acs"
#Import "Const.acs"
#Import "Func.acs"

int timeBeforePicking;
int minutes;
int secs;
int truesecs;

script 800(void){
	int IGetGameTypes=GetMapType();
	while(GetGameState()!=4){	
		SetFont("BIGFONT");
		Acs_execute(801,0,GetGameState(),IGetGameTypes);
		delay(35);
	}
}

script 801(int IGameState,int IGameType) CLIENTSIDE{
	switch(IGameState){
		case -1:
			HudMessageBold(s:"This Game needs at least 2 players";HUDMSG_FADEOUT,300,CR_WHITE,0.5,0.6,1.0,1.0);
			break;
		case 0:
			if(GetTimePickingZombie()<0){
			RequestScriptPuke(901,0);
			}
			HudMessageBold(s:"A Zombie will be picked in ",d:GetTimePickingZombie(),s:" ...";HUDMSG_FADEOUT,300,CR_WHITE,0.5,0.85,1.0,1.0);
			break;
		case 1:
		case 2:
		case 3:
			if(IGameType==2){ //Round Timer
				SetFont("BIGFONT");
				truesecs=GetTimerSeconds();
				minutes=truesecs/60;
				secs=truesecs%60;
				if(secs<10){
					HudMessageBold(s:"Time left: ",d:minutes,s:":0",d:secs,s:" ";HUDMSG_FADEOUT,301,CR_WHITE,0.5,0.92,2.0,1.0);
				}else{
					HudMessageBold(s:"Time left: ",d:minutes,s:":",d:secs,s:" ";HUDMSG_FADEOUT,301,CR_WHITE,0.5,0.92,2.0,1.0);
				}
			}
			SETFONT("HUDHUM");
			HudMessageBold(s:"A";HUDMSG_PLAIN, 304, CR_UNTRANSLATED, 0.9, 0.65, 1.0);
			SETFONT("HUDZOM");
			HudMessageBold(s:"A";HUDMSG_PLAIN, 305, CR_UNTRANSLATED, 0.9, 0.7, 1.0);
			SetFont("BIGFONT");
			HudMessageBold(s:"\cD",d:GetHumanNumber();HUDMSG_PLAIN,302,CR_UNTRANSLATED,0.93,0.65,1.0);
			HudMessageBold(s:"\cD",d:GetZombieNumber();HUDMSG_PLAIN,303,CR_UNTRANSLATED,0.93,0.7,1.0);
			break;
		case 4:
		
			break;
	}
}

script 802 (int CompressedTID,int y,int type) CLIENTSIDE{
	delay(1);//This is some patchwork, but yea, this script was faster than functions.
	SetFont("SMALLFONT");
	int infected=0;
	int infector=0;
	//Decode
	infector=CompressedTID>>8;
	infected=CompressedTID-(infector<<8);
	infector++;//For n: ->(1;64) and here we have (0;63)
	infected++;
	int x= 0.5;	
	if(type==1){	
		HudMessageBold(s:"\cJ",n:infected,s:" \cGgot infected by a \cMVirus!";HUDMSG_FADEOUT,0,CR_UNTRANSLATED,x,y,3.0,1.0);
	}else if(type==2){
		HudMessageBold(s:"\cGA bunch of \cMZombie \cGappeared!";HUDMSG_FADEOUT,0,CR_UNTRANSLATED,x,y,3.0,1.0);
	}
	else if(type==3){
		HudMessageBold(s:"\cM",n:infector,s:" \cGHas infected \cJ ",n:infected;HUDMSG_FADEOUT,0,CR_UNTRANSLATED,x,y,3.0,1.0);
	}
	else if(type==4){
		HudMessageBold(s:"\cJ",n:infected,s:" \cGGot infected mysteriously...";HUDMSG_FADEOUT,0,CR_UNTRANSLATED,x,y,3.0,1.0);
	}else{
		
	}
}

script 806(void) CLIENTSIDE{
	SETFONT("HUDSTUNT");
	HudMessage(s:"A"; HUDMSG_FADEINOUT, 306, CR_UNTRANSLATED, 0.5, 0.5, 0.5,0.5, 2.0);
}
