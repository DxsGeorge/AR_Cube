#include "MyUtils.h"

vector<vector<float>> ArrLstToVec()
{

}

float Distance (int x1, int y1, int x2, int y2)
{
    float abs1=pow(float (x1-x2),2)+pow(float (y1-y2),2);
    float dist=sqrt(abs1);
    return dist;
}

float Distance (Point p1, Point p2)
{
    return Distance(p1.x,p1.y,p2.x,p2.y);
}

bool SamePoint (Point p1, Point p2, int offset)
{
    return (abs(p1.x-p2.x)<offset && abs(p1.y-p2.y)<offset);
}

bool FindIntersection(Vec4i l1, Vec4i l2, float &ua, float &ub, Point &pt)
{
    Point p1(l1[0], l1[1]) , p2 (l1[2], l1[3]), p3 (l2[0], l2[1]) , p4(l2[2], l2[3]);
    float den = (p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x)*(p2.y - p1.y);
    if (abs(den) < 0.1) return false;
    ua = (p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y)*(p1.x - p3.x);
    ub = (p2.x - p1.x)*(p1.y - p3.y) - (p2.y - p1.y)*(p1.x - p3.x);
    ua = ua / den;
    ub = ub / den;
    float x = p1.x + ua*(p2.x - p1.x);
    float y = p1.y + ua*(p2.y - p1.y);
    pt = Point(x, y);
    if (ua>0 && ub>0 && ua<1 && ub<1) return true;
    return false;
}

float AngletoX(Vec4i pt)
{
    float ang = atan(float(pt[1] - pt[3] )/ float(pt[0] - pt[2]));
    ang >= 0 ? ang : ang += CV_PI;
    return ang;
}

int compfaces(vector<Point> f1, vector<Point> f2)
{
    int totd = 0;
    for (int i = 0; i < f1.size(); ++i)
    {
        int mind = 10000;
        for (int j = 0; j < f2.size(); ++j)
        {
            float d = Distance(f1[i], f2[j]);
            if (d < mind) mind = d;
        }
        totd += mind;
    }

    return (totd / 4);
}

LinePair::LinePair()
{};

LinePair::LinePair(Vec4i line1, Vec4i line2)
{
    this->line1 = line1;
    this->line2 = line2;
    if (SamePoint(Point(line1[0], line1[1]), Point(line2[0], line2[1]), 10))
    {
        this->same = Point(line1[0], line1[1]);
        this->end1 = Point(line1[2], line1[3]);
        this->end2 = Point(line2[2], line2[3]);
    }
    if (SamePoint(Point(line1[0], line1[1]), Point(line2[2], line2[3]), 10))
    {
        this->same = Point(line1[0], line1[1]);
        this->end1 = Point(line1[2], line1[3]);
        this->end2 = Point(line2[0], line2[1]);
    }
    if (SamePoint(Point(line1[2], line1[3]), Point(line2[0], line2[1]), 10))
    {
        this->same = Point(line1[2], line1[3]);
        this->end1 = Point(line1[0], line1[1]);
        this->end2 = Point(line2[2], line2[3]);
    }
    if (SamePoint(Point(line1[2], line1[3]), Point(line2[2], line2[3]), 10))
    {
        this->same = Point(line1[2], line1[3]);
        this->end1 = Point(line1[0], line1[1]);
        this->end2 = Point(line2[0], line2[1]);
    }
    this->distance = Distance(Point(line1[0], line1[1]), Point(line1[2], line1[3]));
    this->evidence = 0;
}

LinePair::LinePair(Point same, Point end1, Point end2)
{
    this->same = same;
    this->end1 = end1;
    this->end2 = end2;
    this->line1 = Vec4i{ same.x, same.y, end1.x, end1.y };
    this->line2 = Vec4i{ same.x, same.y, end2.x, end2.y };
    this->distance = (Distance(same, end1)+Distance(same,end2))/2.0;
    this->evidence = 0;
}

Vec4i LinePair::getLine1()
{
    return this->line1;
}

Vec4i LinePair::getLine2()
{
    return this->line2;
}

float LinePair::getDist()
{
    return distance;
}

Point LinePair::getend1()
{
    return this->end1;
}

Point LinePair::getend2()
{
    return this->end2;
}

Point LinePair::getsame()
{
    return this->same;
}

void FindCubeFace1(vector<Vec4i> lines, vector<Point> &prevface, int &succ, int &detected, int &tracking,
                   Point &v1, Point &v2, Point &p0, vector<Point2f> &features, vector<Point> &pt)
{
    //find cube corners based on paper
    vector <LinePair> lp;
    vector <Point> edges, test, minps;
    float qwe = 0.06;
    int minch = 10000;
    for (size_t i = 0; i < lines.size(); i++)
    {
        for (size_t j = i + 1; j < lines.size(); j++)
        {
            int matched = 0;
            LinePair hypothesis;
            Point p1, p2, p3, p4;
            float dd1, dd2;
            p1 = Point(lines[i][0], lines[i][1]);
            p2 = Point(lines[i][2], lines[i][3]);
            p3 = Point(lines[j][0], lines[j][1]);
            p4 = Point(lines[j][2], lines[j][3]);
            dd1 = Distance(p1, p2);
            dd2 = Distance(p3, p4);
            if (max(dd1, dd2) / min(dd1, dd2) > 1.3) continue;
            if (SamePoint(p1, p3, 10))
            {
                hypothesis = LinePair(lines[i], lines[j]);
                matched++;
            }
            if (SamePoint(p1, p4, 10))
            {
                hypothesis = LinePair(lines[i], lines[j]);
                matched++;
            }
            if (SamePoint(p2, p3, 10))
            {
                hypothesis = LinePair(lines[i], lines[j]);
                matched++;
            }
            if (SamePoint(p2, p4, 10))
            {
                hypothesis = LinePair(lines[i], lines[j]);
                matched++;
            }
            if (matched == 0)
            {
                //check intersection at 1/3 or 2/3
                Point pt, temp, same, end1, end2;
                float ua, ub;
                if (FindIntersection(lines[i], lines[j], ua, ub, pt))
                {
                    int ok1 = 0, ok2 = 0;
                    if (abs(ua - 1.0 / 3) < 0.05) ok1 = 1;
                    if (abs(ua - 2.0 / 3) < 0.05)  ok1 = 2;
                    if (abs(ub - 1.0 / 3)<0.05)  ok2 = 1;
                    if (abs(ub - 2.0 / 3)<0.05) ok2 = 2;
                    if (ok1>0 && ok2>0)
                    {
                        if (ok1 == 2)
                        {
                            temp = p1;
                            p1 = p2;
                            p2 = temp;

                        }
                        if (ok2 == 2)
                        {
                            temp = p3;
                            p3 = p4;
                            p4 = temp;
                        }
                        end1 = Point((p3.x + 2.0 / 3 * (p2.x - p1.x)), (p3.y + 2.0 / 3 * (p2.y - p1.y)));
                        end2 = Point((p1.x + 2.0 / 3 * (p4.x - p3.x)), (p1.y + 2.0 / 3 * (p4.y - p3.y)));
                        same = Point((p1.x - 1.0 / 3 * (p4.x - p3.x)), (p1.y - 1.0 / 3 * (p4.y - p3.y)));
                        hypothesis = LinePair(same, end1, end2);
                        matched = 1;
                    }
                }
            }
            if (matched == 1)
            {
                float ang1, ang2, ang;
                ang1 = atan(float(p1.y - p2.y) / float(p1.x - p2.x));
                ang2 = atan(float(p3.y - p4.y) / float(p3.x - p4.x));
                if (ang1 < 0) ang1 += CV_PI;
                if (ang2 < 0) ang2 += CV_PI;
                ang = abs(abs(ang2 - ang1) - CV_PI / 2);
                if (ang < 0.5) lp.push_back(hypothesis);
            }
        }
    }

    //check how many lines align to a grid
    for (size_t i = 0; i < lp.size(); i++)
    {
        Point same, end1, end2;
        same = lp[i].getsame();
        end1 = lp[i].getend1();
        end2 = lp[i].getend2();
        float ang1, ang2, distance;
        distance = lp[i].getDist();
        if (end1.x - same.x != 0) ang1 = atan((end1.y - same.y) / (end1.x - same.x));
        else ang1 = CV_PI / 2;
        if (end2.x - same.x != 0) ang2 = atan((end2.y - same.y) / (end2.x - same.x));
        else ang2 = CV_PI / 2;
        if (ang1 < 0) ang1 += CV_PI;
        if (ang2 < 0) ang2 += CV_PI;
        distance = 1.7*distance;
        int evidence = 0;
        int totallines = 0;
        Eigen::Matrix3f A, Ainv;
        A << end2.x - same.x, end1.x - same.x, same.x,
                end2.y - same.y, end1.y - same.y, same.y,
                0, 0, 1;
        Ainv = A.inverse();
        for (size_t j = 0; j < lines.size(); j++)
        {
            float ang = AngletoX(lines[j]);
            float a1, a2;
            a1 = abs(abs(ang - ang1) - CV_PI / 2);
            a2 = abs(abs(ang - ang2) - CV_PI / 2);
            if (a1 > 0.1 && a2 > 0.1) continue;
            Point q1, q2;
            q1 = Point(lines[j][0], lines[j][1]); q2 = Point(lines[j][2], lines[j][3]);
            Eigen::Matrix<float, 3, 1> v;
            v << q1.y,
                    q1.x,
                    1;
            Eigen::Matrix<float, 3, 1> vp1 = Ainv*v;
            if (vp1(0, 0) > 1.1 || vp1(0, 0)<-0.1) continue;
            if (vp1(1, 0) > 1.1 || vp1(1, 0)<-0.1) continue;
            if ((abs(vp1(0, 0) - 1 / 3.0) > qwe)
                &&
                (abs(vp1(0, 0) - 2 / 3.0) > qwe)
                &&
                (abs(vp1(1, 0) - 1 / 3.0) > qwe)
                &&
                (abs(vp1(1, 0) - 2 / 3.0) > qwe)) continue;
            Eigen::Matrix<float, 3, 1> v2;
            v2 << q2.y,
                    q2.x,
                    1;
            Eigen::Matrix<float, 3, 1> vp2 = Ainv*v2;
            if (vp2(0, 0) > 1.1 || vp2(0, 0)<-0.1) continue;
            if (vp2(1, 0) > 1.1 || vp2(1, 0)<-0.1) continue;
            if ((abs(vp2(0, 0) - 1 / 3.0) > qwe)
                &&
                (abs(vp2(0, 0) - 2 / 3.0) > qwe)
                &&
                (abs(vp2(1, 0) - 1 / 3.0) > qwe)
                &&
                (abs(vp2(1, 0) - 2 / 3.0) > qwe)) continue;
            lp[i].evidence++;
        }
    }
    std::sort(lp.begin(), lp.end(), [](LinePair a, LinePair b){ return (a.evidence > b.evidence); });
    if (lp.size() > 0)
    {
        //check for same grid at least 3 times
        for (size_t i = 0; i < lp.size(); ++i)
        {

            if (lp[i].evidence > 0.05*lines.size())
            {
                Point same(lp[i].getsame()), end1(lp[i].getend1()), end2(lp[i].getend2());
                Point p3(end2.x + end1.x - same.x, end2.y + end1.y - same.y);
                test = { same, end1, end2, p3 };
                p3 = Point(prevface[2].x + prevface[1].x - prevface[0].x, prevface[2].y + prevface[1].y - prevface[0].y);
                vector<Point> tc{ prevface[0], prevface[1], prevface[2], p3 };
                int ch = compfaces(test, tc);
                if (ch < minch)
                {
                    minch = ch;
                    minps = { same, end1, end2 };
                }
            }
        }
        if (minps.size() > 0)
        {
            prevface = minps;
            if (minch < 10)
            {
                succ += 1;
                pt = prevface;
                detected = 1;
            }
        }
        else
        {
            succ = 0;
        }
        if (succ > 2)
        {
            pt = {};
            for (int i = 1; i < 3; ++i)
            {
                for (int j = 1; j < 3; ++j)
                {
                    pt.push_back(Point(p0.x + float(i) / 3 * v1.x + float(j) / 3 * v2.x, p0.y + float(i) / 3 * v1.y + float(j) / 3 * v2.y));
                }
            }
            features = {};
            for (size_t i = 0; i < pt.size(); ++i)
            {
                features.push_back(Point2f(pt[i].x, pt[i].y));
            }
            tracking = 1;
            succ = 0;
        }
    }


}



Scalar colavg(Mat src, Point2f point, float dist)
{
    Scalar col, sum;
    vector<Point2f> points = pointcube(point, dist);
    for (size_t i = 0; i < points.size(); ++i)
    {
        sum += Scalar(src.at<Vec3b>(points[i]));
    }
    col = Scalar(sum[0]/9,sum[1]/9,sum[2]/9);
    return col;
}

AnglePoint::AnglePoint()
{

}

AnglePoint::AnglePoint(float angle, Point point)
{
    this->angle = angle;
    this->point = point;
}

float AnglePoint::getAngle()
{
    return this->angle;
}

Point AnglePoint::getPoint()
{
    return this->point;
}

vector<Point> winded(Point p1, Point p2, Point p3, Point p4)
{
    vector<AnglePoint> ps;
    vector<Point> ps1;
    Point2f avg = Point2f(0.25*(p1.x + p2.x + p3.x + p4.x), 0.25*(p1.y + p2.y + p3.y + p4.y));
    AnglePoint ts1(atan2(p1.y - avg.y, p1.x - avg.x), p1);
    AnglePoint ts2(atan2(p2.y - avg.y, p2.x - avg.x), p2);
    AnglePoint ts3(atan2(p3.y - avg.y, p3.x - avg.x), p3);
    AnglePoint ts4(atan2(p4.y - avg.y, p4.x - avg.x), p4);
    ps = {ts1, ts2, ts3, ts4};
    std::sort(ps.begin(), ps.end(), [](AnglePoint a, AnglePoint b){ return (a.getAngle() > b.getAngle()); });
    for (size_t i = 0; i < ps.size(); i++)
    {
        ps1.push_back(ps[i].getPoint());
    }
    return ps1;
}

vector<Point2f> pointcube(Point2f a, float dist)
{
    vector<Point2f> cube;
    dist = dist / 4.0;
    Point2f ul = Point2f(a.x - dist, a.y - dist);
    Point2f um = Point2f(a.x, a.y - dist);
    Point2f ur = Point2f(a.x + dist, a.y - dist);
    Point2f ml = Point2f(a.x - dist, a.y);
    Point2f mm = a;
    Point2f mr = Point2f(a.x + dist, a.y);
    Point2f bl = Point2f(a.x - dist, a.y + dist);
    Point2f bm = Point2f(a.x, a.y + dist);
    Point2f br = Point2f(a.x + dist, a.y + dist);
    cube = { ul, um, ur, ml, mm, mr, bl, bm, br };
    return cube;
}

float ptdstHSV(Scalar a, Scalar b)
{
    float dist;
    if (a(1) < 100 || b(1) < 100)
    {
        dist = 1000;
        if (a(1) < 100 && b(1) < 100)
        {
            dist = sqrt((a(1) - b(1))*(a(1) - b(1)));
        }
    }
    else dist = sqrt((a(0) - b(0))*(a(0) - b(0)));

    return dist;
}

MyCube ProcessColors(vector<SimpleFace> faces)
{
    vector<MatchedFace> correctedfaces;
    int bestj = 0;
    int besti = 0;
    int bestcon = 0;
    int matchesto;
    int bestd = 10001;
    int done = 0;
    int taken[6] = { 0, 0, 0, 0, 0, 0 };
    float dist, dist1, dist2, dist3;
    map<int, int> opposite;
    opposite[0] = 5;
    opposite[1] = 2;
    opposite[2] = 1;
    opposite[3] = 4;
    opposite[4] = 3;
    opposite[5] = 0;
    vector<int> range = { 0, 1, 2, 3, 4, 5 };


    int assigned[6][9] = {};
    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            assigned[i][j] = -1;
        }
    }
    for (int i = 0; i < 6; ++i)
    {
        assigned[i][4] = i;
    }


    map<tuple<int, int>, vector<int>> poss;
    for (int j = 0; j < 6; ++j)
    {
        for (int i = 0; i < 9; ++i)
        {
            tuple<int, int> a(j,i);
            pair<tuple<int, int>, vector<int>> b = make_pair(a,range);
            poss.insert(b);
        }
    }
    while (done < 48)
    {
        bestd = 10000;
        bool forced = false;
        for (int j = 0; j < 6; ++j)
        {
            vector<Scalar> facecols = faces[j].getColorsHSV();
            for (int i = 0; i < 9; ++i)
            {
                if ((i != 4) && (assigned[j][i] == -1) && (!forced))
                {
                    int considered = 0;
                    for (int k = 0; k < 6; ++k)
                    {
                        if (taken[k] < 8 && poss[make_tuple(j, i)][k] != -1)
                        {
                            dist = ptdstHSV(faces[k].getCenterHSV(), facecols[i]);
                            considered++;
                            if (dist < bestd)
                            {
                                bestd = dist;
                                bestj = j;
                                besti = i;
                                matchesto = k;
                            }
                        }
                    }
                    if (besti == i && bestj == j) bestcon = considered;
                    if (considered == 1)
                    {
                        forced = true;
                        cout << "Sticker" << j << ", " << i << " was forced." << endl;
                    }
                }
            }
        }
        done += 1;
        assigned[bestj][besti] = matchesto;
        if (besti == 4) cout << "what" << endl;
        int op = opposite[matchesto];
        tuple<tuple<int,int>,tuple<int,int>> ns;
        ns = neighbors(bestj, besti);
        tuple <int, int> ns1 = get<0>(ns);
        tuple <int, int> ns2 = get<1>(ns);
        if (get<0>(ns1) != -1)
        {
            //int *p = poss.at(ns1);
            for (int i = 0; i < 6; ++i)
            {
                if (matchesto == poss.at(ns1)[i])
                {
                    poss.at(ns1)[i] = -1;
                }
                if (op == poss.at(ns1)[i])
                {
                    poss.at(ns1)[i] = -1;
                }
            }
        }
        if (get<0>(ns2) != -1)
        {
            //int *p = poss.at(ns2);
            for (int i = 0; i < 6; ++i)
            {
                if (matchesto == poss.at(ns2)[i])
                {
                    poss.at(ns2)[i] = -1;
                }
                if (op == poss.at(ns2)[i])
                {
                    poss.at(ns2)[i] = -1;
                }
            }
        }
        taken[matchesto] += 1;
    }
    for (int j = 0; j < 6; ++j)
    {
        vector<int> corrface;
        for (int i = 0; i < 9; ++i)
        {
            corrface.push_back(assigned[j][i]);
        }
        correctedfaces.push_back(MatchedFace(corrface));
    }
    MyCube cube(correctedfaces,faces);
    return cube;
}

SimpleFace::SimpleFace()
{

}

SimpleFace::SimpleFace(vector<Scalar> face1)
{
    this->centerHSV = face1[4];
    this->colorsHSV = face1;
};

Scalar SimpleFace::getCenterHSV()
{
    return this->centerHSV;
}

vector<Scalar> SimpleFace::getColorsHSV()
{
    return this->colorsHSV;
}

MatchedFace::MatchedFace()
{

}

MatchedFace::MatchedFace(vector<int> color)
{
    this->colors = color;
}


bool ScalarCompareH(const Scalar& a, const Scalar& b)
{
    return a[0] < b[0];
}



bool ScalarEqual(const tuple<Scalar, char>& a, const tuple<Scalar, char>& b)
{
    int col1 = get<0>(a)[0];
    int col2 = get<0>(b)[0];
    return col1==col2;
}

MyCube::MyCube()
{

}

Scalar letter_to_scal(char c)
{
    switch (c)
    {
        case 'R':
            return Scalar(0,0,255);
            break;
        case 'B':
            return Scalar(255,0,0);
            break;
        case 'G':
            return Scalar(0,255,0);
            break;
        case 'W':
            return Scalar(255,255,255);
            break;
        case 'Y':
            return Scalar(0,255,255);
            break;
        case 'O':
            return Scalar(0,125,255);
            break;
    }

}

vector<MatchedFace> simple_to_matched(string colors)
{
    vector<MatchedFace> m_faces;
    vector<int> cols;
    int int_count = 0;
    for (int i = 0 ; i < 54 ; ++i)
    {
        if (colors[i] == colors[4]) m_faces[int_count].colors.push_back(1);
        else if (colors[i] == colors[13]) m_faces[int_count].colors.push_back(2);
        else if (colors[i] == colors[22]) m_faces[int_count].colors.push_back(3);
        else if (colors[i] == colors[31]) m_faces[int_count].colors.push_back(4);
        else if (colors[i] == colors[40]) m_faces[int_count].colors.push_back(5);
        else if (colors[i] == colors[49]) m_faces[int_count].colors.push_back(6);
        if (i%9 == 0 && i!=0) int_count++;
    }

    return m_faces;
}

MyCube::MyCube(string colors)
{
    vector<SimpleFace> simplefaces;
    for (int i = 0 ; i != colors.length() ; i+=6)
    {
        Scalar col;
        vector<Scalar> temp_face;
        for (int j = 0 ; j < 9 ; ++j) temp_face.push_back(letter_to_scal(col[i+j]));
        simplefaces.push_back(temp_face);
    }
    vector<MatchedFace> m_faces = simple_to_matched(colors);
    this->F = m_faces[0];
    this->L = m_faces[1];
    this->R = m_faces[2];
    this->U = m_faces[3];
    this->D = m_faces[4];
    this->B = m_faces[5];
    this->faces = { this->F, this->L, this->R, this->U, this->D, this->B };

    this->colfaces = simplefaces;
}

MyCube::MyCube(vector<MatchedFace> faces, vector<SimpleFace> colorfaces)
{
    if (faces.size() != 6) cout << "too many faces" << endl;
    this->F = faces[0];
    this->L = faces[1];
    this->R = faces[2];
    this->U = faces[3];
    this->D = faces[4];
    this->B = faces[5];
    this->faces = { this->F, this->L, this->R, this->U, this->D, this->B };

    this->colfaces = colorfaces;

}

MyCube::MyCube(vector<SimpleFace> colorfaces)
{
    this->colfaces = colorfaces;
}

void MyCube::printFaces()
{
    cout << "F: " << endl;
    cout << "| " << this->F.colors[0] << " | " << F.colors[3] << " | " << F.colors[6] << " |" << endl;
    cout << "| " << this->F.colors[1] << " | " << F.colors[4] << " | " << F.colors[7] << " |" << endl;
    cout << "| " << this->F.colors[2] << " | " << F.colors[5] << " | " << F.colors[8] << " |" << endl;

    cout << "L :" << endl;
    cout << "| " << this->L.colors[0] << " | " << L.colors[3] << " | " << L.colors[6] << " |" << endl;
    cout << "| " << this->L.colors[1] << " | " << L.colors[4] << " | " << L.colors[7] << " |" << endl;
    cout << "| " << this->L.colors[2] << " | " << L.colors[5] << " | " << L.colors[8] << " |" << endl;

    cout << "R: " << endl;
    cout << "| " << this->R.colors[0] << " | " << R.colors[3] << " | " << R.colors[6] << " |" << endl;
    cout << "| " << this->R.colors[1] << " | " << R.colors[4] << " | " << R.colors[7] << " |" << endl;
    cout << "| " << this->R.colors[2] << " | " << R.colors[5] << " | " << R.colors[8] << " |" << endl;

    cout << "U: " << endl;
    cout << "| " << this->U.colors[0] << " | " << U.colors[3] << " | " << U.colors[6] << " |" << endl;
    cout << "| " << this->U.colors[1] << " | " << U.colors[4] << " | " << U.colors[7] << " |" << endl;
    cout << "| " << this->U.colors[2] << " | " << U.colors[5] << " | " << U.colors[8] << " |" << endl;

    cout << "D: " << endl;
    cout << "| " << this->D.colors[0] << " | " << D.colors[3] << " | " << D.colors[6] << " |" << endl;
    cout << "| " << this->D.colors[1] << " | " << D.colors[4] << " | " << D.colors[7] << " |" << endl;
    cout << "| " << this->D.colors[2] << " | " << D.colors[5] << " | " << D.colors[8] << " |" << endl;

    cout << "B: " << endl;
    cout << "| " << this->B.colors[0] << " | " << B.colors[3] << " | " << B.colors[6] << " |" << endl;
    cout << "| " << this->B.colors[1] << " | " << B.colors[4] << " | " << B.colors[7] << " |" << endl;
    cout << "| " << this->B.colors[2] << " | " << B.colors[5] << " | " << B.colors[8] << " |" << endl;
}

void MyCube::printCubeCharacters()
{
    for (auto it = this->facecolors_char.begin(); it != facecolors_char.end(); ++it)
    {
        cout << endl;
        cout << "| " << (*it)[0] << " | " << (*it)[1] << " | " << (*it)[2] << " |" << endl;
        cout << "| " << (*it)[3] << " | " << (*it)[4] << " | " << (*it)[5] << " |" << endl;
        cout << "| " << (*it)[6] << " | " << (*it)[7] << " | " << (*it)[8] << " |" << endl;
        cout << endl;
    }
}

void MyCube::centerToColor()
{
    vector<Scalar> centers,centers2;
    for (size_t i = 0; i < this->colfaces.size(); ++i)
    {
        centers.push_back(this->colfaces[i].getCenterHSV()); //sort by H value
        centers2.push_back(this->colfaces[i].getCenterHSV()); //original cube set
    }
    sort(centers.begin(), centers.end(), ScalarCompareH);
    for (auto it = centers.begin(); it != centers.end(); ++it)
    {
        Scalar color = *it;
        if (color[1] < 100) rotate(it,it+1,centers.end());
    }
    vector<tuple<Scalar, char>> colcenters;
    colcenters.push_back(make_tuple(centers[0], 'R'));
    colcenters.push_back(make_tuple(centers[1], 'O'));
    colcenters.push_back(make_tuple(centers[2], 'Y'));
    colcenters.push_back(make_tuple(centers[3], 'G'));
    colcenters.push_back(make_tuple(centers[4], 'B'));
    colcenters.push_back(make_tuple(centers[5], 'W'));
    for (auto it2 = centers2.begin(); it2 != centers2.end(); ++it2)
    {
        for (auto it = centers.begin(); it != centers.end(); ++it)
        {
            Scalar col1 = *it;
            Scalar col2 = *it2;
            auto it3 = find_if(colcenters.begin(), colcenters.end(), [&](const tuple<Scalar, char>& e) {
                return get<0>(e) == *it2;
            });
            if (abs(col1[0] - col2[0])  < 0.1)
            {
                this->centercolors.push_back(tuple<Scalar, char>(*it2, get<1>(*it3)));
            }
        }
    }
}

void MyCube::numToColor()
{
    for (auto it = this->faces.begin(); it != this->faces.end(); ++it)
    {
        vector<int> face = it->colors;
        vector<char> oneface_char;
        for (auto it2 = face.begin(); it2 != face.end(); ++it2)
        {
            oneface_char.push_back(get<1>(this->centercolors.at(*it2)));
        }
        swap(oneface_char[1], oneface_char[3]);
        swap(oneface_char[2], oneface_char[6]);
        swap(oneface_char[5], oneface_char[7]);
        this->facecolors_char.push_back(oneface_char);
    }
}

void MyCube::CubeModify(string step)
{
    int int_step, count;
    bool cw;

    if (step[0] == 'F') int_step = 0;
    else if (step[0] == 'L') int_step = 2;
    else if (step[0] == 'R') int_step = 1;
    else if (step[0] == 'U') int_step = 3;
    else if (step[0] == 'D') int_step = 4;
    else if (step[0] == 'B') int_step = 5;


    if (step[1] == '2') count = 2;
    else count = 1;

    if (step[1] == '\'') cw = false;
    else cw = true;
    vector<MatchedFace> newfaces(6);

    if (cw)
    {
        switch (int_step)
        {
            case 0:
                //front face
                newfaces[0] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[0].colors[2], this->faces[0].colors[5], this->faces[0].colors[8],
                                        //4                                 //5                           //6
                                  this->faces[0].colors[1], this->faces[0].colors[4], this->faces[0].colors[7],
                                        //7                                 //8                           //9
                                  this->faces[0].colors[0], this->faces[0].colors[3], this->faces[0].colors[6] });

                //opposite face
                newfaces[5] = this->faces[5];

                //right face
                newfaces[1] = this->faces[1];
                newfaces[1].colors[0] = this->faces[3].colors[2];
                newfaces[1].colors[1] = this->faces[3].colors[5];
                newfaces[1].colors[2] = this->faces[3].colors[8];

                //left face
                newfaces[2] = this->faces[2];
                newfaces[2].colors[6] = this->faces[4].colors[0];
                newfaces[2].colors[7] = this->faces[4].colors[3];
                newfaces[2].colors[8] = this->faces[4].colors[6];

                //up face
                newfaces[3] = this->faces[3];
                newfaces[3].colors[2] = this->faces[2].colors[8];
                newfaces[3].colors[5] = this->faces[2].colors[7];
                newfaces[3].colors[8] = this->faces[2].colors[6];

                //down face
                newfaces[4] = this->faces[4];
                newfaces[4].colors[0] = this->faces[1].colors[2];
                newfaces[4].colors[3] = this->faces[1].colors[1];
                newfaces[4].colors[6] = this->faces[1].colors[0];
                break;

            case 1:
                //front face
                newfaces[1] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[1].colors[2], this->faces[1].colors[5], this->faces[1].colors[8],
                                        //4                                 //5                           //6
                                  this->faces[1].colors[1], this->faces[1].colors[4], this->faces[1].colors[7],
                                        //7                                 //8                           //9
                                  this->faces[1].colors[0], this->faces[1].colors[3], this->faces[1].colors[6] });

                //opposite face
                newfaces[2] = this->faces[2];

                //right face
                newfaces[5] = this->faces[5];
                newfaces[5].colors[0] = this->faces[3].colors[8];
                newfaces[5].colors[1] = this->faces[3].colors[7];
                newfaces[5].colors[2] = this->faces[3].colors[6];

                //left face
                newfaces[0] = this->faces[0];
                newfaces[0].colors[6] = this->faces[4].colors[6];
                newfaces[0].colors[7] = this->faces[4].colors[7];
                newfaces[0].colors[8] = this->faces[4].colors[8];

                //up face
                newfaces[3] = this->faces[3];
                newfaces[3].colors[6] = this->faces[0].colors[6];
                newfaces[3].colors[7] = this->faces[0].colors[7];
                newfaces[3].colors[8] = this->faces[0].colors[8];

                //down face
                newfaces[4] = this->faces[4];
                newfaces[4].colors[6] = this->faces[5].colors[2];
                newfaces[4].colors[7] = this->faces[5].colors[1];
                newfaces[4].colors[8] = this->faces[5].colors[0];
                break;

            case 2:
                //front face
                newfaces[2] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[2].colors[2], this->faces[2].colors[5], this->faces[2].colors[8],
                                        //4                                 //5                           //6
                                  this->faces[2].colors[1], this->faces[2].colors[4], this->faces[2].colors[7],
                                        //7                                 //8                           //9
                                  this->faces[2].colors[0], this->faces[2].colors[3], this->faces[2].colors[6] });

                //opposite face
                newfaces[1] = this->faces[1];

                //right face
                newfaces[0] = this->faces[0];
                newfaces[0].colors[0] = this->faces[3].colors[0];
                newfaces[0].colors[1] = this->faces[3].colors[1];
                newfaces[0].colors[2] = this->faces[3].colors[2];

                //left face
                newfaces[5] = this->faces[5];
                newfaces[5].colors[6] = this->faces[4].colors[2];
                newfaces[5].colors[7] = this->faces[4].colors[1];
                newfaces[5].colors[8] = this->faces[4].colors[0];

                //up face
                newfaces[3] = this->faces[3];
                newfaces[3].colors[0] = this->faces[5].colors[8];
                newfaces[3].colors[1] = this->faces[5].colors[7];
                newfaces[3].colors[2] = this->faces[5].colors[6];

                //down face
                newfaces[4] = this->faces[4];
                newfaces[4].colors[0] = this->faces[0].colors[0];
                newfaces[4].colors[1] = this->faces[0].colors[1];
                newfaces[4].colors[2] = this->faces[0].colors[2];
                break;

            case 3:
                //front face
                newfaces[3] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[3].colors[2], this->faces[3].colors[5], this->faces[3].colors[8],
                                        //4                                 //5                           //6
                                  this->faces[3].colors[1], this->faces[3].colors[4], this->faces[3].colors[7],
                                        //7                                 //8                           //9
                                  this->faces[3].colors[0], this->faces[3].colors[3], this->faces[3].colors[6] });

                //opposite face
                newfaces[4] = this->faces[4];

                //right face
                newfaces[1] = this->faces[1];
                newfaces[1].colors[0] = this->faces[5].colors[0];
                newfaces[1].colors[3] = this->faces[5].colors[3];
                newfaces[1].colors[6] = this->faces[5].colors[6];

                //left face
                newfaces[2] = this->faces[2];
                newfaces[2].colors[0] = this->faces[0].colors[0];
                newfaces[2].colors[3] = this->faces[0].colors[3];
                newfaces[2].colors[6] = this->faces[0].colors[6];

                //up face
                newfaces[5] = this->faces[5];
                newfaces[5].colors[0] = this->faces[2].colors[0];
                newfaces[5].colors[3] = this->faces[2].colors[3];
                newfaces[5].colors[6] = this->faces[2].colors[6];

                //down face
                newfaces[0] = this->faces[0];
                newfaces[0].colors[0] = this->faces[1].colors[0];
                newfaces[0].colors[3] = this->faces[1].colors[3];
                newfaces[0].colors[6] = this->faces[1].colors[6];
                break;

            case 4:
                //front face
                newfaces[4] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[4].colors[2], this->faces[4].colors[5], this->faces[4].colors[8],
                                        //4                                 //5                           //6
                                  this->faces[4].colors[1], this->faces[4].colors[4], this->faces[4].colors[7],
                                        //7                                 //8                           //9
                                  this->faces[4].colors[0], this->faces[4].colors[3], this->faces[4].colors[6] });

                //opposite face
                newfaces[3] = this->faces[3];

                //right face
                newfaces[1] = this->faces[1];
                newfaces[1].colors[2] = this->faces[0].colors[2];
                newfaces[1].colors[5] = this->faces[0].colors[5];
                newfaces[1].colors[8] = this->faces[0].colors[8];

                //left face
                newfaces[2] = this->faces[2];
                newfaces[2].colors[2] = this->faces[5].colors[2];
                newfaces[2].colors[5] = this->faces[5].colors[5];
                newfaces[2].colors[8] = this->faces[5].colors[8];

                //up face
                newfaces[0] = this->faces[0];
                newfaces[0].colors[2] = this->faces[2].colors[2];
                newfaces[0].colors[5] = this->faces[2].colors[5];
                newfaces[0].colors[8] = this->faces[2].colors[8];

                //down face
                newfaces[5] = this->faces[5];
                newfaces[5].colors[2] = this->faces[1].colors[2];
                newfaces[5].colors[5] = this->faces[1].colors[5];
                newfaces[5].colors[8] = this->faces[1].colors[8];
                break;

            case 5:
                //front face
                newfaces[5] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[5].colors[2], this->faces[5].colors[5], this->faces[5].colors[8],
                                        //4                                 //5                           //6
                                  this->faces[5].colors[1], this->faces[5].colors[4], this->faces[5].colors[7],
                                        //7                                 //8                           //9
                                  this->faces[5].colors[0], this->faces[5].colors[3], this->faces[5].colors[6] });

                //opposite face
                newfaces[0] = this->faces[0];

                //right face
                newfaces[2] = this->faces[2];
                newfaces[2].colors[0] = this->faces[3].colors[6];
                newfaces[2].colors[1] = this->faces[3].colors[3];
                newfaces[2].colors[2] = this->faces[3].colors[0];

                //left face
                newfaces[1] = this->faces[1];
                newfaces[1].colors[6] = this->faces[4].colors[8];
                newfaces[1].colors[7] = this->faces[4].colors[5];
                newfaces[1].colors[8] = this->faces[4].colors[2];

                //up face
                newfaces[3] = this->faces[3];
                newfaces[3].colors[0] = this->faces[1].colors[6];
                newfaces[3].colors[3] = this->faces[1].colors[7];
                newfaces[3].colors[6] = this->faces[1].colors[8];

                //down face
                newfaces[4] = this->faces[4];
                newfaces[4].colors[2] = this->faces[2].colors[0];
                newfaces[4].colors[5] = this->faces[2].colors[1];
                newfaces[4].colors[8] = this->faces[2].colors[2];
                break;
        }
    }
    else
    {
        switch (int_step)
        {
            case 0:
                //front face
                newfaces[0] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[0].colors[6], this->faces[0].colors[3], this->faces[0].colors[0],
                                        //4                                 //5                           //6
                                  this->faces[0].colors[7], this->faces[0].colors[4], this->faces[0].colors[1],
                                        //7                                 //8                           //9
                                  this->faces[0].colors[8], this->faces[0].colors[5], this->faces[0].colors[2] });

                //opposite face
                newfaces[5] = this->faces[5];

                //right face
                newfaces[1] = this->faces[1];
                newfaces[1].colors[0] = this->faces[4].colors[6];
                newfaces[1].colors[1] = this->faces[4].colors[3];
                newfaces[1].colors[2] = this->faces[4].colors[0];

                //left face
                newfaces[2] = this->faces[2];
                newfaces[2].colors[6] = this->faces[3].colors[8];
                newfaces[2].colors[7] = this->faces[3].colors[5];
                newfaces[2].colors[8] = this->faces[3].colors[2];

                //up face
                newfaces[3] = this->faces[3];
                newfaces[3].colors[2] = this->faces[1].colors[0];
                newfaces[3].colors[5] = this->faces[1].colors[1];
                newfaces[3].colors[8] = this->faces[1].colors[2];

                //down face
                newfaces[4] = this->faces[4];
                newfaces[4].colors[0] = this->faces[2].colors[6];
                newfaces[4].colors[3] = this->faces[2].colors[7];
                newfaces[4].colors[6] = this->faces[2].colors[8];
                break;

            case 1:
                //front face
                newfaces[1] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[1].colors[6], this->faces[1].colors[3], this->faces[1].colors[0],
                                        //4                                 //5                           //6
                                  this->faces[1].colors[7], this->faces[1].colors[4], this->faces[1].colors[1],
                                        //7                                 //8                           //9
                                  this->faces[1].colors[8], this->faces[1].colors[5], this->faces[1].colors[2] });

                //opposite face
                newfaces[2] = this->faces[2];

                //right face
                newfaces[5] = this->faces[5];
                newfaces[5].colors[0] = this->faces[4].colors[8];
                newfaces[5].colors[1] = this->faces[4].colors[7];
                newfaces[5].colors[2] = this->faces[4].colors[6];

                //left face
                newfaces[0] = this->faces[0];
                newfaces[0].colors[6] = this->faces[3].colors[6];
                newfaces[0].colors[7] = this->faces[3].colors[7];
                newfaces[0].colors[8] = this->faces[3].colors[8];

                //up face
                newfaces[3] = this->faces[3];
                newfaces[3].colors[6] = this->faces[5].colors[2];
                newfaces[3].colors[7] = this->faces[5].colors[1];
                newfaces[3].colors[8] = this->faces[5].colors[0];

                //down face
                newfaces[4] = this->faces[4];
                newfaces[4].colors[6] = this->faces[0].colors[6];
                newfaces[4].colors[7] = this->faces[0].colors[7];
                newfaces[4].colors[8] = this->faces[0].colors[8];
                break;

            case 2:
                //front face
                newfaces[2] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[2].colors[6], this->faces[2].colors[3], this->faces[2].colors[0],
                                        //4                                 //5                           //6
                                  this->faces[2].colors[7], this->faces[2].colors[4], this->faces[2].colors[1],
                                        //7                                 //8                           //9
                                  this->faces[2].colors[8], this->faces[2].colors[5], this->faces[2].colors[2] });

                //opposite face
                newfaces[1] = this->faces[1];

                //right face
                newfaces[0] = this->faces[0];
                newfaces[0].colors[0] = this->faces[4].colors[0];
                newfaces[0].colors[1] = this->faces[4].colors[1];
                newfaces[0].colors[2] = this->faces[4].colors[2];

                //left face
                newfaces[5] = this->faces[5];
                newfaces[5].colors[6] = this->faces[3].colors[2];
                newfaces[5].colors[7] = this->faces[3].colors[1];
                newfaces[5].colors[8] = this->faces[3].colors[0];

                //up face
                newfaces[3] = this->faces[3];
                newfaces[3].colors[0] = this->faces[0].colors[0];
                newfaces[3].colors[1] = this->faces[0].colors[1];
                newfaces[3].colors[2] = this->faces[0].colors[2];

                //down face
                newfaces[4] = this->faces[4];
                newfaces[4].colors[0] = this->faces[5].colors[8];
                newfaces[4].colors[1] = this->faces[5].colors[7];
                newfaces[4].colors[2] = this->faces[5].colors[6];
                break;

            case 3:
                //front face
                newfaces[3] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[3].colors[6], this->faces[3].colors[3], this->faces[3].colors[0],
                                        //4                                 //5                           //6
                                  this->faces[3].colors[7], this->faces[3].colors[4], this->faces[3].colors[1],
                                        //7                                 //8                           //9
                                  this->faces[3].colors[8], this->faces[3].colors[5], this->faces[3].colors[2] });

                //opposite face
                newfaces[4] = this->faces[4];

                //right face
                newfaces[1] = this->faces[1];
                newfaces[1].colors[0] = this->faces[0].colors[0];
                newfaces[1].colors[3] = this->faces[0].colors[3];
                newfaces[1].colors[6] = this->faces[0].colors[6];

                //left face
                newfaces[2] = this->faces[2];
                newfaces[2].colors[0] = this->faces[5].colors[0];
                newfaces[2].colors[3] = this->faces[5].colors[3];
                newfaces[2].colors[6] = this->faces[5].colors[6];

                //up face
                newfaces[5] = this->faces[5];
                newfaces[5].colors[0] = this->faces[1].colors[0];
                newfaces[5].colors[3] = this->faces[1].colors[3];
                newfaces[5].colors[6] = this->faces[1].colors[6];

                //down face
                newfaces[0] = this->faces[0];
                newfaces[0].colors[0] = this->faces[2].colors[0];
                newfaces[0].colors[3] = this->faces[2].colors[3];
                newfaces[0].colors[6] = this->faces[2].colors[6];
                break;

            case 4:
                //front face
                newfaces[4] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[4].colors[6], this->faces[4].colors[3], this->faces[4].colors[0],
                                        //4                                 //5                           //6
                                  this->faces[4].colors[7], this->faces[4].colors[4], this->faces[4].colors[1],
                                        //7                                 //8                           //9
                                  this->faces[4].colors[8], this->faces[4].colors[5], this->faces[4].colors[2] });

                //opposite face
                newfaces[3] = this->faces[3];

                //right face
                newfaces[1] = this->faces[1];
                newfaces[1].colors[2] = this->faces[5].colors[2];
                newfaces[1].colors[5] = this->faces[5].colors[5];
                newfaces[1].colors[8] = this->faces[5].colors[8];

                //left face
                newfaces[2] = this->faces[2];
                newfaces[2].colors[2] = this->faces[0].colors[2];
                newfaces[2].colors[5] = this->faces[0].colors[5];
                newfaces[2].colors[8] = this->faces[0].colors[8];

                //up face
                newfaces[0] = this->faces[0];
                newfaces[0].colors[2] = this->faces[1].colors[2];
                newfaces[0].colors[5] = this->faces[1].colors[5];
                newfaces[0].colors[8] = this->faces[1].colors[8];

                //down face
                newfaces[5] = this->faces[5];
                newfaces[5].colors[2] = this->faces[2].colors[2];
                newfaces[5].colors[5] = this->faces[2].colors[5];
                newfaces[5].colors[8] = this->faces[2].colors[8];
                break;

            case 5:
                //front face
                newfaces[5] =
                        //1                                 //2                           //3
                        MatchedFace(
                                { this->faces[5].colors[6], this->faces[5].colors[3], this->faces[5].colors[0],
                                        //4                                 //5                           //6
                                  this->faces[5].colors[7], this->faces[5].colors[4], this->faces[5].colors[1],
                                        //7                                 //8                           //9
                                  this->faces[5].colors[8], this->faces[5].colors[5], this->faces[5].colors[2] });

                //opposite face
                newfaces[0] = this->faces[0];

                //right face
                newfaces[2] = this->faces[2];
                newfaces[2].colors[0] = this->faces[4].colors[2];
                newfaces[2].colors[1] = this->faces[4].colors[5];
                newfaces[2].colors[2] = this->faces[4].colors[8];

                //left face
                newfaces[1] = this->faces[1];
                newfaces[1].colors[6] = this->faces[3].colors[0];
                newfaces[1].colors[7] = this->faces[3].colors[3];
                newfaces[1].colors[8] = this->faces[3].colors[6];

                //up face
                newfaces[3] = this->faces[3];
                newfaces[3].colors[0] = this->faces[2].colors[2];
                newfaces[3].colors[3] = this->faces[2].colors[1];
                newfaces[3].colors[6] = this->faces[2].colors[0];

                //down face
                newfaces[4] = this->faces[4];
                newfaces[4].colors[2] = this->faces[1].colors[8];
                newfaces[4].colors[5] = this->faces[1].colors[7];
                newfaces[4].colors[8] = this->faces[1].colors[6];
                break;
        }
    }

    this->faces = newfaces;
    this->facecolors_char.clear();
    this->numToColor();

    if (count == 2)
    {
        if (cw)
        {
            switch (int_step)
            {
                case 0:
                    //front face
                    newfaces[0] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[0].colors[2], this->faces[0].colors[5], this->faces[0].colors[8],
                                            //4                                 //5                           //6
                                      this->faces[0].colors[1], this->faces[0].colors[4], this->faces[0].colors[7],
                                            //7                                 //8                           //9
                                      this->faces[0].colors[0], this->faces[0].colors[3], this->faces[0].colors[6] });

                    //opposite face
                    newfaces[5] = this->faces[5];

                    //right face
                    newfaces[1] = this->faces[1];
                    newfaces[1].colors[0] = this->faces[3].colors[2];
                    newfaces[1].colors[1] = this->faces[3].colors[5];
                    newfaces[1].colors[2] = this->faces[3].colors[8];

                    //left face
                    newfaces[2] = this->faces[2];
                    newfaces[2].colors[6] = this->faces[4].colors[0];
                    newfaces[2].colors[7] = this->faces[4].colors[3];
                    newfaces[2].colors[8] = this->faces[4].colors[6];

                    //up face
                    newfaces[3] = this->faces[3];
                    newfaces[3].colors[2] = this->faces[2].colors[8];
                    newfaces[3].colors[5] = this->faces[2].colors[7];
                    newfaces[3].colors[8] = this->faces[2].colors[6];

                    //down face
                    newfaces[4] = this->faces[4];
                    newfaces[4].colors[0] = this->faces[1].colors[2];
                    newfaces[4].colors[3] = this->faces[1].colors[1];
                    newfaces[4].colors[6] = this->faces[1].colors[0];
                    break;

                case 1:
                    //front face
                    newfaces[1] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[1].colors[2], this->faces[1].colors[5], this->faces[1].colors[8],
                                            //4                                 //5                           //6
                                      this->faces[1].colors[1], this->faces[1].colors[4], this->faces[1].colors[7],
                                            //7                                 //8                           //9
                                      this->faces[1].colors[0], this->faces[1].colors[3], this->faces[1].colors[6] });

                    //opposite face
                    newfaces[2] = this->faces[2];

                    //right face
                    newfaces[5] = this->faces[5];
                    newfaces[5].colors[0] = this->faces[3].colors[8];
                    newfaces[5].colors[1] = this->faces[3].colors[7];
                    newfaces[5].colors[2] = this->faces[3].colors[6];

                    //left face
                    newfaces[0] = this->faces[0];
                    newfaces[0].colors[6] = this->faces[4].colors[6];
                    newfaces[0].colors[7] = this->faces[4].colors[7];
                    newfaces[0].colors[8] = this->faces[4].colors[8];

                    //up face
                    newfaces[3] = this->faces[3];
                    newfaces[3].colors[6] = this->faces[0].colors[6];
                    newfaces[3].colors[7] = this->faces[0].colors[7];
                    newfaces[3].colors[8] = this->faces[0].colors[8];

                    //down face
                    newfaces[4] = this->faces[4];
                    newfaces[4].colors[6] = this->faces[5].colors[2];
                    newfaces[4].colors[7] = this->faces[5].colors[1];
                    newfaces[4].colors[8] = this->faces[5].colors[0];
                    break;

                case 2:
                    //front face
                    newfaces[2] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[2].colors[2], this->faces[2].colors[5], this->faces[2].colors[8],
                                            //4                                 //5                           //6
                                      this->faces[2].colors[1], this->faces[2].colors[4], this->faces[2].colors[7],
                                            //7                                 //8                           //9
                                      this->faces[2].colors[0], this->faces[2].colors[3], this->faces[2].colors[6] });

                    //opposite face
                    newfaces[1] = this->faces[1];

                    //right face
                    newfaces[0] = this->faces[0];
                    newfaces[0].colors[0] = this->faces[3].colors[0];
                    newfaces[0].colors[1] = this->faces[3].colors[1];
                    newfaces[0].colors[2] = this->faces[3].colors[2];

                    //left face
                    newfaces[5] = this->faces[5];
                    newfaces[5].colors[6] = this->faces[4].colors[2];
                    newfaces[5].colors[7] = this->faces[4].colors[1];
                    newfaces[5].colors[8] = this->faces[4].colors[0];

                    //up face
                    newfaces[3] = this->faces[3];
                    newfaces[3].colors[0] = this->faces[5].colors[8];
                    newfaces[3].colors[1] = this->faces[5].colors[7];
                    newfaces[3].colors[2] = this->faces[5].colors[6];

                    //down face
                    newfaces[4] = this->faces[4];
                    newfaces[4].colors[0] = this->faces[0].colors[0];
                    newfaces[4].colors[1] = this->faces[0].colors[1];
                    newfaces[4].colors[2] = this->faces[0].colors[2];
                    break;

                case 3:
                    //front face
                    newfaces[3] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[3].colors[2], this->faces[3].colors[5], this->faces[3].colors[8],
                                            //4                                 //5                           //6
                                      this->faces[3].colors[1], this->faces[3].colors[4], this->faces[3].colors[7],
                                            //7                                 //8                           //9
                                      this->faces[3].colors[0], this->faces[3].colors[3], this->faces[3].colors[6] });

                    //opposite face
                    newfaces[4] = this->faces[4];

                    //right face
                    newfaces[1] = this->faces[1];
                    newfaces[1].colors[0] = this->faces[5].colors[0];
                    newfaces[1].colors[3] = this->faces[5].colors[3];
                    newfaces[1].colors[6] = this->faces[5].colors[6];

                    //left face
                    newfaces[2] = this->faces[2];
                    newfaces[2].colors[0] = this->faces[0].colors[0];
                    newfaces[2].colors[3] = this->faces[0].colors[3];
                    newfaces[2].colors[6] = this->faces[0].colors[6];

                    //up face
                    newfaces[5] = this->faces[5];
                    newfaces[5].colors[0] = this->faces[2].colors[0];
                    newfaces[5].colors[3] = this->faces[2].colors[3];
                    newfaces[5].colors[6] = this->faces[2].colors[6];

                    //down face
                    newfaces[0] = this->faces[0];
                    newfaces[0].colors[0] = this->faces[1].colors[0];
                    newfaces[0].colors[3] = this->faces[1].colors[3];
                    newfaces[0].colors[6] = this->faces[1].colors[6];
                    break;

                case 4:
                    //front face
                    newfaces[4] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[4].colors[2], this->faces[4].colors[5], this->faces[4].colors[8],
                                            //4                                 //5                           //6
                                      this->faces[4].colors[1], this->faces[4].colors[4], this->faces[4].colors[7],
                                            //7                                 //8                           //9
                                      this->faces[4].colors[0], this->faces[4].colors[3], this->faces[4].colors[6] });

                    //opposite face
                    newfaces[3] = this->faces[3];

                    //right face
                    newfaces[1] = this->faces[1];
                    newfaces[1].colors[2] = this->faces[0].colors[2];
                    newfaces[1].colors[5] = this->faces[0].colors[5];
                    newfaces[1].colors[8] = this->faces[0].colors[8];

                    //left face
                    newfaces[2] = this->faces[2];
                    newfaces[2].colors[2] = this->faces[5].colors[2];
                    newfaces[2].colors[5] = this->faces[5].colors[5];
                    newfaces[2].colors[8] = this->faces[5].colors[8];

                    //up face
                    newfaces[0] = this->faces[0];
                    newfaces[0].colors[2] = this->faces[2].colors[2];
                    newfaces[0].colors[5] = this->faces[2].colors[5];
                    newfaces[0].colors[8] = this->faces[2].colors[8];

                    //down face
                    newfaces[5] = this->faces[5];
                    newfaces[5].colors[2] = this->faces[1].colors[2];
                    newfaces[5].colors[5] = this->faces[1].colors[5];
                    newfaces[5].colors[8] = this->faces[1].colors[8];
                    break;

                case 5:
                    //front face
                    newfaces[5] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[5].colors[2], this->faces[5].colors[5], this->faces[5].colors[8],
                                            //4                                 //5                           //6
                                      this->faces[5].colors[1], this->faces[5].colors[4], this->faces[5].colors[7],
                                            //7                                 //8                           //9
                                      this->faces[5].colors[0], this->faces[5].colors[3], this->faces[5].colors[6] });

                    //opposite face
                    newfaces[0] = this->faces[0];

                    //right face
                    newfaces[2] = this->faces[2];
                    newfaces[2].colors[0] = this->faces[3].colors[6];
                    newfaces[2].colors[1] = this->faces[3].colors[3];
                    newfaces[2].colors[2] = this->faces[3].colors[0];

                    //left face
                    newfaces[1] = this->faces[1];
                    newfaces[1].colors[6] = this->faces[4].colors[8];
                    newfaces[1].colors[7] = this->faces[4].colors[5];
                    newfaces[1].colors[8] = this->faces[4].colors[2];

                    //up face
                    newfaces[3] = this->faces[3];
                    newfaces[3].colors[0] = this->faces[1].colors[6];
                    newfaces[3].colors[3] = this->faces[1].colors[7];
                    newfaces[3].colors[6] = this->faces[1].colors[8];

                    //down face
                    newfaces[4] = this->faces[4];
                    newfaces[4].colors[2] = this->faces[2].colors[0];
                    newfaces[4].colors[5] = this->faces[2].colors[1];
                    newfaces[4].colors[8] = this->faces[2].colors[2];
                    break;
            }
        }
        else
        {
            switch (int_step)
            {
                case 0:
                    //front face
                    newfaces[0] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[0].colors[6], this->faces[0].colors[3], this->faces[0].colors[0],
                                            //4                                 //5                           //6
                                      this->faces[0].colors[7], this->faces[0].colors[4], this->faces[0].colors[1],
                                            //7                                 //8                           //9
                                      this->faces[0].colors[8], this->faces[0].colors[5], this->faces[0].colors[2] });

                    //opposite face
                    newfaces[5] = this->faces[5];

                    //right face
                    newfaces[1] = this->faces[1];
                    newfaces[1].colors[0] = this->faces[4].colors[6];
                    newfaces[1].colors[1] = this->faces[4].colors[3];
                    newfaces[1].colors[2] = this->faces[4].colors[0];

                    //left face
                    newfaces[2] = this->faces[2];
                    newfaces[2].colors[6] = this->faces[3].colors[8];
                    newfaces[2].colors[7] = this->faces[3].colors[5];
                    newfaces[2].colors[8] = this->faces[3].colors[2];

                    //up face
                    newfaces[3] = this->faces[3];
                    newfaces[3].colors[2] = this->faces[1].colors[0];
                    newfaces[3].colors[5] = this->faces[1].colors[1];
                    newfaces[3].colors[8] = this->faces[1].colors[2];

                    //down face
                    newfaces[4] = this->faces[4];
                    newfaces[4].colors[0] = this->faces[2].colors[6];
                    newfaces[4].colors[3] = this->faces[2].colors[7];
                    newfaces[4].colors[6] = this->faces[2].colors[8];
                    break;

                case 1:
                    //front face
                    newfaces[1] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[1].colors[6], this->faces[1].colors[3], this->faces[1].colors[0],
                                            //4                                 //5                           //6
                                      this->faces[1].colors[7], this->faces[1].colors[4], this->faces[1].colors[1],
                                            //7                                 //8                           //9
                                      this->faces[1].colors[8], this->faces[1].colors[5], this->faces[1].colors[2] });

                    //opposite face
                    newfaces[2] = this->faces[2];

                    //right face
                    newfaces[5] = this->faces[5];
                    newfaces[5].colors[0] = this->faces[4].colors[8];
                    newfaces[5].colors[1] = this->faces[4].colors[7];
                    newfaces[5].colors[2] = this->faces[4].colors[6];

                    //left face
                    newfaces[0] = this->faces[0];
                    newfaces[0].colors[6] = this->faces[3].colors[6];
                    newfaces[0].colors[7] = this->faces[3].colors[7];
                    newfaces[0].colors[8] = this->faces[3].colors[8];

                    //up face
                    newfaces[3] = this->faces[3];
                    newfaces[3].colors[6] = this->faces[5].colors[2];
                    newfaces[3].colors[7] = this->faces[5].colors[1];
                    newfaces[3].colors[8] = this->faces[5].colors[0];

                    //down face
                    newfaces[4] = this->faces[4];
                    newfaces[4].colors[6] = this->faces[0].colors[6];
                    newfaces[4].colors[7] = this->faces[0].colors[7];
                    newfaces[4].colors[8] = this->faces[0].colors[8];
                    break;

                case 2:
                    //front face
                    newfaces[2] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[2].colors[6], this->faces[2].colors[3], this->faces[2].colors[0],
                                            //4                                 //5                           //6
                                      this->faces[2].colors[7], this->faces[2].colors[4], this->faces[2].colors[1],
                                            //7                                 //8                           //9
                                      this->faces[2].colors[8], this->faces[2].colors[5], this->faces[2].colors[2] });

                    //opposite face
                    newfaces[1] = this->faces[1];

                    //right face
                    newfaces[0] = this->faces[0];
                    newfaces[0].colors[0] = this->faces[4].colors[0];
                    newfaces[0].colors[1] = this->faces[4].colors[1];
                    newfaces[0].colors[2] = this->faces[4].colors[2];

                    //left face
                    newfaces[5] = this->faces[5];
                    newfaces[5].colors[6] = this->faces[3].colors[2];
                    newfaces[5].colors[7] = this->faces[3].colors[1];
                    newfaces[5].colors[8] = this->faces[3].colors[0];

                    //up face
                    newfaces[3] = this->faces[3];
                    newfaces[3].colors[0] = this->faces[0].colors[0];
                    newfaces[3].colors[1] = this->faces[0].colors[1];
                    newfaces[3].colors[2] = this->faces[0].colors[2];

                    //down face
                    newfaces[4] = this->faces[4];
                    newfaces[4].colors[0] = this->faces[5].colors[8];
                    newfaces[4].colors[1] = this->faces[5].colors[7];
                    newfaces[4].colors[2] = this->faces[5].colors[6];
                    break;

                case 3:
                    //front face
                    newfaces[3] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[3].colors[6], this->faces[3].colors[3], this->faces[3].colors[0],
                                            //4                                 //5                           //6
                                      this->faces[3].colors[7], this->faces[3].colors[4], this->faces[3].colors[1],
                                            //7                                 //8                           //9
                                      this->faces[3].colors[8], this->faces[3].colors[5], this->faces[3].colors[2] });

                    //opposite face
                    newfaces[4] = this->faces[4];

                    //right face
                    newfaces[1] = this->faces[1];
                    newfaces[1].colors[0] = this->faces[0].colors[0];
                    newfaces[1].colors[3] = this->faces[0].colors[3];
                    newfaces[1].colors[6] = this->faces[0].colors[6];

                    //left face
                    newfaces[2] = this->faces[2];
                    newfaces[2].colors[0] = this->faces[5].colors[0];
                    newfaces[2].colors[3] = this->faces[5].colors[3];
                    newfaces[2].colors[6] = this->faces[5].colors[6];

                    //up face
                    newfaces[5] = this->faces[5];
                    newfaces[5].colors[0] = this->faces[1].colors[0];
                    newfaces[5].colors[3] = this->faces[1].colors[3];
                    newfaces[5].colors[6] = this->faces[1].colors[6];

                    //down face
                    newfaces[0] = this->faces[0];
                    newfaces[0].colors[0] = this->faces[2].colors[0];
                    newfaces[0].colors[3] = this->faces[2].colors[3];
                    newfaces[0].colors[6] = this->faces[2].colors[6];
                    break;

                case 4:
                    //front face
                    newfaces[4] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[4].colors[6], this->faces[4].colors[3], this->faces[4].colors[0],
                                            //4                                 //5                           //6
                                      this->faces[4].colors[7], this->faces[4].colors[4], this->faces[4].colors[1],
                                            //7                                 //8                           //9
                                      this->faces[4].colors[8], this->faces[4].colors[5], this->faces[4].colors[2] });

                    //opposite face
                    newfaces[3] = this->faces[3];

                    //right face
                    newfaces[1] = this->faces[1];
                    newfaces[1].colors[2] = this->faces[5].colors[2];
                    newfaces[1].colors[5] = this->faces[5].colors[5];
                    newfaces[1].colors[8] = this->faces[5].colors[8];

                    //left face
                    newfaces[2] = this->faces[2];
                    newfaces[2].colors[2] = this->faces[0].colors[2];
                    newfaces[2].colors[5] = this->faces[0].colors[5];
                    newfaces[2].colors[8] = this->faces[0].colors[8];

                    //up face
                    newfaces[0] = this->faces[0];
                    newfaces[0].colors[2] = this->faces[1].colors[2];
                    newfaces[0].colors[5] = this->faces[1].colors[5];
                    newfaces[0].colors[8] = this->faces[1].colors[8];

                    //down face
                    newfaces[5] = this->faces[5];
                    newfaces[5].colors[2] = this->faces[2].colors[2];
                    newfaces[5].colors[5] = this->faces[2].colors[5];
                    newfaces[5].colors[8] = this->faces[2].colors[8];
                    break;

                case 5:
                    //front face
                    newfaces[5] =
                            //1                                 //2                           //3
                            MatchedFace(
                                    { this->faces[5].colors[6], this->faces[5].colors[3], this->faces[5].colors[0],
                                            //4                                 //5                           //6
                                      this->faces[5].colors[7], this->faces[5].colors[4], this->faces[5].colors[1],
                                            //7                                 //8                           //9
                                      this->faces[5].colors[8], this->faces[5].colors[5], this->faces[5].colors[2] });

                    //opposite face
                    newfaces[0] = this->faces[0];

                    //right face
                    newfaces[2] = this->faces[2];
                    newfaces[2].colors[0] = this->faces[4].colors[2];
                    newfaces[2].colors[1] = this->faces[4].colors[5];
                    newfaces[2].colors[2] = this->faces[4].colors[8];

                    //left face
                    newfaces[1] = this->faces[1];
                    newfaces[1].colors[6] = this->faces[3].colors[0];
                    newfaces[1].colors[7] = this->faces[3].colors[3];
                    newfaces[1].colors[8] = this->faces[3].colors[6];

                    //up face
                    newfaces[3] = this->faces[3];
                    newfaces[3].colors[0] = this->faces[2].colors[2];
                    newfaces[3].colors[3] = this->faces[2].colors[1];
                    newfaces[3].colors[6] = this->faces[2].colors[0];

                    //down face
                    newfaces[4] = this->faces[4];
                    newfaces[4].colors[2] = this->faces[1].colors[8];
                    newfaces[4].colors[5] = this->faces[1].colors[7];
                    newfaces[4].colors[8] = this->faces[1].colors[6];
                    break;
            }
        }
    }

    this->faces = newfaces;
    this->facecolors_char.clear();
    this->numToColor();
}

String MyCube::colorChars()
{
    ostringstream oss;
    for (vector<vector<char>>::iterator it = this->facecolors_char.begin(); it != facecolors_char.end(); ++it)
    {
        for (vector<char>::iterator it2 = it->begin(); it2 != it->end(); ++it2) oss<<(*it2);
    }
    string cols = oss.str();
    return cols;
}

tuple<tuple<int,int>,tuple<int,int>> neighbors(int f, int s)
{
    tuple<int, int> a;
    tuple<int, int> b;
    if (f == 0 && s == 0) { a = make_tuple( 2, 6 ); b = make_tuple( 3, 2 );}
    if (f == 0 && s == 1) { a = make_tuple( 2, 7 ); b = make_tuple(-1, -1);}
    if (f == 0 && s == 2) { a = make_tuple( 2, 8 ); b = make_tuple( 4, 0 );}
    if (f == 0 && s == 3) { a = make_tuple( 3, 5 ); b = make_tuple( -1, -1 );}
    if (f == 0 && s == 5) { a = make_tuple( 4, 3 ); b = make_tuple( -1, -1 );}
    if (f == 0 && s == 6) { a = make_tuple( 3, 8 ); b = make_tuple( 1, 0 );}
    if (f == 0 && s == 7) { a = make_tuple( 1, 1 ); b = make_tuple( -1, -1 );}
    if (f == 0 && s == 8) { a = make_tuple( 1, 2 ); b = make_tuple( 4, 6 );}

    if (f == 1 && s == 0) { a = make_tuple( 0, 6 ); b = make_tuple( 3, 8 );}
    if (f == 1 && s == 1) { a = make_tuple( 0, 7 ); b = make_tuple( -1, -1 );}
    if (f == 1 && s == 2) { a = make_tuple( 0, 8 ); b = make_tuple( 4, 6 );}
    if (f == 1 && s == 3) { a = make_tuple( 3, 7 ); b = make_tuple( -1, -1 );}
    if (f == 1 && s == 5) { a = make_tuple( 4, 7 ); b = make_tuple( -1, -1 );}
    if (f == 1 && s == 6) { a = make_tuple( 3, 6 ); b = make_tuple( 5, 0 );}
    if (f == 1 && s == 7) { a = make_tuple( 5, 1 ); b = make_tuple(-1, -1);}
    if (f == 1 && s == 8) { a = make_tuple( 4, 8 ); b = make_tuple(5, 2);}

    if (f == 2 && s == 0) { a = make_tuple(3, 0); b = make_tuple(5, 6);}
    if (f == 2 && s == 1) { a = make_tuple(5, 7); b = make_tuple(-1, -1);}
    if (f == 2 && s == 2) { a = make_tuple(4, 2); b = make_tuple(5, 8);}
    if (f == 2 && s == 3) { a = make_tuple(3, 1); b = make_tuple(-1, -1);}
    if (f == 2 && s == 5) { a = make_tuple(4, 1); b = make_tuple(-1, -1);}
    if (f == 2 && s == 6) { a = make_tuple(0, 0); b = make_tuple(3, 2);}
    if (f == 2 && s == 7) { a = make_tuple(0, 1); b = make_tuple(-1, -1);}
    if (f == 2 && s == 8) { a = make_tuple(0, 2); b = make_tuple(4, 0);}

    if (f == 3 && s == 0) {a = make_tuple(2, 0); b = make_tuple(5, 6);}
    if (f == 3 && s == 1) {a = make_tuple(2, 3); b = make_tuple(-1, -1);}
    if (f == 3 && s == 2) {a = make_tuple(0, 0); b = make_tuple(2, 6);}
    if (f == 3 && s == 3) {a = make_tuple(5, 3); b = make_tuple(-1, -1);}
    if (f == 3 && s == 5) {a = make_tuple(0, 3); b = make_tuple(-1, -1);}
    if (f == 3 && s == 6) {a = make_tuple(1, 6); b = make_tuple(5, 0);}
    if (f == 3 && s == 7) {a = make_tuple(1, 3); b = make_tuple(-1, -1);}
    if (f == 3 && s == 8) {a = make_tuple(0, 6); b = make_tuple(1, 0);}

    if (f == 4 && s == 0) {a = make_tuple(0, 2); b = make_tuple(2, 8);}
    if (f == 4 && s == 1) {a = make_tuple(2, 5); b = make_tuple(-1, -1);}
    if (f == 4 && s == 2) {a = make_tuple(2, 2); b = make_tuple(5, 8);}
    if (f == 4 && s == 3) {a = make_tuple(0, 5); b = make_tuple(-1, -1);}
    if (f == 4 && s == 5) {a = make_tuple(5, 5); b = make_tuple(-1, -1);}
    if (f == 4 && s == 6) {a = make_tuple(0, 8); b = make_tuple(1, 2);}
    if (f == 4 && s == 7) {a = make_tuple(1, 5); b = make_tuple(-1, -1);}
    if (f == 4 && s == 8) {a = make_tuple(1, 8); b = make_tuple(5, 2);}

    if (f == 5 && s == 0) {a = make_tuple(3, 6); b = make_tuple(1, 6);}
    if (f == 5 && s == 1) {a = make_tuple(1, 7); b = make_tuple(-1, -1);}
    if (f == 5 && s == 2) {a = make_tuple(1, 8); b = make_tuple(4, 8);}
    if (f == 5 && s == 3) {a = make_tuple(3, 3); b = make_tuple(-1, -1);}
    if (f == 5 && s == 5) {a = make_tuple(4, 5); b = make_tuple(-1, -1);}
    if (f == 5 && s == 6) {a = make_tuple(2, 0); b = make_tuple(3, 0);}
    if (f == 5 && s == 7) {a = make_tuple(2, 1); b = make_tuple(-1, -1);}
    if (f == 5 && s == 8) {a = make_tuple(2, 2); b = make_tuple(4, 2);}
    tuple<tuple<int,int>, tuple<int,int>> neighbor(a,b);
    return neighbor;
}

