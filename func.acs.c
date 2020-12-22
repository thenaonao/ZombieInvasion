#library "Func"
#Include "zcommon.acs"
#Import "Const.acs"

int NbrOfZombieAtStart;
int TIDMotherZombie[8];
int TIDToZombify;

int ZombieNumber;
int HumanNumber;

int GameState=-2;
int MapType=-1;
int MapState;
int MapStateMax;

int IsSafePlayer[64];
int IsDead[64];
int IsAlive[64];
int ZChanceFactor[64];

int TimerSeconds;

//For stats (future)
int nbrZMKilled[64]; //Stores a value for each player , counts zm kills
int nbrZMDeath[64]; //How many times the same zombies died ? XD
int nbrInfection[64]; //How many infect by 1 person xD vvhat else to say ?

function int ZombieCounter (void){ 
	int number; 
	for(int i=0;i<64;i++){
		if(PlayerInGame(i) && CheckActorInventory(PLAYER_ID+i, "IsZombie")&& GetAliveState(i)==1){
			number++;
		}
	}
	ZombieNumber = number;
	return ZombieNumber;
} 
function int HumanCounter (void){
	int number;
	for(int i=0;i<64;i++){
		if(PlayerInGame(i) && CheckActorInventory(PLAYER_ID+i, "IsHuman")){
			number++;
		}
	}
	HumanNumber=number;
	return HumanNumber;
} 

function int PlayerCounter(void){
	int number;
	for(int i=0;i<64;i++){
		if(PlayerInGame(i)){
			number++;
		}
	}
	return number;
}




// 	int who is the time to give the immunity
function void GiveZombieImmunity(int who)
{
	GiveActorInventory(who+PLAYER_ID,"TemporaryProtection",1);
}



function void StarterMap(int typeV0, int MapStateV1,int CountdownV2, int ZimmunityV3)
{
	if(typeV0==1 || typeV0==2){
	SetMapStateMaximum(MapStateV1); //usless for ZI type map
	ACS_Execute(700,0,typeV0,CountdownV2,ZimmunityV3);
	}
	else{Print(s:"Error while starting the map... incorrect type id");
	}
}

function void FinishMap(void){
	SetMapState(GetMapStateMaximum());
}


function void EndMap(int killfeature)
{
	int tester=0;
	//IF ESCAPE
	if(GetMapType()==1){
		SetGameState(3);
		if(killfeature==1){
			for(int i=0;i<64;i++){
				if(PlayerInGame(i)){
					if(GetSafePlayer(i)==0){
						//SetActivator(i+PLAYER_ID);
						Thing_Damage(i+PLAYER_ID, 99999, 19);
						SetPlayerProperty(1, 1, PROP_TOTALLYFROZEN);
					}
				}
			}
		}
		else{
			for(int j=0;j<64;j++){
				if(PlayerInGame(j)){
					//SetActivator(j+PLAYER_ID);
					SetPlayerProperty(1, 1, PROP_TOTALLYFROZEN);
				}
			}
		}
	}else if(GetMapType()==2){
		SetGameState(4);
		for(int k=0;k<64;k++){
			if(PlayerInGame(k)){
				if(killfeature==1){
					//SetActivator(k+PLAYER_ID);
						if(CheckActorInventory(k+PLAYER_ID,"IsZombie")){
							Thing_Damage(k+PLAYER_ID, 99999, 19);;
						}
				}
				SetPlayerProperty(1, 1, PROP_TOTALLYFROZEN);
			}
		}
	}
}

function void InfectMessage(int x,int y,int id,int infected,int type,int infector){
	SetFont("SMALLFONT");
	infected-=(PLAYER_ID-1);
	infector-=(PLAYER_ID-1);
	
	if(type==1){	
		HudMessageBold(s:"\cJ",n:infected,s:" \cGgot infected by a \cMVirus!";HUDMSG_FADEOUT,350+id,CR_UNTRANSLATED,x,y,3.0,1.0);
	}else if(type==2){
		HudMessageBold(s:"\cGA bunch of \cMZombie \cGappeared!";HUDMSG_FADEOUT,355,CR_UNTRANSLATED,x,y,3.0,1.0);
	}
	else if(type==3){
		HudMessageBold(s:"\cM",n:infector,s:" \cGHas infected \cJ ",n:infected;HUDMSG_FADEOUT,356+id,CR_UNTRANSLATED,x,y,3.0,1.0);
	}
	else if(type==4){
		HudMessageBold(s:"\cJ",n:infected,s:" \cGGot infected mysteriously...";HUDMSG_FADEOUT,300+id,CR_UNTRANSLATED,x,y,3.0,1.0);
	}else{
	
	}
}
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
/* Functions Getters/Setters 		*/


//*****

function void SetDeathState(int a){
	IsAlive[a] = 0;
}
function int GetAliveState(int a){
	return IsAlive[a];
}
function void SetAliveState(int a){
	IsAlive[a] = 1;
}






function int GetZombieMotherTID(int a){
	Clip(1,8,a);
	return TIDMOTHERZOMBIE[a];
}

function void SetZombieMotherTID(int a, int b){
	Clip(1,8,a);
	TIDMOTHERZOMBIE[a] = b;
}
// int a = WHO -> TID, b is value
function void SetSafePlayer(int a,int b){
	a-=PLAYER_ID;
	IsSafePlayer[a] = b;
}

function int GetSafePlayer(int a){
	return IsSafePlayer[a];
}

/* Surely activated by an ENTER script; to give every player his own TID	*/
function void SetPlayerTID(int player){
	Thing_ChangeTID(0, PLAYER_ID+player);
}

//par player is PlayerNumber(); but tbh its quite useless
//Use GetPlayerTID2() if you are reading this.
function int GetPlayerTID(int player){
	return PLAYER_ID+player;
}

function int GetPlayerTID2(void){
	return PLAYER_ID+PlayerNumber();
}
















function void GiveStamina(void)
{
	for(int i=0;i<64;i++){
		if(CheckActorInventory(i+PLAYER_ID,"IsHuman")== 1 && CheckActorInventory(i+PLAYER_ID,"HasPanicked")== 0){
			GiveActorInventory(i+PLAYER_ID,"StaminaSprint",4);
		}
	}
}
 



//--------------------------------------------------------------------------------------------------------------------------------
/* Functions for Maps*/



function void DisplayResist(str s1,int time,str s2,int id){
	if(id>=0 && id<99){
	SetFont("BIGFONT");
	id*=0.1;
	hudmessagebold(s:"\cG",s:s1,s:" \cU",d:time,s:" \cG",s:s2;HUDMSG_FADEOUT,200+id,CR_GRAY,0.5,0.1+id,0.5);
	}
}

function void BreakGlass(int SectorID){
	Acs_executeAlways(713,0,SectorID,0,0);
}

function void ZombieRepulsor(int dir, int force){
	Acs_executeAlways(712,0,dir,force,0);
}


//--------------------------------------------------------------------------------------------------------------------------------
/* Math Functions */  
// int a & int b are the borders ||| a < b !!
// int c is the number to clip 
function int Clip(int a,int b, int c)
{
	if(a>b){return -1;}
	if(c<a){return a;}
	if(b<c){return b;}
	return c;
}

function int ClipRandom(int a,int b,int c){
	if(a>b){return Random(a,b);}
	if(c<a){return a;}
	if(b<c){return b;}
	return c;
}

function int floor(int fixedNumber)
{
	return fixedNumber & 0xFFFF0000;
}

function int ceiling(int num)
{
	return num+1;
	
}

function int abs(int a)
{
	if(a<0)return -a;
	return a;
}


function void AwardPlayer(int who){
	if(who<PLAYER_ID){
		who+=PLAYER_ID; //Auto convert if we get a playernumber instead of TID
	}
	int CrateRandom = (random(0, 400)*0.25)>>16;
	
	if(CrateRandom>=50){ //Common
		GiveActorInventory(who,"CommonBonusCrate",1);
	}else if(CrateRandom>=65){//Weapon
		GiveActorInventory(who,"WeaponBonusCrate",1);
	}else if(CrateRandom>=80){//Support
		GiveActorInventory(who,"SupportBonusCrate",1);
	}else if(CrateRandom>=95){
		GiveActorInventory(who,"RareBonusCrate",1);
	}else{
		GiveActorInventory(who,"UltimateBonusCrate",1);
	}
}

 
//---------------------------------For stats----------------------------------//
function void setStats(int what, int who,int value){
	if(who>=PLAYER_ID){
		who-=PLAYER_ID;
	}
	switch(what){
		case 1:
			nbrZMKilled[who]=value;
			break;
		case 2:
			nbrZMDeath[who]=value;
			break;
		case 3:
			nbrInfection[who]=value;
			break;
		default:
			LOG(s:"BUG!! Please report this! Error code: STATSSET");
			break;
	}
}

function void incrementStats(int what, int who){
	switch(what){
		case 1:
			nbrZMKilled[who]++;
			break;
		case 2:
			nbrZMDeath[who]++;
			break;
		case 3:
			nbrInfection[who]++;
			break;
		default:
			LOG(s:"BUG!! Please report this! Error code: STATSINC");
			break;
	}
}
function int getStats(int what,int who){
	int ret=-1; //because ACC is dumb
	switch(what){
		case 1:
			ret = nbrZMKilled[who];
			break;
		case 2:
			ret = nbrZMDeath[who];
			break;
		case 3:
			ret = nbrInfection[who];
			break;
	}
	return ret;
}

//Flush everything
function void resetStats(void){
	for(int i=0;i<64;i++){
		nbrZMKilled[i] =0;
		nbrZMDeath[i] =0;
		nbrInfection[i] =0;
	}
}



//////////////// Start of the Redo
////////////////
////////////////
int ITimeBeforePickingAZombie;
function void SetTimePickingZombie(int a){
	ITimeBeforePickingAZombie=a;
}

function void DecrementTimePickingZombie(void){
	ITimeBeforePickingAZombie--;
}

function int GetTimePickingZombie(void){
	return ITimeBeforePickingAZombie;
}
//Clean Everything \Keep the bonuses
function void cleanInventory(int TypeOfWork,int player){
	if(TypeOfWork==0){
		for(int i=0;i<64;i++){
			//Human Stuff
			TakeActorInventory(i+PLAYER_ID,"IsHuman",1); 
			TakeActorInventory(i+PLAYER_ID,"HumanImmune",1);
			TakeActorInventory(i+PLAYER_ID,"HasPanicked",1);
			TakeActorInventory(i+PLAYER_ID,"PanicSpeed",1);
			TakeActorInventory(i+PLAYER_ID,"IsSprinting",1);
			TakeActorInventory(i+PLAYER_ID,"ZIPistol",1);
			TakeActorInventory(i+PLAYER_ID,"ZIShotGun",1);
			TakeActorInventory(i+PLAYER_ID,"ZIShells",7);
			TakeActorInventory(i+PLAYER_ID,"ZIRifle",1);
			TakeActorInventory(i+PLAYER_ID,"ZIRifleAmmo",35);
			TakeActorInventory(i+PLAYER_ID,"ZIGrenade",1);
			TakeActorInventory(i+PLAYER_ID,"ZIExpGrenade",1);
			//TakeActorInventory(i+PLAYER_ID,"HandGrenadeAmmo",2);
			//TakeActorInventory(i+PLAYER_ID,"HandExpGrenadeAmmo",2);	
			
			//Zombie Stuff
			TakeActorInventory(i+PLAYER_ID,"IsZombie",1); 
			TakeActorInventory(i+PLAYER_ID,"ZombieImmune",1);
			TakeActorInventory(i+PLAYER_ID,"ZombieSecondaryCoolDown",9999);
			TakeActorInventory(i+PLAYER_ID,"IsMotherZombie",1);
			TakeActorInventory(i+PLAYER_ID,"IsZF",1); 
			TakeActorInventory(i+PLAYER_ID,"ZombieHands",1);
			TakeActorInventory(i+PLAYER_ID,"TPChecker",3);
			TakeActorInventory(i+PLAYER_ID,"IsBurning",1);
			TakeActorInventory(i+PLAYER_ID,"BurningSpeed",1);
			TakeActorInventory(i+PLAYER_ID,"StuntSpeed",1);
			
			//Human Bonuses
			TakeActorInventory(i+PLAYER_ID,"LeaderSpeed",1);
			TakeActorInventory(i+PLAYER_ID,"LeaderDamage",1);
			TakeActorInventory(i+PLAYER_ID,"LeaderSpeed",1);
			TakeActorInventory(i+PLAYER_ID,"LeaderDamage",1);
			TakeActorInventory(i+PLAYER_ID,"BonusFallDamageProtection",1);
			TakeActorInventory(i+PLAYER_ID,"DamageBonus",1);
			
			//Zombie Bonuses
			TakeActorInventory(i+PLAYER_ID,"NightVision",1);
			TakeActorInventory(i+PLAYER_ID,"BonusHighJump",1);
			TakeActorInventory(i+PLAYER_ID,"BonusInvisibility",1);
			TakeActorInventory(i+PLAYER_ID,"BonusSpeed",1);
			//Put back normal stats for players
			SetActorProperty(i+PLAYER_ID, APROP_SPEED, 1.0);		
			SetActorProperty(i+PLAYER_ID,APROP_Health,100); 
			SetActorProperty(i+PLAYER_ID,APROP_Invulnerable,OFF);
		}
	}else{
			//Human Stuff
			TakeActorInventory(player+PLAYER_ID,"IsHuman",1); 
			TakeActorInventory(player+PLAYER_ID,"HumanImmune",1);
			TakeActorInventory(player+PLAYER_ID,"HasPanicked",1);
			TakeActorInventory(player+PLAYER_ID,"PanicSpeed",1);
			TakeActorInventory(player+PLAYER_ID,"IsSprinting",1);
			TakeActorInventory(player+PLAYER_ID,"ZIPistol",1);
			TakeActorInventory(player+PLAYER_ID,"ZIShotGun",1);
			TakeActorInventory(player+PLAYER_ID,"ZIShells",7);
			TakeActorInventory(player+PLAYER_ID,"ZIRifle",1);
			TakeActorInventory(player+PLAYER_ID,"ZIRifleAmmo",35);
			TakeActorInventory(player+PLAYER_ID,"ZIGrenade",1);
			TakeActorInventory(player+PLAYER_ID,"ZIExpGrenade",1);
			//TakeActorInventory(i+PLAYER_ID,"HandGrenadeAmmo",2);
			//TakeActorInventory(i+PLAYER_ID,"HandExpGrenadeAmmo",2);	
			
			//Zombie Stuff
			TakeActorInventory(player+PLAYER_ID,"IsZombie",1); 
			TakeActorInventory(player+PLAYER_ID,"ZombieImmune",1);
			TakeActorInventory(player+PLAYER_ID,"ZombieSecondaryCoolDown",9999);
			TakeActorInventory(player+PLAYER_ID,"IsMotherZombie",1);
			TakeActorInventory(player+PLAYER_ID,"IsZF",1); 
			TakeActorInventory(player+PLAYER_ID,"ZombieHands",1);
			TakeActorInventory(player+PLAYER_ID,"TPChecker",3);
			TakeActorInventory(player+PLAYER_ID,"IsBurning",1);
			TakeActorInventory(player+PLAYER_ID,"BurningSpeed",1);
			TakeActorInventory(player+PLAYER_ID,"StuntSpeed",1);
			
			//Human Bonuses
			TakeActorInventory(player+PLAYER_ID,"LeaderSpeed",1);
			TakeActorInventory(player+PLAYER_ID,"LeaderDamage",1);
			TakeActorInventory(player+PLAYER_ID,"LeaderSpeed",1);
			TakeActorInventory(player+PLAYER_ID,"LeaderDamage",1);
			TakeActorInventory(player+PLAYER_ID,"BonusFallDamageProtection",1);
			TakeActorInventory(player+PLAYER_ID,"DamageBonus",1);
			
			//Zombie Bonuses
			TakeActorInventory(player+PLAYER_ID,"NightVision",1);
			TakeActorInventory(player+PLAYER_ID,"BonusHighJump",1);
			TakeActorInventory(player+PLAYER_ID,"BonusInvisibility",1);
			TakeActorInventory(player+PLAYER_ID,"BonusSpeed",1);
			//Put back normal stats for players
			SetActorProperty(player+PLAYER_ID, APROP_SPEED, 1.0);		
			SetActorProperty(player+PLAYER_ID,APROP_Health,100); 
			SetActorProperty(player+PLAYER_ID,APROP_Invulnerable,OFF);
	}
}

function void inventoryStarter(int TypeOfWork,int player){
	if(TypeOfWork==0){
		for(int i=0;i<64;i++){
			GiveActorInventory(i+PLAYER_ID,"IsHuman",1);
			GiveActorInventory(i+PLAYER_ID,"HumanImmune",1);
			GiveActorInventory(i+PLAYER_ID,"ZIGrenade",1);
			GiveActorInventory(i+PLAYER_ID,"HandGrenadeAmmo",1);
			GiveActorInventory(i+PLAYER_ID,"ZIExpGrenade",1);
			GiveActorInventory(i+PLAYER_ID,"HandExpGrenadeAmmo",1);
			GiveActorInventory(i+PLAYER_ID,"StaminaSprint",1200);
			GiveActorInventory(i+PLAYER_ID,"ZIPistol",1);
			GiveActorInventory(i+PLAYER_ID,"ZIShotGun",1);
			GiveActorInventory(i+PLAYER_ID,"ZIShells",7);
			GiveActorInventory(i+PLAYER_ID,"ZIRifle",1);
			GiveActorInventory(i+PLAYER_ID,"ZIRifleAmmo",35);
		}
	}else{
			GiveActorInventory(player+PLAYER_ID,"IsHuman",1);
			GiveActorInventory(player+PLAYER_ID,"HumanImmune",1);
			GiveActorInventory(player+PLAYER_ID,"ZIGrenade",1);
			GiveActorInventory(player+PLAYER_ID,"HandGrenadeAmmo",1);
			GiveActorInventory(player+PLAYER_ID,"ZIExpGrenade",1);
			GiveActorInventory(player+PLAYER_ID,"HandExpGrenadeAmmo",1);
			GiveActorInventory(player+PLAYER_ID,"StaminaSprint",1200);
			GiveActorInventory(player+PLAYER_ID,"ZIPistol",1);
			GiveActorInventory(player+PLAYER_ID,"ZIShotGun",1);
			GiveActorInventory(player+PLAYER_ID,"ZIShells",7);
			GiveActorInventory(player+PLAYER_ID,"ZIRifle",1);
			GiveActorInventory(player+PLAYER_ID,"ZIRifleAmmo",35);
	}
}
//--- MAP TYPES || 1 == ZF ||2 == ZR
function void SetMapType(int a){
	
	MapType=a;
}
function int GetMapType(void){
	return MapType;
}
//\\
//---
/** Surely activated  by a script 
	-2=Start Script not called				NOGAME
	-1=Not enough player					NOPLAYER
	0=Not started							WAITING
	1=Zombie Choosen (Start of the game)	STARTING
	2=End of Temporary Immunity if set 		PLAYING
	3=End of the map for Escape				ENDING
	4=Game Ended							END
*/
function void SetGameState(int a)
{
	GameState=a;
}
function int GetGameState(void)
{
	return GameState;
}
//\\

//---Map States, Or checkpoints
function void SetMapState(int a){
	MapState=a;
}

function int GetMapState(void){
	return MapState;
}

function int GetMapStateMaximum(void){
	return MapStateMax;
}
function void SetMapStateMaximum(int a){
	MapStateMax =a;
}

function void IncrementMapState(void){
	MapState++;
}
//\\
//--- Zombie Chance Factor
function int GetZombifyChanceFactor(int who){
	return GetCVar(StrParam(s:"p",d:who));
}
function void SetZombifyChanceFactor(int who, int var){
	SetCvar(StrParam(s:"p",d:who),var);
}
function void AddZombifyChanceFactor(int who){
	SetCvar(StrParam(s:"p",d:who),GetZombifyChanceFactor(who)+1);
}
//\\
//--- Number of ppl to Zombify
function void SetTotalZombieAtStart(int a)
{NbrOfZombieAtStart=a;}
function int GetTotalZombieAtStart(void)
{return NbrOfZombieAtStart;}
//\\
//--- For counters
function int GetHumanNumber(void){
	return  HumanNumber;
}

function int GetZombieNumber(void){
	return ZombieNumber;
}

//***** Specially for client side, else server use the counter to update those values
function void SetHumanNumber(int a){
	HumanNumber=a;
}
function void SetZombieNumber(int a){
	ZombieNumber=a;
}
//\\
//--- For Timer
function void addSecondsToTimer(int a){
	TimerSeconds+=a;
}
function void setSecondsToTimer(int a){
	TimerSeconds=a;
}
function void SetMinuteToTimer(int a){
	TimerSeconds=a*60;
}
function int GetTimerSeconds(void){
	return TimerSeconds;
}
function void DecrementTimer(void){
	TimerSeconds--;
}
//\\







/* 	Function called at the start of a map;
		Pick randomly a player or playerS depending of how many active player there are.	*/
function void ZombieStarter (void)
{
	int NbrOfPlayers=HumanCounter(); //Because the game has not yet started...
	int NbrOfZombieNeeded=1; //Number of zombie choosen for a number of Human
	int PlayerToZombify;//TID of player to zombify
	
	while(NbrOfPlayers>=12){
		NbrOfZombieNeeded++;
		NbrOfPlayers-=12;
	}	
	SetTotalZombieAtStart(Clip(1,8,NbrOfZombieNeeded));
	
	ACS_execute(705,0,NbrOfZombieNeeded);	
}

/*		 Function called by item, switch or by an admin to infect a human
		 First: Is he MotherZombie?							
		 Infector: Infector TID	 !!! (not the playernumber)					*/
int incrementator=0;
function void MakeZombie (int player,int first,int infector,int nomessage)
{
	if(player==0){
		printBold(s:"BUG! Please Report MKZ=0");
	}else{
		int HealthToGive;
		int HealthInfector = GetActorProperty(infector,APROP_Health);
		
		if(HealthInfector<ZombieMinimumHealth){
		 HealthInfector=ZombieMinimumHealth;
		}
		
		if(first==1){
			HealthToGive=Random(MotherZombieMinHealth,MotherZombieMaxHealth);
			GiveActorInventory(player,"IsMotherZombie",1);
		}else if(nomessage==1){
			HealthToGive=ZombieMinimumHealth+Random(0,1000);
		}else{
			if(infector==0){
				HealthToGive=Clip(ZombieMinimumHealth,ZombieMaximumHealth, ((MotherZombieMinHealth+ZombieMinimumHealth)/2)+random(-500,500));
			}else{
				HealthToGive=Clip(ZombieMinimumHealth,ZombieMaximumHealth, ((HealthInfector+ZombieMaximumHealth)/3)+random(-800,800));
			}
		}
		
		//SetActorProperty(player,APROP_Health,500); 
		ACS_ExecuteAlways(725,0,player,HealthToGive); 
		
		if(first==0 && nomessage==0){
			acs_executeAlways(706,0,player,checkActorInventory(player,"HandGrenadeAmmo"),0);
			acs_executeAlways(706,0,player,checkActorInventory(player,"HandExpGrenadeAmmo"),1);
			if(nomessage==0){SpawnProjectile(player, "ZombieFlame", 0, 0, 0, 0, 0);
			}
		}
		cleanInventory(1,player-PLAYER_ID);
		TakeActorInventory(player,"ZISuperShotgun",1);
		TakeActorInventory(player,"ZISniper",1);
		TakeActorInventory(player,"ZIRocketLauncher",1);
		TakeActorInventory(player,"HandGrenadeAmmo",2);
		TakeActorInventory(player,"HandExpGrenadeAmmo",2);
		thing_remove(500+player); //Remove Leader symbol
		
		SetActorProperty(player, APROP_SPEED, 1.10);
		GiveActorInventory(player,"IsZombie",1);
		GiveActorInventory(player,"ZombieImmune",1);
		GiveActorInventory(player,"ZombieHands",1);
		GiveActorInventory(player,"TPChecker",3);
		//reward zombie for infection with less cooldown for spit
		if(GetMapType()==2){
			GiveActorInventory(infector,"ZombieSecondaryCoolDown",300);
			incrementStats(3,infector);
		}
		if(GetMapType()==1){
			GiveActorInventory(player,"IsZF",1);
		}
		
		
		
		//Effect todo
		HumanCounter();//So we update those value server side, so we can send it after to the client via 903
		ZombieCounter();
		SetFont("SMALLFONT");
		
		int type;
		if(first==1)
		{
			if(GetTotalZombieAtStart()>4){
				type=2;
			}else{
				type=1;
			}
		}else{
			type=3;
		}
		
		if(nomessage==1){
			type=0;
		}
		if(nomessage==1||first==1){
			SetActivator(player);
			if(GetMapType()==1){Teleport_NoFog(ZombieTID,1,0,0);}
			SetActivator(-1);//go back to server
		}
		Acs_executeAlways(900,0,7,GetHumanNumber());//We send them directly to the client, no need to reupdate it server-wise
		Acs_executeAlways(900,0,8,GetZombieNumber());
		
		//Display The infect message
		int y = 0.525+(0.03*incrementator);
		int compressedTID;//contain both TID
		compressedTID=(infector-PLAYER_ID);
		compressedTID=compressedTID<<8;
		compressedTID+=player-PLAYER_ID;
		
		ACS_ExecuteAlways(802,0,compressedTID,y,type);
		incrementator++;
		if(incrementator>=6){
			incrementator=0;
		}
	}
}
