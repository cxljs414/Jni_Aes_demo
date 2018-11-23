package com.example.xingchang.jni_aes256_demo;

public class JniUtil {
    static {
        System.loadLibrary("JniUtil");
    }
    public native String encrypt(String plainText);
    public native String decrypt(String cipherText);

}
