package com.example.zhouzhe.testjni;

import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;

/**
 * Created by zhouzhe on 2018/6/27.
 */

public class ComplexClass {
    public int intValue;
    public float floatValue;
    public ComplexClass cc;
    public ComplexClass() {
        intValue = 1;
        floatValue = 1.1f;
        cc = null;
    }
    public String getDeclaredFieldsString() {
        Class cls = this.getClass();
        Field[] fields = cls.getDeclaredFields();
        JSONArray obj = new JSONArray();
        for(int i = 0; i < fields.length; i++) {
            JSONObject jsonBuffer = new JSONObject();
            String[] splited = fields[i].toString().split("\\s+");
            if(splited.length != 3) {
                continue;
            }
            try {
                jsonBuffer.put("FieldName",splited[2].substring(splited[2].lastIndexOf(".") + 1));

                jsonBuffer.put("ClassName",splited[2].substring(0, splited[2].lastIndexOf(".")));

                jsonBuffer.put("Type",splited[1]);
                jsonBuffer.put("Modifier",splited[0]);
            } catch (JSONException e) {
                e.printStackTrace();
            }
            obj.put(jsonBuffer);
        }
        return obj.toString();
    }
}
