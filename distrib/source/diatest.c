#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <conio.h>

#ifdef __CBM__
#include <cbm.h>
#elif defined __APPLE2__ || defined __APPLE2ENH__
#include <apple2.h>
#endif
#include "dialog.h"
#include "menus.h"

/* Menus: */
struct menuitemops mnFile=
{
	6,6,
	"File",
	{"New", "Open", "Save", "Quit"}
};
struct menuitemops mnEdit=
{
	7,5,
	"Edit",
	{"Cut", "Copy", "Paste"}
};
struct menuitemops mnTest=
{
	15,4,
	"Tester",
	{"Op1", "Op2"}
};
/* Main menu: */
struct mainmenuops mnMain=
{
#ifdef __COLOR__
#if defined __C64__ || defined __C128__
	2,5,
#else
	0x62,0x25,
#endif
#endif
	9,3,
	{&mnFile, &mnEdit, &mnTest}
};
char *h="Help line";

field f;
char * CPUsel[]={
	"Amiga",
	"Apple",
	"Atari",
	"Commodore"
};
char* ComponentSel[]=
{
	"Graphics",
	"Sound",
	"Keyboard",
	"Mouse",
	"Joystick"
};
field fCPU={
	10,2,16,
	0,2,"Computer:",
	fldtSel,
	"Select a computer..."
};
field fComponent={
	10,3,16,
	0,3,
	"IC:",
	fldtSel,
	"Select component."
	
};
field fRAM={
	10,4,17,
	0, 4, "RAM (k):",
	fldtNum,
	"Type in RAM in k..."
};
field fUser={
	10, 5, 17,
	0, 5, "User:",
	fldtText,
	"Type in your alias..."
};
field fLoggedIn={
	10,6,1,
	12, 6, "Logged in",
	fldtChk,
	"Logged In?"
};
field* flds[5]={&fCPU,&fComponent,&fRAM,&fUser,&fLoggedIn};

fldenv dia={
#if  defined(__C64__) || defined(__C128__)
	0,2,10,5,0,
#elif defined(__PLUS4__) || defined(__C16__)
	0x00,0x48,0x4A,0x45,0x41,
#endif
	"Test Dialog",
	"Just a demo...!",
	5,flds
};

struct _InF {
	unsigned mach;
	unsigned component;
	unsigned RAM;
	char user[17];
	unsigned loggedin;
} InF = {
	0,
	0,
	5,
	"Harry Potter",
	1
};

int main ()
{
	unsigned r;
#ifdef __C128__
	char sx,sy;
	screensize(&sx, &sy);
	if (sx==40) {
		printf ("Please switch to 80-column mode now.\n");
		toggle_videomode();
	}
#endif
#if  defined(__C64__) || defined(__C128__)
	bordercolor(3);
	bgcolor (1);
#elif defined(__PLUS4__) || defined(__C16__)
	bordercolor(0x6C);
	bgcolor (0x71);
#endif
	fCPU.data.sel.sel=&InF.mach;
	fCPU.data.sel.values=CPUsel;

	fComponent.data.sel.sel=&InF.component;
	fComponent.data.sel.values=ComponentSel;
	fComponent.data.sel.num=5;

	fRAM.data.num=&InF.RAM;
	
	fUser.data.text=InF.user;

	fLoggedIn.data.num=&InF.loggedin;

	dialog (&dia);

	clrscr ();
	printf ("Computer:    %s\n"
		"Component:   %s\n"
		"RAM:         %dk\n"
		"User:        %s\n"
		"Logged in:   %s\n",
		//dia.field[0]->data.sel.values[InF.mach],
		//dia.field[1]->data.sel.values[InF.component],
		CPUsel[InF.mach],
		ComponentSel[InF.component],
		
		InF.RAM,
		InF.user,
		InF.loggedin?"true":"false");
	puts ("\n\nPress any key for menu demo.\n");
	cgetc();

	clrscr();
	r=mainmenu(&mnMain);
	revers(0); clrscr();
	printf ("Menu returned: %4.4X.\n\n", r);
	return 0;
}
