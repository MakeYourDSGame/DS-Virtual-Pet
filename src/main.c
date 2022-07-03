#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <maxmod9.h>

SpriteEntry OAMCopy[128];

#include <Start.h>
#include <Game.h>
#include <Pet16.h>
#include <Enemy.h>
#include <Hearts.h>
#include <Hunger.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#include "events.c"

volatile int HungerFrame = 0;
volatile int MovementFrame = 0;
int Hunger = 5;
int Health = 5;

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
    videoSetMode(  MODE_0_2D | 
                   DISPLAY_SPR_ACTIVE |		//turn on sprites
                   DISPLAY_BG0_ACTIVE |		//turn on background 0
                   DISPLAY_SPR_1D			//this is used when in tile mode
                    );

	videoSetMode(MODE_5_2D);
	videoSetModeSub(MODE_0_2D);


	//Setting the banks. Does not include the F bank
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	//Set up oam
	oamInit(&oamMain, SpriteMapping_1D_128, true);
	oamInit(&oamSub, SpriteMapping_1D_128, false);

	//Set bank F
	vramSetBankF(VRAM_F_LCD);
	init16(&Pet, (u8*)Pet16Tiles);
	dmaCopy(Pet16Pal, &VRAM_F_EXT_SPR_PALETTE[0][0],Pet16PalLen);

	init32(&Food1, (u8*)EnemyTiles);
	dmaCopy(EnemyPal, &VRAM_F_EXT_SPR_PALETTE[1][0],EnemyPalLen);

	init16(&Heart1, (u8*)HeartsTiles);
	dmaCopy(HeartsPal, &VRAM_F_EXT_SPR_PALETTE[2][0],HeartsPalLen);
	init16(&Heart2, (u8*)HeartsTiles);
	dmaCopy(HeartsPal, &VRAM_F_EXT_SPR_PALETTE[2][0],HeartsPalLen);
	init16(&Heart3, (u8*)HeartsTiles);
	dmaCopy(HeartsPal, &VRAM_F_EXT_SPR_PALETTE[2][0],HeartsPalLen);
	init16(&Heart4, (u8*)HeartsTiles);
	dmaCopy(HeartsPal, &VRAM_F_EXT_SPR_PALETTE[2][0],HeartsPalLen);
	init16(&Heart5, (u8*)HeartsTiles);
	dmaCopy(HeartsPal, &VRAM_F_EXT_SPR_PALETTE[2][0],HeartsPalLen);

	init16(&Food1, (u8*)HungerTiles);
	dmaCopy(HungerPal, &VRAM_F_EXT_SPR_PALETTE[1][0],HungerPalLen);
	init16(&Food2, (u8*)HungerTiles);
	dmaCopy(HungerPal, &VRAM_F_EXT_SPR_PALETTE[1][0],HungerPalLen);
	init16(&Food3, (u8*)HungerTiles);
	dmaCopy(HungerPal, &VRAM_F_EXT_SPR_PALETTE[1][0],HungerPalLen);
	init16(&Food4, (u8*)HungerTiles);
	dmaCopy(HungerPal, &VRAM_F_EXT_SPR_PALETTE[1][0],HungerPalLen);
	init16(&Food5, (u8*)HungerTiles);
	dmaCopy(HungerPal, &VRAM_F_EXT_SPR_PALETTE[1][0],HungerPalLen);

	vramSetBankF(VRAM_F_SPRITE_EXT_PALETTE);

	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	dmaCopy(StartBitmap, bgGetGfxPtr(bg3), 256*192);
	dmaCopy(StartPal, BG_PALETTE, 256*2);
		
	//Make the bottom screen a blue color
	setBackdropColorSub(5555000000009999);
	
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

	consoleDemoInit();

	//Loops every frame
	while(1) {

		swiWaitForVBlank();

		scanKeys();

		touchRead(&touch);
		
		int pressed = keysDown();
		int held = keysHeld();
		int released = keysUp();

		if(Health == 5){
			Heart5.anim_frame = 0;
			Heart4.anim_frame = 0;
			Heart3.anim_frame = 0;
			Heart2.anim_frame = 0;
			Heart1.anim_frame = 0;
		}
		if(Health == 4){
			Heart5.anim_frame = 1;
			Heart4.anim_frame = 0;
			Heart3.anim_frame = 0;
			Heart2.anim_frame = 0;
			Heart1.anim_frame = 0;
		}
		if(Health == 3){
			Heart5.anim_frame = 1;
			Heart4.anim_frame = 1;
			Heart3.anim_frame = 0;
			Heart2.anim_frame = 0;
			Heart1.anim_frame = 0;
		}
		if(Health == 2){
			Heart5.anim_frame = 1;
			Heart4.anim_frame = 1;
			Heart3.anim_frame = 1;
			Heart2.anim_frame = 0;
			Heart1.anim_frame = 0;
		}
		if(Health == 1){
			Heart5.anim_frame = 1;
			Heart4.anim_frame = 1;
			Heart3.anim_frame = 1;
			Heart2.anim_frame = 1;
			Heart1.anim_frame = 0;
		}
		if(Health <= 0){
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
		if(Hunger == 4){
			Food5.anim_frame = 1;
			Food4.anim_frame = 0;
			Food3.anim_frame = 0;
			Food2.anim_frame = 0;
			Food1.anim_frame = 0;
		}
		if(Hunger == 3){
			Food5.anim_frame = 1;
			Food4.anim_frame = 1;
			Food3.anim_frame = 0;
			Food2.anim_frame = 0;
			Food1.anim_frame = 0;
		}
		if(Hunger == 2){
			Food5.anim_frame = 1;
			Food4.anim_frame = 1;
			Food3.anim_frame = 1;
			Food2.anim_frame = 0;
			Food1.anim_frame = 0;
		}
		if(Hunger == 1){
			Food5.anim_frame = 1;
			Food4.anim_frame = 1;
			Food3.anim_frame = 1;
			Food2.anim_frame = 1;
			Food1.anim_frame = 0;
		}
		if(Hunger <= 0){
			Food5.anim_frame = 1;
			Food4.anim_frame = 1;
			Food3.anim_frame = 1;
			Food2.anim_frame = 1;
			Food1.anim_frame = 1;
			//GameStart = false;
		}

		PetBox.Xpos = Pet.Xpos + PetBox.OffsetX;
		PetBox.Ypos = Pet.Ypos + PetBox.OffsetY;

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

		bool isGoing;
		int goingX;

		if(GameStart == true){
			oamClear(&oamMain, 0, 127);

			//Movement
			if(MovementFrame >= 100){
				MovementFrame = -1;
				isGoing = false;
			}

			//Health & Hunger
			if(HungerFrame >= 500){
				if(Hunger == 0){
					if(HungerFrame >= 700){
						Health--;
						HungerFrame = -1;
					}
				}
				else{
					Hunger--;
					HungerFrame = -1;
				}
			}

			if(isGoing == false){
				goingX = rand() % 240;
				isGoing = true;
			}
			else{
				Pet = MoveActor(goingX, Pet.Ypos, Pet);
			}

			oamSet(&oamMain, 0, Pet.Xpos, Pet.Ypos, 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color, 
				Pet.sprite_gfx_mem, -1, false, false, false, false, false);
			oamSet(&oamMain, 1, Heart1.Xpos, Heart1.Ypos, 0, 2, SpriteSize_16x16, SpriteColorFormat_256Color, 
				Heart1.sprite_gfx_mem, -1, false, false, false, false, false);
			oamSet(&oamMain, 2, Heart2.Xpos, Heart2.Ypos, 0, 2, SpriteSize_16x16, SpriteColorFormat_256Color, 
				Heart2.sprite_gfx_mem, -1, false, false, false, false, false);
			oamSet(&oamMain, 3, Heart3.Xpos, Heart3.Ypos, 0, 2, SpriteSize_16x16, SpriteColorFormat_256Color, 
				Heart3.sprite_gfx_mem, -1, false, false, false, false, false);
			oamSet(&oamMain, 4, Heart4.Xpos, Heart4.Ypos, 0, 2, SpriteSize_16x16, SpriteColorFormat_256Color, 
				Heart4.sprite_gfx_mem, -1, false, false, false, false, false);
			oamSet(&oamMain, 5, Heart5.Xpos, Heart5.Ypos, 0, 2, SpriteSize_16x16, SpriteColorFormat_256Color, 
				Heart5.sprite_gfx_mem, -1, false, false, false, false, false);
			oamSet(&oamMain, 6, Food1.Xpos, Food1.Ypos, 0, 1, SpriteSize_16x16, SpriteColorFormat_256Color, 
				Food1.sprite_gfx_mem, -1, false, false, false, false, false);
			oamSet(&oamMain, 7, Food2.Xpos, Food2.Ypos, 0, 1, SpriteSize_16x16, SpriteColorFormat_256Color, 
				Food2.sprite_gfx_mem, -1, false, false, false, false, false);
			oamSet(&oamMain, 8, Food3.Xpos, Food3.Ypos, 0, 1, SpriteSize_16x16, SpriteColorFormat_256Color, 
				Food3.sprite_gfx_mem, -1, false, false, false, false, false);
			oamSet(&oamMain, 9, Food4.Xpos, Food4.Ypos, 0, 1, SpriteSize_16x16, SpriteColorFormat_256Color, 
				Food4.sprite_gfx_mem, -1, false, false, false, false, false);
			oamSet(&oamMain, 10, Food5.Xpos, Food5.Ypos, 0, 1, SpriteSize_16x16, SpriteColorFormat_256Color, 
				Food5.sprite_gfx_mem, -1, false, false, false, false, false);

			HungerFrame++;
			MovementFrame++;
		}
		if(GameStart == false){
			if(pressed & KEY_START){
				bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
				dmaCopy(GameBitmap, bgGetGfxPtr(bg3), 256*192);
				dmaCopy(GamePal, BG_PALETTE, 256*2);
				GameStart = true;
			}
		}

		oamUpdate(&oamMain);
	}
}