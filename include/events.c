#include <stdio.h>
#include <nds.h>

//Animation Frame Variables
#define FRAMES_PER_ANIMATION 2
//Animation States
enum SpriteState {W_Walk_Left = 0, W_Walk_Right = 1, W_Walk_Up = 2, W_Walk_Down = 3};

//Objects
typedef struct 
{
	int Xpos;
	int Ypos;

	int AnimationFrames;

	u16* sprite_gfx_mem;
	u8*  frame_gfx;

	int state;
	int anim_frame;
}Sprite;

typedef struct
{
	int Xpos;
	int Ypos;

	int SizeX;
	int SizeY;

	int OffsetX;
	int OffsetY;
}Box;

bool MoveActor(int Xpos, int Ypos, Sprite *object){
	bool Go_Right;
	bool Go_Left;
	bool Go_Up;
	bool Go_Down;

	Go_Right = false;
	Go_Left = false;
	Go_Up = false;
	Go_Down = false;

	bool DoneMoving = false;

	DoneMoving = false;
	
	if(object->Xpos < Xpos){
		object->state = W_Walk_Right;
		Go_Right = true;
	}
	else if(object->Xpos > Xpos){
		object->state = W_Walk_Left;
		Go_Left = true;
	}

	if(object->Ypos < Ypos){
		object->state = W_Walk_Down;
		Go_Down = true;
	}
	else if(object->Ypos > Ypos){
		object->state = W_Walk_Up;
		Go_Up = true;
	}
	else{
		DoneMoving = true;
	}

	if(Go_Right){
		object->Xpos++;
	}
	else if(Go_Left){
		object->Xpos--;
	}

	if(Go_Down){
		object->Ypos++;
	}
	else if(Go_Up){
		object->Ypos--;
	}

	return DoneMoving;
}

Sprite MoveActorSmooth(int Xpos, int Ypos, Sprite object){
	bool Go_Right;
	bool Go_Left;
	bool Go_Up;
	bool Go_Down;

	Go_Right = false;
	Go_Left = false;
	Go_Up = false;
	Go_Down = false;
	
	if(object.Xpos < Xpos){
		object.state = W_Walk_Right;
		Go_Right = true;
	}
	else if(object.Xpos > Xpos){
		object.state = W_Walk_Left;
		Go_Left = true;
	}

	if(object.Ypos < Ypos){
		object.state = W_Walk_Down;
		Go_Down = true;
	}
	else if(object.Ypos > Ypos){
		object.state = W_Walk_Up;
		Go_Up = true;
	}

	if(Go_Right){
		object.Xpos++;
	}
	else if(Go_Left){
		object.Xpos--;
	}

	if(Go_Down){
		object.Ypos++;
	}
	else if(Go_Up){
		object.Ypos--;
	}

	return object;
}

void animate64(Sprite *sprite)
{
	int frame = sprite->anim_frame + sprite->state * FRAMES_PER_ANIMATION;

	u8* offset = sprite->frame_gfx + frame * 64*64;

	dmaCopy(offset, sprite->sprite_gfx_mem, 64*64);
}

void init64(Sprite *sprite, u8* gfx)
{
	sprite->sprite_gfx_mem = oamAllocateGfx(&oamMain, SpriteSize_64x64, SpriteColorFormat_256Color);
	
	sprite->frame_gfx = (u8*)gfx;
}

void init64Sub(Sprite *sprite, u8* gfx)
{
	sprite->sprite_gfx_mem = oamAllocateGfx(&oamSub, SpriteSize_64x64, SpriteColorFormat_256Color);
	
	sprite->frame_gfx = (u8*)gfx;
}

void animate32(Sprite *sprite)
{
	int frame = sprite->anim_frame + sprite->state * FRAMES_PER_ANIMATION;

	u8* offset = sprite->frame_gfx + frame * 32*32;

	dmaCopy(offset, sprite->sprite_gfx_mem, 32*32);
}

void init32(Sprite *sprite, u8* gfx)
{
	sprite->sprite_gfx_mem = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	
	sprite->frame_gfx = (u8*)gfx;
}

void init32Sub(Sprite *sprite, u8* gfx)
{
	sprite->sprite_gfx_mem = oamAllocateGfx(&oamSub, SpriteSize_32x32, SpriteColorFormat_256Color);
	
	sprite->frame_gfx = (u8*)gfx;
}

void animate16(Sprite *sprite)
{
	int frame = sprite->anim_frame + sprite->state * FRAMES_PER_ANIMATION;

	u8* offset = sprite->frame_gfx + frame * 16*16;

	dmaCopy(offset, sprite->sprite_gfx_mem, 16*16);
}

void init16(Sprite *sprite, u8* gfx)
{
	sprite->sprite_gfx_mem = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
	
	sprite->frame_gfx = (u8*)gfx;
}

void init16Sub(Sprite *sprite, u8* gfx)
{
	sprite->sprite_gfx_mem = oamAllocateGfx(&oamSub, SpriteSize_16x16, SpriteColorFormat_256Color);
	
	sprite->frame_gfx = (u8*)gfx;
}

void animate8(Sprite *sprite)
{
	int frame = sprite->anim_frame + sprite->state * FRAMES_PER_ANIMATION;

	u8* offset = sprite->frame_gfx + frame * 8*8;

	dmaCopy(offset, sprite->sprite_gfx_mem, 8*8);
}

void init8(Sprite *sprite, u8* gfx)
{
	sprite->sprite_gfx_mem = oamAllocateGfx(&oamMain, SpriteSize_8x8, SpriteColorFormat_256Color);
	
	sprite->frame_gfx = (u8*)gfx;
}

void init8Sub(Sprite *sprite, u8* gfx)
{
	sprite->sprite_gfx_mem = oamAllocateGfx(&oamSub, SpriteSize_8x8, SpriteColorFormat_256Color);
	
	sprite->frame_gfx = (u8*)gfx;
}

bool CollisionCheck(Box r1, Box r2){
	return(r1.Xpos < r2.Xpos + r2.SizeX && r1.Xpos + r1.SizeX > r2.Xpos && r1.Ypos < r2.Ypos + r2.SizeY && r1.Ypos + r1.SizeY > r2.Ypos);
}

void ScrollBackground(int BackgroundId, int BackgroundWidth, int BackgroundHeight, int X, int Y, bool ScrollWithDPad){
	int keys;

	if(ScrollWithDPad){
		keys = keysHeld();

      	if(keys & KEY_UP) X--;
      	if(keys & KEY_DOWN) X++;
      	if(keys & KEY_LEFT) X--;
      	if(keys & KEY_RIGHT) X++;

      	if(X < 0) X = 0;
      	if(X >= BackgroundWidth - 256) X = BackgroundWidth - 1 - 256;
      	if(Y < 0) Y = 0;
      	if(Y >= BackgroundHeight - 192) Y = BackgroundHeight - 1 - 192;

      	swiWaitForVBlank();

      	bgSetScroll(BackgroundId, X, Y);

	  	bgUpdate();
	}
	else{

	}
}