#include <jni.h>
#include <string>

static bool is_valid = false;

/**
 *这个key就是要用来加密传输内容的rsa的公钥
 */
const char *AUTH_KEY = "GmSqgSgb3dQEB4qU";

/**
 * 发布的app 签名,只有和本签名一致的app 才会返回 正确的AUTH_KEY
 */
const char *RELEASE_SIGN = "此处填写获取到的签名信息";

/**
 * 检查加载该so的应用的签名，与预置的签名是否一致
 */
static jboolean checkSignature(JNIEnv *env);

extern "C"
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;
    if (vm->GetEnv((void **) (&env), JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    if (checkSignature(env) != JNI_TRUE) {
        return -1;
    } else {

    }
    return JNI_VERSION_1_4;
}


JNIEXPORT jboolean
Java_org_afuos_fault_ndkjnidemo_MainActivity_stringFromJNI(
        JNIEnv *env, jclass jclazz,
        jobject contextObject) {
    jclass native_class = env->GetObjectClass(contextObject);
    jmethodID pm_id = env->GetMethodID(native_class, "getPackageManager",
                                       "()Landroid/content/pm/PackageManager;");
    jobject pm_obj = env->CallObjectMethod(contextObject, pm_id);
    jclass pm_clazz = env->GetObjectClass(pm_obj);
// 得到 getPackageInfo 方法的 ID
    jmethodID package_info_id = env->GetMethodID(pm_clazz, "getPackageInfo",
                                                 "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jclass native_classs = env->GetObjectClass(contextObject);
    jmethodID mId = env->GetMethodID(native_classs, "getPackageName", "()Ljava/lang/String;");
    jstring pkg_str = static_cast<jstring>(env->CallObjectMethod(contextObject, mId));
// 获得应用包的信息
    jobject pi_obj = env->CallObjectMethod(pm_obj, package_info_id, pkg_str, 64);
// 获得 PackageInfo 类
    jclass pi_clazz = env->GetObjectClass(pi_obj);
// 获得签名数组属性的 ID
    jfieldID signatures_fieldId = env->GetFieldID(pi_clazz, "signatures",
                                                  "[Landroid/content/pm/Signature;");
    jobject signatures_obj = env->GetObjectField(pi_obj, signatures_fieldId);
    jobjectArray signaturesArray = (jobjectArray) signatures_obj;
    jsize size = env->GetArrayLength(signaturesArray);
    jobject signature_obj = env->GetObjectArrayElement(signaturesArray, 0);
    jclass signature_clazz = env->GetObjectClass(signature_obj);
    jmethodID string_id = env->GetMethodID(signature_clazz, "toCharsString",
                                           "()Ljava/lang/String;");
    jstring str = static_cast<jstring>(env->CallObjectMethod(signature_obj, string_id));
    char *c_msg = (char *) env->GetStringUTFChars(str, 0);
    //return str;
    if (strcmp(c_msg, RELEASE_SIGN) == 0)//签名一致  返回合法的 api key，否则返回错误
    {
        is_valid = JNI_TRUE;
    } else {
        is_valid = JNI_FALSE;
    }
    return is_valid;
}

/**
 * 检查加载该so的应用的签名，与预置的签名是否一致
 */
static jboolean checkSignature(JNIEnv *env) {
    // 得到当前app的NativeContext类
    jclass classNativeContext = env->FindClass("android/app/ActivityThread");
    if (classNativeContext != NULL) {
        // 得到getAppContext静态方法
        jmethodID midGetAppContext = env->GetStaticMethodID(classNativeContext,
                                                            "currentApplication",
                                                            "()Landroid/app/Application;");
        if (midGetAppContext != NULL) {
            // 调用getAppContext方法得到context对象
            jobject appContext = env->CallStaticObjectMethod(classNativeContext, midGetAppContext);
            if (appContext != NULL) {
                jboolean signatureValid = Java_org_afuos_fault_ndkjnidemo_MainActivity_stringFromJNI(
                        env,
                        NULL,
                        appContext);

                return signatureValid;
            }
        }
        return JNI_FALSE;
    }
}

extern "C"
JNIEXPORT jstring
JNICALL Java_org_afuos_mediaplayer_BaseApplication_getAESKey(JNIEnv *env, jobject instance) {
    if (is_valid) {
        return (env)->NewStringUTF(AUTH_KEY);
    } else {
        return (env)->NewStringUTF("failed");
    }
}
