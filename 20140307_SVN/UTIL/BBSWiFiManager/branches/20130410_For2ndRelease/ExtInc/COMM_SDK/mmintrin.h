//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//

/**
*** Copyright (C) Intel Corporation.  All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
**/

/*
 * Definitions and declarations for use with compiler intrinsics.
 */
#ifndef _MMINTRIN_H_INCLUDED
#define _MMINTRIN_H_INCLUDED

#if defined(_M_IX86) || (defined(_M_ARM) && (_MSC_VER >= 1400))

#if defined __cplusplus
extern "C" { /* Begin "C" */
/* Intrinsics use C name-mangling.
 */
#endif /* __cplusplus */

#ifdef __ICL
typedef unsigned long long __m64; 
#elif _MSC_VER >= 1300
typedef union __declspec(intrin_type) __declspec(align(8)) __m64
{
    unsigned __int64    m64_u64;
    float               m64_f32[2];
    __int8              m64_i8[8];
    __int16             m64_i16[4];
    __int32             m64_i32[2];
    __int64             m64_i64;
    unsigned __int8     m64_u8[8];
    unsigned __int16    m64_u16[4];
    unsigned __int32    m64_u32[2];
} __m64;
#endif

/* General support intrinsics */
void  _m_empty(void);
__m64 _m_from_int(int i);
int   _m_to_int(__m64 m);

/* Packed arithmetic intrinsics */
__m64 _m_paddb(__m64 m1, __m64 m2);
__m64 _m_paddw(__m64 m1, __m64 m2);
__m64 _m_paddd(__m64 m1, __m64 m2);
__m64 _m_paddsb(__m64 m1, __m64 m2);
__m64 _m_paddsw(__m64 m1, __m64 m2);
__m64 _m_paddsd(__m64 m1, __m64 m2);
__m64 _m_paddusb(__m64 m1, __m64 m2);
__m64 _m_paddusw(__m64 m1, __m64 m2);
__m64 _m_paddusd(__m64 m1, __m64 m2);
__m64 _m_psubb(__m64 m1, __m64 m2);
__m64 _m_psubw(__m64 m1, __m64 m2);
__m64 _m_psubd(__m64 m1, __m64 m2);
__m64 _m_psubsb(__m64 m1, __m64 m2);
__m64 _m_psubsw(__m64 m1, __m64 m2);
__m64 _m_psubsd(__m64 m1, __m64 m2);
__m64 _m_psubusb(__m64 m1, __m64 m2);
__m64 _m_psubusw(__m64 m1, __m64 m2);
__m64 _m_psubusd(__m64 m1, __m64 m2);
__m64 _m_pmaddwd(__m64 m1, __m64 m2);
__m64 _m_pmadduwd(__m64 m1, __m64 m2);
__m64 _m_pmulhw(__m64 m1, __m64 m2);
__m64 _m_pmulhuw(__m64 m1, __m64 m2);
__m64 _m_pmullw(__m64 m1, __m64 m2);
__m64 _m_pmacsw(__m64 m1, __m64 m2, __m64 m3);
__m64 _m_pmacuw(__m64 m1, __m64 m2, __m64 m3);
__m64 _m_pmacszw(__m64 m1, __m64 m2);
__m64 _m_pmacuzw(__m64 m1, __m64 m2);
__m64 _m_paccb(__m64 m1);
__m64 _m_paccw(__m64 m1);
__m64 _m_paccd(__m64 m1);
__m64 _m_pmia(__m64 m1, int i1, int i0);
__m64 _m_pmiaph(__m64 m1, int i1, int i0);
__m64 _m_pmiabb(__m64 m1, int i1, int i0);
__m64 _m_pmiabt(__m64 m1, int i1, int i0);
__m64 _m_pmiatb(__m64 m1, int i1, int i0);
__m64 _m_pmiatt(__m64 m1, int i1, int i0);
/* Shift intrinsics */
__m64 _m_psllw(__m64 m, __m64 count);
__m64 _m_psllwi(__m64 m, int count);
__m64 _m_pslld(__m64 m, __m64 count);
__m64 _m_pslldi(__m64 m, int count);
__m64 _m_psllq(__m64 m, __m64 count);
__m64 _m_psllqi(__m64 m, int count);
__m64 _m_psraw(__m64 m, __m64 count);
__m64 _m_psrawi(__m64 m, int count);
__m64 _m_psrad(__m64 m, __m64 count);
__m64 _m_psradi(__m64 m, int count);
__m64 _m_psraq(__m64 m, __m64 count);
__m64 _m_psraqi(__m64 m, int count);
__m64 _m_psrlw(__m64 m, __m64 count);
__m64 _m_psrlwi(__m64 m, int count);
__m64 _m_psrld(__m64 m, __m64 count);
__m64 _m_psrldi(__m64 m, int count);
__m64 _m_psrlq(__m64 m, __m64 count);
__m64 _m_psrlqi(__m64 m, int count);
__m64 _m_prorw(__m64 m, __m64 count);
__m64 _m_prorwi(__m64 m, int count);
__m64 _m_prord(__m64 m, __m64 count);
__m64 _m_prordi(__m64 m, int count);
__m64 _m_prorq(__m64 m, __m64 count);
__m64 _m_prorqi(__m64 m, int count);

/* Logical intrinsics */
__m64 _m_pand(__m64 m1, __m64 m2);
__m64 _m_pandn(__m64 m1, __m64 m2);
__m64 _m_por(__m64 m1, __m64 m2);
__m64 _m_pxor(__m64 m1, __m64 m2);

/* Comparison intrinsics */
__m64 _m_pcmpeqb(__m64 m1, __m64 m2);
__m64 _m_pcmpeqw(__m64 m1, __m64 m2);
__m64 _m_pcmpeqd(__m64 m1, __m64 m2);
__m64 _m_pcmpgtb(__m64 m1, __m64 m2);
__m64 _m_pcmpgtub(__m64 m1, __m64 m2);
__m64 _m_pcmpgtw(__m64 m1, __m64 m2);
__m64 _m_pcmpgtuw(__m64 m1, __m64 m2);
__m64 _m_pcmpgtd(__m64 m1, __m64 m2);
__m64 _m_pcmpgtud(__m64 m1, __m64 m2);

/* Pack/Unpack intrinsics */
__m64 _m_packsswb(__m64 m1, __m64 m2);
__m64 _m_packssdw(__m64 m1, __m64 m2);
__m64 _m_packuswb(__m64 m1, __m64 m2);
__m64 _m_punpckhbw(__m64 m1, __m64 m2);
__m64 _m_punpckhwd(__m64 m1, __m64 m2);
__m64 _m_punpckhdq(__m64 m1, __m64 m2);
__m64 _m_punpcklbw(__m64 m1, __m64 m2);
__m64 _m_punpcklwd(__m64 m1, __m64 m2);
__m64 _m_punpckldq(__m64 m1, __m64 m2);
__m64 _m_packssqd(__m64 m1, __m64 m2);
__m64 _m_packusqd(__m64 m1, __m64 m2);
__m64 _m_packusdw(__m64 m1, __m64 m2);
__m64 _m_punpckehsbw(__m64 m1);
__m64 _m_punpckehswd(__m64 m1);
__m64 _m_punpckehsdq(__m64 m1);
__m64 _m_punpckehubw(__m64 m1);
__m64 _m_punpckehuwd(__m64 m1);
__m64 _m_punpckehudq(__m64 m1);
__m64 _m_punpckelsbw(__m64 m1);
__m64 _m_punpckelswd(__m64 m1);
__m64 _m_punpckelsdq(__m64 m1);
__m64 _m_punpckelubw(__m64 m1);
__m64 _m_punpckeluwd(__m64 m1);
__m64 _m_punpckeludq(__m64 m1);

/* Utility intrinsics */
__m64 _mm_setzero_si64();
__m64 _mm_set_pi32(int i1, int i0);
__m64 _mm_set_pi16(short s3, short s2, short s1, short s0);
__m64 _mm_set_pi8(char b7, char b6, char b5, char b4,
                  char b3, char b2, char b1, char b0);
__m64 _mm_set1_pi32(int i);
__m64 _mm_set1_pi16(short s);
__m64 _mm_set1_pi8(char b);
__m64 _mm_setr_pi32(int i1, int i0);
__m64 _mm_setr_pi16(short s3, short s2, short s1, short s0);
__m64 _mm_setr_pi8(char b7, char b6, char b5, char b4,
                   char b3, char b2, char b1, char b0);
void  _mm_setwcx(int i1, int i0);
int   _mm_getwcx(int i);

/* General support intrinsics */
int _m_pextrb(__m64 m1, const int c);
int _m_pextrw(__m64 m1, const int c);
int _m_pextrd(__m64 m1, const int c);
unsigned int _m_pextrub(__m64 m1, const int c);
unsigned int _m_pextruw(__m64 m1, const int c);
unsigned int _m_pextrud(__m64 m1, const int c);
__m64 _m_pinsrb(__m64 m1, int i, const int c);
__m64 _m_pinsrw(__m64 m1, int i, const int c);
__m64 _m_pinsrd(__m64 m1, int i, const int c);
__m64 _m_pmaxsb(__m64 m1, __m64 m2);
__m64 _m_pmaxsw(__m64 m1, __m64 m2);
__m64 _m_pmaxsd(__m64 m1, __m64 m2);
__m64 _m_pmaxub(__m64 m1, __m64 m2);
__m64 _m_pmaxuw(__m64 m1, __m64 m2);
__m64 _m_pmaxud(__m64 m1, __m64 m2);
__m64 _m_pminsb(__m64 m1, __m64 m2);
__m64 _m_pminsw(__m64 m1, __m64 m2);
__m64 _m_pminsd(__m64 m1, __m64 m2);
__m64 _m_pminub(__m64 m1, __m64 m2);
__m64 _m_pminuw(__m64 m1, __m64 m2);
__m64 _m_pminud(__m64 m1, __m64 m2);
int _m_pmovmskb(__m64 m1);
int _m_pmovmskw(__m64 m1);
int _m_pmovmskd(__m64 m1);
__m64 _m_pshufw(__m64 m1, int i);
__m64 _m_pavgb(__m64 m1, __m64 m2);
__m64 _m_pavgw(__m64 m1, __m64 m2);
__m64 _m_pavg2b(__m64 m1, __m64 m2);
__m64 _m_pavg2w(__m64 m1, __m64 m2);
__m64 _m_psadabw(__m64 m1, __m64 m2, __m64 m3);
__m64 _m_psadawd(__m64 m1, __m64 m2, __m64 m3);
__m64 _m_psadzbw(__m64 m1, __m64 m2);
__m64 _m_psadzwd(__m64 m1, __m64 m2);
__m64 _m_paligniq(__m64 m1, __m64 m2, int i);
__m64 _m_cvt_si2pi(__int64 i);
__int64 _m_cvt_pi2si(__m64 m1);

/* Alternate intrinsic name definitions */
#define _mm_empty         _m_empty
#define _mm_cvtsi32_si64  _m_from_int
#define _mm_cvtsi64_si32  _m_to_int
#define _mm_packs_pi16    _m_packsswb
#define _mm_packs_pi32    _m_packssdw
#define _mm_packs_pu16    _m_packuswb
#define _mm_unpackhi_pi8  _m_punpckhbw
#define _mm_unpackhi_pi16 _m_punpckhwd
#define _mm_unpackhi_pi32 _m_punpckhdq
#define _mm_unpacklo_pi8  _m_punpcklbw
#define _mm_unpacklo_pi16 _m_punpcklwd
#define _mm_unpacklo_pi32 _m_punpckldq
#define _mm_packs_si64    _m_packssqd
#define _mm_packs_su64    _m_packusqd
#define _mm_packs_pu32    _m_packusdw
#define _mm_unpackeh_pi8  _m_punpckehsbw
#define _mm_unpackeh_pi16 _m_punpckehswd
#define _mm_unpackeh_pi32 _m_punpckehsdq
#define _mm_unpackeh_pu8  _m_punpckehubw
#define _mm_unpackeh_pu16 _m_punpckehuwd
#define _mm_unpackeh_pu32 _m_punpckehudq
#define _mm_unpackel_pi8  _m_punpckelsbw
#define _mm_unpackel_pi16 _m_punpckelswd
#define _mm_unpackel_pi32 _m_punpckelsdq
#define _mm_unpackel_pu8  _m_punpckelubw
#define _mm_unpackel_pu16 _m_punpckeluwd
#define _mm_unpackel_pu32 _m_punpckeludq
#define _mm_add_pi8       _m_paddb
#define _mm_add_pi16      _m_paddw
#define _mm_add_pi32      _m_paddd
#define _mm_adds_pi8      _m_paddsb
#define _mm_adds_pi16     _m_paddsw
#define _mm_adds_pi32     _m_paddsd
#define _mm_adds_pu8      _m_paddusb
#define _mm_adds_pu16     _m_paddusw
#define _mm_adds_pu32     _m_paddusd
#define _mm_sub_pi8       _m_psubb
#define _mm_sub_pi16      _m_psubw
#define _mm_sub_pi32      _m_psubd
#define _mm_subs_pi8      _m_psubsb
#define _mm_subs_pi16     _m_psubsw
#define _mm_subs_pi32     _m_psubsd
#define _mm_subs_pu8      _m_psubusb
#define _mm_subs_pu16     _m_psubusw
#define _mm_subs_pu32     _m_psubusd
#define _mm_madd_pi16     _m_pmaddwd
#define _mm_madd_pu16     _m_pmadduwd
#define _mm_mulhi_pi16    _m_pmulhw
#define _mm_mulhi_pu16    _m_pmulhuw
#define _mm_mullo_pi16    _m_pmullw
#define _mm_mac_pi16      _m_pmacsw
#define _mm_mac_pu16      _m_pmacuw
#define _mm_macz_pi16     _m_pmacszw
#define _mm_macz_pu16     _m_pmacuzw
#define _mm_acc_pu8       _m_paccb
#define _mm_acc_pu16      _m_paccw
#define _mm_acc_pu32      _m_paccd
#define _mm_mia_si64      _m_pmia
#define _mm_miaph_si64    _m_pmiaph
#define _mm_miabb_si64    _m_pmiabb
#define _mm_miabt_si64    _m_pmiabt
#define _mm_miatb_si64    _m_pmiatb
#define _mm_miatt_si64    _m_pmiatt
#define _mm_sll_pi16      _m_psllw
#define _mm_slli_pi16     _m_psllwi
#define _mm_sll_pi32      _m_pslld
#define _mm_slli_pi32     _m_pslldi
#define _mm_sll_si64      _m_psllq
#define _mm_slli_si64     _m_psllqi
#define _mm_sra_pi16      _m_psraw
#define _mm_srai_pi16     _m_psrawi
#define _mm_sra_pi32      _m_psrad
#define _mm_srai_pi32     _m_psradi
#define _mm_sra_si64      _m_psraq
#define _mm_srai_si64     _m_psraqi
#define _mm_srl_pi16      _m_psrlw
#define _mm_srli_pi16     _m_psrlwi
#define _mm_srl_pi32      _m_psrld
#define _mm_srli_pi32     _m_psrldi
#define _mm_srl_si64      _m_psrlq
#define _mm_srli_si64     _m_psrlqi
#define _mm_ror_pi16      _m_prorw
#define _mm_rori_pi16     _m_prorwi
#define _mm_ror_pi32      _m_prord
#define _mm_rori_pi32     _m_prordi
#define _mm_ror_si64      _m_prorq
#define _mm_rori_si64     _m_prorqi
#define _mm_and_si64      _m_pand
#define _mm_andnot_si64   _m_pandn
#define _mm_or_si64       _m_por
#define _mm_xor_si64      _m_pxor
#define _mm_cmpeq_pi8     _m_pcmpeqb
#define _mm_cmpeq_pi16    _m_pcmpeqw
#define _mm_cmpeq_pi32    _m_pcmpeqd
#define _mm_cmpgt_pi8     _m_pcmpgtb
#define _mm_cmpgt_pu8     _m_pcmpgtub
#define _mm_cmpgt_pi16    _m_pcmpgtw
#define _mm_cmpgt_pu16    _m_pcmpgtuw
#define _mm_cmpgt_pi32    _m_pcmpgtd
#define _mm_cmpgt_pu32    _m_pcmpgtud
#define _mm_extract_pi8   _m_pextrb
#define _mm_extract_pi16  _m_pextrw
#define _mm_extract_pi32  _m_pextrd
#define _mm_extract_pu8   _m_pextrub
#define _mm_extract_pu16  _m_pextruw
#define _mm_extract_pu32  _m_pextrud
#define _mm_insert_pi8    _m_pinsrb
#define _mm_insert_pi16   _m_pinsrw
#define _mm_insert_pi32   _m_pinsrd
#define _mm_max_pi8       _m_pmaxsb
#define _mm_max_pi16      _m_pmaxsw
#define _mm_max_pi32      _m_pmaxsd
#define _mm_max_pu8       _m_pmaxub
#define _mm_max_pu16      _m_pmaxuw
#define _mm_max_pu32      _m_pmaxud
#define _mm_min_pi8       _m_pminsb
#define _mm_min_pi16      _m_pminsw
#define _mm_min_pi32      _m_pminsd
#define _mm_min_pu8       _m_pminub
#define _mm_min_pu16      _m_pminuw
#define _mm_min_pu32      _m_pminud
#define _mm_movemask_pi8  _m_pmovmskb
#define _mm_movemask_pi16 _m_pmovmskw
#define _mm_movemask_pi32 _m_pmovmskd
#define _mm_shuffle_pi16  _m_pshufw
#define _mm_avg_pu8       _m_pavgb
#define _mm_avg_pu16      _m_pavgw
#define _mm_avg2_pu8      _m_pavg2b
#define _mm_avg2_pu16     _m_pavg2w
#define _mm_sada_pu8      _m_psadabw
#define _mm_sada_pu16     _m_psadawd
#if defined(_M_ARM)
#define _mm_sad_pu8       _m_psadzbw
#define _mm_sad_pu16      _m_psadzwd
#endif // _M_ARM
#define _mm_align_si64    _m_paligniq
#define _mm_cvtsi64_m64   _m_cvt_si2pi
#define _mm_cvtm64_si64   _m_cvt_pi2si

__m64 _mm_abs_pi8(__m64 m1);
__m64 _mm_abs_pi16(__m64 m1);
__m64 _mm_abs_pi32(__m64 m1);
__m64 _mm_absdiff_pu8(__m64 a, __m64 b);
__m64 _mm_absdiff_pu16(__m64 a, __m64 b);
__m64 _mm_absdiff_pu32(__m64 a, __m64 b);
__m64 _mm_addbhusm_pu8(__m64 a, __m64 b);
__m64 _mm_addbhusl_pu8(__m64 a, __m64 b);
__m64 _mm_addsubhx_pi16(__m64 a, __m64 b);
__m64 _mm_avg4_pu8(__m64 a, __m64 b);
__m64 _mm_avg4r_pu8(__m64 a, __m64 b);
__m64 _mm_merge_si64(__m64 a, __m64 b, const int n);
__m64 _mm_wmiabb_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiabt_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiatb_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiatt_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiabbn_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiabtn_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiatbn_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiattn_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiawbb_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiawbt_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiawtb_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiawtt_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiawbbn_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiawbtn_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiawtbn_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_wmiawttn_si64(__m64 a, __m64 b, __m64 c);
__m64 _mm_mulhi_pi32(__m64 a, __m64 b);
__m64 _mm_mulhi_pu32(__m64 a, __m64 b);
__m64 _mm_mullo_pi32(__m64 a, __m64 b);
__m64 _mm_mulhir_pi32(__m64 a, __m64 b);
__m64 _mm_mulhir_pu32(__m64 a, __m64 b);
__m64 _mm_qmiabb_pi32(__m64 a, __m64 b, __m64 c);
__m64 _mm_qmiabt_pi32(__m64 a, __m64 b, __m64 c);
__m64 _mm_qmiatb_pi32(__m64 a, __m64 b, __m64 c);
__m64 _mm_qmiatt_pi32(__m64 a, __m64 b, __m64 c);
__m64 _mm_qmiabbn_pi32(__m64 a, __m64 b, __m64 c);
__m64 _mm_qmiabtn_pi32(__m64 a, __m64 b, __m64 c);
__m64 _mm_qmiatbn_pi32(__m64 a, __m64 b, __m64 c);
__m64 _mm_qmiattn_pi32(__m64 a, __m64 b, __m64 c);
__m64 _mm_qmulm_pi16(__m64 a, __m64 b);
__m64 _mm_qmulmr_pi16(__m64 a, __m64 b);
__m64 _mm_qmulm_pi32(__m64 a, __m64 b);
__m64 _mm_qmulmr_pi32(__m64 a, __m64 b);
__m64 _mm_subaddhx_pi16( __m64 a, __m64 b);
__m64 _mm_addc_pu16(__m64 a, __m64 b);
__m64 _mm_addc_pu32(__m64 a, __m64 b);
__m64 _mm_mulhir_pi16(__m64 a, __m64 b);
__m64 _mm_mulhir_pu16(__m64 a, __m64 b);
__m64 _mm_maddx_pi16(__m64 a, __m64 b);
__m64 _mm_maddx_pu16(__m64 a, __m64 b);
__m64 _mm_msub_pi16(__m64 a, __m64 b);
__m64 _mm_msub_pu16(__m64 a, __m64 b);

#if defined __cplusplus
}; /* End "C" */
#endif /* __cplusplus */

#endif /* _M_IX86 || (_M_ARM && (_MSC_VER >= 1400)) */

#endif /* _MMINTRIN_H_INCLUDED */

