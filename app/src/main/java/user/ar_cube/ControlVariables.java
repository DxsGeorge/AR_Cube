package user.ar_cube;

import java.util.Vector;
import java.util.ArrayList;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.MatOfPoint;
import org.opencv.core.Point;

public class ControlVariables {
    public int trackingmode;
    public int undetectednum;
    public boolean of;
    public int thr;
    public int detected;
    public int tracking;
    public int count;
    public boolean colorextract;
    public boolean parsedsolution;
    public int solution_iterator;
    public String solution;
    public Vector<String> steps;
    public boolean finished;
    public int stat;
    public boolean cap;
    public boolean newface;
    public String state;
    public int seenFacesCount;

    public int x1, x2, x3, x4, y1, y2, y3, y4;
    public int faces_count;
    public int succ;
    public ControlVariables()
    {
        trackingmode = 2;
        undetectednum = 100;
        thr = 100;
        detected = 0;
        tracking = 0;
        count = 0;
        colorextract = false;
        parsedsolution = false;
        solution_iterator = 0;
        finished = false;
        stat = 0;
        faces_count = 0;
        succ = 0;
        cap = false;
        newface = false;
        state = "";
        seenFacesCount = 0;
    }

    public int getTracking() { return tracking; }

    public int getDetected() { return detected; }

    public int getThr() { return thr; }

    public int getUndetectednum() {return undetectednum;}

    public boolean getColorextract() {return colorextract;}

    public int getStat() {return stat;}

    public int getSucc() {return succ;}

    public boolean getCap() {return cap;}

    public int getFacesCount() {return faces_count;}

    public boolean getNewface() {return newface;}

    public void setTracking(int tr) {tracking = tr;}

    public void setDetected(int det) {detected = det;}

    public void setThr(int t) {thr = t;}

    public void setUndetectednum (int u) {undetectednum = u;}

    public void setStat(int st) {stat = st;}

    public void setSucc(int s) {succ = s;}

    public void setColorextract(boolean c) {colorextract = c;}

}
