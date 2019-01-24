#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <conio.h>

#ifdef __CBM__
#include <cbm.h>
#endif
#ifdef __APPLE2__
#include <apple2.h>
#endif
#ifdef __APPLE2ENH__
#include <apple2enh.h>
#endif
#ifdef __ATARI__
#include <atari.h>
#endif

#include "dialog.h"

//static and __fastcall__ declarations recommended by Greg King.

/* Import colors for field-editing routines. */
extern char diaHilightCol, diaFieldCol, diaHelpCol;
//Define color macros:
//selcolor() = Color(s) for selected text.
//unselcolor() = Color(s) for normal text.

//For computers with colors:
#ifdef __COLOR__
/*#define selcolor()			\
	textcolor (diaHilightCol);	\
	revers(1)
#define unselcolor()			\
	revers(0);			\
	textcolor(diaFieldCol)*/
void selcolor ()
{
	textcolor (diaHilightCol);
	revers(1);
}
void unselcolor()
{
	revers(0);
	textcolor(diaFieldCol);
}
//For computers with no colors:
#else
#define selcolor()			\
	revers(1)
#define unselcolor()			\
	revers(0)

#endif

//-----------------------------------------------------------------

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

static void __fastcall__ delitemhelp (void)
{
	cclearxy (0,__Rows-3,__Columns-1);
}
// Following routine has been discontinued by recommendation of Greg King.
/*void EmptyField (unsigned x, unsigned y, unsigned len)
{
	gotoxy (x,y);
	for (;len>0;--len) cputc (32);
}*/
//Routine to draw the contents of a text field on the screen.
//Replaced by Greg King.
void __fastcall__ DrawField (unsigned char x, unsigned char y,
                                    unsigned char len, const char* s)
{
	//Clear field on screen.
	cclearxy (x,y,len);

	//Print field to screen in selected colors.
	cputsxy (x,y,s);
}

static int __fastcall__ InChkField (field* f)
{
	unsigned char i=*f->data.num;
	/*register*/ /*int*/ unsigned char r=0;
	//char c;
	unsigned char x=f->x, y=f->y;
	//gotoxy (0,__Rows-3); cprintf ("Up/down:move Space:toggle Enter");
	cputsxy (0,__Rows-3, "Up/down:move Space:toggle Enter");
	//revers(1);
	selcolor();
	
	while (r==/*-1*/0)//(1)
	{
		//cputcxy (f->x, f->y, *f->data.num?'X':' ');
		cputcxy (x, y, i?'X':' ');
		//c=cgetc ();
		/* Process. */
		//r=-1; /* Value not -1 if to exit the field. */
		switch (cgetc())//(c)
		{
		/* Exit good if enter key pressed. */
		case CH_ENTER: r=3; break;
		case ' ': i=!i; break;
		case CH_CURS_UP:
			r=1; break;
		case CH_CURS_DOWN:
			r=2; break;
		}
		/* If to exit, exit main loop. */
		//if (r!=-1) break;
	}
	unselcolor();
	cputcxy (x, y,i?'X':' ');
	*f->data.num=i;
	//cclearxy (0,__Rows-3,__Columns);
	delitemhelp ();
	return r-1;
}

//Signed char return type recommended by
static signed char __fastcall__ InSelField (field* f)
{
	//int c;
	//signed char c;
	// r's initialization was moved
				/* Temp. values. */
	signed char r=-1;
	unsigned char sx=*f->data.sel.sel;/* Quick access to # user
					 * selection. */
	char** s=f->data.sel.values;	/* Quick access to selection
					 * values */
	unsigned char x=f->x, y=f->y;
	unsigned char len=f->len-1;
	unsigned char n=f->data.sel.num;
#if __Columns==40
	/* Print field type help for 40-column screen. */
	cputsxy (0,__Rows-3,"Left/right:select Up/down:move Enter");
#else
	/* Print field type help for 80-column screen. */
	cputsxy (0,__Rows-3,"Left/right:select Up/down:move Enter: continue");
#endif
	/* Set to selection color. */
	selcolor();
	/* Main routine loop. */
	while (1)
	{
		/* Write current user selection. */
		DrawField (x, y, len, s[sx]);
		/* Get key. */
		//c=cgetc ();
		/* Process. */
		// switch modified from orig. by Greg King:
		// * if(incntrl(c) removed.
		// * First four conditions replaced.
		switch (cgetc())//(c)
		{
#ifdef CH_EOL
		case CH_EOL:
#else
		case CH_ENTER:
#endif
			r=2; break;
#ifdef CH_STOP
		case CH_STOP:
#endif
		case CH_ESC:
			r=3; break;

		/* If cursor left: */
		case CH_CURS_LEFT:
			/* Decrease selection # if >0 */
			//if (sx>0) --sx;
			/* or wrap-around otherwise */
			//else {
			//	/* by searching for null selection */
			//	//for (;s[x]!=0;++x);
			//	/* and selecting prev. selection. */
			//	//--x;
			//	sx=n-1;
			//}
			if (--sx==0xFF) sx=n-1;
			break;
		/* If right: */
		case CH_CURS_RIGHT:
			/* Increase selection #. */
			if (++sx>=n) sx=0;
			break;
		/* If up/down, move to prev. or next field. */
		case CH_CURS_UP:
			r=0; break;
		case CH_CURS_DOWN:
			r=1; break;
		}
		/* If to exit, exit main loop. */
		if (r!=-1) break;
	}
	/* Draw field in unselected colors. */
	unselcolor();
	DrawField (x, y, len, s[sx]);
	/* Write selection #. */
	*f->data.sel.sel=sx;
	/* Clear field type help. */
	//cclearxy (0,__Rows-3,__Columns);
	delitemhelp ();
	/* Exit with return code. */
	return r;
}

signed char __fastcall__ InField (field* f)
{
	// Temporary field data
	char in[81];
	unsigned char c;	// Tmp. char
	//unsigned i;		// Tmp. var
	signed char r=0;	// Return value.
	unsigned char sx=0;	// Tmp. x pos. in field
	unsigned char tmplen=f->len-1;
	unsigned char x=f->x, y=f->y;
	unsigned char t=f->type;


	//Display help.
#ifdef __COLOR__
	textcolor (diaHelpCol);
#endif
	DrawField (0, __Rows-2,__Columns-1, f->help);

	/* Handle selection fields specially. */
	if (t==fldtSel) return InSelField (f);
	
	/* Same with check fields. */
	if (t==fldtChk) return InChkField (f);

	//memset (in,0,81);
	//Copy field to temp. field.
	switch (t)
	{
	case fldtText: strcpy (in,f->data.text);
		//gotoxy (0,__Rows-3); cprintf ("Type text. Up/down:move, Enter:continue");
		cputsxy (0,__Rows-3, "Type text. Up/down:move, Enter:continue");
		break;
	case fldtNum: itoa (*f->data.num, in,10);
		//gotoxy (0,__Rows-3); cprintf ("Type #. Up/down:move, Enter:continue");
		cputsxy (0,__Rows-3, "Type #. Up/down:move, Enter:continue");
		break;
	}
	// Write field to screen.
	selcolor();
	DrawField (x, y, tmplen, in);

	//Main loop for keyboard input.
	while (1)
	{
		//Get key with cursor.
		DrawField (x, y, tmplen/*f->len*/, in);
#if defined __APPLE2__ || defined __APPLE2ENH__
		revers(0);
		cputcxy (sx<tmplen?x+sx:x+tmplen-1,y,'*');
		c=cgetc();
		revers(1);
#else
		gotox (x+sx);
		cursor(1);
		c=cgetc();
		cursor(0);
#endif
		//If the key is a displayable character
		//(# if field is a number or text if text)
		if (((isdigit(c)||(c=='-'&&sx==0))&&t==fldtNum)
			|| (isprint(c)&&t==fldtText)) {
			//ignore if end of field.
			if (strlen(in)>=tmplen/*f->len*/) continue;
			memmove (&in[sx+1], &in[sx],tmplen-sx);
			if (in[sx]==0) in[sx+1]=0;
			in[sx++]=c; //if (sx>=strlen(in)) 

			//Print char and advance x.
		//If control character, do:
		} else /*if (iscntrl(c))*/ {
			r=-1; /* Set return value to -1 for no exit. */
			switch (c)
			{
			/* Exit loop if enter. */
			case CH_ENTER: r=2; break;
#ifdef CH_INS	/* Recommended by Greg King. */
			/* If insert key: */
			case CH_INS:
				/* If field not full, */
				if (strlen(in)<tmplen/*f->len*/) {
					/* open space for new character, */
					/* write space to current pos. */
					in[sx]=32;
				} break;
#endif
			/* If delete */
			case CH_DEL:
				/* and cursor not at left of field, */
				if (sx>0) {
					//in[strlen(in)]=0;
					--sx;
					/* move the rest of the field left to cover current pos., */
					memmove (&in[sx], &in[sx+1],tmplen-sx);//strlen(&in[sx]));
					/* and move the cursor left. */
				} break;
			/* If cursor left */
			case CH_CURS_LEFT:
				/* and not at left of the screen, move cursor left. */
				if (sx>0) --sx; break;
			/* If cursor right */
			case CH_CURS_RIGHT:
				/* and not at end of field, move cursor right. */
				if (in[sx]) ++sx; break;
			/* If up/down, mark return value for move up/down. */
			case CH_CURS_UP:
				r=0; break;
			case CH_CURS_DOWN:
				r=1; break;
			}
			/* Exit loop if return code set for exit. */
			if (r!=-1) break;
		}
	}
	/* Draw field unhilighted. */
	unselcolor();
	DrawField (x, y, tmplen/*f->len*/, in);

	/* Save field information to the field descriptor. */
	switch (t)
	{
	/* If text, */
	case fldtText:
		/* Copy field to memory. */
		strcpy(f->data.text, in);
		break;
	/* If #: */
	case fldtNum:
		/* convert to int and save. */
		*f->data.num=atoi (in);
		break;
	}
	/* Clear field type help. */
	delitemhelp ();
	return r;
}
