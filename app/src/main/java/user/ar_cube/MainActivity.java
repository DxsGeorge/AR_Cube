package user.ar_cube;

import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Surface;
import android.view.SurfaceView;
import android.widget.BaseAdapter;
import android.widget.TextView;
import java.util.ArrayList;
import android.widget.Button;
import android.view.View;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.JavaCameraView;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.MatOfPoint;
import org.opencv.core.MatOfPoint2f;
import org.opencv.core.Scalar;
import org.opencv.imgproc.Imgproc;


public class MainActivity extends AppCompatActivity implements CameraBridgeViewBase.CvCameraViewListener2 {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("opencv_java3");

    }

    GLSurfaceView mGLSurfaceView;

    ArrayList<Point> prevpts = new ArrayList<Point>();
    ArrayList<Point> v_list = new ArrayList<Point>();
    MatOfPoint prevface = new MatOfPoint();
    MatOfPoint pt = new MatOfPoint();
    MatOfPoint lastpt = new MatOfPoint();
    MatOfPoint v_pts = new MatOfPoint();
    Point v1 = new Point(0,0);
    Point v2 = new Point(0,0);
    Point p0 = new Point(0,0);
    public static MatOfPoint2f points = new MatOfPoint2f();
    ArrayList<Double> faces = new ArrayList<>();
    double[] face = new double[27];
    //String state = new String();


    public static ControlVariables controlvars = new ControlVariables();

    JavaCameraView javaCameraView;
    Mat mRGBA, mGray, mRGBAt, mGrayt;
    BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case BaseLoaderCallback.SUCCESS: {
                    javaCameraView.enableView();
                    break;
                }
                default: {
                    super.onManagerConnected(status);
                    break;
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);



        setContentView(R.layout.activity_main);

        final Button button = findViewById(R.id.button_id);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                controlvars.colorextract = true;
            }
        });

        javaCameraView = (JavaCameraView) findViewById(R.id.java_camera_view);
        javaCameraView.setVisibility(SurfaceView.VISIBLE);
        javaCameraView.setCvCameraViewListener(this);

        mGLSurfaceView = (GLSurfaceView) findViewById(R.id.my_surface_view);


        prevpts.add(new Point (0,0));
        prevpts.add(new Point (5,0));
        prevpts.add(new Point (0,5));
        prevface.fromList(prevpts);
        v_list.add(v1);
        v_list.add(v2);
        v_list.add(p0);
        v_pts.fromList(v_list);

        controlvars.state = "RRRRRRRRROOOOOOOOOYYYYYYYYYGGGGGGGGGBBBBBBBBBWWWWWWWWW";
    }


    @Override
    protected void onPause() {
        super.onPause();
        if (javaCameraView!=null)
            javaCameraView.disableView();
        mGLSurfaceView.onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (javaCameraView!=null)
            javaCameraView.disableView();
    }
    @Override
    protected void onResume() {
        super.onResume();
        if (OpenCVLoader.initDebug()) {
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
        mGLSurfaceView.onResume();

    }

    @Override
    public void onCameraViewStarted(int width, int height) {
        mRGBA = new Mat(height,width, CvType.CV_8UC4);
    }

    @Override
    public void onCameraViewStopped() {
        mRGBA.release();
    }

    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {


        mRGBA = inputFrame.rgba();
        mGray = inputFrame.gray();
        mRGBAt = mRGBA.t();
        mGrayt = mGray.t();
        if(getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT){
            Core.flip(mRGBA.t(),mRGBAt,90);
            Imgproc.resize(mRGBAt,mRGBAt,mRGBA.size());
            Core.flip(mGray.t(),mGrayt,90);
            Imgproc.resize(mGrayt,mGrayt,mGray.size());
            mRGBA = mRGBAt;
            mGray = mGrayt;
        }
        long addr1 = mRGBA.getNativeObjAddr();
        long addr2 = mGray.getNativeObjAddr();



        if (addr1 == 0 || addr2 == 0)
            throw new UnsupportedOperationException("Native object address is NULL");
        else
        if (controlvars.faces_count < 6)
        {

            process(mGray.getNativeObjAddr(),
                    mRGBA.getNativeObjAddr(),
                    controlvars,
                    points.getNativeObjAddr(),
                    prevface.getNativeObjAddr(),
                    pt.getNativeObjAddr(),
                    lastpt.getNativeObjAddr(),
                    v_pts.getNativeObjAddr(),
                    face);
            if (controlvars.newface)
            {
                for (int i = 0; i<27; ++i) faces.add(face[i]);
            }
            controlvars.newface = false;

            controlvars.solution = kociemba(controlvars.state);

            if (controlvars.seenFacesCount < 16 ) controlvars.seenFacesCount++;
        }
        else if (controlvars.parsedsolution == false)
        {
            double [] arr_faces = new double[faces.size()];
            for (int i = 0 ; i < faces.size() ; ++i)
            {
                arr_faces[i] = faces.get(i).doubleValue();
            }
            controlvars.state = processColors(arr_faces);



            controlvars.parsedsolution = true;

            if (controlvars.seenFacesCount < 16 ) controlvars.seenFacesCount++;
        }
        else
        {
            System.out.println(controlvars.solution);



            if (controlvars.seenFacesCount < 16 ) controlvars.seenFacesCount++;
        }
        return mRGBA;
    }

    public native void process(long matAddrGr,
                               long matAddrRgba,
                               ControlVariables controlvars,
                               long points_addr,
                               long prevface_addr,
                               long pt_addr,
                               long lastpt_addr,
                               long v_addr,
                               double[] face);

    public native String processColors(double[] faces);

    public native String kociemba(String state);
}