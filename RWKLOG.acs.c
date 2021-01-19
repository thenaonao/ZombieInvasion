#library "RwkLog"
#Include "zcommon.acs"
#Import "Const.acs"
#Import "Func.acs"

script 700 (int type,int SecureTime,int ZombieImmuneTime)
{
	int IRestarterTimer=0;
	Acs_ExecuteAlways(903,0,1,SecureTime);//Send/Update Time Before Picking A Zombie
	SetMapState(0);
	Acs_ExecuteAlways(903,0,3,-1);// -1 to GameState
	Acs_ExecuteAlways(903,0,12,0);//Make sure  the timer doesnt countdown
	//---------------------Check if the header is correct---------------------//
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
	////////////////////////////////////////////////////////////////////////////	
	ACS_Execute(800,0);//We start drawing the hud
	SetPlayerProperty(TRUE, OFF, PROP_TOTALLYFROZEN);
	SetMapType(type);
	
	delay(15); //This delay is very important, because the script is executed too fast after a reset, so it fucks up

	//------------------Let's wait for at least 2 players---------------------//
	while(true){
		delay(10);
		if(PlayerCounter()<2){
			if(GetGameState()==0){
				Acs_ExecuteAlways(903,0,3,-1);//Set the Game state to -1
			}
			Acs_ExecuteAlways(903,0,1,SecureTime);
			if(PlayerCounter()==1){
				IRestarterTimer++;
			}
		}else{
			if(IRestarterTimer>=10){
				printbold(s:"Restarting the round...!");
				delay(35*5);
				ResetMap();
				terminate;
			}	
			IRestarterTimer=0;//If there are more than 2 players, we can reset this var.
			if(GetTimePickingZombie()>0){	
				Acs_ExecuteAlways(903,0,3,0); //Set Game State to 0
				SetFont("SMALLFONT");
				Acs_ExecuteAlways(903,0,2);// Decrement Time picking before a zombie
			}else{
				break;
			}
		}		
		delay(25);
	}
	//[DEPRECATED]
	//ACS_Execute(711,0);//For Infect Message, Maybe I will redo this code But for the moment it works
	//If its ZR, we send the timer data to the clients (and server)
	if(type==2){
		ACS_Execute(903,0,6,ZombieImmuneTime);
	}
	//For each player, we add 1 to the chance of being choosen
	for(int k = 0; k<64; k++) {
		if(PlayerInGame(k)){
			AddZombifyChanceFactor(k);	
		}
	}
	delay(5);
	Acs_ExecuteAlways(903,0,3,1);
	ZombieStarter();//After this, normally, we have choosen our zombies
	delay(5);//Escape weird priority ass script vs func acs
	
	Acs_ExecuteAlways(903,0,3,2);

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
	Acs_ExecuteAlways(903,0,12,1);
	while(GetGameState()!=4){
		GiveStamina();
		timerSec = GetTimerSeconds();
		currentMapState = GetMapState();
		nbrHum = HumanCounter();
		nbrZom = ZombieCounter();

		if(currentMapType==1){ //ZF
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
				Acs_ExecuteAlways(903,0,3,4);
				//printbold(s:"Zombie Victory");//In this case, If the round is running, but all humans rq or left
				//We need to send to the Clients to display
				//setWinState(2);
				Acs_ExecuteAlways(903,0,11,2);
				EndMap(0);
				break;
			}
			if(currentMapState==maxMapState){
				EndMap(1);
				break;
			}
			if(nbrZom==0){
				if(zmNbrTimer>8){
					//PrintBold(s:"Human Victory, Zombie RQ!!");
					//We need to send to the Clients to display
					//setWinState(1);
					Acs_ExecuteAlways(903,0,11,1);
					EndMap(0);
					break;
				}
				zmNbrTimer++;
			}else{zmNbrTimer=0;}
		}
		if(currentMapType==2){ //ZR
			if(nbrHum==0){
				//printbold(s:"Zombie Victory");
				//We need to send to the Clients to display
				//setWinState(2);
				Acs_ExecuteAlways(903,0,11,2);
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
					//PrintBold(s:"Human Victory");
					//We need to send to the Clients to display
					//setWinState(1);
					Acs_ExecuteAlways(903,0,11,1);
					EndMap(0);
					break;
				}
				zmNbrTimer++;
			}else{zmNbrTimer=0;}
			if(timerSec==0){
				//setWinState(1);
				Acs_ExecuteAlways(903,0,11,1);
				EndMap(1);
				//kill zm maybe ?
				//printbold(s:"Human Victory");
				//We need to send to the Clients to display
				break;
			}else{
				//ACS_ExecuteAlways(903,0,9);//Decrement the timer
				//Replaced by (903,0,12);
				//It saves bandwidth as we parse only once a boolean
				//Instad of parsing each seconds a new seconds to display
				
				//So we just need to decrement it only server side, as its now
				//async
				DecrementTimer();
			}
			if(timerSec%60==0 && ConsolePlayerNumber() == -1){
				ACS_ExecuteAlways(900,0,10,GetTimerSeconds());//Sends to the client the actual timer if there's a desync or whatever
				//ACS_ExecuteAlways(900,0,7,GetHumanNumber());
				//ACS_ExecuteAlways(900,0,8,GetZombieNumber());
			}
		}
		delay(35);
	} 
	
	
	//So we are obligated to do that here, and not in the loop, because, if everyone died, lets say from the end, it will simply
	//trigger a human/zombie win, but actually, everybody died, also GameState=4 so everyone is freezed, so its better to do that now
	//after we exited while block
	if(currentMapType==1){
		while(ZombieCounter()!=0 && HumanCounter()!=0){
			delay(5);
		}
		if(ZombieCounter()==0 && HumanCounter()==0){
			/* Do nothing
			Printbold(s:"uh? Everyone died ?????");
			setWinState(-1);
			delay(35*2);
			ResetMap();*/
		}else if(ZombieCounter()==0){
			PrintBold(s:"Human Victory");
			//setWinState(1);
			Acs_ExecuteAlways(903,0,11,1);
		}else{
			PrintBold(s:"Zombie Victory");
			//setWinState(2);
			Acs_ExecuteAlways(903,0,11,2);
		}
		Acs_ExecuteAlways(903,0,3,4);
	}
	
	//NOW WE FREEZE EVERYBODY
	SetPlayerProperty(TRUE, ON, PROP_TOTALLYFROZEN);//(2021.01.19) so The round should be ended by now, so we freeze everybody
	//Dead ppl by not reaching the end should respawn with the frozen flag.
	
	
	//Reward player who finished a ZF map.
	int BRewarded;//ITs a bool to check if someone got rewarded or not.
	int PlayerCountToReward=Random(0,PlayerCount()-1);//GET a player ID to then get his TID
	for(int tries=0;tries<64;tries++){
		if(GetSafePlayer(PlayerCountToReward)==1){
			BRewarded=1;
			AwardPlayer(PlayerCountToReward);
			Log(s:"Tried Awarding"); 
			Log(n:PlayerCountToReward+1);
			break;
		}
		PlayerCountToReward=Random(0,PlayerCount()-1);
	}
	
	if(!BRewarded){
		for(int z=0;z<64;z++){ //If they are safe, reward them 
			if(GetSafePlayer(z)==1){ //TODO Check if its accurate
				AwardPlayer(z);
				Log(s:"Tried Awarding 2");
				Log(n:z+1);
				break;
			}
		}
	}
	
	
	for(int w=0;w<64;w++){
		SetActorProperty(w+PLAYER_ID,APROP_Invulnerable,ON);
		SetActorProperty(w+PLAYER_ID, APROP_SPEED, 1.0);
	}
	delay(35*8);
	Acs_ExecuteAlways(903,0,12,0);//Make sure the status is set to 0
	CleanInventory(0,0);
	resetStats();
	setWinState(0);
	Exit_Normal(0);
	
}






//When you join, or rejoin after spectating.
script 701 ENTER{
	Acs_ExecuteAlways(901,0); //We send ALL the informations needed to the client
	SetPlayerTID(PlayerNumber()); //Give the player a TID
	cleanInventory(1,PlayerNumber());//Takes the newly TID to flushes everything
	SetAliveState(PlayerNumber());//Set him Alive from the code pov
	
	//Give The Basic Inventory
	if(GetGameState()==2){ //Game has already started
		MakeZombie(ActivatorTID(),0,0,1);
		//[TODO]Put a Flag to disallow tp to the last checkpoint
		TeleportOther(ActivatorTID(),ZombieTID+GetMapState(),1);
	}else if(GetGameState()>=3){ //If the game is being finished
		SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
	}else{//Else, we are at the start then ! Be a human, be FREE!
		InventoryStarter(1,PlayerNumber());
		Acs_executeAlways(900,0,7,HumanCounter());//Update the value, not sure if this is usefull, because the round didnt start.
	}
}


script 702 DEATH{
	SetSafePlayer(PLAYER_ID+PlayerNumber(),0);//The player is not safe anymore, because he died, and surely will respawn as a zombie
	thing_remove(500+PLAYER_ID+PlayerNumber()); //Remove The Leader Symbole...
	SetDeathState(PlayerNumber());
	Thing_ChangeTID(0, 0);//Put the corpse to tid 0
}


//When you get killed/Telefragged.
script 703 RESPAWN{
	Acs_ExecuteAlways(901,0);
	SetPlayerTID(PlayerNumber()); //Give the player a TID
	cleanInventory(1,PlayerNumber());//Takes the newly TID to flushes everything
	SetAliveState(PlayerNumber());//Set him Alive from the code pov
	//Give The Basic Inventory
	if(GetGameState()==2){ //Game has already started
		MakeZombie(ActivatorTID(),0,0,1);
		//[TODO]Put a Flag to disallow tp to the last checkpoint
		TeleportOther(ActivatorTID(),ZombieTID+GetMapState(),1);
	}else if(GetGameState()>=3){ //If the game is being finished
		SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
	}else{//Else, we are at the start then ! Be a human, be FREE!
		InventoryStarter(1,PlayerNumber());
		Acs_executeAlways(900,0,7,HumanCounter());//Update the value, not sure if this is usefull, because the round didnt start.
	}
}


script 704(int gone) DISCONNECT{
	cleanInventory(1,gone);
	Thing_ChangeTID(0, 0);
	SetZombifyChanceFactor(gone,0);
	thing_remove(500+gone+PLAYER_ID);//Leader thing removal
	Acs_executeAlways(900,0,7,HumanCounter());//HumanCounter && ZombieCounter already update the value server wise, so we just need to send it to 
	Acs_executeAlways(900,0,8,ZombieCounter());//The clients
}


int ArrayRandom[2048];//Buffer all the listing of TID
script 705(int NbrOfZombieNeeded){
	int NbrOfPlayers=HumanCounter();
	int PlayerToZombify;
	int GZCF=0; //See below what this name means
	int TotalFactor=0;
	int chosenTID;
	int loopChecker=0; 
	while(NbrOfZombieNeeded!=0){
		for(int j=0;j<64;j++){//For all players, but it wont do anything because we check if they exist and are in game
			if(PlayerInGame(j)){
				GZCF = GetZombifyChanceFactor(j);
				TotalFactor+=GZCF;
				for(int factor=0;factor<GZCF;factor++){
					ArrayRandom[loopChecker] = (j+PLAYER_ID);//We put the player tid in the array, for how many factors he has
					loopChecker++;
				}
			}
		}
		for(int m=0;m<TotalFactor;m++){//Shuffle the data
			int shuffler=random(0,TotalFactor-1);
			int temp=ArrayRandom[m];
			ArrayRandom[m]=ArrayRandom[shuffler];
			ArrayRandom[shuffler]=temp;			
		}
		//so like the range is 0 to 63 = 64players.....
		chosenTID = Random(0,TotalFactor-1);
		PlayerToZombify = ArrayRandom[chosenTID];
		//Create a zombie
		if(CheckActorInventory(PlayerToZombify,"IsZombie")==0){
			MakeZombie(PlayerToZombify,1,0,0);
			PlayerToZombify-=PLAYER_ID;
			SetZombifyChanceFactor(PlayerToZombify,-1);
			NbrOfZombieNeeded--;
		}
		delay(3);
	}
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


//Zombie burn/fire script
Script 707 (void) 
{
	GiveInventory("IsBurning", 1);
	for(int i=0;i<=9;i++) {
		if(GetActorProperty(0,APROP_Waterlevel)>=2){
			TakeInventory("IsBurning", 1);
			terminate;
		}
		if(GetAliveState(GetPlayerTID2()-PLAYER_ID)==0){
			terminate;
		}
		Thing_Damage(0, 10+Random(0,10), 0);
		ThrustThingZ(0,1,1,0);
		SpawnProjectile(0, "ZIFlame", 0, 0, 0, 0, 0);
		Delay(18);
	}
	TakeInventory("IsBurning", 1);
}

//Zombie hand script called
script 708(int player,int infector,int type){
	if(player==0){Log(s:"BUG! REPORT: P=0"); terminate;}
	MakeZombie(player,type,infector,0);
}

script 709 OPEN NET CLIENTSIDE{
	RequestScriptPuke(901,0);
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

//Sprint pre-Checker
script 715 (int IsRunning) NET{
	if(!PlayerInGame(PlayerNumber())){terminate;}
	if(IsRunning && CheckInventory("HasPanicked")==0 ){
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
	while(CheckInventory("StaminaSprint")>20 && CheckInventory("IsSprinting")==1) {
		SetActorProperty(0, APROP_Speed, 1.3); //panicking speed
		TakeInventory("StaminaSprint", 60);
		Delay(10);
	}//If we break out of the loop, then it means ppl has no more stamina or dont sprint anymore
	if(CheckInventory("IsHuman")){
		SetActorProperty(0, APROP_Speed, 1.0);
	}
	if(CheckInventory("StaminaSprint")<=20){
		GiveInventory("IsOutOfBreath",1);
		delay(7);
		TakeInventory("IsOutOfBreath",1);
		delay(7);
		GiveInventory("IsOutOfBreath",1);
		delay(7);
		TakeInventory("IsOutOfBreath",1);
		delay(7);
		GiveInventory("IsOutOfBreath",1);
		delay(7);
		TakeInventory("IsOutOfBreath",1);
		
	}
}
//Panic script
Script 717 (void) NET {
	if( !PlayerInGame(PlayerNumber())){terminate;}	
	switch(GetGameState()){
		case -1:
		case 0:
		case 1:
			Print(s:"Is there something to be afraid of ?");
			break;		
		case 2:
			if(CheckInventory("IsHuman")){
				if(CheckInventory("HasPanicked")){
					Print(s:"You have already panicked this round!");
				}else if(CheckInventory("StaminaSprint") != 1200){
					Print(s:"You dont have enough stamina to panic!!");
					terminate;
				}else{
					Acs_executeAlways(718,0,0,0,0);
					//TakeInventory("StaminaSprint", 1200);
					GiveInventory("HasPanicked", 1);
					GiveInventory("PanicSpeed", 1);
					//GiveInventory("IsOutOfBreath", 1);
					TakeInventory("ZIShotgun", 1);
					TakeInventory("ZISuperShotgun", 1);
					TakeInventory("ZIRifle", 1);
					TakeInventory("ZISniper", 1);
					TakeInventory("ZIRocketLauncher", 1);
					HudmessageBold(n:0, s:" \cJhas panicked!";HUDMSG_FADEOUT, 360, CR_WHITE, 0.5, 0.43, 2.0); 
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
		TakeInventory("StaminaSprint",40);
		delay(35);
	}
	GiveInventory("IsOutOfBreath", 1);
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
				TeleportOther(P_id,ZombieTID,1);
				TakeActorInventory(P_id,"TPChecker",1);
				Print(s:"You now have ",d:CheckActorInventory(P_id,"TPChecker"),s:" TP left.");
			}else{
				Print(s:"You cannot teleport anymore...");
			}
		}else if(GetMapType()==1){ //IF ZF
			print(s:"you will be teleported in 3 seconds");
			delay(35*3);
			Teleport_NoFog(ZombieTID+GetMapState(),1,0,0);
			setSafePlayer(ActivatorTID(),0);
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

//Opening crate Script
int randArray[100];
script 722 (int who,int type){
	if(GetGameState()<=1){
		Print(s:"You cant use that now.");
		terminate;
	}
	for(int i=0;i<100;i++){
		randArray[i]=i;
	}
	for(int j=0;j<100;j++){
		int mem=random(0,99);
		int temp=randArray[j];
		randArray[j]=randArray[mem];
		randArray[mem]=temp;		
	}
	
	int newRandom = random(0,99);
	
	int rollRandom=randArray[newRandom];
	if(CheckActorInventory(who,"IsZombie")==1){
		switch(type){
			case 1:
				TakeActorInventory(who,"CommonBonusCrate",1);
				break;
			case 2:
				TakeActorInventory(who,"WeaponBonusCrate",1);
				break;
			case 3:
				TakeActorInventory(who,"SupportBonusCrate",1);
				break;
			case 4:
				TakeActorInventory(who,"RareBonusCrate",1);
				break;
			case 5:
				TakeActorInventory(who,"UltimateBonusCrate",1);
				break;
		}
		if(rollRandom<=20){
			Print(s:"You got Nothing!");
		}//Nothing
		else if(rollRandom<=36){
			Print(s:"Speed Boost!");
			GiveActorInventory(who,"BonusSpeed",1);
		}//Speed Boost
		else if(rollRandom<=52){
			Print(s:"Jump Boost!");
			GiveActorInventory(who,"BonusHighJump",1);
		}//Jump Boost
		else if(rollRandom<=68){
			Print(s:"Night Vision!");
			GiveActorInventory(who,"NightVision",1);
		}//Night Vision
		else if(rollRandom<=84){
			Print(s:"Instant Spit!");
			GiveActorInventory(who,"ZombieSecondaryCoolDown",3000);
		}// Spit
		else{
			Print(s:"You are now invisible for 35 seconds!");
			GiveActorInventory(who,"BonusInvisibility",1);
		}//Invisible
	}else{
		switch(type){
			case 0:
				terminate;
			case 1:
				TakeActorInventory(who,"CommonBonusCrate",1);
				if(rollRandom<=20){
					print(s:"Nothing! :c");
				}else if(rollRandom<=30){
					if(CheckActorInventory(who,"StaminaSprint")>1000){
						print(s:"Nothing! :c");
					}else{
					print(s:"Stamina Restock!");
					GiveActorInventory(who,"StaminaSprint",1200);
					}
				}else if(rollRandom<=40){
					Print(s:"10% damage for 30 secondes!");
					GiveActorInventory(who,"DamageBonus",1);
				}else if(rollRandom<=50){
					Print(s:"Additional molotov!");
					GiveActorInventory(who,"HandGrenadeAmmo",1);
				}else if(rollRandom<=60){
					Print(s:"Additional Stunt grenade!");
					GiveActorInventory(who,"HandExpGrenadeAmmo",1);
				}else if(rollRandom<=70){
					if(CheckActorInventory(who,"ZISniper")==1){
						print(s:"Nothing! :c");
					}else{
					Print(s:"You found a sniper!");
					GiveActorInventory(who,"ZISniper",1);
					}
				}else if(rollRandom<=80){
					if(CheckActorInventory(who,"ZISuperShotGun")==1){
						print(s:"Nothing! :c");
					}else{
					Print(s:"You found a SSG!");
					GiveActorInventory(who,"ZISuperShotGun",1);
					}
				}else if(rollRandom<=90){
					if(CheckActorInventory(who,"ZIRocketLauncher")==1){
						print(s:"Nothing! :c");
					}else{
					Print(s:"You found a Rocket Launcher!");
					GiveActorInventory(who,"ZIRocketLauncher",1);
					}
				}else{
					Print(s:"Fall reduction!");
					GiveActorInventory(who,"BonusFallDamageProtection",1);
				}
				break;
			case 2:
				TakeActorInventory(who,"WeaponBonusCrate",1);
				if(rollRandom<=5){
					Print(s:"10% damage for 30 secondes!");
					GiveActorInventory(who,"DamageBonus",1);
				}else if(rollRandom<=15){
					Print(s:"Additional molotov!");
					GiveActorInventory(who,"HandGrenadeAmmo",1);
				}else if(rollRandom<=25){
					Print(s:"Additional Stunt grenade!");
					GiveActorInventory(who,"HandExpGrenadeAmmo",1);
				}else if(rollRandom<=50){
					if(CheckActorInventory(who,"ZISniper")==1){
						Print(s:"10% damage for 30 secondes!");
						GiveActorInventory(who,"DamageBonus",1);
					}else{
					Print(s:"You found a sniper!");
					GiveActorInventory(who,"ZISniper",1);
					}
				}else if(rollRandom<=75){
					if(CheckActorInventory(who,"ZISuperShotGun")==1){
						Print(s:"10% damage for 30 secondes!");
						GiveActorInventory(who,"DamageBonus",1);
					}else{
					Print(s:"You found a SSG!");
					GiveActorInventory(who,"ZISuperShotGun",1);
					}
				}else{
					if(CheckActorInventory(who,"ZIRocketLauncher")==1){
						Print(s:"10% damage for 30 secondes!");
						GiveActorInventory(who,"DamageBonus",1);
					}else{
					Print(s:"You found a Rocket Launcher!");
					GiveActorInventory(who,"ZIRocketLauncher",1);
					}
				}
				break;
			case 3:
				TakeActorInventory(who,"SupportBonusCrate",1);
				if(rollRandom<=20){
					if(CheckActorInventory(who,"StaminaSprint")>1000){
						print(s:"Nothing! :c");
					}else{
					print(s:"Stamina Restock!");
					GiveActorInventory(who,"StaminaSprint",1200);
					}
				}else if(rollRandom<=40){
					Print(s:"10% damage for 30 secondes!");
					GiveActorInventory(who,"DamageBonus",1);
				}else if(rollRandom<=65){
					Print(s:"Additional molotov!");
					GiveActorInventory(who,"HandGrenadeAmmo",1);
				}else if(rollRandom<=90){
					Print(s:"Additional Stunt grenade!");
					GiveActorInventory(who,"HandExpGrenadeAmmo",1);
				}else{
					if(CheckActorInventory(who,"LeaderSpeed")==1){
						print(s:"you got 3 Rare Bonus Crate!");
						GiveActorInventory(who,"RareBonusCrate",3);
					}else{
					PrintBold(n:who-PLAYER_ID+1,s:" is now an ELITE!");
					GiveActorInventory(who,"LeaderSpeed",1);
					GiveActorInventory(who,"LeaderDamage",1);
					}
				}
				break;
			case 4:
				TakeActorInventory(who,"RareBonusCrate",1);
				if(rollRandom<=5){
					print(s:"Nothing! :c");
				}else if(rollRandom<=10){
					print(s:"Stamina Restock!");
					GiveActorInventory(who,"StaminaSprint",1200);
				}else if(rollRandom<=20){
					Print(s:"10% damage for 30 secondes!");
					GiveActorInventory(who,"DamageBonus",1);
				}else if(rollRandom<=30){
					Print(s:"Additional molotov!");
					GiveActorInventory(who,"HandGrenadeAmmo",1);
				}else if(rollRandom<=40){
					Print(s:"Additional Stunt grenade!");
					GiveActorInventory(who,"HandExpGrenadeAmmo",1);
				}else if(rollRandom<=55){
					if(CheckActorInventory(who,"ZISniper")==1){
						Print(s:"10% damage for 30 secondes!");
						GiveActorInventory(who,"DamageBonus",1);
					}else{
					Print(s:"You found a sniper!");
					GiveActorInventory(who,"ZISniper",1);
					}
				}else if(rollRandom<=70){
					if(CheckActorInventory(who,"ZISuperShotGun")==1){
						Print(s:"10% damage for 30 secondes!");
						GiveActorInventory(who,"DamageBonus",1);
					}else{
					Print(s:"You found a SSG!");
					GiveActorInventory(who,"ZISuperShotGun",1);
					}
				}else if(rollRandom<=85){
					if(CheckActorInventory(who,"ZIRocketLauncher")==1){
						Print(s:"10% damage for 30 secondes!");
						GiveActorInventory(who,"DamageBonus",1);
					}else{
					Print(s:"You found a Rocket Launcher!");
					GiveActorInventory(who,"ZIRocketLauncher",1);
					}
				}else{
					Print(s:"Fall reduction!");
					GiveActorInventory(who,"BonusFallDamageProtection",1);
				}
				break;
			case 5:
				TakeActorInventory(who,"UltimateBonusCrate",1);
				if(rollRandom<=5){
					Print(s:"10% damage for 30 secondes!");
					GiveActorInventory(who,"DamageBonus",1);
				}else if(rollRandom<=10){
					Print(s:"Additional molotov!");
					GiveActorInventory(who,"HandGrenadeAmmo",1);
				}else if(rollRandom<=15){
					Print(s:"Additional Stunt grenade!");
					GiveActorInventory(who,"HandExpGrenadeAmmo",1);
				}else if(rollRandom<=35){
					if(CheckActorInventory(who,"ZISniper")==1){
						Print(s:"10% damage for 30 secondes!");
						GiveActorInventory(who,"DamageBonus",1);
					}else{
					Print(s:"You found a sniper!");
					GiveActorInventory(who,"ZISniper",1);
					}
				}else if(rollRandom<=55){
					if(CheckActorInventory(who,"ZISuperShotGun")==1){
						Print(s:"10% damage for 30 secondes!");
						GiveActorInventory(who,"DamageBonus",1);
					}else{
					Print(s:"You found a SSG!");
					GiveActorInventory(who,"ZISuperShotGun",1);
					}
				}else if(rollRandom<=75){
					if(CheckActorInventory(who,"ZIRocketLauncher")==1){
						Print(s:"10% damage for 30 secondes!");
						GiveActorInventory(who,"DamageBonus",1);
					}else{
					Print(s:"You found a Rocket Launcher!");
					GiveActorInventory(who,"ZIRocketLauncher",1);
					}
				}else if(rollRandom<=90){
					Print(s:"Fall reduction!");
					GiveActorInventory(who,"BonusFallDamageProtection",1);
				}else{
					if(CheckActorInventory(who,"LeaderSpeed")==1){
						print(s:"you got 3 Ultimate Bonus Crate!");
						GiveActorInventory(who,"UltimateBonusCrate",3);
					}else{
					PrintBold(n:who-PLAYER_ID+1,s:" is now an ELITE!");
					GiveActorInventory(who,"LeaderSpeed",1);
					GiveActorInventory(who,"LeaderDamage",1);
					}
				}
				break;
				
		}
	}
}

//Leader symbol script
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

//Give the killer a bonus box
script 724(int DEADTID){
	incrementStats(2,DEADTID);
	if(!SetActivatorToTarget(0)){terminate;} //If we cant get the killer as activator
	int killer_TID = PlayerNumber()+PLAYER_ID;
	int randomaward = Random(0,100);
	if(CheckActorInventory(killer_TID,"IsZombie")==1){terminate;}
	if(killer_TID == DEADTID || killer_TID==0){terminate;}//(2020.11.24) Maybe somehow he killed himself ? (2021.01.07) more fixe
	if(GetGameState()>=3){terminate;} //prevent telefuck award at the end lol
	incrementStats(1,killer_TID);
	
	delay(2);
	AwardPlayer(killer_TID);
}


script 725(int tid,int health){
	SetActorProperty(tid,APROP_HEALTH,health);
	ACS_ExecuteAlways(726,0,tid,health);
}
//IDK why i have to do that to update health properly but lawl. . .
script 726(int tid,int health)CLIENTSIDE{
	SetActorProperty(tid,APROP_HEALTH,health);
}


//Suicide Script
script 727 (void) NET{
	if(CheckActorInventory(ActivatorTID(),"isZombie")==1){
		setSafePlayer(ActivatorTID(),0);
		setSuicideState(ActivatorTID(),1);
		setHealthBeforeSuicide(ActivatorTID(),getActorProperty(ActivatorTID(),APROP_HEALTH));
	}
	delay(5);
	SetActorProperty(ActivatorTID(),APROP_HEALTH,0);
}


// Teleport to the start
Script 728 (void) NET {
	int P_id = PLAYER_ID+PlayerNumber();
	if(CheckActorInventory(P_id,"IsZombie")==1){
		if(GetMapType()==2){//If ZR
			Print(s:"This is not an escape map!!");
		}
		else{ //IF ZF
			print(s:"you will be teleported in 3 seconds");
			delay(35*3);
			Teleport_NoFog(ZombieTID,1,0,0);
			setSafePlayer(P_id,0);
		}
	}else{
		Print(s:"Humans cannot tp to the start...");
	}
}

script 729(void)NET{
	int IP_ID=ActivatorTID();
	int type=random(0,4);
	switch(type){
		case 0:
			PlaySound(IP_ID,"Player/Taunt/OMG",6,1.0,0,1.0);
			break;
		case 1:
			PlaySound(IP_ID,"Player/Taunt/Stop",6,1.0,0,1.0);
			break;
		case 2:
			PlaySound(IP_ID,"Player/Taunt/Shit",6,1.0,0,1.0);
			break;
		case 3:
			PlaySound(IP_ID,"Player/Taunt/FuckYou",6,1.0,0,1.0);
			break;
		case 4:
			PlaySound(IP_ID,"Player/Taunt/Surprise",6,1.0,0,1.0);
			break;
	}
}





//Client server script, For readability.

//Send one thing Server->Client
script 900 (int whatToUpdate,int Value){
	Acs_executeAlways(902,0,WhatToUpdate,Value);
}

//Resend Everything Server->Client
//Good when player join the server
//Net because if someone want to redownload the data
script 901(void)Net{
	Acs_executeAlways(902,0,1,GetTimePickingZombie());
	Acs_executeAlways(902,0,3,GetGameState());
	Acs_executeAlways(902,0,4,GetMapState()); //Give the mapState to the player
	Acs_executeAlways(902,0,10,GetTimerSeconds());//Sends the current timer to the newly connected person
	Acs_executeAlways(902,0,7,GetHumanNumber());//We send them directly to the client, no need to reupdate it server-wise
	Acs_executeAlways(902,0,8,GetZombieNumber());
}

//Updating client's values
script 902(int whatToUpdate,int Value)CLIENTSIDE{
	switch(whatToUpdate){
		case 0:
			break;
		case 1:
			SetTimePickingZombie(Value);
			break;
		case 2:
			if(isNetworkGame()){
				DecrementTimePickingZombie();
			}
			break;
		case 3:
			SetGameState(Value);
			break;
		case 4:
			SetMapState(Value);
			break;
		case 5:
			IncrementMapState();
			break;
		case 6:
			SetMinuteToTimer(Value);
			break;
		case 7:
			SetHumanNumber(Value);
			break;
		case 8:
			SetZombieNumber(Value);
			break;
		case 9:
			if(isNetworkGame()){
				DecrementTimer();//Not a good way to do that.....XD
				//TBH i should parse a boolean to the client, and the client
				//Should anticipate the countdown of the round.
				//So the client is needing information from the server, constantly....
				//Too bad!
			}
			break;
		case 10:
			setSecondsToTimer(Value);
			break;
		case 11:
			setWinState(Value);
			break;
		case 12:
			setCountDownStatus(Value);
			break;
	}
}

//Update Server AND Client
script 903(int whatToUpdate,int Value){
	switch(whatToUpdate){
		case 0:
			break;
		case 1: 
			SetTimePickingZombie(Value);
			break;
		case 2:
			DecrementTimePickingZombie();
			break;
		case 3:
			SetGameState(Value);
			break;
		case 4:
			SetMapState(Value);
			break;
		case 5:
			IncrementMapState();
			break;
		case 6:
			SetMinuteToTimer(Value);
			break;
		case 7:
			SetHumanNumber(Value);
			break;
		case 8:
			SetZombieNumber(Value);
			break;
		case 9:
			DecrementTimer();
			break;
		case 10:
			setSecondsToTimer(Value);
			break;
		case 11:
			setWinState(Value);
			break;
		case 12:
			setCountDownStatus(Value);
			break;
	}
	Acs_executeAlways(902,0,WhatToUpdate,Value);	
}
