#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class SVM {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        float kernels[62] = { 0 };
                        float decisions[1] = { 0 };
                        int votes[2] = { 0 };
                        kernels[0] = compute_kernel(x,   43.0  , 0.0  , 132.0  , 341.0  , 1.0  , 0.0  , 136.0  , 1.0  , 0.0 );
                        kernels[1] = compute_kernel(x,   62.0  , 0.0  , 130.0  , 263.0  , 0.0  , 1.0  , 97.0  , 0.0  , 1.0 );
                        kernels[2] = compute_kernel(x,   65.0  , 1.0  , 135.0  , 254.0  , 0.0  , 0.0  , 127.0  , 0.0  , 1.0 );
                        kernels[3] = compute_kernel(x,   48.0  , 1.0  , 130.0  , 256.0  , 1.0  , 0.0  , 150.0  , 1.0  , 2.0 );
                        kernels[4] = compute_kernel(x,   63.0  , 0.0  , 150.0  , 407.0  , 0.0  , 0.0  , 154.0  , 0.0  , 3.0 );
                        kernels[5] = compute_kernel(x,   65.0  , 1.0  , 138.0  , 282.0  , 1.0  , 0.0  , 174.0  , 0.0  , 1.0 );
                        kernels[6] = compute_kernel(x,   54.0  , 1.0  , 110.0  , 239.0  , 0.0  , 1.0  , 126.0  , 1.0  , 1.0 );
                        kernels[7] = compute_kernel(x,   70.0  , 1.0  , 145.0  , 174.0  , 0.0  , 1.0  , 125.0  , 1.0  , 0.0 );
                        kernels[8] = compute_kernel(x,   35.0  , 1.0  , 120.0  , 198.0  , 0.0  , 1.0  , 130.0  , 1.0  , 0.0 );
                        kernels[9] = compute_kernel(x,   59.0  , 1.0  , 170.0  , 288.0  , 0.0  , 0.0  , 159.0  , 0.0  , 0.0 );
                        kernels[10] = compute_kernel(x,   47.0  , 1.0  , 108.0  , 243.0  , 0.0  , 1.0  , 152.0  , 0.0  , 0.0 );
                        kernels[11] = compute_kernel(x,   51.0  , 1.0  , 140.0  , 299.0  , 0.0  , 1.0  , 173.0  , 1.0  , 0.0 );
                        kernels[12] = compute_kernel(x,   58.0  , 1.0  , 125.0  , 300.0  , 0.0  , 0.0  , 171.0  , 0.0  , 2.0 );
                        kernels[13] = compute_kernel(x,   60.0  , 1.0  , 140.0  , 293.0  , 0.0  , 0.0  , 170.0  , 0.0  , 2.0 );
                        kernels[14] = compute_kernel(x,   35.0  , 1.0  , 126.0  , 282.0  , 0.0  , 0.0  , 156.0  , 1.0  , 0.0 );
                        kernels[15] = compute_kernel(x,   59.0  , 0.0  , 174.0  , 249.0  , 0.0  , 1.0  , 143.0  , 1.0  , 0.0 );
                        kernels[16] = compute_kernel(x,   64.0  , 1.0  , 145.0  , 212.0  , 0.0  , 0.0  , 132.0  , 0.0  , 2.0 );
                        kernels[17] = compute_kernel(x,   48.0  , 1.0  , 124.0  , 274.0  , 0.0  , 0.0  , 166.0  , 0.0  , 0.0 );
                        kernels[18] = compute_kernel(x,   54.0  , 1.0  , 192.0  , 283.0  , 0.0  , 0.0  , 195.0  , 0.0  , 1.0 );
                        kernels[19] = compute_kernel(x,   59.0  , 1.0  , 160.0  , 273.0  , 0.0  , 0.0  , 125.0  , 0.0  , 0.0 );
                        kernels[20] = compute_kernel(x,   62.0  , 0.0  , 150.0  , 244.0  , 0.0  , 1.0  , 154.0  , 1.0  , 0.0 );
                        kernels[21] = compute_kernel(x,   38.0  , 1.0  , 120.0  , 231.0  , 0.0  , 1.0  , 182.0  , 1.0  , 0.0 );
                        kernels[22] = compute_kernel(x,   66.0  , 0.0  , 178.0  , 228.0  , 1.0  , 1.0  , 165.0  , 1.0  , 2.0 );
                        kernels[23] = compute_kernel(x,   52.0  , 1.0  , 112.0  , 230.0  , 0.0  , 1.0  , 160.0  , 0.0  , 1.0 );
                        kernels[24] = compute_kernel(x,   63.0  , 0.0  , 108.0  , 269.0  , 0.0  , 1.0  , 169.0  , 1.0  , 2.0 );
                        kernels[25] = compute_kernel(x,   66.0  , 1.0  , 112.0  , 212.0  , 0.0  , 0.0  , 132.0  , 1.0  , 1.0 );
                        kernels[26] = compute_kernel(x,   49.0  , 1.0  , 118.0  , 149.0  , 0.0  , 0.0  , 126.0  , 0.0  , 3.0 );
                        kernels[27] = compute_kernel(x,   46.0  , 1.0  , 120.0  , 249.0  , 0.0  , 0.0  , 144.0  , 0.0  , 0.0 );
                        kernels[28] = compute_kernel(x,   61.0  , 1.0  , 134.0  , 234.0  , 0.0  , 1.0  , 145.0  , 0.0  , 2.0 );
                        kernels[29] = compute_kernel(x,   58.0  , 1.0  , 100.0  , 234.0  , 0.0  , 1.0  , 156.0  , 0.0  , 1.0 );
                        kernels[30] = compute_kernel(x,   71.0  , 0.0  , 160.0  , 302.0  , 0.0  , 1.0  , 162.0  , 0.0  , 2.0 );
                        kernels[31] = compute_kernel(x,   65.0  , 0.0  , 140.0  , 417.0  , 1.0  , 0.0  , 157.0  , 0.0  , 1.0 );
                        kernels[32] = compute_kernel(x,   54.0  , 1.0  , 125.0  , 273.0  , 0.0  , 0.0  , 152.0  , 0.0  , 1.0 );
                        kernels[33] = compute_kernel(x,   51.0  , 1.0  , 125.0  , 213.0  , 0.0  , 0.0  , 125.0  , 1.0  , 1.0 );
                        kernels[34] = compute_kernel(x,   65.0  , 0.0  , 160.0  , 360.0  , 0.0  , 0.0  , 151.0  , 0.0  , 0.0 );
                        kernels[35] = compute_kernel(x,   51.0  , 0.0  , 140.0  , 308.0  , 0.0  , 0.0  , 142.0  , 0.0  , 1.0 );
                        kernels[36] = compute_kernel(x,   45.0  , 1.0  , 104.0  , 208.0  , 0.0  , 0.0  , 148.0  , 1.0  , 0.0 );
                        kernels[37] = compute_kernel(x,   47.0  , 1.0  , 138.0  , 257.0  , 0.0  , 0.0  , 156.0  , 0.0  , 0.0 );
                        kernels[38] = compute_kernel(x,   53.0  , 0.0  , 128.0  , 216.0  , 0.0  , 0.0  , 115.0  , 0.0  , 0.0 );
                        kernels[39] = compute_kernel(x,   62.0  , 1.0  , 130.0  , 231.0  , 0.0  , 1.0  , 146.0  , 0.0  , 3.0 );
                        kernels[40] = compute_kernel(x,   52.0  , 1.0  , 134.0  , 201.0  , 0.0  , 1.0  , 158.0  , 0.0  , 1.0 );
                        kernels[41] = compute_kernel(x,   41.0  , 1.0  , 135.0  , 203.0  , 0.0  , 1.0  , 132.0  , 0.0  , 0.0 );
                        kernels[42] = compute_kernel(x,   51.0  , 1.0  , 100.0  , 222.0  , 0.0  , 1.0  , 143.0  , 1.0  , 0.0 );
                        kernels[43] = compute_kernel(x,   51.0  , 1.0  , 94.0  , 227.0  , 0.0  , 1.0  , 154.0  , 1.0  , 1.0 );
                        kernels[44] = compute_kernel(x,   51.0  , 1.0  , 140.0  , 261.0  , 0.0  , 0.0  , 186.0  , 1.0  , 0.0 );
                        kernels[45] = compute_kernel(x,   59.0  , 1.0  , 140.0  , 221.0  , 0.0  , 1.0  , 164.0  , 1.0  , 0.0 );
                        kernels[46] = compute_kernel(x,   58.0  , 1.0  , 105.0  , 240.0  , 0.0  , 0.0  , 154.0  , 1.0  , 0.0 );
                        kernels[47] = compute_kernel(x,   67.0  , 0.0  , 115.0  , 564.0  , 0.0  , 0.0  , 160.0  , 0.0  , 0.0 );
                        kernels[48] = compute_kernel(x,   68.0  , 1.0  , 118.0  , 277.0  , 0.0  , 1.0  , 151.0  , 0.0  , 1.0 );
                        kernels[49] = compute_kernel(x,   57.0  , 1.0  , 132.0  , 207.0  , 0.0  , 1.0  , 168.0  , 1.0  , 0.0 );
                        kernels[50] = compute_kernel(x,   52.0  , 1.0  , 138.0  , 223.0  , 0.0  , 1.0  , 169.0  , 0.0  , 4.0 );
                        kernels[51] = compute_kernel(x,   54.0  , 0.0  , 132.0  , 288.0  , 1.0  , 0.0  , 159.0  , 1.0  , 1.0 );
                        kernels[52] = compute_kernel(x,   53.0  , 1.0  , 142.0  , 226.0  , 0.0  , 0.0  , 111.0  , 1.0  , 0.0 );
                        kernels[53] = compute_kernel(x,   52.0  , 1.0  , 108.0  , 233.0  , 1.0  , 1.0  , 147.0  , 0.0  , 3.0 );
                        kernels[54] = compute_kernel(x,   43.0  , 1.0  , 130.0  , 315.0  , 0.0  , 1.0  , 162.0  , 0.0  , 1.0 );
                        kernels[55] = compute_kernel(x,   53.0  , 1.0  , 130.0  , 246.0  , 1.0  , 0.0  , 173.0  , 0.0  , 3.0 );
                        kernels[56] = compute_kernel(x,   42.0  , 1.0  , 148.0  , 244.0  , 0.0  , 0.0  , 178.0  , 0.0  , 2.0 );
                        kernels[57] = compute_kernel(x,   59.0  , 1.0  , 178.0  , 270.0  , 0.0  , 0.0  , 145.0  , 0.0  , 0.0 );
                        kernels[58] = compute_kernel(x,   69.0  , 1.0  , 160.0  , 234.0  , 1.0  , 0.0  , 131.0  , 0.0  , 1.0 );
                        kernels[59] = compute_kernel(x,   45.0  , 0.0  , 138.0  , 236.0  , 0.0  , 0.0  , 152.0  , 1.0  , 0.0 );
                        kernels[60] = compute_kernel(x,   64.0  , 0.0  , 180.0  , 325.0  , 0.0  , 1.0  , 154.0  , 1.0  , 0.0 );
                        kernels[61] = compute_kernel(x,   64.0  , 0.0  , 140.0  , 313.0  , 0.0  , 1.0  , 133.0  , 0.0  , 0.0 );
                        float decision = 1.249758594558;
                        decision = decision - ( + kernels[0] * -1.0  + kernels[1] * -1.0  + kernels[2] * -1.0  + kernels[3] * -1.0  + kernels[4] * -0.847103066261  + kernels[5] * -1.0  + kernels[6] * -0.916776476583  + kernels[7] * -0.564460274512  + kernels[8] * -1.0  + kernels[9] * -1.0  + kernels[10] * -1.0  + kernels[11] * -1.0  + kernels[12] * -1.0  + kernels[13] * -1.0  + kernels[14] * -1.0  + kernels[15] * -1.0  + kernels[16] * -0.543051942676  + kernels[17] * -1.0  + kernels[18] * -1.0  + kernels[19] * -0.906288546202  + kernels[20] * -1.0  + kernels[21] * -1.0  + kernels[22] * -1.0  + kernels[23] * -1.0  + kernels[24] * -1.0  + kernels[25] * -1.0  + kernels[26] * -1.0  + kernels[27] * -1.0  + kernels[28] * -1.0  + kernels[29] * -1.0 );
                        decision = decision - ( + kernels[30] * 1.0  + kernels[31] * 0.480615512296  + kernels[32] * 1.0  + kernels[33] * 1.0  + kernels[34] * 1.0  + kernels[35] * 1.0  + kernels[36] * 1.0  + kernels[37] * 1.0  + kernels[38] * 0.4246057439  + kernels[39] * 1.0  + kernels[40] * 0.484968912824  + kernels[41] * 1.0  + kernels[42] * 1.0  + kernels[43] * 1.0  + kernels[44] * 0.331752165372  + kernels[45] * 1.0  + kernels[46] * 1.0  + kernels[47] * 0.055737971842  + kernels[48] * 1.0  + kernels[49] * 1.0  + kernels[50] * 1.0  + kernels[51] * 1.0  + kernels[52] * 1.0  + kernels[53] * 1.0  + kernels[54] * 1.0  + kernels[55] * 1.0  + kernels[56] * 1.0  + kernels[57] * 1.0  + kernels[58] * 1.0  + kernels[59] * 1.0  + kernels[60] * 1.0  + kernels[61] * 1.0 );

                        return decision > 0 ? 0 : 1;
                    }

                    /**
                    * Predict readable class name
                    */
                    const char* predictLabel(float *x) {
                        return idxToLabel(predict(x));
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

                protected:
                    /**
                    * Compute kernel between feature vector and support vector.
                    * Kernel type: linear
                    */
                    float compute_kernel(float *x, ...) {
                        va_list w;
                        va_start(w, 9);
                        float kernel = 0.0;

                        for (uint16_t i = 0; i < 9; i++) {
                            kernel += x[i] * va_arg(w, double);
                        }

                        return kernel;
                    }
                };
            }
        }
    }