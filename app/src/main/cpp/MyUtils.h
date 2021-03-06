//
// Created by user on 13.6.2018.
//

#ifndef OPENCV2_MYUTILS_H
#define OPENCV2_MYUTILS_H

#include <opencv2/opencv.hpp>
#include "eigen-git-mirror-master/Eigen/Core"
#include "eigen-git-mirror-master/Eigen/Dense"
#include <vector>
#include <map>
#include <tuple>

using namespace cv;
using namespace std;

vector<vector<float>> ArrLstToVec();

float Distance (int x1, int y1, int x2, int y2);

float Distance (Point p1, Point p2);

bool SamePoint (Point p1, Point p2, int offset);

bool FindIntersection(Vec4i l1, Vec4i l2, float &ua, float &ub, Point &pt);

float AngletoX(Vec4i pt);

int compfaces(vector<Point> f1, vector<Point> f2);


void FindCubeFace1(vector<Vec4i> lines, vector<Point> &prevface, int &succ, int &detected, int &tracking,
                   Point &v1, Point &v2, Point &p0, vector<Point2f> &features, vector<Point> &pt);



class LinePair
{
private:
    Vec4i line1;
    Vec4i line2;
    Point end1, end2, same;
    float distance;


public:
    LinePair();
    LinePair(Vec4i line1, Vec4i line2);
    LinePair(Point same, Point end1, Point end2);
    int evidence;
    Vec4i getLine1();
    Vec4i getLine2();
    Point getend1();
    Point getend2();
    Point getsame();
    float getDist();


};

class AnglePoint
{
private:
    float angle;
    Point point;
public:
    AnglePoint(float angle, Point point);
    AnglePoint();
    float getAngle();
    Point getPoint();
};

Scalar colavg(Mat src, Point2f point, float dist);

vector<Point> winded(Point p1, Point p2, Point p3, Point p4);

vector<Point2f> pointcube(Point2f a, float dist);

float ptdstHSV(Scalar a, Scalar b);

class SimpleFace
{
private:
    Scalar centerHSV;
    vector<Scalar> colorsHSV;

public:
    SimpleFace();
    SimpleFace(vector<Scalar> face1);
    Scalar getCenterHSV();
    vector<Scalar> getColorsHSV();
};

class MatchedFace
{
public:
    vector<int> colors;
    MatchedFace();
    MatchedFace(vector<int> color);
};

class MyCube
{
public:
    MatchedFace F; //front
    MatchedFace L; //left
    MatchedFace R; //right
    MatchedFace U; //under
    MatchedFace D; //down
    MatchedFace B; //back

    vector<SimpleFace> colfaces;

    MyCube();
    MyCube(vector<MatchedFace> faces, vector<SimpleFace> colorfaces);
    MyCube(vector<SimpleFace> colorfaces);
    MyCube(string colors);
    vector<MatchedFace> faces;
    vector <tuple<Scalar, char>> centercolors;
    vector<vector<char>> facecolors_char;
    void printFaces();
    void centerToColor();
    void numToColor();
    void printCubeCharacters();
    void CubeModify(string step);
    String colorChars();
};

MyCube ProcessColors(vector<SimpleFace> faces);

tuple<tuple<int,int>,tuple<int,int>> neighbors(int f, int s);


#endif //OPENCV2_MYUTILS_H
