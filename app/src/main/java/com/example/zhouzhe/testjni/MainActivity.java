package com.example.zhouzhe.testjni;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import java.lang.reflect.*;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ComplexClass cc = new ComplexClass();
        Class cls = cc.getClass();
        Field[] fields = cls.getDeclaredFields();
        for(int i = 0; i < fields.length; i++) {
            Log.d("CPP-TEST", "Field = " + fields[i].toString());
        }
        Log.d("CPP-TEST", ""+CppNative.getInt("zhouzhe".getBytes(), "jstr:zhouzhe", cc));
    }
}
