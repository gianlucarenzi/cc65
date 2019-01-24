#ifndef __Fields_Menu_H
#define __Fields_Menu_H

/* Submenu struct: hols one submenu. */
struct menuitemops
{
	unsigned char	xlen,		/* Width of menu, must be at least 2
					 * more than widest item. */
			ylen;		/* Height of menu, must be exactly 2
					 * more than # items. */
	char* name;			/* Menu name */
	char* menuitems[];		/* Name of menu items, ptr. to array.
					 * pulldown() returns 0-delimeted
					 * # item in array, or 0xFF if
					 * canceled. */
};

/* Main menu struct. */
struct mainmenuops
{
#if defined (__C64__) || defined (__C128__) || defined (__PLUS4__) || defined (__C16__)
	unsigned char color, hicolor;	/* Menu normal and hi-light colors,
					 * for color computers only */
#endif
	unsigned char xlen;		/* Width of menu item, must be at
					 * least 1 more than widest item.
					 */
	unsigned char numitems;		/* # items in main menu. */
	struct menuitemops* m[];	/* Submenu array. */
};
/* Display pull-down menu with options in menuitems[] at (xloc, yloc) in
 * specified len. and get # selection from user.
 * xlen and ylen are assumed to be correct.
 * This routine doesn't clear the screen.
 */
unsigned char __fastcall__ pulldown (char* m[],
	unsigned char xloc,
	unsigned char yloc,
   	unsigned char xlen,
	unsigned char ylen);

/* Display main menu specified by m at the top of the screen.  Returns
 * submenu # << 8 + menu # for a selection or 0xFFFF if cancel.  Doesn't
 * redraw the screen.
 */
unsigned __fastcall__ mainmenu (struct mainmenuops* m);

#endif

