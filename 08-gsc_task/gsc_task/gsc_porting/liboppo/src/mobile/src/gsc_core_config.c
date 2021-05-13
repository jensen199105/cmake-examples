#include "AISP_TSL_base.h"
#include "gsc_core_config.h"
#ifndef GSC_FIXED
#include "rbin.h"
#endif

/* Local macro declaration */
#define USE_CONFIG_DUMP

#ifndef GSC_FIXED
/* STATIC function declaration */
STATIC S32 gscCfgUpdateLocal(GSC_CoreCfg_S *pstGscCfg, rbin2_local_cfg_t *main);
#endif

STATIC VOID gscCfgDump(GSC_CoreCfg_S *pstGscCfg);

/************************************************************
 *   Function   : gscDefaultCfgInit()
 *   Description:
 *   Calls      :
 *   Called By  :
 *   Input      :
 *   Output     :
 *   Return     :
 *   Others     :
 *   History    :
 *       2019/04/28, chao.xu create
 *
 *************************************************************/
S32 gscDefaultCfgInit(GSC_CoreCfg_S *pstGscCfg)
{
    GSC_CoreCfg_S *pstCfg = pstGscCfg;

    if (pstCfg == NULL)
    {
        return -1;
    }

    /* Const configs */
#ifdef GSC_FIXED
    pstCfg->iMu           = 1342177;/* Fixed: Q24 */
    pstCfg->iTapsPSD      =    24576;/* Fixed: Q15 */
    pstCfg->iTapsDalta    = 4;       /* Fixed: Q0, Attention: not Q24! */
    pstCfg->iPriorFact    = 1;       /* Fixed: Q0, Attention: 1 by default! */
    pstCfg->iLogGLRFact   = -58468777; /* Fixed: Q24 */
    pstCfg->iGLRexp       = 16262937;/* Fixed: Q24 */
    pstCfg->iAlphaPH1mean = 15099494;/* Fixed: Q24 */
    pstCfg->iAlphaPSD     = 13421773;/* Fixed: Q24 */
    pstCfg->iAmth         =  2516582;/* Fixed: Q24 */
#else
    pstCfg->iMu           = 0.08;
    pstCfg->iTapsPSD      = 0.75;
    pstCfg->iTapsDalta    = 4.0;
    pstCfg->iPriorFact    = 1.0;
    pstCfg->iLogGLRFact   = -3.48501071318057;
    pstCfg->iGLRexp       = 0.969346569968284;
    pstCfg->iAlphaPH1mean = 0.9;
    pstCfg->iAlphaPSD     = 0.8;
    pstCfg->iAmth         = 0.15;
#endif
    pstCfg->iGsc     = 3;
    pstCfg->iAdaBM   = 0;
#ifdef GSC_ENABLE_HALF_VAD
    pstCfg->iTaps    = 2;
#else
    pstCfg->iTaps    = 4;
#endif
    pstCfg->iVAD     = 0;
    pstCfg->iSpeCnt  = 0;
    pstCfg->iSilCnt  = 0;
    pstCfg->iSil2Spe = 1;
    pstCfg->iSpe2Sil = 20;
    /* Variables */
    pstCfg->iIdxFstFrm_nonzero = 1;
    pstCfg->iFlgFstFrm_nonzero = 0;
    pstCfg->iIdxExeFrm  = 0;
    pstCfg->iGscCurxIdx = 0;
    pstCfg->bInited   = FALSE;
    AISP_TSL_printf("%s", "Default config in function \'gscDefaultCfgInit\':\n");
    gscCfgDump(pstCfg);
    return 0;
}


#ifndef GSC_FIXED
/************************************************************
 *   Function   : gscCfgUpdate()
 *   Description:
 *   Calls      :
 *   Called By  :
 *   Input      :
 *   Output     :
 *   Return     :
 *   Others     :
 *   History    :
 *       2019/04/28, chao.xu create
 *
 *************************************************************/
S32 gscCfgUpdate(GSC_CoreCfg_S *pstGscCfg, CONST CHAR *pcBinFn)
{
    S32 iRet;
    rbin2_st_t       *rbin;
    rbin2_item_t     *item;
    rbin2_cfg_file_t *cfile;
    CONST CHAR *cfg_fn = "./udalite.cfg.r";
    rbin = rbin2_new();
    iRet = rbin2_read(rbin, pcBinFn);

    if (iRet != 0)
    {
        rbin2_debug("read failed\n");
        goto end1;
    }

    item = rbin2_get2(rbin, cfg_fn, AISP_TSL_strlen(cfg_fn));

    if (!item)
    {
        rbin2_debug("%s not found %s\n", cfg_fn, pcBinFn);
        iRet = -1;
        goto end1;
    }

    cfile = rbin2_cfg_file_new();
    rbin2_cfg_file_add_var_ks(cfile, "pwd", ".", 1);
    iRet = rbin2_cfg_file_feed(cfile, item->data->data, item->data->len);

    if (iRet != 0)
    {
        goto end2;
    }

    iRet = gscCfgUpdateLocal(pstGscCfg, cfile->main);
    AISP_TSL_printf("%s", "Updated config in function \'gscCfgUpdate\':\n");
    gscCfgDump(pstGscCfg);
end2:
    rbin2_cfg_file_delete(cfile);
end1:
    rbin2_delete(rbin);
    return iRet;
}

/************************************************************
 *   Function   : gscCfgUpdateLocal()
 *   Description:
 *   Calls      :
 *   Called By  :
 *   Input      :
 *   Output     :
 *   Return     :
 *   Others     :
 *   History    :
 *       2019/04/28, chao.xu create
 *
 *************************************************************/
STATIC S32 gscCfgUpdateLocal(GSC_CoreCfg_S *pstGscCfg, rbin2_local_cfg_t *main)
{
    rbin2_string_t    *v  = NULL;
    rbin2_local_cfg_t *lc = NULL;
    /* confParm */
    lc = rbin2_local_cfg_find_lc_s(main, "confParm");

    if (lc != NULL)
    {
        rbin2_local_cfg_update_cfg_f(lc, pstGscCfg, iMu,           v);
        rbin2_local_cfg_update_cfg_f(lc, pstGscCfg, iTapsPSD,      v);
        rbin2_local_cfg_update_cfg_f(lc, pstGscCfg, iTapsDalta,    v);
        rbin2_local_cfg_update_cfg_f(lc, pstGscCfg, iPriorFact,    v);
        rbin2_local_cfg_update_cfg_f(lc, pstGscCfg, iLogGLRFact,   v);
        rbin2_local_cfg_update_cfg_f(lc, pstGscCfg, iGLRexp,       v);
        rbin2_local_cfg_update_cfg_f(lc, pstGscCfg, iAlphaPH1mean, v);
        rbin2_local_cfg_update_cfg_f(lc, pstGscCfg, iAlphaPSD,     v);
        rbin2_local_cfg_update_cfg_f(lc, pstGscCfg, iAmth,         v);
        rbin2_local_cfg_update_cfg_i(lc, pstGscCfg, iGsc,     v);
        rbin2_local_cfg_update_cfg_i(lc, pstGscCfg, iAdaBM,   v);
        rbin2_local_cfg_update_cfg_i(lc, pstGscCfg, iTaps,    v);
        rbin2_local_cfg_update_cfg_i(lc, pstGscCfg, iVAD,     v);
        rbin2_local_cfg_update_cfg_i(lc, pstGscCfg, iSpeCnt,  v);
        rbin2_local_cfg_update_cfg_i(lc, pstGscCfg, iSilCnt,  v);
        rbin2_local_cfg_update_cfg_i(lc, pstGscCfg, iSil2Spe, v);
        rbin2_local_cfg_update_cfg_i(lc, pstGscCfg, iSpe2Sil, v);
    }

    return 0;
}
#endif

/************************************************************
 *   Function   : gscCfgDump()
 *   Description: Dump the total config for debug.
 *   Calls      :
 *   Called By  :
 *   Input      :
 *   Output     :
 *   Return     :
 *   Others     :
 *   History    :
 *       2019/04/28, chao.xu create
 *
 *************************************************************/
STATIC VOID gscCfgDump(GSC_CoreCfg_S *pstGscCfg)
{
#ifdef USE_CONFIG_DUMP
    AISP_TSL_printf("iMu           = %f\n", (float)pstGscCfg->iMu);
    AISP_TSL_printf("iTapsPSD      = %f\n", (float)pstGscCfg->iTapsPSD);
    AISP_TSL_printf("iTapsDalta    = %f\n", (float)pstGscCfg->iTapsDalta);
    AISP_TSL_printf("iPriorFact    = %f\n", (float)pstGscCfg->iPriorFact);
    AISP_TSL_printf("iLogGLRFact   = %.12f\n", (float)pstGscCfg->iLogGLRFact);
    AISP_TSL_printf("iGLRexp       = %.12f\n", (float)pstGscCfg->iGLRexp);
    AISP_TSL_printf("iAlphaPH1mean = %f\n", (float)pstGscCfg->iAlphaPH1mean);
    AISP_TSL_printf("iAlphaPSD     = %f\n", (float)pstGscCfg->iAlphaPSD);
    AISP_TSL_printf("iAmth         = %f\n", (float)pstGscCfg->iAmth);
    AISP_TSL_printf("iGsc     = %d\n", pstGscCfg->iGsc);
    AISP_TSL_printf("iAdaBM   = %d\n", pstGscCfg->iAdaBM);
    AISP_TSL_printf("iTaps    = %d\n", pstGscCfg->iTaps);
    AISP_TSL_printf("iVAD     = %d\n", pstGscCfg->iVAD);
    AISP_TSL_printf("iSpeCnt  = %d\n", pstGscCfg->iSpeCnt);
    AISP_TSL_printf("iSilCnt  = %d\n", pstGscCfg->iSilCnt);
    AISP_TSL_printf("iSil2Spe = %d\n", pstGscCfg->iSil2Spe);
    AISP_TSL_printf("iSpe2Sil = %d\n", pstGscCfg->iSpe2Sil);
    AISP_TSL_printf("iIdxFstFrm_nonzero = %d\n", pstGscCfg->iIdxFstFrm_nonzero);
    AISP_TSL_printf("iFlgFstFrm_nonzero = %d\n", pstGscCfg->iFlgFstFrm_nonzero);
    AISP_TSL_printf("iIdxExeFrm  = %d\n", pstGscCfg->iIdxExeFrm);
    AISP_TSL_printf("iGscCurxIdx = %d\n", pstGscCfg->iGscCurxIdx);
    AISP_TSL_printf("iInitFlag   = %d\n", pstGscCfg->bInited);
#endif
}

