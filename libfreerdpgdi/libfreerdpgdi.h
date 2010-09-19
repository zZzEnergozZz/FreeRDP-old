/* -*- c-basic-offset: 8 -*-
   FreeRDP: A Remote Desktop Protocol client.
   RDP GDI Adaption Layer

   Copyright (C) Marc-Andre Moreau <marcandre.moreau@gmail.com> 2010

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <freerdp/freerdp.h>

#ifndef __LIBFREERDPGDI_H
#define __LIBFREERDPGDI_H

/* For more information, see [MS-RDPEGDI].pdf */

/* Binary Raster Operations (ROP2) */
#define R2_BLACK		0x01  /* 0    */
#define R2_NOTMERGEPEN		0x02  /* DPon */
#define R2_MASKNOTPEN		0x03  /* DPna */
#define R2_NOTCOPYPEN		0x04  /* PN   */
#define R2_MASKPENNOT		0x05  /* PDna */
#define R2_NOT			0x06  /* Dn   */
#define R2_XORPEN		0x07  /* DPx  */
#define R2_NOTMASKPEN		0x08  /* DPan */
#define R2_MASKPEN		0x09  /* DPa  */
#define R2_NOTXORPEN		0x0A  /* DPxn */
#define R2_NOP			0x0B  /* D    */
#define R2_MERGENOTPEN		0x0C  /* DPno */
#define R2_COPYPEN		0x0D  /* P    */
#define R2_MERGEPENNOT		0x0E  /* PDno */
#define R2_MERGEPEN		0x0F  /* DPo  */
#define R2_WHITE		0x10  /* 1    */

/* Ternary Raster Operations (ROP3) */
#define SRCCOPY			0x00CC0020 /* D = S       */
#define SRCPAINT		0x00EE0086 /* D = S | D   */
#define SRCAND			0x008800C6 /* D = S & D   */
#define SRCINVERT		0x00660046 /* D = S ^ D   */
#define SRCERASE		0x00440328 /* D = S & !D  */
#define NOTSRCCOPY		0x00330008 /* D = !S      */
#define NOTSRCERASE		0x001100A6 /* D = !S & !D */
#define MERGECOPY		0x00C000CA /* D = S & P   */
#define MERGEPAINT		0x00BB0226 /* D = !S | D  */
#define PATCOPY			0x00F00021 /* D = P       */
#define PATPAINT		0x00FB0A09 /* D = DPSnoo  */
#define PATINVERT		0x005A0049 /* D = P ^ D   */
#define DSTINVERT		0x00550009 /* D = !D      */
#define BLACKNESS		0x00000042 /* D = BLACK   */
#define WHITENESS		0x00FF0062 /* D = WHITE   */

/* Brush Styles */
#define BS_SOLID		0x00
#define BS_NULL			0x01
#define BS_HATCHED		0x02
#define BS_PATTERN		0x03

/* Hatch Patterns */
#define HS_HORIZONTAL		0x00
#define HS_VERTICAL		0x01
#define HS_FDIAGONAL		0x02
#define HS_BDIAGONAL		0x03
#define HS_CROSS		0x04
#define HS_DIAGCROSS		0x05

/* Pen Styles */
#define PS_SOLID		0x00
#define PS_DASH			0x01
#define PS_NULL			0x05

/* GDI Object Types */
#define GDIOBJ_BITMAP		0x00
#define GDIOBJ_PEN		0x01
#define GDIOBJ_PALETTE		0x02
#define GDIOBJ_BRUSH		0x03
#define GDIOBJ_RECT		0x04

/* Background Modes */
#define OPAQUE			0x00000001
#define TRANSPARENT		0x00000002

struct _PIXEL
{
	int red;
	int green;
	int blue;
	int alpha;
};
typedef struct _PIXEL PIXEL;

struct _GDIOBJ
{
	unsigned char objectType;
};
typedef struct _GDIOBJ GDIOBJ;
typedef GDIOBJ* HGDIOBJ;

/* RGB encoded as 0x00BBGGRR */
typedef unsigned int COLORREF;
typedef COLORREF* LPCOLORREF;

struct _RECT
{
	unsigned int left;
	unsigned int top;
	unsigned int right;
	unsigned int bottom;
};
typedef struct _RECT RECT;
typedef RECT* HRECT;

struct _RGN
{
	unsigned int left;
	unsigned int top;
	unsigned int right;
	unsigned int bottom;
};
typedef struct _RGN RGN;
typedef RGN* HRGN;

struct _DC
{
	HGDIOBJ selectedObject;
	unsigned int bytesPerPixel;
	unsigned int bitsPerPixel;
	HRGN clippingRegion;
	COLORREF bkColor;
	COLORREF textColor;
	int bkMode;
};
typedef struct _DC DC;
typedef DC* HDC;

struct _BITMAP
{
	unsigned char objectType;
	unsigned int bytesPerPixel;
	unsigned int bitsPerPixel;
	unsigned int width;
	unsigned int height;
	char* data;
};
typedef struct _BITMAP BITMAP;
typedef BITMAP* HBITMAP;

struct _PEN
{
	unsigned char objectType;
	unsigned int style;
	unsigned int width;
};
typedef struct _PEN PEN;
typedef PEN* HPEN;

struct _PALETTEENTRY
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};
typedef struct _PALETTEENTRY PALETTEENTRY;

struct _LOGPALETTE
{
	unsigned int count;
	PALETTEENTRY *entries;
};
typedef struct _LOGPALETTE LOGPALETTE;

struct _PALETTE
{
	unsigned char objectType;
	LOGPALETTE *logicalPalette;
};
typedef struct _PALETTE PALETTE;
typedef PALETTE* HPALETTE;

struct _BRUSH
{
	unsigned char objectType;
	unsigned int style;
	HBITMAP pattern;
	COLORREF color;
};
typedef struct _BRUSH BRUSH;
typedef BRUSH* HBRUSH;

struct _WND
{
	HRECT invalid;
	int dirty;
};
typedef struct _WND WND;
typedef WND* HWND;

unsigned int gdi_rop3_code(unsigned char code);
unsigned int gdi_make_colorref(PIXEL *pixel);
void gdi_colour_convert(PIXEL *pixel, int colour, int bpp, HPALETTE palette);
char* gdi_image_convert(char* srcData, int width, int height, int srcBpp, int dstBpp, HPALETTE palette);

HDC GetDC();
HDC CreateCompatibleDC(HDC hdc);
HBITMAP CreateBitmap(int nWidth, int nHeight, int cBitsPerPixel, char* data);
HBITMAP CreateCompatibleBitmap(HDC hdc, int nWidth, int nHeight);
HPEN CreatePen(int fnPenStyle, int nWidth, int crColor);
HPALETTE CreatePalette(LOGPALETTE *lplgpl);
HBRUSH CreateSolidBrush(COLORREF crColor);
HBRUSH CreatePatternBrush(HBITMAP hbmp);
int SetRect(HRECT rc, int xLeft, int yTop, int xRight, int yBottom);
int CopyRect(HRECT dst, HRECT src);
int FillRect(HDC hdc, HRECT rect, HBRUSH hbr);
HRGN CreateRectRgn(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect);
int SelectClipRgn(HDC hdc, HRGN hrgn);
int InvalidateRect(HWND hWnd, HRECT lpRect);
COLORREF GetPixel(HDC hdc, int nXPos, int nYPos);
COLORREF SetPixel(HDC hdc, int X, int Y, COLORREF crColor);
COLORREF GetBkColor(HDC hdc);
COLORREF SetBkColor(HDC hdc, COLORREF crColor);
COLORREF SetTextColor(HDC hdc, COLORREF crColor);
int SetBkMode(HDC hdc, int iBkMode);
int PatBlt(HDC hdc, int nXLeft, int nYLeft, int nWidth, int nHeight, int rop);
int BitBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, int rop);
HGDIOBJ SelectObject(HDC hdc, HGDIOBJ hgdiobj);
int DeleteObject(HGDIOBJ hgdiobj);
int DeleteDC(HDC hdc);

#endif /* __LIBFREERDPGDI_H */