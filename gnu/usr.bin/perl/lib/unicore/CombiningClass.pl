# !!!!!!!   DO NOT EDIT THIS FILE   !!!!!!!
# This file is machine-generated by lib/unicore/mktables from the Unicode
# database, Version 5.2.0.  Any changes made here will be lost!

# !!!!!!!   INTERNAL PERL USE ONLY   !!!!!!!
# This file is for internal use by the Perl program only.  The format and even
# the name or existence of this file are subject to change without notice.
# Don't use it directly.

# This file returns the synonymous mappings:
#         Perl_Canonical_Combining_Class(cp)
#         Perl_ccc(cp)
# 
# where 'cp' is one of the 594 code points in Unicode Version 5.2.0 for which
# the mapping is not to '0'.  Note that these mappings are not accessible
# through the Perl core directly.
# 
# This mapping is for normalize.pm.  It is currently identical to the Unicode
# Canonical_Combining_Class property.
# 
# The format of the non-comment portions of the main body of lines of this
# file is: START\tSTOP\tMAPPING where START is the starting code point of the
# range, in hex; STOP is the ending point, or if omitted, the range has just
# one code point; MAPPING is what each code point between START and STOP maps
# to.  Numbers in comments in [brackets] indicate how many code points are in
# the range (omitted when the range is a single code point or if the mapping
# is to the null string).

# The name this swash is to be known by, with the format of the mappings in
# the main body of the table, and what all code points missing from this file
# map to.
$utf8::SwashInfo{'ToCombiningClass'}{'format'} = 'i'; # integer
$utf8::SwashInfo{'ToCombiningClass'}{'missing'} = '0';

return <<'END';
0300	0314	230	# [21]
0315		232
0316	0319	220	# [4]
031A		232
031B		216
031C	0320	220	# [5]
0321	0322	202	# [2]
0323	0326	220	# [4]
0327	0328	202	# [2]
0329	0333	220	# [11]
0334	0338	1	# [5]
0339	033C	220	# [4]
033D	0344	230	# [8]
0345		240
0346		230
0347	0349	220	# [3]
034A	034C	230	# [3]
034D	034E	220	# [2]
0350	0352	230	# [3]
0353	0356	220	# [4]
0357		230
0358		232
0359	035A	220	# [2]
035B		230
035C		233
035D	035E	234	# [2]
035F		233
0360	0361	234	# [2]
0362		233
0363	036F	230	# [13]
0483	0487	230	# [5]
0591		220
0592	0595	230	# [4]
0596		220
0597	0599	230	# [3]
059A		222
059B		220
059C	05A1	230	# [6]
05A2	05A7	220	# [6]
05A8	05A9	230	# [2]
05AA		220
05AB	05AC	230	# [2]
05AD		222
05AE		228
05AF		230
05B0		10
05B1		11
05B2		12
05B3		13
05B4		14
05B5		15
05B6		16
05B7		17
05B8		18
05B9	05BA	19	# [2]
05BB		20
05BC		21
05BD		22
05BF		23
05C1		24
05C2		25
05C4		230
05C5		220
05C7		18
0610	0617	230	# [8]
0618		30
0619		31
061A		32
064B		27
064C		28
064D		29
064E		30
064F		31
0650		32
0651		33
0652		34
0653	0654	230	# [2]
0655	0656	220	# [2]
0657	065B	230	# [5]
065C		220
065D	065E	230	# [2]
0670		35
06D6	06DC	230	# [7]
06DF	06E2	230	# [4]
06E3		220
06E4		230
06E7	06E8	230	# [2]
06EA		220
06EB	06EC	230	# [2]
06ED		220
0711		36
0730		230
0731		220
0732	0733	230	# [2]
0734		220
0735	0736	230	# [2]
0737	0739	220	# [3]
073A		230
073B	073C	220	# [2]
073D		230
073E		220
073F	0741	230	# [3]
0742		220
0743		230
0744		220
0745		230
0746		220
0747		230
0748		220
0749	074A	230	# [2]
07EB	07F1	230	# [7]
07F2		220
07F3		230
0816	0819	230	# [4]
081B	0823	230	# [9]
0825	0827	230	# [3]
0829	082D	230	# [5]
093C		7
094D		9
0951		230
0952		220
0953	0954	230	# [2]
09BC		7
09CD		9
0A3C		7
0A4D		9
0ABC		7
0ACD		9
0B3C		7
0B4D		9
0BCD		9
0C4D		9
0C55		84
0C56		91
0CBC		7
0CCD		9
0D4D		9
0DCA		9
0E38	0E39	103	# [2]
0E3A		9
0E48	0E4B	107	# [4]
0EB8	0EB9	118	# [2]
0EC8	0ECB	122	# [4]
0F18	0F19	220	# [2]
0F35		220
0F37		220
0F39		216
0F71		129
0F72		130
0F74		132
0F7A	0F7D	130	# [4]
0F80		130
0F82	0F83	230	# [2]
0F84		9
0F86	0F87	230	# [2]
0FC6		220
1037		7
1039	103A	9	# [2]
108D		220
135F		230
1714		9
1734		9
17D2		9
17DD		230
18A9		228
1939		222
193A		230
193B		220
1A17		230
1A18		220
1A60		9
1A75	1A7C	230	# [8]
1A7F		220
1B34		7
1B44		9
1B6B		230
1B6C		220
1B6D	1B73	230	# [7]
1BAA		9
1C37		7
1CD0	1CD2	230	# [3]
1CD4		1
1CD5	1CD9	220	# [5]
1CDA	1CDB	230	# [2]
1CDC	1CDF	220	# [4]
1CE0		230
1CE2	1CE8	1	# [7]
1CED		220
1DC0	1DC1	230	# [2]
1DC2		220
1DC3	1DC9	230	# [7]
1DCA		220
1DCB	1DCC	230	# [2]
1DCD		234
1DCE		214
1DCF		220
1DD0		202
1DD1	1DE6	230	# [22]
1DFD		220
1DFE		230
1DFF		220
20D0	20D1	230	# [2]
20D2	20D3	1	# [2]
20D4	20D7	230	# [4]
20D8	20DA	1	# [3]
20DB	20DC	230	# [2]
20E1		230
20E5	20E6	1	# [2]
20E7		230
20E8		220
20E9		230
20EA	20EB	1	# [2]
20EC	20EF	220	# [4]
20F0		230
2CEF	2CF1	230	# [3]
2DE0	2DFF	230	# [32]
302A		218
302B		228
302C		232
302D		222
302E	302F	224	# [2]
3099	309A	8	# [2]
A66F		230
A67C	A67D	230	# [2]
A6F0	A6F1	230	# [2]
A806		9
A8C4		9
A8E0	A8F1	230	# [18]
A92B	A92D	220	# [3]
A953		9
A9B3		7
A9C0		9
AAB0		230
AAB2	AAB3	230	# [2]
AAB4		220
AAB7	AAB8	230	# [2]
AABE	AABF	230	# [2]
AAC1		230
ABED		9
FB1E		26
FE20	FE26	230	# [7]
101FD		220
10A0D		220
10A0F		230
10A38		230
10A39		1
10A3A		220
10A3F		9
110B9		9
110BA		7
1D165	1D166	216	# [2]
1D167	1D169	1	# [3]
1D16D		226
1D16E	1D172	216	# [5]
1D17B	1D182	220	# [8]
1D185	1D189	230	# [5]
1D18A	1D18B	220	# [2]
1D1AA	1D1AD	230	# [4]
1D242	1D244	230	# [3]
END
