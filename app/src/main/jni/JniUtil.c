//
// Created by xing.chang on 2018/10/24.
//

#include "com_example_xingchang_jni_aes256_demo_JniUtil.h"
#include "base64.h"
#include <stdlib.h>
#include <aes256.h>
#include<android/log.h>
const char *DES_KEY = "b5e52765b81d101510dc0afdc52b1d64";
const char *VIPARA = "1982051319810208";

#define TAG "myDemo-jni" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

jstring charToJstring(JNIEnv* envPtr, char *src) {
    JNIEnv env = *envPtr;

    jsize len = strlen(src);
    jclass clsstring = env->FindClass(envPtr, "java/lang/String");
    jstring strencode = env->NewStringUTF(envPtr, "UTF-8");
    jmethodID mid = env->GetMethodID(envPtr, clsstring, "<init>", "([BLjava/lang/String;)V");
    jbyteArray barr = env->NewByteArray(envPtr, len);
    env->SetByteArrayRegion(envPtr, barr, 0, len, (jbyte*) src);

    return (jstring) env->NewObject(envPtr, clsstring, mid, barr, strencode);
}

jstring getImportInfo(JNIEnv* envPtr, jstring mingwen) {
    JNIEnv env = *envPtr;

    //b5e52765b81d101510dc0afdc52b1d64 秘钥
     unsigned char key[32] = {0x62,0x35,0x65,0x35,0x32,0x37,0x36,0x35,
                                  0x62,0x38,0x31,0x64,0x31,0x30,0x31,0x35,
                                  0x31,0x30,0x64,0x63,0x30,0x61,0x66,0x64,
                                  0x63,0x35,0x32,0x62,0x31,0x64,0x36,0x34};

    //****************************************开始加密******************************************************
    //1.初始化数据
    //初始化向量
    uint8_t iv[16] = { 0x31,0x39,0x38,0x32,0x30,0x35,0x31,0x33,
                       0x31,0x39,0x38,0x31,0x30,0x32,0x30,0x38 };

    //初始化加密参数
    aes256_context ctx;
    aes256_init(&ctx, key);

    //2.将jstring转为char
    const char *mwChar = env->GetStringUTFChars(envPtr, mingwen, JNI_FALSE);

    //3.分组填充加密
    int i;
    int mwSize = strlen(mwChar);
    int remainder = mwSize % 16;
    jstring entryptString;
    if (mwSize < 16) {	//小于16字节，填充16字节，后面填充几个几 比方说10个字节 就要补齐6个6 11个字节就补齐5个5
        uint8_t input[16];
        for (i = 0; i < 16; i++) {
            if (i < mwSize) {
                input[i] = (unsigned char) mwChar[i];
            } else {
                input[i] = (unsigned char) (16 - mwSize);
            }
        }
        //加密
        uint8_t output[16];
        aes256_encrypt_cbc(&ctx, input, iv, output);
        //base64加密后然后jstring格式输出
        char *enc = base64_encode((const char *) output, sizeof(output));
        entryptString = charToJstring(envPtr, enc);

        free(enc);
    } else {	//如果是16的倍数，填充16字节，后面填充0x10
        int group = mwSize / 16;
        int size = 16 * (group + 1);
        uint8_t input[size];
        for (i = 0; i < size; i++) {
            if (i < mwSize) {
                input[i] = (unsigned char) mwChar[i];
            } else {
                if (remainder == 0) {
                    input[i] = 0x10;
                } else {	//如果不足16位 少多少位就补几个几  如：少4为就补4个4 以此类推
                    int dif = size - mwSize;
                    input[i] = (unsigned char) dif;
                }
            }
        }
        //加密
        uint8_t output[size];
        aes256_encrypt_cbc(&ctx, input, iv, output);
        //base64加密后然后jstring格式输出
        LOGD("encrypt output size=%d",size);
        char *enc = base64_encode((const char *) output, sizeof(output));
        LOGD("encrypt enc=%s",enc);
        entryptString = charToJstring(envPtr, enc);

        free(enc);
    }

    //释放mwChar
    env->ReleaseStringUTFChars(envPtr, mingwen, mwChar);

    return entryptString;
}

JNIEXPORT jstring JNICALL Java_com_example_xingchang_jni_1aes256_1demo_JniUtil_encrypt
  (JNIEnv *env, jobject instance, jstring jstr){
    if (jstr == NULL) {
        return NULL;
    }
    return getImportInfo(env,jstr);
  }

jstring doecrypt(JNIEnv* env, jstring miwen) {
    jstring result;

    //b5e52765b81d101510dc0afdc52b1d64 秘钥
    unsigned char key[32] = {0x62,0x35,0x65,0x35,0x32,0x37,0x36,0x35,
                             0x62,0x38,0x31,0x64,0x31,0x30,0x31,0x35,
                             0x31,0x30,0x64,0x63,0x30,0x61,0x66,0x64,
                             0x63,0x35,0x32,0x62,0x31,0x64,0x36,0x34};

    //1.初始化数据
    //初始化向量
    uint8_t iv[16] = { 0x31,0x39,0x38,0x32,0x30,0x35,0x31,0x33,
                       0x31,0x39,0x38,0x31,0x30,0x32,0x30,0x38 };
    aes256_context ctx;
    aes256_init(&ctx, key);

    //2.将jstring转为char
    const char *mwChar = (*env)->GetStringUTFChars(env, miwen, JNI_FALSE);
    char *enc = base64_decode(mwChar, strlen(mwChar));
    uint8_t output[4096];
    aes256_decrypt_cbc(&ctx, (unsigned char *) enc, iv, output);
    int size = strlen((const char *) output);
    LOGD("output size=%d",size);
    int i;
    for(i=0;i<size;i++){
        LOGD("cha %d = %c",i,output[i]);
        if(output[i]>=1&&output[i]<=16){
            output[i] = 0;
        }
    }
    result = charToJstring(env, (char *) output);
    LOGD("result=%s",(char *) output);
    free(enc);
    //释放mwChar
    (*env)->ReleaseStringUTFChars(env, miwen, mwChar);
    aes256_done(&ctx);
    return result;
}

JNIEXPORT jstring JNICALL Java_com_example_xingchang_jni_1aes256_1demo_JniUtil_decrypt
  (JNIEnv *env, jobject instance, jstring jstr){
    if (jstr == NULL) {
        return NULL;
    }
    return doecrypt(env,jstr);
  }

