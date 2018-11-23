package com.example.xingchang.jni_aes256_demo

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val jniUtil = JniUtil()
        val str= "abcdefg1234567"
        val encryptResult= jniUtil.encrypt(str)
        Log.i("jniUtil","encryptResult=$encryptResult")
        val decryptResult= jniUtil.decrypt(encryptResult)
        Log.i("jniUtil","decryptResult=$decryptResult")
    }
}
