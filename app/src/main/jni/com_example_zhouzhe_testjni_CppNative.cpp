#include "com_example_zhouzhe_testjni_CppNative.h"
#include <android/log.h>
#include <any>
#include <cassert>
#include <cinttypes>
#include <cstdint>
#include <functional>
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

jint jint_value;
jfloat jfloat_value;
jlong jlong_value;
jboolean jbool_value;
jbyte jbyte_value;
jchar jchar_value;
jdouble jdouble_value;
jshort jshort_value;

jbyteArray* jbyte_array;
jstring* jstring_value;
jclass* jobject_value;

class BaseCppTypeInterface {
public:
    virtual ~BaseCppTypeInterface() = default;
    virtual std::any GetValue() const = 0;
    template <typename T>
    static T base_type_cast(const std::shared_ptr<BaseCppTypeInterface>& ptr);
    template <typename T>
    static std::shared_ptr<BaseCppTypeInterface> make(JNIEnv* env, const T& jvalue);
};

class CppTypeInt final : public BaseCppTypeInterface {
public:
    CppTypeInt(const std::any& jvalue) { value = static_cast<int>(std::any_cast<jint>(jvalue)); }
    ~CppTypeInt() { LOGD("%s\n", __PRETTY_FUNCTION__); }
    std::any GetValue() const {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        return value;
    }
    int32_t value;
};

class CppTypeFloat final : public BaseCppTypeInterface {
public:
    CppTypeFloat(const std::any& jvalue) {
        value = static_cast<float>(std::any_cast<jfloat>(jvalue));
    }
    ~CppTypeFloat() { LOGD("%s\n", __PRETTY_FUNCTION__); }
    std::any GetValue() const {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        return std::any(value);
    }
    float value;
};

class CppTypeByte final : public BaseCppTypeInterface {
public:
    CppTypeByte(const std::any& jvalue) {
        value = static_cast<int8_t>(std::any_cast<jbyte>(jvalue));
    }
    ~CppTypeByte() { LOGD("%s\n", __PRETTY_FUNCTION__); }
    std::any GetValue() const {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        return std::any(value);
    }
    int8_t value;
};

class CppTypeBoolean final : public BaseCppTypeInterface {
public:
    CppTypeBoolean(const std::any& jvalue) {
        value = static_cast<uint8_t>(std::any_cast<jboolean>(jvalue));
    }
    ~CppTypeBoolean() { LOGD("%s\n", __PRETTY_FUNCTION__); }
    std::any GetValue() const {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        return std::any(value);
    }
    uint8_t value;
};

class CppTypeChar final : public BaseCppTypeInterface {
public:
    CppTypeChar(const std::any& jvalue) {
        value = static_cast<uint16_t>(std::any_cast<jchar>(jvalue));
    }
    ~CppTypeChar() { LOGD("%s\n", __PRETTY_FUNCTION__); }
    std::any GetValue() const {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        return std::any(value);
    }
    uint16_t value;
};

class CppTypeShort final : public BaseCppTypeInterface {
public:
    CppTypeShort(const std::any& jvalue) {
        value = static_cast<int16_t>(std::any_cast<jshort>(jvalue));
    }
    ~CppTypeShort() { LOGD("%s\n", __PRETTY_FUNCTION__); }
    std::any GetValue() const {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        return std::any(value);
    }
    int16_t value;
};

class CppTypeLong final : public BaseCppTypeInterface {
public:
    CppTypeLong(const std::any& jvalue) {
        value = static_cast<int64_t>(std::any_cast<jlong>(jvalue));
    }
    ~CppTypeLong() { LOGD("%s\n", __PRETTY_FUNCTION__); }
    std::any GetValue() const {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        return std::any(value);
    }
    int64_t value;
};

class CppTypeDouble final : public BaseCppTypeInterface {
public:
    CppTypeDouble(const std::any& jvalue) {
        value = static_cast<double>(std::any_cast<jdouble>(jvalue));
    }
    ~CppTypeDouble() { LOGD("%s\n", __PRETTY_FUNCTION__); }
    std::any GetValue() const {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        return std::any(value);
    }
    double value;
};

class ByteArray {
public:
    char* data;
    int length;
};

class CppTypeByteArray final : public BaseCppTypeInterface {
public:
    CppTypeByteArray(_JNIEnv* env_, const std::any& jvalue) : env(env_) {
        jbyteArray* java_jbyte_array_ = std::any_cast<jbyteArray*>(jvalue);
        jboolean is_copyed = false;
        value.data = (char*)(env->GetByteArrayElements(*java_jbyte_array_, &is_copyed));
        value.length = env->GetArrayLength(*java_jbyte_array_);
        java_jbyte_array = java_jbyte_array_;
    }
    ~CppTypeByteArray() {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        env->ReleaseByteArrayElements(*java_jbyte_array, (jbyte*)(value.data), JNI_ABORT);
    }
    std::any GetValue() const {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        return std::any(value);
    }
    jbyteArray* java_jbyte_array;
    ByteArray value;
    JNIEnv* env;
};

class CppTypeString final : public BaseCppTypeInterface {
public:
    CppTypeString(_JNIEnv* env_, const std::any& jvalue) : env(env_) {
        jstring* java_jstring_ = std::any_cast<jstring*>(jvalue);
        value = (char*)(env->GetStringUTFChars(*java_jstring_, NULL));
        java_jstring = java_jstring_;
    }
    ~CppTypeString() {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        env->ReleaseStringUTFChars(*java_jstring, (const char*)(value));
    }
    std::any GetValue() const {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        return std::any(value);
    }
    jstring* java_jstring;
    char* value;
    JNIEnv* env;
};

// class CppTypeClass final : public BaseCppTypeInterface {
// public:
//     CppTypeClass(_JNIEnv* env_, const std::any& jvalue) : env(env_) {

//     }
//     ~CppTypeClass() {
//         LOGD("%s\n", __PRETTY_FUNCTION__);
//         env->ReleaseStringUTFChars(*java_jstring, (const char*)(value));
//     }
//     std::any GetValue() const {
//         LOGD("%s\n", __PRETTY_FUNCTION__);
//         return std::any(value);
//     }
//     std::vector<std::shared_ptr<BaseCppTypeInterface>> dependent_list;
// };

template <typename T>
std::shared_ptr<BaseCppTypeInterface> BaseCppTypeInterface::make(JNIEnv* env, const T& jvalue) {
    if (typeid(jvalue).hash_code() == typeid(jint_value).hash_code()) {
        return std::make_shared<CppTypeInt>(std::any(jvalue));
    }
    if (typeid(jvalue).hash_code() == typeid(jfloat_value).hash_code()) {
        return std::make_shared<CppTypeFloat>(std::any(jvalue));
    }
    if (typeid(jvalue).hash_code() == typeid(jlong_value).hash_code()) {
        return std::make_shared<CppTypeLong>(std::any(jvalue));
    }
    if (typeid(jvalue).hash_code() == typeid(jbyte_value).hash_code()) {
        return std::make_shared<CppTypeByte>(std::any(jvalue));
    }
    if (typeid(jvalue).hash_code() == typeid(jbool_value).hash_code()) {
        return std::make_shared<CppTypeBoolean>(std::any(jvalue));
    }
    if (typeid(jvalue).hash_code() == typeid(jdouble_value).hash_code()) {
        return std::make_shared<CppTypeDouble>(std::any(jvalue));
    }
    if (typeid(jvalue).hash_code() == typeid(jchar_value).hash_code()) {
        return std::make_shared<CppTypeChar>(std::any(jvalue));
    }
    if (typeid(jvalue).hash_code() == typeid(jshort_value).hash_code()) {
        return std::make_shared<CppTypeShort>(std::any(jvalue));
    }
    if (typeid(jvalue).hash_code() == typeid(jbyte_array).hash_code()) {
        return std::make_shared<CppTypeByteArray>(env, std::any(jvalue));
    }
    if (typeid(jvalue).hash_code() == typeid(jstring_value).hash_code()) {
        return std::make_shared<CppTypeString>(env, std::any(jvalue));
    }
    if (typeid(jvalue).hash_code() == typeid(jobject_value).hash_code()) {
        return std::make_shared<CppTypeString>(env, std::any(jvalue));
    }
}

template <typename T>
T BaseCppTypeInterface::base_type_cast(const std::shared_ptr<BaseCppTypeInterface>& ptr) {
    return std::any_cast<T>(ptr->GetValue());
}

typedef enum FacePassImageType { NV21 = 0, GRAY, RGB } FacePassImageType;

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

//FacePassImage make_face_pass_image(const MultipleType& data, const MultipleType& width,
//                                   const MultipleType& height, const MultipleType& type) {
//    FacePassImage ret;
//    auto data_ = std::any_cast<CppCharArray>(data.any_value);
//    ret.data = (unsigned char*)(data_.data);
//    ret.width = std::any_cast<int>(width.any_value);
//    ret.height = std::any_cast<int>(height.any_value);
//    ret.type = static_cast<FacePassImageType>(std::any_cast<int>(type.any_value));
//    return ret;
//}

JNIEXPORT jint JNICALL Java_com_example_zhouzhe_testjni_CppNative_getInt(JNIEnv* env, jclass,
                                                                         jbyteArray jarray,
                                                                         jstring jstr, jclass complex_class) {
    LOGD("Java_com_example_zhouzhe_testjni_CppNative_getInt\n");
    jint test_jint_a = 1123;
    jfloat test_jfloat_a = 1.2f;
    jlong test_jlong_a = 1231283790127037121;
    jboolean test_jbool_a = 1;
    jbyte test_jbye_a = 127;
    auto a = BaseCppTypeInterface::make(env, test_jint_a);
    LOGD("a.value = %d\n", BaseCppTypeInterface::base_type_cast<int>(a));
    auto b = BaseCppTypeInterface::make(env, &jarray);
    LOGD("b.value.data = %s\n", BaseCppTypeInterface::base_type_cast<ByteArray>(b).data);
    auto c = BaseCppTypeInterface::make(env, &jstr);
    LOGD("c.value.data = %s\n", BaseCppTypeInterface::base_type_cast<char*>(c));
    return 11;
}