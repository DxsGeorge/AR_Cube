package user.ar_cube;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.Log;

/**
 * Provides drawing instructions for a GLSurfaceView object. This class
 * must override the OpenGL ES drawing lifecycle methods:
 * <ul>
 *   <li>{@link android.opengl.GLSurfaceView.Renderer#onSurfaceCreated}</li>
 *   <li>{@link android.opengl.GLSurfaceView.Renderer#onDrawFrame}</li>
 *   <li>{@link android.opengl.GLSurfaceView.Renderer#onSurfaceChanged}</li>
 * </ul>
 */
public class MyGLRenderer implements GLSurfaceView.Renderer {


    private ControlVariables vars;

    private static final String TAG = "MyGLRenderer";

    private Square   mSquare;

    // mMVPMatrix is an abbreviation for "Model View Projection Matrix"
    private final float[] mMVPMatrix = new float[16];
    private final float[] mProjectionMatrix = new float[16];
    private final float[] mViewMatrix = new float[16];
    private final float[] mRotationMatrix = new float[16];
    private final float[] mModelMatrix = new float[16];
    private float[] mTempMatrix = new float[16];
    private float angleX = 0;
    private float angleY = 0;
    private float f_angleX = 0;
    private float f_angleY = 0;
    private boolean finished_horizontal = false;
    private boolean finished_vertical = false;
    private String temp_state = MainActivity.controlvars.color_state;

    private final float[] mAccumulatedRotation = new float[16];
    private final float[] mCurrentRotation = new float[16];

    private float mAngle;

    private int step_count;

    String face1 = "";
    String face2 = "";
    String face3 = "";
    String face4 = "";
    String face5 = "";
    String face6 = "";


    @Override
    public void onSurfaceCreated(GL10 unused, EGLConfig config) {

        // Set the background frame color
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0f);
        GLES20.glEnable(GLES20.GL_DEPTH_TEST);
        float[] color = {0f,1f,1f,1f};
        Matrix.setIdentityM(mAccumulatedRotation, 0);
        mSquare   = new Square();

    }

    @Override
    public void onDrawFrame(GL10 unused) {

        GLES20.glDisable(GLES20.GL_CULL_FACE);

        float[] scratch = new float[16];

        angleX+=0.4f;
        if (angleX > 360) finished_horizontal = true;
        if (finished_horizontal) angleY+=0.4f;
        if (angleY > 360) finished_vertical = true;

        // Draw background color
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);


        Matrix.setIdentityM(mModelMatrix, 0); // initialize to identity matrix
        Matrix.translateM(mModelMatrix, 0, 0.5f, 0.9f, 0f);


        // Set the camera position (View matrix)
        Matrix.setLookAtM(mViewMatrix, 0, 0, 0, -3, 0f, 0f, 0f, 0f, 1.0f, 0.0f);

        // Calculate the projection and view transformation
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);

        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        if ((MainActivity.controlvars.state.length() == 54) && (MainActivity.controlvars.seenFacesCount < 15))
        {

            for (int i = 0 ; i < 9 ; ++i)
            {
                face1 = face1+MainActivity.controlvars.color_state.charAt(i);
            }
            // Draw square
            drawFace(face1);

            Matrix.translateM(mModelMatrix, 0, -0.12f, 0.06f, 0f);
            Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
            mTempMatrix = mMVPMatrix.clone();
            Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);


            for (int i = 0 ; i < 9 ; ++i)
            {
                face2 = face2+MainActivity.controlvars.color_state.charAt(i+9);
            }
            // Draw square
            drawFace(face2);

            Matrix.translateM(mModelMatrix, 0, -0.12f, 0.06f, 0f);
            Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
            mTempMatrix = mMVPMatrix.clone();
            Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

            for (int i = 0 ; i < 9 ; ++i)
            {
                face3 = face3+MainActivity.controlvars.color_state.charAt(i+18);
            }
            // Draw square
            drawFace(face3);

            Matrix.translateM(mModelMatrix, 0, -0.12f, 0.06f, 0f);
            Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
            mTempMatrix = mMVPMatrix.clone();
            Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

            for (int i = 0 ; i < 9 ; ++i)
            {
                face4 = face4+MainActivity.controlvars.color_state.charAt(i+27);
            }
            // Draw square
            drawFace(face4);

            Matrix.translateM(mModelMatrix, 0, -0.12f, 0.06f, 0f);
            Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
            mTempMatrix = mMVPMatrix.clone();
            Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

            for (int i = 0 ; i < 9 ; ++i)
            {
                face5 = face5+MainActivity.controlvars.color_state.charAt(i+36);
            }
            // Draw square
            drawFace(face5);

            Matrix.translateM(mModelMatrix, 0, -0.12f, 0.06f, 0f);
            Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
            mTempMatrix = mMVPMatrix.clone();
            Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

            for (int i = 0 ; i < 9 ; ++i)
            {
                face6 = face6+MainActivity.controlvars.color_state.charAt(i+45);
            }
            // Draw square
            drawFace(face6);

        }

        else if (MainActivity.controlvars.seenFacesCount > 15)
        {
            if (MainActivity.controlvars.show_solution && finished_horizontal && finished_vertical)
            {
                if (MainActivity.controlvars.solution.charAt(MainActivity.controlvars.solution_iterator) == ' ')
                    MainActivity.controlvars.solution_iterator++;
                if (MainActivity.controlvars.solution.charAt(MainActivity.controlvars.solution_iterator+1) == 39)
                {
                    switch (MainActivity.controlvars.solution.charAt(MainActivity.controlvars.solution_iterator)) {
                        case 'F':
                            rotateFront();
                            drawOppositeArrow();
                            break;

                        case 'B':
                            rotateBack();
                            drawOppositeArrow();
                            break;

                        case 'U':
                            rotateUp();
                            drawOppositeArrow();
                            break;

                        case 'D':
                            rotateDown();
                            drawOppositeArrow();
                            break;

                        case 'L':
                            rotateLeft();
                            drawOppositeArrow();
                            break;

                        case 'R':
                            rotateRight();
                            drawOppositeArrow();
                            break;
                    }
                }
                else
                {
                    switch (MainActivity.controlvars.solution.charAt(MainActivity.controlvars.solution_iterator)) {
                        case 'F':
                            rotateFront();
                            drawArrow();
                            break;

                        case 'B':
                            rotateBack();
                            drawArrow();
                            break;

                        case 'U':
                            rotateUp();
                            drawArrow();
                            break;

                        case 'D':
                            rotateDown();
                            drawArrow();
                            break;

                        case 'L':
                            rotateLeft();
                            drawArrow();
                            break;

                        case 'R':
                            rotateRight();
                            drawArrow();
                            break;
                    }
                }


                if (MainActivity.controlvars.solution.charAt(MainActivity.controlvars.solution_iterator+1) == '2')
                {
                    switch (MainActivity.controlvars.solution.charAt(MainActivity.controlvars.solution_iterator))
                    {
                        case 'F':
                            rotateFront();
                            drawArrow();
                            break;

                        case 'B':
                            rotateBack();
                            drawArrow();
                            break;

                        case 'U':
                            rotateUp();
                            drawArrow();
                            break;

                        case 'D':
                            rotateDown();
                            drawArrow();
                            break;

                        case 'L':
                            rotateLeft();
                            drawArrow();
                            break;

                        case 'R':
                            rotateRight();
                            drawArrow();
                            break;
                    }
                    MainActivity.controlvars.solution_iterator++;
                }
            }
            createCube();


        }
    }

    @Override
    public void onSurfaceChanged(GL10 unused, int width, int height) {
        // Adjust the viewport based on geometry changes,
        // such as screen rotation
        GLES20.glViewport(0, 0, width, height);

        float ratio = (float) width / height;

        // this projection matrix is applied to object coordinates
        // in the onDrawFrame() method
        Matrix.frustumM(mProjectionMatrix, 0, -ratio, ratio, -1, 1, 3, 7);

    }

    /**
     * Utility method for compiling a OpenGL shader.
     *
     * <p><strong>Note:</strong> When developing shaders, use the checkGlError()
     * method to debug shader coding errors.</p>
     *
     * @param type - Vertex or fragment shader type.
     * @param shaderCode - String containing the shader code.
     * @return - Returns an id for the shader.
     */
    public static int loadShader(int type, String shaderCode){

        // create a vertex shader type (GLES20.GL_VERTEX_SHADER)
        // or a fragment shader type (GLES20.GL_FRAGMENT_SHADER)
        int shader = GLES20.glCreateShader(type);

        // add the source code to the shader and compile it
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);

        return shader;
    }

    /**
     * Utility method for debugging OpenGL calls. Provide the name of the call
     * just after making it:
     *
     * <pre>
     * mColorHandle = GLES20.glGetUniformLocation(mProgram, "vColor");
     * MyGLRenderer.checkGlError("glGetUniformLocation");</pre>
     *
     * If the operation is not successful, the check throws an error.
     *
     * @param glOperation - Name of the OpenGL call to check.
     */
    public static void checkGlError(String glOperation) {
        int error;
        while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
            Log.e(TAG, glOperation + ": glError " + error);
            throw new RuntimeException(glOperation + ": glError " + error);
        }
    }

    /**
     * Returns the rotation angle of the triangle shape (mTriangle).
     *
     * @return - A float representing the rotation angle.
     */
    public float getAngle() {
        return mAngle;
    }

    /**
     * Sets the rotation angle of the triangle shape (mTriangle).
     */
    public void setAngle(float angle) {
        mAngle = angle;
    }


    public void drawFace(String face)
    {
        char sq_col = face.charAt(0);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(1);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(2);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, 0.06f, -0.03f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(3);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(4);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(5);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, 0.06f, -0.03f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(6);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(7);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(8);
        mSquare.draw(mMVPMatrix, sq_col);


    }

    public void drawVerticalFace(String face)
    {
        char sq_col = face.charAt(0);

        float rot_angle = 90f;


        Matrix.rotateM(mModelMatrix,0,rot_angle,0f,1f,0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(1);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(2);
        mSquare.draw(mMVPMatrix, sq_col);


        Matrix.translateM(mModelMatrix, 0, 0.06f, -0.03f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(3);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(4);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(5);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, 0.06f, -0.03f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(6);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(7);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(8);
        mSquare.draw(mMVPMatrix, sq_col);

    }

    void drawHorizontalFace(String face)
    {
        char sq_col = face.charAt(0);

        float rot_angle = 90f;

        Matrix.rotateM(mModelMatrix,0,rot_angle,1f,0f,0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(1);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(2);
        mSquare.draw(mMVPMatrix, sq_col);


        Matrix.translateM(mModelMatrix, 0, 0.06f, -0.03f, 0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(3);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(4);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(5);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, 0.06f, -0.03f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(6);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(7);
        mSquare.draw(mMVPMatrix, sq_col);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0f, 0f);

        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        sq_col = face.charAt(8);
        mSquare.draw(mMVPMatrix, sq_col);
    }

    public void createCube()
    {

        //Matrix.setIdentityM(mModelMatrix,0);
        Matrix.translateM(mModelMatrix, 0, -0.55f, 0f, 0.5f);
        if (!finished_horizontal)
            Matrix.rotateM(mModelMatrix,0,angleX,0,1f,0f);

        else if(!finished_vertical)
            Matrix.rotateM(mModelMatrix,0,angleY,1f,0,0f);

        Matrix.rotateM(mModelMatrix,0,f_angleX,1f,0,0);
        Matrix.rotateM(mModelMatrix, 0, f_angleY, 0 , 1f,0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        drawFace(face1);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0.06f, 0.03f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        drawVerticalFace(face2);

        Matrix.translateM(mModelMatrix, 0, -0.03f, 0.06f, 0.09f);
        Matrix.rotateM(mModelMatrix,0,-90,0f,1f,0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        drawFace(face6);

        Matrix.translateM(mModelMatrix, 0, 0.09f, 0.06f, -0.09f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        drawVerticalFace(face3);

        Matrix.translateM(mModelMatrix, 0, 0f, 0.09f, -0.03f);
        Matrix.rotateM(mModelMatrix,0,-90,0f,1f,0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        drawHorizontalFace(face4);

        Matrix.translateM(mModelMatrix, 0, 0.06f, 0.06f, 0.12f);
        Matrix.rotateM(mModelMatrix,0,-90,1f,0f,0f);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);
        mTempMatrix = mMVPMatrix.clone();
        Matrix.multiplyMM(mMVPMatrix, 0, mTempMatrix, 0, mModelMatrix, 0);

        drawHorizontalFace(face5);
    }

    void rotateFront()
    {
        //Matrix.setIdentityM(mModelMatrix,0);
    }

    void rotateBack()
    {
        //Matrix.setIdentityM(mModelMatrix,0);
        face1 = face6;

    }

    void rotateUp()
    {
        //Matrix.setIdentityM(mModelMatrix,0);
        face1 = face4;
    }

    void rotateDown()
    {
        face1 = face5;
    }

    void rotateLeft()
    {
        //Matrix.setIdentityM(mModelMatrix,0);
        face1 = face2;
    }

    void rotateRight()
    {
        //Matrix.setIdentityM(mModelMatrix,0);
        face1 = face3;
    }

    void drawArrow()
    {
        Line arr_base = new Line();
        arr_base.SetVerts(-0.50f,-0.26f,0.05f,-0.60f,-0.26f,0.05f);
        arr_base.SetColor(0,1f,0,1f);
        arr_base.draw(mMVPMatrix);
    }

    void drawOppositeArrow()
    {
        Line arr_base = new Line();
        arr_base.SetVerts(-0.50f,-0.26f,0.05f,-0.60f,-0.26f,0.05f);
        arr_base.SetColor(0,1f,0,1f);
        arr_base.draw(mMVPMatrix);
    }
}
