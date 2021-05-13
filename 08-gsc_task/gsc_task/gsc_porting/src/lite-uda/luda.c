/************************************************************
  Copyright (C), AISpeechTech. Co., Ltd.

  FileName   : lite-uda.c
  Project    :
  Module     :
  Version    :
  Date       : 2019/10/10
  Author     : Chao.Xu
  Document   :
  Description:

  Function List :

  History :
  <author>      <time>            <version >        <desc>
  Chao.Xu       2019/10/10         1.00              Create

************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#ifndef _WIN32
//#include <sys/time.h>
//#endif
#include "mtk_pdk.h"
#include "lite-uda/lite-uda.h"

//#define TEST_RESET
//#define USE_PARALLEL_FEED
#define US_SCALAR       (1000000)
#define MS_SCALAR       (1000)
#define FRM_INC_TIME    (16)        /* 16 ms */
#define WAV_HEADER_LEN  (44)
#define LUDA_IN_CHAN    (2)

extern char acResInfo[];

static void usage(char *tool);
static void bf_outputHook(void *usrdata, int idx, char *data, int len);
#ifdef UDA_HEADSET
static void bf_windHook(void *usrdata, int idx, char *data, int len);
#endif
//extern int AISP_TSL_pcmAddWavHeader(FILE *fp, int channels, int bits, int sample_rate, int len);

/************************************************************
  Function   : main()

  Description:
  Calls      :
  Called By  :
  Input      :
  Output     :
  Return     :
  Others     :

  History    :
    2018/11/15, Chao.Xu create

 ************************************************************/
int main(int argc, char *argv[])
{
    char   *audio = NULL;
    char   *outdir = NULL;
    char   *index = NULL;
    char   *bin = NULL;
    char   acBuf[1024] = {0};
    int    iLenPerSnd  = 0;
    int    iLen        = 0;
    int    iCnt        = 0;
    int    iIdx        = 0;
    int    iOutFlag    = 0;
    char *pcData       = NULL;
    FILE *pfIn         = NULL;
    FILE *pfGscOut[3] = {NULL, NULL, NULL};
    GSC_Api_S     *pstGscApi = NULL;
    GSC_CoreCfg_S *pstGscCfg = NULL;
    int iRet            =   -1;
    int iSize           =    0;
    char *pcSharedBuf   = NULL;
    char *pcUnsharedBuf = NULL;
//#ifndef _WIN32
//    struct timeval tvStart;
//    struct timeval tvEnd;
//    float  fElapseTime = 0.0F;
//#endif
#ifdef USE_PARALLEL_FEED
    short chan1[256] = {0};
    short chan2[256] = {0};
    short *psRaw     = NULL;
    int iLoop        = 0;
#endif

//    if (argc < 2)
//    {
//        usage(argv[0]);
//        return -1;
//    }

//    audio = argv[1];
//    outdir = argc > 2 ? argv[2] : ".";
    index = "all";
    bin = NULL;

    /**************************** Create wav ******************************/
    if (!strcmp(index, "all"))
    {
        iOutFlag = 0x7;
    }
    else
    {
        int shift = (int)strtol(index, NULL, 0) - 1;
        iOutFlag = 1 << shift;
    }

    /************************ open input wav file *************************/
    pfIn = fopen("oppo18115_010-NHXB-house-norm.wav", "rb");
    fseek(pfIn, WAV_HEADER_LEN, SEEK_SET);

    for (iIdx = 0; iIdx < 3; iIdx++)
    {
        if ((1 << iIdx) & iOutFlag)
        {
            memset(acBuf, 0, sizeof(acBuf));
            snprintf(acBuf, sizeof(acBuf), "%s/chan%d.pcm", "./", iIdx);
            pfGscOut[iIdx] = fopen(acBuf, "wb");

            if (NULL == pfGscOut[iIdx])
            {
                printf("Create %s fail.\n", acBuf);
                iRet = -1;
                goto out;
            }

            fseek(pfGscOut[iIdx], WAV_HEADER_LEN, SEEK_SET);
        }
        else
        {
            pfGscOut[iIdx] = NULL;
        }
    }

    /******************* Create udalite-gsc engine *********************/
    iSize = LUDA_CfgMemSizeGet();
    pstGscCfg = (GSC_CoreCfg_S *)calloc(1, iSize);
    if (NULL == pstGscCfg)
    {
        printf("GSC_CoreCfg_S struct alloc fail.\n");
        iRet = -1;
        goto out;
    }
    printf("isize=%d\n", iSize);
    iRet = LUDA_CfgInit(pstGscCfg, bin);
    if (iRet != 0)
    {
        printf("LUDA_CfgInit fail.\n");
        goto out;
    }

    /* SharedBuf/UnsharedBuf size */
    iSize = LUDA_SharedMemSizeGet(pstGscCfg);
    printf("isize=%d\n", iSize);
    pcSharedBuf = (char *)calloc(1, iSize);
    if (pcSharedBuf == NULL)
    {
        iRet = -1;
        goto out;
    }

    iSize = LUDA_UnsharedMemSizeGet(pstGscCfg);
    printf("isize=%d\n", iSize);
    pcUnsharedBuf = (char *)calloc(1, iSize);
    if (pcUnsharedBuf == NULL)
    {
        iRet = -1;
        goto out;
    }

    pstGscApi = (GSC_Api_S *)LUDA_New(pstGscCfg, pcSharedBuf, pcUnsharedBuf, (GSC_FUNCHOOK_T)bf_outputHook, pfGscOut);
    if (NULL == pstGscApi)
    {
        printf("LUDA_New return NULL\n");
        iRet = -1;
        goto out;
    }

#ifdef UDA_HEADSET
    LUDA_WindCbRegister(pstGscApi, bf_windHook, NULL);
#endif
    iLenPerSnd = LUDA_LenPerSnd(pstGscApi);
    pcData     = calloc(1, iLenPerSnd);

//#ifndef _WIN32
//    gettimeofday(&tvStart, NULL);
//#endif
    profile_on();
    int count = 0;
    while (count < 100)
    //while (!feof(pfIn))
    {
        iCnt++;

        iLen = (int)fread(pcData, 1, iLenPerSnd, pfIn);
        if (iLen < iLenPerSnd)
        {
            break;
        }

#ifdef USE_PARALLEL_FEED
        /* input chan is 4, [mic0 mic1 ref1 ref2] */
        psRaw = (short *)pcData;

        for (loop = 0; loop < 256; loop++, psRaw += LUDA_IN_CHAN)
        {
            chan1[loop] = *psRaw;
            chan2[loop] = *(psRaw + 1);
        }

        LUDA_FeedParallel(pstGscApi, (char *)chan1, (char *)chan2, iLen / LUDA_IN_CHAN);
#else
        count++;
        if (iLen != LUDA_Feed(pstGscApi, pcData, iLen))
        {
            printf("failed to feed\n");
            iRet = -1;
            goto out;
        }

#endif
    }

//#ifndef _WIN32
//    gettimeofday(&tvEnd, NULL);
//#endif

#ifdef TEST_RESET
    LUDA_Reset(pstGscApi);
    fseek(pfIn, WAV_HEADER_LEN, SEEK_SET);

    while (!feof(pfIn))
    {
        iCnt++;
        iLen = fread(pcData, 1, iLenPerSnd, pfIn);
#ifdef USE_PARALLEL_FEED
        /* input chan is 4, [mic0 mic1 ref1 ref2] */
        psRaw = (short *)pcData;

        for (loop = 0; loop < 256; loop++, psRaw += LUDA_IN_CHAN)
        {
            chan1[loop] = *psRaw;
            chan2[loop] = *(psRaw + 1);
        }

        LUDA_FeedParallel(pstGscApi, (char *)chan1, (char *)chan2, iLen / LUDA_IN_CHAN);
#else
        LUDA_Feed(pstGscApi, pcData, iLen);
#endif
    }

#endif

//#ifndef _WIN32
//    fElapseTime = (float)(tvEnd.tv_sec - tvStart.tv_sec) + (float)(tvEnd.tv_usec - tvStart.tv_usec) / US_SCALAR;
//    printf("Elapse Time %f\n", fElapseTime);
//    printf("RRT: %f\n", fElapseTime * MS_SCALAR / (float)(iCnt * FRM_INC_TIME));
//#endif

    /* add wav header */
    for (iIdx = 0; iIdx < 3; iIdx++)
    {
        if (pfGscOut[iIdx])
        {
            iLen = (int)ftell(pfGscOut[iIdx]);
//
//            if (0 != AISP_TSL_pcmAddWavHeader(pfGscOut[iIdx], 1, 16, 16000, iLen))
//            {
//                goto out;
//            }

            fclose(pfGscOut[iIdx]);
            pfGscOut[iIdx] = NULL;
        }
    }

    /* release rdma engine */
    iRet = LUDA_Delete(pstGscApi);
    if (0 != iRet)
    {
        printf("failed to delete UDA\n");
        goto out;
    }

    iRet = 0;

out:

    /* release resources */
    if (NULL != pcData)
    {
        free(pcData);
    }

    if (NULL != pfIn)
    {
        fclose(pfIn);
    }

    for (iIdx = 0; iIdx < 3; iIdx++)
    {
        if (NULL != pfGscOut[iIdx])
        {
            fclose(pfGscOut[iIdx]);
        }
    }

    if (NULL != pcUnsharedBuf)
    {
        free(pcUnsharedBuf);
    }

    if (NULL != pcSharedBuf)
    {
        free(pcSharedBuf);
    }

    if (NULL != pstGscCfg)
    {
        free(pstGscCfg);
    }
    vPDKTaskEnd(0);
    return iRet;
}

/************************************************************
 *   Function   : usage()
 *   Description:
 *   Calls      :
 *   Called By  :
 *   Input      :
 *   Output     :
 *   Return     :
 *   Others     :
 *   History    :
 *     2019/10/10, Chao.Xu create
 ***********************************************************/
static void usage(char *tool)
{
    printf("*****************************************\n"
           "   Copyright (C) AIspeech Co., Ltd.      \n"
           "   Version : %s\n"
           "   WB Info : %s\n"
           "   Usage   :\n"
           "   %s <input> [<output dir> [<index> [bin]]]      \n"
           "*****************************************\n", LUDA_Version(), acResInfo, "gsc");
}

/************************************************************
 *   Function   : bf_outputHook()
 *   Description: store gsc output data
 *   Calls      :
 *   Called By  :
 *   Input      :
 *   Output     :
 *   Return     :
 *   Others     :
 *   History    :
 *     2018/11/15, Youhai.Jiang create
 ***********************************************************/
static void bf_outputHook(void *usrdata, int idx, char *data, int len)
{
    FILE **fp = (FILE **)usrdata;

    printf("output data\n");
    if (fp[idx])
    {
        if (len != (int)fwrite(data, 1, len, fp[idx]))
        {
            printf("failed to write data to file\n");
        }
    }
}

#ifdef UDA_HEADSET
/************************************************************
 *   Function   : bf_windHook()
 *   Description: Post the wind noise status.
 *   Calls      :
 *   Called By  :
 *   Input      :
 *   Output     :
 *   Return     :
 *   Others     :
 *   History    :
 *     2018/11/15, Youhai.Jiang create
 ***********************************************************/
static void bf_windHook(void *usrdata, int idx, char *data, int len)
{
    printf("Main:bf_windHook: The current status is %d\n", idx);
}
#endif

