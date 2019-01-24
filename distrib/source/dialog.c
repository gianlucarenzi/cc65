#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#include "dialog.h"

/* Export colors to field editing routines */
extern char diaHilightCol, diaFieldCol, diaHelpCol;

/* Define same. */
char diaHilightCol, diaFieldCol, diaHelpCol;

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


/* Entry point for fields handler. */
unsigned char __fastcall__ dialog (fldenv* f)
{
	//char x,y;			/* Tmp. coordinates. */
	signed char center;		/* Left x coordinate for centering title. */

	unsigned char i;//int i;
	char c[12];
	field *ff;
	unsigned char r=0;
	unsigned char nf=f->numfields-1;
	/* Get screen size and clear. */
	clrscr();
	
	/* Center title text and print at top of the screen. */
	center=(__Columns-strlen(f->title))>>1; if (center<0) center=0;
#ifdef __COLOR__

	/* Set colors for field editor. */
	diaFieldCol=f->color; diaHilightCol=f->actcolor; diaHelpCol=f->helpcol;
#endif
#ifdef __COLOR__
	textcolor(f->titlecol); 
#endif
	revers(1); cputsxy (center,0,f->title); revers(0);

	/* Print help. */
#ifdef __COLOR__
	textcolor(diaHelpCol); 
#endif
	// The following line was changed by Grreg King. */
	//gotoxy (0,__Rows-1); cputs(f->globalhelp);
	cputsxy (0,__Rows-1,f->globalhelp);
	
	/* Print field data. */
	for (i=0; i<=nf; ++i)
	{
		ff=f->field[i];
		/* Print field label. */
#ifdef __COLOR__
		textcolor (f->lblcolor);
#endif
		cputsxy (ff->lx, ff->ly, ff->label);
		
		/* Print field data. */
#ifdef __COLOR__
		textcolor (diaFieldCol);
#endif
		gotoxy (ff->x, ff->y);
		switch (ff->type)
		{
		case fldtText:
			cputs (ff->data.text);
			break;
		case fldtNum:
			itoa(*ff->data.num,c,10);
			cputs (c); break;
		case fldtSel:
			cputs (ff->data.sel.values[*ff->data.sel.sel]); break;
		case fldtChk:
			cputc (*ff->data.num?'X':' '); break;
		}
	}
	/* Prepare to start dialog data loop. */
	i=0; /* Set current field to # 0. */
	while (!r)
	{
		/* Edit current field and get action. */
		switch (InField(f->field[i]))
		{
		case 0: /* If cursor up, */
			/* move up one field (wrap-around). */
			//if(i<=0) i=nf;
			//else --i;
			if (--i==0xFF) i=nf;
			break;
		case 1: /* If down, */
			/* move down one field. */
			if (++i>nf) i=0;
			//else ++i;
			break;
		case 2: /* If cancel, exit cancelled. */
			r=2; break;
		case 3: /* If continue, exit continue. */
			r=1; break;
		}
	}
	return r-1;
}


