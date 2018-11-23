package com.example.xingchang.jni_aes256_demo

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        //jni加解密
        val jniUtil = JniUtil()
        val str= "abcdefg1234567"
        val encryptResult= jniUtil.encrypt(str)
        Log.i("jniUtil","JNI encryptResult=$encryptResult")
        val decryptResult= jniUtil.decrypt(encryptResult)
        Log.i("jniUtil","JNI decryptResult=$decryptResult")

        //java加解密
        val key_md5 ="b5e52765b81d101510dc0afdc52b1d64"
        val javaEncrypt= AesUtils.aesEncrypt(str,key_md5)
        Log.i("jniUtil","Java encryptResult=$javaEncrypt")
        val javaDecrypt = AesUtils.aesDecrypt(javaEncrypt,key_md5)
        Log.i("jniUtil","Java decryptResult=$javaDecrypt")
    }
}
