#include <native-lib.h>

using namespace std;
using namespace cv;


extern "C" {

JNIEXPORT void JNICALL Java_user_ar_1cube_MainActivity_process(JNIEnv *env,
                                                               jobject,
                                                               jlong matAddrGr,
                                                               jlong matAddrRgba,
                                                               jobject controlvars,
                                                               jlong points_addr,
                                                               jlong prevface_addr,
                                                               jlong pt_addr,
                                                               jlong lastpt_addr,
                                                               jlong v_pts_addr,
                                                               jdoubleArray j_faces) {
    //std::string hello = "entered";
    //env->NewStringUTF(hello.c_str());
    jobject obj = (jobject) env->GetObjectClass(controlvars);
    jclass cls = env->GetObjectClass(controlvars);
    jclass scl = env->FindClass("org/opencv/core/Scalar");
    jfieldID f_detected = env->GetFieldID(cls, "detected", "I");
    jfieldID f_tracking = env->GetFieldID(cls, "tracking", "I");
    jfieldID f_stat = env->GetFieldID(cls, "stat", "I");
    jfieldID f_undetectednum = env->GetFieldID(cls, "undetectednum", "I");
    jfieldID f_colorextract = env->GetFieldID(cls, "colorextract", "Z");
    jfieldID f_thr = env->GetFieldID(cls, "thr", "I");
    jfieldID f_succ = env->GetFieldID(cls, "succ", "I");
    jfieldID f_cap = env->GetFieldID(cls, "cap", "Z");
    jfieldID f_faces_count = env->GetFieldID(cls, "faces_count", "I");
    jfieldID f_newface = env->GetFieldID(cls, "newface", "Z");

    jmethodID m_detected = env->GetMethodID(cls, "getDetected", "()I");
    jmethodID m_tracking = env->GetMethodID(cls, "getTracking", "()I");
    jmethodID m_stat = env->GetMethodID(cls, "getStat", "()I");
    jmethodID m_succ = env->GetMethodID(cls, "getSucc", "()I");
    jmethodID m_undetectednum = env->GetMethodID(cls, "getUndetectednum", "()I");
    jmethodID m_thr = env->GetMethodID(cls, "getThr", "()I");
    jmethodID m_colorextract = env->GetMethodID(cls, "getColorextract", "()Z");
    jmethodID m_cap = env->GetMethodID(cls, "getCap", "()Z");
    jmethodID m_faces_count = env->GetMethodID(cls, "getFacesCount", "()I");
    jmethodID m_newface = env->GetMethodID(cls, "getNewface", "()Z");

    jmethodID g_detected = env->GetMethodID(cls, "setDetected", "(I)V");
    /*
    jmethodID g_tracking = env->GetMethodID(cls,"setTracking","(I)V");
    jmethodID g_stat = env->GetMethodID(cls,"setStat","(I)V");
    jmethodID g_succ = env->GetMethodID(cls,"setSucc","(I)V");
    jmethodID g_undetectednum = env->GetMethodID(cls,"setUndetectednum","(I)V");
    jmethodID g_thr = env->GetMethodID(cls,"setThr","(I)V");
    jmethodID g_colorextract = env->GetMethodID(cls,"setColorextract","(Z)V");
    */

    jint detected = env->CallIntMethod(controlvars, m_detected);
    jint tracking = env->CallIntMethod(controlvars, m_tracking);
    jint stat = env->CallIntMethod(controlvars, m_stat);
    jint undetectednum = env->CallIntMethod(controlvars, m_undetectednum);
    jboolean colorextract = env->CallBooleanMethod(controlvars, m_colorextract);
    jint thr = env->CallIntMethod(controlvars, m_thr);
    jint succ = env->CallIntMethod(controlvars, m_succ);
    jboolean cap = env->CallBooleanMethod(controlvars, m_cap);
    jint faces_count = env->CallIntMethod(controlvars, m_faces_count);
    jboolean newface = env->CallBooleanMethod(controlvars, m_newface);

    Mat &mRGBA = *(Mat *) matAddrRgba;
    Mat &mGrey = *(Mat *) matAddrGr;
    Mat prev_mGrey;
    Mat dst = FilterImage(mRGBA);
    Mat hsv, pyrdown;
    pyrDown(mRGBA, pyrdown, Size(mRGBA.cols / 2, mRGBA.rows / 2));
    cvtColor(pyrdown, hsv, CV_RGB2HSV);
    vector<Point2f> pts, prev_pts;
    vector<Point> prevface, pt, lastpt;
    Mat &point_mat = *(Mat *) points_addr;
    Mat &prevface_mat = *(Mat *) prevface_addr;
    Mat &pt_mat = *(Mat *) pt_addr;
    Mat &lastpt_mat = *(Mat *) lastpt_addr;
    Mat &v_pts_mat = *(Mat *) v_pts_addr;
    Point v1 = Point(v_pts_mat.at<Vec<int, 2>>(0, 0));
    Point v2 = Point(v_pts_mat.at<Vec<int, 2>>(1, 0));
    Point p0 = Point(v_pts_mat.at<Vec<int, 2>>(2, 0));
    vector<Scalar> face1;
    for (int i = 0; i < point_mat.rows; ++i) {
        Vec<float, 2> v = point_mat.at<Vec<float, 2> >(i, 0);
        Point2f p(v[0], v[1]);
        pts.push_back(p);
    }
    for (int i = 0; i < prevface_mat.rows; ++i) {
        Vec<int, 2> v = prevface_mat.at<Vec<int, 2> >(i, 0);
        Point p(v[0], v[1]);
        prevface.push_back(p);
    }
    for (int i = 0; i < pt_mat.rows; ++i) {
        Vec<int, 2> v = pt_mat.at<Vec<int, 2> >(i, 0);
        Point p(v[0], v[1]);
        pt.push_back(p);
    }
    for (int i = 0; i < lastpt_mat.rows; ++i) {
        Vec<int, 2> v = lastpt_mat.at<Vec<int, 2> >(i, 0);
        Point p(v[0], v[1]);
        lastpt.push_back(p);
    }
    if (tracking > 0) {
        env->CallVoidMethod(controlvars, m_detected, 2); //detected = 2;
        TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.3);
        Size subPixWinSize(10, 10), winSize(5, 5);
        vector<uchar> status;
        Mat err;
        calcOpticalFlowPyrLK(prev_mGrey, mGrey, prev_pts, pts, status, err, winSize);
        /*
        for (size_t i = 0; i < pointsf.size(); ++i)
        {
            //circle(src, pointsf_next[i], 2, Scalar(0, 255, 0), 1);
            //circle(off_view, pointsf_next[i], 2, Scalar(0, 255, 0), 1);
        }
        circle(src, pointsf_next[0], 2, Scalar(255, 0, 0), 1);
        circle(src, pointsf_next[1], 2, Scalar(0, 255, 0), 1);
        circle(src, pointsf_next[2], 2, Scalar(0, 0, 255), 1);
        circle(src, pointsf_next[3], 2, Scalar(255, 255, 255), 1);
         */
        prev_pts = pts;

        //check outliers from lk tracing
        for (size_t i = 0; i < status.size(); ++i) {
            int c_stat = (int) stat;
            c_stat++;
            if (status[i] == 0) env->SetIntField(controlvars, f_stat, (jint) c_stat); //stat++;
        }
        if (stat == 3) {
            env->SetIntField(controlvars, f_tracking, 0);//tracking = 0;
            env->SetIntField(controlvars, f_stat, 0); //stat = 0;
        }
        for (size_t i = 0; i < pts.size(); ++i) {
            if (pts[i].x > pyrdown.cols / 2 || pts[i].y > pyrdown.rows / 2) tracking = 0;
        }

        //Find cube pose in 3D
        //rvec = FindCubeOrientation(pointsf_next, cam, dist, tvec);
        //vector<Point2f> impts;
        //projectPoints(normal, rvec, tvec, cam, dist, impts);

        //Check if lost
        /*
        float ds1 = Distance(pts[0], pts[1]);
        float ds2 = Distance(pts[2], pts[3]);
        if (max(ds1, ds2) / min(ds1, ds2) > 1.4)
            env->SetIntField(controlvars, f_tracking, 0); //tracking = 0;
        float ds3 = Distance(pts[0], pts[2]);
        float ds4 = Distance(pts[1], pts[3]);
        if (max(ds3, ds4) / min(ds3, ds4) > 1.4)
            env->SetIntField(controlvars, f_tracking, 0); //tracking = 0;
        if (ds1 < 10 || ds2 < 10 || ds3 < 10 || ds4 < 10)
            env->SetIntField(controlvars, f_tracking, 0); //tracking = 0;
        */
        if (tracking == 0) {
            env->CallVoidMethod(controlvars, g_detected, 0);
            env->SetBooleanField(controlvars, f_colorextract, false); //colorextract = false;
        }
    }
    if ((int) tracking == 0) {
        //find cube face
        vector<Vec4i> lines;
        vector<Point> edges;
        HoughLinesP(dst, lines, 1, CV_PI / 45, thr, 10, 5);

        for (size_t i = 0; i < lines.size(); i++) {
            line(mRGBA, Point(2 * lines[i][0], 2 * lines[i][1]),
                 Point(2 * lines[i][2], 2 * lines[i][3]), Scalar(0, 0, 255), 1);
        }

        if (lines.size() < 50)
            env->SetIntField(controlvars, f_thr, (jint) max(thr - 1, 2)); //thr = max(thr - 1, 2);
        else env->SetIntField(controlvars, f_thr, thr + 1); //thr++
        //cout << "lines: " << lines.size() << "threshold: " << thr << endl;
        FindCubeFace1(lines, prevface, succ, detected, tracking, v1, v2, p0, pts, pt);
    } else {
        Point p = pts[0];
        Point p1 = pts[1];
        Point p2 = pts[2];
        v1 = Point(p1.x - p.x, p1.y - p.y);
        v2 = Point(p2.x - p.x, p2.y - p.y);
        pt = {Point(p.x - v1.x - v2.x, p.y - v1.y - v2.y),
              Point(p.x + 2 * v2.x - v1.x, p.y + 2 * v2.y - v1.y),
              Point(p.x + 2 * v1.x - v2.x, p.y + 2 * v1.y - v2.y)};
        prevface = {pt[0], pt[1], pt[2]};
    }
    if ((detected > 0 || undetectednum < 1)) {
        if (detected <= 0) {
            env->SetIntField(controlvars, f_undetectednum,
                             (jint) undetectednum + 1); //undetectedenum++
            pt = lastpt;
        } else {
            env->SetIntField(controlvars, f_undetectednum, 0); //undetectednum = 0
            lastpt = pt;
        }
        Point p3(pt[2].x + pt[1].x - pt[0].x, pt[2].y + pt[1].y - pt[0].y);
        line(mRGBA, Point(2 * pt[0].x, 2 * pt[0].y), Point(2 * pt[1].x, 2 * pt[1].y),
             Scalar(0, 255, 0), 2);
        line(mRGBA, Point(2 * pt[1].x, 2 * pt[1].y), Point(2 * p3.x, 2 * p3.y), Scalar(0, 255, 0),
             2);
        line(mRGBA, Point(2 * p3.x, 2 * p3.y), Point(2 * pt[2].x, 2 * pt[2].y), Scalar(0, 255, 0),
             2);
        line(mRGBA, Point(2 * pt[2].x, 2 * pt[2].y), Point(2 * pt[0].x, 2 * pt[0].y),
             Scalar(0, 255, 0), 2);
        /*
        line(off_view, pt[0], pt[1], Scalar(0, 255, 0), 2);
        line(off_view, pt[1], p3, Scalar(0, 255, 0), 2);
        line(off_view, p3, pt[2], Scalar(0, 255, 0), 2);
        line(off_view, pt[2], pt[0], Scalar(0, 255, 0), 2);
        */
        pt = winded(pt[0], pt[1], pt[2], p3);

        v1 = Point(pt[1].x - pt[0].x, pt[1].y - pt[0].y);
        v2 = Point(pt[3].x - pt[0].x, pt[3].y - pt[0].y);
        p0 = Point(pt[0].x, pt[0].y);


        vector<Point2f> ep;
        int i = 1;
        int j = 5;

        if (colorextract) {
            //cube is tracked
            for (size_t k = 0; k < 9; ++k) {
                ep.push_back(Point2f((p0.x + i * v1.x / 6.0 + j * v2.x / 6.0),
                                     (p0.y + i * v1.y / 6.0 + j * v2.y / 6.0)));
                i += 2;
                if (i == 7) {
                    i = 1;
                    j -= 2;
                }
            }
            float rad = Distance(v1, Point(0, 0)) / 6.0;
            rad = 0.8 * rad;
            float stickdist = Distance(ep[0], ep[1]);
            for (size_t i = 0; i < ep.size(); ++i) {
                //if (ep[i].x>rad && ep[i].x < src.cols / 2 - rad && ep[i].y>rad && ep[i].y < src.rows - rad)
                {
                    Scalar col_avg1, col_avg2, col_avg3;
                    vector<Point2f> cubepoints = pointcube(ep[i], stickdist);

                    //Vec3b color = yuv.at<Vec3b>(ep[i]);
                    col_avg1 = colavg(hsv, ep[i], stickdist);
                    vector<Scalar> col_avg = {col_avg1, col_avg2, col_avg3};
                    face1.push_back(col_avg1);
                    for (size_t it = 0; it < cubepoints.size(); ++it) {
                        circle(mRGBA, Point(2 * cubepoints[it].x, 2 * cubepoints[it].y), 2,
                               Scalar(255, 255, 255), 1);
                    }
                }
            }
            bool fullface = true;
            if (face1.size() != 9) fullface = false;
            /*
            for (size_t i = 0; i < faces.size() && newface; ++i) {
                if (ScalarCompare(faces[i].getCenterHSV(), face1[4]) < 25.5)
                if (CompareOnlyH(faces[i].getCenterHSV(), face1[4]) < 25)
                if	(face2.size() < 9 || FaceCompareYUV(faces[i], face2) < 1.8)
                {
                    cout << "wrong face" << endl;
                    fullface = false;
                }

            }
            */
            if (fullface) {
                //faces.push_back(SimpleFace(face1));
                double buf[27];
                for (int i = 0; i < 9; ++i) {
                    buf[3 * i] = face1[i][0];
                    buf[3 * i + 1] = face1[i][1];
                    buf[3 * i + 2] = face1[i][2];
                }
                env->SetDoubleArrayRegion(j_faces, 0, 27, buf);
                env->SetBooleanField(controlvars, f_newface, (jboolean) true);
                env->SetIntField(controlvars, f_faces_count, faces_count + 1);
            }
            face1.clear();
            env->SetBooleanField(controlvars, f_colorextract,
                                 (jboolean) false); //colorextract = false;
            //env->ReleaseIntArrayElements(j_face,);


        }
    }
    pt_mat = Mat(pt, true);
    prevface_mat = Mat(prevface, true);
    lastpt_mat = Mat(lastpt, true);
    point_mat = Mat(pts, true);
    v_pts_mat = Mat(vector<Point>{v1, v2, p0}, true);
}


JNIEXPORT jstring JNICALL
Java_user_ar_1cube_MainActivity_processColors(JNIEnv *env, jobject, jdoubleArray faces) {
    vector<SimpleFace> s_faces;
    vector<Scalar> scals;
    jsize len = env->GetArrayLength(faces);
    jdouble *cols = env->GetDoubleArrayElements(faces, 0);

    for (int i = 0; i < len; i += 3) scals.push_back(Scalar(cols[i], cols[i + 1], cols[i + 2]));
    for (int i = 0; i < len / 3; i += 9) {
        vector<Scalar> temp1 = {scals[i], scals[i + 1], scals[i + 2], scals[i + 3], scals[i + 4],
                                scals[i + 5], scals[i + 6], scals[i + 7], scals[i + 8]};
        s_faces.push_back(SimpleFace(temp1));
    }
    MyCube cube = ProcessColors(s_faces);
    cube.centerToColor();
    cube.numToColor();
    cube.printCubeCharacters();
    String c_cols = cube.colorChars();
    const char *c_state = c_cols.c_str();
    jstring state = env->NewStringUTF(c_state);
    return state;
}

#if 0


jstring Java_user_ar_1cube_MainActivity_validate(JNIEnv *env, jobject, jlong addrGray, jlong addrRgba)
{
    cv::Rect();
    cv::Mat();
    cv::Mat lines;
    cv::Mat src;
    std::string hello2 = "Hello from validate";
    return env->NewStringUTF(hello2.c_str());
}
#endif

}