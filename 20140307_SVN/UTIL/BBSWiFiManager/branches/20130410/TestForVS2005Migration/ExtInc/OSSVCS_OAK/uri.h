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
// ************************************************************
// uri.h
// 
// URI handling class
//
// Copyright 2003 Microsoft Corporation, All Rights Reserved
//
// ************************************************************

#pragma once


// See RFC 2396 for URI definition.
#define DELIMITER_CHAR      TEXT('/')
#define QUERY_CHAR          TEXT('?')


// For now, URI validation consists of making sure that:
//          1. the URI does not contain spaces.
//          2. the URI does not end in a delimiter character.
//          3. the URI does not contain more than one query character.
//          4. the URI does not have successive delimiter characters (i.e., "//").
//          5. the URI does not have an empty query part (if the query marker is specified).
//          6. the URI contains at least one segment.
//          7. the URI does not end in a query marker.
//          8. the URI does not contain any segment that is "..".
//          8. the URI does not contain any segment that is ".".


class CURI
{
public:

    typedef struct tagSegmentInfo
    {
        DWORD   dwSegIndex;
        DWORD   cchSeg;
    } SEGMENTINFO, *PSEGMENTINFO;

    CURI() : 
        m_pszURI(NULL), 
        m_cchURI(0),
        m_dwCurrEnumIndex(0),
        m_dwSegmentCount(0),
        m_rgsiSegments(NULL),
        m_chURIDelimiter(DELIMITER_CHAR),
        m_fHasQuery(FALSE),
        m_fURIParsed(FALSE) 
    {}
    
    ~CURI() 
    { 
        delete[] m_pszURI; 
        delete[] m_rgsiSegments; 
    } 

    HRESULT SetDelimiterChar(TCHAR ch);

    HRESULT SetURI(LPCTSTR pszURI);

    HRESULT SetURIWithLen(LPCTSTR pszURI, DWORD cchURI);
    
    void ClearData();

    BOOL    HasQuery() const;

    DWORD   GetSegmentCount() const;

    HRESULT GetURI(LPCTSTR *ppszURI) const;
    
    DWORD GetURILength() const;

    HRESULT GetPath(LPCTSTR *ppszPath, DWORD *pcchPath) const;

    HRESULT GetPathCopy(DWORD dwSegIndex, BSTR *pbstrPath) const;

    HRESULT GetSegment(DWORD dwSegIndex, LPCTSTR *ppszSeg, DWORD *pcchSeg) const;

    HRESULT GetSegmentCopy(DWORD dwSegIndex, BSTR *pbstrSeg, DWORD *pcchSeg) const;

    HRESULT SplicePathCopy(DWORD dwSegSplice, BSTR *pbstrSeg1, BSTR *pbstrSeg2) const;

    HRESULT GetQuery(LPCTSTR *ppszQuery, DWORD *pcchQuery) const;

    HRESULT IsSegmentBoundary(DWORD dwCharIdx, BOOL *pfDelim) const;

private:
    // Declare, but do not implement a copy constructor so that anyone
    // attempting to use it will get a build-time error.
    CURI(const CURI&);
    
    HRESULT ParseURI();

    LPTSTR      m_pszURI;
    DWORD       m_cchURI;
    DWORD       m_dwCurrEnumIndex;
    DWORD       m_dwSegmentCount;
    SEGMENTINFO *m_rgsiSegments; // Array of indices into m_pszURI where each segment starts.
    TCHAR       m_chURIDelimiter;
    BOOL        m_fHasQuery;
    BOOL        m_fURIParsed;
};


