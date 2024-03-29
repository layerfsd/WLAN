/*------------------------------------------------------------------------------ */
/* <copyright file="reg_dbschema.h" company="Atheros"> */
/*    Copyright (c) 2005 Atheros Corporation.  All rights reserved. */
/*  */
/* The software source and binaries included in this development package are */
/* licensed, not sold. You, or your company, received the package under one */
/* or more license agreements. The rights granted to you are specifically */
/* listed in these license agreement(s). All other rights remain with Atheros */
/* Communications, Inc., its subsidiaries, or the respective owner including */
/* those listed on the included copyright notices.  Distribution of any */
/* portion of this package must be in strict compliance with the license */
/* agreement(s) terms. */
/* </copyright> */
/*  */
/* <summary> */
/* 	Wifi driver for AR6002 */
/* </summary> */
/* */
/*------------------------------------------------------------------------------ */
/*============================================================================== */
/* Author(s): ="Atheros" */
/*============================================================================== */

#ifndef __REG_DBSCHEMA_H__
#define __REG_DBSCHEMA_H__

/*
 * This file describes the regulatory DB schema, which is common between the
 * 'generator' and 'parser'. The 'generator' runs on a host(typically a x86
 * Linux) and spits outs two binary files, which follow the DB file
 * format(described below). The resultant output "regulatoryData_AG.bin"
 * is binary file which has information regarding A and G regulatory
 * information, while the "regulatoryData_G.bin" consists of G-ONLY regulatory
 * information. This binary file is parsed in the target for extracting
 * regulatory information.
 * 
 * The DB values used to populate the regulatory DB are defined in
 * reg_dbvalues.h
 *
 */

/* Binary data file - Representation of Regulatory DB*/
#define REG_DATA_FILE_AG    "./regulatoryData_AG.bin"
#define REG_DATA_FILE_G     "./regulatoryData_G.bin"


/* Table tags used to encode different tables in the database */
enum data_tags_t{
    REG_DMN_PAIR_MAPPING_TAG = 0,
    REG_COUNTRY_CODE_TO_ENUM_RD_TAG,
    REG_DMN_FREQ_BAND_regDmn5GhzFreq_TAG,
    REG_DMN_FREQ_BAND_regDmn2Ghz11_BG_Freq_TAG,
    REG_DOMAIN_TAG,
    MAX_DB_TABLE_TAGS
    };



/*
 ****************************************************************************
 * Regulatory DB file format :
 * 4-bytes : "RGDB" (Magic Key)
 * 4-bytes : version (Default is 5379(my extn))
 * 4-bytes : length of file
 * dbType(4)
 * TAG(4)
 * Entries(1)entrySize(1)searchType(1)reserved[3]tableSize(2)"0xdeadbeef"(4)struct_data....
 * TAG(4)
 * Entries(1)entrySize(1)searchType(1)reserved[3]tableSize(2)"0xdeadbeef"(4)struct_data....
 * TAG(4)
 * Entries(1)entrySize(1)searchType(1)reserved[3]tableSize(2)"0xdeadbeef"(4)struct_data....
 * ...
 * ...
 ****************************************************************************
 *
 */

/*
 * Length of the file would be filled in when the file is created and
 * it would include the header size.
 */

#define REG_DB_KEY          "RGDB" /* Should be EXACTLY 4-bytes */
#define REG_DB_VER           5379  /* Between 0-9999 */

#define MAGIC_KEY_OFFSET    0
#define VERSION_OFFSET      4
#define FILE_SZ_OFFSET      8
#define DB_TYPE_OFFSET      12

#define MAGIC_KEY_SZ        4
#define VERSION_SZ          4
#define FILE_SZ_SZ          4
#define DB_TYPE_SZ          4
#define DB_TAG_SZ           4

#define REGDB_GET_MAGICKEY(x)     ((char *)x + MAGIC_KEY_OFFSET) 
#define REGDB_GET_VERSION(x)      ((char *)x + VERSION_OFFSET)
#define REGDB_GET_FILESIZE(x)     *((unsigned int *)((char *)x + FILE_SZ_OFFSET))
#define REGDB_GET_DBTYPE(x)       *((char *)x + DB_TYPE_OFFSET)

#define REGDB_SET_FILESIZE(x, sz_) *((unsigned int *)((char *)x + FILE_SZ_OFFSET)) = (sz_)
#define REGDB_IS_EOF(cur, begin)  ( REGDB_GET_FILESIZE(begin) > ((cur) - (begin)) )


/* A Table can be search based on key as a parameter or accessed directly
 * by giving its index in to the table.
 */
enum searchType {
    KEY_BASED_TABLE_SEARCH = 1,
    INDEX_BASED_TABLE_ACCESS
    };


/* Data is organised as different tables. There is a Master table, which
 * holds information regarding all the tables. It does not have any
 * knowledge about the attributes of the table it is holding
 * but has external view of the same(for ex, how many entries, record size,
 * how to search the table, total table size and reference to the data
 * instance of table).
 */
typedef PREPACK struct dbMasterTable_t {    /* Hold ptrs to Table data structures */
    A_UCHAR     numOfEntries;   /* NB: For AR6002_REV2, be aware of wreg_patch.c */
    A_CHAR      entrySize;      /* Entry size per table row */
    A_CHAR      searchType;     /* Index based access or key based */
    A_CHAR      reserved[3];    /* for alignment */
    A_UINT16    tableSize;      /* Size of this table */
    A_CHAR      *dataPtr;       /* Ptr to the actual Table */
} POSTPACK dbMasterTable;    /* Master table - table of tables */


/* used to get the number of rows in a table */
#define REGDB_NUM_OF_ROWS(a)    (sizeof (a) / sizeof (a[0]))

/* 
 * Used to set the RegDomain bitmask which chooses which frequency
 * band specs are used.
 */

#define BMLEN 2         /* Use 2 32-bit uint for channel bitmask */
#define BMZERO {0,0}    /* BMLEN zeros */

#define BM(_fa, _fb, _fc, _fd, _fe, _ff, _fg, _fh) \
      {((((_fa >= 0) && (_fa < 32)) ? (((A_UINT32) 1) << _fa) : 0) | \
    (((_fb >= 0) && (_fb < 32)) ? (((A_UINT32) 1) << _fb) : 0) | \
    (((_fc >= 0) && (_fc < 32)) ? (((A_UINT32) 1) << _fc) : 0) | \
    (((_fd >= 0) && (_fd < 32)) ? (((A_UINT32) 1) << _fd) : 0) | \
    (((_fe >= 0) && (_fe < 32)) ? (((A_UINT32) 1) << _fe) : 0) | \
    (((_ff >= 0) && (_ff < 32)) ? (((A_UINT32) 1) << _ff) : 0) | \
    (((_fg >= 0) && (_fg < 32)) ? (((A_UINT32) 1) << _fg) : 0) | \
    (((_fh >= 0) && (_fh < 32)) ? (((A_UINT32) 1) << _fh) : 0)), \
       ((((_fa > 31) && (_fa < 64)) ? (((A_UINT32) 1) << (_fa - 32)) : 0) | \
        (((_fb > 31) && (_fb < 64)) ? (((A_UINT32) 1) << (_fb - 32)) : 0) | \
        (((_fc > 31) && (_fc < 64)) ? (((A_UINT32) 1) << (_fc - 32)) : 0) | \
        (((_fd > 31) && (_fd < 64)) ? (((A_UINT32) 1) << (_fd - 32)) : 0) | \
        (((_fe > 31) && (_fe < 64)) ? (((A_UINT32) 1) << (_fe - 32)) : 0) | \
        (((_ff > 31) && (_ff < 64)) ? (((A_UINT32) 1) << (_ff - 32)) : 0) | \
        (((_fg > 31) && (_fg < 64)) ? (((A_UINT32) 1) << (_fg - 32)) : 0) | \
        (((_fh > 31) && (_fh < 64)) ? (((A_UINT32) 1) << (_fh - 32)) : 0))}


/*
 * THE following table is the mapping of regdomain pairs specified by
 * a regdomain value to the individual unitary reg domains
 */

typedef PREPACK struct reg_dmn_pair_mapping {
    A_UINT16 regDmnEnum;    /* 16 bit reg domain pair */
    A_UINT16 regDmn5GHz;    /* 5GHz reg domain */
    A_UINT16 regDmn2GHz;    /* 2GHz reg domain */
    A_UINT8  flags5GHz;     /* Requirements flags (AdHoc disallow etc) */
    A_UINT8  flags2GHz;     /* Requirements flags (AdHoc disallow etc) */
    A_UINT32 pscanMask;     /* Passive Scan flags which can override unitary domain passive scan
                                   flags.  This value is used as a mask on the unitary flags*/
} POSTPACK REG_DMN_PAIR_MAPPING;

#define REGDB_YES 1
#define REGDB_NO  0

typedef PREPACK struct {
    A_UINT16    countryCode;       
    A_UINT16    regDmnEnum;
    A_CHAR      isoName[3];
    A_CHAR      allow11g;
} POSTPACK COUNTRY_CODE_TO_ENUM_RD;

typedef PREPACK struct RegDmnFreqBand {
    A_UINT16    lowChannel;     /* Low channel center in MHz */
    A_UINT16    highChannel;    /* High Channel center in MHz */
    A_UINT8     power;          /* Max power (dBm) for channel range */
    A_UINT8     channelSep;     /* Channel separation within the band */
    A_UINT8     useDfs;         /* Use DFS in the RegDomain if corresponding bit is set */
    A_UINT8     mode;           /* Mode of operation */
    A_UINT32    usePassScan;    /* Use Passive Scan in the RegDomain if corresponding bit is set */
} POSTPACK REG_DMN_FREQ_BAND;



typedef PREPACK struct regDomain {
    A_UINT16    regDmnEnum;     /* value from EnumRd table */
    A_UINT8     rdCTL;
    A_UINT8     maxAntGain;
    A_UINT8     dfsMask;        /* DFS bitmask for 5Ghz tables */
    A_UINT8     flags;          /* Requirement flags (AdHoc disallow etc) */
    A_UINT16    reserved;       /* for alignment */
    A_UINT32    pscan;          /* Bitmask for passive scan */
    A_UINT32    chan11a[BMLEN]; /* 64 bit bitmask for channel/band selection */
    A_UINT32    chan11bg[BMLEN];/* 64 bit bitmask for channel/band selection */
} POSTPACK REG_DOMAIN;

#endif /* __REG_DBSCHEMA_H__ */
