/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2010 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libfreenect.h"
#include "libfreenect-registration.h"

#include <pthread.h>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <math.h>

#include <cv.h>
#include <highgui.h>
#include <mpi.h>
# include <background_segm.hpp>

# include "Image.h"
# include "ImagePool.h"
# include "common.h"
# include "TrainData.h"
# include "Node.h"
# include "RandomForest.h"
# include "parseTreeArgs.h"
# include "Sparse.h"
# include "FloodFill.h"

# include <ImfRgbaFile.h>
# include <ImfArray.h>
# include <queue>


// DEFINITIONS
# define MAX_AREA_CONTOUR 6500
# define MAX_AREA_CONTOUR_DILATE 100
# define DILATE_ITER 1
# define ERODE_ITER 2
# define PIXEL_JUMP 3
# define FLOOD_FILL_ITER 1
# define FLOOD_FILL_THRESHOLD 10
# define MS_ITER 5
# define MS_EP 1
# define MS_WINSIZE 10

using namespace Imath;
using namespace Imf;
using namespace cv;

RandomForest g_a;

float thresTMP = 0.0;


bool stop_learning = false;

BackgroundSubtractorMOG2 BGModel;

pthread_t freenect_thread;
volatile int die = 0;

int g_argc;
char **g_argv;

int window;

pthread_mutex_t gl_backbuf_mutex = PTHREAD_MUTEX_INITIALIZER;

// back: owned by libfreenect (implicit for depth)
// mid: owned by callbacks, "latest frame ready"
// front: owned by GL, "currently being drawn"
uint8_t *depth_mid, *depth_front;
uint8_t *rgb_back, *rgb_mid, *rgb_front;

GLuint gl_depth_tex;
GLuint gl_rgb_tex;

freenect_context *f_ctx;
freenect_device *f_dev;
int freenect_angle = 0;
int freenect_led;

freenect_video_format requested_format = FREENECT_VIDEO_RGB;
freenect_video_format current_format = FREENECT_VIDEO_RGB;

pthread_cond_t gl_frame_cond = PTHREAD_COND_INITIALIZER;
int got_rgb = 0;
int got_depth = 0;
bool meanshift = false;

unsigned char color[7][3] = 
{
    {  0,  0,  0},
    {255,  0,  0},
    {  0,255,  0},
    {  0,  0,255},
    {255,255,  0},
    {  0,255,255},
    {255,  0,255},
};

/*
void meanShift (Mat& probMat, Rect& com, int niters)
{
    
    Mat data;

    Rect window;
    
    
    window.x = com.x - (com.width / 2); 
    window.y = com.y - (com.height / 2); 

    window = Rect(window) & Rect(0, 0, probMat->cols, probMat->rows);
    

    for (i = 0; i < niters; i++) {
        


    }

}
*/

void refineSegments(Mat& img, Mat& mask)
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    Mat temp;
    
    // 3x3 element used.
    erode(mask, temp, Mat(), Point(-1,-1), ERODE_ITER);
    
    findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	
	mask = Mat::zeros(img.size(), CV_8U);
    
    if( contours.size() == 0 )
        return;
        
    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0;
   
    Scalar color(255);
    
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        const vector<Point>& c = contours[idx];
        double area = fabs(contourArea(Mat(c)));
        if( area > MAX_AREA_CONTOUR )
        {
            approxPolyDP(contours[idx], contours[idx], 2, true);
            drawContours(mask, contours, idx, color, CV_FILLED, 8, hierarchy );
        }
    }

    // Set to 0 the background
    img.setTo(0, 255 - mask);
}

/**
 * Refine the probability mask
 */

void refineSegments2(Mat& img, Mat& mask)
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    Mat temp;
    //namedWindow("no contour");
    //imshow("no contour", img);
    //waitKey(0);

   findContours( mask, contours, hierarchy, CV_RETR_CCOMP, 
                    CV_CHAIN_APPROX_SIMPLE );
	
    if( contours.size() == 0 )
        return;
        
    // iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0;
   
    Scalar color(255);
    
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        const vector<Point>& c = contours[idx];
        double area = fabs(contourArea(Mat(c)));
        if( area > MAX_AREA_CONTOUR_DILATE )
        {
            approxPolyDP(contours[idx], contours[idx], 2, true);
            drawContours(mask, contours, idx, color, CV_FILLED, 8, hierarchy );
        }
    }
    
    // Set to 0 the background
    img.setTo(0, 255 - mask);

    dilate(img, img, Mat(), Point(-1,-1), DILATE_ITER);
}



void DrawGLScene()
{
	pthread_mutex_lock(&gl_backbuf_mutex);

	// When using YUV_RGB mode, RGB frames only arrive at 15Hz, so we shouldn't force them to draw in lock-step.
	// However, this is CPU/GPU intensive when we are receiving frames in lockstep.
	if (current_format == FREENECT_VIDEO_YUV_RGB) {
		while (!got_depth && !got_rgb) {
			pthread_cond_wait(&gl_frame_cond, &gl_backbuf_mutex);
		}
	} else {
		while ((!got_depth || !got_rgb) && requested_format != current_format) {
			pthread_cond_wait(&gl_frame_cond, &gl_backbuf_mutex);
		}
	}

	if (requested_format != current_format) {
		pthread_mutex_unlock(&gl_backbuf_mutex);
		return;
	}

	uint8_t *tmp;
	if (got_depth) {
		tmp = depth_front;
		depth_front = depth_mid;
		depth_mid = tmp;
		got_depth = 0;
	}
	if (got_rgb) {
		tmp = rgb_front;
		rgb_front = rgb_mid;
		rgb_mid = tmp;
		got_rgb = 0;
	}

	pthread_mutex_unlock(&gl_backbuf_mutex);
        
        
       
	glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, depth_front);

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(0,0,0);
	glTexCoord2f(1, 0); glVertex3f(640,0,0);
	glTexCoord2f(1, 1); glVertex3f(640,480,0);
	glTexCoord2f(0, 1); glVertex3f(0,480,0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
	if (current_format == FREENECT_VIDEO_RGB || current_format == FREENECT_VIDEO_YUV_RGB)
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_front);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, rgb_front+640*4);

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(640,0,0);
	glTexCoord2f(1, 0); glVertex3f(1280,0,0);
	glTexCoord2f(1, 1); glVertex3f(1280,480,0);
	glTexCoord2f(0, 1); glVertex3f(640,480,0);
	glEnd();

	glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y)
{
	if (key == 27) {
		die = 1;
		pthread_join(freenect_thread, NULL);
		glutDestroyWindow(window);
		free(depth_mid);
		free(depth_front);
		free(rgb_back);
		free(rgb_mid);
		free(rgb_front);
		// Not pthread_exit because OSX leaves a thread lying around and doesn't exit
		exit(0);
	}
	if (key == 'w') {
		freenect_angle++;
		if (freenect_angle > 30) {
			freenect_angle = 30;
		}
	}
	if (key == 's') {
		freenect_angle = 0;
	}
	if (key == 'f') {
		if (requested_format == FREENECT_VIDEO_IR_8BIT)
			requested_format = FREENECT_VIDEO_RGB;
		else if (requested_format == FREENECT_VIDEO_RGB)
			requested_format = FREENECT_VIDEO_YUV_RGB;
		else
			requested_format = FREENECT_VIDEO_IR_8BIT;
	}
	if (key == 'x') {
		freenect_angle--;
		if (freenect_angle < -30) {
			freenect_angle = -30;
		}
	}
	if (key == '1') {
		freenect_set_led(f_dev,LED_GREEN);
	}
	if (key == '2') {
		freenect_set_led(f_dev,LED_RED);
	}
	if (key == '3') {
		freenect_set_led(f_dev,LED_YELLOW);
	}
	if (key == '4') {
		freenect_set_led(f_dev,LED_BLINK_GREEN);
	}
	if (key == '5') {
		// 5 is the same as 4
		freenect_set_led(f_dev,LED_BLINK_GREEN);
	}
	if (key == '6') {
		freenect_set_led(f_dev,LED_BLINK_RED_YELLOW);
	}
	if (key == '0') {
		freenect_set_led(f_dev,LED_OFF);
	}

    if (key == 'l') {
        stop_learning = !stop_learning;
    }
    if (key == 'o') {
        thresTMP -= 0.01;
    }
    if (key == 'p') {
        thresTMP += 0.01;
    }
    if (key == 'i') {
    }
    if (key == 'u') {
    }
    if (key == 'm') {
        meanshift = true;
    }

	freenect_set_tilt_degs(f_dev,freenect_angle);
}

void ReSizeGLScene(int Width, int Height)
{
	glViewport(0,0,Width,Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0, 1280, 480, 0, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void InitGL(int Width, int Height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
    glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_TEXTURE_2D);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_FLAT);

	glGenTextures(1, &gl_depth_tex);
	glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &gl_rgb_tex);
	glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	ReSizeGLScene(Width, Height);
}

void *gl_threadfunc(void *arg)
{
	printf("GL thread\n");

	glutInit(&g_argc, g_argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(1280, 480);
	glutInitWindowPosition(0, 0);

	window = glutCreateWindow("LibFreenect");

	glutDisplayFunc(&DrawGLScene);
	glutIdleFunc(&DrawGLScene);
	glutReshapeFunc(&ReSizeGLScene);
	glutKeyboardFunc(&keyPressed);

	InitGL(1280, 480);

	glutMainLoop();

	return NULL;
}

uint16_t t_gamma[2048];

/**
 * Set the center of mass of the different body parts.
 * @param src Source image matrix.
 * @param labMask Probability per joints.
 * @param labComs Center of mass of the joints.
 */
void setCOM (Mat src, 
             vector<Mat> labMask, 
             vector<Point>& labComs,
             vector<int>& labNums)
{
    Vec3b ncolor;

    for (int i = 0; i < src.rows; i++) {
        for(int j = 0; j < src.cols; j++) {
            ncolor = Vec3b(src.at<Vec3b>(i,j));
            for (int k = 1; k <= NUMBER_OF_LABELS; k++) {
                if (ncolor == Vec3b(color[k][2],
                                    color[k][1],
                                    color[k][0])) 
                {
                    if (labMask[k - 1].at<uchar>(i,j) >= int(0.60 * 255))
                    {                    
                        labNums[k - 1]++;
                        labComs[k - 1].x += i;
                        labComs[k - 1].y += j;
                    }
                }
            }
        }
    }   

    for (int i = 0; i < NUMBER_OF_LABELS; i++) {
        if (labNums[i] != 0) {
            labComs[i].x /= labNums[i];
            labComs[i].y /= labNums[i];
        }
    }
}

/**
 * Mean shift procedure that returns the center of mass of the joints.
 * @param src Source image matrix.
 * @param labMask Probability per joints.
 * @return Vector containing the centers of mass.
 */
vector<Rect> meanShiftProc (Mat src, vector<Mat> labMask)
{

    Rect com;
    vector<Rect> comVec;
    vector<Point> labComs (NUMBER_OF_LABELS, Point(0, 0));
    vector<int> labNums (NUMBER_OF_LABELS, 0);

    TermCriteria c = TermCriteria(CV_TERMCRIT_ITER + 
                                  CV_TERMCRIT_EPS, 
                                  MS_ITER, 
                                  MS_EP);

    // Calculates the centers of mass
    setCOM(src, labMask, labComs, labNums);

    com.x = 0;
    com.y = 0;
    com.width = MS_WINSIZE;
    com.height = MS_WINSIZE;

    // Applies mean shift to the joint point clouds.
    for (int lab = 0; lab < 6; lab++) {

        if (labNums[lab] == 0) {
            continue;
        }

        // setting center of the rectangle in the COM
        com.x = labComs[lab].x;
        com.y = labComs[lab].y;

        com.x -= com.width / 2;
        com.y -= com.height / 2;

        meanShift(labMask[lab], com, c);

        comVec.push_back(com);
    }
    
    return comVec;

}



void depth_cb(freenect_device *dev, 
              void *v_depth, 
              uint32_t timestamp,
              Mat& display,
              Mat& depthImg,
              Mat& mask)
{
	unsigned i;
    int j;
    int i_jump;
    int j_jump;

    float prob;
    Label lab;
    PixelInfo pi;
    KinectImage kimg;

    // Mean shift variables
    Rect com2;
    vector<Rect> comVec;

    Mat minimat(480 / PIXEL_JUMP, 
                640 / PIXEL_JUMP, 
                CV_8UC3, 
                CV_RGB(0,0,0));

    Mat probMask(480 / PIXEL_JUMP, 
                 640 / PIXEL_JUMP, 
                 CV_32FC1, 
                 Scalar(0));

    vector<Mat> labMask(NUMBER_OF_LABELS);

    // Initialize display window
    for (i = 0; i < 480; i++) {
        for (j = 0; j < 640; j++) {
            display.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
        }
    }

    // Init label mask matrix
    for (i = 0; i < NUMBER_OF_LABELS; i++) {
        labMask[i] = Mat::zeros(480 / PIXEL_JUMP, 
                                640 / PIXEL_JUMP, 
                                CV_8UC1);
    }
    
    kimg = KinectImage((unsigned short*) v_depth);


    // Iterate trough the mini image and classify it
    for (i = 0; i < (480 / PIXEL_JUMP); i++) {
        for (j = 0; j < (640 / PIXEL_JUMP); j++) {
            i_jump = i * PIXEL_JUMP;
            j_jump = j * PIXEL_JUMP;
            if (mask.at<uchar>(i_jump, j_jump) != 0) {
                pi = PixelInfo(i_jump, j_jump);
                pi.id = 0;
                lab = g_a.predict((Image*)&kimg, pi, prob);
                if (prob > thresTMP) {
                    minimat.at<Vec3b>(i, j) = Vec3b(color[lab][2], 
                                                    color[lab][1], 
                                                    color[lab][0]);
                    labMask[lab - 1].at<uchar>(i, j) = (unsigned char)(prob * 255);
                    probMask.at<float>(i,j) = prob;
                }
            }
        }
    }

    // Flood fill BFS procedure to fix the image
    flood_fill_proc(minimat, 
                    labMask, 
                    FLOOD_FILL_ITER, 
                    FLOOD_FILL_THRESHOLD);

        
    //Smooth probs
    Mat temp_mask;
    for (i = 0; i < NUMBER_OF_LABELS; i++) {
        labMask[i].copyTo(temp_mask);
        refineSegments2(labMask[i], temp_mask);
        
    }
    
    // Mean shift to track the diferent body parts
 //   comVec = meanShiftProc (minimat, labMask);
    

    // Restore image size
    for (i = 0; i < (480 / PIXEL_JUMP); i++) {
        for (j = 0; j < (640 / PIXEL_JUMP); j++) {
            i_jump = i * PIXEL_JUMP;
            j_jump = j * PIXEL_JUMP;
            display.at<Vec3b>(i_jump, j_jump) = minimat.at<Vec3b>(i, j);
        }
    }

    // Draw meanshift rectangles
    for (i = 0; i < comVec.size(); i++) {
        //com = comVec[i];
        com2 =  comVec[i];
        com2.y *= PIXEL_JUMP;   
        com2.x *= PIXEL_JUMP;
        com2.height *= PIXEL_JUMP;
        com2.width *= PIXEL_JUMP;
   /*     rectangle(display, 
                  Point(com2.y, com2.x), 
                  Point(com2.y + com2.height, com2.x + com2.width), 
                  Scalar(255,255,255));

        display.at<Vec3b>(com2.x, com2.y) = Vec3b(255, 255, 255); */
    }

    if (meanshift) {

        namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
        imshow("Display window", minimat);
        waitKey(0);

        namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
        imshow("Display window", labMask[0]);
        waitKey(0);

        namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
        imshow("Display window", labMask[1]);
        waitKey(0);

        namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
        imshow("Display window", labMask[2]);
        waitKey(0);

        namedWindow( "Display window2", CV_WINDOW_AUTOSIZE );
        imshow("Display window", labMask[3]);
        waitKey(0);

        namedWindow( "Display window2", CV_WINDOW_AUTOSIZE );
        imshow("Display window", labMask[4]);
        waitKey(0);

        namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
        imshow("Display window", labMask[5]);
        waitKey(0);
        

    }
    printf("\n\nTHRESHOLD SELECTED %f\n", thresTMP);
 }




void learn_background(freenect_device *dev, 
                      void *v_depth, 
                      uint32_t timestamp) 
{
    int i;
    int j;
    static int num_frames = 0;
    bool all_zero = true;

    Mat display(480, 640, CV_8UC3, depth_mid);
    Mat depthImg(480, 640, CV_16U, v_depth);
    Mat mask (480, 640, CV_8U);
    Mat learned(480, 640, CV_8UC3, Scalar(255,0,0));

	pthread_mutex_lock(&gl_backbuf_mutex);

    BGModel(depthImg, mask, -1);
    refineSegments(depthImg, mask);

    // Background subtraction
    if (stop_learning) {
        BGModel.nframes = 10000;
        BGModel.history = 10000;
        depth_cb(dev, v_depth, timestamp, display, depthImg, mask);
    }
    else {


        // See if the background is learned
        for (i = 0; i < depthImg.rows; i++) {
            for (j = 0; j < depthImg.cols; j++) {
                if (depthImg.at<float>(i,j) != 0) {
                    num_frames = 0;
                    all_zero = false;
                    i = depthImg.rows;
                    break;
                }
            }
        }
        
        if (all_zero) {
            if (num_frames == 30) {
                cout << "Background Learned" << endl;
                sleep(1);
                stop_learning = true;
            }
            cout << "Learning background, frame: " << num_frames << endl;
            num_frames ++;
        }
    }

    got_depth++;
	pthread_cond_signal(&gl_frame_cond);
	pthread_mutex_unlock(&gl_backbuf_mutex);

}

void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp)
{
	pthread_mutex_lock(&gl_backbuf_mutex);

	// swap buffers
	assert (rgb_back == rgb);
	rgb_back = rgb_mid;
	freenect_set_video_buffer(dev, rgb_back);
	rgb_mid = (uint8_t*)rgb;

	got_rgb++;
	pthread_cond_signal(&gl_frame_cond);
	pthread_mutex_unlock(&gl_backbuf_mutex);
}

void *freenect_threadfunc(void *arg)
{
	int accelCount = 0;

	freenect_set_tilt_degs(f_dev,freenect_angle);
	freenect_set_led(f_dev,LED_RED);
//	freenect_set_depth_callback(f_dev, depth_cb);
	freenect_set_depth_callback(f_dev, learn_background);
	freenect_set_video_callback(f_dev, rgb_cb);
	freenect_set_video_mode(f_dev, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, current_format));
	freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_MM));
	freenect_set_video_buffer(f_dev, rgb_back);

	freenect_start_depth(f_dev);
	freenect_start_video(f_dev);

	printf("'w'-tilt up, 's'-level, 'x'-tilt down, '0'-'6'-select LED mode, 'f'-video format\n");

	while (!die && freenect_process_events(f_ctx) >= 0) {
		//Throttle the text output
		if (accelCount++ >= 2000)
		{
			accelCount = 0;
			freenect_raw_tilt_state* state;
			freenect_update_tilt_state(f_dev);
			state = freenect_get_tilt_state(f_dev);
			double dx,dy,dz;
			freenect_get_mks_accel(state, &dx, &dy, &dz);
			printf("\r raw acceleration: %4d %4d %4d  mks acceleration: %4f %4f %4f", state->accelerometer_x, state->accelerometer_y, state->accelerometer_z, dx, dy, dz);
			fflush(stdout);
		}

		if (requested_format != current_format) {
			freenect_stop_video(f_dev);
			freenect_set_video_mode(f_dev, freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, requested_format));
			freenect_start_video(f_dev);
			current_format = requested_format;
		}
	}

	printf("\nshutting down streams...\n");

	freenect_stop_depth(f_dev);
	freenect_stop_video(f_dev);

	freenect_close_device(f_dev);
	freenect_shutdown(f_ctx);

	printf("-- done!\n");
	return NULL;
}

/**
 * Parse the input parameter file and fill the 
 * trainParams structure.
 * @param fileParam Input parameter filename.
 * @param train Indicates if the program is set to train or classify.
 * @param tp trainParams structure to be filled.
 */
void parseParameterFile (string fileParam, trainParams& tp) 
{
    FILE *fp;
    NumRange o;
    NumRange t;

    char imgDir[50];

    if ((fp = fopen(fileParam.c_str(), "r")) == NULL) {
        printf ("Error opening parameters file\n");
    }

    fscanf(fp, "offsetStart: %d\n", &o.start);
    fscanf(fp, "offsetEnd: %d\n", &o.end);
    fscanf(fp, "rangeStart: %d\n", &t.start);
    fscanf(fp, "rangetEnd: %d\n", &t.end);
    fscanf(fp, "treeNum: %d\n", &tp.treeNum);
    fscanf(fp, "labelNum: %d\n", &tp.labelNum);
    fscanf(fp, "imgDir: %s\n", imgDir);
    fscanf(fp, "maxDepth: %d\n", &tp.maxDepth);
    fscanf(fp, "minSampleCount: %d\n", &tp.minSampleCount);
    fscanf(fp, "samplePixelNum: %d\n", &tp.samplePixelNum);
    fscanf(fp, "offsetNum: %d\n", &tp.offsetNum);
    fscanf(fp, "thresholdNum: %d\n", &tp.thresholdNum);
    tp.imgDir = string (imgDir);
    tp.offsetRange = o;
    tp.thresholdRange = t;
}


int main(int argc, char **argv)
{
    ParseArgs p;
    
    // Load filenames and params
    p.parseCommandLine(argc, argv);

    srand ( time(NULL) );

    trainParams tp;

    string loadTreePath;
    string saveTreePath;
    string imagePath;

    g_a = RandomForest();
    parseParameterFile (p.fileParam, tp);
    double cla = 0.0;
    double ave = 0.0;
    stringstream strdir;
    
    DIR *pdir = NULL;
    struct dirent *pent = NULL;
    int classifImageNum = 0;
    double ti = 0.0;

    switch (p.mode) {
        case TRAIN:
            
            ti = takeInitialTime();

            saveTreePath = p.treePath;

            MPI_Init (&argc, &argv);

            int rank;
            MPI_Comm_rank (MPI_COMM_WORLD, &rank);
            
            g_a.trainForest(tp);

            MPI_Finalize();

            if (rank == 0) {
                g_a.writeForest(saveTreePath);
            }
            
            takeFinalTime(ti);

            exit(0);
            break;

        case CLASSIFY:
            imagePath = p.imgPath;
            loadTreePath = p.treePath;

            g_a.loadForest(tp.treeNum, tp.labelNum, loadTreePath);

            pdir = opendir (imagePath.c_str());
            
            if (pdir == NULL) {
                printf ("Error! directory not initialized correctly\n");
                exit(EXIT_FAILURE);
            } 

            while ((pent = readdir(pdir))) {
                if (pent == NULL) {
                    printf("Error: could not read directory\n");
                }       

                strdir << imagePath << "/" << pent -> d_name;
                
                cout << "image: " << strdir.str() << endl;

                // Checking extension
                if ((strdir.str()).substr((strdir.str()).size() - 5, 5) == ".simg") {
         
                    TrainImage ti(strdir.str());
                
                    cla = g_a.testClassification(ti) * 100;

                    ave += cla;

                    printf("Classification %f%%\n", cla);
                    classifImageNum++;
                }
                strdir.flush();
                strdir.str("");
            }

            ave /= classifImageNum;

            printf("Average %f%%\n", ave);

            exit(0);
  
            break;
        case RUN:

            loadTreePath = p.treePath;
            g_a.loadForest(tp.treeNum, tp.labelNum, loadTreePath);

            break;

        default:
            printf("Mode not selected\n");
            exit(EXIT_FAILURE);
            break;
    }

	int res;

	depth_mid = (uint8_t*)malloc(640*480*3);
	depth_front = (uint8_t*)malloc(640*480*3);
	rgb_back = (uint8_t*)malloc(640*480*3);
	rgb_mid = (uint8_t*)malloc(640*480*3);
	rgb_front = (uint8_t*)malloc(640*480*3);

	printf("Kinect camera test\n");

	int i;
	for (i=0; i<2048; i++) {
		float v = i/2048.0;
		v = powf(v, 3)* 6;
		t_gamma[i] = v*6*256;
	}

	g_argc = argc;
	g_argv = argv;

	if (freenect_init(&f_ctx, NULL) < 0) {
		printf("freenect_init() failed\n");
		return 1;
	}

	freenect_set_log_level(f_ctx, FREENECT_LOG_DEBUG);
	freenect_select_subdevices(f_ctx, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));

	int nr_devices = freenect_num_devices (f_ctx);
	printf ("Number of devices found: %d\n", nr_devices);

	int user_device_number = 0;
	if (argc > 1)
		user_device_number = atoi(argv[1]);

	if (nr_devices < 1)
		return 1;

	if (freenect_open_device(f_ctx, &f_dev, user_device_number) < 0) {
		printf("Could not open device\n");
		return 1;
	}

	res = pthread_create(&freenect_thread, NULL, freenect_threadfunc, NULL);
	if (res) {
		printf("pthread_create failed\n");
		return 1;
	}

	// OS X requires GLUT to run on the main thread
	gl_threadfunc(NULL);
    //*/

	return 0;
}
