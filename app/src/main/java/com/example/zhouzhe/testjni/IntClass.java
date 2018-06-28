package com.example.zhouzhe.testjni;

/**
 * Created by zhouzhe on 2018/6/27.
 */

public class IntClass {
    public int[] value;
    public IntClass() {
        this.value = new int[3];
        for(int i = 0; i < 3; i++) {
            this.value[i] = i;
        }
    }
}
