#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define SCE_DISPLAY_UPDATETIMING_NEXTVSYNC SCE_DISPLAY_SETBUF_NEXTFRAME
#include <psp2/display.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/kernel/threadmgr.h>

#include "graphics.h"

enum {
	SCREEN_WIDTH = 960,
	SCREEN_HEIGHT = 544,
	LINE_SIZE = 960,
	FRAMEBUFFER_SIZE = 2 * 1024 * 1024,
	FRAMEBUFFER_ALIGNMENT = 256 * 1024
};

typedef union
{
	int rgba;
	struct
	{
		char r;
		char g;
		char b;
		char a;
	} c;
} color_t;

extern u8 msx[];
void* g_vram_base;
static int gX = 0;
static int gY = 0;

static Color g_fg_color;
static Color g_bg_color;

static Color* getVramDisplayBuffer()
{
	Color* vram = (Color*) g_vram_base;
	return vram;
}

void *psvDebugScreenGetVram() {
	return g_vram_base;
}

int psvDebugScreenGetX() {
	return gX;
}

int psvDebugScreenGetY() {
	return gY;
}

int g_log_mutex;

void psvDebugScreenInit() {
	g_log_mutex = sceKernelCreateMutex("log_mutex", 0, 0, NULL);

	SceKernelAllocMemBlockOpt opt = { 0 };
	opt.size = sizeof(opt);
	opt.attr = 0x00000004;
	opt.alignment = FRAMEBUFFER_ALIGNMENT;
	SceUID displayblock = sceKernelAllocMemBlock("display", SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, FRAMEBUFFER_SIZE, &opt);
	void *base;
	sceKernelGetMemBlockBase(displayblock, &base);
	// LOG("base: 0x%08x", base);

	SceDisplayFrameBuf framebuf = { 0 };
	framebuf.size = sizeof(framebuf);
	framebuf.base = base;
	framebuf.pitch = SCREEN_WIDTH;
	framebuf.pixelformat = SCE_DISPLAY_PIXELFORMAT_A8B8G8R8;
	framebuf.width = SCREEN_WIDTH;
	framebuf.height = SCREEN_HEIGHT;

	g_vram_base = base;

	int ret = sceDisplaySetFrameBuf(&framebuf, SCE_DISPLAY_UPDATETIMING_NEXTVSYNC);

	if (!ret) {
		printf("Failure to set Framebuffer!");
	}

	g_fg_color = 0xFFFFFFFF;
	g_bg_color = 0x00000000;
}

void psvDebugScreenClear(int bg_color)
{
	gX = gY = 0;
	int i;
	color_t *pixel = (color_t *)getVramDisplayBuffer();
	for(i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		pixel->rgba = bg_color;
		pixel++;
	}
}

static void printTextScreen(const char * text)
{
	int c, i, j, l;
	u8 *font;
	Color *vram_ptr;
	Color *vram;

	int fontSize = 16;
	float zoom = (float) fontSize / 8;
	for (c = 0; c < strlen(text); c++) {
		if (gX + fontSize > SCREEN_WIDTH) {
			gY += fontSize;
			gX = 0;
		}
		if (gY + fontSize > SCREEN_HEIGHT) {
			gY = 0;
			psvDebugScreenClear(g_bg_color);
		}
		char ch = text[c];
		if (ch == '\n') {
			gX = 0;
			gY += fontSize;
			continue;
		} else if (ch == '\r') {
			gX = 0;
			continue;
		}

		vram = getVramDisplayBuffer() + gX + gY * LINE_SIZE;

		for (i = l = 0; i < fontSize; i++, l += fontSize) {
			font = &msx[ (int)ch * 8] + (int) (i / zoom);
			vram_ptr  = vram;
			for (j = 0; j < fontSize; j++) {
				if ((*font & (128 >> (int) (j/zoom)))) *vram_ptr = g_fg_color;
				else *vram_ptr = g_bg_color;
				vram_ptr++;
			}
			vram += LINE_SIZE;
		}
		gX += fontSize;
	}
}

void psvDebugScreenPrintf(const char *format, ...) {
	char buf[0x1000];

	sceKernelLockMutex(g_log_mutex, 1, NULL);

	va_list opt;
	va_start(opt, format);
	vsnprintf(buf, sizeof(buf), format, opt);
	printTextScreen(buf);
	va_end(opt);

	sceKernelUnlockMutex(g_log_mutex, 1);
}

Color psvDebugScreenSetFgColor(Color color) {
	Color prev_color = g_fg_color;
	g_fg_color = color;
	return prev_color;
}

Color psvDebugScreenSetBgColor(Color color) {
	Color prev_color = g_bg_color;
	g_bg_color = color;
	return prev_color;
}
