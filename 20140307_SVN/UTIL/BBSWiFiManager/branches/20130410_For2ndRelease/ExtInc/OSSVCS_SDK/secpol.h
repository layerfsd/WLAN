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
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// System Security Policies
// Please keep them in numerical order.
//

// This policy setting determines whether applications stored on an MMC card are allowed to
// auto-run when inserted into the device.
//      0x00000000 - Applications ARE allowed to auto-run (default)
//      0x00000001 - Applications are NOT allowed to auto-run
#define SECPOLICY_CFAUTORUN                     0x00000002

#define POLICYVAL_CFAUTORUN_ALLOWED             0x00000000
#define POLICYVAL_CFAUTORUN_DISABLED            0x00000001


// This policy setting determines APPS Policy
#define POLICYVAL_APPS_DISALLOW       0
#define POLICYVAL_APPS_ALLOW          1

// This policy setting enforces whether RAPI calls can be used on the device
#define SECPOLICY_RAPI                          0x1001

#define SECPOLICY_RAPI_DISABLED                 0
#define SECPOLICY_RAPI_ALLOWED                  1
#define SECPOLICY_RAPI_RESTRICTED_MODE          2

// This policy setting determines the role mask for unsigned cabs
#define SECPOLICY_UNSIGNEDCABS                  0x1005

// This policy setting enforces whether unsigned applications 
// are allowed to run on the device.
#define SECPOLICY_UNSIGNEDAPPS                  0x1006

// This policy setting determines the role mask for processing .xml file for .hme and .tsk files
#define SECPOLICY_UNSIGNEDTHEMES                0x1007

// This policy setting determines whether carriers are allowed 
// to be assigned Trusted Provisioning Server (TPS) role.
#define SECPOLICY_TPSCARRIERROLE                0x1008

// This policy setting defines the maximum number allowed of retry 
// times for the user to authenticate a WAP pin-signed message.
#define SECPOLICY_MAXAUTHENTICATIONRETRY        0x1009

// SECPOLICY_WAPSIGNEDMSG is deprecated. Use SECPOLICY_OMACPNETWPINMSG, SECPOLICY_OMACPUSERPINMSG and SECPOLICY_OMACPUSERNETWPINMSG instead

// This policy setting determines how SL signed messages 
// should be processed based on origin.
#define SECPOLICY_SL_MESSAGE                    0x100c

// This policy setting determines how SI signed messages 
// should be processed based on origin.
#define SECPOLICY_SI_MESSAGE                    0x100d

// This policy setting determines whether to accept non WAP-signed and 
// non ESP-encrypted messages, processed by the default security provider 
// in Security Module, based on their origin.
#define SECPOLICY_UNAUTHMESSAGES                0x100e

// This policy setting determines which provisioning messages are accepted 
// by cfghost based on the message's role(s).  This policy is used to limit 
// provisioning messages that come from Push Router.
#define SECPOLICY_OTAPROVISIONING               0x100f

// This policy setting determines whether routing of WSP notifications 
// from WAP stack is allowed.
#define SECPOLICY_WSPNOTIFICATIONS              0x1011

// This policy allows granting other roles administrative privileges to the system 
// without having to explicitly acquire the manager role.  
#define SECPOLICY_GRANTMANAGER                  0x1017

// This policy allows other roles to impersonate the User Auth role.
#define SECPOLICY_GRANTUSERAUTH                 0x1018

// This policy specifies the level of permissions required to create, modify, 
// and delete a trusted push proxy gateway (PPG).
#define SECPOLICY_TRUSTED_WAP_PROXY             0x1019

// This policy controls whether the user will be prompted with a Yes/No
// dialog before an unsigned application is allowed to install or execute.
// If the policy is equal to 1, then the user is never prompted.
#define SECPOLICY_UNSIGNEDPROMPT                0x101a

// This policy controls whether the 1- or 2-tier security model is in use.
// If 0, then 1-tier mode is disabled, apps run either privileged/unprivileged
// depending on how they're signed.
// If 1, then 1-tier mode is enabled and all apps run privileged, if they're
// allowed to run at all. 
#define SECPOLICY_PRIVILEGEDAPPS                0x101b

// SL security policy
// Operator can override https to use http or wsps to wsp.
// Value of 0 means we need to use https or wsps
// Value of 1 means we can use http or wsp
#define SECPOLICY_SLSECUREDOWNLOAD              0x101c

// This policy determines whether the Inbox app will send all messages signed.
// If sending signed, it signifies which algorithm to use.
//
// This policy has been deprecated! Use SECPOLICY_SMIMESIGNING and
// SECPOLICY_SMIMESIGNINGALGORITHM instead.
#define SECPOLICY_USESIGN                       0x101d

#define POLICYVAL_USESIGN_DEFAULT               0x00000000
#define POLICYVAL_USESIGN_NONE                  0x00000001
#define POLICYVAL_USESIGN_SHA_1                 0x00000002
#define POLICYVAL_USESIGN_MD5                   0x00000003

// This policy determines whether the Inbox app will send all messages encrypted.
// If sending encrypted, it signifies which algorithm to use.
//
// This policy has been deprecated! Use SECPOLICY_SMIMEENCRYPTION and
// SECPOLICY_SMIMEENCRYPTIONALGORITHM instead.
#define SECPOLICY_USEENCRYPT                    0x101e

#define POLICYVAL_USEENCRYPT_DEFAULT            0x00000000
#define POLICYVAL_USEENCRYPT_NONE               0x00000001
#define POLICYVAL_USEENCRYPT_3DES               0x00000002
#define POLICYVAL_USEENCRYPT_DES                0x00000003
#define POLICYVAL_USEENCRYPT_RC2_128            0x00000004
#define POLICYVAL_USEENCRYPT_RC2_64             0x00000005
#define POLICYVAL_USEENCRYPT_RC2_40             0x00000006

// This policy determines whether software certificates can be used to
// sign outgoing messages.  A value of 0 indicates that software certificates
// CANNOT be used to sign messages, and a value of 1 indicates that they CAN.
#define SECPOLICY_SOFTCERTS                     0x101f

// This policy determines which DRM rights messages are accepted by the DRM engine
// based on the message's role(s).
#define SECPOLICY_DRM_WAPRIGHTS                 0x1021

// This policy determines whether the OMA device management transport client 
// will enforce SSL usage when connecting to a server, no matter what is bootstrapped.  
// By default, the client always uses SSL.
#define SECPOLICY_OMADMSSLCONN                  0x1022

#define POLICYVAL_OMADMSSLCONN_NO_ENFORCE       0x1

// This policy specifies if a password must be configured 
// on the device (if it is not already configured).
//     0 - Password is required
//     Any other value - Password is not required
#define SECPOLICY_LASS_PWD_REQUIRED             0x1023

#define POLICYVAL_LASS_PWD_REQUIRED             0x0


// This policy determines whether to display a prompt for network-pin-signed
// WAP provisioning messages.
#define SECPOLICY_WAP_NETWPIN_PROMPT            0x1024

#define POLICYVAL_WAP_NETWPIN_PROMPT            0x0

#define POLICYVAL_WAP_NETWPIN_NO_PROMPT         0x1


// This policy specifies how desktop should handle device authentication.
// 0 - User must authenticate on device if locked upon connect
// 1 - User can authenticate through PIN on desktop
//
// Note: This policy is now obsolete for WM6 and greater devices. 
//       These devices can not be unlocked by a PIN from the desktop 
//       See new policy (SECPOLICY_LASS_DESKTOP_QUICK_CONNECT)
 
#define SECPOLICY_LASS_DESKTOP                  0x1025
 
#define POLICYVAL_LASS_DESKTOP_NO_AUTH          0x0
#define POLICYVAL_LASS_DESKTOP_AUTH_ALLOWED     0x1


// This policy specifies if the user is allowed to change mobile encryption
// settings for removable storage.
// 0 - User can NOT change settings
// 1 - User CAN change settings

#define SECPOLICY_MENCRYPT_REMOVABLE            0x1026
 
#define POLICYVAL_MENCRYPT_REMOVABLE_NO_USER    0x0
#define POLICYVAL_MENCRYPT_REMOVABLE_USER_ALLOW 0x1

// This policy is used to determine if a bluetooth enabled device is allowed
// to be searched by other devices

#define SECPOLICY_BLUETOOTH                     0x1027

#define POLICYVAL_BLUETOOTH_VISIBLE_BLOCKED  0x0
#define POLICYVAL_BLUETOOTH_VISIBLE_ALLOWED  0x1

// This policy specifies whether message transports can handle HTML messages
// 0 - HTML message is disabled. Message is processed as plain text
// 1 - HTML message is enabled. 
#define SECPOLICY_HTML_MESSAGE                  0x1028

#define POLICYVAL_HTML_MESSAGE_DISABLED         0
#define POLICYVAL_HTML_MESSAGE_ENABLED          1

// This policy determines whether the Inbox app will send all messages 
// signed.
#define SECPOLICY_SMIMESIGNING                  0x1029

#define POLICYVAL_SMIMESIGNING_FORCED           0x00000000
#define POLICYVAL_SMIMESIGNING_OPTIONAL         0x00000001

// This policy determines whether the Inbox app will send all messages 
// encrypted.
#define SECPOLICY_SMIMEENCRYPTION               0x102A

#define POLICYVAL_SMIMEENCRYPTION_FORCED        0x00000000
#define POLICYVAL_SMIMEENCRYPTION_OPTIONAL      0x00000001

// This policy determines which algorithm will be used by the Inbox app 
// when a message is to be signed.
// Uses same value range specified for SECPOLICY_USESIGN policy.
// Algorithm specified by SECPOLICY_USESIGN overrides this policy.
#define SECPOLICY_SMIMESIGNINGALGORITHM         0x102B

#define POLICYVAL_SMIMESIGNINGALGORITHM_DEFAULT 0x00000000
#define POLICYVAL_SMIMESIGNINGALGORITHM_SHA_1   0x00000002
#define POLICYVAL_SMIMESIGNINGALGORITHM_MD5     0x00000003


// This policy determines which algorithm will be used by the Inbox app 
// when a message is to be encrypted.
// Uses same value range specified for SECPOLICY_USEENCRYPT policy.
// Algorithm specified by SECPOLICY_USEENCRYPT overrides this policy.
#define SECPOLICY_SMIMEENCRYPTIONALGORITHM      0x102C

#define POLICYVAL_SMIMEENCRYPTIONALGORITHM_DEFAULT  0x00000000
#define POLICYVAL_SMIMEENCRYPTIONALGORITHM_3DES     0x00000002
#define POLICYVAL_SMIMEENCRYPTIONALGORITHM_DES      0x00000003
#define POLICYVAL_SMIMEENCRYPTIONALGORITHM_RC2_128  0x00000004
#define POLICYVAL_SMIMEENCRYPTIONALGORITHM_RC2_64   0x00000005
#define POLICYVAL_SMIMEENCRYPTIONALGORITHM_RC2_40   0x00000006

// These policies replace SECPOLICY_WAPSIGNEDMSG
// These policies determine which origin roles are allowed for the various WAP authentication methods (NETWPIN, USERPIN/USERPINMAC and USERNETWPIN)
// Based on WAP authentication method, the message's role mask is AND-ed with the corresponding policy's role mask. 
// If the result is non-zero, the message is allowed for authentication

#define SECPOLICY_OMACPNETWPINMSG               0x102d
#define SECPOLICY_OMACPUSERPINMSG               0x102e
#define SECPOLICY_OMACPUSERNETWPINMSG           0x102f


// This policy determines if messaging can negotiate to another 
// encryption algorithm during SMIME encryption, if a recipient's 
// certificate is not supporting desired encryption algorithm. 
#define SECPOLICY_SMIMEENCRYPTIONNEGOTIATION    0x1030

#define POLICYVAL_SMIMEENCRYPTIONNEGOTIATION_NONE           0x00000000
#define POLICYVAL_SMIMEENCRYPTIONNEGOTIATION_ALLOWSTRONG    0x00000001
#define POLICYVAL_SMIMEENCRYPTIONNEGOTIATION_ALLOWALL       0x00000002

// SECPOLICY_SHAREPOINTUNCPROTOCOLACCESS - so that IT admins can turn off
// sharepoint for devices but not OWA; also gives them an escape hatch if
// this turns out to be a bad user experience in some network environments.
#define SECPOLICY_SHAREPOINTUNCPROTOCOLACCESS   0x1031
#define POLICYVAL_SHAREPOINTUNCPROTOCOLACCESS_DISALLOW      0x00000000
#define POLICYVAL_SHAREPOINTUNCPROTOCOLACCESS_ALLOW         0x00000001


// This policy specifies how desktop should handle quick connect authentication.
// 0 - User must authenticate on device upon connect, if device lock is active.
// 1 - User can quick authenticate to the desktop
#define SECPOLICY_LASS_DESKTOP_QUICK_CONNECT             0x1032
 
#define POLICYVAL_LASS_DESKTOP_QUICK_CONNECT_DISALLOWED  0x0
#define POLICYVAL_LASS_DESKTOP_QUICK_CONNECT_ALLOWED     0x1



// This policy specifies whether SMS and MMS services should be disabled.
#define SECPOLICY_MSGSERVICE_SMS_MMS                    0x1033

#define POLICYVAL_MSGSERVICE_SMS_MMS_DISABLED           0x0
#define POLICYVAL_MSGSERVICE_SMS_MMS_ENABLED            0x1



// This policy specifies whether internet email (non-outlook) service
// should be disabled.
#define SECPOLICY_MSGSERVICE_INTERNET_EMAIL             0x1034

#define POLICYVAL_MSGSERVICE_INTERNET_EMAIL_DISABLED    0x0
#define POLICYVAL_MSGSERVICE_INTERNET_EMAIL_ENABLED     0x1


