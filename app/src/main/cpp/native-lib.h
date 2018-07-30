#include <jni.h>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include "ImageFilters.h"
#include "MyUtils.h"

extern "C" {

JNIEXPORT void JNICALL
Java_user_ar_1cube_MainActivity_process(JNIEnv *env,
                                                   jobject,
                                                   jlong matAddrGr,
                                                   jlong matAddrRgba,
                                                   jobject controlvars,
                                                   jlong points_addr,
                                                   jlong prevface_addr,
                                                   jlong pt_addr,
                                                   jlong lastpt_addr,
                                                   jlong v_pts_addr,
                                                   jdoubleArray j_faces);


JNIEXPORT jstring JNICALL
Java_user_ar_1cube_MainActivity_processColors(JNIEnv *env, jobject, jdoubleArray faces);

}