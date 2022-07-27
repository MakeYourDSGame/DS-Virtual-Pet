#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <maxmod9.h>
#include <time.h>

SpriteEntry OAMCopy[128];

//Backgrounds
#include <Start.h>
#include <BottomStart.h>
#include <Game.h>
#include <BottomGame.h>
#include <BattleMenu.h>
#include <BattleIcons.h>

#include <BattleBottom.h>

//Sprites
#include <Ghost.h>
#include <BSOD.h>
#include <Three.h>
#include <Four.h>
#include <Five.h>
#include <Hearts.h>
#include <Hunger.h>
#include <Sick.h>
#include <Numbers.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#include "events.c"

volatile int HungerFrame = 0;
volatile int MovementFrame = 0;
int MovementFrameNum = 0;
int TimeBetweenFrames = 25;
int TimeBetweenFramesDone = 0;
int SickFrames = 0;

int Coins = 100;
int Place1 = 0;
int Place2 = 0;
int Place3 = 0;
int Hunger = 5;
int Health = 5;
int MaxHealth = 5;
int GetHealth = 0;
bool isSick = false;
bool BattleMenuOpen = false;
bool isInBattle = false;
bool GameStart = false;

bool doneMove = false;
bool doneMoveBack = false;

/*
0 = Player's Turn
1 = Enemy's turn
2 = Player Move
3 = Enemy Move
4 = Player Won
5 = Player Lost
*/
int battleState = 0;

/*
0 = Null
1 = Rock
2 = Paper
3 = Scissors
*/
int PlayersChoice = 0;

/*
0 = Ghost
1 = PC
2 = ?
3 = ?
4 = ?
5 = Choose
*/
int PetYouHave = 5;

int BattleEnemyHealth;
int BattlePlayerHealth;

int bg3;

enum { CONTINUOUS, SINGLE } TouchType = CONTINUOUS;

void Vblank() {
	HungerFrame++;
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	//Create Pet
	Sprite Pet = {0,0};
	Pet.Xpos = 50;
	Pet.Ypos = 140;

	//Create Foods
	Sprite Food1 = {0,0};
	Food1.Xpos = 0;
	Food1.Ypos = 16;

	Sprite Food2 = {0,0};
	Food2.Xpos = 16;
	Food2.Ypos = 16;

	Sprite Food3 = {0,0};
	Food3.Xpos = 32;
	Food3.Ypos = 16;

	Sprite Food4 = {0,0};
	Food4.Xpos = 48;
	Food4.Ypos = 16;

	Sprite Food5 = {0,0};
	Food5.Xpos = 64;
	Food5.Ypos = 16;

	Sprite Heart1 = {0,0};
	Heart1.Xpos = 0;
	Heart1.Ypos = 0;

	Sprite Heart2 = {0,0};
	Heart2.Xpos = 16;
	Heart2.Ypos = 0;

	Sprite Heart3 = {0,0};
	Heart3.Xpos = 32;
	Heart3.Ypos = 0;

	Sprite Heart4 = {0,0};
	Heart4.Xpos = 48;
	Heart4.Ypos = 0;

	Sprite Heart5 = {0,0};
	Heart5.Xpos = 64;
	Heart5.Ypos = 0;

	Sprite Effect = {0,0};
	Effect.Xpos = 0;
	Effect.Ypos = 32;

	Sprite Ones = {0,0};
	Sprite Tens = {0,0};
	Sprite Hunds = {0,0};

	Sprite Enem = {0,0};
	Enem.Xpos = 198;
	Enem.Ypos = 140;

	Sprite EnemyAttack = {0,0};

	Sprite PlayerAttack = {0,0};

	Sprite Phantom = {0,0};
	Sprite BSOL = {0,0};
	Sprite Three = {0,0};
	Sprite Four = {0,0};
	Sprite Five = {0,0};

	
	//Set Up Collision Boxes
	Box PetBox;
	PetBox.OffsetX = 0;
	PetBox.OffsetY = 0; //26
	PetBox.Xpos = PetBox.Xpos + PetBox.OffsetX;
	PetBox.Ypos = PetBox.Ypos + PetBox.OffsetY;
	PetBox.SizeX = 16;
	PetBox.SizeY = 16;

	Box TouchBox;
	TouchBox.SizeX = 1;
	TouchBox.SizeY = 1;
	TouchBox.Xpos = -20;
	TouchBox.Ypos = -20;

	Box FoodButton1;
	FoodButton1.SizeX = 48;
	FoodButton1.SizeY = 24;
	FoodButton1.Xpos = 57;
	FoodButton1.Ypos = 64;

	Box FoodButton2;
	FoodButton2.SizeX = 48;
	FoodButton2.SizeY = 24;
	FoodButton2.Xpos = 153;
	FoodButton2.Ypos = 64;

	Box FoodButton3;
	FoodButton3.SizeX = 48;
	FoodButton3.SizeY = 24;
	FoodButton3.Xpos = 16;
	FoodButton3.Ypos = 117;

	Box FoodButton4;
	FoodButton4.SizeX = 48;
	FoodButton4.SizeY = 24;
	FoodButton4.Xpos = 105;
	FoodButton4.Ypos = 117;

	Box FoodButton5;
	FoodButton5.SizeX = 48;
	FoodButton5.SizeY = 24;
	FoodButton5.Xpos = 193;
	FoodButton5.Ypos = 117;

	Box BattleButton1;
	BattleButton1.SizeX = 48;
	BattleButton1.SizeY = 24;
	BattleButton1.Xpos = 105;
	BattleButton1.Ypos = 53;

	Box BattleButton2;
	BattleButton2.SizeX = 48;
	BattleButton2.SizeY = 24;
	BattleButton2.Xpos = 105;
	BattleButton2.Ypos = 101;

	Box BattleButton3;
	BattleButton3.SizeX = 48;
	BattleButton3.SizeY = 24;
	BattleButton3.Xpos = 105;
	BattleButton3.Ypos = 149;

	Box RockButton;
	RockButton.SizeX = 48;
	RockButton.SizeY = 24;
	RockButton.Xpos = 25;
	RockButton.Ypos = 93;

	Box PaperButton;
	PaperButton.SizeX = 48;
	PaperButton.SizeY = 24;
	PaperButton.Xpos = 105;
	PaperButton.Ypos = 93;

	Box ScissorsButton;
	ScissorsButton.SizeX = 48;
	ScissorsButton.SizeY = 24;
	ScissorsButton.Xpos = 185;
	ScissorsButton.Ypos = 93;

	//Touch Position
	touchPosition touch;
   
   //Setting Video Modes
	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_5_2D);

	//Set up SpritesMapping
	oamInit(&oamMain, SpriteMapping_1D_128, true);
	oamInit(&oamSub, SpriteMapping_1D_128, true);

	//Set F Bank.
	vramSetBankF(VRAM_F_LCD);
	//Pet
	init32(&Phantom, (u8*)GhostTiles);
	dmaCopy(GhostPal, &VRAM_F_EXT_SPR_PALETTE[0][0],GhostPalLen);
	init32(&BSOL, (u8*)BSODTiles);
	dmaCopy(BSODPal, &VRAM_F_EXT_SPR_PALETTE[1][0],BSODPalLen);
	init32(&Three, (u8*)ThreeTiles);
	dmaCopy(ThreePal, &VRAM_F_EXT_SPR_PALETTE[3][0],ThreePalLen);
	init32(&Four, (u8*)FourTiles);
	dmaCopy(FourPal, &VRAM_F_EXT_SPR_PALETTE[4][0],FourPalLen);
	init32(&Five, (u8*)FiveTiles);
	dmaCopy(FivePal, &VRAM_F_EXT_SPR_PALETTE[5][0],FivePalLen);
	//Enemy
	init32(&Enem, (u8*)BSODTiles);
	dmaCopy(BSODPal, &VRAM_F_EXT_SPR_PALETTE[1][0],BSODPalLen);
	//Battle Choices
	init32(&EnemyAttack, (u8*)BattleIconsTiles);
	dmaCopy(BattleIconsPal, &VRAM_F_EXT_SPR_PALETTE[2][0],BattleIconsPalLen);
	init32(&PlayerAttack, (u8*)BattleIconsTiles);
	dmaCopy(BattleIconsPal, &VRAM_F_EXT_SPR_PALETTE[2][0],BattleIconsPalLen);
	//Set F Bank.
	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);

	//Set I Bank.
	vramSetBankI(VRAM_I_LCD);
	//Hearts
	init16Sub(&Heart1, (u8*)HeartsTiles);
	dmaCopy(HeartsPal, &VRAM_I_EXT_SPR_PALETTE[0][0],HeartsPalLen);
	init16Sub(&Heart2, (u8*)HeartsTiles);
	dmaCopy(HeartsPal, &VRAM_I_EXT_SPR_PALETTE[0][0],HeartsPalLen);
	init16Sub(&Heart3, (u8*)HeartsTiles);
	dmaCopy(HeartsPal, &VRAM_I_EXT_SPR_PALETTE[0][0],HeartsPalLen);
	init16Sub(&Heart4, (u8*)HeartsTiles);
	dmaCopy(HeartsPal, &VRAM_I_EXT_SPR_PALETTE[0][0],HeartsPalLen);
	init16Sub(&Heart5, (u8*)HeartsTiles);
	dmaCopy(HeartsPal, &VRAM_I_EXT_SPR_PALETTE[0][0],HeartsPalLen);
	//Food
	init16Sub(&Food1, (u8*)HungerTiles);
	dmaCopy(HungerPal, &VRAM_I_EXT_SPR_PALETTE[1][0],HungerPalLen);
	init16Sub(&Food2, (u8*)HungerTiles);
	dmaCopy(HungerPal, &VRAM_I_EXT_SPR_PALETTE[1][0],HungerPalLen);
	init16Sub(&Food3, (u8*)HungerTiles);
	dmaCopy(HungerPal, &VRAM_I_EXT_SPR_PALETTE[1][0],HungerPalLen);
	init16Sub(&Food4, (u8*)HungerTiles);
	dmaCopy(HungerPal, &VRAM_I_EXT_SPR_PALETTE[1][0],HungerPalLen);
	init16Sub(&Food5, (u8*)HungerTiles);
	dmaCopy(HungerPal, &VRAM_I_EXT_SPR_PALETTE[1][0],HungerPalLen);
	//Sick
	init16Sub(&Effect, (u8*)SickTiles);
	dmaCopy(SickPal, &VRAM_I_EXT_SPR_PALETTE[2][0],SickPalLen);
	//Numbers
	init8Sub(&Ones, (u8*)NumbersTiles);
	dmaCopy(NumbersPal, &VRAM_I_EXT_SPR_PALETTE[3][0],NumbersPalLen);
	init8Sub(&Tens, (u8*)NumbersTiles);
	dmaCopy(NumbersPal, &VRAM_I_EXT_SPR_PALETTE[3][0],NumbersPalLen);
	init8Sub(&Hunds, (u8*)NumbersTiles);
	dmaCopy(NumbersPal, &VRAM_I_EXT_SPR_PALETTE[3][0],NumbersPalLen);
	//Set I Bank.
	vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);

	animate32(&Phantom);
	animate32(&BSOL);
	animate32(&Three);
	animate32(&Four);
	animate32(&Five);

	srand ( time(NULL) );
	PetYouHave = rand() % 5;
	if(PetYouHave == 0){
		Pet.sprite_gfx_mem = Phantom.sprite_gfx_mem;
		Pet.frame_gfx = Phantom.frame_gfx;
		Pet.palette = 0;
	}
	else if(PetYouHave == 1){
		Pet.sprite_gfx_mem = BSOL.sprite_gfx_mem;
		Pet.frame_gfx = BSOL.frame_gfx;
		Pet.palette = 1;
	}
	else if(PetYouHave == 2){
		Pet.sprite_gfx_mem = Three.sprite_gfx_mem;
		Pet.frame_gfx = Three.frame_gfx;
		Pet.palette = 3;
	}
	else if(PetYouHave == 3){
		Pet.sprite_gfx_mem = Four.sprite_gfx_mem;
		Pet.frame_gfx = Four.frame_gfx;
		Pet.palette = 4;
	}
	else if(PetYouHave == 4){
		Pet.sprite_gfx_mem = Five.sprite_gfx_mem;
		Pet.frame_gfx = Five.frame_gfx;
		Pet.palette = 5;
	}

	//Title screen background.
	bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(StartBitmap, bgGetGfxPtr(bg3), 256*192);
	dmaCopy(StartPal, BG_PALETTE, 256*2);
	//Bottom Screen
	bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(BottomStartBitmap, bgGetGfxPtr(bg3), 256*192);
	dmaCopy(BottomStartPal, BG_PALETTE_SUB, 256*2);
	
	mmInitDefaultMem((mm_addr)soundbank_bin);
	
	// load the module
	mmLoad( MOD_MYSONG );

	// load sound effects
	//mmLoadEffect( SFX_AMBULANCE );
	//mmLoadEffect( SFX_BOOM );

	// Start playing module
	mmStart( MOD_MYSONG, MM_PLAY_LOOP );

/*
	mm_sound_effect ambulance = {
		{ SFX_AMBULANCE } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};
*/

	//Loops every frame
	while(1) {

		swiWaitForVBlank();

		scanKeys();

		touchRead(&touch);
		
		int pressed = keysDown();
		int held = keysHeld();
		int released = keysUp();

		PetBox.Xpos = Pet.Xpos + PetBox.OffsetX;
		PetBox.Ypos = Pet.Ypos + PetBox.OffsetY;

		bool isGoing;
		int goingX;

		if(GameStart == true){
			//If "Health" is less than "MaxHealth" and "Health" is not equal to 0.
			if(Health < MaxHealth && Health != 0){
				//If "Hunger" is equal to 5.
				if(Hunger == 5){
					//"GetHealth" + 1.
					GetHealth++;
					//If "GeatHealth" is greater than or equal to 200.
					if(GetHealth >= 200){
						//Reset "GetHealth" variable.
						GetHealth = 0;
						//Health + 1.
						Health++;
					}
				}
			}

			//Movement
			if(MovementFrame >= MovementFrameNum){
				//Set the random variable.
				MovementFrameNum = rand() % 100;
				//Reset the "MovementFrame" variable.
				MovementFrame = -1;
				//Choose where to move.
				isGoing = false;
			}

			//Health & Hunger
			if(HungerFrame >= 500){
				//If fully hungry.
				if(Hunger == 0){
					//If has been hungry long enough to lose a life.
					if(HungerFrame >= 700){
						//Lose a life.
						Health--;
						//Reset the "HungerFrame" variable.
						HungerFrame = -1;
					}
				}
				else{
					//Lose one hunger.
					Hunger--;
					//Reset the "HungerFrame" variable.
					HungerFrame = -1;
				}
			}
			else if(isSick == true){
					if(HungerFrame >= 250){
						//If fully hungry.
					if(Hunger == 0){
						//If has been hungry long enough to lose a life.
						if(HungerFrame >= 700){
							//Lose a life.
							Health--;
							//Reset the "HungerFrame" variable.
							HungerFrame = -1;
						}
					}
					else{
						//Lose one hunger.
						Hunger--;
						//Reset the "HungerFrame" variable.
						HungerFrame = -1;
					}
				}
			}

			//If you can move, move. Else, choose random spot to move to.
			if(isGoing == false){
				//Get random X position to move to.
				goingX = rand() % 240;
				//Can start moving.
				isGoing = true;
			}
			else{
				//Move the "Pet".
				Pet = MoveActorSmooth(goingX, Pet.Ypos, Pet);
			}

			if(SickFrames >= 800){
				if(isSick == false){
					int GetSick = rand() % 100;
					if(Hunger <= 1){
						if(GetSick < 35){
							isSick = true;
						}
					}
					else if(Hunger < 4){
						if(GetSick < 25){
							isSick = true;
						}
					}
					else{
						if(GetSick < 10){
							isSick = true;
						}
					}
					SickFrames = -1;
				}
			}
			if(isSick == true){
				if(SickFrames >= 500){
					if(Health > 0){
						Health--;
					}
					SickFrames = -1;
				}
			}
			SickFrames++;

			if(pressed & KEY_TOUCH){
				TouchBox.Xpos = touch.px;
				TouchBox.Ypos = touch.py;
			}
			else{
				TouchBox.Xpos = -20;
				TouchBox.Ypos = -20;
			}

			if(pressed & KEY_START){
				if(BattleMenuOpen == false){
					bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
					dmaCopy(BattleMenuBitmap, bgGetGfxPtr(bg3), 256*192);
					dmaCopy(BattleMenuPal, BG_PALETTE_SUB, 256*2);
					BattleMenuOpen = true;
				}
				else{
					bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
					dmaCopy(BottomGameBitmap, bgGetGfxPtr(bg3), 256*192);
					dmaCopy(BottomGamePal, BG_PALETTE_SUB, 256*2);
					BattleMenuOpen = false;
				}
			}

			bool hit = false;

			if(BattleMenuOpen == false){
				hit = CollisionCheck(TouchBox, FoodButton1);
				if(hit == true){
					Feed(1);
				}
				hit = false;
				hit = CollisionCheck(TouchBox, FoodButton2);
				if(hit == true){
					Feed(2);
				}
				hit = false;
				hit = CollisionCheck(TouchBox, FoodButton3);
				if(hit == true){
					Heal(1);
				}
				hit = false;
				hit = CollisionCheck(TouchBox, FoodButton4);
				if(hit == true){
					Heal(2);
				}
				hit = false;
				hit = CollisionCheck(TouchBox, FoodButton5);
				if(hit == true){
					Heal(3);
				}
			}
			else{
				hit = CollisionCheck(TouchBox, BattleButton1);
				if(hit == true){
					Pet.Xpos = 25;
					openOfflineBattle();
				}
				hit = false;
				//hit = CollisionCheck(TouchBox, BattleButton2);
				if(hit == true){
					openLocalBattle();
				}
				hit = false;
				//hit = CollisionCheck(TouchBox, BattleButton3);
				if(hit == true){
					openOnlineBattle();
				}
			}

			//---------------------------------------------------------------------------------------------------------------------------

			Place1 = Coins/100%10;
			Place2 = Coins/10%10;
			Place3 = Coins%10;

			if(Place1 == 0){
				Hunds.anim_frame = 0;
			}
			else if(Place1 == 1){
				Hunds.anim_frame = 1;
			}
			else if(Place1 == 2){
				Hunds.anim_frame = 2;
			}
			else if(Place1 == 3){
				Hunds.anim_frame = 3;
			}
			else if(Place1 == 4){
				Hunds.anim_frame = 4;
			}
			else if(Place1 == 5){
				Hunds.anim_frame = 5;
			}
			else if(Place1 == 6){
				Hunds.anim_frame = 6;
			}
			else if(Place1 == 7){
				Hunds.anim_frame = 7;
			}
			else if(Place1 == 8){
				Hunds.anim_frame = 8;
			}
			else if(Place1 == 9){
				Hunds.anim_frame = 9;
			}

			if(Place2 == 0){
				Tens.anim_frame = 0;
			}
			else if(Place2 == 1){
				Tens.anim_frame = 1;
			}
			else if(Place2 == 2){
				Tens.anim_frame = 2;
			}
			else if(Place2 == 3){
				Tens.anim_frame = 3;
			}
			else if(Place2 == 4){
				Tens.anim_frame = 4;
			}
			else if(Place2 == 5){
				Tens.anim_frame = 5;
			}
			else if(Place2 == 6){
				Tens.anim_frame = 6;
			}
			else if(Place2 == 7){
				Tens.anim_frame = 7;
			}
			else if(Place2 == 8){
				Tens.anim_frame = 8;
			}
			else if(Place2 == 9){
				Tens.anim_frame = 9;
			}

			if(Place3 == 0){
				Ones.anim_frame = 0;
			}
			else if(Place3 == 1){
				Ones.anim_frame = 1;
			}
			else if(Place3 == 2){
				Ones.anim_frame = 2;
			}
			else if(Place3 == 3){
				Ones.anim_frame = 3;
			}
			else if(Place3 == 4){
				Ones.anim_frame = 4;
			}
			else if(Place3 == 5){
				Ones.anim_frame = 5;
			}
			else if(Place3 == 6){
				Ones.anim_frame = 6;
			}
			else if(Place3 == 7){
				Ones.anim_frame = 7;
			}
			else if(Place3 == 8){
				Ones.anim_frame = 8;
			}
			else if(Place3 == 9){
				Ones.anim_frame = 9;
			}

			//Update the hunger and health.
			if(Health == 5){
				Heart5.anim_frame = 0;
				Heart4.anim_frame = 0;
				Heart3.anim_frame = 0;
				Heart2.anim_frame = 0;
				Heart1.anim_frame = 0;
			}
			else if(Health == 4){
				Heart5.anim_frame = 1;
				Heart4.anim_frame = 0;
				Heart3.anim_frame = 0;
				Heart2.anim_frame = 0;
				Heart1.anim_frame = 0;
			}
			else if(Health == 3){
				Heart5.anim_frame = 1;
				Heart4.anim_frame = 1;
				Heart3.anim_frame = 0;
				Heart2.anim_frame = 0;
				Heart1.anim_frame = 0;
			}
			else if(Health == 2){
				Heart5.anim_frame = 1;
				Heart4.anim_frame = 1;
				Heart3.anim_frame = 1;
				Heart2.anim_frame = 0;
				Heart1.anim_frame = 0;
			}
			else if(Health == 1){
				Heart5.anim_frame = 1;
				Heart4.anim_frame = 1;
				Heart3.anim_frame = 1;
				Heart2.anim_frame = 1;
				Heart1.anim_frame = 0;
			}
			else if(Health <= 0){
				Heart5.anim_frame = 1;
				Heart4.anim_frame = 1;
				Heart3.anim_frame = 1;
				Heart2.anim_frame = 1;
				Heart1.anim_frame = 1;
				GameStart = false;
			}
			if(Hunger == 5){
				Food5.anim_frame = 0;
				Food4.anim_frame = 0;
				Food3.anim_frame = 0;
				Food2.anim_frame = 0;
				Food1.anim_frame = 0;
			}
			else if(Hunger == 4){
				Food5.anim_frame = 1;
				Food4.anim_frame = 0;
				Food3.anim_frame = 0;
				Food2.anim_frame = 0;
				Food1.anim_frame = 0;
			}
			else if(Hunger == 3){
				Food5.anim_frame = 1;
				Food4.anim_frame = 1;
				Food3.anim_frame = 0;
				Food2.anim_frame = 0;
				Food1.anim_frame = 0;
			}
			else if(Hunger == 2){
				Food5.anim_frame = 1;
				Food4.anim_frame = 1;
				Food3.anim_frame = 1;
				Food2.anim_frame = 0;
				Food1.anim_frame = 0;
			}
			else if(Hunger == 1){
				Food5.anim_frame = 1;
				Food4.anim_frame = 1;
				Food3.anim_frame = 1;
				Food2.anim_frame = 1;
				Food1.anim_frame = 0;
			}
			else if(Hunger <= 0){
				Food5.anim_frame = 1;
				Food4.anim_frame = 1;
				Food3.anim_frame = 1;
				Food2.anim_frame = 1;
				Food1.anim_frame = 1;
			}
			if(isSick == true){
				Effect.anim_frame = 1;
			}
			else{
				Effect.anim_frame = 0;
			}

			TimeBetweenFramesDone++;
			if(TimeBetweenFramesDone >= TimeBetweenFrames){
				TimeBetweenFramesDone = 0;
				Pet.anim_frame++;
				if(Pet.anim_frame == FRAMES_PER_ANIMATION){
					Pet.anim_frame = 0;
				}
			}

			//Animate Sprites.
			animate32(&Pet);
			animate32(&Food1);
			animate32(&Food2);
			animate32(&Food3);
			animate32(&Food4);
			animate32(&Food5);

			animate16(&Heart1);
			animate16(&Heart2);
			animate16(&Heart3);
			animate16(&Heart4);
			animate16(&Heart5);

			animate16(&Food1);
			animate16(&Food2);
			animate16(&Food3);
			animate16(&Food4);
			animate16(&Food5);

			animate16(&Effect);

			animate8(&Ones);
			animate8(&Tens);
			animate8(&Hunds);

			if(isInBattle == false){
				//Set the sprites
				oamSet(&oamMain, 0, Pet.Xpos, Pet.Ypos, 0, Pet.palette, SpriteSize_32x32, SpriteColorFormat_256Color, 
					Pet.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 0, Heart1.Xpos, Heart1.Ypos, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
					Heart1.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 1, Heart2.Xpos, Heart2.Ypos, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
					Heart2.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 2, Heart3.Xpos, Heart3.Ypos, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
					Heart3.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 3, Heart4.Xpos, Heart4.Ypos, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
					Heart4.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 4, Heart5.Xpos, Heart5.Ypos, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
					Heart5.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 5, Food1.Xpos, Food1.Ypos, 0, 1, SpriteSize_16x16, SpriteColorFormat_256Color, 
					Food1.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 6, Food2.Xpos, Food2.Ypos, 0, 1, SpriteSize_16x16, SpriteColorFormat_256Color, 
					Food2.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 7, Food3.Xpos, Food3.Ypos, 0, 1, SpriteSize_16x16, SpriteColorFormat_256Color, 
					Food3.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 8, Food4.Xpos, Food4.Ypos, 0, 1, SpriteSize_16x16, SpriteColorFormat_256Color, 
					Food4.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 9, Food5.Xpos, Food5.Ypos, 0, 1, SpriteSize_16x16, SpriteColorFormat_256Color, 
					Food5.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 10, Effect.Xpos, Effect.Ypos, 0, 2, SpriteSize_16x16, SpriteColorFormat_256Color, 
					Effect.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 11, 236, 25, 0, 3, SpriteSize_8x8, SpriteColorFormat_256Color, 
					Ones.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 12, 228, 25, 0, 3, SpriteSize_8x8, SpriteColorFormat_256Color, 
					Tens.sprite_gfx_mem, -1, false, false, false, false, false);
				oamSet(&oamSub, 13, 220, 25, 0, 3, SpriteSize_8x8, SpriteColorFormat_256Color, 
					Hunds.sprite_gfx_mem, -1, false, false, false, false, false);

				//Increase the variables.
				HungerFrame++;
				MovementFrame++;
			}
		}

		//If on the title screen.
		if(GameStart == false){
			if(isInBattle == true){
				int EnemyChoice = 0;

				if(pressed & KEY_TOUCH){
					TouchBox.Xpos = touch.px;
					TouchBox.Ypos = touch.py;
				}
				else{
					TouchBox.Xpos = -20;
					TouchBox.Ypos = -20;
				}

				if(battleState == 0){ // Player's Turn
					bool hit = false;
					hit = CollisionCheck(TouchBox, RockButton);
					if(hit == true){
						PlayersChoice = 1;
					}
					else{
						hit = CollisionCheck(TouchBox, PaperButton);
						if(hit == true){
							PlayersChoice = 2;
						}
						else{
							hit = CollisionCheck(TouchBox, ScissorsButton);
							if(hit == true){
								PlayersChoice = 3;
							}
						}
					}

					if(PlayersChoice != 0){
						if(PlayersChoice == 1){
							PlayerAttack.anim_frame = 0;	
						}
						else if(PlayersChoice == 2){
							PlayerAttack.anim_frame = 1;
						}
						else{
							PlayerAttack.anim_frame = 2;
						}
						battleState = 1;
						EnemyChoice = 0;
					}
				}
				else if(battleState == 1){ // Enemy's Turn
					if(EnemyChoice == 0){
						EnemyChoice = rand() % 3;
						//EnemyChoice = 2;
					}

					if(EnemyChoice == 1){
						EnemyAttack.anim_frame = 0;	
					}
					else if(EnemyChoice == 2){
						EnemyAttack.anim_frame = 1;
					}
					else{
						EnemyAttack.anim_frame = 2;
					}
			
					if(EnemyChoice == 1){
						if(PlayersChoice != 1){
							if(PlayersChoice == 2){
								BattleEnemyHealth--;
								battleState = 2;
							}
							else{
								BattlePlayerHealth--;
								battleState = 3;
							}
						}
						else{
							battleState = 0;
							PlayersChoice = 0;
							EnemyChoice = 0;
						}

					}
					else if(EnemyChoice == 2){
						if(PlayersChoice != 2){
							if(PlayersChoice == 3){
								BattleEnemyHealth--;
								battleState = 2;
							}
							else{
								BattlePlayerHealth--;
								battleState = 3;
							}
						}
						else{
							battleState = 0;
							PlayersChoice = 0;
							EnemyChoice = 0;
						}
					}
					else if(EnemyChoice == 3){
						if(PlayersChoice != 3){
							if(PlayersChoice == 1){
								BattleEnemyHealth--;
								battleState = 2;
							}
							else{
								BattlePlayerHealth--;
								battleState = 3;
							}
						}
						else{
							battleState = 0;
							PlayersChoice = 0;
							EnemyChoice = 0;
						}
					}
				}
				else if(battleState == 2){ // Player Move
					if(doneMove == false){
						doneMove = MoveActor(Enem.Xpos - 32, Pet.Ypos, &Pet);
					}

					if(doneMove == true){
						doneMoveBack = MoveActor(25, Pet.Ypos, &Pet);
					}

					if(doneMoveBack == true){
						Pet.state = W_Walk_Right;
						if(BattleEnemyHealth <= 0){
							battleState = 4;
						}
						else{
							battleState = 0;
							PlayersChoice = 0;
							EnemyChoice = 0;
							doneMove = false;
							doneMoveBack = false;
						}
					}
				}
				else if(battleState == 3){ // Enemy Move
					if(doneMove == false){
						doneMove = MoveActor(Pet.Xpos + 32, Enem.Ypos, &Enem);
					}

					if(doneMove == true){
						doneMoveBack = MoveActor(198, Enem.Ypos, &Enem);
					}

					if(doneMoveBack == true){
						Enem.state = W_Walk_Left;
						if(BattlePlayerHealth <= 0){
							battleState = 5;
						}
						else{
							battleState = 0;
							PlayersChoice = 0;
							EnemyChoice = 0;
							doneMove = false;
							doneMoveBack = false;
						}
					}
				}
				else if(battleState == 4){ // Player Won
					if(Hunger > 0){
						Hunger--;
					}

					int randCoins = rand() % 100;
					if(randCoins < 51){
						Coins += 5;
					}
					else{
						Coins += 10;
					}

					isInBattle = false;
					GameStart = true;
					BattleMenuOpen = false;

					bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
					dmaCopy(GameBitmap, bgGetGfxPtr(bg3), 256*192);
					dmaCopy(GamePal, BG_PALETTE, 256*2);

					bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
					dmaCopy(BottomGameBitmap, bgGetGfxPtr(bg3), 256*192);
					dmaCopy(BottomGamePal, BG_PALETTE_SUB, 256*2);

					oamClear(&oamMain, 0, 127);
				}
				else if(battleState == 5){ // Player Lost
					if(Hunger > 0){
						Hunger--;
					}
					if(Health > 0){
						Health--;
					}

					isInBattle = false;
					GameStart = true;
					BattleMenuOpen = false;

					bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
					dmaCopy(GameBitmap, bgGetGfxPtr(bg3), 256*192);
					dmaCopy(GamePal, BG_PALETTE, 256*2);

					bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
					dmaCopy(BottomGameBitmap, bgGetGfxPtr(bg3), 256*192);
					dmaCopy(BottomGamePal, BG_PALETTE_SUB, 256*2);

					oamClear(&oamMain, 0, 127);
				}

				TimeBetweenFramesDone++;
				if(TimeBetweenFramesDone >= TimeBetweenFrames){
					TimeBetweenFramesDone = 0;
					Pet.anim_frame++;
					if(Pet.anim_frame == FRAMES_PER_ANIMATION){
						Pet.anim_frame = 0;
					}
				}

				int TimeBetweenFramesEnemyDone;
				TimeBetweenFramesEnemyDone++;
				if(TimeBetweenFramesEnemyDone >= TimeBetweenFrames){
					TimeBetweenFramesEnemyDone = 0;
					Enem.anim_frame++;
					if(Enem.anim_frame == FRAMES_PER_ANIMATION){
						Enem.anim_frame = 0;
					}
				}


				animate32(&Pet);
				animate32(&Enem);
				animate16(&PlayerAttack);
				animate16(&EnemyAttack);

				oamSet(&oamMain, 0, Pet.Xpos, Pet.Ypos, 0, Pet.palette, SpriteSize_32x32, SpriteColorFormat_256Color, 
					Pet.sprite_gfx_mem, -1, false, false, false, false, false);
				if(battleState != 4 && battleState != 5){
					oamSet(&oamMain, 1, Enem.Xpos, Enem.Ypos, 0, 1, SpriteSize_32x32, SpriteColorFormat_256Color, 
						Enem.sprite_gfx_mem, -1, false, false, false, false, false);
					oamSet(&oamMain, 2, 16, 16, 0, 2, SpriteSize_16x16, SpriteColorFormat_256Color, 
						PlayerAttack.sprite_gfx_mem, -1, false, false, false, false, false);
					oamSet(&oamMain, 3, 256 - 32, 16, 0, 2, SpriteSize_16x16, SpriteColorFormat_256Color, 
						EnemyAttack.sprite_gfx_mem, -1, false, false, false, false, false);
				}
			}
			else{
				//If you press start.
				if(pressed & KEY_START){
					//Change the backgrounds.
					bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
					dmaCopy(GameBitmap, bgGetGfxPtr(bg3), 256*192);
					dmaCopy(GamePal, BG_PALETTE, 256*2);

					bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
					dmaCopy(BottomGameBitmap, bgGetGfxPtr(bg3), 256*192);
					dmaCopy(BottomGamePal, BG_PALETTE_SUB, 256*2);
					//Start the game.
					GameStart = true;
				}
			}
		}

		//Update the OAMs.
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}
}

void Feed(int Num){
	if(Health != 0 && Health <= MaxHealth){
		
		if(Num == 1){
			if(Coins >= 5){
				if(Hunger < 5){
					Hunger++;
					HungerFrame = -1;
					Coins -= 5;
				}
			}
		}
		else{
			if(Coins >= 10){
				if(Hunger < 4){
					Hunger += 2;
					HungerFrame = -1;
					Coins -= 10;
				}
				else if(Hunger < MaxHealth){
					Hunger++;
					HungerFrame = -1;
					Coins -= 10;
				}
			}
		}
	}
}

void Heal(int Num){
	if(Health > 0){
		if(isSick == true){
			if(Coins >= Num * 10){
				int random = rand() % 100;
				if(Num == 1){
					if(random < 21){
						isSick = false;
						SickFrames = 0;
					}
				}
				else if(Num == 2){
					if(random < 51){
						isSick = false;
						SickFrames = 0;
					}
				}
				else{
					if(random < 81){
						isSick = false;
						SickFrames = 0;
					}
				}
				Coins -= Num * 10;
			}
		}
	}
}

void openOfflineBattle(){
	//Clear all sprites from the bottom screen
	oamClear(&oamSub, 0, 127);

	//Change the backgrounds.
	bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(GameBitmap, bgGetGfxPtr(bg3), 256*192);
	dmaCopy(GamePal, BG_PALETTE, 256*2);

	bg3 = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(BattleBottomBitmap, bgGetGfxPtr(bg3), 256*192);
	dmaCopy(BattleBottomPal, BG_PALETTE_SUB, 256*2);

	//Start Battling
	BattlePlayerHealth = 5;
	BattleEnemyHealth = 5;
	isInBattle = true;
	GameStart = false;
	battleState = 0;
	doneMove = false;
	doneMoveBack = false;
	PlayersChoice = 0;
}

void openLocalBattle(){
	oamClear(&oamSub, 0, 127);
	isInBattle = true;
	GameStart = false;
	battleState = 0;
}

void openOnlineBattle(){
	oamClear(&oamSub, 0, 127);
	isInBattle = true;
	GameStart = false;
	battleState = 0;
}
