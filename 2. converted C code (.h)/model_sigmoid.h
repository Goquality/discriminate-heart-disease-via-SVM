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
                        float kernels[118] = { 0 };
                        float decisions[1] = { 0 };
                        int votes[2] = { 0 };
                        kernels[0] = compute_kernel(x,   43.0  , 0.0  , 132.0  , 341.0  , 1.0  , 0.0  , 136.0  , 1.0  , 0.0 );
                        kernels[1] = compute_kernel(x,   62.0  , 0.0  , 130.0  , 263.0  , 0.0  , 1.0  , 97.0  , 0.0  , 1.0 );
                        kernels[2] = compute_kernel(x,   63.0  , 1.0  , 130.0  , 330.0  , 1.0  , 0.0  , 132.0  , 1.0  , 3.0 );
                        kernels[3] = compute_kernel(x,   65.0  , 1.0  , 135.0  , 254.0  , 0.0  , 0.0  , 127.0  , 0.0  , 1.0 );
                        kernels[4] = compute_kernel(x,   48.0  , 1.0  , 130.0  , 256.0  , 1.0  , 0.0  , 150.0  , 1.0  , 2.0 );
                        kernels[5] = compute_kernel(x,   63.0  , 0.0  , 150.0  , 407.0  , 0.0  , 0.0  , 154.0  , 0.0  , 3.0 );
                        kernels[6] = compute_kernel(x,   55.0  , 1.0  , 140.0  , 217.0  , 0.0  , 1.0  , 111.0  , 1.0  , 0.0 );
                        kernels[7] = compute_kernel(x,   65.0  , 1.0  , 138.0  , 282.0  , 1.0  , 0.0  , 174.0  , 0.0  , 1.0 );
                        kernels[8] = compute_kernel(x,   56.0  , 0.0  , 200.0  , 288.0  , 1.0  , 0.0  , 133.0  , 1.0  , 2.0 );
                        kernels[9] = compute_kernel(x,   54.0  , 1.0  , 110.0  , 239.0  , 0.0  , 1.0  , 126.0  , 1.0  , 1.0 );
                        kernels[10] = compute_kernel(x,   70.0  , 1.0  , 145.0  , 174.0  , 0.0  , 1.0  , 125.0  , 1.0  , 0.0 );
                        kernels[11] = compute_kernel(x,   62.0  , 1.0  , 120.0  , 281.0  , 0.0  , 0.0  , 103.0  , 0.0  , 1.0 );
                        kernels[12] = compute_kernel(x,   35.0  , 1.0  , 120.0  , 198.0  , 0.0  , 1.0  , 130.0  , 1.0  , 0.0 );
                        kernels[13] = compute_kernel(x,   59.0  , 1.0  , 170.0  , 288.0  , 0.0  , 0.0  , 159.0  , 0.0  , 0.0 );
                        kernels[14] = compute_kernel(x,   64.0  , 1.0  , 125.0  , 309.0  , 0.0  , 1.0  , 131.0  , 1.0  , 0.0 );
                        kernels[15] = compute_kernel(x,   47.0  , 1.0  , 108.0  , 243.0  , 0.0  , 1.0  , 152.0  , 0.0  , 0.0 );
                        kernels[16] = compute_kernel(x,   57.0  , 1.0  , 165.0  , 289.0  , 1.0  , 0.0  , 124.0  , 0.0  , 3.0 );
                        kernels[17] = compute_kernel(x,   55.0  , 1.0  , 160.0  , 289.0  , 0.0  , 0.0  , 145.0  , 1.0  , 1.0 );
                        kernels[18] = compute_kernel(x,   64.0  , 1.0  , 120.0  , 246.0  , 0.0  , 0.0  , 96.0  , 1.0  , 1.0 );
                        kernels[19] = compute_kernel(x,   70.0  , 1.0  , 130.0  , 322.0  , 0.0  , 0.0  , 109.0  , 0.0  , 3.0 );
                        kernels[20] = compute_kernel(x,   51.0  , 1.0  , 140.0  , 299.0  , 0.0  , 1.0  , 173.0  , 1.0  , 0.0 );
                        kernels[21] = compute_kernel(x,   58.0  , 1.0  , 125.0  , 300.0  , 0.0  , 0.0  , 171.0  , 0.0  , 2.0 );
                        kernels[22] = compute_kernel(x,   60.0  , 1.0  , 140.0  , 293.0  , 0.0  , 0.0  , 170.0  , 0.0  , 2.0 );
                        kernels[23] = compute_kernel(x,   77.0  , 1.0  , 125.0  , 304.0  , 0.0  , 0.0  , 162.0  , 1.0  , 3.0 );
                        kernels[24] = compute_kernel(x,   35.0  , 1.0  , 126.0  , 282.0  , 0.0  , 0.0  , 156.0  , 1.0  , 0.0 );
                        kernels[25] = compute_kernel(x,   70.0  , 1.0  , 160.0  , 269.0  , 0.0  , 1.0  , 112.0  , 1.0  , 1.0 );
                        kernels[26] = compute_kernel(x,   59.0  , 0.0  , 174.0  , 249.0  , 0.0  , 1.0  , 143.0  , 1.0  , 0.0 );
                        kernels[27] = compute_kernel(x,   64.0  , 1.0  , 145.0  , 212.0  , 0.0  , 0.0  , 132.0  , 0.0  , 2.0 );
                        kernels[28] = compute_kernel(x,   57.0  , 1.0  , 152.0  , 274.0  , 0.0  , 1.0  , 88.0  , 1.0  , 1.0 );
                        kernels[29] = compute_kernel(x,   56.0  , 1.0  , 132.0  , 184.0  , 0.0  , 0.0  , 105.0  , 1.0  , 1.0 );
                        kernels[30] = compute_kernel(x,   48.0  , 1.0  , 124.0  , 274.0  , 0.0  , 0.0  , 166.0  , 0.0  , 0.0 );
                        kernels[31] = compute_kernel(x,   56.0  , 0.0  , 134.0  , 409.0  , 0.0  , 0.0  , 150.0  , 1.0  , 2.0 );
                        kernels[32] = compute_kernel(x,   66.0  , 1.0  , 160.0  , 246.0  , 0.0  , 1.0  , 120.0  , 1.0  , 3.0 );
                        kernels[33] = compute_kernel(x,   54.0  , 1.0  , 192.0  , 283.0  , 0.0  , 0.0  , 195.0  , 0.0  , 1.0 );
                        kernels[34] = compute_kernel(x,   69.0  , 1.0  , 140.0  , 254.0  , 0.0  , 0.0  , 146.0  , 0.0  , 3.0 );
                        kernels[35] = compute_kernel(x,   51.0  , 1.0  , 140.0  , 298.0  , 0.0  , 1.0  , 122.0  , 1.0  , 3.0 );
                        kernels[36] = compute_kernel(x,   43.0  , 1.0  , 132.0  , 247.0  , 1.0  , 0.0  , 143.0  , 1.0  , 4.0 );
                        kernels[37] = compute_kernel(x,   62.0  , 0.0  , 138.0  , 294.0  , 1.0  , 1.0  , 106.0  , 0.0  , 3.0 );
                        kernels[38] = compute_kernel(x,   67.0  , 1.0  , 100.0  , 299.0  , 0.0  , 0.0  , 125.0  , 1.0  , 2.0 );
                        kernels[39] = compute_kernel(x,   59.0  , 1.0  , 160.0  , 273.0  , 0.0  , 0.0  , 125.0  , 0.0  , 0.0 );
                        kernels[40] = compute_kernel(x,   45.0  , 1.0  , 142.0  , 309.0  , 0.0  , 0.0  , 147.0  , 1.0  , 3.0 );
                        kernels[41] = compute_kernel(x,   58.0  , 1.0  , 128.0  , 259.0  , 0.0  , 0.0  , 130.0  , 1.0  , 2.0 );
                        kernels[42] = compute_kernel(x,   50.0  , 1.0  , 144.0  , 200.0  , 0.0  , 0.0  , 126.0  , 1.0  , 0.0 );
                        kernels[43] = compute_kernel(x,   62.0  , 0.0  , 150.0  , 244.0  , 0.0  , 1.0  , 154.0  , 1.0  , 0.0 );
                        kernels[44] = compute_kernel(x,   38.0  , 1.0  , 120.0  , 231.0  , 0.0  , 1.0  , 182.0  , 1.0  , 0.0 );
                        kernels[45] = compute_kernel(x,   66.0  , 0.0  , 178.0  , 228.0  , 1.0  , 1.0  , 165.0  , 1.0  , 2.0 );
                        kernels[46] = compute_kernel(x,   52.0  , 1.0  , 112.0  , 230.0  , 0.0  , 1.0  , 160.0  , 0.0  , 1.0 );
                        kernels[47] = compute_kernel(x,   53.0  , 1.0  , 123.0  , 282.0  , 0.0  , 1.0  , 95.0  , 1.0  , 2.0 );
                        kernels[48] = compute_kernel(x,   63.0  , 0.0  , 108.0  , 269.0  , 0.0  , 1.0  , 169.0  , 1.0  , 2.0 );
                        kernels[49] = compute_kernel(x,   54.0  , 1.0  , 110.0  , 206.0  , 0.0  , 0.0  , 108.0  , 1.0  , 1.0 );
                        kernels[50] = compute_kernel(x,   66.0  , 1.0  , 112.0  , 212.0  , 0.0  , 0.0  , 132.0  , 1.0  , 1.0 );
                        kernels[51] = compute_kernel(x,   55.0  , 0.0  , 180.0  , 327.0  , 0.0  , 2.0  , 117.0  , 1.0  , 0.0 );
                        kernels[52] = compute_kernel(x,   49.0  , 1.0  , 118.0  , 149.0  , 0.0  , 0.0  , 126.0  , 0.0  , 3.0 );
                        kernels[53] = compute_kernel(x,   54.0  , 1.0  , 122.0  , 286.0  , 0.0  , 0.0  , 116.0  , 1.0  , 2.0 );
                        kernels[54] = compute_kernel(x,   56.0  , 1.0  , 130.0  , 283.0  , 1.0  , 0.0  , 103.0  , 1.0  , 0.0 );
                        kernels[55] = compute_kernel(x,   46.0  , 1.0  , 120.0  , 249.0  , 0.0  , 0.0  , 144.0  , 0.0  , 0.0 );
                        kernels[56] = compute_kernel(x,   61.0  , 1.0  , 134.0  , 234.0  , 0.0  , 1.0  , 145.0  , 0.0  , 2.0 );
                        kernels[57] = compute_kernel(x,   67.0  , 1.0  , 120.0  , 237.0  , 0.0  , 1.0  , 71.0  , 0.0  , 0.0 );
                        kernels[58] = compute_kernel(x,   58.0  , 1.0  , 100.0  , 234.0  , 0.0  , 1.0  , 156.0  , 0.0  , 1.0 );
                        kernels[59] = compute_kernel(x,   48.0  , 1.0  , 122.0  , 222.0  , 0.0  , 0.0  , 186.0  , 0.0  , 0.0 );
                        kernels[60] = compute_kernel(x,   45.0  , 1.0  , 115.0  , 260.0  , 0.0  , 0.0  , 185.0  , 0.0  , 0.0 );
                        kernels[61] = compute_kernel(x,   34.0  , 1.0  , 118.0  , 182.0  , 0.0  , 0.0  , 174.0  , 0.0  , 0.0 );
                        kernels[62] = compute_kernel(x,   57.0  , 0.0  , 128.0  , 303.0  , 0.0  , 0.0  , 159.0  , 0.0  , 1.0 );
                        kernels[63] = compute_kernel(x,   71.0  , 0.0  , 110.0  , 265.0  , 1.0  , 0.0  , 130.0  , 0.0  , 1.0 );
                        kernels[64] = compute_kernel(x,   54.0  , 1.0  , 108.0  , 309.0  , 0.0  , 1.0  , 156.0  , 0.0  , 0.0 );
                        kernels[65] = compute_kernel(x,   52.0  , 1.0  , 118.0  , 186.0  , 0.0  , 0.0  , 190.0  , 0.0  , 0.0 );
                        kernels[66] = compute_kernel(x,   41.0  , 1.0  , 135.0  , 203.0  , 0.0  , 1.0  , 132.0  , 0.0  , 0.0 );
                        kernels[67] = compute_kernel(x,   58.0  , 1.0  , 140.0  , 211.0  , 1.0  , 0.0  , 165.0  , 0.0  , 0.0 );
                        kernels[68] = compute_kernel(x,   35.0  , 0.0  , 138.0  , 183.0  , 0.0  , 1.0  , 182.0  , 0.0  , 0.0 );
                        kernels[69] = compute_kernel(x,   51.0  , 1.0  , 100.0  , 222.0  , 0.0  , 1.0  , 143.0  , 1.0  , 0.0 );
                        kernels[70] = compute_kernel(x,   45.0  , 0.0  , 130.0  , 234.0  , 0.0  , 0.0  , 175.0  , 0.0  , 0.0 );
                        kernels[71] = compute_kernel(x,   44.0  , 1.0  , 120.0  , 220.0  , 0.0  , 1.0  , 170.0  , 0.0  , 0.0 );
                        kernels[72] = compute_kernel(x,   62.0  , 0.0  , 124.0  , 209.0  , 0.0  , 1.0  , 163.0  , 0.0  , 0.0 );
                        kernels[73] = compute_kernel(x,   54.0  , 1.0  , 120.0  , 258.0  , 0.0  , 0.0  , 147.0  , 0.0  , 0.0 );
                        kernels[74] = compute_kernel(x,   51.0  , 1.0  , 94.0  , 227.0  , 0.0  , 1.0  , 154.0  , 1.0  , 1.0 );
                        kernels[75] = compute_kernel(x,   29.0  , 1.0  , 130.0  , 204.0  , 0.0  , 0.0  , 202.0  , 0.0  , 0.0 );
                        kernels[76] = compute_kernel(x,   51.0  , 1.0  , 140.0  , 261.0  , 0.0  , 0.0  , 186.0  , 1.0  , 0.0 );
                        kernels[77] = compute_kernel(x,   43.0  , 0.0  , 122.0  , 213.0  , 0.0  , 1.0  , 165.0  , 0.0  , 0.0 );
                        kernels[78] = compute_kernel(x,   55.0  , 0.0  , 135.0  , 250.0  , 0.0  , 0.0  , 161.0  , 0.0  , 0.0 );
                        kernels[79] = compute_kernel(x,   51.0  , 1.0  , 125.0  , 245.0  , 1.0  , 0.0  , 166.0  , 0.0  , 0.0 );
                        kernels[80] = compute_kernel(x,   59.0  , 1.0  , 140.0  , 221.0  , 0.0  , 1.0  , 164.0  , 1.0  , 0.0 );
                        kernels[81] = compute_kernel(x,   52.0  , 1.0  , 128.0  , 205.0  , 1.0  , 1.0  , 184.0  , 0.0  , 0.0 );
                        kernels[82] = compute_kernel(x,   58.0  , 1.0  , 105.0  , 240.0  , 0.0  , 0.0  , 154.0  , 1.0  , 0.0 );
                        kernels[83] = compute_kernel(x,   41.0  , 1.0  , 112.0  , 250.0  , 0.0  , 1.0  , 179.0  , 0.0  , 0.0 );
                        kernels[84] = compute_kernel(x,   45.0  , 1.0  , 128.0  , 308.0  , 0.0  , 0.0  , 170.0  , 0.0  , 0.0 );
                        kernels[85] = compute_kernel(x,   60.0  , 0.0  , 102.0  , 318.0  , 0.0  , 1.0  , 160.0  , 0.0  , 1.0 );
                        kernels[86] = compute_kernel(x,   52.0  , 1.0  , 152.0  , 298.0  , 1.0  , 1.0  , 178.0  , 0.0  , 0.0 );
                        kernels[87] = compute_kernel(x,   42.0  , 0.0  , 102.0  , 265.0  , 0.0  , 0.0  , 122.0  , 0.0  , 0.0 );
                        kernels[88] = compute_kernel(x,   67.0  , 0.0  , 115.0  , 564.0  , 0.0  , 0.0  , 160.0  , 0.0  , 0.0 );
                        kernels[89] = compute_kernel(x,   68.0  , 1.0  , 118.0  , 277.0  , 0.0  , 1.0  , 151.0  , 0.0  , 1.0 );
                        kernels[90] = compute_kernel(x,   46.0  , 1.0  , 101.0  , 197.0  , 1.0  , 1.0  , 156.0  , 0.0  , 0.0 );
                        kernels[91] = compute_kernel(x,   54.0  , 0.0  , 110.0  , 214.0  , 0.0  , 1.0  , 158.0  , 0.0  , 0.0 );
                        kernels[92] = compute_kernel(x,   58.0  , 0.0  , 100.0  , 248.0  , 0.0  , 0.0  , 122.0  , 0.0  , 0.0 );
                        kernels[93] = compute_kernel(x,   48.0  , 1.0  , 124.0  , 255.0  , 1.0  , 1.0  , 175.0  , 0.0  , 2.0 );
                        kernels[94] = compute_kernel(x,   57.0  , 1.0  , 132.0  , 207.0  , 0.0  , 1.0  , 168.0  , 1.0  , 0.0 );
                        kernels[95] = compute_kernel(x,   52.0  , 1.0  , 138.0  , 223.0  , 0.0  , 1.0  , 169.0  , 0.0  , 4.0 );
                        kernels[96] = compute_kernel(x,   54.0  , 0.0  , 132.0  , 288.0  , 1.0  , 0.0  , 159.0  , 1.0  , 1.0 );
                        kernels[97] = compute_kernel(x,   45.0  , 0.0  , 112.0  , 160.0  , 0.0  , 1.0  , 138.0  , 0.0  , 0.0 );
                        kernels[98] = compute_kernel(x,   53.0  , 1.0  , 142.0  , 226.0  , 0.0  , 0.0  , 111.0  , 1.0  , 0.0 );
                        kernels[99] = compute_kernel(x,   62.0  , 0.0  , 140.0  , 394.0  , 0.0  , 0.0  , 157.0  , 0.0  , 0.0 );
                        kernels[100] = compute_kernel(x,   52.0  , 1.0  , 108.0  , 233.0  , 1.0  , 1.0  , 147.0  , 0.0  , 3.0 );
                        kernels[101] = compute_kernel(x,   43.0  , 1.0  , 130.0  , 315.0  , 0.0  , 1.0  , 162.0  , 0.0  , 1.0 );
                        kernels[102] = compute_kernel(x,   53.0  , 1.0  , 130.0  , 246.0  , 1.0  , 0.0  , 173.0  , 0.0  , 3.0 );
                        kernels[103] = compute_kernel(x,   42.0  , 1.0  , 148.0  , 244.0  , 0.0  , 0.0  , 178.0  , 0.0  , 2.0 );
                        kernels[104] = compute_kernel(x,   59.0  , 1.0  , 178.0  , 270.0  , 0.0  , 0.0  , 145.0  , 0.0  , 0.0 );
                        kernels[105] = compute_kernel(x,   63.0  , 0.0  , 140.0  , 195.0  , 0.0  , 1.0  , 179.0  , 0.0  , 2.0 );
                        kernels[106] = compute_kernel(x,   42.0  , 1.0  , 120.0  , 240.0  , 1.0  , 1.0  , 194.0  , 0.0  , 0.0 );
                        kernels[107] = compute_kernel(x,   50.0  , 1.0  , 129.0  , 196.0  , 0.0  , 1.0  , 163.0  , 0.0  , 0.0 );
                        kernels[108] = compute_kernel(x,   68.0  , 0.0  , 120.0  , 211.0  , 0.0  , 0.0  , 115.0  , 0.0  , 0.0 );
                        kernels[109] = compute_kernel(x,   69.0  , 1.0  , 160.0  , 234.0  , 1.0  , 0.0  , 131.0  , 0.0  , 1.0 );
                        kernels[110] = compute_kernel(x,   45.0  , 0.0  , 138.0  , 236.0  , 0.0  , 0.0  , 152.0  , 1.0  , 0.0 );
                        kernels[111] = compute_kernel(x,   50.0  , 0.0  , 120.0  , 244.0  , 0.0  , 1.0  , 162.0  , 0.0  , 0.0 );
                        kernels[112] = compute_kernel(x,   50.0  , 0.0  , 110.0  , 254.0  , 0.0  , 0.0  , 159.0  , 0.0  , 0.0 );
                        kernels[113] = compute_kernel(x,   64.0  , 0.0  , 180.0  , 325.0  , 0.0  , 1.0  , 154.0  , 1.0  , 0.0 );
                        kernels[114] = compute_kernel(x,   57.0  , 1.0  , 150.0  , 126.0  , 1.0  , 1.0  , 173.0  , 0.0  , 1.0 );
                        kernels[115] = compute_kernel(x,   64.0  , 0.0  , 140.0  , 313.0  , 0.0  , 1.0  , 133.0  , 0.0  , 0.0 );
                        kernels[116] = compute_kernel(x,   43.0  , 1.0  , 110.0  , 211.0  , 0.0  , 1.0  , 161.0  , 0.0  , 0.0 );
                        kernels[117] = compute_kernel(x,   55.0  , 1.0  , 130.0  , 262.0  , 0.0  , 1.0  , 155.0  , 0.0  , 0.0 );
                        float decision = 1.0;
                        decision = decision - ( + kernels[0] * -1.0  + kernels[1] * -1.0  + kernels[2] * -1.0  + kernels[3] * -1.0  + kernels[4] * -1.0  + kernels[5] * -1.0  + kernels[6] * -1.0  + kernels[7] * -1.0  + kernels[8] * -1.0  + kernels[9] * -1.0  + kernels[10] * -1.0  + kernels[11] * -1.0  + kernels[12] * -1.0  + kernels[13] * -1.0  + kernels[14] * -1.0  + kernels[15] * -1.0  + kernels[16] * -1.0  + kernels[17] * -1.0  + kernels[18] * -1.0  + kernels[19] * -1.0  + kernels[20] * -1.0  + kernels[21] * -1.0  + kernels[22] * -1.0  + kernels[23] * -1.0  + kernels[24] * -1.0  + kernels[25] * -1.0  + kernels[26] * -1.0  + kernels[27] * -1.0  + kernels[28] * -1.0  + kernels[29] * -1.0  + kernels[30] * -1.0  + kernels[31] * -1.0  + kernels[32] * -1.0  + kernels[33] * -1.0  + kernels[34] * -1.0  + kernels[35] * -1.0  + kernels[36] * -1.0  + kernels[37] * -1.0  + kernels[38] * -1.0  + kernels[39] * -1.0  + kernels[40] * -1.0  + kernels[41] * -1.0  + kernels[42] * -1.0  + kernels[43] * -1.0  + kernels[44] * -1.0  + kernels[45] * -1.0  + kernels[46] * -1.0  + kernels[47] * -1.0  + kernels[48] * -1.0  + kernels[49] * -1.0  + kernels[50] * -1.0  + kernels[51] * -1.0  + kernels[52] * -1.0  + kernels[53] * -1.0  + kernels[54] * -1.0  + kernels[55] * -1.0  + kernels[56] * -1.0  + kernels[57] * -1.0  + kernels[58] * -1.0 );
                        decision = decision - ( + kernels[59] * 1.0  + kernels[60] * 1.0  + kernels[61] * 1.0  + kernels[62] * 1.0  + kernels[63] * 1.0  + kernels[64] * 1.0  + kernels[65] * 1.0  + kernels[66] * 1.0  + kernels[67] * 1.0  + kernels[68] * 1.0  + kernels[69] * 1.0  + kernels[70] * 1.0  + kernels[71] * 1.0  + kernels[72] * 1.0  + kernels[73] * 1.0  + kernels[74] * 1.0  + kernels[75] * 1.0  + kernels[76] * 1.0  + kernels[77] * 1.0  + kernels[78] * 1.0  + kernels[79] * 1.0  + kernels[80] * 1.0  + kernels[81] * 1.0  + kernels[82] * 1.0  + kernels[83] * 1.0  + kernels[84] * 1.0  + kernels[85] * 1.0  + kernels[86] * 1.0  + kernels[87] * 1.0  + kernels[88] * 1.0  + kernels[89] * 1.0  + kernels[90] * 1.0  + kernels[91] * 1.0  + kernels[92] * 1.0  + kernels[93] * 1.0  + kernels[94] * 1.0  + kernels[95] * 1.0  + kernels[96] * 1.0  + kernels[97] * 1.0  + kernels[98] * 1.0  + kernels[99] * 1.0  + kernels[100] * 1.0  + kernels[101] * 1.0  + kernels[102] * 1.0  + kernels[103] * 1.0  + kernels[104] * 1.0  + kernels[105] * 1.0  + kernels[106] * 1.0  + kernels[107] * 1.0  + kernels[108] * 1.0  + kernels[109] * 1.0  + kernels[110] * 1.0  + kernels[111] * 1.0  + kernels[112] * 1.0  + kernels[113] * 1.0  + kernels[114] * 1.0  + kernels[115] * 1.0  + kernels[116] * 1.0  + kernels[117] * 1.0 );

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
                    * Kernel type: sigmoid
                    */
                    float compute_kernel(float *x, ...) {
                        va_list w;
                        va_start(w, 9);
                        float kernel = 0.0;

                        for (uint16_t i = 0; i < 9; i++) {
                            kernel += x[i] * va_arg(w, double);
                        }

                        return sigmoid((0.001 * kernel) + 0.0);
                    }
                };
            }
        }
    }