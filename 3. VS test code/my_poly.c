#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<stdint.h>
#include<avr/io.h>
/**
    * Compute kernel between feature vector and support vector.
    * Kernel type: poly
*/

float compute_kernel(float* x, ...) {
    float kernel = 0.0;
    /*
    for (uint16_t i = 0; i < 9; i++) {
        kernel += x[i] * va_arg(w, double);
    }
    */
    float compute_kernel(float* x, float data1, float data2, float data3, float data4, float data5, float data6, float data7, float data8, float data9) {
        float kernel = 0.0;
        kernel += x[0] * data1;
        kernel += x[1] * data2;
        kernel += x[2] * data3;
        kernel += x[3] * data4;
        kernel += x[4] * data5;
        kernel += x[5] * data6;
        kernel += x[6] * data7;
        kernel += x[7] * data8;
        kernel += x[8] * data9;

    return pow((0.001 * kernel) + 0.0, 3);
}

int predict(float* x) {
    float kernels[61] = { 0 };
    kernels[0] = compute_kernel(x, 43.0, 0.0, 132.0, 341.0, 1.0, 0.0, 136.0, 1.0, 0.0);
    kernels[1] = compute_kernel(x, 62.0, 0.0, 130.0, 263.0, 0.0, 1.0, 97.0, 0.0, 1.0);
    kernels[2] = compute_kernel(x, 65.0, 1.0, 135.0, 254.0, 0.0, 0.0, 127.0, 0.0, 1.0);
    kernels[3] = compute_kernel(x, 48.0, 1.0, 130.0, 256.0, 1.0, 0.0, 150.0, 1.0, 2.0);
    kernels[4] = compute_kernel(x, 55.0, 1.0, 140.0, 217.0, 0.0, 1.0, 111.0, 1.0, 0.0);
    kernels[5] = compute_kernel(x, 65.0, 1.0, 138.0, 282.0, 1.0, 0.0, 174.0, 0.0, 1.0);
    kernels[6] = compute_kernel(x, 56.0, 0.0, 200.0, 288.0, 1.0, 0.0, 133.0, 1.0, 2.0);
    kernels[7] = compute_kernel(x, 54.0, 1.0, 110.0, 239.0, 0.0, 1.0, 126.0, 1.0, 1.0);
    kernels[8] = compute_kernel(x, 35.0, 1.0, 120.0, 198.0, 0.0, 1.0, 130.0, 1.0, 0.0);
    kernels[9] = compute_kernel(x, 59.0, 1.0, 170.0, 288.0, 0.0, 0.0, 159.0, 0.0, 0.0);
    kernels[10] = compute_kernel(x, 47.0, 1.0, 108.0, 243.0, 0.0, 1.0, 152.0, 0.0, 0.0);
    kernels[11] = compute_kernel(x, 51.0, 1.0, 140.0, 299.0, 0.0, 1.0, 173.0, 1.0, 0.0);
    kernels[12] = compute_kernel(x, 58.0, 1.0, 125.0, 300.0, 0.0, 0.0, 171.0, 0.0, 2.0);
    kernels[13] = compute_kernel(x, 60.0, 1.0, 140.0, 293.0, 0.0, 0.0, 170.0, 0.0, 2.0);
    kernels[14] = compute_kernel(x, 35.0, 1.0, 126.0, 282.0, 0.0, 0.0, 156.0, 1.0, 0.0);
    kernels[15] = compute_kernel(x, 59.0, 0.0, 174.0, 249.0, 0.0, 1.0, 143.0, 1.0, 0.0);
    kernels[16] = compute_kernel(x, 64.0, 1.0, 145.0, 212.0, 0.0, 0.0, 132.0, 0.0, 2.0);
    kernels[17] = compute_kernel(x, 48.0, 1.0, 124.0, 274.0, 0.0, 0.0, 166.0, 0.0, 0.0);
    kernels[18] = compute_kernel(x, 54.0, 1.0, 192.0, 283.0, 0.0, 0.0, 195.0, 0.0, 1.0);
    kernels[19] = compute_kernel(x, 59.0, 1.0, 160.0, 273.0, 0.0, 0.0, 125.0, 0.0, 0.0);
    kernels[20] = compute_kernel(x, 62.0, 0.0, 150.0, 244.0, 0.0, 1.0, 154.0, 1.0, 0.0);
    kernels[21] = compute_kernel(x, 38.0, 1.0, 120.0, 231.0, 0.0, 1.0, 182.0, 1.0, 0.0);
    kernels[22] = compute_kernel(x, 52.0, 1.0, 112.0, 230.0, 0.0, 1.0, 160.0, 0.0, 1.0);
    kernels[23] = compute_kernel(x, 63.0, 0.0, 108.0, 269.0, 0.0, 1.0, 169.0, 1.0, 2.0);
    kernels[24] = compute_kernel(x, 54.0, 1.0, 110.0, 206.0, 0.0, 0.0, 108.0, 1.0, 1.0);
    kernels[25] = compute_kernel(x, 66.0, 1.0, 112.0, 212.0, 0.0, 0.0, 132.0, 1.0, 1.0);
    kernels[26] = compute_kernel(x, 49.0, 1.0, 118.0, 149.0, 0.0, 0.0, 126.0, 0.0, 3.0);
    kernels[27] = compute_kernel(x, 46.0, 1.0, 120.0, 249.0, 0.0, 0.0, 144.0, 0.0, 0.0);
    kernels[28] = compute_kernel(x, 61.0, 1.0, 134.0, 234.0, 0.0, 1.0, 145.0, 0.0, 2.0);
    kernels[29] = compute_kernel(x, 58.0, 1.0, 100.0, 234.0, 0.0, 1.0, 156.0, 0.0, 1.0);
    kernels[30] = compute_kernel(x, 71.0, 0.0, 160.0, 302.0, 0.0, 1.0, 162.0, 0.0, 2.0);
    kernels[31] = compute_kernel(x, 51.0, 1.0, 110.0, 175.0, 0.0, 1.0, 123.0, 0.0, 0.0);
    kernels[32] = compute_kernel(x, 54.0, 1.0, 125.0, 273.0, 0.0, 0.0, 152.0, 0.0, 1.0);
    kernels[33] = compute_kernel(x, 51.0, 1.0, 125.0, 213.0, 0.0, 0.0, 125.0, 1.0, 1.0);
    kernels[34] = compute_kernel(x, 46.0, 0.0, 142.0, 177.0, 0.0, 0.0, 160.0, 1.0, 0.0);
    kernels[35] = compute_kernel(x, 65.0, 0.0, 160.0, 360.0, 0.0, 0.0, 151.0, 0.0, 0.0);
    kernels[36] = compute_kernel(x, 45.0, 1.0, 104.0, 208.0, 0.0, 0.0, 148.0, 1.0, 0.0);
    kernels[37] = compute_kernel(x, 39.0, 1.0, 140.0, 321.0, 0.0, 0.0, 182.0, 0.0, 0.0);
    kernels[38] = compute_kernel(x, 47.0, 1.0, 138.0, 257.0, 0.0, 0.0, 156.0, 0.0, 0.0);
    kernels[39] = compute_kernel(x, 66.0, 1.0, 120.0, 302.0, 0.0, 0.0, 151.0, 0.0, 0.0);
    kernels[40] = compute_kernel(x, 62.0, 1.0, 130.0, 231.0, 0.0, 1.0, 146.0, 0.0, 3.0);
    kernels[41] = compute_kernel(x, 41.0, 1.0, 135.0, 203.0, 0.0, 1.0, 132.0, 0.0, 0.0);
    kernels[42] = compute_kernel(x, 51.0, 1.0, 100.0, 222.0, 0.0, 1.0, 143.0, 1.0, 0.0);
    kernels[43] = compute_kernel(x, 54.0, 1.0, 120.0, 258.0, 0.0, 0.0, 147.0, 0.0, 0.0);
    kernels[44] = compute_kernel(x, 51.0, 1.0, 94.0, 227.0, 0.0, 1.0, 154.0, 1.0, 1.0);
    kernels[45] = compute_kernel(x, 51.0, 1.0, 140.0, 261.0, 0.0, 0.0, 186.0, 1.0, 0.0);
    kernels[46] = compute_kernel(x, 59.0, 1.0, 140.0, 221.0, 0.0, 1.0, 164.0, 1.0, 0.0);
    kernels[47] = compute_kernel(x, 58.0, 1.0, 105.0, 240.0, 0.0, 0.0, 154.0, 1.0, 0.0);
    kernels[48] = compute_kernel(x, 41.0, 1.0, 112.0, 250.0, 0.0, 1.0, 179.0, 0.0, 0.0);
    kernels[49] = compute_kernel(x, 68.0, 1.0, 118.0, 277.0, 0.0, 1.0, 151.0, 0.0, 1.0);
    kernels[50] = compute_kernel(x, 52.0, 1.0, 138.0, 223.0, 0.0, 1.0, 169.0, 0.0, 4.0);
    kernels[51] = compute_kernel(x, 54.0, 0.0, 132.0, 288.0, 1.0, 0.0, 159.0, 1.0, 1.0);
    kernels[52] = compute_kernel(x, 53.0, 1.0, 142.0, 226.0, 0.0, 0.0, 111.0, 1.0, 0.0);
    kernels[53] = compute_kernel(x, 52.0, 1.0, 108.0, 233.0, 1.0, 1.0, 147.0, 0.0, 3.0);
    kernels[54] = compute_kernel(x, 43.0, 1.0, 130.0, 315.0, 0.0, 1.0, 162.0, 0.0, 1.0);
    kernels[55] = compute_kernel(x, 42.0, 1.0, 148.0, 244.0, 0.0, 0.0, 178.0, 0.0, 2.0);
    kernels[56] = compute_kernel(x, 59.0, 1.0, 178.0, 270.0, 0.0, 0.0, 145.0, 0.0, 0.0);
    kernels[57] = compute_kernel(x, 68.0, 0.0, 120.0, 211.0, 0.0, 0.0, 115.0, 0.0, 0.0);
    kernels[58] = compute_kernel(x, 69.0, 1.0, 160.0, 234.0, 1.0, 0.0, 131.0, 0.0, 1.0);
    kernels[59] = compute_kernel(x, 45.0, 0.0, 138.0, 236.0, 0.0, 0.0, 152.0, 1.0, 0.0);
    kernels[60] = compute_kernel(x, 64.0, 0.0, 180.0, 325.0, 0.0, 1.0, 154.0, 1.0, 0.0);
    float decision = 1.246732033997;
    decision = decision - (+kernels[0] * -0.25792644204 + kernels[1] * -0.086975602818 + kernels[2] * -1.0 + kernels[3] * -0.34357693487 + kernels[4] * -1.0 + kernels[5] * -1.0 + kernels[6] * -0.177101524746 + kernels[7] * -1.0 + kernels[8] * -1.0 + kernels[9] * -1.0 + kernels[10] * -1.0 + kernels[11] * -0.7693941541 + kernels[12] * -0.312119665282 + kernels[13] * -1.0 + kernels[14] * -0.040204636388 + kernels[15] * -0.505668486186 + kernels[16] * -0.417175939698 + kernels[17] * -1.0 + kernels[18] * -0.29917720442 + kernels[19] * -1.0 + kernels[20] * -1.0 + kernels[21] * -1.0 + kernels[22] * -1.0 + kernels[23] * -0.820318685885 + kernels[24] * -0.17240477281 + kernels[25] * -0.490545888904 + kernels[26] * -1.0 + kernels[27] * -1.0 + kernels[28] * -1.0 + kernels[29] * -1.0);
    decision = decision - (+kernels[30] * 0.749080958054 + kernels[31] * 0.456240568686 + kernels[32] * 1.0 + kernels[33] * 1.0 + kernels[34] * 0.137235903234 + kernels[35] * 0.089366605723 + kernels[36] * 0.882911205533 + kernels[37] * 0.45730649354 + kernels[38] * 1.0 + kernels[39] * 0.235330514468 + kernels[40] * 1.0 + kernels[41] * 1.0 + kernels[42] * 1.0 + kernels[43] * 1.0 + kernels[44] * 1.0 + kernels[45] * 0.804996797786 + kernels[46] * 0.598912778962 + kernels[47] * 1.0 + kernels[48] * 0.705163893817 + kernels[49] * 1.0 + kernels[50] * 0.771768239421 + kernels[51] * 0.357337893416 + kernels[52] * 1.0 + kernels[53] * 1.0 + kernels[54] * 0.34984886983 + kernels[55] * 0.143375332417 + kernels[56] * 1.0 + kernels[57] * 0.02446405933 + kernels[58] * 1.0 + kernels[59] * 0.013954298553 + kernels[60] * 0.915295525377);

    return decision > 0 ? 0 : 1;
}

/**
    * Convert class idx to readable name
*/
const char* idxToLabel(uint8_t classIdx) {
    switch (classIdx) {
    case 0:
        return "class1";
    case 1:
        return "class2";
    default:
        return "Houston we have a problem";
    }
}

/**
* Predict readable class name
*/

const char* predictLabel(float* x) {
    return idxToLabel(predict(x));
}


void init_port() {
    DDRF = 0xf0;
    PORTF = 0x00;
}

int main() {
    init_port();

    float x_sample[] = {};

}