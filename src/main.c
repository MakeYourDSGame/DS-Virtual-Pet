#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <maxmod9.h>

SpriteEntry OAMCopy[128];

#include <Start.h>
#include <BottomStart.h>
#include <Game.h>
#include <BottomGame.h>
#include <Pet16.h>
#include <Enemy.h>
#include <Hearts.h>
#include <Hunger.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#include "events.c"

volatile int HungerFrame = 0;
volatile int MovementFrame = 0;
int MovementFrameNum = 0;
int Hunger = 5;
int Health = 5;
int MaxHealth = 5;
int GetHealth = 0;

enum { CONTINUOUS, SINGLE } TouchType = CONTINUOUS;

void Vblank() {
	HungerFrame++;
}

//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------

	//Create Foods
	Sprite Pet = {0,0};
	Pet.Xpos = 50;
	Pet.Ypos = 134;

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
	
	//Set Up Collision Boxes
	Box PetBox;
	PetBox.OffsetX = 0;
	PetBox.OffsetY = 0; //26
	PetBox.Xpos = PetBox.Xpos + PetBox.OffsetX;
	PetBox.Ypos = PetBox.Ypos + PetBox.OffsetY;
	PetBox.SizeX = 16;
	PetBox.SizeY = 16;

	bool GameStart = false;

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
	init16(&Pet, (u8*)Pet16Tiles);
	dmaCopy(Pet16Pal, &VRAM_F_EXT_SPR_PALETTE[0][0],Pet16PalLen);
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
	//Set I Bank.
	vramSetBankI(VRAM_I_SUB_SPRITE_EXT_PALETTE);

	//Title screen background.
	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
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

	//consoleDemoInit();

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
			//oamClear(&oamMain, 0, 127);

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

			//If you can move, move. Else, choose random spot to move to.
			if(isGoing == false){
				//Get random X position to move to.
				goingX = rand() % 240;
				//Can start moving.
				isGoing = true;
			}
			else{
				//Move the "Pet".
				Pet = MoveActor(goingX, Pet.Ypos, Pet);
			}

			//WIP feeding system.
			if(pressed & KEY_A){
				if(Health != 0 && Health != MaxHealth){
					Hunger++;
					HungerFrame = -1;
				}
			}

			//---------------------------------------------------------------------------------------------------------------------------

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
				//GameStart = false;
			}

			//Animate Sprites.
			animate16(&Pet);
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

			//Set the sprites
			oamSet(&oamMain, 0, Pet.Xpos, Pet.Ypos, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
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

			//Increase the variables.
			HungerFrame++;
			MovementFrame++;
		}

		//If on the title screen.
		if(GameStart == false){
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

		//Update the OAMs.
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}
}