#include "com_example_zhouzhe_testjni_CppNative.h"
#include <android/log.h>
#include <any>
#include <cassert>
#include <cinttypes>
#include <cstdint>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <variant>

using namespace std::literals;

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "CPP-TEST", __VA_ARGS__)
#define DDEBUG LOGD("[%s:%d]", __FILE__, __LINE__)

#define REGISTER_TYPE(x)   \
    if (type_name == #x) { \
        return x##_value;  \
    }

class CharArray {
public:
    CharArray() {
        length = 0;
        data = new char[0];
    }
    CharArray(const char* data_, int length_) {
        length = length_;
        data = new char[length];
        memcpy(data, data_, length);
    }
    ~CharArray() { delete[] data; }
    char* data;
    int length;
};

class CppJbyteArray {
public:
    CppJbyteArray(jbyte* data_, int length_) {
        length = length_;
        data = data_;
    }
    jbyte* data;
    int length;
};

class MultipleType {
public:
    std::any any_value;
    size_t type_hash_code;
};

int int_value;
float float_value;
int64_t long_value;
uint8_t bool_value;
int8_t byte_value;
CharArray char_array;
char* cstring_value;

jint jint_value;
jfloat jfloat_value;
jlong jlong_value;
jboolean jbool_value;
jbyte jbyte_value;
jbyteArray jbyte_array;
jstring jstring_value;

typedef enum FacePassImageType {
    NV21 = 0,
    GRAY,
    RGB
} FacePassImageType;

typedef enum FacePassImageRotation {
    DEG0 = 0,
    DEG90 = 90,
    DEG180 = 180,
    DEG270 = 270
} FacePassImageRotation;

typedef struct FacePassImage {
    unsigned char* data;
    int width;
    int height;
    FacePassImageRotation rotation;
    FacePassImageType type;
    uint64_t track_id;
} FacePassImage;

FacePassImage make_face_pass_image(const MultipleType& data, const MultipleType& width,
                                   const MultipleType& height, const MultipleType& type) {
    FacePassImage ret;
    CharArray* data_ = std::any_cast<CharArray*>(data.any_value);
    ret.data = (unsigned char*)(data_->data);
    data_->data = new char[0];
    delete data_;
    ret.width = std::any_cast<int>(width.any_value);
    ret.height = std::any_cast<int>(height.any_value);
    ret.type = static_cast<FacePassImageType>(std::any_cast<int>(type.any_value));
    return ret;
}

template <typename T>
MultipleType cast_to_multi_value(JNIEnv* env, const T& j_value) {
    std::any any_buffer(j_value);
    MultipleType ret;
    if (typeid(j_value).hash_code() == typeid(jint_value).hash_code()) {
        ret.any_value = std::any(static_cast<int>(std::any_cast<jint>(any_buffer)));
        ret.type_hash_code = typeid(int_value).hash_code();
        return ret;
    }
    if (typeid(j_value).hash_code() == typeid(jfloat_value).hash_code()) {
        ret.any_value = std::any(static_cast<float>(std::any_cast<jfloat>(any_buffer)));
        ret.type_hash_code = typeid(float_value).hash_code();
        return ret;
    }
    if (typeid(j_value).hash_code() == typeid(jlong_value).hash_code()) {
        ret.any_value = std::any(static_cast<int64_t>(std::any_cast<jlong>(any_buffer)));
        ret.type_hash_code = typeid(long_value).hash_code();
        return ret;
    }
    if (typeid(j_value).hash_code() == typeid(jbyte_value).hash_code()) {
        ret.any_value = std::any(static_cast<int8_t>(std::any_cast<jbyte>(any_buffer)));
        ret.type_hash_code = typeid(byte_value).hash_code();
        return ret;
    }
    if (typeid(j_value).hash_code() == typeid(jbool_value).hash_code()) {
        ret.any_value = std::any(static_cast<uint8_t>(std::any_cast<jboolean>(any_buffer)));
        ret.type_hash_code = typeid(bool_value).hash_code();
        return ret;
    }
    if (typeid(j_value).hash_code() == typeid(jbyte_array).hash_code()) {
        const jbyteArray& jba = std::any_cast<const jbyteArray&>(any_buffer);
        jbyte* data_ = env->GetByteArrayElements(jba, NULL);
        int length_ = env->GetArrayLength(jba);
        CharArray* cap = new CharArray((const char*)data_, length_);
        ret.any_value = std::any(cap);
        env->ReleaseByteArrayElements(jba, data_, JNI_ABORT);
        ret.type_hash_code = typeid(char_array).hash_code();
        return ret;
    }
    if (typeid(j_value).hash_code() == typeid(jstring_value).hash_code()) {
        const jstring& jstr = std::any_cast<const jstring&>(any_buffer);
        jsize jstr_size = env->GetStringUTFLength(jstr);
        const char* jstr_data = env->GetStringUTFChars(jstr, NULL);
        char* cstring = new char[jstr_size + 1];
        memcpy(cstring, jstr_data, jstr_size + 1);
        env->ReleaseStringUTFChars(jstr, jstr_data);
        ret.any_value = std::any(cstring);
        ret.type_hash_code = typeid(cstring_value).hash_code();
        return ret;
    }
}

void print_multi_value(const MultipleType& multi_value) {
    if (multi_value.type_hash_code == typeid(int_value).hash_code()) {
        LOGD("any_value type = int\n");
        LOGD("any_value = %d\n", std::any_cast<int>(multi_value.any_value));
        return;
    }
    if (multi_value.type_hash_code == typeid(float_value).hash_code()) {
        LOGD("any_value type = float\n");
        LOGD("any_value = %f\n", std::any_cast<float>(multi_value.any_value));
        return;
    }
    if (multi_value.type_hash_code == typeid(long_value).hash_code()) {
        LOGD("any_value type = long\n");
        LOGD("any_value = %lld\n", std::any_cast<int64_t>(multi_value.any_value));
        return;
    }
    if (multi_value.type_hash_code == typeid(bool_value).hash_code()) {
        LOGD("any_value type = bool\n");
        LOGD("any_value = %u\n", std::any_cast<uint8_t>(multi_value.any_value));
        return;
    }
    if (multi_value.type_hash_code == typeid(byte_value).hash_code()) {
        LOGD("any_value type = byte\n");
        LOGD("any_value = %d\n", std::any_cast<int8_t>(multi_value.any_value));
        return;
    }
    if (multi_value.type_hash_code == typeid(char_array).hash_code()) {
        LOGD("any_value type = char_array\n");
        CharArray* tmp = std::any_cast<CharArray*>(multi_value.any_value);
        LOGD("any_value.length = %d\n", tmp->length);
        LOGD("any_value.data = %s\n", tmp->data);
        delete tmp;
        return;
    }
    if (multi_value.type_hash_code == typeid(cstring_value).hash_code()) {
        LOGD("any_value type = cstring\n");
        LOGD("any_value = %s\n", std::any_cast<char*>(multi_value.any_value));
        return;
    }
}

JNIEXPORT jint JNICALL Java_com_example_zhouzhe_testjni_CppNative_getInt(JNIEnv* env, jclass,
                                                                         jbyteArray jarray,
                                                                         jstring jstr) {
    jint test_jint_a = 1;
    jfloat test_jfloat_a = 1.2f;
    jlong test_jlong_a = 1231283790127037121;
    jboolean test_jbool_a = 1;
    jbyte test_jbye_a = 127;
//    MultipleType result_0 = cast_to_multi_value(env, test_jint_a);
//    MultipleType result_1 = cast_to_multi_value(env, test_jfloat_a);
//    MultipleType result_2 = cast_to_multi_value(env, jarray);
//    MultipleType result_3 = cast_to_multi_value(env, jstr);
//    MultipleType result_4 = cast_to_multi_value(env, test_jlong_a);
//    MultipleType result_5 = cast_to_multi_value(env, test_jbool_a);
//    MultipleType result_6 = cast_to_multi_value(env, test_jbye_a);
//    print_multi_value(result_0);
//    print_multi_value(result_1);
//    print_multi_value(result_2);
//    print_multi_value(result_3);
//    print_multi_value(result_4);
//    print_multi_value(result_5);
//    print_multi_value(result_6);
    jint width = 1;
    jint height = 2;
    jint type = 3;
    FacePassImage image =
        make_face_pass_image(cast_to_multi_value(env, jarray), cast_to_multi_value(env, width),
                             cast_to_multi_value(env, height), cast_to_multi_value(env, type));
    LOGD("image->data = %s\n", image.data);
    LOGD("image->data = %d\n", image.width);
    LOGD("image->data = %d\n", image.height);
    LOGD("image->data = %d\n", image.type);
    delete [] image.data;
    return 11;
}