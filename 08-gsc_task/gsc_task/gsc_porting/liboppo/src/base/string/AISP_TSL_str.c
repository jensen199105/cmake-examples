#include "AISP_TSL_base.h"

S32 AISP_TSL_isspace(S32 iChr)
{
    return iChr == ' ' || iChr == '\t' || iChr == '\r' || iChr == '\n';
}

S32 AISP_TSL_strlen(CONST CHAR *pcStr)
{
    S32 iLen = 0;

    for (iLen = 0; *pcStr != '\0'; pcStr++)
    {
        iLen++;
    }

    return iLen;
}



S32 AISP_TSL_strncmp(CONST CHAR *cs, CONST CHAR *ct, S32 count)
{
    U8 c1 = 0;
    U8 c2 = 0;

    while (count)
    {
        c1 = *cs++;
        c2 = *ct++;

        if (c1 != c2)
        {
            return c1 < c2 ? -1 : 1;
        }

        if (!c1)
        {
            break;
        }

        count--;
    }

    return 0;
}

S8 *AISP_TSL_strstr(CONST CHAR *s1, CONST CHAR *s2)
{
    S32 l1 = 0;
    S32 l2 = 0;

    l2 = AISP_TSL_strlen(s2);

    if (!l2)
    {
        return (S8 *) s1;
    }

    l1 = AISP_TSL_strlen(s1);

    while (l1 >= l2)
    {
        l1--;

        if (!AISP_TSL_memcmp(s1, s2, l2))
        {
            return (S8 *) s1;
        }

        s1++;
    }

    return NULL;
}

U32 AISP_TSL_atoi(CONST CHAR *str)
{
    CONST S8 *cp = NULL;
    U32 data = 0;

    for (cp = str, data = 0; *cp != 0; ++cp)
    {
        if (*cp >= '0' && *cp <= '9')
        {
            data = data * 10 + *cp - '0';
        }
        else
        {
            break;
        }
    }

    return data;
}

S8 *AISP_TSL_strchr(CONST CHAR *s, S32 c)
{
    for (; *s != (S8) c; ++s)
    {
        if (*s == '\0')
        {
            return 0;
        }
    }

    return (S8 *) s;
}

STATIC U64 AISP_TSL_strspn(CONST CHAR *s, CONST CHAR *accept)
{
    CONST S8 *p = NULL;
    CONST S8 *a = NULL;
    U64 count = 0;

    for (p = s; *p != '\0'; ++p)
    {
        for (a = accept; *a != '\0'; ++a)
        {
            if (*p == *a)
            {
                break;
            }
        }

        if (*a == '\0')
        {
            return count;
        }

        ++count;
    }

    return count;
}

STATIC S8 *AISP_TSL_strpbrk(CONST CHAR *cs, CONST CHAR *ct)
{
    CONST S8 *sc1 = NULL;
    CONST S8 *sc2 = NULL;

    for (sc1 = cs; *sc1 != '\0'; ++sc1)
    {
        for (sc2 = ct; *sc2 != '\0'; ++sc2)
        {
            if (*sc1 == *sc2)
            {
                return (S8 *) sc1;
            }
        }
    }

    return NULL;
}



S8 *AISP_TSL_strtok_r(CHAR *s, CONST CHAR *delim, CHAR * *save_ptr)
{
    S8 *token = NULL;

    if (s == NULL)
    {
        s = *save_ptr;
    }

    /* Scan leading delimiters.  */
    s += AISP_TSL_strspn(s, delim);

    if (*s == '\0')
    {
        return NULL;
    }

    /* Find the end of the token.  */
    token = s;
    s = AISP_TSL_strpbrk(token, delim);

    if (s == NULL)
        /* This token finishes the string.  */
    {
        *save_ptr = AISP_TSL_strchr(token, '\0');
    }
    else
    {
        /* Terminate the token and make *SAVE_PTR point past it.  */
        *s = '\0';
        *save_ptr = s + 1;
    }

    return token;
}
