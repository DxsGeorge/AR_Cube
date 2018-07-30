package user.ar_cube;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.SurfaceView;

public class GLView extends GLSurfaceView{
    public GLView(Context context) {
        super(context);
        init();
    }

    public GLView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init(){
        setEGLContextClientVersion(2);
        setPreserveEGLContextOnPause(true);
        setRenderer(new OpenGLRenderer());
    }
}
