#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <conio.h>

#include "menus.h"
#include "dialog.h"

//Set screen columns for different computers:
#if defined __APPLE2__ || defined __APPLE2ENH__ || defined __C64__ || defined __PLUS4__ || defined __C16__ || defined __PET__ || defined __CBM510__
#define __Columns 40
#elif defined __CBM610__ | defined __C128__
#define __Columns 80
#endif
//Now rows:
#if defined __APPLE2__ || defined __APPLE2ENH__
#define __Rows 24
#else
#define __Rows 25
#endif
/* Define colors macro if color computer: */
#if defined __C64__ || defined __C128__ || defined __PLUS4__ || defined __C16__
#define __Color__
static char menucolor, menuhicolor;
#endif

static unsigned char  	top,		/* Top of menu. */
			numops;		/* # options in menu. */
static unsigned char	leftwindow;	/* X coordinate of left text
					 * pos. in menu */
static unsigned char	widwindow;	/* Width inside menu */

/* Display menu item text in item at y lines from top inside
 * menu. */
static void __fastcall__ DispMenuOp (unsigned char y, char* item)
{
//        cclearxy (leftwindow, y, widwindow);
//        cputsxy (leftwindow, y, item);
	DrawField (leftwindow, y, widwindow, item);
}

/* Draws a border around an area of the screen.
 * Starts at (xloc, yloc) and draws a window of size (xlen, ylen).
 */
void __fastcall__ drawborder (unsigned char xloc,
	unsigned char yloc,
   	unsigned char xlen,
	unsigned char ylen)
{
	unsigned char r=xloc+xlen-1,b=yloc+ylen-1;
	/* Draw a border using graphics characters under a CBM computer. */
#ifdef __CBM__
	//gotoxy (xloc+1, yloc);
	/* Draw top window border. */
	//for (l=0; l<xlen-2; ++l)
	//	cputc (0xC0); /* top border char. */
	//chline (xlen-2);
	/* Draw bottom window border. */
	//gotoxy (xloc+1, b);
	//for (l=0; l<xlen-2; ++l)
	//	cputc (0xC0); /* bottom border char. */
	chlinexy (xloc, yloc, xlen);
	chlinexy (xloc, b, xlen);
	/* Draw left and right sides. */
	//for (l=1; l<ylen-1; l++) {
	//	cputcxy (xloc, yloc+l, 0xDD); /* left border char. */
	//	cputcxy (r, yloc+l, 0xDD); /* right border char. */
	//}
	cvlinexy (xloc, yloc,ylen);
	cvlinexy (r, yloc,ylen);
	
	/* Draw corners. */
	cputcxy (xloc, yloc, 0xB0); /* Top-left corner char. */
	cputcxy (r, yloc, 0xAE); /* Top-right corner char. */
	cputcxy (xloc, b, 0xAD); /* Bottom-left corner char. */
	cputcxy (r, b, 0xBD); /* Bottom-right corner char. */
	/* Draw a border of inverted spaces for an Apple computer. */
#elif defined __APPLE2__ || defined __APPLE2ENH__
	unsigned char l;
	revers(1);
	/* Draw top. */
	cclearxy (xloc, yloc, xlen);
	/* Draw bottom. */
	cclearxy (xloc, yloc+ylen-1, xlen);
	/* Draw left and right. */
	for (l=0; l<ylen-1; l++) {
		cputcxy (xloc, yloc+l, ' ');
		cputcxy (xloc+xlen-1, yloc+l, ' ');
	}
	revers (0);	
#endif
}

/* Display pull-down menu with options in menuitems[] at (xloc, yloc) in
 * specified len. and get # selection from user.
 * xlen and ylen are assumed to be correct.
 * This routine doesn't clear the screen.
 */
unsigned char __fastcall__ pulldown (char** menuitems,
	unsigned char xloc,
	unsigned char yloc,
   	unsigned char xlen,
	unsigned char ylen)
{
	/* Tmp. variable. */
	unsigned char i;
	unsigned char 	s=0;//,	/* Current selection. */
			//sold=0;	/* Prev. selection. */
	top=yloc+1;		/* Top of menu. */
	numops=ylen-2;//yloc-1;	/* # options in menu. */
	leftwindow=xloc+1;
	widwindow=xlen-2;

#ifdef __Color__
	textcolor (menuhicolor);
#endif
	revers(0);
	/* Draw the border around the menu. */
	drawborder (xloc,yloc,xlen,ylen);
	/* Color of menu background. */
	/* User: change text color as desired. */
	/* Write menu options.  xlen and ylen are assumed to be correct. */
#ifdef __Color__
	textcolor (menucolor);
#endif
	for (i=top; i<yloc+ylen-1;++i)
	{
		/* Clear menu line. */
		/* Write menu option. */
		DispMenuOp (i, menuitems[i-top]);
	}
	s=0;
	while (1)
	{
		/* Set menu disply color and reverse mode for current menu
		 * option. */
		/* User: change text color as needed. */
#ifdef __Color__
		textcolor (menuhicolor);
#endif
		revers(1);
		DispMenuOp (s+top, menuitems[s]);
		i=cgetc();
		/* Menu background color. */
		/* User: make sure the color value in the next line is the
		 * same as above menu background color. */
#ifdef __Color__
		textcolor (menucolor);
#endif
		revers (0);
		/* Write menu option. */
		DispMenuOp (s+top, menuitems[s]);
		switch (i)
		{
	        case CH_CURS_UP:
			//if (s==0) s=numops;
			//--s;
			if (--s==0xFF) s=numops-1;
			//s=(--s)%numops;
			break;
        	case CH_CURS_DOWN:
			//++s;
			if (++s>=numops) s=0;
			//s=(++s)%numops;
			break;
		case CH_ENTER:
			//revers (0);
			//clearwin (xloc, yloc, xlen, ylen);
			return s;
		case CH_ESC:
#ifdef CH_STOP
		case CH_STOP:
#endif
			//revers (0);
			//clearwin (xloc, yloc, xlen, ylen);
			revers(0);
			for (i=yloc; i<yloc+ylen; ++i)
				cclearxy (xloc, i, xlen);
			return -1;
		}
		//sold=s;
	}
}

/*static void dispmainmenu (struct mainmenuops* m)
{
	unsigned char i=0;
	unsigned char x=0, y=0, xlen=m->xlen;
	for (; i<m->numitems; ++i)
	{
		if (x+xlen>=__Columns) {x=0; y++;}
		cputsxy (x,y,m->m[i]->name);
		x+=xlen;
	}
}*/

unsigned __fastcall__ mainmenu (struct mainmenuops* m)
{
	unsigned char x=0, y=0, mperx=__Columns/m->xlen;
	unsigned char i, j, r;
	struct menuitemops* m2;
	//int c;
	unsigned char c;
#ifdef __Color__
	menucolor=m->color;
	menuhicolor=m->hicolor;
#endif
	//i=0;
	while (1)
	{
		m2=m->m[i];
#ifdef __Color__
		textcolor (menucolor);
#endif
		revers(0);
		for (j=0; j<m->numitems; ++j)
			cputsxy ((j%mperx)*m->xlen, j/mperx, m->m[j]->name);
#ifdef __Color__
		textcolor (menuhicolor);
#endif
		revers(1);
		cputsxy ((i%mperx)*m->xlen, i/mperx, m2->name);
		c=cgetc();
		
/*#ifdef __Color__
		textcolor (menucolor);
#endif
		revers(0);*/
		//cputsxy ((i%mperx)*m->xlen, i/mperx, m2->name);
		switch (c)
		{
		case CH_CURS_LEFT:
		case CH_CURS_UP:
			//--i;
			if (--i==0xFF) i=m->numitems-1; break;
			//i=(i+1)%m->numitems; break;
		case CH_CURS_RIGHT:
		case CH_CURS_DOWN:
			//++i;
			if (++i>=m->numitems) i=0; break;
			//i=(i-1)%m->numitems; break;
#ifdef CH_STOP:
		case CH_STOP:
#endif
		case CH_ESC:
			return -1;
		case CH_ENTER:
			x=m->xlen*(i%mperx); y=i/mperx+1;
			if (x+m2->xlen>__Columns) x=__Columns-m2->xlen;
			r=pulldown (m2->menuitems, x,y, m2->xlen,m2->ylen);
			if (r!=0xFF) return (r<<8)+i;
			cputsxy ((i%mperx)*m->xlen, i/mperx, m2->name);
		}
	}
}

