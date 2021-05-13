/************************************************************
  Copyright (C), AISpeechTech. Co., Ltd.

  FileName   : AISP_TSL_str.h
  Project    :
  Module     :
  Version    :
  Date       : 2019/08/28
  Author     : Jin.Chen
  Document   :
  Description:

  Function List :

  History :
  <author>      <time>            <version >        <desc>
  Jin.Chen      2019/07/15        1.00              Create

************************************************************/

#ifndef __AISP_TSL_STR_H__
#define __AISP_TSL_STR_H__

#ifndef __AISP_TSL_BASE_H__
#error "forbid to include this header directly"
#endif

S32 AISP_TSL_isspace(S32 iChr);
S32 AISP_TSL_strlen(CONST CHAR *pcStr);

S32 AISP_TSL_strncmp(CONST CHAR *cs, CONST S8 *ct, S32 count);
S8 *AISP_TSL_strstr(CONST CHAR *s1, CONST S8 *s2);

U32 AISP_TSL_atoi(CONST CHAR *str);
S8 *AISP_TSL_strchr(CONST CHAR *s, S32 c);
S8 *AISP_TSL_strtok_r(CHAR *s, CONST CHAR *delim, CHAR * *save_ptr);

#endif
