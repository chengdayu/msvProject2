typedef union{
   char     * cString;   /* A character string */
   int       nInteger;   /* An intliteral value */
   float floatval;	/* A float value added by Jane 2012-06-06 */
   char character;	/* A character added by Jane 2012-06-06*/
   CSyntaxNode *tnode;    /* Node in the syntax tree */
   enum NODETYPE  nodetype;
   enum RETURNTYPE  returntype;
} YYSTYPE;
#define	SWITCH	258
#define	CASE	259
#define	BREAK	260
#define	DEFAULT	261
#define	IF	262
#define	ELSE	263
#define	EMPTY	264
#define	AWAIT	265
#define	PROJECTION	266
#define	PBEGIN	267
#define	PEND	268
#define	POINTERNULL	269
#define	ARRAY	270
#define	STRUCT	271
#define	DOT	272
#define	UNION	273
#define	FILEDECLARATION	274
#define	MORE	275
#define	DEFINE	276
#define	MY_TRUE	277
#define	MY_FALSE	278
#define	EXIST	279
#define	FRAME	280
#define	FOR	281
#define	WHILE	282
#define	DO	283
#define	TRUE	284
#define	FALSE	285
#define	REPEAT	286
#define	UNTIL	287
#define	DISPLAY	288
#define	SKIP	289
#define	THEN	290
#define	COMMA	291
#define	COLON	292
#define	INTDECLARATION	293
#define	STRDECLARATION	294
#define	POINTERDECLARATION	295
#define	CHARDECLARATION	296
#define	VOIDDECLARATION	297
#define	FLOATDECLARATION	298
#define	LISTDECLARATION	299
#define	OPEN_PAR	300
#define	CLOSE_PAR	301
#define	TIMES	302
#define	OPEN_BPAR	303
#define	CLOSE_BPAR	304
#define	OPEN_MPAR	305
#define	CLOSE_MPAR	306
#define	FUNCTION	307
#define	OVER	308
#define	CYLINDER	309
#define	OVEREP	310
#define	INTER_OR	311
#define	NON_DETERMINED	312
#define	SEND	313
#define	RECEIVE	314
#define	PROCESS	315
#define	CHANNEL	316
#define	PUT	317
#define	GET	318
#define	ERROR_TOKEN	319
#define	STRFUNCHEAD	320
#define	STRFUNCTAIL	321
#define	STRFUNCCAT	322
#define	STRFUNCCMP	323
#define	STRFUNCCPY	324
#define	STRFUNCLEN	325
#define	EXTERN	326
#define	UNSIGNED	327
#define	SIGNED	328
#define	CHOP	329
#define	OR	330
#define	PARALLEL	331
#define	AND	332
#define	ASS_P	333
#define	ASS_N	334
#define	CON	335
#define	ADDRESS	336
#define	IMPLY	337
#define	IFF	338
#define	PROPIMPLY	339
#define	NE	340
#define	EQ	341
#define	GE	342
#define	LE	343
#define	GT	344
#define	LT	345
#define	LST	346
#define	RST	347
#define	ADD	348
#define	SUB	349
#define	MUL	350
#define	DIV	351
#define	MOD	352
#define	BNE	353
#define	CHOPSTAR	354
#define	NEXT	355
#define	NEGATION	356
#define	LENGTH	357
#define	REQUEST	358
#define	FINAL	359
#define	KEEP	360
#define	ALWAYS	361
#define	HALT	362
#define	W_NEXT	363
#define	PREFIX	364
#define	SOMETIMES	365
#define	ID	366
#define	STRLITERAL	367
#define	STRUCT_TYPE	368
#define	INTLITERAL	369
#define	FLOATLITERAL	370
#define	CHARLITERAL	371


extern YYSTYPE yylval;
