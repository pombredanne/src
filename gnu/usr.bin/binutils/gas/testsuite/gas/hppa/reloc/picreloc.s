
	.SPACE	$TEXT$,SORT=8
	.SUBSPA	$CODE$,QUAD=0,ALIGN=4,ACCESS=0x2c,CODE_ONLY,SORT=24
	.SPACE	$PRIVATE$,SORT=16
	.SUBSPA	$DATA$,QUAD=1,ALIGN=8,ACCESS=0x1f,SORT=16
bogo
	.ALIGN	8
	.WORD  bogo+4	; = 0x4
	.STRING	"\x00\x00\x00{\x00\x00\x01\xC8\x00\x00\x03\x15"
	.SPACE	$PRIVATE$
	.SUBSPA	$DATA$
	.EXPORT	bogo
	.END
