#library "Func"
#Include "zcommon.acs"
#Import "Const.acs"

int NbrOfZombieAtStart;
int TIDMotherZombie[8];
int TIDToZombify;

int ZombieNumber;
int HumanNumber;

int GameState=-1;
int MapType=-1;
int MapState;
int MapStateMax;

int IsSafePlayer[64];
int IsDead[64];
int IsAlive[64];
int ZChanceFactor[64];

int TimerSeconds;

int InfectMsgID,InfectMsgType;
int varX,varY;
int UpdateOnInfect;
int InfectedTID;
int InfectorTID;

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


/* 	Function called at the start of a map;
		Pick randomly a player or playerS depending of how many active player there are.	*/
function void ZombieStarter (void)
{
	int NbrOfPlayers=HumanCounter(); //Because the game has not yet started...
	int NbrOfZombieNeeded=1; //Number of zombie choosen for a number of Human
	int PlayerToZombify;//TID of player to zombify
	
	//New way
	while(NbrOfPlayers>=8){
		NbrOfZombieNeeded++;
		NbrOfPlayers-=8;
	}
	NbrOfZombieNeeded=Clip(1,8,NbrOfZombieNeeded);
	/* Old way
	NbrOfZombieNeeded=Clip(1,8,NbrOfPlayers/7);*/ 
	
	SetTotalZombieAtStart(NbrOfZombieNeeded);
	
	ACS_execute(704,0,NbrOfZombieNeeded);
	SetGameState(1);
	
}

/*		Function called at the VERY start of a map
		Clean the inventory and give the basic weapon and items for all players		*/
function void InventoryStarter(void){
	for(int i=0;i<64;i++){
		TakeActorInventory(i+PLAYER_ID,"IsZombie",1); 
		TakeActorInventory(i+PLAYER_ID,"IsZF",1); 
		TakeActorInventory(i+PLAYER_ID,"ZombieImmune",1);
		//Take zombies bonuses
		TakeActorInventory(i+PLAYER_ID,"BonusSpeed",1);
		TakeActorInventory(i+PLAYER_ID,"BonusHighJump",1);
		TakeActorInventory(i+PLAYER_ID,"NightVision",1);
		TakeActorInventory(i+PLAYER_ID,"BonusInvisibility",1);
		//Take Humans bonuses
		TakeActorInventory(i+PLAYER_ID,"LeaderSpeed",1);
		TakeActorInventory(i+PLAYER_ID,"LeaderDamage",1);
		TakeActorInventory(i+PLAYER_ID,"BonusFallDamageProtection",1);
		TakeActorInventory(i+PLAYER_ID,"DamageBonus",1);
		
		//
		GiveActorInventory(i+PLAYER_ID,"IsHuman",1);
		GiveActorInventory(i+PLAYER_ID,"HumanImmune",1);
		GiveActorInventory(i+PLAYER_ID,"ZIGrenade",1);
		GiveActorInventory(i+PLAYER_ID,"HandGrenadeAmmo",1);
		GiveActorInventory(i+PLAYER_ID,"ZIExpGrenade",1);
		GiveActorInventory(i+PLAYER_ID,"HandExpGrenadeAmmo",1);
		//GiveActorInventory(i+PLAYER_ID,"ZIKnife",1);
		GiveActorInventory(i+PLAYER_ID,"StaminaSprint",1200);
		GiveActorInventory(i+PLAYER_ID,"ZIPistol",1);
		GiveActorInventory(i+PLAYER_ID,"ZIShotGun",1);
		GiveActorInventory(i+PLAYER_ID,"ZIShells",7);
		GiveActorInventory(i+PLAYER_ID,"ZIRifle",1);
		GiveActorInventory(i+PLAYER_ID,"ZIRifleAmmo",30);
		SetActorProperty(i+PLAYER_ID,APROP_Invulnerable,OFF);
	}
}

function void InventoryReStarter(int player){
		SetActorProperty(player+PLAYER_ID, APROP_SPEED, 1.0);
		TakeActorInventory(player+PLAYER_ID,"BonusSpeed",1);
		TakeActorInventory(player+PLAYER_ID,"BonusHighJump",1);
		TakeActorInventory(player+PLAYER_ID,"NightVision",1);
		TakeActorInventory(player+PLAYER_ID,"BonusInvisibility",1);
		TakeActorInventory(player+PLAYER_ID,"LeaderSpeed",1);
		TakeActorInventory(player+PLAYER_ID,"LeaderDamage",1);
		TakeActorInventory(player+PLAYER_ID,"BonusFallDamageProtection",1);
		TakeActorInventory(player+PLAYER_ID,"DamageBonus",1);
		TakeActorInventory(player+PLAYER_ID,"IsZombie",1); 
		TakeActorInventory(player+PLAYER_ID,"IsZF",1); 
		TakeActorInventory(player+PLAYER_ID,"ZombieImmune",1);
		GiveActorInventory(player+PLAYER_ID,"IsHuman",1);
		GiveActorInventory(player+PLAYER_ID,"HumanImmune",1);
		GiveActorInventory(player+PLAYER_ID,"ZIGrenade",1);
		GiveActorInventory(player+PLAYER_ID,"HandGrenadeAmmo",1);
		GiveActorInventory(player+PLAYER_ID,"ZIExpGrenade",1);
		GiveActorInventory(player+PLAYER_ID,"HandExpGrenadeAmmo",1);
		//GiveActorInventory(i+PLAYER_ID,"ZIKnife",1);
		GiveActorInventory(player+PLAYER_ID,"StaminaSprint",1200);
		GiveActorInventory(player+PLAYER_ID,"ZIPistol",1);
		GiveActorInventory(player+PLAYER_ID,"ZIShotGun",1);
		GiveActorInventory(player+PLAYER_ID,"ZIShells",7);
		GiveActorInventory(player+PLAYER_ID,"ZIRifle",1);
		GiveActorInventory(player+PLAYER_ID,"ZIRifleAmmo",30);
		SetActorProperty(player+PLAYER_ID,APROP_Invulnerable,OFF);
}

// 	int who is the time to give the immunity
function void GiveZombieImmunity(int who)
{
	GiveActorInventory(who+PLAYER_ID,"TemporaryProtection",1);
}

/*		 Function called by item, switch or by an admin to infect a human
		 First: Is he MotherZombie?							
		 Infector: Infector TID	 !!! (not the playernumber)					*/
function void MakeZombie (int player,int first,int infector,int nomessage)
{
	if(player==0){
		Log(s:"Omega Error, plz");
	}
	int HealthToGive;
	int HealthInfector = GetActorProperty(infector,APROP_Health);
	
	if(HealthInfector==100){
	 HealthInfector=ZombieMinimumHealth;
	}
	
	if(first==1){
		HealthToGive=Random(MotherZombieMinHealth,MotherZombieMaxHealth);
		if(GetMapType()==2){
			HealthToGive+=Random(4000,6000);
		}
		GiveActorInventory(player,"IsMotherZombie",1);
	}else if(nomessage==1){
		HealthToGive=ZombieMinimumHealth+Random(250,2000);
	}else{
		if(infector==0){
			HealthToGive=Clip(ZombieMinimumHealth,ZombieMinimumHealth*4,(HealthInfector/2+256)+Random(500,1250));
			if(GetMapType()==2){
				HealthToGive*=2;
			}
		}else{
			HealthToGive=Clip(ZombieMinimumHealth,HealthInfector,(HealthInfector/2+256)+Random(500,1250));
		}
	}
	
	//reward zombie for infection with less cooldown for spit
	if(GetMapType()==2){
		GiveActorInventory(infector,"ZombieSecondaryCoolDown",300);
		incrementStats(3,infector);
	}
	if(GetMapType()==1){
		GiveActorInventory(player,"IsZF",1);
	}
	SetActorProperty(player,APROP_Health,HealthToGive); 
	Thing_Damage(player,1,0);
	
	if(first==0 && nomessage==0){
		acs_executeAlways(706,0,player,checkActorInventory(player,"HandGrenadeAmmo"),0);
		acs_executeAlways(706,0,player,checkActorInventory(player,"HandExpGrenadeAmmo"),1);
		if(nomessage==0){SpawnProjectile(player, "ZombieFlame", 0, 0, 0, 0, 0);
		}
	}
	
	SetActorProperty(player, APROP_SPEED, 1.15);
	
	//GlobalVarUpdater(1,ZombieCounter(),0);
	TakeActorInventory(player,"IsHuman",1);
	TakeActorInventory(player,"HumanImmune",1);
	GiveActorInventory(player,"IsZombie",1);
	GiveActorInventory(player,"ZombieImmune",1);
	GiveActorInventory(player,"ZombieHands",1);
	GiveActorInventory(player,"TPChecker",3);
	//(player,"NightVision",1); I made it a bonus item
	TakeActorInventory(player,"ZIKnife",1);
	TakeActorInventory(player,"ZIPistol",1);
	TakeActorInventory(player,"ZISuperShotgun",1);
	TakeActorInventory(player,"ZIShotGun",1);
	TakeActorInventory(player,"ZIShells",7);
	TakeActorInventory(player,"ZIRifle",1);
	TakeActorInventory(player,"ZIRifleAmmo",30);
	TakeActorInventory(player,"ZISniper",1);
	TakeActorInventory(player,"ZIRocketLauncher",1);
	TakeActorInventory(player,"LeaderSpeed",1);
	TakeActorInventory(player,"LeaderDamage",1);
	TakeActorInventory(player,"BonusFallDamageProtection",1);
	TakeActorInventory(player,"DamageBonus",1);
	
	TakeActorInventory(player,"HandGrenadeAmmo",2);
	TakeActorInventory(player,"ZIGrenade",1);
	TakeActorInventory(player,"ZIExpGrenade",1);
	TakeActorInventory(player,"HandExpGrenadeAmmo",2);
	
	thing_remove(500+player);
	
	//Effect todo
	
	SetFont("SMALLFONT");
	
	SetInfectorTID(infector);
	SetInfectedTID(player);
	SetVarX(0.5);
	SetVarY(0.225);
	
	if(first==1)
	{
		if(GetTotalZombieAtStart()>4){
		SetInfectMsgType(2);
		}else{
		SetInfectMsgType(1);
		}
	}else{
	SetInfectMsgType(3);
	}
	
	if(nomessage==1){
		SetInfectMsgType(0);
	}
	
	HumanCounter();
	ZombieCounter();
	AddUpdateOnInfect(1);
	if(nomessage==1||first==1){
		SetActivator(player);
		if(GetMapType()==1){Teleport_NoFog(ZombieTID,1,0,0);}
	}
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
						SetActivator(i+PLAYER_ID);
						Thing_Damage(i+PLAYER_ID, 99999, 19);
						SetPlayerProperty(1, 1, PROP_TOTALLYFROZEN);
					}
				}
			}
		}
		else{
			for(int i=0;i<64;i++){
				if(PlayerInGame(i)){
					SetActivator(i+PLAYER_ID);
					SetPlayerProperty(1, 1, PROP_TOTALLYFROZEN);
				}
			}
		}
	}else if(GetMapType()==2){
		SetGameState(4);
		for(int i=0;i<64;i++){
			if(PlayerInGame(i)){
				if(killfeature==1){
					SetActivator(i+PLAYER_ID);
						if(CheckActorInventory(i+PLAYER_ID,"IsZombie")){
							Thing_Damage(i+PLAYER_ID, 99999, 19);;
						}
				}
				SetPlayerProperty(1, 1, PROP_TOTALLYFROZEN);
			}
		}
	}
}


 /*
 I should make a super fonction with a par1
 So we dont have 3 redundants function for approx the same thing
 */
// Prepare future zombie after death 
// It just delete humans items
function void CleanInv (int player)
{
	TakeActorInventory(player+PLAYER_ID,"IsHuman",1); 
	TakeActorInventory(player+PLAYER_ID,"HumanImmune",1);
	TakeActorInventory(player+PLAYER_ID,"ZIKnife",1);
	TakeActorInventory(player+PLAYER_ID,"ZIPistol",1);
	TakeActorInventory(player+PLAYER_ID,"ZIShotGun",1);
	TakeActorInventory(player+PLAYER_ID,"ZIShells",7);
	TakeActorInventory(player+PLAYER_ID,"ZIRifle",1);
	TakeActorInventory(player+PLAYER_ID,"ZISuperShotgun",1);
	TakeActorInventory(player+PLAYER_ID,"ZIRifleAmmo",30);
	TakeActorInventory(player+PLAYER_ID,"ZISniper",1);
	TakeActorInventory(player+PLAYER_ID,"ZIRocketLauncher",1);
	TakeActorInventory(player+PLAYER_ID,"HandGrenadeAmmo",2);
	TakeActorInventory(player+PLAYER_ID,"ZIGrenade",1);
	TakeActorInventory(player+PLAYER_ID,"ZIExpGrenade",1);
	TakeActorInventory(player+PLAYER_ID,"HandExpGrenadeAmmo",2);
	TakeActorInventory(player+PLAYER_ID,"LeaderSpeed",1);
	TakeActorInventory(player+PLAYER_ID,"LeaderDamage",1);
	TakeActorInventory(player+PLAYER_ID,"PanicSpeed",1);
	//Take way bonuses
	TakeActorInventory(player+PLAYER_ID,"LeaderSpeed",1);
	TakeActorInventory(player+PLAYER_ID,"LeaderDamage",1);
	TakeActorInventory(player+PLAYER_ID,"BonusFallDamageProtection",1);
	TakeActorInventory(player+PLAYER_ID,"DamageBonus",1);
	


}

/*		Function called at the end of a map;
		Reset the inventory and some powerup for players	*/
function void ResetInv (void)
{
	for(int i=0;i<64;i++){
		if(playerInGame(i)){
			SetActorProperty(i+PLAYER_ID,APROP_Health,100); 
			SetActorProperty(i+PLAYER_ID, APROP_SPEED, 1);
			TakeActorInventory(i+PLAYER_ID,"IsHuman",1);
			TakeActorInventory(i+PLAYER_ID,"IsZombie",1); 
			TakeActorInventory(i+PLAYER_ID,"HumanImmune",1);
			TakeActorInventory(i+PLAYER_ID,"ZombieImmune",1);
			TakeActorInventory(i+PLAYER_ID,"ZombieSecondaryCoolDown",9999);
			TakeActorInventory(i+PLAYER_ID,"IsMotherZombie",1);
			TakeActorInventory(i+PLAYER_ID,"TPChecker",3);
			TakeActorInventory(i+PLAYER_ID,"HasPanicked",1);
			TakeActorInventory(i+PLAYER_ID,"IsSprinting",1);
			TakeActorInventory(i+PLAYER_ID,"ZombieHands",1);
			TakeActorInventory(i+PLAYER_ID,"ZIKnife",1);
			TakeActorInventory(i+PLAYER_ID,"ZIPistol",1);
			TakeActorInventory(i+PLAYER_ID,"ZIShotGun",1);
			TakeActorInventory(i+PLAYER_ID,"ZIShells",7);
			TakeActorInventory(i+PLAYER_ID,"ZIRifle",1);
			//TakeActorInventory(i+PLAYER_ID,"ZISniper",1);
			//TakeActorInventory(i+PLAYER_ID,"ZIRocketLauncher",1);
			TakeActorInventory(i+PLAYER_ID,"ZIRifleAmmo",30);
			TakeActorInventory(i+PLAYER_ID,"IsBurning",1);
			TakeActorInventory(i+PLAYER_ID,"BurningSpeed",1);
			TakeActorInventory(i+PLAYER_ID,"HandGrenadeAmmo",2);
			TakeActorInventory(i+PLAYER_ID,"ZIGrenade",1);
			TakeActorInventory(i+PLAYER_ID,"ZIExpGrenade",1);
			TakeActorInventory(i+PLAYER_ID,"HandExpGrenadeAmmo",2);
			TakeActorInventory(i+PLAYER_ID,"BurningSpeed",1);
			TakeActorInventory(i+PLAYER_ID,"StuntSpeed",1);
			TakeActorInventory(i+PLAYER_ID,"PanicSpeed",1);
			TakeActorInventory(i+PLAYER_ID,"NightVision",1);
			TakeActorInventory(i+PLAYER_ID,"BonusSpeed",1);
			TakeActorInventory(i+PLAYER_ID,"BonusHighJump",1);
			TakeActorInventory(i+PLAYER_ID,"BonusInvisibility",1);
			TakeActorInventory(i+PLAYER_ID,"LeaderSpeed",1);
			TakeActorInventory(i+PLAYER_ID,"LeaderDamage",1);
			TakeActorInventory(i+PLAYER_ID,"BonusFallDamageProtection",1);
			TakeActorInventory(i+PLAYER_ID,"DamageBonus",1);
		}
	}
}

//CLEAR Everything when joining, respawning
function void DestroyInv(int player)
{
	TakeActorInventory(player+PLAYER_ID,"IsZombie",1);
	TakeActorInventory(player+PLAYER_ID,"IsHuman",1);
	TakeActorInventory(player+PLAYER_ID,"fist",1);
	TakeActorInventory(player+PLAYER_ID,"pistol",1);
	TakeActorInventory(player+PLAYER_ID,"clip",50);
	TakeActorInventory(player+PLAYER_ID,"Shotgun",1);
	TakeActorInventory(player+PLAYER_ID,"shell",50);
	TakeActorInventory(player+PLAYER_ID,"chaingun",1);
	TakeActorInventory(player+PLAYER_ID,"rocketlauncher",1);
	TakeActorInventory(player+PLAYER_ID,"rocket",50);
	TakeActorInventory(player+PLAYER_ID,"cells",50);
	TakeActorInventory(player+PLAYER_ID,"plasmarifle",1);
	TakeActorInventory(player+PLAYER_ID,"bfg9000",1);
	TakeActorInventory(player+PLAYER_ID,"chainsaw",1);
	
	TakeActorInventory(player+PLAYER_ID,"HandGrenadeAmmo",2);
	TakeActorInventory(player+PLAYER_ID,"ZIGrenade",1);
	TakeActorInventory(player+PLAYER_ID,"ZIExpGrenade",1);
	TakeActorInventory(player+PLAYER_ID,"HandExpGrenadeAmmo",2);
	TakeActorInventory(player+PLAYER_ID,"TPChecker",3);
	TakeActorInventory(player+PLAYER_ID,"NightVision",1);
	TakeActorInventory(player+PLAYER_ID,"BonusSpeed",1);
	TakeActorInventory(player+PLAYER_ID,"BonusHighJump",1);
	TakeActorInventory(player+PLAYER_ID,"BonusInvisibility",1);
}


/*	 Function called by item, switch or by an admin to cure a zombie into a human back	
function void CureZombie (int player){
	TakeActorInventory(player,"IsZombie",1);
	GiveActorInventory(player,"IsHuman",1);
	TakeActorInventory(player,"TPChecker",3);
	TakeActorInventory(player,"NightVision",1);
}
*/


/* USELESS
function void GlobalVarUpdater(int a,int b,int c){
	Acs_executeAlways(708,0,a,b,c);
}
//Surely called when someone died 
function void UpdateCounters(void){
	GlobalVarUpdater(1,ZombieCounter(),0);
	GlobalVarUpdater(2,HumanCounter(),0);
	log(s:"UpdateCounters");
}
*/
function void PlayGlobalSound(void){
}








//CS


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

function int GetHumanNumber(void){
	return  HumanNumber;
}

function int GetZombieNumber(void){
	return ZombieNumber;
}

function int GetDeathState(int a){
	return IsDead[a];
}
function void SetDeathState(int a){
	IsDead[a] = 1;
	IsAlive[a] = 0;
}

function int GetAliveState(int a){
	return IsAlive[a];
}
function void SetAliveState(int a){
	IsAlive[a] = 1;
	IsDead[a] = 0;
}

function int GetUpdateOnInfect(void){
	return UpdateOnInfect;
}
function void SetUpdateOnInfect(int a){
	UpdateOnInfect=a;
}
function void AddUpdateOnInfect(int a){
	UpdateOnInfect+=a;
}
function void RemoveUpdateOnInfect(int a){
	UpdateOnInfect-=a;
}

function void SetTotalZombieAtStart(int a)
{
	NbrOfZombieAtStart=a;
}
function int GetTotalZombieAtStart(void)
{
	return NbrOfZombieAtStart;
}

function int GetInfectedTID(void){
	return InfectedTID;
}
function void SetInfectedTID(int TID){
	InfectedTID=TID;
}

function int GetInfectorTID(void){
	return InfectorTID;
}
function void SetInfectorTID(int TID){
	InfectorTID=TID;
}

function int GetVarX(void){
	return varX;
}
function int GetVarY(void){
	return varY;
}

function void SetVarX(int a){
	varX=a;
}
function void SetVarY(int a){
	varY=a;
}

function int GetInfectMsgID(void){
	return InfectMsgID;
}
function void SetInfectMsgID(int a){
	InfectMsgID=a;
}

function int GetInfectMsgType(void){
	return InfectMsgType;
}
function void SetInfectMsgType(int a){
	InfectMsgType=a;
}

function void addSecondsToTimer(int a){
		TimerSeconds+=a;
}

function void SetMinuteToTimer(int a){
	int b=(a*60);
	TimerSeconds=b;
}

function int GetTimerSeconds(void){
	return TimerSeconds;
}

function void SetMapType(int a){
	
	MapType=a;
}
function int GetMapType(void){
	return MapType;
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
function int GetPlayerTID(int player){
	return PLAYER_ID+player;
}

function int GetPlayerTID2(void){
	return PLAYER_ID+PlayerNumber();
}

/** Surely activated  by a script 
	-1= Start Script not called				NOGAME
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

function void AddMapState(int a){
	MapState+=a;
}

function void PutTidToZombify(int a){
	TIDToZombify=a;
}

function int GetTidToZombify(void){
	return TIDToZombify;
}

function int GetZombifyChanceFactor(int who){
	//return ZChanceFactor[who];
	return GetCVar(StrParam(s:"p",d:who));
}

function void SetZombifyChanceFactor(int who, int var){
	//ZChanceFactor[who] = var;
	SetCvar(StrParam(s:"p",d:who),var);

}
function void AddZombifyChanceFactor(int who){
	//ZChanceFactor[who]+=1;
	SetCvar(StrParam(s:"p",d:who),GetZombifyChanceFactor(who)+1);

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
// AM I DUMB ? 
function int floor(int fixedNumber)
{
	return fixedNumber & 0xFFFF0000;
}

function int ceiling(int num)
{
	return num+1; //LOL i guess
	
}

function int abs(int a)
{
	if(a<0)return -a;
	return a;
}


function void AwardPlayer(int who){
	if(who<PLAYER_ID){
		who+=PLAYER_ID;
	}
	if(CheckActorInventory(who,"IsZombie")==1){
		//GiveActorInventory(who,"ZombieBonusCrate",1);
		GiveActorInventory(who,"CommonBonusCrate",1);
	}
	else{
		int RobertIsBack=who+Random(1,48);
		if((RobertIsBack%3)>2){
			GiveActorInventory(who,"UncommonBonusCrate",1);
		}
		else{
			GiveActorInventory(who,"CommonBonusCrate",1);
		}
	}
}

 
// For stats
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
	switch(what){
		case 1:
			return nbrZMKilled[who];
		case 2:
			return nbrZMDeath[who];
		case 3:
			return nbrInfection[who];
		default:
			LOG(s:"BUG!! Please report this! Error code:STATSGET");
			return -1;
	}
}

//Flush everything
function void resetStats(void){
	for(int i=0;i<64;i++){
		nbrZMKilled[i] =0;
		nbrZMDeath[i] =0;
		nbrInfection[i] =0;
	}
}





