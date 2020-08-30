#library "Game"
#Include "zcommon.acs"
#Import "Const.acs"
#Import "Func.acs"
#Import "Hud.acs"

// GAME START SCRIPT
script 700 (int type,int SecureTime,int ZombieImmuneTime)
{
	bool restarter= false;
	int restartertimer=0;
	int ResetSecureTime = SecureTime;
	SetMapState(0);
	if(type!=1 && type!=2){
		printbold(s:"Bad Game Configuration");
		terminate;
	}
	if(Securetime <= 4){
		SecureTime=10;
		printbold(s:"SecureTime Value is wrong... Corrected internally ._.");
	}
	if(ZombieImmuneTime < 0){
		ZombieImmuneTime=0;
		printbold(s:"ZombieImmuneTime Value is wrong... Corrected internally ._.");
	}
	
	//InventoryStarter(); //Give all basic human stuff
	SetPlayerProperty(TRUE, OFF, PROP_TOTALLYFROZEN);
	SetMapType(type);
	
	while(true){
		if(PlayerCounter()<2){
			HudMessageBold(s:"This Game needs at least 2 players";HUDMSG_FADEOUT,300,CR_WHITE,0.5,0.6,1.0,1.0);
			SecureTime = ResetSecureTime;
			restartertimer++;
		}else{
			if(restartertimer>=10){
				printbold(s:"Restarting the round...!");
				delay(35*5);
				ResetMap();
			}
			if(type==2){SetMinuteToTimer(ZombieImmuneTime);}
			
			
			if(SecureTime>0){		
				SetFont("SMALLFONT");
				HudMessageBold(s:"The Game Starts in ",d:SecureTime,s:" ...";HUDMSG_FADEOUT,300,CR_WHITE,0.5,0.85,1.0,1.0);
				SecureTime--;
			}else{break;}
		}
		delay(35);
	}
	SetGameState(0);
	acs_execute(710,0); // FOR HUD
	acs_execute(711,0);
	
	//For each player, we add 1 to the chance of being choosen
	//(2020/07/31): fixed.
	//for(int k = 0; k<HumanCounter(); k++) {
	for(int k = 0; k<64; k++) {
		if(PlayerInGame(k)){
			AddZombifyChanceFactor(k);	
		}
	}
	
	ZombieStarter();
	delay(35);
	//TODO need to TEST THIS FEATURE
	Acs_execute(721,0,ZombieImmuneTime,type);
	
	SetGameState(2);

	Delay(70);
	//So it doesnt call the same function multiple times in a loop
	int timerSec;
	int currentMapState;
	int currentMapType = GetMapType();
	int maxMapState = GetMapStateMaximum();
	int zmNbrTimer=0;
	int nbrHum = HumanCounter();
	int nbrZom = ZombieCounter();
	
	//MAIN GAME LOGIC
	while(GetGameState()!=4){
		GiveStamina();
		timerSec = GetTimerSeconds();
		currentMapState = GetMapState();
		nbrHum = HumanCounter();
		nbrZom = ZombieCounter();
		if(currentMapType==1){
			/* To end the round as a human, you have to finish the map. So 
				it will force ppl to go at the end and not bully the zombies
			if(ZombieCounter()==0){
				SetGameState(4);
				printbold(s:"Human Victory");
				EndMap(0);
				break;
			}
			*/
			if(nbrHum==0){
				SetGameState(4);
				printbold(s:"Zombie Victory");
				EndMap(0);
				break;
			}
			if(currentMapState==maxMapState){
				EndMap(1);
				break;
			}
			if(nbrZom==0){
				if(zmNbrTimer>8){
					PrintBold(s:"Human Victory, Zombie RQ!!");
					EndMap(0);
					break;
				}
				zmNbrTimer++;
			}else{zmNbrTimer=0;}
		}
		if(currentMapType==2){
			if(nbrHum==0){
				printbold(s:"Zombie Victory");
				EndMap(0);
				break;
			}
			if(nbrZom>0 && nbrHum==1){ //heartbeat
				for(int j =0;j<64;j++){
					if(CheckActorInventory(j+PLAYER_ID,"IsHuman") && PlayerInGame(j)){
						acs_execute(719,0,j,0,0);
					}
				}
			}
			//So, if  there is no zombie (like he rq or something...)
			if(nbrZom==0){
				if(zmNbrTimer>5){
					PrintBold(s:"Human Victory");
					EndMap(0);
					break;
				}
				zmNbrTimer++;
			}else{zmNbrTimer=0;}
			if(timerSec==0){
				EndMap(1);
				//kill zm maybe ?
				printbold(s:"Human Victory");
				break;
			}else{
				addSecondsToTimer(-1);
			}
		}
		delay(35);
	} 

	if(currentMapType==1){
		while(ZombieCounter()!=0 && HumanCounter()!=0){
			delay(5);
		}
		if(ZombieCounter()==0 && HumanCounter()==0){
			Printbold(s:"uh? Everyone died ?????");
			delay(35*2);
			ResetMap();
		}else if(ZombieCounter()==0){
			PrintBold(s:"Human Victory");
		}else{
			PrintBold(s:"Zombie Victory");
		}
		
		
	}
	
	int Laura;
	int Ashley;
	int CoolDude=Random(0,PlayerCount()-1);
	for(int JeanPaul=0;JeanPaul<64;JeanPaul++){
		if(GetSafePlayer(CoolDude==1)){
			Ashley=1;
			AwardPlayer(CoolDude);
			Log(s:"Tried Awarding");
			Log(n:CoolDude+1);
			break;
		}
		CoolDude=Random(0,PlayerCount()-1);
	}
	
	if(!Ashley){
		for(int Kassandra=0;Kassandra<64;Kassandra++){
			if(GetSafePlayer(Kassandra)==1){
				AwardPlayer(Kassandra);
				Log(s:"Tried Awarding 2");
				Log(n:Kassandra+1);
				break;
			}
		}
	}
	
	
	
	//NOW WE FREEZE EVERYBODY SO THEY CANT MOVE LOL
	SetPlayerProperty(TRUE, ON, PROP_TOTALLYFROZEN);
	for(int w=0;w<64;w++){
		SetActorProperty(w+PLAYER_ID,APROP_Invulnerable,ON);
		SetActorProperty(w+PLAYER_ID, APROP_SPEED, 1.0);
		
		
	}
	delay(35*5);
	ResetInv();
	resetStats();
	SetMapStateMaximum(0);
	SetMapState(0);
	SetMapType(0);
	Exit_Normal(0);
}

//GIVING TID TO PLAYERS		AUTOMATIC
script 701 ENTER{
	DestroyInv(PlayerNumber());
	SetPlayerTID(PlayerNumber());
	SetAliveState(PlayerNumber());
	if(GetGameState()<=1){
		InventoryReStarter(PlayerNumber());
	}else{
		ClearInventory();
		MakeZombie(PlayerNumber()+PLAYER_ID,0,0,1);
		Teleport_NoFog(ZombieTID+GetMapState(),1,0,0);
	}
}

script 702 DEATH{
	ClearInventory();
	DestroyInv(PlayerNumber());
	SetDeathState(PlayerNumber());
	thing_remove(500+PLAYER_ID+PlayerNumber());
	Thing_ChangeTID(0, 0);
}
script 703 RESPAWN{
	Thing_ChangeTID(0, 0);
	SetPlayerTID(PlayerNumber());
	DestroyInv(PlayerNumber());
	SetAliveState(PlayerNumber());
	CleanInv(PlayerNumber());
	if(GetGameState()==2){
		ClearInventory();
		MakeZombie(PlayerNumber()+PLAYER_ID,0,0,1);
		Teleport_NoFog(ZombieTID+GetMapState(),1,0,0);
	}else if(GetGameState()>=3){
		//SetDeadSpectator(PlayerNumber(),1);
		//MakeSpectator(PlayerNumber()+PLAYER_ID);
		SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
	}else{
		InventoryReStarter(PlayerNumber());
	}
}


//Infect Random TID at start
int ArrayRandom[2048]; //Should be enought lol( I think worst case scenario is like 64 player with 32 max factor ? 32*64 = 2048)
//I could be wrong, maybe up to 48 max factor ? XD 
//(2020/07/31): so 64*48...
//(2020/07/31): Also inspecting for a possible bug if player1 is spectating, and player0 & player2 are playing = Insta infect everyone?
script 704(int NbrOfZombieNeeded){

	int NbrOfPlayers=HumanCounter();
	int PlayerToZombify;
	
	int GZCF =0; //See below what this name means
	int TotalFactor=0;
	int chosenTID;
	int loopChecker=0; //Cause im lazy
	while(NbrOfZombieNeeded!=0)
	{
	

		
		/* OLD RANDOM
		PlayerToZombify = ClipRandom(1,NbrOfPlayers,Random(1,NbrOfPlayers));
		PlayerToZombify+=PLAYER_ID;
		PlayerToZombify--;
		*/
		/*
		for(int i=0;i<NbrOfPlayers;i++){
			TotalFactor+=GetZombifyChanceFactor(i);
		}
		*/
		
		//for(int j=0;j<NbrOfPlayers;j++){ (2020/07/31): Possible try of fix the insta infect ? 
		for(int j=0;j<64;j++){
			if(PlayerInGame(j)){
				GZCF = GetZombifyChanceFactor(j);
				TotalFactor+=GZCF;
				for(int factor=0;factor<GZCF;factor++){
					
					ArrayRandom[loopChecker] = (j+PLAYER_ID);
					loopChecker++;
				}
			}
		}
		//so like the range is 0 to 63 = 64players.....
		chosenTID = Random(0,TotalFactor-1);
		PlayerToZombify = ArrayRandom[chosenTID];
		//Create a zombie
		if(CheckActorInventory(PlayerToZombify,"IsZombie")==0){
			TakeActorInventory(PlayerToZombify,"IsHuman",1);
			TakeActorInventory(PlayerToZombify,"HumanImmune",1);
			GiveActorInventory(PlayerToZombify,"IsZombie",1);
			GiveActorInventory(PlayerToZombify,"ZombieImmune",1);
			MakeZombie(PlayerToZombify,1,0,0);
			PlayerToZombify-=PLAYER_ID;
			SetZombifyChanceFactor(PlayerToZombify,-1);
			NbrOfZombieNeeded--;
		}
		delay(3);
	}
}
//Infection script
script 705(int player,int infector,int type){
	if(player==0){Log(s:"Error, Player==0"); terminate;}
	MakeZombie(player,type,infector,0);
	
}
//Drop Grenade Script
script 706(int player, int howmany,int type){
	if(type==0){
		Spawn("MolotovCollect",GetActorX(player)+5.0,GetActorY(player)+5.0,GetActorZ(player),0,0);
		delay(1);
		if(howmany==2){
			Spawn("MolotovCollect",GetActorX(player)-5.0,GetActorY(player)-5.0,GetActorZ(player),0,0);
		}
	}
	if(type==1){
		Spawn("StuntGrenadeCollect",GetActorX(player)+10.0,GetActorY(player)-10.0,GetActorZ(player),0,0);
		delay(1);
		if(howmany==2){
			Spawn("StuntGrenadeCollect",GetActorX(player)-10.0,GetActorY(player)+10.0,GetActorZ(player),0,0);
		}
	}
}
//Zombie burn script
Script 707 (void) 
{
	GiveInventory("IsBurning", 1);
	for(int i=0;i<=9;i++) {
		if(GetActorProperty(0,APROP_Waterlevel)>=2){
			TakeInventory("IsBurning", 1);
			terminate;
		}
		Thing_Damage(0, 10+Random(0,8), 0);
		ThrustThingZ(0,1,1,0);
		SpawnProjectile(0, "ZIFlame", 0, 0, 0, 0, 0);
		Delay(18);
	}
	TakeInventory("IsBurning", 1);
}
/*
 //Value Setter, Please use this script instead of 709 to update a intern value
script 708(int a,int b, int c)NET{
	ACS_ExecuteAlways(709,0,a,b,c);
}
//Do not use this script
script 709(int a, int b,int c)CLIENTSIDE{
	//UpdateInternValue(a,b,c);
}
*/
// HUD SCRIPT SETUP -> To update  value, call script 708
script 710 (void){
	Acs_execute(800,0);
}

// Type of msg: 0-> NULL
//				1-> Deprecated
//				2-> Normal Infect MSG
//				3-> Item Infect MSG
//
//				4,5,6,7,8->Mother Zombie MSG
//
//
//
//  who: TID of the infected
//
//  infector: TID of the infector
//			  666 is the TID of VIRUS ITEM
script 711 (void)  {
/*	//str a = GetPlayerAccountName(who);
	//str b;
	//str c;
	int a = who;
	int b,c;
	int d=0;
	if(infector !=666){
		c = infector; //666 TID is a Virus ITEM
	}else{
		type=3;
		c=-1;
	}
	
	switch(type){
	case 4:
		if(d==0){d=1;}
	case 5:
		if(d==0){d=2;}
	case 6:
		if(d==0){d=3;}
	case 7:
		if(d==0){d=4;}
		//b = " \cGGot infected by a virus!";
		b=1;
		break; 
	case 8:
		d=5;
		//b = "A bunch of Zombie appeared!";
		b=2;
		break;
	case 2:
		//b = " \cGGot infect by \cM";
		b=3;
		break;
	case 3:
		//b = " \cgGot infected mysteriously...";
		b=4;
		break;
	}
	//b type msg
	//c tid
	//d how many
	//
	//int x,int y,int id,int infected,int b,int infector */
	int x,y,id,type;
	int infected,infector;
	int secWait,incrementator;
	int InfectedDisplay; 
	int currentGameState = GetGameState();
	while(currentGameState!=4){
		if(currentGameState!= 1){
			delay(10);
		}else{
			delay(2);
		}
		InfectedDisplay=GetUpdateOnInfect();
		if(InfectedDisplay>0){
			type = GetInfectMsgType();
			id = incrementator;
			if(type!=2){
			y = GetVarY()+(incrementator*0.03);
			x = GetVarX();
			}else{
			y = GetVarY();
			x = GetVarX();
			}
			infected = GetInfectedTID();
			infector = GetInfectorTID();
			InfectMessage(x,0.3+y,id,infected,type,infector);
			RemoveUpdateOnInfect(1);
			secWait=0;
			incrementator++;
		}else{
			secWait++;
			if(secWait>=(3*34)){
				incrementator=0;
			}
		}
		
	}
	currentGameState = GetGameState();
	//InfectMessage(d,who,b,c);
}
//Check if Zombie; then push in the direction
//0 EAST; 1 NORTH; 2 WEST; 3 SOUTH
script 712 (int direction,int force){
	int push= force*20;
	if(CheckInventory("IsZombie")==1){
		switch(direction){
			case 0:
				ThrustThing(0,push,1,0);
				break;
			case 1:
				ThrustThing(64,push,1,0);
				break;
			case 2:
				ThrustThing(128,push,1,0);
				break;
			case 3:
				ThrustThing(192,push,1,0);
				break;
			default:
				PrintBold(s:"Error pushing !! Script 712");
				break;
		}
	}
}
//Glass destroy script
script 713(int WhatSector){
	SetLineBlocking(WhatSector,BLOCK_NOTHING);
	SetLineTexture(WhatSector,SIDE_FRONT,TEXTURE_MIDDLE ,"-");
	SetLineTexture(WhatSector,SIDE_BACK ,TEXTURE_MIDDLE ,"-");
	Playsound(0,"GLASSBRK",5,1.0,FALSE,ATTN_STATIC);
}

//Reset the tid and for ZombieChanceFactor
script 714 (int whoIsGONE) Disconnect{
	DestroyInv(whoIsGONE);
	SetDeathState(whoIsGONE);
	Thing_ChangeTID(0, 0);
	SetZombifyChanceFactor(whoIsGONE,0);
	thing_remove(500+whoIsGONE+PLAYER_ID);
}

//Sprint pre-Checker
script 715 (int IsRunning) NET{
	if(!PlayerInGame(PlayerNumber())){terminate;}
	
	if(IsRunning){
		if(CheckInventory("IsHuman") && GetMapType()==2 && GetGameState()>=2){ //If its ZR MAP we check before like a priority
			GiveInventory("IsSprinting",1);
			Acs_ExecuteAlways(716, 0, 0, 0, 0);
		}else if(CheckInventory("IsHuman") && GetGameState()>=2){ // for ZF maps...
			GiveInventory("IsSprinting",1);
			Acs_ExecuteAlways(716, 0, 0, 0, 0);
		}else{ //If its ZF, but game has not started yet
			print(s:"Why r u running ?");
		}
	}else{
		TakeInventory("IsSprinting",1);
	}
			

}
//Sprint Loop checker
Script 716 (void) {
	while(CheckInventory("StaminaSprint")>10 && CheckInventory("IsSprinting")==1) {
		SetActorProperty(0, APROP_Speed, 1.25); //panicking is +0.15
		//if(!CheckInventory("EnergyDrinkEffect"))
		TakeInventory("StaminaSprint", 40);
		Delay(10);
	}//If we break out of the loop, then it means ppl has no more stamina or dont sprint anymore
	if(CheckInventory("IsHuman")){
		SetActorProperty(0, APROP_Speed, 1.0);
	}
}
//Panic script
Script 717 (void) NET {

	if( !PlayerInGame(PlayerNumber())){terminate;}
		
	switch(GetGameState())
	{
		case -1:
		case 0:
		case 1:
			Print(s:"Is there something to be afraid of ?");
			break;		
		case 2:
			if(CheckInventory("IsHuman"))
			{
				if(CheckInventory("HasPanicked")){
					Print(s:"You have already panicked this round!");
				}else if(CheckInventory("StaminaSprint") != 1200){
					Print(s:"You dont have enough stamina to panic!!");
					terminate;
				}
				else
				{
					Acs_execute(718,0,0,0,0);
					TakeInventory("StaminaSprint", 1200);
					GiveInventory("HasPanicked", 1);
					GiveInventory("PanicSpeed", 1);
					
					TakeInventory("ZIPistol", 1);
					TakeInventory("ZIShotgun", 1);
					TakeInventory("ZISuperShotgun", 1);
					TakeInventory("ZIRifle", 1);
					TakeInventory("ZISniper", 1);
					TakeInventory("ZIRocketLauncher", 1);
					
					GiveInventory("ZIKnife", 1);
					HudmessageBold(n:0, s:" \cJhas panicked!";
								HUDMSG_FADEOUT, 360, CR_WHITE, 0.5, 0.43, 2.0); 
				}
			}
			break;
		
		case 3:
		case 4:
			Print(s:"Why do you want to panic ? IT'S OVER!");
			break;
	}
}
//Panic breath at the end
Script 718 (void) CLIENTSIDE
{
	while(CheckInventory("PanicSpeed")){
		delay(35);
	}
	
	if(PlayerInGame(PlayerNumber()) && !CheckInventory("IsZombie")){
		if(GetPlayerInfo(PlayerNumber(), PLAYERINFO_GENDER)==1){
			ActivatorSound("Player/WBreath",127);
		}else{
			ActivatorSound("Player/MBreath",127);
		}
	}
}
//Heart beat script
script 719 (int who){
	while(GetGameState() == 2 && CheckActorInventory(who+PLAYER_ID,"IsHuman")==1 && HumanCounter()==1){
		PlaySound(who+PLAYER_ID,"Player/HeartBeat",4,1.0,0,1.0);
		delay(35*3);
	}
}
// Checkpoint tele
Script 720 (void) NET {
	int P_id = PLAYER_ID+PlayerNumber();
	if(CheckActorInventory(P_id,"IsZombie")==1){
		if(GetMapType()==2){//If ZR
			if(CheckActorInventory(P_id,"TPChecker") > 0){
				print(s:"you will be teleported in 3 seconds");
				delay(35*3);
				TeleportOther(P_id,200,1);
				TakeActorInventory(P_id,"TPChecker",1);
				Print(s:"You now have ",d:CheckActorInventory(P_id,"TPChecker"),s:" TP left.");
			}else{
				Print(s:"You cannot teleport anymore...");
			}
		}else if(GetMapType()==1){ //IF ZF
			print(s:"you will be teleported in 3 seconds");
			delay(35*3);
			Teleport_NoFog(ZombieTID+GetMapState(),1,0,0);
		}
	}else{
		Print(s:"Humans cannot tp...");
	}
}
//ZImmunity script
script 721 (int ZombieImmuneTime,int type) {
	while(ZombieImmuneTime!=0 && type==1){
		for(int i=0;i<GetTotalZombieAtStart();i++){
			GiveZombieImmunity(GetZombieMotherTID(i));
		}
		ZombieImmuneTime--;
		delay(35);
	}
}

script 722 (int who,int type){
	int r1 = Random(0,100); //Rarity
	int r2 = Random(0,100); //Random inside the rarity
	int HealthNew = GetActorProperty(who,APROP_Health);
	if(GetGameState()<=1){
		Print(s:"You cant use that now.");
		terminate;
	}
	if(CheckActorInventory(who,"IsZombie")==1 && type < 5){
		Print(s:"You cant use that now.");
		terminate;
	}
	switch(type){
		case 1:
			TakeActorInventory(who,"CommonBonusCrate",1);
			if(r1<=75){ //COMMON
				if(r2<=50){
					Print(s:"Nothing! :c");
				}
				else if(r2<=75){//Sprint Half restock
					Print(s:"Stamina restock!");
					GiveActorInventory(who,"StaminaSprint",600);
				}
				else {//Health 50
					Print(s:"You got healed by 50%!");
					HealthNew+=50;
					SetActorProperty(who,APROP_Health,HealthNew);
				}
			}
			else if(r1<=90){ //UNCOMMON
				if(r2<=20){ //Molotov
					Print(s:"Additional molotov!");
					GiveActorInventory(who,"HandGrenadeAmmo",1);
				}
				else if(r2<=40){//Stunt grenade
					Print(s:"Additional Stunt grenade!");
					GiveActorInventory(who,"HandExpGrenadeAmmo",1);
				}
				else if(r2<=60){//10% DMG
					Print(s:"10% damage for 30 secondes!");
					GiveActorInventory(who,"DamageBonus",1);
				}
				else if(r2<=80){//Full Stamina restock
					if(CheckActorInventory(who,"HasPanicked")==1){
						Print(s:"10% damage for 30 secondes!");
						GiveActorInventory(who,"DamageBonus",1);
					}
					else{
					Print(s:"Full stamina restock!");
					GiveActorInventory(who,"StaminaSprint",1200);
					}
				}
				else{ //Sniper
					Print(s:"You found a sniper!");
					GiveActorInventory(who,"ZISniper",1);
				}
			}
			else if(r1<=98){ //RARE
				if(r2<=25){
					Print(s:"10% damage for 30 secondes!");
					GiveActorInventory(who,"DamageBonus",1);
				}
				else if(r2<=50){
					Print(s:"You found a SSG!");
					GiveActorInventory(who,"ZISuperShotGun",1);
				}
				else if(r2<=75){
					Print(s:"You found a Rocket Launcher!");
					GiveActorInventory(who,"ZIRocketLauncher",1);
				}
				else{
					Print(s:"Fall reduction!");
					GiveActorInventory(who,"BonusFallDamageProtection",1);
				}
			}
			else{ //EPIC
				//Well for  the moment, there is no epic bonus unless  the elite one, so i implement it right there
				PrintBold(n:who-PLAYER_ID+1,s:" is now an ELITE!");
				GiveActorInventory(who,"LeaderSpeed",1);
				GiveActorInventory(who,"LeaderDamage",1);
			}
			break;
		case 2:
			TakeActorInventory(who,"UncommonBonusCrate",1);
			if(r1<=90){ //Common+Uncommon
				if(r2<=15){
					if(CheckActorInventory(who,"HasPanicked")==1){
						Print(s:"10% damage for 30 secondes!");
						GiveActorInventory(who,"DamageBonus",1);
					}
					else{
					Print(s:"Full stamina restock!");
					GiveActorInventory(who,"StaminaSprint",1200);
					}
				}
				else if(r2<=30){
					Print(s:"10% damage for 30 secondes!");
					GiveActorInventory(who,"DamageBonus",1);
				}
				else if(r2<=45){
					Print(s:"You got healed by 50%!");
					HealthNew+=50;
					SetActorProperty(who,APROP_Health,HealthNew);
				}
				else if(r2<=60){
					Print(s:"Additional Stunt grenade!");
					GiveActorInventory(who,"HandExpGrenadeAmmo",1);
				}
				else if(r2<=75){
					Print(s:"Additional molotov!");
					GiveActorInventory(who,"HandGrenadeAmmo",1);
				}
				else{
					Print(s:"You found a sniper!");
					GiveActorInventory(who,"ZISniper",1);
				}
			}
			
			else if(r1<=98){ //RARE
				if(r2<=25){
					Print(s:"10% damage for 30 secondes!");
					GiveActorInventory(who,"DamageBonus",1);
				}
				else if(r2<=50){
					Print(s:"You found a SSG!");
					GiveActorInventory(who,"ZISuperShotGun",1);
				}
				else if(r2<=75){
					Print(s:"You found a Rocket Launcher!");
					GiveActorInventory(who,"ZIRocketLauncher",1);
				}
				else{
					Print(s:"Fall reduction!");
					GiveActorInventory(who,"BonusFallDamageProtection",1);
				}
			}
			else{ //EPIC
				//Well for  the moment, there is no epic bonus unless  the elite one, so i implement it right there
				PrintBold(n:who-PLAYER_ID+1,s:" is now an ELITE!");
				GiveActorInventory(who,"LeaderSpeed",1);
				GiveActorInventory(who,"LeaderDamage",1);
			}
			break;
		case 3:
			TakeActorInventory(who,"RareBonusCrate",1);
			break;
		case 4:
			TakeActorInventory(who,"EpicBonusCrate",1);
			PrintBold(n:who-PLAYER_ID+1,s:" is now an ELITE!");
			GiveActorInventory(who,"LeaderSpeed",1);
			GiveActorInventory(who,"LeaderDamage",1);
			break;
		case 5:
			TakeActorInventory(who,"ZombieBonusCrate",1);
			//If he is human??? idk how, but why not.
			if(CheckActorInventory(who,"IsHuman")==1){
				Print(s:"A zombie was hiding in it!");
				MakeZombie(who,0,0,1);
				break;
			}
			//Zombie crate just give bonuses.
			if(r2<=20){Print(s:"You got Nothing!");
			}//Nothing
			else if(r2<=35){
				Print(s:"You got healed");
				HealthNew+=500;
				SetActorProperty(who,APROP_Health,HealthNew);
			}
			else if(r2<=50){
				Print(s:"Speed Boost!");
				GiveActorInventory(who,"BonusSpeed",1);
			}//Speed Boost
			else if(r2<=65){
				Print(s:"Jump Boost!");
				GiveActorInventory(who,"BonusHighJump",1);
			}//Jump Boost
			else if(r2<=85){
				Print(s:"Night Vision!");
				GiveActorInventory(who,"NightVision",1);
			}//Night Vision
			else if(r2<=95){
				Print(s:"Instant Spit!");
				GiveActorInventory(who,"ZombieSecondaryCoolDown",3000);
			}// Spit
			else{
				Print(s:"You are now invisible for 35 seconds!");
				GiveActorInventory(who,"BonusInvisibility",1);
			}//Invisible
			break;
		default:
			log(s:"BUG!! Please Report this bug!!");
			break;
	}
}

script 723 OPEN{
	int j;
	int x,y,z;
	int velx,vely,velz;
	while(true){
		for(int i=0;i<64;i++){
			j=i+PLAYER_ID;
			if(CheckActorInventory(j,"LeaderSpeed")==1){
				//This doesnt work, because clean/clear inventory is faster
				if(getactorproperty(j, APROP_HEALTH) <= 0)
				{
					thing_remove(500+j);
				}
				x = GetActorX(j);
				y = GetActorY(j);
				z = getActorZ(j) + getactorviewheight(j) + 18.0;
				velx = GetActorVelX(j);
				vely = GetActorVelY(j);
				velz = GetActorVelZ(j);
				
				if(ThingCount(0, 500+j) <= 0) //IF its doesnt exist, then we create it
				{
					Spawn("LeaderSign", x,y,z, 500+j,0); 
				}
				//We update the position
				// Also, this fix people wallrunning/Running into wall, getting desync with the sign
				if(setactorposition(500+j, x, y, z, 0)==true){
					setactorvelocity(500+j, velx, vely, velz, 0, 0);
				}else{
					thing_remove(500+j);
					SpawnForced("LeaderSign", x,y,z, 500+j,0); 
				}
				
				
				
			}
		}
		delay(1);
	}
} 

script 724(int DEADTID){
	incrementStats(2,DEADTID);
	if(!SetActivatorToTarget(0)){terminate;} //If we cant get the killer as activator
	int killer_TID = PlayerNumber()+PLAYER_ID;
	int randomaward = Random(0,100);
	if(CheckActorInventory(killer_TID,"IsZombie")==1){terminate;}
	if(GetGameState()>=3){terminate;} //prevent telefuck award at the end lol
	incrementStats(1,killer_TID);
	
	delay(2);
	//TODO in the future ? xd
	
	if(randomaward >=50){
		printbold(n:0,s:" was awarded a bonus crate!");
		GiveActorInventory(killer_TID,"CommonBonusCrate",1);
	}
}








