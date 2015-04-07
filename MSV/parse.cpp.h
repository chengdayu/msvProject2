typedef union{
   char     * cString;   /* A character string */
   int       nInteger;   /* An integer value */
   float floatval;	/* A float value added by Jane 2012-06-06 */
   char character;	/* A character added by Jane 2012-06-06*/
   CSyntaxNode *tnode;    /* Node in the syntax tree */
   enum NODETYPE  nodetype;
   enum RETURNTYPE  returntype;
} YYSTYPE;
#define	IF	258
#define	ELSE	259
#define	EMPTY	260
#define	AWAIT	261
#define	PROJECTION	262
#define	PBEGIN	263
#define	PEND	264
#define	POINTERNULL	265
#define	ARRAY	266
#define	STRUCT	267
#define	DOT	268
#define	UNION	269
#define	FOPEN	270
#define	FCLOSE	271
#define	FILEDECLARATION	272
#define	FGETS	273
#define	FPUTS	274
#define	FGETC	275
#define	FPUTC	276
#define	MORE	277
#define	DEFINE	278
#define	MY_TRUE	279
#define	MY_FALSE	280
#define	EXIST	281
#define	FRAME	282
#define	FOR	283
#define	WHILE	284
#define	DO	285
#define	TRUE	286
#define	FALSE	287
#define	REPEAT	288
#define	UNTIL	289
#define	DISPLAY	290
#define	SKIP	291
#define	THEN	292
#define	COMMA	293
#define	COLON	294
#define	INTDECLARATION	295
#define	STRDECLARATION	296
#define	POINTERDECLARATION	297
#define	CHARDECLARATION	298
#define	VOIDDECLARATION	299
#define	FLOATDECLARATION	300
#define	LISTDECLARATION	301
#define	OPEN_PAR	302
#define	CLOSE_PAR	303
#define	TIMES	304
#define	OPEN_BPAR	305
#define	CLOSE_BPAR	306
#define	OPEN_MPAR	307
#define	CLOSE_MPAR	308
#define	FUNCTION	309
#define	OVER	310
#define	CYLINDER	311
#define	OVEREP	312
#define	INTER_OR	313
#define	NON_DETERMINED	314
#define	SEND	315
#define	RECEIVE	316
#define	CHANNEL	317
#define	PUT	318
#define	GET	319
#define	ERROR_TOKEN	320
#define	PROCESS	321
#define	STRFUNCHEAD	322
#define	STRFUNCTAIL	323
#define	STRFUNCCAT	324
#define	STRFUNCCMP	325
#define	STRFUNCCPY	326
#define	STRFUNCLEN	327
#define	SIZEOF	328
#define	SYSTEM	329
#define	SWITCH	330
#define	BREAK	331
#define	CASE	332
#define	DEFAULT	333
#define	CEIL	334
#define	FLOOR	335
#define	ROUND	336
#define	SIN	337
#define	COS	338
#define	TAN	339
#define	ASIN	340
#define	ACOS	341
#define	ATAN	342
#define	SINH	343
#define	COSH	344
#define	TANH	345
#define	EXP	346
#define	LOG	347
#define	LOG10	348
#define	SQRT	349
#define	ATAN2	350
#define	POW	351
#define	ABS	352
#define	FABS	353
#define	LABS	354
#define	FMOD	355
#define	MODF	356
#define	LDEXP	357
#define	FREXP	358
#define	UNSIGNED	359
#define	SIGNED	360
#define	MALLOC	361
#define	FREE	362
#define	CHOP	363
#define	IMPLY	364
#define	PROPIMPLY	365
#define	IFF	366
#define	OR	367
#define	PARALLEL	368
#define	AND	369
#define	ASS_EQU	370
#define	UNITASSIGN	371
#define	CON	372
#define	ADDRESS	373
#define	NE	374
#define	EQU	375
#define	GE	376
#define	LE	377
#define	GT	378
#define	LT	379
#define	LST	380
#define	RST	381
#define	ADD	382
#define	SUB	383
#define	MUL	384
#define	DIV	385
#define	MOD	386
#define	BNE	387
#define	CHOPSTAR	388
#define	NEXT	389
#define	NEGATION	390
#define	LENGTH	391
#define	REQUEST	392
#define	FINAL	393
#define	KEEP	394
#define	ALWAYS	395
#define	HALT	396
#define	W_NEXT	397
#define	PREFIX	398
#define	PREVIOUS	399
#define	SOMETIMES	400
#define	ID	401
#define	STR	402
#define	STRUCT_TYPE	403
#define	INTEGER	404
#define	FLOATLITERAL	405
#define	CHARLITERAL	406


extern YYSTYPE yylval;
