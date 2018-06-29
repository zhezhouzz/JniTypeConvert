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
#include "json.hpp"

using json = nlohmann::json;

using namespace std::literals;

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "CPP-TEST", __VA_ARGS__)
#define DDEBUG LOGD("[%s:%d]", __FILE__, __LINE__)

#define REGISTER_TYPE(x)   \
    if (type_name == #x) { \
        return x##_value;  \
    }

typedef void(*UpdateFunType)(int64_t,int64_t,int64_t);

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
    static T base_type_cast(const std::shared_ptr<BaseCppTypeInterface>& ptr, UpdateFunType constuctor = nullptr);
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

class CppTypeJClass final : public BaseCppTypeInterface {
public:
    CppTypeJClass(_JNIEnv* env_, const std::any& jvalue) {
        jobject* java_object_ = std::any_cast<jobject*>(jvalue);
        jclass clz = env_->GetObjectClass(*java_object_);
        jmethodID mid = env_->GetMethodID(clz, "getDeclaredFieldsString", "()Ljava/lang/String;");
        jstring declare_string = (jstring)env_->CallObjectMethod(*java_object_, mid);
        auto declare_cstring = BaseCppTypeInterface::make(env_, &declare_string);
        json j_reflect =
            json::parse(std::string(BaseCppTypeInterface::base_type_cast<char*>(declare_cstring)));
        for (int i = 0; i < j_reflect.size(); i++) {
            std::string FieldName = j_reflect[i]["FieldName"];
            std::string Type = j_reflect[i]["Type"];
            if (Type == "int") {
                jfieldID fieldID = env_->GetFieldID(clz, FieldName.c_str(), "I");
                field_map[FieldName] =
                    BaseCppTypeInterface::make(env_, env_->GetIntField(*java_object_, fieldID));
            } else if (Type == "float") {
                jfieldID fieldID = env_->GetFieldID(clz, FieldName.c_str(), "F");
                field_map[FieldName] =
                    BaseCppTypeInterface::make(env_, env_->GetFloatField(*java_object_, fieldID));
            } else {
                LOGD("[class]%s is a jobject\n", FieldName.c_str());
            }
        }
    }
    ~CppTypeJClass() { LOGD("%s\n", __PRETTY_FUNCTION__); }
    std::any GetValue() const {
        LOGD("%s\n", __PRETTY_FUNCTION__);
        return std::any(field_map);
    }
    std::unordered_map<std::string, std::shared_ptr<BaseCppTypeInterface>> field_map;
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

class ComplexCPPClass {
public:
    int intValue = 2;
    float floatValue = 2.2f;
    ComplexCPPClass() {
        return;
    };
};

void UpdateComplexCPPClass(int64_t complex_cpp_class_ptr_, int64_t a_, int64_t b_) {
    DDEBUG;
    float* b = (float*)(b_);DDEBUG;
    LOGD("[[b]] = %f\n", *b);DDEBUG;
    int* a = (int*)(a_);DDEBUG;
    LOGD("[[a]] = %f\n", *a);DDEBUG;
    ComplexCPPClass* complex_cpp_class_ptr = (ComplexCPPClass*)(complex_cpp_class_ptr_);DDEBUG;
    complex_cpp_class_ptr->intValue = *a;DDEBUG;
    complex_cpp_class_ptr->floatValue = *b;DDEBUG;
    delete a;DDEBUG;
    delete b;DDEBUG;
    return;
};

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
        LOGD("[IS JOBJECT]\n");
        return std::make_shared<CppTypeJClass>(env, std::any(jvalue));
    }
}

template <typename T>
T* MakePointer() {
    return new T();
}

template <typename T>
T BaseCppTypeInterface::base_type_cast(const std::shared_ptr<BaseCppTypeInterface>& ptr, UpdateFunType constuctor) {
    if ((not std::is_same<T, int>::value) and (not std::is_same<T, float>::value) and
        (not std::is_same<T, double>::value) and (not std::is_same<T, uint64_t>::value) and
        (not std::is_same<T, uint16_t>::value) and (not std::is_same<T, uint8_t>::value) and
        (not std::is_same<T, int64_t>::value) and (not std::is_same<T, int16_t>::value) and
        (not std::is_same<T, int8_t>::value) and (not std::is_same<T, char*>::value) and
        (not std::is_same<T, ByteArray*>::value)) {
        LOGD("[IS JOBJECT]\n");
        auto field_map =
            std::any_cast<std::unordered_map<std::string, std::shared_ptr<BaseCppTypeInterface>>>(
                ptr->GetValue());
        std::vector<std::any>* any_list = new std::vector<std::any>();
        int* a = new int;
        float* b = new float;
        for (auto i = field_map.begin(); i != field_map.end(); i++) {
            LOGD("[JOBJECT][%s]\n", i->first.c_str());
            if (i->first == "intValue") {
                *a = BaseCppTypeInterface::base_type_cast<int>(i->second);
//                any_list->push_back(std::any(BaseCppTypeInterface::base_type_cast<int>(i->second)));
            } else {
                *b = BaseCppTypeInterface::base_type_cast<float>(i->second);
//                any_list->push_back(std::any(BaseCppTypeInterface::base_type_cast<float>(i->second)));
            }
        }
        DDEBUG;
        if (constuctor == nullptr) {
            return std::any_cast<T>(std::any(MakePointer<typename std::remove_pointer<T>::type>()));
        } else {
            T ret = std::any_cast<T>(std::any(MakePointer<typename std::remove_pointer<T>::type>()));
            DDEBUG;
            constuctor((int64_t)(ret), int64_t(a), (int64_t)(b));
            return ret;
        }
    } else if (std::is_same<T, ByteArray*>::value) {
        DDEBUG;
        T ret = std::any_cast<T>(std::any(MakePointer<typename std::remove_pointer<T>::type>()));DDEBUG;
        ByteArray buffer = std::any_cast<ByteArray>(ptr->GetValue());DDEBUG;
        ByteArray* ret_buffer = new ByteArray;
        ret_buffer->data = buffer.data;
        ret_buffer->length = buffer.length;
        return std::any_cast<T>(ret_buffer);
    }
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

// FacePassImage make_face_pass_image(const MultipleType& data, const MultipleType& width,
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
                                                                         jstring jstr,
                                                                         jobject complex_class) {
    LOGD("Java_com_example_zhouzhe_testjni_CppNative_getInt\n");
    jint test_jint_a = 1123;
    jfloat test_jfloat_a = 1.2f;
    jlong test_jlong_a = 1231283790127037121;
    jboolean test_jbool_a = 1;
    jbyte test_jbye_a = 127;
    auto a = BaseCppTypeInterface::make(env, test_jint_a);
    LOGD("a.value = %d\n", BaseCppTypeInterface::base_type_cast<int>(a));
    auto b = BaseCppTypeInterface::make(env, &jarray);
    DDEBUG;
    LOGD("b.value.data = %s\n", BaseCppTypeInterface::base_type_cast<ByteArray*>(b, UpdateByteArray)->data);
    auto c = BaseCppTypeInterface::make(env, &jstr);
    LOGD("c.value.data = %s\n", BaseCppTypeInterface::base_type_cast<char*>(c));
    jclass clz = env->GetObjectClass(complex_class);
    jmethodID mid = env->GetMethodID(clz, "getDeclaredFieldsString", "()Ljava/lang/String;");
    jstring declare_string = (jstring)env->CallObjectMethod(complex_class, mid);
    auto d = BaseCppTypeInterface::make(env, &declare_string);
    LOGD("declare_string.data = %s\n", BaseCppTypeInterface::base_type_cast<char*>(d));
    json j_reflect = json::parse(std::string(BaseCppTypeInterface::base_type_cast<char*>(d)));
    for (int i = 0; i < j_reflect.size(); i++) {
        std::string Modifier = j_reflect[i]["Modifier"];
        std::string Type = j_reflect[i]["Type"];
        std::string ClassName = j_reflect[i]["ClassName"];
        std::string FieldName = j_reflect[i]["FieldName"];
        if (Type == "int") {
            jfieldID fieldID = env->GetFieldID(clz, FieldName.c_str(), "I");
            auto test = BaseCppTypeInterface::make(env, env->GetIntField(complex_class, fieldID));
            LOGD("[class]%s::%s.%s = %d\n", Type.c_str(), ClassName.c_str(), FieldName.c_str(),
                 BaseCppTypeInterface::base_type_cast<int>(test));
        } else if (Type == "float") {
            jfieldID fieldID = env->GetFieldID(clz, FieldName.c_str(), "F");
            auto test = BaseCppTypeInterface::make(env, env->GetFloatField(complex_class, fieldID));
            LOGD("[class]%s::%s.%s = %f\n", Type.c_str(), ClassName.c_str(), FieldName.c_str(),
                 BaseCppTypeInterface::base_type_cast<float>(test));
        } else {
            LOGD("[class]%s::%s.%s is a jobject\n", Type.c_str(), ClassName.c_str(),
                 FieldName.c_str());
        }
    }
    auto e = BaseCppTypeInterface::make(env, &complex_class);
    ComplexCPPClass* e_ = BaseCppTypeInterface::base_type_cast<ComplexCPPClass*>(e, UpdateComplexCPPClass);
    DDEBUG;
    LOGD("e_->intValue = %d\n", e_->intValue);
    DDEBUG;
    LOGD("e_->floatValue = %f\n", e_->floatValue);
    return 11;
}