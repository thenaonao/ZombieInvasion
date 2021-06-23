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
	int infector=CompressedTID>>8;
	int infected=CompressedTID-(infector<<8);
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

script 807(void) CLIENTSIDE{
	SETFONT("HUDINFEC");
	HudMessage(s:"A"; HUDMSG_FADEINOUT, 306, CR_UNTRANSLATED, 0.5, 0.5, 0.5,0.5, 0.75);
}

script 808(void) NET CLIENTSIDE{

	for(int time=0;time<35*5;time++){
	//while(true){
		int cur_x=GetActorX(activatorTID());
		int cur_y=GetActorY(activatorTID());
		int cur_z=GetActorZ(activatorTID());
		int dist_min=1<<25;
		int actual_i=0;
		for(int i=0;i<64;i++){//Spot the nearest human, Because its freaking OP... imagine having 20 spotter zm in 1 round... LOL
			if(CheckActorInventory(i+PLAYER_ID,"IsHuman")==1 && i+PLAYER_ID!=activatorTID()){
				int enn_x=GetActorX(i+PLAYER_ID);
				int enn_y=GetActorY(i+PLAYER_ID);
				int enn_z=GetActorZ(i+PLAYER_ID);
				//int cur_dist=ThreeD_Dist(enn_x-cur_x,enn_y-cur_y,enn_z-cur_z);//The nearest in a 3D space
				int cur_dist=TwoD_Dist(enn_x-cur_x,enn_y-cur_y);
				if(cur_dist<dist_min){
					dist_min=cur_dist;
					actual_i=i; 
				}
			}
		}
		
		int vang = VectorAngle (GetActorX(actual_i+PLAYER_ID)-cur_x,GetActorY(actual_i+PLAYER_ID)-cur_y);
        int angle = (vang-GetActorAngle(ActivatorTID()) + 1.0) % 1.0;
		
		if(((vang+0.125)%0.5) > 0.25) dist_min = fixeddiv(GetActorY(actual_i+PLAYER_ID)-cur_y, sin(vang));
		else dist_min = fixeddiv(GetActorX(actual_i+PLAYER_ID)-cur_x, cos(vang));
		
		
		int pitch = VectorAngle(dist_min,GetActorZ(actual_i+PLAYER_ID)-(cur_z + 32.0));
		pitch = (pitch + GetActorPitch(ActivatorTID()) + 1.0) % 1.0;
        if (angle < 0.2 || angle > 0.8)
        {
            int sx = 320 - (320 * Sin (angle) / Cos (angle));
			int sy=0;
			if(Cos (pitch)==0){
				sy = 240 - (240 * Sin (pitch) / Cos (0.24));
			}else{
				sy = 240 - (240 * Sin (pitch) / Cos (pitch));
			}
            SetHudSize (640, 480, 0);
			SetFont("HUDHEAD");
            HudMessage (s:"A"; HUDMSG_PLAIN, 1, CR_RED, sx * 1.0, sy*1.0, 0);
        }
        else
        {
            HudMessage (s:""; HUDMSG_PLAIN, 1, 0, 0, 0, 0);
        }

        Delay(1);
		}
	HudMessage (s:""; HUDMSG_PLAIN, 1, CR_RED,0, 0, 0);
	
}
