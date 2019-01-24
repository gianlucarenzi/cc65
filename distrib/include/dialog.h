#ifndef __Dialog_H
#define __Dialog_H


/* Selector field data type */
typedef struct _flddata_sel
{
	unsigned* sel;			/* # current selection from user, value at
					 * given address */
	unsigned num;			/* # possible selections */
	char** values;			/* pointers to selector text */
} flddatasel;

/* User-entered data types */
typedef union _flddata
{
	char* text;			/* Text entered by user, buffer must be
					 * large enough to hold text. */
	unsigned* num;			/* Number/check box bool, entered by
					 * user. */
	flddatasel sel;			/* Selector field, as specified
					 * above. */
} flddata;

typedef struct _field
{
	unsigned char x,y, len;		/* location and len. of field. */
	unsigned char lx, ly;		/* location of label */
	char* label;			/* label of field */
	/* Type of field: */
	enum {fldtNum,			/* Integer, */
		fldtText,		/* Text, */
		fldtSel,		/* Selection, */
		fldtChk			/* Check box. */
		} type;		
	char* help;			/* Field help */
	flddata data;			/* Data in field */
} field;

typedef struct _fldenv
{
//Exclude colors from monochrome computers
#if !defined __APPLE2__ && !defined __APPLE2ENH__ && !defined __PET__ && !defined __CBM610__ && !defined __CBM510__
	unsigned char lblcolor;		// text color of label
	unsigned char color;		// text color of field
	unsigned char actcolor;		// text color of field when selected
	unsigned char titlecol;		// text color for title
	unsigned char helpcol;		// text color for help
#define __COLOR__
#else
#define __MONO__
#endif
	char* title;			// title of dialog
	char* globalhelp;		// help for dialog
	unsigned numfields;		// # fields in dialog
	field** field;			// pointers to fields
} fldenv;


void __fastcall__ DrawField (unsigned char x, unsigned char y,
	unsigned char len, const char* s);

// The next two functions' declaration and return type were altered by Greg King. */
//Process text or number field.
//Returns 0 for prev. field, 1 for next,
//2 for advance to next step (i.e. process a dialog or go to
//   next step dialog),
//3 for cancel.
signed char __fastcall__ InField (field* f);


/* Displays a dialog box-like screen, fields specified by f.
 * Returns 0 if good or 1 if cancelled. */
unsigned char __fastcall__ dialog (fldenv* f);


#endif
