//---------------------------------------------------------------------------------
// Copyright Steven Wan 2009-2010
// E-mail:   Steven Wan@163.com
// Project:			SGUI(Simple GUI)
// File:			sguibase.cpp
// Description:		SGUI for Power user elect energy acquire system;unsupport multi_thread
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef WIN32
#include "sguibase.h"
#include "osrelate.h"
#include "htrace.h"
//
//
//
#define SGUI_LCD_WIDTH		192
#define SGUI_LCD_HEIGHT		64
#define SGUI_LCD_BPP		1
#define SGUI_LCD_LINE	(SGUI_LCD_WIDTH * SGUI_LCD_BPP / 8)
#define SGUI_LCD_SIZE	(SGUI_LCD_LINE * SGUI_LCD_HEIGHT)

#define SGUI_LCD_WIDTH_5000D		320
#define SGUI_LCD_HEIGHT_5000D		240
#define SGUI_LCD_BPP_5000D		1
#define SGUI_LCD_LINE_5000D	(SGUI_LCD_WIDTH_5000D * SGUI_LCD_BPP_5000D / 8)
#define SGUI_LCD_SIZE_5000D	(SGUI_LCD_LINE_5000D * SGUI_LCD_HEIGHT_5000D)


#define SGUI_LCD_WIDTH_5000C		160
#define SGUI_LCD_HEIGHT_5000C		160
#define SGUI_LCD_BPP_5000C		1
#define SGUI_LCD_LINE_5000C	(SGUI_LCD_WIDTH_5000C * SGUI_LCD_BPP_5000C / 8)
#define SGUI_LCD_SIZE_5000C	(SGUI_LCD_LINE_5000C * SGUI_LCD_HEIGHT_5000C)

//键盘按键
struct KEY_VALUE 
{
	int button;
	int right_cnt[4];
	int left_cnt[4];
};
/////
//
//SGUI全局资源
//
char g_SGUI_ResourcePath[CFG_PATH_FILE_MAX_LEN] = "./"; //显示资源路径
int g_SGUI_ScreenDirection = 0; //屏幕方向
char SGUI_UsrBuf[SGUI_LCD_SIZE_5000D]; //用户显存 [用最大屏幕的]
char *SGUI_fb = NULL; //屏幕显存

C_SguiDriverBase g_SguiDriverBaseDefault;
C_SguiDriverEac5000D g_SguiDriverEac5000D;
C_SguiDriverEac5000C g_SguiDriverEac5000C;
C_SguiDriverCq6100 g_SguiDriverCq6100;
C_SguiDriverBase *pSguiDriverBase = &g_SguiDriverBaseDefault;

//
//显示屏幕旋转函数
//
static const unsigned char SGUI_BitReverse[256] =
{
	'\x00',	'\x80',	'\x40',	'\xc0',	'\x20',	'\xa0',	'\x60',	'\xe0',
	'\x10',	'\x90',	'\x50',	'\xd0',	'\x30',	'\xb0',	'\x70',	'\xf0',
	'\x08',	'\x88',	'\x48',	'\xc8',	'\x28',	'\xa8',	'\x68',	'\xe8',
	'\x18',	'\x98',	'\x58',	'\xd8',	'\x38',	'\xb8',	'\x78',	'\xf8',
	'\x04',	'\x84',	'\x44',	'\xc4',	'\x24',	'\xa4',	'\x64',	'\xe4',
	'\x14',	'\x94',	'\x54',	'\xd4',	'\x34',	'\xb4',	'\x74',	'\xf4',
	'\x0c',	'\x8c',	'\x4c',	'\xcc',	'\x2c',	'\xac',	'\x6c',	'\xec',
	'\x1c',	'\x9c',	'\x5c',	'\xdc',	'\x3c',	'\xbc',	'\x7c',	'\xfc',
	'\x02',	'\x82',	'\x42',	'\xc2',	'\x22',	'\xa2',	'\x62',	'\xe2',
	'\x12',	'\x92',	'\x52',	'\xd2',	'\x32',	'\xb2',	'\x72',	'\xf2',
	'\x0a',	'\x8a',	'\x4a',	'\xca',	'\x2a',	'\xaa',	'\x6a',	'\xea',
	'\x1a',	'\x9a',	'\x5a',	'\xda',	'\x3a',	'\xba',	'\x7a',	'\xfa',
	'\x06',	'\x86',	'\x46',	'\xc6',	'\x26',	'\xa6',	'\x66',	'\xe6',
	'\x16',	'\x96',	'\x56',	'\xd6',	'\x36',	'\xb6',	'\x76',	'\xf6',
	'\x0e',	'\x8e',	'\x4e',	'\xce',	'\x2e',	'\xae',	'\x6e',	'\xee',
	'\x1e',	'\x9e',	'\x5e',	'\xde',	'\x3e',	'\xbe',	'\x7e',	'\xfe',
	'\x01',	'\x81',	'\x41',	'\xc1',	'\x21',	'\xa1',	'\x61',	'\xe1',
	'\x11',	'\x91',	'\x51',	'\xd1',	'\x31',	'\xb1',	'\x71',	'\xf1',
	'\x09',	'\x89',	'\x49',	'\xc9',	'\x29',	'\xa9',	'\x69',	'\xe9',
	'\x19',	'\x99',	'\x59',	'\xd9',	'\x39',	'\xb9',	'\x79',	'\xf9',
	'\x05',	'\x85',	'\x45',	'\xc5',	'\x25',	'\xa5',	'\x65',	'\xe5',
	'\x15',	'\x95',	'\x55',	'\xd5',	'\x35',	'\xb5',	'\x75',	'\xf5',
	'\x0d',	'\x8d',	'\x4d',	'\xcd',	'\x2d',	'\xad',	'\x6d',	'\xed',
	'\x1d',	'\x9d',	'\x5d',	'\xdd',	'\x3d',	'\xbd',	'\x7d',	'\xfd',
	'\x03',	'\x83',	'\x43',	'\xc3',	'\x23',	'\xa3',	'\x63',	'\xe3',
	'\x13',	'\x93',	'\x53',	'\xd3',	'\x33',	'\xb3',	'\x73',	'\xf3',
	'\x0b',	'\x8b',	'\x4b',	'\xcb',	'\x2b',	'\xab',	'\x6b',	'\xeb',
	'\x1b',	'\x9b',	'\x5b',	'\xdb',	'\x3b',	'\xbb',	'\x7b',	'\xfb',
	'\x07',	'\x87',	'\x47',	'\xc7',	'\x27',	'\xa7',	'\x67',	'\xe7',
	'\x17',	'\x97',	'\x57',	'\xd7',	'\x37',	'\xb7',	'\x77',	'\xf7',
	'\x0f',	'\x8f',	'\x4f',	'\xcf',	'\x2f',	'\xaf',	'\x6f',	'\xef',
	'\x1f',	'\x9f',	'\x5f',	'\xdf',	'\x3f',	'\xbf',	'\x7f',	'\xff'
};
static const unsigned char SGUI_BitMask[8] =
{
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

static void ScreenChange_LineRotate90Degree(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer);
void ScreenDirection_PixelBitblt_1(S_BYTE Pixel, S_BYTE *pScreen, S_BYTE BitMask)
{
	if (Pixel&0x01) *(pScreen + 0*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x02) *(pScreen + 1*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x04) *(pScreen + 2*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x08) *(pScreen + 3*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x10) *(pScreen + 4*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x20) *(pScreen + 5*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x40) *(pScreen + 6*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x80) *(pScreen + 7*SGUI_LCD_LINE) |= BitMask;
}
void ScreenDirection_PixelBitblt_3(S_BYTE Pixel, S_BYTE *pScreen, S_BYTE BitMask)
{
	if (Pixel&0x80) *(pScreen + 0*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x40) *(pScreen + 1*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x20) *(pScreen + 2*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x10) *(pScreen + 3*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x08) *(pScreen + 4*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x04) *(pScreen + 5*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x02) *(pScreen + 6*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x01) *(pScreen + 7*SGUI_LCD_LINE) |= BitMask;
}
void ScreenDirection_0(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
#ifdef WIN32
	memcpy(pScreenBuffer, pUserBuffer, SGUI_LCD_SIZE);
#else
	memset(pScreenBuffer, 0x00, SGUI_LCD_SIZE);
	ScreenChange_LineRotate90Degree(pScreenBuffer, pUserBuffer);
#endif
}
void ScreenDirection_1(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	memset(pScreenBuffer, 0x00, SGUI_LCD_SIZE);
	for (int m = 0; m < SGUI_LCD_HEIGHT; ++m)
	{
		const S_BYTE *pSrc = pUserBuffer + m * SGUI_LCD_LINE;
		S_WORD DestX = (SGUI_LCD_HEIGHT - 1 - m) % 8;
		S_BYTE *pDst = pScreenBuffer + (SGUI_LCD_HEIGHT - 1 - m) / 8;
		if (*(pSrc+0)) ScreenDirection_PixelBitblt_1(*(pSrc+0), pDst+0*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+1)) ScreenDirection_PixelBitblt_1(*(pSrc+1), pDst+1*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+2)) ScreenDirection_PixelBitblt_1(*(pSrc+2), pDst+2*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+3)) ScreenDirection_PixelBitblt_1(*(pSrc+3), pDst+3*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+4)) ScreenDirection_PixelBitblt_1(*(pSrc+4), pDst+4*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+5)) ScreenDirection_PixelBitblt_1(*(pSrc+5), pDst+5*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+6)) ScreenDirection_PixelBitblt_1(*(pSrc+6), pDst+6*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+7)) ScreenDirection_PixelBitblt_1(*(pSrc+7), pDst+7*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+8)) ScreenDirection_PixelBitblt_1(*(pSrc+8), pDst+8*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+9)) ScreenDirection_PixelBitblt_1(*(pSrc+9), pDst+9*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+10)) ScreenDirection_PixelBitblt_1(*(pSrc+10), pDst+10*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+11)) ScreenDirection_PixelBitblt_1(*(pSrc+11), pDst+11*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+12)) ScreenDirection_PixelBitblt_1(*(pSrc+12), pDst+12*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+13)) ScreenDirection_PixelBitblt_1(*(pSrc+13), pDst+13*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+14)) ScreenDirection_PixelBitblt_1(*(pSrc+14), pDst+14*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+15)) ScreenDirection_PixelBitblt_1(*(pSrc+15), pDst+15*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+16)) ScreenDirection_PixelBitblt_1(*(pSrc+16), pDst+16*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+17)) ScreenDirection_PixelBitblt_1(*(pSrc+17), pDst+17*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+18)) ScreenDirection_PixelBitblt_1(*(pSrc+18), pDst+18*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+19)) ScreenDirection_PixelBitblt_1(*(pSrc+19), pDst+19*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
	}
}
void ScreenDirection_2(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	for (int m = 0; m < SGUI_LCD_HEIGHT; ++m)
	{
		const S_BYTE *pSrc = pUserBuffer + m * SGUI_LCD_LINE;
		S_BYTE *pDst = pScreenBuffer + (SGUI_LCD_HEIGHT - 1 - m) * SGUI_LCD_LINE;
		*(pDst+19) = SGUI_BitReverse[*(pSrc+0)];
		*(pDst+18) = SGUI_BitReverse[*(pSrc+1)];
		*(pDst+17) = SGUI_BitReverse[*(pSrc+2)];
		*(pDst+16) = SGUI_BitReverse[*(pSrc+3)];
		*(pDst+15) = SGUI_BitReverse[*(pSrc+4)];
		*(pDst+14) = SGUI_BitReverse[*(pSrc+5)];
		*(pDst+13) = SGUI_BitReverse[*(pSrc+6)];
		*(pDst+12) = SGUI_BitReverse[*(pSrc+7)];
		*(pDst+11) = SGUI_BitReverse[*(pSrc+8)];
		*(pDst+10) = SGUI_BitReverse[*(pSrc+9)];
		*(pDst+9) = SGUI_BitReverse[*(pSrc+10)];
		*(pDst+8) = SGUI_BitReverse[*(pSrc+11)];
		*(pDst+7) = SGUI_BitReverse[*(pSrc+12)];
		*(pDst+6) = SGUI_BitReverse[*(pSrc+13)];
		*(pDst+5) = SGUI_BitReverse[*(pSrc+14)];
		*(pDst+4) = SGUI_BitReverse[*(pSrc+15)];
		*(pDst+3) = SGUI_BitReverse[*(pSrc+16)];
		*(pDst+2) = SGUI_BitReverse[*(pSrc+17)];
		*(pDst+1) = SGUI_BitReverse[*(pSrc+18)];
		*(pDst+0) = SGUI_BitReverse[*(pSrc+19)];
	}
}
void ScreenDirection_3(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	memset(pScreenBuffer, 0x00, SGUI_LCD_SIZE);
	for (int m = 0; m < SGUI_LCD_HEIGHT; ++m)
	{
		const S_BYTE *pSrc = pUserBuffer + m * SGUI_LCD_LINE;
		S_WORD DestX = m % 8;
		S_BYTE *pDst = pScreenBuffer + m / 8;
		if (*(pSrc+19)) ScreenDirection_PixelBitblt_3(*(pSrc+19), pDst+0*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+18)) ScreenDirection_PixelBitblt_3(*(pSrc+18), pDst+1*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+17)) ScreenDirection_PixelBitblt_3(*(pSrc+17), pDst+2*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+16)) ScreenDirection_PixelBitblt_3(*(pSrc+16), pDst+3*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+15)) ScreenDirection_PixelBitblt_3(*(pSrc+15), pDst+4*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+14)) ScreenDirection_PixelBitblt_3(*(pSrc+14), pDst+5*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+13)) ScreenDirection_PixelBitblt_3(*(pSrc+13), pDst+6*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+12)) ScreenDirection_PixelBitblt_3(*(pSrc+12), pDst+7*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+11)) ScreenDirection_PixelBitblt_3(*(pSrc+11), pDst+8*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+10)) ScreenDirection_PixelBitblt_3(*(pSrc+10), pDst+9*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+9)) ScreenDirection_PixelBitblt_3(*(pSrc+9), pDst+10*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+8)) ScreenDirection_PixelBitblt_3(*(pSrc+8), pDst+11*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+7)) ScreenDirection_PixelBitblt_3(*(pSrc+7), pDst+12*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+6)) ScreenDirection_PixelBitblt_3(*(pSrc+6), pDst+13*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+5)) ScreenDirection_PixelBitblt_3(*(pSrc+5), pDst+14*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+4)) ScreenDirection_PixelBitblt_3(*(pSrc+4), pDst+15*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+3)) ScreenDirection_PixelBitblt_3(*(pSrc+3), pDst+16*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+2)) ScreenDirection_PixelBitblt_3(*(pSrc+2), pDst+17*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+1)) ScreenDirection_PixelBitblt_3(*(pSrc+1), pDst+18*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+0)) ScreenDirection_PixelBitblt_3(*(pSrc+0), pDst+19*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
	}
}

void ScreenChange_ByteReverse(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	for (int m = 0; m < SGUI_LCD_HEIGHT_5000C; ++m)
	{
		const S_BYTE *pSrc = pUserBuffer + m * SGUI_LCD_LINE_5000C;
		S_BYTE *pDst = pScreenBuffer + m * SGUI_LCD_LINE_5000C;
		*(pDst+0) = SGUI_BitReverse[*(pSrc+0)];
		*(pDst+1) = SGUI_BitReverse[*(pSrc+1)];
		*(pDst+2) = SGUI_BitReverse[*(pSrc+2)];
		*(pDst+3) = SGUI_BitReverse[*(pSrc+3)];
		*(pDst+4) = SGUI_BitReverse[*(pSrc+4)];
		*(pDst+5) = SGUI_BitReverse[*(pSrc+5)];
		*(pDst+6) = SGUI_BitReverse[*(pSrc+6)];
		*(pDst+7) = SGUI_BitReverse[*(pSrc+7)];
		*(pDst+8) = SGUI_BitReverse[*(pSrc+8)];
		*(pDst+9) = SGUI_BitReverse[*(pSrc+9)];
		*(pDst+10) = SGUI_BitReverse[*(pSrc+10)];
		*(pDst+11) = SGUI_BitReverse[*(pSrc+11)];
		*(pDst+12) = SGUI_BitReverse[*(pSrc+12)];
		*(pDst+13) = SGUI_BitReverse[*(pSrc+13)];
		*(pDst+14) = SGUI_BitReverse[*(pSrc+14)];
		*(pDst+15) = SGUI_BitReverse[*(pSrc+15)];
		*(pDst+16) = SGUI_BitReverse[*(pSrc+16)];
		*(pDst+17) = SGUI_BitReverse[*(pSrc+17)];
		*(pDst+18) = SGUI_BitReverse[*(pSrc+18)];
		*(pDst+19) = SGUI_BitReverse[*(pSrc+19)];
	}
}

void ScreenChange_LineReverse(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	for (int m = 0; m < SGUI_LCD_HEIGHT; ++m)
	{
		const S_BYTE *pSrc = pUserBuffer + m * SGUI_LCD_LINE;
		S_BYTE *pDst = pScreenBuffer + m * SGUI_LCD_LINE;
		*(pDst+0) = SGUI_BitReverse[*(pSrc+19)];
		*(pDst+1) = SGUI_BitReverse[*(pSrc+18)];
		*(pDst+2) = SGUI_BitReverse[*(pSrc+17)];
		*(pDst+3) = SGUI_BitReverse[*(pSrc+16)];
		*(pDst+4) = SGUI_BitReverse[*(pSrc+15)];
		*(pDst+5) = SGUI_BitReverse[*(pSrc+14)];
		*(pDst+6) = SGUI_BitReverse[*(pSrc+13)];
		*(pDst+7) = SGUI_BitReverse[*(pSrc+12)];
		*(pDst+8) = SGUI_BitReverse[*(pSrc+11)];
		*(pDst+9) = SGUI_BitReverse[*(pSrc+10)];
		*(pDst+10) = SGUI_BitReverse[*(pSrc+9)];
		*(pDst+11) = SGUI_BitReverse[*(pSrc+8)];
		*(pDst+12) = SGUI_BitReverse[*(pSrc+7)];
		*(pDst+13) = SGUI_BitReverse[*(pSrc+6)];
		*(pDst+14) = SGUI_BitReverse[*(pSrc+5)];
		*(pDst+15) = SGUI_BitReverse[*(pSrc+4)];
		*(pDst+16) = SGUI_BitReverse[*(pSrc+3)];
		*(pDst+17) = SGUI_BitReverse[*(pSrc+2)];
		*(pDst+18) = SGUI_BitReverse[*(pSrc+1)];
		*(pDst+19) = SGUI_BitReverse[*(pSrc+0)];
	}
}

void ScreenChange_LineRotate90Degree(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	int t=0,l=0,b=0;

	for (int u=0,j=0; j<64; j++)
	{
		if (j%8 == 0)
		{
			b = 0;
		}
		l = j/8;
		for (int i=0; i<24; i++)
		{
			for (int n=0; n<8; n++,t++)
			{
				pScreenBuffer[8*t+l] |= ((pUserBuffer[u]>>n)&0x01)<<b;
			}
			u++;
		}
		t=0;
		b++;
	}
}

#ifdef WIN32

//
// 基础UI驱动
//
int C_SguiDriverBase::DriverID(void)
{
	return SGUI_DRIVER_ID_DEFAULT;
}
unsigned short C_SguiDriverBase::LcdWidth(void)
{
	return SGUI_LCD_WIDTH;
}
unsigned short C_SguiDriverBase::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT;
}
unsigned short C_SguiDriverBase::LcdBpp(void)
{
	return SGUI_LCD_BPP;
}

int C_SguiDriverBase::fbinit(void)
{
	return 0;
}

int C_SguiDriverBase::fbfresh(void)
{
	S_BYTE C[] = {'\xFF', '\x00'};
	S_BYTE ScreenBuffer[SGUI_LCD_SIZE] = {0}; //屏幕缓冲

	
	switch (g_SGUI_ScreenDirection)
	{
	default:
	case 0:ScreenDirection_0(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 1:ScreenDirection_1(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 2:ScreenDirection_2(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 3:ScreenDirection_3(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	}

	for (int m = 0; m < SGUI_LCD_HEIGHT; ++m)
	{
		S_BYTE *pSrc = (S_BYTE *)ScreenBuffer + m * SGUI_LCD_LINE;
		S_BYTE *pDst = (S_BYTE *)SGUI_fb + m * (480*3);
		for (int n = 0; n < SGUI_LCD_LINE; ++n)
		{
			S_BYTE pixels = *(pSrc + n);
			*(pDst+0) = C[(pixels>>0)&0x01];*(pDst+1) = C[(pixels>>0)&0x01];*(pDst+2) = C[(pixels>>0)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>1)&0x01];*(pDst+1) = C[(pixels>>1)&0x01];*(pDst+2) = C[(pixels>>1)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>2)&0x01];*(pDst+1) = C[(pixels>>2)&0x01];*(pDst+2) = C[(pixels>>2)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>3)&0x01];*(pDst+1) = C[(pixels>>3)&0x01];*(pDst+2) = C[(pixels>>3)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>4)&0x01];*(pDst+1) = C[(pixels>>4)&0x01];*(pDst+2) = C[(pixels>>4)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>5)&0x01];*(pDst+1) = C[(pixels>>5)&0x01];*(pDst+2) = C[(pixels>>5)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>6)&0x01];*(pDst+1) = C[(pixels>>6)&0x01];*(pDst+2) = C[(pixels>>6)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>7)&0x01];*(pDst+1) = C[(pixels>>7)&0x01];*(pDst+2) = C[(pixels>>7)&0x01];pDst += 3;
		}
	}
	return 0;
}

void C_SguiDriverBase::backlight(int on) //on =1 开
{
}

void C_SguiDriverBase::SetContrast(int contrast)
{
}

void C_SguiDriverBase::SetBrightness(int brightness)
{
}

void C_SguiDriverBase::SetGrayness(int grayness)
{
}

void C_SguiDriverBase::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverBase::OpenScreen(void)
{
}

void C_SguiDriverBase::CloseScreen(void)
{
}

int C_SguiDriverBase::kbinit(void)
{
	return 0;
}

char SGUI_key = 0;
int C_SguiDriverBase::ScanKeyboard(int &turnCount)
{
	int key = -1;

	if (SGUI_key)
	{
		key = SGUI_key;
		SGUI_key = 0;
	}
	return key;
}

//
// 重庆6100UI驱动
//
int C_SguiDriverCq6100::DriverID(void)
{
	return SGUI_DRIVER_ID_CQ_6100;
}
unsigned short C_SguiDriverCq6100::LcdWidth(void)
{
	return SGUI_LCD_WIDTH;
}
unsigned short C_SguiDriverCq6100::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT;
}
unsigned short C_SguiDriverCq6100::LcdBpp(void)
{
	return SGUI_LCD_BPP;
}

int C_SguiDriverCq6100::fbinit(void)
{
	return 0;
}

int C_SguiDriverCq6100::fbfresh(void)
{
	return C_SguiDriverBase::fbfresh();
}

void C_SguiDriverCq6100::backlight(int on) //on =1 开
{
}

void C_SguiDriverCq6100::SetContrast(int contrast)
{
}

void C_SguiDriverCq6100::SetBrightness(int brightness)
{
}

void C_SguiDriverCq6100::SetGrayness(int grayness)
{
}

void C_SguiDriverCq6100::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverCq6100::OpenScreen(void)
{
}

void C_SguiDriverCq6100::CloseScreen(void)
{
}

int C_SguiDriverCq6100::kbinit(void)
{
	return 0;
}

int C_SguiDriverCq6100::ScanKeyboard(int &turnCount)
{
	return C_SguiDriverBase::ScanKeyboard(turnCount);
}

//
// EAC5000D驱动
//
int C_SguiDriverEac5000D::DriverID(void)
{
	return SGUI_DRIVER_ID_EAC_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdWidth(void)
{
	return SGUI_LCD_WIDTH_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdBpp(void)
{
	return SGUI_LCD_BPP_5000D;
}

int C_SguiDriverEac5000D::fbinit(void)
{
	return 0;
}

int C_SguiDriverEac5000D::fbfresh(void)
{
	S_BYTE C[] = {'\xFF', '\x00'};
	for (int m = 0; m < SGUI_LCD_HEIGHT_5000D; ++m)
	{
		S_BYTE *pSrc = (S_BYTE *)SGUI_UsrBuf + m * SGUI_LCD_LINE_5000D;
		S_BYTE *pDst = (S_BYTE *)SGUI_fb + m * (480*3);
		for (int n = 0; n < SGUI_LCD_LINE_5000D; ++n)
		{
			S_BYTE pixels = *(pSrc + n);
			*(pDst+0) = C[(pixels>>0)&0x01];*(pDst+1) = C[(pixels>>0)&0x01];*(pDst+2) = C[(pixels>>0)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>1)&0x01];*(pDst+1) = C[(pixels>>1)&0x01];*(pDst+2) = C[(pixels>>1)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>2)&0x01];*(pDst+1) = C[(pixels>>2)&0x01];*(pDst+2) = C[(pixels>>2)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>3)&0x01];*(pDst+1) = C[(pixels>>3)&0x01];*(pDst+2) = C[(pixels>>3)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>4)&0x01];*(pDst+1) = C[(pixels>>4)&0x01];*(pDst+2) = C[(pixels>>4)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>5)&0x01];*(pDst+1) = C[(pixels>>5)&0x01];*(pDst+2) = C[(pixels>>5)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>6)&0x01];*(pDst+1) = C[(pixels>>6)&0x01];*(pDst+2) = C[(pixels>>6)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>7)&0x01];*(pDst+1) = C[(pixels>>7)&0x01];*(pDst+2) = C[(pixels>>7)&0x01];pDst += 3;
		}
	}
	return 0;
}

void C_SguiDriverEac5000D::backlight(int on) //on =1 开
{
}

void C_SguiDriverEac5000D::SetContrast(int contrast)
{
}

void C_SguiDriverEac5000D::SetBrightness(int brightness)
{
}

void C_SguiDriverEac5000D::SetGrayness(int grayness)
{
}

void C_SguiDriverEac5000D::SetDirection(int direction)
{
}

void C_SguiDriverEac5000D::OpenScreen(void)
{
}

void C_SguiDriverEac5000D::CloseScreen(void)
{
}

int C_SguiDriverEac5000D::kbinit(void)
{
	return 0;
}

int C_SguiDriverEac5000D::ScanKeyboard(int &turnCount)
{
	return _ScanKeyboard(turnCount);
}

int C_SguiDriverEac5000D::_ScanKeyboard(int &turnCount)
{
	int key = -1;

	if (SGUI_key)
	{
		key = SGUI_key;
		SGUI_key = 0;
	}
	return key;
}

//
// EAC5000C驱动
//
int C_SguiDriverEac5000C::DriverID(void)
{
	return SGUI_DRIVER_ID_EAC_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdWidth(void)
{
	return SGUI_LCD_WIDTH_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdBpp(void)
{
	return SGUI_LCD_BPP_5000C;
}

int C_SguiDriverEac5000C::fbinit(void)
{
	return 0;
}

int C_SguiDriverEac5000C::fbfresh(void)
{
	S_BYTE C[] = {'\xFF', '\x00'};
	S_BYTE ScreenBuffer[SGUI_LCD_SIZE_5000C]; //屏幕缓冲

	switch (g_SGUI_ScreenDirection)
	{
	default:
	case 0:ScreenDirection_0(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 1:ScreenDirection_1(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 2:ScreenDirection_2(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 3:ScreenDirection_3(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	}

	for (int m = 0; m < SGUI_LCD_HEIGHT_5000C; ++m)
	{
		S_BYTE *pSrc = (S_BYTE *)ScreenBuffer + m * SGUI_LCD_LINE_5000C;
		S_BYTE *pDst = (S_BYTE *)SGUI_fb + m * (480*3);
		for (int n = 0; n < SGUI_LCD_LINE_5000C; ++n)
		{
			S_BYTE pixels = *(pSrc + n);
			*(pDst+0) = C[(pixels>>0)&0x01];*(pDst+1) = C[(pixels>>0)&0x01];*(pDst+2) = C[(pixels>>0)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>1)&0x01];*(pDst+1) = C[(pixels>>1)&0x01];*(pDst+2) = C[(pixels>>1)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>2)&0x01];*(pDst+1) = C[(pixels>>2)&0x01];*(pDst+2) = C[(pixels>>2)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>3)&0x01];*(pDst+1) = C[(pixels>>3)&0x01];*(pDst+2) = C[(pixels>>3)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>4)&0x01];*(pDst+1) = C[(pixels>>4)&0x01];*(pDst+2) = C[(pixels>>4)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>5)&0x01];*(pDst+1) = C[(pixels>>5)&0x01];*(pDst+2) = C[(pixels>>5)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>6)&0x01];*(pDst+1) = C[(pixels>>6)&0x01];*(pDst+2) = C[(pixels>>6)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>7)&0x01];*(pDst+1) = C[(pixels>>7)&0x01];*(pDst+2) = C[(pixels>>7)&0x01];pDst += 3;
		}
	}
	return 0;
}

void C_SguiDriverEac5000C::backlight(int on) //on =1 开
{
}

void C_SguiDriverEac5000C::SetContrast(int contrast)
{
}

void C_SguiDriverEac5000C::SetBrightness(int brightness)
{
}

void C_SguiDriverEac5000C::SetGrayness(int grayness)
{
}

void C_SguiDriverEac5000C::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverEac5000C::OpenScreen(void)
{
}

void C_SguiDriverEac5000C::CloseScreen(void)
{
}

int C_SguiDriverEac5000C::kbinit(void)
{
	return 0;
}

int C_SguiDriverEac5000C::ScanKeyboard(int &turnCount)
{
	int key = _ScanKeyboard(turnCount);
	switch(key)
	{
	case 1: key = GVK_UP; break;
	case 2: key = GVK_DN; break;
	case 3: key = GVK_LEFT; break;
	case 4: key = GVK_RIGHT; break;
	default: key = -1; break;
	}
	return key;
}

int C_SguiDriverEac5000C::_ScanKeyboard(int &turnCount)
{
	int key = -1;

	if (SGUI_key)
	{
		key = SGUI_key;
		SGUI_key = 0;
	}
	return key;
}

#else

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

/** @brief 缺省驱动
 */
#define FBDEV  "/dev/fb0"
#define BUTTONDEV "/dev/kgeio"

#define ST7529FB_IOC_MAGIC          'S'
#define ST7529FB_IOC_RESET          _IO(ST7529FB_IOC_MAGIC, 0)
#define ST7529FB_IOC_REFRESH        _IO(ST7529FB_IOC_MAGIC, 1)
#define ST7529FB_IOC_BACKLIGHT      _IO(ST7529FB_IOC_MAGIC, 2)
#define ST7529FB_IOC_SETCONTRAST      _IO(ST7529FB_IOC_MAGIC, 3)
#define ST7529FB_IOC_SETGRAYSCALE      _IO(ST7529FB_IOC_MAGIC, 4)

int SGUI_fd = -1;
int SGUI_kd = -1;

int C_SguiDriverBase::DriverID(void)
{
	return SGUI_DRIVER_ID_DEFAULT;
}
unsigned short C_SguiDriverBase::LcdWidth(void)
{
	return SGUI_LCD_WIDTH;
}
unsigned short C_SguiDriverBase::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT;
}
unsigned short C_SguiDriverBase::LcdBpp(void)
{
	return SGUI_LCD_BPP;
}

int C_SguiDriverBase::fbinit(void)
{
	if (SGUI_fd >= 0)// 初始化完成了
		return 0;

	SGUI_fd = open(FBDEV, O_RDWR);
	if(SGUI_fd<0)	{
		printf(FBDEV"Open failed!\n");
	}

	SGUI_fb = (char *)mmap(0, SGUI_LCD_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, SGUI_fd, 0);
	if(SGUI_fb == NULL){
		printf("Map %s failed!\n", FBDEV);
	}

	memset(SGUI_UsrBuf, 0x00, SGUI_LCD_SIZE);
	fbfresh();

	return 0;
}

int C_SguiDriverBase::fbfresh(void)
{
	switch (g_SGUI_ScreenDirection)
	{
	default:
	case 0:ScreenDirection_0((S_BYTE *)SGUI_fb, (S_BYTE *)SGUI_UsrBuf);break;
	case 1:ScreenDirection_1((S_BYTE *)SGUI_fb, (S_BYTE *)SGUI_UsrBuf);break;
	case 2:ScreenDirection_2((S_BYTE *)SGUI_fb, (S_BYTE *)SGUI_UsrBuf);break;
	case 3:ScreenDirection_3((S_BYTE *)SGUI_fb, (S_BYTE *)SGUI_UsrBuf);break;
	}
	ioctl(SGUI_fd, ST7529FB_IOC_REFRESH);
	return 0;
}

void C_SguiDriverBase::backlight(int on) //on =1 开
{
	extern int io_WriteCmd(const char *pCmd, int state);
	io_WriteCmd("backlight", on);
}

void C_SguiDriverBase::SetContrast(int contrast)
{
	if (SGUI_fd >= 0)
		ioctl(SGUI_fd, ST7529FB_IOC_SETCONTRAST, contrast);
}

void C_SguiDriverBase::SetBrightness(int brightness)
{
}

void C_SguiDriverBase::SetGrayness(int grayness)
{
	if (SGUI_fd >= 0)
		ioctl(SGUI_fd, ST7529FB_IOC_SETGRAYSCALE, grayness);
}

void C_SguiDriverBase::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverBase::OpenScreen(void)
{
}

void C_SguiDriverBase::CloseScreen(void)
{
}

int C_SguiDriverBase::kbinit(void)
{
	if (SGUI_kd >= 0)// 初始化完成了
		return 0;

	SGUI_kd = open(BUTTONDEV, O_RDWR);
	if (SGUI_kd < 0)
	{
		printf(BUTTONDEV"open failed!\n");
		return -1;
	}
	return 0;
}

int C_SguiDriverBase::ScanKeyboard(int &turnCount)
{
	int key = _ScanKeyboard(turnCount);
	if (key > GVK_KEYBEGIN && key < GVK_KEYEND)
		return key;


	return -1;
}

int C_SguiDriverBase::_ScanKeyboard(int &turnCount)
{
	KEY_VALUE event;
	fd_set rset;
	struct timeval tv;
	event.button = 0;
	for (int i = 0;i < 4;i++)
	{
		event.left_cnt[i] = 0;
		event.right_cnt[i] = 0;
	}

	SGUI_kd = open("/dev/kgetc", O_RDONLY);
	if (SGUI_kd <= 0)
		return -1;

	FD_ZERO(&rset);
	FD_SET(SGUI_kd, &rset);

	tv.tv_sec = 0;
	tv.tv_usec = 100000;
	volatile int delay = 250;

	if (select(SGUI_kd + 1, &rset, NULL, NULL, &tv) > 0) 
	{
		read(SGUI_kd, &event, sizeof(KEY_VALUE));
		//return (event&0xFF);
		for(int i = 0;i < 4;i++)
		{
			if(event.left_cnt[i] >= 1)
			{
				turnCount = event.left_cnt[i];
				return (i + 1) * 3;
			}
			if(event.right_cnt[i] >= 1)
			{
				turnCount = event.right_cnt[i];
				return (i + 1) * 7;
			}
		}
		while (delay)
		{
			delay--;
		}
		if (0 == event.button)
		{
			return -1;
		}
		else
		{
			return event.button;
		}
	}
	close(SGUI_kd);
	return -1;
}


/** @brief CQ6100驱动
 */
int C_SguiDriverCq6100::DriverID(void)
{
	return SGUI_DRIVER_ID_CQ_6100;
}
unsigned short C_SguiDriverCq6100::LcdWidth(void)
{
	return SGUI_LCD_WIDTH;
}
unsigned short C_SguiDriverCq6100::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT;
}
unsigned short C_SguiDriverCq6100::LcdBpp(void)
{
	return SGUI_LCD_BPP;
}

int C_SguiDriverCq6100::fbinit(void)
{
	if (SGUI_fd >= 0)// 初始化完成了
		return 0;

	SGUI_fd = open(FBDEV, O_RDWR);
	if(SGUI_fd<0)	{
		printf(FBDEV"Open failed!\n");
	}

	SGUI_fb = (char *)mmap(0, SGUI_LCD_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, SGUI_fd, 0);
	if(SGUI_fb == NULL){
		printf("Map %s failed!\n", FBDEV);
	}

	memset(SGUI_UsrBuf, 0x00, SGUI_LCD_SIZE);
	fbfresh();

	return 0;
}

int C_SguiDriverCq6100::fbfresh(void)
{
	S_BYTE ScreenBuffer[SGUI_LCD_SIZE]; //屏幕缓冲
	S_BYTE ScreenBuffer2[SGUI_LCD_SIZE]; //屏幕缓冲
	ScreenChange_LineReverse(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);
	ScreenDirection_3((S_BYTE *)ScreenBuffer2, (S_BYTE *)ScreenBuffer);
	ScreenChange_ByteReverse((S_BYTE *)SGUI_fb, (S_BYTE *)ScreenBuffer2);
	ioctl(SGUI_fd, ST7529FB_IOC_REFRESH);
	return 0;
}

void C_SguiDriverCq6100::backlight(int on) //on =1 开
{
#define KGESG_IO_MAGIC        'K'
#define KGESG_IO_LCDBL_ON             _IO(KGESG_IO_MAGIC, 28)
#define KGESG_IO_LCDBL_OFF            _IO(KGESG_IO_MAGIC, 29)
	int fd = open(BUTTONDEV, O_RDWR);
	if(fd < 0)
	{
		printf(BUTTONDEV"open failed!\n");
	}
	else
	{
		if (on != 0)
			ioctl(fd, KGESG_IO_LCDBL_ON);
		else
			ioctl(fd, KGESG_IO_LCDBL_OFF);
		close(fd);
	}
}

void C_SguiDriverCq6100::SetContrast(int contrast)
{
}

void C_SguiDriverCq6100::SetBrightness(int brightness)
{
}

void C_SguiDriverCq6100::SetGrayness(int grayness)
{
}

void C_SguiDriverCq6100::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverCq6100::OpenScreen(void)
{
}

void C_SguiDriverCq6100::CloseScreen(void)
{
}

int C_SguiDriverCq6100::kbinit(void)
{
	if (SGUI_kd >= 0)// 初始化完成了
		return 0;

	SGUI_kd = open(BUTTONDEV, O_RDWR);
	if (SGUI_kd < 0)
	{
		printf(BUTTONDEV"open failed!\n");
		return -1;
	}
	return 0;
}

int C_SguiDriverCq6100::ScanKeyboard(int &turnCount)
{
	int key = _ScanKeyboard(turnCount);
	if (key > GVK_KEYBEGIN && key < GVK_KEYEND)
		return key;

	return -1;
}


/** @brief EAC5000D驱动
 */
#define FBDEV_EAC5000D  "/dev/fb/0" //ZHGX 旧驱动
#define BUTTONDEV_EAC5000D "/dev/misc/button"//ZHGX 旧驱动

int C_SguiDriverEac5000D::DriverID(void)
{
	return SGUI_DRIVER_ID_EAC_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdWidth(void)
{
	return SGUI_LCD_WIDTH_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdBpp(void)
{
	return SGUI_LCD_BPP_5000D;
}

int C_SguiDriverEac5000D::fbinit(void)
{
	if (SGUI_fd >= 0)// 初始化完成了
		return 0;

	SGUI_fd = open(FBDEV_EAC5000D, O_RDWR);
	if(SGUI_fd<0)	{
		printf(FBDEV_EAC5000D" Open failed!\n");
	}
	//实际屏幕是320*240，16位色
	SGUI_fb = (char *)mmap(0, SGUI_LCD_WIDTH_5000D*SGUI_LCD_HEIGHT_5000D*2, PROT_READ|PROT_WRITE, MAP_SHARED, SGUI_fd, 0);
	if(SGUI_fb == NULL){
		printf("Map %s failed!\n", FBDEV_EAC5000D);
	}

	memset(SGUI_UsrBuf, 0x00, SGUI_LCD_SIZE_5000D);
	fbfresh();

	return 0;
}

int C_SguiDriverEac5000D::fbfresh(void)
{
	S_WORD C[] = {0xFFFF, 0x202F};
	for (int m = 0; m < SGUI_LCD_HEIGHT_5000D; ++m)
	{
		S_BYTE *pSrc = (S_BYTE *)SGUI_UsrBuf + m * SGUI_LCD_LINE_5000D;
		S_BYTE *pDst = (S_BYTE *)SGUI_fb + m * (SGUI_LCD_WIDTH_5000D*2);
		S_WORD *pScreen = (S_WORD *)pDst;
		for (int n = 0; n < SGUI_LCD_LINE_5000D; ++n)
		{
			S_BYTE pixels = *(pSrc + n);
			*(pScreen+0) = C[(pixels>>0)&0x01];
			*(pScreen+1) = C[(pixels>>1)&0x01];
			*(pScreen+2) = C[(pixels>>2)&0x01];
			*(pScreen+3) = C[(pixels>>3)&0x01];
			*(pScreen+4) = C[(pixels>>4)&0x01];
			*(pScreen+5) = C[(pixels>>5)&0x01];
			*(pScreen+6) = C[(pixels>>6)&0x01];
			*(pScreen+7) = C[(pixels>>7)&0x01];

			pScreen += 8;
		}
	}
	return 0;
}

#define IO_DEV_EAC5000D "/dev/misc/io"
void C_SguiDriverEac5000D::backlight(int on) //on =1 开
{
	int func = 0x665e;//EAC5000D
	int io_btfd = open(IO_DEV_EAC5000D, O_RDWR);

	if(io_btfd < 0)
	{
		printf("Can't open "IO_DEV_EAC5000D"!\n");
	}
	if(ioctl(io_btfd, func, !on) != 0)
	{
		printf("%s lcd light error\n",on==0?"close":"open");
	}
	close(io_btfd);
}

void C_SguiDriverEac5000D::SetContrast(int contrast)
{
}

void C_SguiDriverEac5000D::SetBrightness(int brightness)
{
}

void C_SguiDriverEac5000D::SetGrayness(int grayness)
{
}

void C_SguiDriverEac5000D::SetDirection(int direction)
{
}

void C_SguiDriverEac5000D::OpenScreen(void)
{
}

void C_SguiDriverEac5000D::CloseScreen(void)
{
}

int C_SguiDriverEac5000D::kbinit(void)
{
	if (SGUI_kd >= 0)// 初始化完成了
		return 0;

	SGUI_kd = open(BUTTONDEV_EAC5000D, O_RDWR);
	if (SGUI_kd < 0)
	{
		printf(BUTTONDEV" open failed!\n");
		return -1;
	}
	return 0;
}

int C_SguiDriverEac5000D::ScanKeyboard(int &turnCount)
{
	int key = _ScanKeyboard(turnCount);
	if (key < 0)
		return key;

	switch(key)
	{
	case 0:key = GVK_UP;break;
	case 1:key = GVK_DN;break;
	case 2:key = GVK_LEFT;break;
	case 3:key = GVK_RIGHT;break;
	case 4:key = GVK_PGUP;break;
	case 5:key = GVK_PGDN;break;
	case 6:key = GVK_CANCEL;break;
	case 7:key = GVK_OK;break;
	default:key = -1;break;
	}
	return key;
}

int C_SguiDriverEac5000D::_ScanKeyboard(int &turnCount)
{
	int event;
	fd_set rset;
	struct timeval tv;

	if (SGUI_kd <= 0)
		return -1;

	FD_ZERO(&rset);
	FD_SET(SGUI_kd, &rset);

	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	if (select(SGUI_kd + 1, &rset, NULL, NULL, &tv) > 0) 
	{
		read(SGUI_kd, &event, 1);
		return (event&0xFF);
	}

	return -1;
}

/** @brief EAC5000C驱动
 */
#define FBDEV_EAC5000C  "/dev/misc/lcd" //ZHGX 旧驱动
#define BUTTONDEV_EAC5000C "/dev/misc/button"//ZHGX 旧驱动
#define IOGET_LCDPARAM			0x8002
#define IOSET_REFRESH			0x8001

int C_SguiDriverEac5000C::DriverID(void)
{
	return SGUI_DRIVER_ID_EAC_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdWidth(void)
{
	return SGUI_LCD_WIDTH_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdBpp(void)
{
	return SGUI_LCD_BPP_5000C;
}

int C_SguiDriverEac5000C::fbinit(void)
{
	if (SGUI_fd >= 0)// 初始化完成了
		return 0;

	SGUI_fd = open(FBDEV_EAC5000C, O_RDWR);
	if(SGUI_fd<0)	{
		printf(FBDEV_EAC5000C" Open failed!\n");
	}
	SGUI_fb = (char *)mmap(0, SGUI_LCD_SIZE_5000C, PROT_READ|PROT_WRITE, MAP_SHARED, SGUI_fd, 0);
	if(SGUI_fb == NULL){
		printf("Map %s failed!\n", FBDEV_EAC5000C);
	}

	memset(SGUI_UsrBuf, 0x00, SGUI_LCD_SIZE_5000C);
	fbfresh();

	return 0;
}

int C_SguiDriverEac5000C::fbfresh(void)
{
	S_BYTE ScreenBuffer[SGUI_LCD_SIZE_5000C]; //屏幕缓冲
	ScreenChange_ByteReverse(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);
	switch (g_SGUI_ScreenDirection)
	{
	default:
	case 0:ScreenDirection_0((S_BYTE *)SGUI_fb, (S_BYTE *)ScreenBuffer);break;
	case 1:ScreenDirection_1((S_BYTE *)SGUI_fb, (S_BYTE *)ScreenBuffer);break;
	case 2:ScreenDirection_2((S_BYTE *)SGUI_fb, (S_BYTE *)ScreenBuffer);break;
	case 3:ScreenDirection_3((S_BYTE *)SGUI_fb, (S_BYTE *)ScreenBuffer);break;
	}
	ioctl(SGUI_fd, IOSET_REFRESH,0);
	return 0;
}

#define IO_DEV_EAC5000C "/dev/misc/io"
void C_SguiDriverEac5000C::backlight(int on) //on =1 开
{
	int func = 0x665e;//EAC5000D
	int io_btfd = open(IO_DEV_EAC5000C, O_RDWR);

	if(io_btfd < 0)
	{
		printf("Can't open "IO_DEV_EAC5000C"!\n");
	}
	if(ioctl(io_btfd, func, !on) != 0)
	{
		printf("%s lcd light error\n",on==0?"close":"open");
	}
	close(io_btfd);
}

void C_SguiDriverEac5000C::SetContrast(int contrast)
{
}

void C_SguiDriverEac5000C::SetBrightness(int brightness)
{
}

void C_SguiDriverEac5000C::SetGrayness(int grayness)
{
}

void C_SguiDriverEac5000C::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverEac5000C::OpenScreen(void)
{
}

void C_SguiDriverEac5000C::CloseScreen(void)
{
}

int C_SguiDriverEac5000C::kbinit(void)
{
	if (SGUI_kd >= 0)// 初始化完成了
		return 0;

	SGUI_kd = open(BUTTONDEV_EAC5000C, O_RDWR);
	if (SGUI_kd < 0)
	{
		printf(BUTTONDEV" open failed!\n");
		return -1;
	}
	return 0;
}

int C_SguiDriverEac5000C::ScanKeyboard(int &turnCount)
{
	int key = _ScanKeyboard(turnCount);
	switch(key)
	{
	case 0:key = GVK_UP;break;
	case 1:key = GVK_DN;break;
	case 2:key = GVK_LEFT;break;
	case 3:key = GVK_RIGHT;break;
	default:key = -1;break;
	}
	return key;
}

int C_SguiDriverEac5000C::_ScanKeyboard(int &turnCount)
{
	int event;
	fd_set rset;
	struct timeval tv;

	if (SGUI_kd <= 0)
		return -1;

	FD_ZERO(&rset);
	FD_SET(SGUI_kd, &rset);

	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	if (select(SGUI_kd + 1, &rset, NULL, NULL, &tv) > 0) 
	{
		read(SGUI_kd, &event, 1);
		return (event&0xFF);
	}

	return -1;
}

#endif

/** @brief 设置SGUI 驱动 */
int SGUI_SetDriver(int SguiDriverID)
{
	int old = pSguiDriverBase->DriverID();
	switch (SguiDriverID)
	{
	case SGUI_DRIVER_ID_DEFAULT:
		pSguiDriverBase = &g_SguiDriverBaseDefault;
		break;
	case SGUI_DRIVER_ID_EAC_5000D:
		pSguiDriverBase = &g_SguiDriverEac5000D;
		break;
	case SGUI_DRIVER_ID_EAC_5000C:
		pSguiDriverBase = &g_SguiDriverEac5000C;
		break;
	case SGUI_DRIVER_ID_CQ_6100:
		pSguiDriverBase = &g_SguiDriverCq6100;
		break;
	default:
		pSguiDriverBase = &g_SguiDriverBaseDefault;
		break;
	};
	return old;
}

/** @brief Lcd参数 */
unsigned short SGUI_LcdWidth(void)
{
	return pSguiDriverBase->LcdWidth();
}
unsigned short SGUI_LcdHeight(void)
{
	return pSguiDriverBase->LcdHeight();
}
unsigned short SGUI_LcdBpp(void)
{
	return pSguiDriverBase->LcdBpp();
}
unsigned long SGUI_LcdLine(void)
{
	return ((unsigned long)SGUI_LcdWidth() * SGUI_LcdBpp() / 8);
}
unsigned long SGUI_LcdSize(void)
{
	return ((unsigned long)SGUI_LcdLine() * SGUI_LcdHeight());
}

/** @brief LCD初始化*/
int SGUI_LcdInit(void)
{
	return pSguiDriverBase->fbinit();
}

/** @brief 键盘初始化*/
int SGUI_KeyboardInit(void)
{
	return pSguiDriverBase->kbinit();
}

static bool flushScrnEnable = true;
/** @brief 屏幕刷新*/
void SGUI_flushScreen(S_WORD x, S_WORD y, S_WORD w, S_WORD h)
{
	if (flushScrnEnable == true)
		pSguiDriverBase->fbfresh();
}

/** @brief 屏幕刷新使能*/
bool SGUI_FlushScreenEn(bool enable)
{
	bool old = flushScrnEnable;
	flushScrnEnable = enable;
//	if (flushScrnEnable == true)
//		fbfresh();
	return old;
}

/** @brief 关闭屏幕*/
void SGUI_CloseScreen(void)
{
	pSguiDriverBase->CloseScreen();
}

/** @brief 打开屏幕*/
void SGUI_OpenScreen(void)
{
	pSguiDriverBase->OpenScreen();
}

/** @brief 背光灯开关*/
void SGUI_backlight(int on)
{
	if (on != 0)
		on = 1;
	pSguiDriverBase->backlight(on);
}

/** @brief 设置对比度
 *  @param[in] contrast 0-99
 */
void SGUI_contrast(int contrast)
{
	if (contrast < 0 || contrast > 99)
		contrast = 99;
	pSguiDriverBase->SetContrast(contrast);
}

/** @brief 设置背光灯亮度
 *  @param[in] brightness 0-99
 */
void SGUI_brightness(int brightness)
{
	if (brightness < 0 || brightness > 99)
		brightness = 99;
	pSguiDriverBase->SetBrightness(brightness);
}

/** @brief 设置灰度
 *  @param[in] grayness 0-99
 */
void SGUI_grayness(int grayness)
{
	if (grayness < 0 || grayness > 99)
		grayness = 99;
	pSguiDriverBase->SetGrayness(grayness);
}

/** @brief 设置屏幕方向
 *  @param[in] direction 0-3
 */
void SGUI_direction(int direction)
{
	if (direction < 0 || direction > 3)
		direction = 0;
	pSguiDriverBase->SetDirection(direction);
}

/** @brief 资源路径初始化*/
char *SGUI_ResourcePathInit(const char *pResourcePath)
{
	if (pResourcePath != NULL)
		sprintf(g_SGUI_ResourcePath, "%s", pResourcePath);
	return g_SGUI_ResourcePath;
}

/** @brief 查询消息*/
S_GUIMSG SGUI_PollMsg(void)
{
	S_GUIMSG aMsg;
	int turnCount = 0;
	int key = pSguiDriverBase->ScanKeyboard(turnCount);

	if (key != -1)
	{
		if (key > GVK_KEYBEGIN && key < GVK_KEYEND)
			aMsg.Msg = GM_KEYUP;
		else if (key > GVK_SYSKEYBEGIN && key < GVK_SYSKEYEND)
			aMsg.Msg = GM_SYSKEY;
		aMsg.wParam = (S_WORD)key;
		if (turnCount >= 3)
		{
			turnCount *= 3;
		}
		aMsg.lParam = turnCount;
	}
	loget << "count *******" << aMsg.lParam << endl;
	
	return aMsg;
}

/** @brief 睡眠函数*/
int SGUI_SleepMS(int ms)
{
	SleepMS(ms);
	return 0;
}

#else
//---------------------------------------------------------------------------------
// Copyright Steven Wan 2009-2010
// E-mail:   Steven Wan@163.com
// Project:			SGUI(Simple GUI)
// File:			sguibase.cpp
// Description:		SGUI for Power user elect energy acquire system;unsupport multi_thread
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "sguibase.h"
#include "osrelate.h"

//
//
//
#define SGUI_LCD_WIDTH		192
#define SGUI_LCD_HEIGHT		64
#define SGUI_LCD_BPP		1
#define SGUI_LCD_LINE	(SGUI_LCD_WIDTH * SGUI_LCD_BPP / 8)
#define SGUI_LCD_SIZE	(SGUI_LCD_LINE * SGUI_LCD_HEIGHT)

#define SGUI_LCD_WIDTH_5000D		320
#define SGUI_LCD_HEIGHT_5000D		240
#define SGUI_LCD_BPP_5000D		1
#define SGUI_LCD_LINE_5000D	(SGUI_LCD_WIDTH_5000D * SGUI_LCD_BPP_5000D / 8)
#define SGUI_LCD_SIZE_5000D	(SGUI_LCD_LINE_5000D * SGUI_LCD_HEIGHT_5000D)


#define SGUI_LCD_WIDTH_5000C		160
#define SGUI_LCD_HEIGHT_5000C		160
#define SGUI_LCD_BPP_5000C		1
#define SGUI_LCD_LINE_5000C	(SGUI_LCD_WIDTH_5000C * SGUI_LCD_BPP_5000C / 8)
#define SGUI_LCD_SIZE_5000C	(SGUI_LCD_LINE_5000C * SGUI_LCD_HEIGHT_5000C)

//
//SGUI全局资源
//
char g_SGUI_ResourcePath[CFG_PATH_FILE_MAX_LEN] = "./"; //显示资源路径
int g_SGUI_ScreenDirection = 0; //屏幕方向
char SGUI_UsrBuf[SGUI_LCD_SIZE_5000D]; //用户显存 [用最大屏幕的]
char *SGUI_fb = NULL; //屏幕显存

C_SguiDriverBase g_SguiDriverBaseDefault;
C_SguiDriverEac5000D g_SguiDriverEac5000D;
C_SguiDriverEac5000C g_SguiDriverEac5000C;
C_SguiDriverCq6100 g_SguiDriverCq6100;
C_SguiDriverBase *pSguiDriverBase = &g_SguiDriverBaseDefault;

//
//显示屏幕旋转函数
//
static const unsigned char SGUI_BitReverse[256] =
{
	'\x00',	'\x80',	'\x40',	'\xc0',	'\x20',	'\xa0',	'\x60',	'\xe0',
	'\x10',	'\x90',	'\x50',	'\xd0',	'\x30',	'\xb0',	'\x70',	'\xf0',
	'\x08',	'\x88',	'\x48',	'\xc8',	'\x28',	'\xa8',	'\x68',	'\xe8',
	'\x18',	'\x98',	'\x58',	'\xd8',	'\x38',	'\xb8',	'\x78',	'\xf8',
	'\x04',	'\x84',	'\x44',	'\xc4',	'\x24',	'\xa4',	'\x64',	'\xe4',
	'\x14',	'\x94',	'\x54',	'\xd4',	'\x34',	'\xb4',	'\x74',	'\xf4',
	'\x0c',	'\x8c',	'\x4c',	'\xcc',	'\x2c',	'\xac',	'\x6c',	'\xec',
	'\x1c',	'\x9c',	'\x5c',	'\xdc',	'\x3c',	'\xbc',	'\x7c',	'\xfc',
	'\x02',	'\x82',	'\x42',	'\xc2',	'\x22',	'\xa2',	'\x62',	'\xe2',
	'\x12',	'\x92',	'\x52',	'\xd2',	'\x32',	'\xb2',	'\x72',	'\xf2',
	'\x0a',	'\x8a',	'\x4a',	'\xca',	'\x2a',	'\xaa',	'\x6a',	'\xea',
	'\x1a',	'\x9a',	'\x5a',	'\xda',	'\x3a',	'\xba',	'\x7a',	'\xfa',
	'\x06',	'\x86',	'\x46',	'\xc6',	'\x26',	'\xa6',	'\x66',	'\xe6',
	'\x16',	'\x96',	'\x56',	'\xd6',	'\x36',	'\xb6',	'\x76',	'\xf6',
	'\x0e',	'\x8e',	'\x4e',	'\xce',	'\x2e',	'\xae',	'\x6e',	'\xee',
	'\x1e',	'\x9e',	'\x5e',	'\xde',	'\x3e',	'\xbe',	'\x7e',	'\xfe',
	'\x01',	'\x81',	'\x41',	'\xc1',	'\x21',	'\xa1',	'\x61',	'\xe1',
	'\x11',	'\x91',	'\x51',	'\xd1',	'\x31',	'\xb1',	'\x71',	'\xf1',
	'\x09',	'\x89',	'\x49',	'\xc9',	'\x29',	'\xa9',	'\x69',	'\xe9',
	'\x19',	'\x99',	'\x59',	'\xd9',	'\x39',	'\xb9',	'\x79',	'\xf9',
	'\x05',	'\x85',	'\x45',	'\xc5',	'\x25',	'\xa5',	'\x65',	'\xe5',
	'\x15',	'\x95',	'\x55',	'\xd5',	'\x35',	'\xb5',	'\x75',	'\xf5',
	'\x0d',	'\x8d',	'\x4d',	'\xcd',	'\x2d',	'\xad',	'\x6d',	'\xed',
	'\x1d',	'\x9d',	'\x5d',	'\xdd',	'\x3d',	'\xbd',	'\x7d',	'\xfd',
	'\x03',	'\x83',	'\x43',	'\xc3',	'\x23',	'\xa3',	'\x63',	'\xe3',
	'\x13',	'\x93',	'\x53',	'\xd3',	'\x33',	'\xb3',	'\x73',	'\xf3',
	'\x0b',	'\x8b',	'\x4b',	'\xcb',	'\x2b',	'\xab',	'\x6b',	'\xeb',
	'\x1b',	'\x9b',	'\x5b',	'\xdb',	'\x3b',	'\xbb',	'\x7b',	'\xfb',
	'\x07',	'\x87',	'\x47',	'\xc7',	'\x27',	'\xa7',	'\x67',	'\xe7',
	'\x17',	'\x97',	'\x57',	'\xd7',	'\x37',	'\xb7',	'\x77',	'\xf7',
	'\x0f',	'\x8f',	'\x4f',	'\xcf',	'\x2f',	'\xaf',	'\x6f',	'\xef',
	'\x1f',	'\x9f',	'\x5f',	'\xdf',	'\x3f',	'\xbf',	'\x7f',	'\xff'
};
static const unsigned char SGUI_BitMask[8] =
{
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};
void ScreenDirection_PixelBitblt_1(S_BYTE Pixel, S_BYTE *pScreen, S_BYTE BitMask)
{
	if (Pixel&0x01) *(pScreen + 0*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x02) *(pScreen + 1*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x04) *(pScreen + 2*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x08) *(pScreen + 3*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x10) *(pScreen + 4*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x20) *(pScreen + 5*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x40) *(pScreen + 6*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x80) *(pScreen + 7*SGUI_LCD_LINE) |= BitMask;
}
void ScreenDirection_PixelBitblt_3(S_BYTE Pixel, S_BYTE *pScreen, S_BYTE BitMask)
{
	if (Pixel&0x80) *(pScreen + 0*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x40) *(pScreen + 1*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x20) *(pScreen + 2*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x10) *(pScreen + 3*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x08) *(pScreen + 4*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x04) *(pScreen + 5*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x02) *(pScreen + 6*SGUI_LCD_LINE) |= BitMask;
	if (Pixel&0x01) *(pScreen + 7*SGUI_LCD_LINE) |= BitMask;
}
void ScreenDirection_0(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	memcpy(pScreenBuffer, pUserBuffer, SGUI_LCD_SIZE);
}
void ScreenDirection_1(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	memset(pScreenBuffer, 0x00, SGUI_LCD_SIZE);
	for (int m = 0; m < SGUI_LCD_HEIGHT; ++m)
	{
		const S_BYTE *pSrc = pUserBuffer + m * SGUI_LCD_LINE;
		S_WORD DestX = (SGUI_LCD_HEIGHT - 1 - m) % 8;
		S_BYTE *pDst = pScreenBuffer + (SGUI_LCD_HEIGHT - 1 - m) / 8;
		if (*(pSrc+0)) ScreenDirection_PixelBitblt_1(*(pSrc+0), pDst+0*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+1)) ScreenDirection_PixelBitblt_1(*(pSrc+1), pDst+1*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+2)) ScreenDirection_PixelBitblt_1(*(pSrc+2), pDst+2*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+3)) ScreenDirection_PixelBitblt_1(*(pSrc+3), pDst+3*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+4)) ScreenDirection_PixelBitblt_1(*(pSrc+4), pDst+4*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+5)) ScreenDirection_PixelBitblt_1(*(pSrc+5), pDst+5*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+6)) ScreenDirection_PixelBitblt_1(*(pSrc+6), pDst+6*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+7)) ScreenDirection_PixelBitblt_1(*(pSrc+7), pDst+7*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+8)) ScreenDirection_PixelBitblt_1(*(pSrc+8), pDst+8*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+9)) ScreenDirection_PixelBitblt_1(*(pSrc+9), pDst+9*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+10)) ScreenDirection_PixelBitblt_1(*(pSrc+10), pDst+10*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+11)) ScreenDirection_PixelBitblt_1(*(pSrc+11), pDst+11*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+12)) ScreenDirection_PixelBitblt_1(*(pSrc+12), pDst+12*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+13)) ScreenDirection_PixelBitblt_1(*(pSrc+13), pDst+13*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+14)) ScreenDirection_PixelBitblt_1(*(pSrc+14), pDst+14*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+15)) ScreenDirection_PixelBitblt_1(*(pSrc+15), pDst+15*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+16)) ScreenDirection_PixelBitblt_1(*(pSrc+16), pDst+16*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+17)) ScreenDirection_PixelBitblt_1(*(pSrc+17), pDst+17*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+18)) ScreenDirection_PixelBitblt_1(*(pSrc+18), pDst+18*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+19)) ScreenDirection_PixelBitblt_1(*(pSrc+19), pDst+19*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
	}
}
void ScreenDirection_2(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	for (int m = 0; m < SGUI_LCD_HEIGHT; ++m)
	{
		const S_BYTE *pSrc = pUserBuffer + m * SGUI_LCD_LINE;
		S_BYTE *pDst = pScreenBuffer + (SGUI_LCD_HEIGHT - 1 - m) * SGUI_LCD_LINE;
		*(pDst+19) = SGUI_BitReverse[*(pSrc+0)];
		*(pDst+18) = SGUI_BitReverse[*(pSrc+1)];
		*(pDst+17) = SGUI_BitReverse[*(pSrc+2)];
		*(pDst+16) = SGUI_BitReverse[*(pSrc+3)];
		*(pDst+15) = SGUI_BitReverse[*(pSrc+4)];
		*(pDst+14) = SGUI_BitReverse[*(pSrc+5)];
		*(pDst+13) = SGUI_BitReverse[*(pSrc+6)];
		*(pDst+12) = SGUI_BitReverse[*(pSrc+7)];
		*(pDst+11) = SGUI_BitReverse[*(pSrc+8)];
		*(pDst+10) = SGUI_BitReverse[*(pSrc+9)];
		*(pDst+9) = SGUI_BitReverse[*(pSrc+10)];
		*(pDst+8) = SGUI_BitReverse[*(pSrc+11)];
		*(pDst+7) = SGUI_BitReverse[*(pSrc+12)];
		*(pDst+6) = SGUI_BitReverse[*(pSrc+13)];
		*(pDst+5) = SGUI_BitReverse[*(pSrc+14)];
		*(pDst+4) = SGUI_BitReverse[*(pSrc+15)];
		*(pDst+3) = SGUI_BitReverse[*(pSrc+16)];
		*(pDst+2) = SGUI_BitReverse[*(pSrc+17)];
		*(pDst+1) = SGUI_BitReverse[*(pSrc+18)];
		*(pDst+0) = SGUI_BitReverse[*(pSrc+19)];
	}
}
void ScreenDirection_3(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	memset(pScreenBuffer, 0x00, SGUI_LCD_SIZE);
	for (int m = 0; m < SGUI_LCD_HEIGHT; ++m)
	{
		const S_BYTE *pSrc = pUserBuffer + m * SGUI_LCD_LINE;
		S_WORD DestX = m % 8;
		S_BYTE *pDst = pScreenBuffer + m / 8;
		if (*(pSrc+19)) ScreenDirection_PixelBitblt_3(*(pSrc+19), pDst+0*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+18)) ScreenDirection_PixelBitblt_3(*(pSrc+18), pDst+1*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+17)) ScreenDirection_PixelBitblt_3(*(pSrc+17), pDst+2*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+16)) ScreenDirection_PixelBitblt_3(*(pSrc+16), pDst+3*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+15)) ScreenDirection_PixelBitblt_3(*(pSrc+15), pDst+4*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+14)) ScreenDirection_PixelBitblt_3(*(pSrc+14), pDst+5*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+13)) ScreenDirection_PixelBitblt_3(*(pSrc+13), pDst+6*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+12)) ScreenDirection_PixelBitblt_3(*(pSrc+12), pDst+7*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+11)) ScreenDirection_PixelBitblt_3(*(pSrc+11), pDst+8*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+10)) ScreenDirection_PixelBitblt_3(*(pSrc+10), pDst+9*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+9)) ScreenDirection_PixelBitblt_3(*(pSrc+9), pDst+10*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+8)) ScreenDirection_PixelBitblt_3(*(pSrc+8), pDst+11*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+7)) ScreenDirection_PixelBitblt_3(*(pSrc+7), pDst+12*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+6)) ScreenDirection_PixelBitblt_3(*(pSrc+6), pDst+13*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+5)) ScreenDirection_PixelBitblt_3(*(pSrc+5), pDst+14*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+4)) ScreenDirection_PixelBitblt_3(*(pSrc+4), pDst+15*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+3)) ScreenDirection_PixelBitblt_3(*(pSrc+3), pDst+16*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+2)) ScreenDirection_PixelBitblt_3(*(pSrc+2), pDst+17*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+1)) ScreenDirection_PixelBitblt_3(*(pSrc+1), pDst+18*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
		if (*(pSrc+0)) ScreenDirection_PixelBitblt_3(*(pSrc+0), pDst+19*8*SGUI_LCD_LINE, SGUI_BitMask[DestX]);
	}
}

void ScreenChange_ByteReverse(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	for (int m = 0; m < SGUI_LCD_HEIGHT_5000C; ++m)
	{
		const S_BYTE *pSrc = pUserBuffer + m * SGUI_LCD_LINE_5000C;
		S_BYTE *pDst = pScreenBuffer + m * SGUI_LCD_LINE_5000C;
		*(pDst+0) = SGUI_BitReverse[*(pSrc+0)];
		*(pDst+1) = SGUI_BitReverse[*(pSrc+1)];
		*(pDst+2) = SGUI_BitReverse[*(pSrc+2)];
		*(pDst+3) = SGUI_BitReverse[*(pSrc+3)];
		*(pDst+4) = SGUI_BitReverse[*(pSrc+4)];
		*(pDst+5) = SGUI_BitReverse[*(pSrc+5)];
		*(pDst+6) = SGUI_BitReverse[*(pSrc+6)];
		*(pDst+7) = SGUI_BitReverse[*(pSrc+7)];
		*(pDst+8) = SGUI_BitReverse[*(pSrc+8)];
		*(pDst+9) = SGUI_BitReverse[*(pSrc+9)];
		*(pDst+10) = SGUI_BitReverse[*(pSrc+10)];
		*(pDst+11) = SGUI_BitReverse[*(pSrc+11)];
		*(pDst+12) = SGUI_BitReverse[*(pSrc+12)];
		*(pDst+13) = SGUI_BitReverse[*(pSrc+13)];
		*(pDst+14) = SGUI_BitReverse[*(pSrc+14)];
		*(pDst+15) = SGUI_BitReverse[*(pSrc+15)];
		*(pDst+16) = SGUI_BitReverse[*(pSrc+16)];
		*(pDst+17) = SGUI_BitReverse[*(pSrc+17)];
		*(pDst+18) = SGUI_BitReverse[*(pSrc+18)];
		*(pDst+19) = SGUI_BitReverse[*(pSrc+19)];
	}
}

void ScreenChange_LineReverse(S_BYTE *pScreenBuffer, const S_BYTE *pUserBuffer)
{
	for (int m = 0; m < SGUI_LCD_HEIGHT; ++m)
	{
		const S_BYTE *pSrc = pUserBuffer + m * SGUI_LCD_LINE;
		S_BYTE *pDst = pScreenBuffer + m * SGUI_LCD_LINE;
		*(pDst+0) = SGUI_BitReverse[*(pSrc+19)];
		*(pDst+1) = SGUI_BitReverse[*(pSrc+18)];
		*(pDst+2) = SGUI_BitReverse[*(pSrc+17)];
		*(pDst+3) = SGUI_BitReverse[*(pSrc+16)];
		*(pDst+4) = SGUI_BitReverse[*(pSrc+15)];
		*(pDst+5) = SGUI_BitReverse[*(pSrc+14)];
		*(pDst+6) = SGUI_BitReverse[*(pSrc+13)];
		*(pDst+7) = SGUI_BitReverse[*(pSrc+12)];
		*(pDst+8) = SGUI_BitReverse[*(pSrc+11)];
		*(pDst+9) = SGUI_BitReverse[*(pSrc+10)];
		*(pDst+10) = SGUI_BitReverse[*(pSrc+9)];
		*(pDst+11) = SGUI_BitReverse[*(pSrc+8)];
		*(pDst+12) = SGUI_BitReverse[*(pSrc+7)];
		*(pDst+13) = SGUI_BitReverse[*(pSrc+6)];
		*(pDst+14) = SGUI_BitReverse[*(pSrc+5)];
		*(pDst+15) = SGUI_BitReverse[*(pSrc+4)];
		*(pDst+16) = SGUI_BitReverse[*(pSrc+3)];
		*(pDst+17) = SGUI_BitReverse[*(pSrc+2)];
		*(pDst+18) = SGUI_BitReverse[*(pSrc+1)];
		*(pDst+19) = SGUI_BitReverse[*(pSrc+0)];
	}
}

#ifdef WIN32

//
// 基础UI驱动
//
int C_SguiDriverBase::DriverID(void)
{
	return SGUI_DRIVER_ID_DEFAULT;
}
unsigned short C_SguiDriverBase::LcdWidth(void)
{
	return SGUI_LCD_WIDTH;
}
unsigned short C_SguiDriverBase::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT;
}
unsigned short C_SguiDriverBase::LcdBpp(void)
{
	return SGUI_LCD_BPP;
}

int C_SguiDriverBase::fbinit(void)
{
	return 0;
}

int C_SguiDriverBase::fbfresh(void)
{
	S_BYTE C[] = {'\xFF', '\x00'};
	S_BYTE ScreenBuffer[SGUI_LCD_SIZE]; //屏幕缓冲

	switch (g_SGUI_ScreenDirection)
	{
	default:
	case 0:ScreenDirection_0(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 1:ScreenDirection_1(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 2:ScreenDirection_2(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 3:ScreenDirection_3(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	}

	for (int m = 0; m < SGUI_LCD_HEIGHT; ++m)
	{
		S_BYTE *pSrc = (S_BYTE *)ScreenBuffer + m * SGUI_LCD_LINE;
		S_BYTE *pDst = (S_BYTE *)SGUI_fb + m * (480*3);
		for (int n = 0; n < SGUI_LCD_LINE; ++n)
		{
			S_BYTE pixels = *(pSrc + n);
			*(pDst+0) = C[(pixels>>0)&0x01];*(pDst+1) = C[(pixels>>0)&0x01];*(pDst+2) = C[(pixels>>0)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>1)&0x01];*(pDst+1) = C[(pixels>>1)&0x01];*(pDst+2) = C[(pixels>>1)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>2)&0x01];*(pDst+1) = C[(pixels>>2)&0x01];*(pDst+2) = C[(pixels>>2)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>3)&0x01];*(pDst+1) = C[(pixels>>3)&0x01];*(pDst+2) = C[(pixels>>3)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>4)&0x01];*(pDst+1) = C[(pixels>>4)&0x01];*(pDst+2) = C[(pixels>>4)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>5)&0x01];*(pDst+1) = C[(pixels>>5)&0x01];*(pDst+2) = C[(pixels>>5)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>6)&0x01];*(pDst+1) = C[(pixels>>6)&0x01];*(pDst+2) = C[(pixels>>6)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>7)&0x01];*(pDst+1) = C[(pixels>>7)&0x01];*(pDst+2) = C[(pixels>>7)&0x01];pDst += 3;
		}
	}
	return 0;
}

void C_SguiDriverBase::backlight(int on) //on =1 开
{
}

void C_SguiDriverBase::SetContrast(int contrast)
{
}

void C_SguiDriverBase::SetBrightness(int brightness)
{
}

void C_SguiDriverBase::SetGrayness(int grayness)
{
}

void C_SguiDriverBase::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverBase::OpenScreen(void)
{
}

void C_SguiDriverBase::CloseScreen(void)
{
}

int C_SguiDriverBase::kbinit(void)
{
	return 0;
}

char SGUI_key = 0;
int C_SguiDriverBase::ScanKeyboard(void)
{
	int key = -1;

	if (SGUI_key)
	{
		key = SGUI_key;
		SGUI_key = 0;
	}
	return key;
}

//
// 重庆6100UI驱动
//
int C_SguiDriverCq6100::DriverID(void)
{
	return SGUI_DRIVER_ID_CQ_6100;
}
unsigned short C_SguiDriverCq6100::LcdWidth(void)
{
	return SGUI_LCD_WIDTH;
}
unsigned short C_SguiDriverCq6100::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT;
}
unsigned short C_SguiDriverCq6100::LcdBpp(void)
{
	return SGUI_LCD_BPP;
}

int C_SguiDriverCq6100::fbinit(void)
{
	return 0;
}

int C_SguiDriverCq6100::fbfresh(void)
{
	return C_SguiDriverBase::fbfresh();
}

void C_SguiDriverCq6100::backlight(int on) //on =1 开
{
}

void C_SguiDriverCq6100::SetContrast(int contrast)
{
}

void C_SguiDriverCq6100::SetBrightness(int brightness)
{
}

void C_SguiDriverCq6100::SetGrayness(int grayness)
{
}

void C_SguiDriverCq6100::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverCq6100::OpenScreen(void)
{
}

void C_SguiDriverCq6100::CloseScreen(void)
{
}

int C_SguiDriverCq6100::kbinit(void)
{
	return 0;
}

int C_SguiDriverCq6100::ScanKeyboard(void)
{
	return C_SguiDriverBase::ScanKeyboard();
}

//
// EAC5000D驱动
//
int C_SguiDriverEac5000D::DriverID(void)
{
	return SGUI_DRIVER_ID_EAC_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdWidth(void)
{
	return SGUI_LCD_WIDTH_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdBpp(void)
{
	return SGUI_LCD_BPP_5000D;
}

int C_SguiDriverEac5000D::fbinit(void)
{
	return 0;
}

int C_SguiDriverEac5000D::fbfresh(void)
{
	S_BYTE C[] = {'\xFF', '\x00'};
	for (int m = 0; m < SGUI_LCD_HEIGHT_5000D; ++m)
	{
		S_BYTE *pSrc = (S_BYTE *)SGUI_UsrBuf + m * SGUI_LCD_LINE_5000D;
		S_BYTE *pDst = (S_BYTE *)SGUI_fb + m * (480*3);
		for (int n = 0; n < SGUI_LCD_LINE_5000D; ++n)
		{
			S_BYTE pixels = *(pSrc + n);
			*(pDst+0) = C[(pixels>>0)&0x01];*(pDst+1) = C[(pixels>>0)&0x01];*(pDst+2) = C[(pixels>>0)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>1)&0x01];*(pDst+1) = C[(pixels>>1)&0x01];*(pDst+2) = C[(pixels>>1)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>2)&0x01];*(pDst+1) = C[(pixels>>2)&0x01];*(pDst+2) = C[(pixels>>2)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>3)&0x01];*(pDst+1) = C[(pixels>>3)&0x01];*(pDst+2) = C[(pixels>>3)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>4)&0x01];*(pDst+1) = C[(pixels>>4)&0x01];*(pDst+2) = C[(pixels>>4)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>5)&0x01];*(pDst+1) = C[(pixels>>5)&0x01];*(pDst+2) = C[(pixels>>5)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>6)&0x01];*(pDst+1) = C[(pixels>>6)&0x01];*(pDst+2) = C[(pixels>>6)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>7)&0x01];*(pDst+1) = C[(pixels>>7)&0x01];*(pDst+2) = C[(pixels>>7)&0x01];pDst += 3;
		}
	}
	return 0;
}

void C_SguiDriverEac5000D::backlight(int on) //on =1 开
{
}

void C_SguiDriverEac5000D::SetContrast(int contrast)
{
}

void C_SguiDriverEac5000D::SetBrightness(int brightness)
{
}

void C_SguiDriverEac5000D::SetGrayness(int grayness)
{
}

void C_SguiDriverEac5000D::SetDirection(int direction)
{
}

void C_SguiDriverEac5000D::OpenScreen(void)
{
}

void C_SguiDriverEac5000D::CloseScreen(void)
{
}

int C_SguiDriverEac5000D::kbinit(void)
{
	return 0;
}

int C_SguiDriverEac5000D::ScanKeyboard(void)
{
	return _ScanKeyboard();
}

int C_SguiDriverEac5000D::_ScanKeyboard(void)
{
	int key = -1;

	if (SGUI_key)
	{
		key = SGUI_key;
		SGUI_key = 0;
	}
	return key;
}

//
// EAC5000C驱动
//
int C_SguiDriverEac5000C::DriverID(void)
{
	return SGUI_DRIVER_ID_EAC_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdWidth(void)
{
	return SGUI_LCD_WIDTH_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdBpp(void)
{
	return SGUI_LCD_BPP_5000C;
}

int C_SguiDriverEac5000C::fbinit(void)
{
	return 0;
}

int C_SguiDriverEac5000C::fbfresh(void)
{
	S_BYTE C[] = {'\xFF', '\x00'};
	S_BYTE ScreenBuffer[SGUI_LCD_SIZE_5000C]; //屏幕缓冲

	switch (g_SGUI_ScreenDirection)
	{
	default:
	case 0:ScreenDirection_0(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 1:ScreenDirection_1(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 2:ScreenDirection_2(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	case 3:ScreenDirection_3(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);break;
	}

	for (int m = 0; m < SGUI_LCD_HEIGHT_5000C; ++m)
	{
		S_BYTE *pSrc = (S_BYTE *)ScreenBuffer + m * SGUI_LCD_LINE_5000C;
		S_BYTE *pDst = (S_BYTE *)SGUI_fb + m * (480*3);
		for (int n = 0; n < SGUI_LCD_LINE_5000C; ++n)
		{
			S_BYTE pixels = *(pSrc + n);
			*(pDst+0) = C[(pixels>>0)&0x01];*(pDst+1) = C[(pixels>>0)&0x01];*(pDst+2) = C[(pixels>>0)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>1)&0x01];*(pDst+1) = C[(pixels>>1)&0x01];*(pDst+2) = C[(pixels>>1)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>2)&0x01];*(pDst+1) = C[(pixels>>2)&0x01];*(pDst+2) = C[(pixels>>2)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>3)&0x01];*(pDst+1) = C[(pixels>>3)&0x01];*(pDst+2) = C[(pixels>>3)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>4)&0x01];*(pDst+1) = C[(pixels>>4)&0x01];*(pDst+2) = C[(pixels>>4)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>5)&0x01];*(pDst+1) = C[(pixels>>5)&0x01];*(pDst+2) = C[(pixels>>5)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>6)&0x01];*(pDst+1) = C[(pixels>>6)&0x01];*(pDst+2) = C[(pixels>>6)&0x01];pDst += 3;
			*(pDst+0) = C[(pixels>>7)&0x01];*(pDst+1) = C[(pixels>>7)&0x01];*(pDst+2) = C[(pixels>>7)&0x01];pDst += 3;
		}
	}
	return 0;
}

void C_SguiDriverEac5000C::backlight(int on) //on =1 开
{
}

void C_SguiDriverEac5000C::SetContrast(int contrast)
{
}

void C_SguiDriverEac5000C::SetBrightness(int brightness)
{
}

void C_SguiDriverEac5000C::SetGrayness(int grayness)
{
}

void C_SguiDriverEac5000C::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverEac5000C::OpenScreen(void)
{
}

void C_SguiDriverEac5000C::CloseScreen(void)
{
}

int C_SguiDriverEac5000C::kbinit(void)
{
	return 0;
}

int C_SguiDriverEac5000C::ScanKeyboard(void)
{
	int key = _ScanKeyboard();
	switch(key)
	{
	case 1: key = GVK_UP; break;
	case 2: key = GVK_DN; break;
	case 3: key = GVK_LEFT; break;
	case 4: key = GVK_RIGHT; break;
	default: key = -1; break;
	}
	return key;
}

int C_SguiDriverEac5000C::_ScanKeyboard(void)
{
	int key = -1;

	if (SGUI_key)
	{
		key = SGUI_key;
		SGUI_key = 0;
	}
	return key;
}

#else

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

/** @brief 缺省驱动
 */
#define FBDEV  "/dev/fb0"
#define BUTTONDEV "/dev/kgeio"

#define ST7529FB_IOC_MAGIC          'S'
#define ST7529FB_IOC_RESET          _IO(ST7529FB_IOC_MAGIC, 0)
#define ST7529FB_IOC_REFRESH        _IO(ST7529FB_IOC_MAGIC, 1)
#define ST7529FB_IOC_BACKLIGHT      _IO(ST7529FB_IOC_MAGIC, 2)
#define ST7529FB_IOC_SETCONTRAST      _IO(ST7529FB_IOC_MAGIC, 3)
#define ST7529FB_IOC_SETGRAYSCALE      _IO(ST7529FB_IOC_MAGIC, 4)

int SGUI_fd = -1;
int SGUI_kd = -1;

int C_SguiDriverBase::DriverID(void)
{
	return SGUI_DRIVER_ID_DEFAULT;
}
unsigned short C_SguiDriverBase::LcdWidth(void)
{
	return SGUI_LCD_WIDTH;
}
unsigned short C_SguiDriverBase::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT;
}
unsigned short C_SguiDriverBase::LcdBpp(void)
{
	return SGUI_LCD_BPP;
}

int C_SguiDriverBase::fbinit(void)
{
	if (SGUI_fd >= 0)// 初始化完成了
		return 0;

	SGUI_fd = open(FBDEV, O_RDWR);
	if(SGUI_fd<0)	{
		printf(FBDEV"Open failed!\n");
	}

	SGUI_fb = (char *)mmap(0, SGUI_LCD_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, SGUI_fd, 0);
	if(SGUI_fb == NULL){
		printf("Map %s failed!\n", FBDEV);
	}

	memset(SGUI_UsrBuf, 0x00, SGUI_LCD_SIZE);
	fbfresh();

	return 0;
}

int C_SguiDriverBase::fbfresh(void)
{
	switch (g_SGUI_ScreenDirection)
	{
	default:
	case 0:ScreenDirection_0((S_BYTE *)SGUI_fb, (S_BYTE *)SGUI_UsrBuf);break;
	case 1:ScreenDirection_1((S_BYTE *)SGUI_fb, (S_BYTE *)SGUI_UsrBuf);break;
	case 2:ScreenDirection_2((S_BYTE *)SGUI_fb, (S_BYTE *)SGUI_UsrBuf);break;
	case 3:ScreenDirection_3((S_BYTE *)SGUI_fb, (S_BYTE *)SGUI_UsrBuf);break;
	}
	ioctl(SGUI_fd, ST7529FB_IOC_REFRESH);
	return 0;
}

void C_SguiDriverBase::backlight(int on) //on =1 开
{
	extern int io_WriteCmd(const char *pCmd, int state);
	io_WriteCmd("backlight", on);
}

void C_SguiDriverBase::SetContrast(int contrast)
{
	if (SGUI_fd >= 0)
		ioctl(SGUI_fd, ST7529FB_IOC_SETCONTRAST, contrast);
}

void C_SguiDriverBase::SetBrightness(int brightness)
{
}

void C_SguiDriverBase::SetGrayness(int grayness)
{
	if (SGUI_fd >= 0)
		ioctl(SGUI_fd, ST7529FB_IOC_SETGRAYSCALE, grayness);
}

void C_SguiDriverBase::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverBase::OpenScreen(void)
{
}

void C_SguiDriverBase::CloseScreen(void)
{
}

int C_SguiDriverBase::kbinit(void)
{
	if (SGUI_kd >= 0)// 初始化完成了
		return 0;

	SGUI_kd = open(BUTTONDEV, O_RDWR);
	if (SGUI_kd < 0)
	{
		printf(BUTTONDEV"open failed!\n");
		return -1;
	}
	return 0;
}

int C_SguiDriverBase::ScanKeyboard(void)
{
	int key = _ScanKeyboard();
	if (key > GVK_KEYBEGIN && key < GVK_KEYEND)
		return key;


	return -1;
}

int C_SguiDriverBase::_ScanKeyboard(void)
{
	int event;
	fd_set rset;
	struct timeval tv;

	if (SGUI_kd <= 0)
		return -1;

	FD_ZERO(&rset);
	FD_SET(SGUI_kd, &rset);

	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	if (select(SGUI_kd + 1, &rset, NULL, NULL, &tv) > 0) 
	{
		read(SGUI_kd, &event, 1);
		return (event&0xFF);
	}

	return -1;
}


/** @brief CQ6100驱动
 */
int C_SguiDriverCq6100::DriverID(void)
{
	return SGUI_DRIVER_ID_CQ_6100;
}
unsigned short C_SguiDriverCq6100::LcdWidth(void)
{
	return SGUI_LCD_WIDTH;
}
unsigned short C_SguiDriverCq6100::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT;
}
unsigned short C_SguiDriverCq6100::LcdBpp(void)
{
	return SGUI_LCD_BPP;
}

int C_SguiDriverCq6100::fbinit(void)
{
	if (SGUI_fd >= 0)// 初始化完成了
		return 0;

	SGUI_fd = open(FBDEV, O_RDWR);
	if(SGUI_fd<0)	{
		printf(FBDEV"Open failed!\n");
	}

	SGUI_fb = (char *)mmap(0, SGUI_LCD_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, SGUI_fd, 0);
	if(SGUI_fb == NULL){
		printf("Map %s failed!\n", FBDEV);
	}

	memset(SGUI_UsrBuf, 0x00, SGUI_LCD_SIZE);
	fbfresh();

	return 0;
}

int C_SguiDriverCq6100::fbfresh(void)
{
	S_BYTE ScreenBuffer[SGUI_LCD_SIZE]; //屏幕缓冲
	S_BYTE ScreenBuffer2[SGUI_LCD_SIZE]; //屏幕缓冲
	ScreenChange_LineReverse(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);
	ScreenDirection_3((S_BYTE *)ScreenBuffer2, (S_BYTE *)ScreenBuffer);
	ScreenChange_ByteReverse((S_BYTE *)SGUI_fb, (S_BYTE *)ScreenBuffer2);
	ioctl(SGUI_fd, ST7529FB_IOC_REFRESH);
	return 0;
}

void C_SguiDriverCq6100::backlight(int on) //on =1 开
{
#define KGESG_IO_MAGIC        'K'
#define KGESG_IO_LCDBL_ON             _IO(KGESG_IO_MAGIC, 28)
#define KGESG_IO_LCDBL_OFF            _IO(KGESG_IO_MAGIC, 29)
	int fd = open(BUTTONDEV, O_RDWR);
	if(fd < 0)
	{
		printf(BUTTONDEV"open failed!\n");
	}
	else
	{
		if (on != 0)
			ioctl(fd, KGESG_IO_LCDBL_ON);
		else
			ioctl(fd, KGESG_IO_LCDBL_OFF);
		close(fd);
	}
}

void C_SguiDriverCq6100::SetContrast(int contrast)
{
}

void C_SguiDriverCq6100::SetBrightness(int brightness)
{
}

void C_SguiDriverCq6100::SetGrayness(int grayness)
{
}

void C_SguiDriverCq6100::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverCq6100::OpenScreen(void)
{
}

void C_SguiDriverCq6100::CloseScreen(void)
{
}

int C_SguiDriverCq6100::kbinit(void)
{
	if (SGUI_kd >= 0)// 初始化完成了
		return 0;

	SGUI_kd = open(BUTTONDEV, O_RDWR);
	if (SGUI_kd < 0)
	{
		printf(BUTTONDEV"open failed!\n");
		return -1;
	}
	return 0;
}

int C_SguiDriverCq6100::ScanKeyboard(void)
{
	int key = _ScanKeyboard();
	if (key > GVK_KEYBEGIN && key < GVK_KEYEND)
		return key;

	return -1;
}


/** @brief EAC5000D驱动
 */
#define FBDEV_EAC5000D  "/dev/fb/0" //ZHGX 旧驱动
#define BUTTONDEV_EAC5000D "/dev/misc/button"//ZHGX 旧驱动

int C_SguiDriverEac5000D::DriverID(void)
{
	return SGUI_DRIVER_ID_EAC_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdWidth(void)
{
	return SGUI_LCD_WIDTH_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT_5000D;
}
unsigned short C_SguiDriverEac5000D::LcdBpp(void)
{
	return SGUI_LCD_BPP_5000D;
}

int C_SguiDriverEac5000D::fbinit(void)
{
	if (SGUI_fd >= 0)// 初始化完成了
		return 0;

	SGUI_fd = open(FBDEV_EAC5000D, O_RDWR);
	if(SGUI_fd<0)	{
		printf(FBDEV_EAC5000D" Open failed!\n");
	}
	//实际屏幕是320*240，16位色
	SGUI_fb = (char *)mmap(0, SGUI_LCD_WIDTH_5000D*SGUI_LCD_HEIGHT_5000D*2, PROT_READ|PROT_WRITE, MAP_SHARED, SGUI_fd, 0);
	if(SGUI_fb == NULL){
		printf("Map %s failed!\n", FBDEV_EAC5000D);
	}

	memset(SGUI_UsrBuf, 0x00, SGUI_LCD_SIZE_5000D);
	fbfresh();

	return 0;
}

int C_SguiDriverEac5000D::fbfresh(void)
{
	S_WORD C[] = {0xFFFF, 0x202F};
	for (int m = 0; m < SGUI_LCD_HEIGHT_5000D; ++m)
	{
		S_BYTE *pSrc = (S_BYTE *)SGUI_UsrBuf + m * SGUI_LCD_LINE_5000D;
		S_BYTE *pDst = (S_BYTE *)SGUI_fb + m * (SGUI_LCD_WIDTH_5000D*2);
		S_WORD *pScreen = (S_WORD *)pDst;
		for (int n = 0; n < SGUI_LCD_LINE_5000D; ++n)
		{
			S_BYTE pixels = *(pSrc + n);
			*(pScreen+0) = C[(pixels>>0)&0x01];
			*(pScreen+1) = C[(pixels>>1)&0x01];
			*(pScreen+2) = C[(pixels>>2)&0x01];
			*(pScreen+3) = C[(pixels>>3)&0x01];
			*(pScreen+4) = C[(pixels>>4)&0x01];
			*(pScreen+5) = C[(pixels>>5)&0x01];
			*(pScreen+6) = C[(pixels>>6)&0x01];
			*(pScreen+7) = C[(pixels>>7)&0x01];

			pScreen += 8;
		}
	}
	return 0;
}

#define IO_DEV_EAC5000D "/dev/misc/io"
void C_SguiDriverEac5000D::backlight(int on) //on =1 开
{
	int func = 0x665e;//EAC5000D
	int io_btfd = open(IO_DEV_EAC5000D, O_RDWR);

	if(io_btfd < 0)
	{
		printf("Can't open "IO_DEV_EAC5000D"!\n");
	}
	if(ioctl(io_btfd, func, !on) != 0)
	{
		printf("%s lcd light error\n",on==0?"close":"open");
	}
	close(io_btfd);
}

void C_SguiDriverEac5000D::SetContrast(int contrast)
{
}

void C_SguiDriverEac5000D::SetBrightness(int brightness)
{
}

void C_SguiDriverEac5000D::SetGrayness(int grayness)
{
}

void C_SguiDriverEac5000D::SetDirection(int direction)
{
}

void C_SguiDriverEac5000D::OpenScreen(void)
{
}

void C_SguiDriverEac5000D::CloseScreen(void)
{
}

int C_SguiDriverEac5000D::kbinit(void)
{
	if (SGUI_kd >= 0)// 初始化完成了
		return 0;

	SGUI_kd = open(BUTTONDEV_EAC5000D, O_RDWR);
	if (SGUI_kd < 0)
	{
		printf(BUTTONDEV" open failed!\n");
		return -1;
	}
	return 0;
}

int C_SguiDriverEac5000D::ScanKeyboard(void)
{
	int key = _ScanKeyboard();
	if (key < 0)
		return key;

	switch(key)
	{
	case 0:key = GVK_UP;break;
	case 1:key = GVK_DN;break;
	case 2:key = GVK_LEFT;break;
	case 3:key = GVK_RIGHT;break;
	case 4:key = GVK_PGUP;break;
	case 5:key = GVK_PGDN;break;
	case 6:key = GVK_CANCEL;break;
	case 7:key = GVK_OK;break;
	default:key = -1;break;
	}
	return key;
}

int C_SguiDriverEac5000D::_ScanKeyboard(void)
{
	int event;
	fd_set rset;
	struct timeval tv;

	if (SGUI_kd <= 0)
		return -1;

	FD_ZERO(&rset);
	FD_SET(SGUI_kd, &rset);

	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	if (select(SGUI_kd + 1, &rset, NULL, NULL, &tv) > 0) 
	{
		read(SGUI_kd, &event, 1);
		return (event&0xFF);
	}

	return -1;
}

/** @brief EAC5000C驱动
 */
#define FBDEV_EAC5000C  "/dev/misc/lcd" //ZHGX 旧驱动
#define BUTTONDEV_EAC5000C "/dev/misc/button"//ZHGX 旧驱动
#define IOGET_LCDPARAM			0x8002
#define IOSET_REFRESH			0x8001

int C_SguiDriverEac5000C::DriverID(void)
{
	return SGUI_DRIVER_ID_EAC_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdWidth(void)
{
	return SGUI_LCD_WIDTH_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdHeight(void)
{
	return SGUI_LCD_HEIGHT_5000C;
}
unsigned short C_SguiDriverEac5000C::LcdBpp(void)
{
	return SGUI_LCD_BPP_5000C;
}

int C_SguiDriverEac5000C::fbinit(void)
{
	if (SGUI_fd >= 0)// 初始化完成了
		return 0;

	SGUI_fd = open(FBDEV_EAC5000C, O_RDWR);
	if(SGUI_fd<0)	{
		printf(FBDEV_EAC5000C" Open failed!\n");
	}
	SGUI_fb = (char *)mmap(0, SGUI_LCD_SIZE_5000C, PROT_READ|PROT_WRITE, MAP_SHARED, SGUI_fd, 0);
	if(SGUI_fb == NULL){
		printf("Map %s failed!\n", FBDEV_EAC5000C);
	}

	memset(SGUI_UsrBuf, 0x00, SGUI_LCD_SIZE_5000C);
	fbfresh();

	return 0;
}

int C_SguiDriverEac5000C::fbfresh(void)
{
	S_BYTE ScreenBuffer[SGUI_LCD_SIZE_5000C]; //屏幕缓冲
	ScreenChange_ByteReverse(ScreenBuffer, (S_BYTE *)SGUI_UsrBuf);
	switch (g_SGUI_ScreenDirection)
	{
	default:
	case 0:ScreenDirection_0((S_BYTE *)SGUI_fb, (S_BYTE *)ScreenBuffer);break;
	case 1:ScreenDirection_1((S_BYTE *)SGUI_fb, (S_BYTE *)ScreenBuffer);break;
	case 2:ScreenDirection_2((S_BYTE *)SGUI_fb, (S_BYTE *)ScreenBuffer);break;
	case 3:ScreenDirection_3((S_BYTE *)SGUI_fb, (S_BYTE *)ScreenBuffer);break;
	}
	ioctl(SGUI_fd, IOSET_REFRESH,0);
	return 0;
}

#define IO_DEV_EAC5000C "/dev/misc/io"
void C_SguiDriverEac5000C::backlight(int on) //on =1 开
{
	int func = 0x665e;//EAC5000D
	int io_btfd = open(IO_DEV_EAC5000C, O_RDWR);

	if(io_btfd < 0)
	{
		printf("Can't open "IO_DEV_EAC5000C"!\n");
	}
	if(ioctl(io_btfd, func, !on) != 0)
	{
		printf("%s lcd light error\n",on==0?"close":"open");
	}
	close(io_btfd);
}

void C_SguiDriverEac5000C::SetContrast(int contrast)
{
}

void C_SguiDriverEac5000C::SetBrightness(int brightness)
{
}

void C_SguiDriverEac5000C::SetGrayness(int grayness)
{
}

void C_SguiDriverEac5000C::SetDirection(int direction)
{
	g_SGUI_ScreenDirection = direction;
}

void C_SguiDriverEac5000C::OpenScreen(void)
{
}

void C_SguiDriverEac5000C::CloseScreen(void)
{
}

int C_SguiDriverEac5000C::kbinit(void)
{
	if (SGUI_kd >= 0)// 初始化完成了
		return 0;

	SGUI_kd = open(BUTTONDEV_EAC5000C, O_RDWR);
	if (SGUI_kd < 0)
	{
		printf(BUTTONDEV" open failed!\n");
		return -1;
	}
	return 0;
}

int C_SguiDriverEac5000C::ScanKeyboard(void)
{
	int key = _ScanKeyboard();
	switch(key)
	{
	case 0:key = GVK_UP;break;
	case 1:key = GVK_DN;break;
	case 2:key = GVK_LEFT;break;
	case 3:key = GVK_RIGHT;break;
	default:key = -1;break;
	}
	return key;
}

int C_SguiDriverEac5000C::_ScanKeyboard(void)
{
	int event;
	fd_set rset;
	struct timeval tv;

	if (SGUI_kd <= 0)
		return -1;

	FD_ZERO(&rset);
	FD_SET(SGUI_kd, &rset);

	tv.tv_sec = 0;
	tv.tv_usec = 100000;

	if (select(SGUI_kd + 1, &rset, NULL, NULL, &tv) > 0) 
	{
		read(SGUI_kd, &event, 1);
		return (event&0xFF);
	}

	return -1;
}

#endif

/** @brief 设置SGUI 驱动 */
int SGUI_SetDriver(int SguiDriverID)
{
	int old = pSguiDriverBase->DriverID();
	switch (SguiDriverID)
	{
	case SGUI_DRIVER_ID_DEFAULT:
		pSguiDriverBase = &g_SguiDriverBaseDefault;
		break;
	case SGUI_DRIVER_ID_EAC_5000D:
		pSguiDriverBase = &g_SguiDriverEac5000D;
		break;
	case SGUI_DRIVER_ID_EAC_5000C:
		pSguiDriverBase = &g_SguiDriverEac5000C;
		break;
	case SGUI_DRIVER_ID_CQ_6100:
		pSguiDriverBase = &g_SguiDriverCq6100;
		break;
	default:
		pSguiDriverBase = &g_SguiDriverBaseDefault;
		break;
	};
	return old;
}

/** @brief Lcd参数 */
unsigned short SGUI_LcdWidth(void)
{
	return pSguiDriverBase->LcdWidth();
}
unsigned short SGUI_LcdHeight(void)
{
	return pSguiDriverBase->LcdHeight();
}
unsigned short SGUI_LcdBpp(void)
{
	return pSguiDriverBase->LcdBpp();
}
unsigned long SGUI_LcdLine(void)
{
	return ((unsigned long)SGUI_LcdWidth() * SGUI_LcdBpp() / 8);
}
unsigned long SGUI_LcdSize(void)
{
	return ((unsigned long)SGUI_LcdLine() * SGUI_LcdHeight());
}

/** @brief LCD初始化*/
int SGUI_LcdInit(void)
{
	return pSguiDriverBase->fbinit();
}

/** @brief 键盘初始化*/
int SGUI_KeyboardInit(void)
{
	return pSguiDriverBase->kbinit();
}

static bool flushScrnEnable = true;
/** @brief 屏幕刷新*/
void SGUI_flushScreen(S_WORD x, S_WORD y, S_WORD w, S_WORD h)
{
	if (flushScrnEnable == true)
		pSguiDriverBase->fbfresh();
}

/** @brief 屏幕刷新使能*/
bool SGUI_FlushScreenEn(bool enable)
{
	bool old = flushScrnEnable;
	flushScrnEnable = enable;
//	if (flushScrnEnable == true)
//		fbfresh();
	return old;
}

/** @brief 关闭屏幕*/
void SGUI_CloseScreen(void)
{
	pSguiDriverBase->CloseScreen();
}

/** @brief 打开屏幕*/
void SGUI_OpenScreen(void)
{
	pSguiDriverBase->OpenScreen();
}

/** @brief 背光灯开关*/
void SGUI_backlight(int on)
{
	if (on != 0)
		on = 1;
	pSguiDriverBase->backlight(on);
}

/** @brief 设置对比度
 *  @param[in] contrast 0-99
 */
void SGUI_contrast(int contrast)
{
	if (contrast < 0 || contrast > 99)
		contrast = 99;
	pSguiDriverBase->SetContrast(contrast);
}

/** @brief 设置背光灯亮度
 *  @param[in] brightness 0-99
 */
void SGUI_brightness(int brightness)
{
	if (brightness < 0 || brightness > 99)
		brightness = 99;
	pSguiDriverBase->SetBrightness(brightness);
}

/** @brief 设置灰度
 *  @param[in] grayness 0-99
 */
void SGUI_grayness(int grayness)
{
	if (grayness < 0 || grayness > 99)
		grayness = 99;
	pSguiDriverBase->SetGrayness(grayness);
}

/** @brief 设置屏幕方向
 *  @param[in] direction 0-3
 */
void SGUI_direction(int direction)
{
	if (direction < 0 || direction > 3)
		direction = 0;
	pSguiDriverBase->SetDirection(direction);
}

/** @brief 资源路径初始化*/
char *SGUI_ResourcePathInit(const char *pResourcePath)
{
	if (pResourcePath != NULL)
		sprintf(g_SGUI_ResourcePath, "%s", pResourcePath);
	return g_SGUI_ResourcePath;
}

/** @brief 查询消息*/
S_GUIMSG SGUI_PollMsg(void)
{
	S_GUIMSG aMsg;
	int key = pSguiDriverBase->ScanKeyboard();
	if (key != -1)
	{
		if (key > GVK_KEYBEGIN && key < GVK_KEYEND)
			aMsg.Msg = GM_KEYUP;
		else if (key > GVK_SYSKEYBEGIN && key < GVK_SYSKEYEND)
			aMsg.Msg = GM_SYSKEY;
		aMsg.wParam = (S_WORD)key;
	}
	return aMsg;
}

/** @brief 睡眠函数*/
int SGUI_SleepMS(int ms)
{
	SleepMS(ms);
	return 0;
}

#endif

