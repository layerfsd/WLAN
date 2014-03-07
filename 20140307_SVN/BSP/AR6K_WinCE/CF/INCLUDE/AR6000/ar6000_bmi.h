//------------------------------------------------------------------------------
// <copyright file="AR6000_bmi.h" company="Atheros">
//    Copyright (c) 2006 Microsoft Corporation.  All rights reserved.
//    Copyright (c) 2006 Atheros Corporation.  All rights reserved.
//
//    The use and distribution terms for this software are covered by the
//    Microsoft Limited Permissive License (Ms-LPL) 
//    http://www.microsoft.com/resources/sharedsource/licensingbasics/limitedpermissivelicense.mspx 
//    which can be found in the file MS-LPL.txt at the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by
//    the terms of this license.
//
//    The software is licensed “as-is.” 
//    You must not remove this notice, or any other, from this software.
// </copyright>
// 
// <summary>
//    Windows CE Wifi Driver for AR-6000
// </summary>
//------------------------------------------------------------------------------
//==============================================================================
// BMI command definitions
//
// Author(s): ="Atheros"
//==============================================================================
#ifndef __AR6000_BMI_H__
#define __AR6000_BMI_H__

/*
 * Bootloader Messaging Interface (BMI)
 *
 * BMI is a very simple messaging interface used during initialization
 * to read memory, write memory, execute code, and to define an
 * application entry PC.
 *
 * It is used to download an application to AR6000, to provide
 * patches to code that is already resident on AR6000, and generally
 * to examine and modify state.  The Host has an opportunity to use
 * BMI only once during bootup.  Once the Host issues a BMI_DONE
 * command, this opportunity ends.
 *
 * The Host writes BMI requests to mailbox0, and reads BMI responses
 * from mailbox0.   BMI requests all begin with a command
 * (see below for specific commands), and are followed by
 * command-specific data.
 *
 * Flow control:
 * The Host can only issue a command once the Target gives it a
 * "BMI Command Credit", using AR6000 Counter #4.  As soon as the
 * Target has completed a command, it issues another BMI Command
 * Credit (so the Host can issue the next command).
 *
 * BMI handles all required Target-side cache flushing.
 */


/* Maximum data size used for BMI transfers */
#define BMI_DATASZ_MAX                     32

/* BMI Commands */

#define BMI_NO_COMMAND                      0

#define BMI_DONE                            1
        /*
         * Semantics: Host is done using BMI
         * Request format:
         *    A_UINT32      command (BMI_DONE)
         * Response format: none
         */

#define BMI_READ_MEMORY                     2
        /*
         * Semantics: Host reads AR6000 memory
         * Request format:
         *    A_UINT32      command (BMI_READ_MEMORY)
         *    A_UINT32      address
         *    A_UINT32      length, at most BMI_DATASZ_MAX
         * Response format:
         *    A_UINT8       data[length]
         */

#define BMI_WRITE_MEMORY                    3
        /*
         * Semantics: Host writes AR6000 memory
         * Request format:
         *    A_UINT32       command (BMI_WRITE_MEMORY)
         *    A_UINT32      address
         *    A_UINT32      length, at most BMI_DATASZ_MAX
         *    A_UINT8       data[length]
         * Response format: none
         */

#define BMI_EXECUTE                         4
        /*
         * Semantics: Causes AR6000 to execute code
         * Request format:
         *    A_UINT32      command (BMI_EXECUTE)
         *    A_UINT32      address
         *    A_UINT32      parameter
         * Response format:
         *    A_UINT32      return value
         */

#define BMI_SET_APP_START                   5
        /*
         * Semantics: Set Target application starting address
         * Request format:
         *    A_UINT32      command (BMI_SET_APP_START)
         *    A_UINT32      address
         * Response format: none
         */

#define BMI_READ_SOC_REGISTER               6
        /*
         * Semantics: Read a 32-bit Target SOC register.
         * Request format:
         *    A_UINT32      command (BMI_READ_REGISTER)
         *    A_UINT32      address
         * Response format: 
         *    A_UINT32      value
         */

#define BMI_WRITE_SOC_REGISTER              7
        /*
         * Semantics: Write a 32-bit Target SOC register.
         * Request format:
         *    A_UINT32      command (BMI_WRITE_REGISTER)
         *    A_UINT32      address
         *    A_UINT32      value
         *
         * Response format: none
         */

#define BMI_GET_TARGET_ID                  8
        /*
         * Semantics: Fetch the 4-byte Target ID.
         * Request format:
         *    A_UINT32      command (BMI_GET_TARGET_ID)
         *
         * Response format:
         *    A_UINT32      TargetID
         */

#endif /* __AR6000_BMI_H__ */
