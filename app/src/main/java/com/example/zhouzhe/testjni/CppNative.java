package com.example.zhouzhe.testjni;

/**
 * Created by zhouzhe on 2018/6/25.
 */

public class CppNative {
    static {
        System.loadLibrary("testjni");
    }
    public native static int getInt(byte[] jarray, String jstr);
}
