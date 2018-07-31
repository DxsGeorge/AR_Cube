package user.ar_cube;

import android.content.Context;
import android.graphics.PixelFormat;
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
        setZOrderOnTop(true);
        setEGLConfigChooser( 8, 8, 8, 8, 16, 0 );
        getHolder().setFormat( PixelFormat.TRANSPARENT );
        setPreserveEGLContextOnPause(true);
        setRenderer(new OpenGLRenderer());
    }
}
