	.SPACE $PRIVATE$
	.SUBSPA $DATA$,QUAD=1,ALIGN=8,ACCESS=31
	.SUBSPA $BSS$,QUAD=1,ALIGN=8,ACCESS=31,ZERO,SORT=82
	.SPACE $TEXT$
	.SUBSPA $LIT$,QUAD=0,ALIGN=8,ACCESS=44
	.SUBSPA $CODE$,QUAD=0,ALIGN=8,ACCESS=44,CODE_ONLY
	.SUBSPA $MILLICODE$,QUAD=0,ALIGN=8,ACCESS=44,SORT=8

	.SPACE $TEXT$
	.SUBSPA $CODE$

	b,n	$$foo
	nop
	nop

	.SPACE $TEXT$
	.SUBSPA $MILLICODE$
$$foo:
	nop
