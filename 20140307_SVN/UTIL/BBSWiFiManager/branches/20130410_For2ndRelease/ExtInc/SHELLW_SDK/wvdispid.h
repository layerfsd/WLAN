//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft
// premium shared source license agreement under which you licensed
// this source code. If you did not accept the terms of the license
// agreement, you are not authorized to use this source code.
// For the terms of the license, please see the license agreement
// signed by you and Microsoft.
// THE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define DISPID_BROWSERNAVIGATE          150
#define DISPID_BROWSERREFRESH           151
#define DISPID_BROWSERSTOP              152
#define DISPID_BROWSERBUSY              153
#define DISPID_BROWSERTITLE             154
#define DISPID_BROWSERLOCATION          155
#define DISPID_BROWSERISFRAME           156
#define DISPID_BROWSERSCROLLSTYLE       157
#define DISPID_BROWSERMARGINWIDTH       158
#define DISPID_BROWSERMARGINHEIGHT      159
#define DISPID_BROWSERFRAMENAVIGATE     160
#define DISPID_BROWSERSELECTEDCODEPAGE  161
#define DISPID_BROWSERPOINTER           162
#define DISPID_BROWSERDOCUMENT          163
#define DISPID_BROWSERSCRIPT            164
#define DISPID_BROWSERFOCUSRECTENABLED  165

#define DISPID_BROWSERLAYOUTWIDTH       175
#define DISPID_BROWSERLAYOUTHEIGHT      176
#define DISPID_BROWSERLOCATIONBASEURL   177
#define DISPID_BROWSERCLEARTYPEENABLED  178
#define DISPID_BROWSERSCRIPTINGENABLED  179
#define DISPID_BROWSERFITTOWINDOW       180
#define DISPID_BROWSERSHOWIMAGES        181
#define DISPID_BROWSERPLAYSOUNDS        182
#define DISPID_BROWSERZOOMLEVEL         183
#define DISPID_BROWSERINTEGRALPAGING    184
#define DISPID_BROWSERCONTEXTMENU       185
#define DISPID_BROWSERACTIVEXENABLED    186
#define DISPID_BROWSERSELECTIONENABLED  187
#define DISPID_BROWSERCANCOPY           188

#define DISPID_FRAMEBEFORENAVIGATE      200
#define DISPID_FRAMENAVIGATECOMPLETE    201
#define DISPID_FRAMENEWWINDOW           204

#define DISPID_HTMLWINDOWEVENTS_ONLOAD              0x0100
#define DISPID_HTMLWINDOWEVENTS_ONUNLOAD            0x0101
#define DISPID_HTMLWINDOWEVENTS_ONERROR             0x0102

#define DISPID_HTMLFRAMESETEVENTS_ONLOAD            0x0116
#define DISPID_HTMLFRAMESETEVENTS_ONUNLOAD          0x0117

#define DISPID_HTMLELEMENTEVENTS_ONCLICK            0x0120

#define DISPID_HTMLANCHORELEMENTEVENTS_ONCLICK      0x0130
#define DISPID_HTMLANCHORELEMENTEVENTS_ONFOCUS      0x0131
#define DISPID_HTMLANCHORELEMENTEVENTS_ONBLUR       0x0132

#define DISPID_HTMLFORMELEMENTEVENTS_ONSUBMIT       0x0140
#define DISPID_HTMLFORMELEMENTEVENTS_ONRESET        0x0141

#define DISPID_HTMLINPUTELEMENTEVENTS_ONCLICK       0x0151
#define DISPID_HTMLINPUTELEMENTEVENTS_ONFOCUS       0x0152
#define DISPID_HTMLINPUTELEMENTEVENTS_ONBLUR        0x0153
#define DISPID_HTMLINPUTTEXTELEMENTEVENTS_ONCHANGE  0x0154

#define DISPID_HTMLSELECTELEMENTEVENTS_ONCHANGE     0x0160
#define DISPID_HTMLSELECTELEMENTEVENTS_ONFOCUS      0x0161
#define DISPID_HTMLSELECTELEMENTEVENTS_ONBLUR       0x0162

#define DISPID_HTMLTEXTAREAELEMENTEVENTS_ONCHANGE   0x0170
#define DISPID_HTMLTEXTAREAELEMENTEVENTS_ONFOCUS    0x0171
#define DISPID_HTMLTEXTAREAELEMENTEVENTS_ONBLUR     0x0172

#define DISPID_HTMLIMGELEMENTEVENTS_ONLOAD          0x0180

#define DISPID_HTMLDOCUMENT_BASE                    0x1000
#define DISPID_HTMLDOCUMENT_TITLE_PROPGET           (DISPID_HTMLDOCUMENT_BASE + 7)
#define DISPID_HTMLDOCUMENT_TITLE_PROPPUT           (DISPID_HTMLDOCUMENT_BASE + 142)
#define DISPID_HTMLWINDOW_BASE                      0x1100
#define DISPID_HTMLFRAMEELEMENT_BASE                0x1200
#define DISPID_HTML_SCREEN_BASE                     0x1300
#define DISPID_HTMLOBJECTELEMENT_BASE               0x1400
#define DISPID_HTMLELEMENT_BASE                     0x60030001  // backwards compat with MIDL generated ids
#define DISPID_HTMLELEMENT2_BASE                    0x1500
#define DISPID_HTMLELEMENT3_BASE                    DISPID_HTMLELEMENT2_BASE + 7
#define DISPID_HTMLELEMENT3_LAST                    DISPID_HTMLELEMENT3_BASE + 7
#define DISPID_HTMLELEMENT_ID_PROPGET               (DISPID_HTMLELEMENT_BASE)
#define DISPID_HTMLELEMENT_ID_PROPPUT               (DISPID_HTMLELEMENT3_BASE + 6)
#define DISPID_HTMLIFRAMEELEMENT_BASE               0x1520
#define DISPID_HTMLBODYELEMENT_BASE                 0x1550


#define DISPID_IHTMLSTYLE_CSSTEXT                   1000

#define DISPID_IHTMLSTYLE_FIRST                     1001

#define DISPID_IHTMLSTYLE_BACKGROUNDATTACHMENT      DISPID_IHTMLSTYLE_FIRST
#define DISPID_IHTMLSTYLE_BACKGROUNDCOLOR           DISPID_IHTMLSTYLE_BACKGROUNDATTACHMENT + 1
#define DISPID_IHTMLSTYLE_BACKGROUNDIMAGE           DISPID_IHTMLSTYLE_BACKGROUNDCOLOR + 1
#define DISPID_IHTMLSTYLE_BACKGROUNDPOSITION        DISPID_IHTMLSTYLE_BACKGROUNDIMAGE + 1
#define DISPID_IHTMLSTYLE_BACKGROUNDREPEAT          DISPID_IHTMLSTYLE_BACKGROUNDPOSITION + 1
#define DISPID_IHTMLSTYLE_BEHAVIOR                  DISPID_IHTMLSTYLE_BACKGROUNDREPEAT + 1
#define DISPID_IHTMLSTYLE_BORDERBOTTOMCOLOR         DISPID_IHTMLSTYLE_BEHAVIOR + 1
#define DISPID_IHTMLSTYLE_BORDERBOTTOMSTYLE         DISPID_IHTMLSTYLE_BORDERBOTTOMCOLOR + 1 
#define DISPID_IHTMLSTYLE_BORDERBOTTOMWIDTH         DISPID_IHTMLSTYLE_BORDERBOTTOMSTYLE + 1
#define DISPID_IHTMLSTYLE_BORDERLEFTCOLOR           DISPID_IHTMLSTYLE_BORDERBOTTOMWIDTH + 1
#define DISPID_IHTMLSTYLE_BORDERLEFTSTYLE           DISPID_IHTMLSTYLE_BORDERLEFTCOLOR + 1
#define DISPID_IHTMLSTYLE_BORDERLEFTWIDTH           DISPID_IHTMLSTYLE_BORDERLEFTSTYLE + 1
#define DISPID_IHTMLSTYLE_BORDERRIGHTCOLOR          DISPID_IHTMLSTYLE_BORDERLEFTWIDTH + 1
#define DISPID_IHTMLSTYLE_BORDERRIGHTSTYLE          DISPID_IHTMLSTYLE_BORDERRIGHTCOLOR + 1
#define DISPID_IHTMLSTYLE_BORDERRIGHTWIDTH          DISPID_IHTMLSTYLE_BORDERRIGHTSTYLE + 1
#define DISPID_IHTMLSTYLE_BORDERTOPCOLOR            DISPID_IHTMLSTYLE_BORDERRIGHTWIDTH + 1
#define DISPID_IHTMLSTYLE_BORDERTOPSTYLE            DISPID_IHTMLSTYLE_BORDERTOPCOLOR + 1
#define DISPID_IHTMLSTYLE_BORDERTOPWIDTH            DISPID_IHTMLSTYLE_BORDERTOPSTYLE + 1
#define DISPID_IHTMLSTYLE_CLEAR                     DISPID_IHTMLSTYLE_BORDERTOPWIDTH + 1
#define DISPID_IHTMLSTYLE_COLOR                     DISPID_IHTMLSTYLE_CLEAR + 1
#define DISPID_IHTMLSTYLE_DISPLAY                   DISPID_IHTMLSTYLE_COLOR + 1
#define DISPID_IHTMLSTYLE_STYLEFLOAT                DISPID_IHTMLSTYLE_DISPLAY + 1
#define DISPID_IHTMLSTYLE_FONTFAMILY                DISPID_IHTMLSTYLE_STYLEFLOAT + 1
#define DISPID_IHTMLSTYLE_FONTSIZE                  DISPID_IHTMLSTYLE_FONTFAMILY + 1
#define DISPID_IHTMLSTYLE_FONTSTYLE                 DISPID_IHTMLSTYLE_FONTSIZE + 1
#define DISPID_IHTMLSTYLE_FONTVARIANT               DISPID_IHTMLSTYLE_FONTSTYLE + 1
#define DISPID_IHTMLSTYLE_FONTWEIGHT                DISPID_IHTMLSTYLE_FONTVARIANT + 1
#define DISPID_IHTMLSTYLE_HEIGHT                    DISPID_IHTMLSTYLE_FONTWEIGHT + 1
#define DISPID_IHTMLSTYLE_LISTSTYLEIMAGE            DISPID_IHTMLSTYLE_HEIGHT + 1
#define DISPID_IHTMLSTYLE_LISTSTYLEPOSITION         DISPID_IHTMLSTYLE_LISTSTYLEIMAGE + 1
#define DISPID_IHTMLSTYLE_LISTSTYLETYPE             DISPID_IHTMLSTYLE_LISTSTYLEPOSITION + 1
#define DISPID_IHTMLSTYLE_MARGINBOTTOM              DISPID_IHTMLSTYLE_LISTSTYLETYPE + 1
#define DISPID_IHTMLSTYLE_MARGINLEFT                DISPID_IHTMLSTYLE_MARGINBOTTOM + 1
#define DISPID_IHTMLSTYLE_MARGINRIGHT               DISPID_IHTMLSTYLE_MARGINLEFT + 1
#define DISPID_IHTMLSTYLE_MARGINTOP                 DISPID_IHTMLSTYLE_MARGINRIGHT + 1
#define DISPID_IHTMLSTYLE_OVERFLOW                  DISPID_IHTMLSTYLE_MARGINTOP + 1
#define DISPID_IHTMLSTYLE_PADDINGBOTTOM             DISPID_IHTMLSTYLE_OVERFLOW + 1
#define DISPID_IHTMLSTYLE_PADDINGLEFT               DISPID_IHTMLSTYLE_PADDINGBOTTOM + 1
#define DISPID_IHTMLSTYLE_PADDINGRIGHT              DISPID_IHTMLSTYLE_PADDINGLEFT + 1
#define DISPID_IHTMLSTYLE_PADDINGTOP                DISPID_IHTMLSTYLE_PADDINGRIGHT + 1
#define DISPID_IHTMLSTYLE_TEXTALIGN                 DISPID_IHTMLSTYLE_PADDINGTOP + 1
#define DISPID_IHTMLSTYLE_TEXTDECORATION            DISPID_IHTMLSTYLE_TEXTALIGN + 1
#define DISPID_IHTMLSTYLE_TEXTINDENT                DISPID_IHTMLSTYLE_TEXTDECORATION + 1
#define DISPID_IHTMLSTYLE_TEXTOVERFLOW              DISPID_IHTMLSTYLE_TEXTINDENT + 1
#define DISPID_IHTMLSTYLE_TEXTTRANSFORM             DISPID_IHTMLSTYLE_TEXTOVERFLOW + 1
#define DISPID_IHTMLSTYLE_VERTICALALIGN             DISPID_IHTMLSTYLE_TEXTTRANSFORM + 1
#define DISPID_IHTMLSTYLE_VISIBILITY                DISPID_IHTMLSTYLE_VERTICALALIGN + 1
#define DISPID_IHTMLSTYLE_WHITESPACE                DISPID_IHTMLSTYLE_VISIBILITY + 1
#define DISPID_IHTMLSTYLE_WIDTH                     DISPID_IHTMLSTYLE_WHITESPACE + 1
#define DISPID_IHTMLSTYLE_WAPMARQUEEDIR             DISPID_IHTMLSTYLE_WIDTH + 1
#define DISPID_IHTMLSTYLE_WAPMARQUEELOOP            DISPID_IHTMLSTYLE_WAPMARQUEEDIR + 1
#define DISPID_IHTMLSTYLE_WAPMARQUEESTYLE           DISPID_IHTMLSTYLE_WAPMARQUEELOOP + 1
#define DISPID_IHTMLSTYLE_WAPACCESSKEY              DISPID_IHTMLSTYLE_WAPMARQUEESTYLE + 1
#define DISPID_IHTMLSTYLE_WAPINPUTFORMAT            DISPID_IHTMLSTYLE_WAPACCESSKEY + 1
#define DISPID_IHTMLSTYLE_WAPINPUTREQUIRED          DISPID_IHTMLSTYLE_WAPINPUTFORMAT + 1
#define DISPID_IHTMLSTYLE_LINEHEIGHT                DISPID_IHTMLSTYLE_WAPINPUTREQUIRED + 1
#define DISPID_IHTMLSTYLE_LEFTPOS                   DISPID_IHTMLSTYLE_LINEHEIGHT + 1
#define DISPID_IHTMLSTYLE_RIGHTPOS                  DISPID_IHTMLSTYLE_LEFTPOS + 1
#define DISPID_IHTMLSTYLE_TOPPOS                    DISPID_IHTMLSTYLE_RIGHTPOS + 1
#define DISPID_IHTMLSTYLE_BOTTOMPOS                 DISPID_IHTMLSTYLE_TOPPOS + 1
#define DISPID_IHTMLSTYLE_ZINDEX                    DISPID_IHTMLSTYLE_BOTTOMPOS + 1