#include "libfreenect.h"
#include "libfreenect-registration.h"
#include <cmath>
#include <cstdio>
#include <pthread.h>

# include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


#define MAX_NUM_DEV 3

using namespace std;
using namespace cv;

int outputs[MAX_NUM_DEV];

pthread_t freenect_thread[MAX_NUM_DEV];

freenect_context* f_ctx;
freenect_device* f_dev[MAX_NUM_DEV];

/**
 *  Number of devices detected.
 */
int nr_devices;

/**
 *  If this variable is set to one the program terminates.
 */
volatile int term = 0;

void depth_cb(freenect_device* dev, void* v_depth, uint32_t timestamp) {
    int i;
    int j;
    uint16_t *depth = (uint16_t*)v_depth;

    IplImage *img = cvCreateImage(cvSize(640, 480),IPL_DEPTH_8U,3);
    cvZero(img);

    for (i = 0; i < nr_devices; i++) {
        if (f_dev[i] == dev) {
            outputs[i] = depth[155840];
        }
        //printf("Kinect %d: %d  ", i, outputs[i]);
    }
    for (i = 0; i < 480; i++) {
        for (j = 0; j < 640; j++) {
            cvSet2D(img,i,j,CV_RGB(0,0,0));
        }
    }
    //printf("\n");
    cvSaveImage("prueba.jpg", img);
    //cvNamedWindow("main",CV_WINDOW_AUTOSIZE);
    //cvShowImage("main",img);
}

void *freenect_threadfunc(void* id) {

    int i;
    int accelCount;

    i = (int) id;

    printf("Iniciando dispositivo %d\n", i);

    freenect_set_depth_callback(f_dev[i], depth_cb);
    freenect_set_depth_mode(f_dev[i], freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_MM));
    freenect_start_depth(f_dev[i]);

    while (!term && freenect_process_events(f_ctx) >= 0) {
        if (accelCount++ >= 2000)
        {
            accelCount = 0;
            freenect_raw_tilt_state* state;
            freenect_update_tilt_state(f_dev[i]);
            state = freenect_get_tilt_state(f_dev[i]);
        }
    }

    printf("Terminando dispositivo %d\n", i);

    return NULL;
}

int main (int argc, char** argv) {

    int i;
    int res;

    if (freenect_init(&f_ctx, NULL) < 0) {
        printf("freenect_init explode!\n");
        return 1;
    }

    nr_devices = freenect_num_devices (f_ctx);

    printf("Number of devices found: %d\n", nr_devices);

    /**
     *  Open Kinect devices detected.
     */
    for (i = 0; i < nr_devices; i++) {
        if (freenect_open_device(f_ctx, &f_dev[i], i) < 0) {
            printf("Could not open device\n");
            return 1;
        }
    }

    /**
     *  Create the device threads.
     */
    for (i = 0; i < nr_devices; i++) {
        res = pthread_create(&freenect_thread[i], NULL, freenect_threadfunc, (void*)i);
        if (res != 0) {
            printf("pthread_create failed\n");
            return 1;
        }
    }

    while (true);

    for (i = 0; i < nr_devices; i++) {
        freenect_stop_depth(f_dev[i]);
        freenect_close_device(f_dev[i]);
    }

    return 0;
}
