Ama|char*|savepvs|const char* s
Ama|SV*|newSVpvs|const char* s
Ama|SV*|newSVpvs_share|const char* s
Am|bool|isALNUM|char ch
Am|bool|isALPHA|char ch
Am|bool|isDIGIT|char ch
Am|bool|isLOWER|char ch
Am|bool|isSPACE|char ch
Am|bool|isUPPER|char ch
Am|bool|strEQ|char* s1|char* s2
Am|bool|strGE|char* s1|char* s2
Am|bool|strGT|char* s1|char* s2
Am|bool|strLE|char* s1|char* s2
Am|bool|strLT|char* s1|char* s2
Am|bool|strNE|char* s1|char* s2
Am|bool|strnEQ|char* s1|char* s2|STRLEN len
Am|bool|strnNE|char* s1|char* s2|STRLEN len
Am|bool|SvIOK_notUV|SV* sv
Am|bool|SvIOK_UV|SV* sv
Am|bool|SvIsCOW_shared_hash|SV* sv
Am|bool|SvIsCOW|SV* sv
Am|bool|SvRXOK|SV* sv
Am|bool|SvTAINTED|SV* sv
Am|bool|SvTRUE|SV* sv
Am|bool|SvUOK|SV* sv
Am|bool|SvVOK|SV* sv
Am|char*|HePV|HE* he|STRLEN len
Am|char*|HvNAME|HV* stash
Am|char*|SvEND|SV* sv
Am|char*|SvGAMAGIC|SV* sv
Am|char *|SvGROW|SV* sv|STRLEN len
Am|char*|SvPVbyte_force|SV* sv|STRLEN len
Am|char*|SvPVbyte_nolen|SV* sv
Am|char*|SvPVbyte|SV* sv|STRLEN len
Am|char*|SvPVbytex_force|SV* sv|STRLEN len
Am|char*|SvPVbytex|SV* sv|STRLEN len
Am|char*|SvPV_force_nomg|SV* sv|STRLEN len
Am|char*|SvPV_force|SV* sv|STRLEN len
Am|char*|SvPV_nolen|SV* sv
Am|char*|SvPV_nomg|SV* sv|STRLEN len
Am|char*|SvPV|SV* sv|STRLEN len
Am|char*|SvPVutf8_force|SV* sv|STRLEN len
Am|char*|SvPVutf8_nolen|SV* sv
Am|char*|SvPVutf8|SV* sv|STRLEN len
Am|char*|SvPVutf8x_force|SV* sv|STRLEN len
Am|char*|SvPVutf8x|SV* sv|STRLEN len
Am|char*|SvPVX|SV* sv
Am|char*|SvPVx|SV* sv|STRLEN len
Am|char|toLOWER|char ch
Am|char|toUPPER|char ch
Am|HV*|CvSTASH|CV* cv
Am|HV*|gv_stashpvs|const char* name|I32 create
Am|HV*|SvSTASH|SV* sv
Am|int|AvFILL|AV* av
Am|IV|SvIV_nomg|SV* sv
Am|IV|SvIV|SV* sv
Am|IV|SvIVx|SV* sv
Am|IV|SvIVX|SV* sv
Amn|char*|CLASS
Amn|char*|POPp
Amn|char*|POPpbytex
Amn|char*|POPpx
Amn|HV*|PL_modglobal
Amn|I32|ax
Amn|I32|items
Amn|I32|ix
Amn|IV|POPi
Amn|long|POPl
Amn|NV|POPn
Amn|STRLEN|PL_na
Amn|SV|PL_sv_no
Amn|SV|PL_sv_undef
Amn|SV|PL_sv_yes
Amn|SV*|POPs
Amn|U32|GIMME
Amn|U32|GIMME_V
Am|NV|SvNV|SV* sv
Am|NV|SvNVx|SV* sv
Am|NV|SvNVX|SV* sv
Amn|(whatever)|RETVAL
Amn|(whatever)|THIS
Am|REGEXP *|SvRX|SV *sv
Ams||dAX
Ams||dAXMARK
Ams||dITEMS
Ams||dMARK
Ams||dMULTICALL
Ams||dORIGMARK
Ams||dSP
Ams||dUNDERBAR
Ams||dXCPT
Ams||dXSARGS
Ams||dXSI32
Ams||ENTER
Ams||FREETMPS
Ams||LEAVE
Ams||MULTICALL
Ams||POP_MULTICALL
Ams||PUSH_MULTICALL
Ams||PUTBACK
Ams||SAVETMPS
Ams||SPAGAIN
Am|STRLEN|HeKLEN|HE* he
Am|STRLEN|SvCUR|SV* sv
Am|STRLEN|SvLEN|SV* sv
Am|SV*|GvSV|GV* gv
Am|SV*|HeSVKEY_force|HE* he
Am|SV*|HeSVKEY|HE* he
Am|SV*|HeSVKEY_set|HE* he|SV* sv
Am|SV*|HeVAL|HE* he
Am|SV**|hv_fetchs|HV* tb|const char* key|I32 lval
Am|SV**|hv_stores|HV* tb|const char* key|NULLOK SV* val
Am|SV*|newRV_inc|SV* sv
Am|SV*|ST|int ix
Am|SV*|SvREFCNT_inc_NN|SV* sv
Am|SV*|SvREFCNT_inc_simple_NN|SV* sv
Am|SV*|SvREFCNT_inc_simple|SV* sv
Am|SV*|SvREFCNT_inc|SV* sv
Am|SV*|SvRV|SV* sv
Am|svtype|SvTYPE|SV* sv
Ams||XCPT_RETHROW
Ams||XSRETURN_EMPTY
Ams||XSRETURN_NO
Ams||XSRETURN_UNDEF
Ams||XSRETURN_YES
Ams||XS_VERSION_BOOTCHECK
Am|U32|HeHASH|HE* he
Am|U32|SvIOKp|SV* sv
Am|U32|SvIOK|SV* sv
Am|U32|SvNIOKp|SV* sv
Am|U32|SvNIOK|SV* sv
Am|U32|SvNOKp|SV* sv
Am|U32|SvNOK|SV* sv
Am|U32|SvOK|SV* sv
Am|U32|SvOOK|SV* sv
Am|U32|SvPOKp|SV* sv
Am|U32|SvPOK|SV* sv
Am|U32|SvREFCNT|SV* sv
Am|U32|SvROK|SV* sv
Am|U32|SvUTF8|SV* sv
AmU||G_ARRAY
AmU||G_DISCARD
AmU||G_EVAL
AmU||G_NOARGS
AmU||G_SCALAR
AmU||G_VOID
AmU||HEf_SVKEY
AmU||MARK
AmU||newXSproto|char* name|XSUBADDR_t f|char* filename|const char *proto
AmU||Nullav
AmU||Nullch
AmU||Nullcv
AmU||Nullhv
AmU||Nullsv
AmU||ORIGMARK
AmU||SP
AmU||SVt_IV
AmU||SVt_NV
AmU||SVt_PV
AmU||SVt_PVAV
AmU||SVt_PVCV
AmU||SVt_PVHV
AmU||SVt_PVMG
AmU||svtype
AmU||UNDERBAR
Am|UV|SvUV_nomg|SV* sv
Am|UV|SvUV|SV* sv
Am|UV|SvUVx|SV* sv
Am|UV|SvUVX|SV* sv
AmU||XCPT_CATCH
AmU||XCPT_TRY_END
AmU||XCPT_TRY_START
AmU||XS
AmU||XS_VERSION
Am|void *|CopyD|void* src|void* dest|int nitems|type
Am|void|Copy|void* src|void* dest|int nitems|type
Am|void|EXTEND|SP|int nitems
Am|void*|HeKEY|HE* he
Am|void *|MoveD|void* src|void* dest|int nitems|type
Am|void|Move|void* src|void* dest|int nitems|type
Am|void|mPUSHi|IV iv
Am|void|mPUSHn|NV nv
Am|void|mPUSHp|char* str|STRLEN len
Am|void|mPUSHu|UV uv
Am|void|mXPUSHi|IV iv
Am|void|mXPUSHn|NV nv
Am|void|mXPUSHp|char* str|STRLEN len
Am|void|mXPUSHu|UV uv
Am|void|Newxc|void* ptr|int nitems|type|cast
Am|void|Newx|void* ptr|int nitems|type
Am|void|Newxz|void* ptr|int nitems|type
Am|void|PoisonFree|void* dest|int nitems|type
Am|void|PoisonNew|void* dest|int nitems|type
Am|void|Poison|void* dest|int nitems|type
Am|void|PoisonWith|void* dest|int nitems|type|U8 byte
Am|void|PUSHi|IV iv
Am|void|PUSHMARK|SP
Am|void|PUSHmortal
Am|void|PUSHn|NV nv
Am|void|PUSHp|char* str|STRLEN len
Am|void|PUSHs|SV* sv
Am|void|PUSHu|UV uv
Am|void|Renewc|void* ptr|int nitems|type|cast
Am|void|Renew|void* ptr|int nitems|type
Am|void|Safefree|void* ptr
Am|void|StructCopy|type src|type dest|type
Am|void|sv_catpvn_nomg|SV* sv|const char* ptr|STRLEN len
Am|void|sv_catpvs|SV* sv|const char* s
Am|void|sv_catsv_nomg|SV* dsv|SV* ssv
Am|void|SvCUR_set|SV* sv|STRLEN len
Am|void|SvGETMAGIC|SV* sv
Am|void|SvIOK_off|SV* sv
Am|void|SvIOK_only|SV* sv
Am|void|SvIOK_only_UV|SV* sv
Am|void|SvIOK_on|SV* sv
Am|void|SvIV_set|SV* sv|IV val
Am|void|SvLEN_set|SV* sv|STRLEN len
Am|void|SvLOCK|SV* sv
Am|void|SvMAGIC_set|SV* sv|MAGIC* val
Am|void|SvNIOK_off|SV* sv
Am|void|SvNOK_off|SV* sv
Am|void|SvNOK_only|SV* sv
Am|void|SvNOK_on|SV* sv
Am|void|SvNV_set|SV* sv|NV val
Am|void|SvPOK_off|SV* sv
Am|void|SvPOK_only|SV* sv
Am|void|SvPOK_only_UTF8|SV* sv
Am|void|SvPOK_on|SV* sv
Am|void|SvPV_set|SV* sv|char* val
Am|void|SvREFCNT_dec|SV* sv
Am|void|SvREFCNT_inc_simple_void_NN|SV* sv
Am|void|SvREFCNT_inc_simple_void|SV* sv
Am|void|SvREFCNT_inc_void_NN|SV* sv
Am|void|SvREFCNT_inc_void|SV* sv
Am|void|SvROK_off|SV* sv
Am|void|SvROK_on|SV* sv
Am|void|SvRV_set|SV* sv|SV* val
Am|void|SvSetMagicSV_nosteal|SV* dsv|SV* ssv
Am|void|SvSETMAGIC|SV* sv
Am|void|SvSetMagicSV|SV* dsb|SV* ssv
Am|void|sv_setpvs|SV* sv|const char* s
Am|void|sv_setsv_nomg|SV* dsv|SV* ssv
Am|void|SvSetSV_nosteal|SV* dsv|SV* ssv
Am|void|SvSetSV|SV* dsb|SV* ssv
Am|void|SvSHARE|SV* sv
Am|void|SvSTASH_set|SV* sv|HV* val
Am|void|SvTAINTED_off|SV* sv
Am|void|SvTAINTED_on|SV* sv
Am|void|SvTAINT|SV* sv
Am|void|SvUNLOCK|SV* sv
Am|void|SvUPGRADE|SV* sv|svtype type
Am|void|SvUTF8_off|SV *sv
Am|void|SvUTF8_on|SV *sv
Am|void|SvUV_set|SV* sv|UV val
Am|void|XPUSHi|IV iv
Am|void|XPUSHmortal
Am|void|XPUSHn|NV nv
Am|void|XPUSHp|char* str|STRLEN len
Am|void|XPUSHs|SV* sv
Am|void|XPUSHu|UV uv
Am|void|XSRETURN|int nitems
Am|void|XSRETURN_IV|IV iv
Am|void|XSRETURN_NV|NV nv
Am|void|XSRETURN_PV|char* str
Am|void|XSRETURN_UV|IV uv
Am|void|XST_mIV|int pos|IV iv
Am|void|XST_mNO|int pos
Am|void|XST_mNV|int pos|NV nv
Am|void|XST_mPV|int pos|char* str
Am|void|XST_mUNDEF|int pos
Am|void|XST_mYES|int pos
Am|void *|ZeroD|void* dest|int nitems|type
Am|void|Zero|void* dest|int nitems|type
m|AV *|CvPADLIST|CV *cv
m|bool|CvWEAKOUTSIDE|CV *cv
m|char *|PAD_COMPNAME_PV|PADOFFSET po
m|HV *|PAD_COMPNAME_OURSTASH|PADOFFSET po
m|HV *|PAD_COMPNAME_TYPE|PADOFFSET po
mn|bool|PL_dowarn
mn|GV *|PL_DBsub
mn|GV*|PL_last_in_gv
mn|SV *|PL_DBsingle
mn|SV *|PL_DBtrace
mn|SV*|PL_ofs_sv
mn|SV*|PL_rs
ms||djSP
m|STRLEN|PAD_COMPNAME_GEN|PADOFFSET po
m|STRLEN|PAD_COMPNAME_GEN_set|PADOFFSET po|int gen
m|SV *|CX_CURPAD_SV|struct context|PADOFFSET po
m|SV *|PAD_BASE_SV	|PADLIST padlist|PADOFFSET po
m|SV *|PAD_SETSV	|PADOFFSET po|SV* sv
m|SV *|PAD_SVl	|PADOFFSET po
m|U32|PAD_COMPNAME_FLAGS|PADOFFSET po
mU||LVRET
m|void|CX_CURPAD_SAVE|struct context
m|void|PAD_CLONE_VARS|PerlInterpreter *proto_perl \
m|void|PAD_DUP|PADLIST dstpad|PADLIST srcpad|CLONE_PARAMS* param
m|void|PAD_RESTORE_LOCAL|PAD *opad
m|void|PAD_SAVE_LOCAL|PAD *opad|PAD *npad
m|void|PAD_SAVE_SETNULLPAD
m|void|PAD_SET_CUR_NOSAVE	|PADLIST padlist|I32 n
m|void|PAD_SET_CUR	|PADLIST padlist|I32 n
m|void|PAD_SV	|PADOFFSET po
m|void|SAVECLEARSV	|SV **svp
m|void|SAVECOMPPAD
m|void|SAVEPADSV	|PADOFFSET po
