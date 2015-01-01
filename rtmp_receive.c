/*
 * @file rtmp_receive.c
 * @author Akagi201
 * @date 2015/01/01
 *
 * receive rtmp live stream and save it as local flv file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "librtmp/rtmp_sys.h"
#include "librtmp/log.h"

int main(int argc, char *argv[]) {

    double duration = -1;
    int nread = 0;
    //is live stream ?
    bool b_live_stream = true;

    int bufsize = 1024 * 1024 * 10;
    char *buf = (char *) malloc(bufsize);
    memset(buf, 0, bufsize);
    long countbufsize = 0;

    FILE *fp = fopen("receive.flv", "wb");
    if (NULL == fp) {
        RTMP_LogPrintf("Open File Error.\n");
        return -1;
    }

    /* set log level */
    //RTMP_LogLevel loglvl=RTMP_LOGDEBUG;
    //RTMP_LogSetLevel(loglvl);

    RTMP *rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);
    //set connection timeout,default 30s
    rtmp->Link.timeout = 10;
    // HKS's live URL
    if (!RTMP_SetupURL(rtmp, "rtmp://live.hkstv.hk.lxdns.com/live/hks")) {
        RTMP_Log(RTMP_LOGERROR, "SetupURL Err\n");
        RTMP_Free(rtmp);
        return -1;
    }
    if (b_live_stream) {
        rtmp->Link.lFlags |= RTMP_LF_LIVE;
    }

    //1hour
    RTMP_SetBufferMS(rtmp, 3600 * 1000);

    if (!RTMP_Connect(rtmp, NULL)) {
        RTMP_Log(RTMP_LOGERROR, "Connect Err\n");
        RTMP_Free(rtmp);
        return -1;
    }

    if (!RTMP_ConnectStream(rtmp, 0)) {
        RTMP_Log(RTMP_LOGERROR, "ConnectStream Err\n");
        RTMP_Free(rtmp);
        RTMP_Close(rtmp);
        return -1;
    }

    while ((nread = RTMP_Read(rtmp, buf, bufsize)) != 0) {
        fwrite(buf, 1, (size_t)nread, fp);

        countbufsize += nread;
        RTMP_LogPrintf("Receive: %5dByte, Total: %5.2fkB\n", nread, countbufsize * 1.0 / 1024);
    }

    if (fp != NULL) {
        fclose(fp);
        fp = NULL;
    }

    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }

    if (rtmp != NULL) {
        RTMP_Close(rtmp);
        RTMP_Free(rtmp);
        rtmp = NULL;
    }
    return 0;
}