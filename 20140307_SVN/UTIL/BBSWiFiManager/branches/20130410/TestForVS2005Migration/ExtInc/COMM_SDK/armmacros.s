;
; Copyright (c) Microsoft Corporation.  All rights reserved.
;
;
; Use of this sample source code is subject to the terms of the Microsoft
; license agreement under which you licensed this sample source code. If
; you did not accept the terms of the license agreement, you are not
; authorized to use this sample source code. For the terms of the license,
; please see the license agreement between you and Microsoft or, if applicable,
; see the LICENSE.RTF on your install media or the root of your tools installation.
; THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
;

;/***********************************************************************
;  Copyright (c) ARM Limited 1998.  All rights reserved.
;
; NOTE: The CALL and CALLEQ macros shouldn't be used in OS startup code
;       before the MMU is enabled since in the Thumbing case it relies
;       on an absolute address which will be virtual in the case of the
;       OS code and if the MMU isn't enabled, all branches should be
;       relative (or a computed physical address).
;
;***********************************************************************/

  IF Interworking :LOR: Thumbing

    MACRO
    CALL $Fn
    ldr     r12, =$Fn
    mov     lr, pc
    bx      r12
    MEND

    MACRO
    CALLEQ $Fn
    ldreq   r12, =$Fn
    moveq   lr, pc
    bxeq    r12
    MEND

    MACRO
    RETURN
    bx      lr
    MEND

    MACRO
    RETURN_EQ
    bxeq    lr
    MEND

    MACRO
    RETURN_NE
    bxne    lr
    MEND

  ELSE

    MACRO
    CALL $Fn
    bl      $Fn
    MEND

    MACRO
    CALLEQ $Fn
    bleq    $Fn
    MEND

    MACRO
    RETURN
    mov     pc, lr
    MEND

    MACRO
    RETURN_EQ
    moveq   pc, lr
    MEND

    MACRO
    RETURN_NE
    movne   pc, lr
    MEND

  ENDIF

    END 
