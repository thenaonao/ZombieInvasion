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
	while(true){	
		SetFont("BIGFONT");
		Acs_execute(801,0,GetGameState(),IGetGameTypes);
		delay(35);
	}
}

script 801(int IGameState,int IGameType) CLIENTSIDE{
	switch(IGameState){
		case -1:
			HudMessage(s:"This Game needs at least 2 players";HUDMSG_FADEOUT,300,CR_WHITE,0.5,0.6,1.0,1.0);
			break;
		case 0:
			if(GetTimePickingZombie()<0){
			RequestScriptPuke(901,0);
			}
			HudMessage(s:"A Zombie will be picked in ",d:GetTimePickingZombie(),s:" ...";HUDMSG_FADEOUT,300,CR_WHITE,0.5,0.85,1.0,1.0);
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
					HudMessage(s:"Time left: ",d:minutes,s:":0",d:secs,s:" ";HUDMSG_FADEOUT,301,CR_WHITE,0.5,0.92,2.0,1.0);
				}else{
					HudMessage(s:"Time left: ",d:minutes,s:":",d:secs,s:" ";HUDMSG_FADEOUT,301,CR_WHITE,0.5,0.92,2.0,1.0);
				}
				if(truesecs<=10){
				RequestScriptPuke(901,0);//An attempt to Sync clients and server to have a final countrown synced.... better than before, but not working as i would hope
					if(truesecs<=2){
						AmbientSound("Effects/BIP2",127);
					}else{
						AmbientSound("Effects/BIP1",127);
					}
				}
			}
			SETFONT("HUDHUM3");
			HudMessage(s:"A";HUDMSG_PLAIN, 304, CR_UNTRANSLATED, 0.9, 0.65, 1.0);
			SETFONT("HUDZOM3");
			HudMessage(s:"A";HUDMSG_PLAIN, 305, CR_UNTRANSLATED, 0.9, 0.7, 1.0);
			SetFont("BIGFONT");
			HudMessage(s:"\cD",d:GetHumanNumber();HUDMSG_PLAIN,302,CR_UNTRANSLATED,0.93,0.65,1.0);
			HudMessage(s:"\cD",d:GetZombieNumber();HUDMSG_PLAIN,303,CR_UNTRANSLATED,0.93,0.7,1.0);
			if(getCountDownStatus()==1){
				Acs_executeAlways(803,0);
			}
			break;
		case 4:
			int IWinState=getWinState(); //(2021.01.17) Added this HUD win at the end of the round
			if(IWinState==-1){
			}else if(IWinState==1){
				SETFONT("HUDHWIN");
				HudMessage(s:"A";HUDMSG_PLAIN, 104, CR_UNTRANSLATED, 0.5, 0.15, 1.0);
				SetFont("BIGFONT");
				HudMessage(s:"Humans Win!";HUDMSG_PLAIN,105,CR_BLUE,0.5,0.3,1.0);
				
			}else if(IWinState==2){
				SETFONT("HUDZWIN");
				HudMessage(s:"A";HUDMSG_PLAIN, 104, CR_UNTRANSLATED, 0.5, 0.15, 1.0);
				SetFont("BIGFONT");
				HudMessage(s:"Zombies Win!";HUDMSG_PLAIN,105,CR_DARKRED,0.5,0.3,1.0);
			}
			break;
	}
}
bool alreadyShowed = false;//(2021.01.12)
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
		if(alreadyShowed){terminate;}
		HudMessageBold(s:"\cGA bunch of \cMZombie \cGappeared!";HUDMSG_FADEOUT,0,CR_UNTRANSLATED,x,y,3.0,1.0);
		alreadyShowed=true;//(2021.01.12)
	}
	else if(type==3){
		HudMessageBold(s:"\cM",n:infector,s:" \cGHas infected \cJ ",n:infected;HUDMSG_FADEOUT,0,CR_UNTRANSLATED,x,y,3.0,1.0);
	}
	else if(type==4){
		HudMessageBold(s:"\cJ",n:infected,s:" \cGGot infected mysteriously...";HUDMSG_FADEOUT,0,CR_UNTRANSLATED,x,y,3.0,1.0);
	}else{
		
	}
}

script 803(void)CLIENTSIDE{
	if(isNetworkGame()){
		DecrementTimer();
	}
}

script 804 ENTER CLIENTSIDE{
	SetFont("CONFONT");
	HudMessage(s:"\cgPro Tip: \n",s:tooltips[Random(0,7)];HUDMSG_FADEOUT,400,CR_WHITE,0.1,0.7,10.0,1.0,1.0,1.0);
}

script 806(void) CLIENTSIDE{
	SETFONT("HUDSTUNT");
	HudMessage(s:"A"; HUDMSG_FADEINOUT, 306, CR_UNTRANSLATED, 0.5, 0.5, 0.5,0.5, 2.0);
}
