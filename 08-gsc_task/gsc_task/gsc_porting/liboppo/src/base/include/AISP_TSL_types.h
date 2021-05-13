/************************************************************
  Copyright (C), AISpeechTech. Co., Ltd.

  FileName   : AISP_TSL_types.h
  Project    :
  Module     :
  Version    :
  Date       : 2019/04/28
  Author     : Chao.Xu
  Document   :
  Description:

  Function List :

  History :
  <author>      <time>            <version >        <desc>
  Chao.Xu       2019/04/28        1.00              Create

************************************************************/
#ifndef __AISP_TSL_TYPES_H__
#define __AISP_TSL_TYPES_H__

#ifndef __AISP_TSL_BASE_H__
#error "forbid to include this header directly"
#endif

#if !defined(__PLATFORM_LE__) && !defined(__PLATFORM_BE__)
#error "Endian order is unknown"
#endif

#ifndef __BASE_TYPE__
#define __BASE_TYPE__
typedef char                S8;
typedef short               S16;
typedef int                 S32;
typedef long long           S64;
typedef unsigned char       U8;
typedef unsigned short      U16;
typedef unsigned int        U32;
typedef unsigned long long  U64;
typedef char                BOOL;
typedef char                CHAR;
typedef unsigned char       BYTE;
/*
 * Windows use #ifdef to test whether VOID defined,
 * If it is defined, it also means CHAR, SHORT and LONG
 * are all defined by typedef.
 */
#ifdef _WIN32
#ifndef VOID
#define VOID                void;
typedef char                CHAR;
typedef short               SHORT;
typedef long                LONG;
#endif
#else
typedef void                VOID;
#endif
#endif

#ifndef STATIC
#define STATIC  static
#endif

#ifndef INLINE
#define INLINE  inline
#endif

#ifndef CONST
#define CONST   const
#endif

#ifndef NULL
#define NULL    ((VOID*)0)
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#if !defined(_MSC_VER)
#define ATTR_ALING(n)         __attribute__((aligned(n)))
#else
#define ATTR_ALING(n)
#endif

#if !defined(_MSC_VER) && defined(HIDE_SYMBOL)
#define ATTR_API      __attribute__((visibility("default")))
#else
#define ATTR_API
#endif

#ifndef ARRSIZE
#define ARRSIZE(a)            ((sizeof(a))/(sizeof((a)[0])))
#endif

#endif
