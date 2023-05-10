#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char FND_SEGNP[12] = { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F,0x00,0x40 };
unsigned char FND_SEGPOS[4] = { 0x01,0x02,0x04,0x08 };

volatile int flag = 1;
volatile int flag2 = 0;
volatile int t = 3906;
volatile int time = 0;
volatile int dim[9] = { 20,0,120,200,0,0,150,0,0 };
volatile int* k = &dim;
volatile int i = 0;
volatile int j = 0;
volatile int svm = 0;
char rx, tx;

ISR(TIMER0_OVF_vect)   //timer overflow가 일어날때 마다 진입
{
    time++;
    if (time == t) //0.5초가 되면 불꺼 
    {
        if (flag2 == 1)
            PORTF = 0x00;
    }
    if (time == 2 * t) //1초가 되면 불켜 
    {
        time = 0;
        if (k == &svm || k == &j)
        {
            if (svm == 0)
                PORTF = 0x80;
            else if (svm == 1)
                PORTF = 0x40;
            else if (svm == 2)
                PORTF = 0x20;
            else if (svm == 3)
                PORTF = 0x10;
        }
    }

}

void init_port(void)      //포트 초기화
{
    DDRA = 0xFF;         //A포트는 전체 입력으로
    DDRE = 0x0E;         //Enable 포트는 3번4번을 켜서 세그먼트 포지션에 대한 입력 허가
    PORTE = 0x04;         //E포트의 3번을 인가해서, 세그먼트 포지션에 대한 인가
    PORTA = 0x0F;         //00001111을 넣어서 4개의 세그먼트를 사용하겠다 선언
}
void init_interrupt(void)
{
    EIMSK = 0x00;   //우선적으로 외부 인터럽트 전체 다 끈다.
    EICRA = 0x00;   //인터럽트 0,1,2,3에 대한 LOW인가
    EICRB = 0x0A;   //인터럽트 4,5번에 대한 falling edge 사용,
    EIMSK = 0x30;   //인터럽트 4번 5번을 사용하겠다 선언
    EIFR = 0xFF;   //전체인터럽트에 일괄적으로 flag세우고,초기화
}

void timer_setting()
{
    TCCR0 = _BV(CS01);
    TCNT0 = 0x00;
    TIMSK = 0x01;
    TIFR = 0xFF;
}

void init_uart(void)
{
    UBRR0H = 0;
    UBRR0L = 103;
    UCSR0B = 0x18;
}
void display() //7segment 조작 
{
    if (k != &j) //7segment에서 k!=j주소가 아니면(즉 svm 또는 dim[i]의 주소라면) 
    {
        PORTA = FND_SEGPOS[0]; //천의자리, 백의자리, 십의자리, 일의자리에 i값을 띄운다. 
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[i];                   //int 5번을 통해 i값, 즉 디멘션을 설정하는데 몇번디멘션인지 출력 
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);

        PORTA = FND_SEGPOS[1];
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[((*k) / 100) % 10];   //sw를 통해 해당 디멘션의 값을 증가 감소하는데 그 값의 백의자리를 출력
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);

        PORTA = FND_SEGPOS[2];
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[((*k) / 10) % 10];    //sw를 통해 해당 디멘션의 값을 증가 감소하는데 그 값의 십의자리를 출력
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);

        PORTA = FND_SEGPOS[3];
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[(*k) % 10];           //sw를 통해 해당 디멘션의 값을 증가 감소하는데 그 값의 일의자리를 출력
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);
    }
    else //k값이 j의 주소 즉 결과값인 경우에는 첫번째부터 3번째까지 7segment에 띄우지 않고 마지막 j의 결과 즉 0 또는 1만 띄우는거야 즉 k값에 j의 주소를 넣었어 
    {
        PORTA = FND_SEGPOS[0];
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[10];
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);

        PORTA = FND_SEGPOS[1];
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[10];
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);

        PORTA = FND_SEGPOS[2];
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[10];
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);

        PORTA = FND_SEGPOS[3];
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[*k];
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);
    }
}
float compute_kernellin(int* x, float data1, float data2, float data3, float data4, float data5, float data6, float data7, float data8, float data9)
{
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

    return kernel;
}

int predictlin(int* x)
{
    float kernels[62] = { 0 };
    kernels[0] = compute_kernellin(x, 43.0, 0.0, 132.0, 341.0, 1.0, 0.0, 136.0, 1.0, 0.0);
    kernels[1] = compute_kernellin(x, 62.0, 0.0, 130.0, 263.0, 0.0, 1.0, 97.0, 0.0, 1.0);
    kernels[2] = compute_kernellin(x, 65.0, 1.0, 135.0, 254.0, 0.0, 0.0, 127.0, 0.0, 1.0);
    kernels[3] = compute_kernellin(x, 48.0, 1.0, 130.0, 256.0, 1.0, 0.0, 150.0, 1.0, 2.0);
    kernels[4] = compute_kernellin(x, 63.0, 0.0, 150.0, 407.0, 0.0, 0.0, 154.0, 0.0, 3.0);
    kernels[5] = compute_kernellin(x, 65.0, 1.0, 138.0, 282.0, 1.0, 0.0, 174.0, 0.0, 1.0);
    kernels[6] = compute_kernellin(x, 54.0, 1.0, 110.0, 239.0, 0.0, 1.0, 126.0, 1.0, 1.0);
    kernels[7] = compute_kernellin(x, 70.0, 1.0, 145.0, 174.0, 0.0, 1.0, 125.0, 1.0, 0.0);
    kernels[8] = compute_kernellin(x, 35.0, 1.0, 120.0, 198.0, 0.0, 1.0, 130.0, 1.0, 0.0);
    kernels[9] = compute_kernellin(x, 59.0, 1.0, 170.0, 288.0, 0.0, 0.0, 159.0, 0.0, 0.0);
    kernels[10] = compute_kernellin(x, 47.0, 1.0, 108.0, 243.0, 0.0, 1.0, 152.0, 0.0, 0.0);
    kernels[11] = compute_kernellin(x, 51.0, 1.0, 140.0, 299.0, 0.0, 1.0, 173.0, 1.0, 0.0);
    kernels[12] = compute_kernellin(x, 58.0, 1.0, 125.0, 300.0, 0.0, 0.0, 171.0, 0.0, 2.0);
    kernels[13] = compute_kernellin(x, 60.0, 1.0, 140.0, 293.0, 0.0, 0.0, 170.0, 0.0, 2.0);
    kernels[14] = compute_kernellin(x, 35.0, 1.0, 126.0, 282.0, 0.0, 0.0, 156.0, 1.0, 0.0);
    kernels[15] = compute_kernellin(x, 59.0, 0.0, 174.0, 249.0, 0.0, 1.0, 143.0, 1.0, 0.0);
    kernels[16] = compute_kernellin(x, 64.0, 1.0, 145.0, 212.0, 0.0, 0.0, 132.0, 0.0, 2.0);
    kernels[17] = compute_kernellin(x, 48.0, 1.0, 124.0, 274.0, 0.0, 0.0, 166.0, 0.0, 0.0);
    kernels[18] = compute_kernellin(x, 54.0, 1.0, 192.0, 283.0, 0.0, 0.0, 195.0, 0.0, 1.0);
    kernels[19] = compute_kernellin(x, 59.0, 1.0, 160.0, 273.0, 0.0, 0.0, 125.0, 0.0, 0.0);
    kernels[20] = compute_kernellin(x, 62.0, 0.0, 150.0, 244.0, 0.0, 1.0, 154.0, 1.0, 0.0);
    kernels[21] = compute_kernellin(x, 38.0, 1.0, 120.0, 231.0, 0.0, 1.0, 182.0, 1.0, 0.0);
    kernels[22] = compute_kernellin(x, 66.0, 0.0, 178.0, 228.0, 1.0, 1.0, 165.0, 1.0, 2.0);
    kernels[23] = compute_kernellin(x, 52.0, 1.0, 112.0, 230.0, 0.0, 1.0, 160.0, 0.0, 1.0);
    kernels[24] = compute_kernellin(x, 63.0, 0.0, 108.0, 269.0, 0.0, 1.0, 169.0, 1.0, 2.0);
    kernels[25] = compute_kernellin(x, 66.0, 1.0, 112.0, 212.0, 0.0, 0.0, 132.0, 1.0, 1.0);
    kernels[26] = compute_kernellin(x, 49.0, 1.0, 118.0, 149.0, 0.0, 0.0, 126.0, 0.0, 3.0);
    kernels[27] = compute_kernellin(x, 46.0, 1.0, 120.0, 249.0, 0.0, 0.0, 144.0, 0.0, 0.0);
    kernels[28] = compute_kernellin(x, 61.0, 1.0, 134.0, 234.0, 0.0, 1.0, 145.0, 0.0, 2.0);
    kernels[29] = compute_kernellin(x, 58.0, 1.0, 100.0, 234.0, 0.0, 1.0, 156.0, 0.0, 1.0);
    kernels[30] = compute_kernellin(x, 71.0, 0.0, 160.0, 302.0, 0.0, 1.0, 162.0, 0.0, 2.0);
    kernels[31] = compute_kernellin(x, 65.0, 0.0, 140.0, 417.0, 1.0, 0.0, 157.0, 0.0, 1.0);
    kernels[32] = compute_kernellin(x, 54.0, 1.0, 125.0, 273.0, 0.0, 0.0, 152.0, 0.0, 1.0);
    kernels[33] = compute_kernellin(x, 51.0, 1.0, 125.0, 213.0, 0.0, 0.0, 125.0, 1.0, 1.0);
    kernels[34] = compute_kernellin(x, 65.0, 0.0, 160.0, 360.0, 0.0, 0.0, 151.0, 0.0, 0.0);
    kernels[35] = compute_kernellin(x, 51.0, 0.0, 140.0, 308.0, 0.0, 0.0, 142.0, 0.0, 1.0);
    kernels[36] = compute_kernellin(x, 45.0, 1.0, 104.0, 208.0, 0.0, 0.0, 148.0, 1.0, 0.0);
    kernels[37] = compute_kernellin(x, 47.0, 1.0, 138.0, 257.0, 0.0, 0.0, 156.0, 0.0, 0.0);
    kernels[38] = compute_kernellin(x, 53.0, 0.0, 128.0, 216.0, 0.0, 0.0, 115.0, 0.0, 0.0);
    kernels[39] = compute_kernellin(x, 62.0, 1.0, 130.0, 231.0, 0.0, 1.0, 146.0, 0.0, 3.0);
    kernels[40] = compute_kernellin(x, 52.0, 1.0, 134.0, 201.0, 0.0, 1.0, 158.0, 0.0, 1.0);
    kernels[41] = compute_kernellin(x, 41.0, 1.0, 135.0, 203.0, 0.0, 1.0, 132.0, 0.0, 0.0);
    kernels[42] = compute_kernellin(x, 51.0, 1.0, 100.0, 222.0, 0.0, 1.0, 143.0, 1.0, 0.0);
    kernels[43] = compute_kernellin(x, 51.0, 1.0, 94.0, 227.0, 0.0, 1.0, 154.0, 1.0, 1.0);
    kernels[44] = compute_kernellin(x, 51.0, 1.0, 140.0, 261.0, 0.0, 0.0, 186.0, 1.0, 0.0);
    kernels[45] = compute_kernellin(x, 59.0, 1.0, 140.0, 221.0, 0.0, 1.0, 164.0, 1.0, 0.0);
    kernels[46] = compute_kernellin(x, 58.0, 1.0, 105.0, 240.0, 0.0, 0.0, 154.0, 1.0, 0.0);
    kernels[47] = compute_kernellin(x, 67.0, 0.0, 115.0, 564.0, 0.0, 0.0, 160.0, 0.0, 0.0);
    kernels[48] = compute_kernellin(x, 68.0, 1.0, 118.0, 277.0, 0.0, 1.0, 151.0, 0.0, 1.0);
    kernels[49] = compute_kernellin(x, 57.0, 1.0, 132.0, 207.0, 0.0, 1.0, 168.0, 1.0, 0.0);
    kernels[50] = compute_kernellin(x, 52.0, 1.0, 138.0, 223.0, 0.0, 1.0, 169.0, 0.0, 4.0);
    kernels[51] = compute_kernellin(x, 54.0, 0.0, 132.0, 288.0, 1.0, 0.0, 159.0, 1.0, 1.0);
    kernels[52] = compute_kernellin(x, 53.0, 1.0, 142.0, 226.0, 0.0, 0.0, 111.0, 1.0, 0.0);
    kernels[53] = compute_kernellin(x, 52.0, 1.0, 108.0, 233.0, 1.0, 1.0, 147.0, 0.0, 3.0);
    kernels[54] = compute_kernellin(x, 43.0, 1.0, 130.0, 315.0, 0.0, 1.0, 162.0, 0.0, 1.0);
    kernels[55] = compute_kernellin(x, 53.0, 1.0, 130.0, 246.0, 1.0, 0.0, 173.0, 0.0, 3.0);
    kernels[56] = compute_kernellin(x, 42.0, 1.0, 148.0, 244.0, 0.0, 0.0, 178.0, 0.0, 2.0);
    kernels[57] = compute_kernellin(x, 59.0, 1.0, 178.0, 270.0, 0.0, 0.0, 145.0, 0.0, 0.0);
    kernels[58] = compute_kernellin(x, 69.0, 1.0, 160.0, 234.0, 1.0, 0.0, 131.0, 0.0, 1.0);
    kernels[59] = compute_kernellin(x, 45.0, 0.0, 138.0, 236.0, 0.0, 0.0, 152.0, 1.0, 0.0);
    kernels[60] = compute_kernellin(x, 64.0, 0.0, 180.0, 325.0, 0.0, 1.0, 154.0, 1.0, 0.0);
    kernels[61] = compute_kernellin(x, 64.0, 0.0, 140.0, 313.0, 0.0, 1.0, 133.0, 0.0, 0.0);
    float decision = 1.249758594558;
    decision = decision - (+kernels[0] * -1.0 + kernels[1] * -1.0 + kernels[2] * -1.0 + kernels[3] * -1.0 + kernels[4] * -0.847103066261 + kernels[5] * -1.0 + kernels[6] * -0.916776476583 + kernels[7] * -0.564460274512 + kernels[8] * -1.0 + kernels[9] * -1.0 + kernels[10] * -1.0 + kernels[11] * -1.0 + kernels[12] * -1.0 + kernels[13] * -1.0 + kernels[14] * -1.0 + kernels[15] * -1.0 + kernels[16] * -0.543051942676 + kernels[17] * -1.0 + kernels[18] * -1.0 + kernels[19] * -0.906288546202 + kernels[20] * -1.0 + kernels[21] * -1.0 + kernels[22] * -1.0 + kernels[23] * -1.0 + kernels[24] * -1.0 + kernels[25] * -1.0 + kernels[26] * -1.0 + kernels[27] * -1.0 + kernels[28] * -1.0 + kernels[29] * -1.0);
    decision = decision - (+kernels[30] * 1.0 + kernels[31] * 0.480615512296 + kernels[32] * 1.0 + kernels[33] * 1.0 + kernels[34] * 1.0 + kernels[35] * 1.0 + kernels[36] * 1.0 + kernels[37] * 1.0 + kernels[38] * 0.4246057439 + kernels[39] * 1.0 + kernels[40] * 0.484968912824 + kernels[41] * 1.0 + kernels[42] * 1.0 + kernels[43] * 1.0 + kernels[44] * 0.331752165372 + kernels[45] * 1.0 + kernels[46] * 1.0 + kernels[47] * 0.055737971842 + kernels[48] * 1.0 + kernels[49] * 1.0 + kernels[50] * 1.0 + kernels[51] * 1.0 + kernels[52] * 1.0 + kernels[53] * 1.0 + kernels[54] * 1.0 + kernels[55] * 1.0 + kernels[56] * 1.0 + kernels[57] * 1.0 + kernels[58] * 1.0 + kernels[59] * 1.0 + kernels[60] * 1.0 + kernels[61] * 1.0);

    return decision > 0 ? 0 : 1;
}

float compute_kernelpoly(int* x, float data1, float data2, float data3, float data4, float data5, float data6, float data7, float data8, float data9)
{
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

int predictpoly(int* x)
{
    float kernels[61] = { 0 };
    kernels[0] = compute_kernelpoly(x, 43.0, 0.0, 132.0, 341.0, 1.0, 0.0, 136.0, 1.0, 0.0);
    kernels[1] = compute_kernelpoly(x, 62.0, 0.0, 130.0, 263.0, 0.0, 1.0, 97.0, 0.0, 1.0);
    kernels[2] = compute_kernelpoly(x, 65.0, 1.0, 135.0, 254.0, 0.0, 0.0, 127.0, 0.0, 1.0);
    kernels[3] = compute_kernelpoly(x, 48.0, 1.0, 130.0, 256.0, 1.0, 0.0, 150.0, 1.0, 2.0);
    kernels[4] = compute_kernelpoly(x, 55.0, 1.0, 140.0, 217.0, 0.0, 1.0, 111.0, 1.0, 0.0);
    kernels[5] = compute_kernelpoly(x, 65.0, 1.0, 138.0, 282.0, 1.0, 0.0, 174.0, 0.0, 1.0);
    kernels[6] = compute_kernelpoly(x, 56.0, 0.0, 200.0, 288.0, 1.0, 0.0, 133.0, 1.0, 2.0);
    kernels[7] = compute_kernelpoly(x, 54.0, 1.0, 110.0, 239.0, 0.0, 1.0, 126.0, 1.0, 1.0);
    kernels[8] = compute_kernelpoly(x, 35.0, 1.0, 120.0, 198.0, 0.0, 1.0, 130.0, 1.0, 0.0);
    kernels[9] = compute_kernelpoly(x, 59.0, 1.0, 170.0, 288.0, 0.0, 0.0, 159.0, 0.0, 0.0);
    kernels[10] = compute_kernelpoly(x, 47.0, 1.0, 108.0, 243.0, 0.0, 1.0, 152.0, 0.0, 0.0);
    kernels[11] = compute_kernelpoly(x, 51.0, 1.0, 140.0, 299.0, 0.0, 1.0, 173.0, 1.0, 0.0);
    kernels[12] = compute_kernelpoly(x, 58.0, 1.0, 125.0, 300.0, 0.0, 0.0, 171.0, 0.0, 2.0);
    kernels[13] = compute_kernelpoly(x, 60.0, 1.0, 140.0, 293.0, 0.0, 0.0, 170.0, 0.0, 2.0);
    kernels[14] = compute_kernelpoly(x, 35.0, 1.0, 126.0, 282.0, 0.0, 0.0, 156.0, 1.0, 0.0);
    kernels[15] = compute_kernelpoly(x, 59.0, 0.0, 174.0, 249.0, 0.0, 1.0, 143.0, 1.0, 0.0);
    kernels[16] = compute_kernelpoly(x, 64.0, 1.0, 145.0, 212.0, 0.0, 0.0, 132.0, 0.0, 2.0);
    kernels[17] = compute_kernelpoly(x, 48.0, 1.0, 124.0, 274.0, 0.0, 0.0, 166.0, 0.0, 0.0);
    kernels[18] = compute_kernelpoly(x, 54.0, 1.0, 192.0, 283.0, 0.0, 0.0, 195.0, 0.0, 1.0);
    kernels[19] = compute_kernelpoly(x, 59.0, 1.0, 160.0, 273.0, 0.0, 0.0, 125.0, 0.0, 0.0);
    kernels[20] = compute_kernelpoly(x, 62.0, 0.0, 150.0, 244.0, 0.0, 1.0, 154.0, 1.0, 0.0);
    kernels[21] = compute_kernelpoly(x, 38.0, 1.0, 120.0, 231.0, 0.0, 1.0, 182.0, 1.0, 0.0);
    kernels[22] = compute_kernelpoly(x, 52.0, 1.0, 112.0, 230.0, 0.0, 1.0, 160.0, 0.0, 1.0);
    kernels[23] = compute_kernelpoly(x, 63.0, 0.0, 108.0, 269.0, 0.0, 1.0, 169.0, 1.0, 2.0);
    kernels[24] = compute_kernelpoly(x, 54.0, 1.0, 110.0, 206.0, 0.0, 0.0, 108.0, 1.0, 1.0);
    kernels[25] = compute_kernelpoly(x, 66.0, 1.0, 112.0, 212.0, 0.0, 0.0, 132.0, 1.0, 1.0);
    kernels[26] = compute_kernelpoly(x, 49.0, 1.0, 118.0, 149.0, 0.0, 0.0, 126.0, 0.0, 3.0);
    kernels[27] = compute_kernelpoly(x, 46.0, 1.0, 120.0, 249.0, 0.0, 0.0, 144.0, 0.0, 0.0);
    kernels[28] = compute_kernelpoly(x, 61.0, 1.0, 134.0, 234.0, 0.0, 1.0, 145.0, 0.0, 2.0);
    kernels[29] = compute_kernelpoly(x, 58.0, 1.0, 100.0, 234.0, 0.0, 1.0, 156.0, 0.0, 1.0);
    kernels[30] = compute_kernelpoly(x, 71.0, 0.0, 160.0, 302.0, 0.0, 1.0, 162.0, 0.0, 2.0);
    kernels[31] = compute_kernelpoly(x, 51.0, 1.0, 110.0, 175.0, 0.0, 1.0, 123.0, 0.0, 0.0);
    kernels[32] = compute_kernelpoly(x, 54.0, 1.0, 125.0, 273.0, 0.0, 0.0, 152.0, 0.0, 1.0);
    kernels[33] = compute_kernelpoly(x, 51.0, 1.0, 125.0, 213.0, 0.0, 0.0, 125.0, 1.0, 1.0);
    kernels[34] = compute_kernelpoly(x, 46.0, 0.0, 142.0, 177.0, 0.0, 0.0, 160.0, 1.0, 0.0);
    kernels[35] = compute_kernelpoly(x, 65.0, 0.0, 160.0, 360.0, 0.0, 0.0, 151.0, 0.0, 0.0);
    kernels[36] = compute_kernelpoly(x, 45.0, 1.0, 104.0, 208.0, 0.0, 0.0, 148.0, 1.0, 0.0);
    kernels[37] = compute_kernelpoly(x, 39.0, 1.0, 140.0, 321.0, 0.0, 0.0, 182.0, 0.0, 0.0);
    kernels[38] = compute_kernelpoly(x, 47.0, 1.0, 138.0, 257.0, 0.0, 0.0, 156.0, 0.0, 0.0);
    kernels[39] = compute_kernelpoly(x, 66.0, 1.0, 120.0, 302.0, 0.0, 0.0, 151.0, 0.0, 0.0);
    kernels[40] = compute_kernelpoly(x, 62.0, 1.0, 130.0, 231.0, 0.0, 1.0, 146.0, 0.0, 3.0);
    kernels[41] = compute_kernelpoly(x, 41.0, 1.0, 135.0, 203.0, 0.0, 1.0, 132.0, 0.0, 0.0);
    kernels[42] = compute_kernelpoly(x, 51.0, 1.0, 100.0, 222.0, 0.0, 1.0, 143.0, 1.0, 0.0);
    kernels[43] = compute_kernelpoly(x, 54.0, 1.0, 120.0, 258.0, 0.0, 0.0, 147.0, 0.0, 0.0);
    kernels[44] = compute_kernelpoly(x, 51.0, 1.0, 94.0, 227.0, 0.0, 1.0, 154.0, 1.0, 1.0);
    kernels[45] = compute_kernelpoly(x, 51.0, 1.0, 140.0, 261.0, 0.0, 0.0, 186.0, 1.0, 0.0);
    kernels[46] = compute_kernelpoly(x, 59.0, 1.0, 140.0, 221.0, 0.0, 1.0, 164.0, 1.0, 0.0);
    kernels[47] = compute_kernelpoly(x, 58.0, 1.0, 105.0, 240.0, 0.0, 0.0, 154.0, 1.0, 0.0);
    kernels[48] = compute_kernelpoly(x, 41.0, 1.0, 112.0, 250.0, 0.0, 1.0, 179.0, 0.0, 0.0);
    kernels[49] = compute_kernelpoly(x, 68.0, 1.0, 118.0, 277.0, 0.0, 1.0, 151.0, 0.0, 1.0);
    kernels[50] = compute_kernelpoly(x, 52.0, 1.0, 138.0, 223.0, 0.0, 1.0, 169.0, 0.0, 4.0);
    kernels[51] = compute_kernelpoly(x, 54.0, 0.0, 132.0, 288.0, 1.0, 0.0, 159.0, 1.0, 1.0);
    kernels[52] = compute_kernelpoly(x, 53.0, 1.0, 142.0, 226.0, 0.0, 0.0, 111.0, 1.0, 0.0);
    kernels[53] = compute_kernelpoly(x, 52.0, 1.0, 108.0, 233.0, 1.0, 1.0, 147.0, 0.0, 3.0);
    kernels[54] = compute_kernelpoly(x, 43.0, 1.0, 130.0, 315.0, 0.0, 1.0, 162.0, 0.0, 1.0);
    kernels[55] = compute_kernelpoly(x, 42.0, 1.0, 148.0, 244.0, 0.0, 0.0, 178.0, 0.0, 2.0);
    kernels[56] = compute_kernelpoly(x, 59.0, 1.0, 178.0, 270.0, 0.0, 0.0, 145.0, 0.0, 0.0);
    kernels[57] = compute_kernelpoly(x, 68.0, 0.0, 120.0, 211.0, 0.0, 0.0, 115.0, 0.0, 0.0);
    kernels[58] = compute_kernelpoly(x, 69.0, 1.0, 160.0, 234.0, 1.0, 0.0, 131.0, 0.0, 1.0);
    kernels[59] = compute_kernelpoly(x, 45.0, 0.0, 138.0, 236.0, 0.0, 0.0, 152.0, 1.0, 0.0);
    kernels[60] = compute_kernelpoly(x, 64.0, 0.0, 180.0, 325.0, 0.0, 1.0, 154.0, 1.0, 0.0);
    float decision = 1.246732033997;
    decision = decision - (+kernels[0] * -0.25792644204 + kernels[1] * -0.086975602818 + kernels[2] * -1.0 + kernels[3] * -0.34357693487 + kernels[4] * -1.0 + kernels[5] * -1.0 + kernels[6] * -0.177101524746 + kernels[7] * -1.0 + kernels[8] * -1.0 + kernels[9] * -1.0 + kernels[10] * -1.0 + kernels[11] * -0.7693941541 + kernels[12] * -0.312119665282 + kernels[13] * -1.0 + kernels[14] * -0.040204636388 + kernels[15] * -0.505668486186 + kernels[16] * -0.417175939698 + kernels[17] * -1.0 + kernels[18] * -0.29917720442 + kernels[19] * -1.0 + kernels[20] * -1.0 + kernels[21] * -1.0 + kernels[22] * -1.0 + kernels[23] * -0.820318685885 + kernels[24] * -0.17240477281 + kernels[25] * -0.490545888904 + kernels[26] * -1.0 + kernels[27] * -1.0 + kernels[28] * -1.0 + kernels[29] * -1.0);
    decision = decision - (+kernels[30] * 0.749080958054 + kernels[31] * 0.456240568686 + kernels[32] * 1.0 + kernels[33] * 1.0 + kernels[34] * 0.137235903234 + kernels[35] * 0.089366605723 + kernels[36] * 0.882911205533 + kernels[37] * 0.45730649354 + kernels[38] * 1.0 + kernels[39] * 0.235330514468 + kernels[40] * 1.0 + kernels[41] * 1.0 + kernels[42] * 1.0 + kernels[43] * 1.0 + kernels[44] * 1.0 + kernels[45] * 0.804996797786 + kernels[46] * 0.598912778962 + kernels[47] * 1.0 + kernels[48] * 0.705163893817 + kernels[49] * 1.0 + kernels[50] * 0.771768239421 + kernels[51] * 0.357337893416 + kernels[52] * 1.0 + kernels[53] * 1.0 + kernels[54] * 0.34984886983 + kernels[55] * 0.143375332417 + kernels[56] * 1.0 + kernels[57] * 0.02446405933 + kernels[58] * 1.0 + kernels[59] * 0.013954298553 + kernels[60] * 0.915295525377);

    return decision > 0 ? 0 : 1;
}

float compute_kernelrbf(int* x, float data1, float data2, float data3, float data4, float data5, float data6, float data7, float data8, float data9)
{
    float kernel = 0.0;

    kernel += (float)((x[0] - data1) * (x[0] - data1));
    kernel += (float)((x[1] - data2) * (x[1] - data2));
    kernel += (float)((x[2] - data3) * (x[2] - data3));
    kernel += (float)((x[3] - data4) * (x[3] - data4));
    kernel += (float)((x[4] - data5) * (x[4] - data5));
    kernel += (float)((x[5] - data6) * (x[5] - data6));
    kernel += (float)((x[6] - data7) * (x[6] - data7));
    kernel += (float)((x[7] - data8) * (x[7] - data8));
    kernel += (float)((x[8] - data9) * (x[8] - data9));


    return exp(-0.001 * kernel);
}

int predictrbf(int* x)
{
    float kernels[113] = { 0 };
    kernels[0] = compute_kernelrbf(x, 43.0, 0.0, 132.0, 341.0, 1.0, 0.0, 136.0, 1.0, 0.0);
    kernels[1] = compute_kernelrbf(x, 63.0, 1.0, 130.0, 330.0, 1.0, 0.0, 132.0, 1.0, 3.0);
    kernels[2] = compute_kernelrbf(x, 65.0, 1.0, 135.0, 254.0, 0.0, 0.0, 127.0, 0.0, 1.0);
    kernels[3] = compute_kernelrbf(x, 48.0, 1.0, 130.0, 256.0, 1.0, 0.0, 150.0, 1.0, 2.0);
    kernels[4] = compute_kernelrbf(x, 63.0, 0.0, 150.0, 407.0, 0.0, 0.0, 154.0, 0.0, 3.0);
    kernels[5] = compute_kernelrbf(x, 55.0, 1.0, 140.0, 217.0, 0.0, 1.0, 111.0, 1.0, 0.0);
    kernels[6] = compute_kernelrbf(x, 65.0, 1.0, 138.0, 282.0, 1.0, 0.0, 174.0, 0.0, 1.0);
    kernels[7] = compute_kernelrbf(x, 56.0, 0.0, 200.0, 288.0, 1.0, 0.0, 133.0, 1.0, 2.0);
    kernels[8] = compute_kernelrbf(x, 54.0, 1.0, 110.0, 239.0, 0.0, 1.0, 126.0, 1.0, 1.0);
    kernels[9] = compute_kernelrbf(x, 70.0, 1.0, 145.0, 174.0, 0.0, 1.0, 125.0, 1.0, 0.0);
    kernels[10] = compute_kernelrbf(x, 35.0, 1.0, 120.0, 198.0, 0.0, 1.0, 130.0, 1.0, 0.0);
    kernels[11] = compute_kernelrbf(x, 59.0, 1.0, 170.0, 288.0, 0.0, 0.0, 159.0, 0.0, 0.0);
    kernels[12] = compute_kernelrbf(x, 64.0, 1.0, 125.0, 309.0, 0.0, 1.0, 131.0, 1.0, 0.0);
    kernels[13] = compute_kernelrbf(x, 47.0, 1.0, 108.0, 243.0, 0.0, 1.0, 152.0, 0.0, 0.0);
    kernels[14] = compute_kernelrbf(x, 57.0, 1.0, 165.0, 289.0, 1.0, 0.0, 124.0, 0.0, 3.0);
    kernels[15] = compute_kernelrbf(x, 55.0, 1.0, 160.0, 289.0, 0.0, 0.0, 145.0, 1.0, 1.0);
    kernels[16] = compute_kernelrbf(x, 64.0, 1.0, 120.0, 246.0, 0.0, 0.0, 96.0, 1.0, 1.0);
    kernels[17] = compute_kernelrbf(x, 70.0, 1.0, 130.0, 322.0, 0.0, 0.0, 109.0, 0.0, 3.0);
    kernels[18] = compute_kernelrbf(x, 51.0, 1.0, 140.0, 299.0, 0.0, 1.0, 173.0, 1.0, 0.0);
    kernels[19] = compute_kernelrbf(x, 58.0, 1.0, 125.0, 300.0, 0.0, 0.0, 171.0, 0.0, 2.0);
    kernels[20] = compute_kernelrbf(x, 60.0, 1.0, 140.0, 293.0, 0.0, 0.0, 170.0, 0.0, 2.0);
    kernels[21] = compute_kernelrbf(x, 77.0, 1.0, 125.0, 304.0, 0.0, 0.0, 162.0, 1.0, 3.0);
    kernels[22] = compute_kernelrbf(x, 35.0, 1.0, 126.0, 282.0, 0.0, 0.0, 156.0, 1.0, 0.0);
    kernels[23] = compute_kernelrbf(x, 59.0, 0.0, 174.0, 249.0, 0.0, 1.0, 143.0, 1.0, 0.0);
    kernels[24] = compute_kernelrbf(x, 64.0, 1.0, 145.0, 212.0, 0.0, 0.0, 132.0, 0.0, 2.0);
    kernels[25] = compute_kernelrbf(x, 57.0, 1.0, 152.0, 274.0, 0.0, 1.0, 88.0, 1.0, 1.0);
    kernels[26] = compute_kernelrbf(x, 56.0, 1.0, 132.0, 184.0, 0.0, 0.0, 105.0, 1.0, 1.0);
    kernels[27] = compute_kernelrbf(x, 48.0, 1.0, 124.0, 274.0, 0.0, 0.0, 166.0, 0.0, 0.0);
    kernels[28] = compute_kernelrbf(x, 56.0, 0.0, 134.0, 409.0, 0.0, 0.0, 150.0, 1.0, 2.0);
    kernels[29] = compute_kernelrbf(x, 66.0, 1.0, 160.0, 246.0, 0.0, 1.0, 120.0, 1.0, 3.0);
    kernels[30] = compute_kernelrbf(x, 54.0, 1.0, 192.0, 283.0, 0.0, 0.0, 195.0, 0.0, 1.0);
    kernels[31] = compute_kernelrbf(x, 69.0, 1.0, 140.0, 254.0, 0.0, 0.0, 146.0, 0.0, 3.0);
    kernels[32] = compute_kernelrbf(x, 51.0, 1.0, 140.0, 298.0, 0.0, 1.0, 122.0, 1.0, 3.0);
    kernels[33] = compute_kernelrbf(x, 43.0, 1.0, 132.0, 247.0, 1.0, 0.0, 143.0, 1.0, 4.0);
    kernels[34] = compute_kernelrbf(x, 67.0, 1.0, 100.0, 299.0, 0.0, 0.0, 125.0, 1.0, 2.0);
    kernels[35] = compute_kernelrbf(x, 45.0, 1.0, 142.0, 309.0, 0.0, 0.0, 147.0, 1.0, 3.0);
    kernels[36] = compute_kernelrbf(x, 58.0, 1.0, 128.0, 259.0, 0.0, 0.0, 130.0, 1.0, 2.0);
    kernels[37] = compute_kernelrbf(x, 50.0, 1.0, 144.0, 200.0, 0.0, 0.0, 126.0, 1.0, 0.0);
    kernels[38] = compute_kernelrbf(x, 62.0, 0.0, 150.0, 244.0, 0.0, 1.0, 154.0, 1.0, 0.0);
    kernels[39] = compute_kernelrbf(x, 38.0, 1.0, 120.0, 231.0, 0.0, 1.0, 182.0, 1.0, 0.0);
    kernels[40] = compute_kernelrbf(x, 66.0, 0.0, 178.0, 228.0, 1.0, 1.0, 165.0, 1.0, 2.0);
    kernels[41] = compute_kernelrbf(x, 52.0, 1.0, 112.0, 230.0, 0.0, 1.0, 160.0, 0.0, 1.0);
    kernels[42] = compute_kernelrbf(x, 63.0, 0.0, 108.0, 269.0, 0.0, 1.0, 169.0, 1.0, 2.0);
    kernels[43] = compute_kernelrbf(x, 54.0, 1.0, 110.0, 206.0, 0.0, 0.0, 108.0, 1.0, 1.0);
    kernels[44] = compute_kernelrbf(x, 66.0, 1.0, 112.0, 212.0, 0.0, 0.0, 132.0, 1.0, 1.0);
    kernels[45] = compute_kernelrbf(x, 55.0, 0.0, 180.0, 327.0, 0.0, 2.0, 117.0, 1.0, 0.0);
    kernels[46] = compute_kernelrbf(x, 49.0, 1.0, 118.0, 149.0, 0.0, 0.0, 126.0, 0.0, 3.0);
    kernels[47] = compute_kernelrbf(x, 54.0, 1.0, 122.0, 286.0, 0.0, 0.0, 116.0, 1.0, 2.0);
    kernels[48] = compute_kernelrbf(x, 46.0, 1.0, 120.0, 249.0, 0.0, 0.0, 144.0, 0.0, 0.0);
    kernels[49] = compute_kernelrbf(x, 61.0, 1.0, 134.0, 234.0, 0.0, 1.0, 145.0, 0.0, 2.0);
    kernels[50] = compute_kernelrbf(x, 67.0, 1.0, 120.0, 237.0, 0.0, 1.0, 71.0, 0.0, 0.0);
    kernels[51] = compute_kernelrbf(x, 58.0, 1.0, 100.0, 234.0, 0.0, 1.0, 156.0, 0.0, 1.0);
    kernels[52] = compute_kernelrbf(x, 71.0, 0.0, 160.0, 302.0, 0.0, 1.0, 162.0, 0.0, 2.0);
    kernels[53] = compute_kernelrbf(x, 59.0, 1.0, 150.0, 212.0, 1.0, 1.0, 157.0, 0.0, 0.0);
    kernels[54] = compute_kernelrbf(x, 51.0, 1.0, 110.0, 175.0, 0.0, 1.0, 123.0, 0.0, 0.0);
    kernels[55] = compute_kernelrbf(x, 65.0, 0.0, 140.0, 417.0, 1.0, 0.0, 157.0, 0.0, 1.0);
    kernels[56] = compute_kernelrbf(x, 41.0, 0.0, 105.0, 198.0, 0.0, 1.0, 168.0, 0.0, 1.0);
    kernels[57] = compute_kernelrbf(x, 65.0, 1.0, 120.0, 177.0, 0.0, 1.0, 140.0, 0.0, 0.0);
    kernels[58] = compute_kernelrbf(x, 54.0, 1.0, 125.0, 273.0, 0.0, 0.0, 152.0, 0.0, 1.0);
    kernels[59] = compute_kernelrbf(x, 51.0, 1.0, 125.0, 213.0, 0.0, 0.0, 125.0, 1.0, 1.0);
    kernels[60] = compute_kernelrbf(x, 46.0, 0.0, 142.0, 177.0, 0.0, 0.0, 160.0, 1.0, 0.0);
    kernels[61] = compute_kernelrbf(x, 54.0, 0.0, 135.0, 304.0, 1.0, 1.0, 170.0, 0.0, 0.0);
    kernels[62] = compute_kernelrbf(x, 54.0, 1.0, 150.0, 232.0, 0.0, 0.0, 165.0, 0.0, 0.0);
    kernels[63] = compute_kernelrbf(x, 65.0, 0.0, 155.0, 269.0, 0.0, 1.0, 148.0, 0.0, 0.0);
    kernels[64] = compute_kernelrbf(x, 65.0, 0.0, 160.0, 360.0, 0.0, 0.0, 151.0, 0.0, 0.0);
    kernels[65] = compute_kernelrbf(x, 51.0, 0.0, 140.0, 308.0, 0.0, 0.0, 142.0, 0.0, 1.0);
    kernels[66] = compute_kernelrbf(x, 53.0, 0.0, 130.0, 264.0, 0.0, 0.0, 143.0, 0.0, 0.0);
    kernels[67] = compute_kernelrbf(x, 39.0, 1.0, 140.0, 321.0, 0.0, 0.0, 182.0, 0.0, 0.0);
    kernels[68] = compute_kernelrbf(x, 47.0, 1.0, 138.0, 257.0, 0.0, 0.0, 156.0, 0.0, 0.0);
    kernels[69] = compute_kernelrbf(x, 53.0, 0.0, 128.0, 216.0, 0.0, 0.0, 115.0, 0.0, 0.0);
    kernels[70] = compute_kernelrbf(x, 51.0, 0.0, 130.0, 256.0, 0.0, 0.0, 149.0, 0.0, 0.0);
    kernels[71] = compute_kernelrbf(x, 66.0, 1.0, 120.0, 302.0, 0.0, 0.0, 151.0, 0.0, 0.0);
    kernels[72] = compute_kernelrbf(x, 62.0, 1.0, 130.0, 231.0, 0.0, 1.0, 146.0, 0.0, 3.0);
    kernels[73] = compute_kernelrbf(x, 44.0, 0.0, 108.0, 141.0, 0.0, 1.0, 175.0, 0.0, 0.0);
    kernels[74] = compute_kernelrbf(x, 63.0, 0.0, 135.0, 252.0, 0.0, 0.0, 172.0, 0.0, 0.0);
    kernels[75] = compute_kernelrbf(x, 48.0, 1.0, 122.0, 222.0, 0.0, 0.0, 186.0, 0.0, 0.0);
    kernels[76] = compute_kernelrbf(x, 45.0, 1.0, 115.0, 260.0, 0.0, 0.0, 185.0, 0.0, 0.0);
    kernels[77] = compute_kernelrbf(x, 57.0, 0.0, 128.0, 303.0, 0.0, 0.0, 159.0, 0.0, 1.0);
    kernels[78] = compute_kernelrbf(x, 71.0, 0.0, 110.0, 265.0, 1.0, 0.0, 130.0, 0.0, 1.0);
    kernels[79] = compute_kernelrbf(x, 52.0, 1.0, 118.0, 186.0, 0.0, 0.0, 190.0, 0.0, 0.0);
    kernels[80] = compute_kernelrbf(x, 41.0, 1.0, 135.0, 203.0, 0.0, 1.0, 132.0, 0.0, 0.0);
    kernels[81] = compute_kernelrbf(x, 35.0, 0.0, 138.0, 183.0, 0.0, 1.0, 182.0, 0.0, 0.0);
    kernels[82] = compute_kernelrbf(x, 51.0, 1.0, 100.0, 222.0, 0.0, 1.0, 143.0, 1.0, 0.0);
    kernels[83] = compute_kernelrbf(x, 54.0, 1.0, 120.0, 258.0, 0.0, 0.0, 147.0, 0.0, 0.0);
    kernels[84] = compute_kernelrbf(x, 51.0, 1.0, 94.0, 227.0, 0.0, 1.0, 154.0, 1.0, 1.0);
    kernels[85] = compute_kernelrbf(x, 29.0, 1.0, 130.0, 204.0, 0.0, 0.0, 202.0, 0.0, 0.0);
    kernels[86] = compute_kernelrbf(x, 51.0, 1.0, 140.0, 261.0, 0.0, 0.0, 186.0, 1.0, 0.0);
    kernels[87] = compute_kernelrbf(x, 58.0, 1.0, 105.0, 240.0, 0.0, 0.0, 154.0, 1.0, 0.0);
    kernels[88] = compute_kernelrbf(x, 41.0, 1.0, 112.0, 250.0, 0.0, 1.0, 179.0, 0.0, 0.0);
    kernels[89] = compute_kernelrbf(x, 45.0, 1.0, 128.0, 308.0, 0.0, 0.0, 170.0, 0.0, 0.0);
    kernels[90] = compute_kernelrbf(x, 60.0, 0.0, 102.0, 318.0, 0.0, 1.0, 160.0, 0.0, 1.0);
    kernels[91] = compute_kernelrbf(x, 52.0, 1.0, 152.0, 298.0, 1.0, 1.0, 178.0, 0.0, 0.0);
    kernels[92] = compute_kernelrbf(x, 42.0, 0.0, 102.0, 265.0, 0.0, 0.0, 122.0, 0.0, 0.0);
    kernels[93] = compute_kernelrbf(x, 67.0, 0.0, 115.0, 564.0, 0.0, 0.0, 160.0, 0.0, 0.0);
    kernels[94] = compute_kernelrbf(x, 68.0, 1.0, 118.0, 277.0, 0.0, 1.0, 151.0, 0.0, 1.0);
    kernels[95] = compute_kernelrbf(x, 58.0, 0.0, 100.0, 248.0, 0.0, 0.0, 122.0, 0.0, 0.0);
    kernels[96] = compute_kernelrbf(x, 54.0, 0.0, 132.0, 288.0, 1.0, 0.0, 159.0, 1.0, 1.0);
    kernels[97] = compute_kernelrbf(x, 45.0, 0.0, 112.0, 160.0, 0.0, 1.0, 138.0, 0.0, 0.0);
    kernels[98] = compute_kernelrbf(x, 53.0, 1.0, 142.0, 226.0, 0.0, 0.0, 111.0, 1.0, 0.0);
    kernels[99] = compute_kernelrbf(x, 62.0, 0.0, 140.0, 394.0, 0.0, 0.0, 157.0, 0.0, 0.0);
    kernels[100] = compute_kernelrbf(x, 52.0, 1.0, 108.0, 233.0, 1.0, 1.0, 147.0, 0.0, 3.0);
    kernels[101] = compute_kernelrbf(x, 43.0, 1.0, 130.0, 315.0, 0.0, 1.0, 162.0, 0.0, 1.0);
    kernels[102] = compute_kernelrbf(x, 59.0, 1.0, 178.0, 270.0, 0.0, 0.0, 145.0, 0.0, 0.0);
    kernels[103] = compute_kernelrbf(x, 42.0, 1.0, 120.0, 240.0, 1.0, 1.0, 194.0, 0.0, 0.0);
    kernels[104] = compute_kernelrbf(x, 68.0, 0.0, 120.0, 211.0, 0.0, 0.0, 115.0, 0.0, 0.0);
    kernels[105] = compute_kernelrbf(x, 69.0, 1.0, 160.0, 234.0, 1.0, 0.0, 131.0, 0.0, 1.0);
    kernels[106] = compute_kernelrbf(x, 45.0, 0.0, 138.0, 236.0, 0.0, 0.0, 152.0, 1.0, 0.0);
    kernels[107] = compute_kernelrbf(x, 50.0, 0.0, 110.0, 254.0, 0.0, 0.0, 159.0, 0.0, 0.0);
    kernels[108] = compute_kernelrbf(x, 64.0, 0.0, 180.0, 325.0, 0.0, 1.0, 154.0, 1.0, 0.0);
    kernels[109] = compute_kernelrbf(x, 57.0, 1.0, 150.0, 126.0, 1.0, 1.0, 173.0, 0.0, 1.0);
    kernels[110] = compute_kernelrbf(x, 64.0, 0.0, 140.0, 313.0, 0.0, 1.0, 133.0, 0.0, 0.0);
    kernels[111] = compute_kernelrbf(x, 43.0, 1.0, 110.0, 211.0, 0.0, 1.0, 161.0, 0.0, 0.0);
    kernels[112] = compute_kernelrbf(x, 55.0, 1.0, 130.0, 262.0, 0.0, 1.0, 155.0, 0.0, 0.0);
    float decision = 0.083698788933;
    decision = decision - (+kernels[0] * -0.979425636318 + kernels[1] * -1.0 + kernels[2] * -0.923772241859 + kernels[3] * -1.0 + kernels[4] * -1.0 + kernels[5] * -1.0 + kernels[6] * -1.0 + kernels[7] * -0.998607697772 + kernels[8] * -1.0 + kernels[9] * -1.0 + kernels[10] * -1.0 + kernels[11] * -1.0 + kernels[12] * -1.0 + kernels[13] * -1.0 + kernels[14] * -0.239282741738 + kernels[15] * -1.0 + kernels[16] * -0.727377990159 + kernels[17] * -0.104677287011 + kernels[18] * -1.0 + kernels[19] * -1.0 + kernels[20] * -1.0 + kernels[21] * -1.0 + kernels[22] * -1.0 + kernels[23] * -1.0 + kernels[24] * -1.0 + kernels[25] * -0.533404884812 + kernels[26] * -0.909785480378 + kernels[27] * -1.0 + kernels[28] * -1.0 + kernels[29] * -0.627223410145 + kernels[30] * -1.0 + kernels[31] * -1.0 + kernels[32] * -0.298753007927 + kernels[33] * -1.0 + kernels[34] * -1.0 + kernels[35] * -1.0 + kernels[36] * -1.0 + kernels[37] * -1.0 + kernels[38] * -1.0 + kernels[39] * -1.0 + kernels[40] * -1.0 + kernels[41] * -1.0 + kernels[42] * -1.0 + kernels[43] * -1.0 + kernels[44] * -1.0 + kernels[45] * -1.0 + kernels[46] * -1.0 + kernels[47] * -0.885163696969 + kernels[48] * -1.0 + kernels[49] * -1.0 + kernels[50] * -0.740157410775 + kernels[51] * -1.0);
    decision = decision - (+kernels[52] * 1.0 + kernels[53] * 1.0 + kernels[54] * 1.0 + kernels[55] * 1.0 + kernels[56] * 0.239098211829 + kernels[57] * 0.920659533243 + kernels[58] * 0.426477540893 + kernels[59] * 1.0 + kernels[60] * 0.350467136485 + kernels[61] * 1.0 + kernels[62] * 0.813794343426 + kernels[63] * 1.0 + kernels[64] * 0.872004530464 + kernels[65] * 1.0 + kernels[66] * 1.0 + kernels[67] * 0.421135179743 + kernels[68] * 1.0 + kernels[69] * 1.0 + kernels[70] * 1.0 + kernels[71] * 1.0 + kernels[72] * 1.0 + kernels[73] * 0.650070817156 + kernels[74] * 0.14084859128 + kernels[75] * 0.478865999545 + kernels[76] * 0.831035842933 + kernels[77] * 1.0 + kernels[78] * 1.0 + kernels[79] * 0.254564338039 + kernels[80] * 1.0 + kernels[81] * 0.021800154731 + kernels[82] * 0.565568690721 + kernels[83] * 1.0 + kernels[84] * 0.995179973626 + kernels[85] * 0.732957061401 + kernels[86] * 0.721290527228 + kernels[87] * 1.0 + kernels[88] * 0.779732971165 + kernels[89] * 0.590146482059 + kernels[90] * 0.974321437455 + kernels[91] * 1.0 + kernels[92] * 1.0 + kernels[93] * 0.916432766177 + kernels[94] * 0.592991135025 + kernels[95] * 1.0 + kernels[96] * 1.0 + kernels[97] * 0.74980395788 + kernels[98] * 1.0 + kernels[99] * 1.0 + kernels[100] * 1.0 + kernels[101] * 0.142323563049 + kernels[102] * 1.0 + kernels[103] * 0.018715971427 + kernels[104] * 1.0 + kernels[105] * 1.0 + kernels[106] * 1.0 + kernels[107] * 0.172531978192 + kernels[108] * 0.991279765101 + kernels[109] * 0.826850995607 + kernels[110] * 1.0 + kernels[111] * 0.080462311929 + kernels[112] * 0.696219678057);

    return decision > 0 ? 0 : 1;
}

float compute_kernelsig(int* x, float data1, float data2, float data3, float data4, float data5, float data6, float data7, float data8, float data9)
{
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

    return (exp(0.001 * kernel) / (exp(0.001 * kernel) + 1));
}


int predictsig(int* x)
{
    float kernels[118] = { 0 };
    kernels[0] = compute_kernelsig(x, 43.0, 0.0, 132.0, 341.0, 1.0, 0.0, 136.0, 1.0, 0.0);
    kernels[1] = compute_kernelsig(x, 62.0, 0.0, 130.0, 263.0, 0.0, 1.0, 97.0, 0.0, 1.0);
    kernels[2] = compute_kernelsig(x, 63.0, 1.0, 130.0, 330.0, 1.0, 0.0, 132.0, 1.0, 3.0);
    kernels[3] = compute_kernelsig(x, 65.0, 1.0, 135.0, 254.0, 0.0, 0.0, 127.0, 0.0, 1.0);
    kernels[4] = compute_kernelsig(x, 48.0, 1.0, 130.0, 256.0, 1.0, 0.0, 150.0, 1.0, 2.0);
    kernels[5] = compute_kernelsig(x, 63.0, 0.0, 150.0, 407.0, 0.0, 0.0, 154.0, 0.0, 3.0);
    kernels[6] = compute_kernelsig(x, 55.0, 1.0, 140.0, 217.0, 0.0, 1.0, 111.0, 1.0, 0.0);
    kernels[7] = compute_kernelsig(x, 65.0, 1.0, 138.0, 282.0, 1.0, 0.0, 174.0, 0.0, 1.0);
    kernels[8] = compute_kernelsig(x, 56.0, 0.0, 200.0, 288.0, 1.0, 0.0, 133.0, 1.0, 2.0);
    kernels[9] = compute_kernelsig(x, 54.0, 1.0, 110.0, 239.0, 0.0, 1.0, 126.0, 1.0, 1.0);
    kernels[10] = compute_kernelsig(x, 70.0, 1.0, 145.0, 174.0, 0.0, 1.0, 125.0, 1.0, 0.0);
    kernels[11] = compute_kernelsig(x, 62.0, 1.0, 120.0, 281.0, 0.0, 0.0, 103.0, 0.0, 1.0);
    kernels[12] = compute_kernelsig(x, 35.0, 1.0, 120.0, 198.0, 0.0, 1.0, 130.0, 1.0, 0.0);
    kernels[13] = compute_kernelsig(x, 59.0, 1.0, 170.0, 288.0, 0.0, 0.0, 159.0, 0.0, 0.0);
    kernels[14] = compute_kernelsig(x, 64.0, 1.0, 125.0, 309.0, 0.0, 1.0, 131.0, 1.0, 0.0);
    kernels[15] = compute_kernelsig(x, 47.0, 1.0, 108.0, 243.0, 0.0, 1.0, 152.0, 0.0, 0.0);
    kernels[16] = compute_kernelsig(x, 57.0, 1.0, 165.0, 289.0, 1.0, 0.0, 124.0, 0.0, 3.0);
    kernels[17] = compute_kernelsig(x, 55.0, 1.0, 160.0, 289.0, 0.0, 0.0, 145.0, 1.0, 1.0);
    kernels[18] = compute_kernelsig(x, 64.0, 1.0, 120.0, 246.0, 0.0, 0.0, 96.0, 1.0, 1.0);
    kernels[19] = compute_kernelsig(x, 70.0, 1.0, 130.0, 322.0, 0.0, 0.0, 109.0, 0.0, 3.0);
    kernels[20] = compute_kernelsig(x, 51.0, 1.0, 140.0, 299.0, 0.0, 1.0, 173.0, 1.0, 0.0);
    kernels[21] = compute_kernelsig(x, 58.0, 1.0, 125.0, 300.0, 0.0, 0.0, 171.0, 0.0, 2.0);
    kernels[22] = compute_kernelsig(x, 60.0, 1.0, 140.0, 293.0, 0.0, 0.0, 170.0, 0.0, 2.0);
    kernels[23] = compute_kernelsig(x, 77.0, 1.0, 125.0, 304.0, 0.0, 0.0, 162.0, 1.0, 3.0);
    kernels[24] = compute_kernelsig(x, 35.0, 1.0, 126.0, 282.0, 0.0, 0.0, 156.0, 1.0, 0.0);
    kernels[25] = compute_kernelsig(x, 70.0, 1.0, 160.0, 269.0, 0.0, 1.0, 112.0, 1.0, 1.0);
    kernels[26] = compute_kernelsig(x, 59.0, 0.0, 174.0, 249.0, 0.0, 1.0, 143.0, 1.0, 0.0);
    kernels[27] = compute_kernelsig(x, 64.0, 1.0, 145.0, 212.0, 0.0, 0.0, 132.0, 0.0, 2.0);
    kernels[28] = compute_kernelsig(x, 57.0, 1.0, 152.0, 274.0, 0.0, 1.0, 88.0, 1.0, 1.0);
    kernels[29] = compute_kernelsig(x, 56.0, 1.0, 132.0, 184.0, 0.0, 0.0, 105.0, 1.0, 1.0);
    kernels[30] = compute_kernelsig(x, 48.0, 1.0, 124.0, 274.0, 0.0, 0.0, 166.0, 0.0, 0.0);
    kernels[31] = compute_kernelsig(x, 56.0, 0.0, 134.0, 409.0, 0.0, 0.0, 150.0, 1.0, 2.0);
    kernels[32] = compute_kernelsig(x, 66.0, 1.0, 160.0, 246.0, 0.0, 1.0, 120.0, 1.0, 3.0);
    kernels[33] = compute_kernelsig(x, 54.0, 1.0, 192.0, 283.0, 0.0, 0.0, 195.0, 0.0, 1.0);
    kernels[34] = compute_kernelsig(x, 69.0, 1.0, 140.0, 254.0, 0.0, 0.0, 146.0, 0.0, 3.0);
    kernels[35] = compute_kernelsig(x, 51.0, 1.0, 140.0, 298.0, 0.0, 1.0, 122.0, 1.0, 3.0);
    kernels[36] = compute_kernelsig(x, 43.0, 1.0, 132.0, 247.0, 1.0, 0.0, 143.0, 1.0, 4.0);
    kernels[37] = compute_kernelsig(x, 62.0, 0.0, 138.0, 294.0, 1.0, 1.0, 106.0, 0.0, 3.0);
    kernels[38] = compute_kernelsig(x, 67.0, 1.0, 100.0, 299.0, 0.0, 0.0, 125.0, 1.0, 2.0);
    kernels[39] = compute_kernelsig(x, 59.0, 1.0, 160.0, 273.0, 0.0, 0.0, 125.0, 0.0, 0.0);
    kernels[40] = compute_kernelsig(x, 45.0, 1.0, 142.0, 309.0, 0.0, 0.0, 147.0, 1.0, 3.0);
    kernels[41] = compute_kernelsig(x, 58.0, 1.0, 128.0, 259.0, 0.0, 0.0, 130.0, 1.0, 2.0);
    kernels[42] = compute_kernelsig(x, 50.0, 1.0, 144.0, 200.0, 0.0, 0.0, 126.0, 1.0, 0.0);
    kernels[43] = compute_kernelsig(x, 62.0, 0.0, 150.0, 244.0, 0.0, 1.0, 154.0, 1.0, 0.0);
    kernels[44] = compute_kernelsig(x, 38.0, 1.0, 120.0, 231.0, 0.0, 1.0, 182.0, 1.0, 0.0);
    kernels[45] = compute_kernelsig(x, 66.0, 0.0, 178.0, 228.0, 1.0, 1.0, 165.0, 1.0, 2.0);
    kernels[46] = compute_kernelsig(x, 52.0, 1.0, 112.0, 230.0, 0.0, 1.0, 160.0, 0.0, 1.0);
    kernels[47] = compute_kernelsig(x, 53.0, 1.0, 123.0, 282.0, 0.0, 1.0, 95.0, 1.0, 2.0);
    kernels[48] = compute_kernelsig(x, 63.0, 0.0, 108.0, 269.0, 0.0, 1.0, 169.0, 1.0, 2.0);
    kernels[49] = compute_kernelsig(x, 54.0, 1.0, 110.0, 206.0, 0.0, 0.0, 108.0, 1.0, 1.0);
    kernels[50] = compute_kernelsig(x, 66.0, 1.0, 112.0, 212.0, 0.0, 0.0, 132.0, 1.0, 1.0);
    kernels[51] = compute_kernelsig(x, 55.0, 0.0, 180.0, 327.0, 0.0, 2.0, 117.0, 1.0, 0.0);
    kernels[52] = compute_kernelsig(x, 49.0, 1.0, 118.0, 149.0, 0.0, 0.0, 126.0, 0.0, 3.0);
    kernels[53] = compute_kernelsig(x, 54.0, 1.0, 122.0, 286.0, 0.0, 0.0, 116.0, 1.0, 2.0);
    kernels[54] = compute_kernelsig(x, 56.0, 1.0, 130.0, 283.0, 1.0, 0.0, 103.0, 1.0, 0.0);
    kernels[55] = compute_kernelsig(x, 46.0, 1.0, 120.0, 249.0, 0.0, 0.0, 144.0, 0.0, 0.0);
    kernels[56] = compute_kernelsig(x, 61.0, 1.0, 134.0, 234.0, 0.0, 1.0, 145.0, 0.0, 2.0);
    kernels[57] = compute_kernelsig(x, 67.0, 1.0, 120.0, 237.0, 0.0, 1.0, 71.0, 0.0, 0.0);
    kernels[58] = compute_kernelsig(x, 58.0, 1.0, 100.0, 234.0, 0.0, 1.0, 156.0, 0.0, 1.0);
    kernels[59] = compute_kernelsig(x, 48.0, 1.0, 122.0, 222.0, 0.0, 0.0, 186.0, 0.0, 0.0);
    kernels[60] = compute_kernelsig(x, 45.0, 1.0, 115.0, 260.0, 0.0, 0.0, 185.0, 0.0, 0.0);
    kernels[61] = compute_kernelsig(x, 34.0, 1.0, 118.0, 182.0, 0.0, 0.0, 174.0, 0.0, 0.0);
    kernels[62] = compute_kernelsig(x, 57.0, 0.0, 128.0, 303.0, 0.0, 0.0, 159.0, 0.0, 1.0);
    kernels[63] = compute_kernelsig(x, 71.0, 0.0, 110.0, 265.0, 1.0, 0.0, 130.0, 0.0, 1.0);
    kernels[64] = compute_kernelsig(x, 54.0, 1.0, 108.0, 309.0, 0.0, 1.0, 156.0, 0.0, 0.0);
    kernels[65] = compute_kernelsig(x, 52.0, 1.0, 118.0, 186.0, 0.0, 0.0, 190.0, 0.0, 0.0);
    kernels[66] = compute_kernelsig(x, 41.0, 1.0, 135.0, 203.0, 0.0, 1.0, 132.0, 0.0, 0.0);
    kernels[67] = compute_kernelsig(x, 58.0, 1.0, 140.0, 211.0, 1.0, 0.0, 165.0, 0.0, 0.0);
    kernels[68] = compute_kernelsig(x, 35.0, 0.0, 138.0, 183.0, 0.0, 1.0, 182.0, 0.0, 0.0);
    kernels[69] = compute_kernelsig(x, 51.0, 1.0, 100.0, 222.0, 0.0, 1.0, 143.0, 1.0, 0.0);
    kernels[70] = compute_kernelsig(x, 45.0, 0.0, 130.0, 234.0, 0.0, 0.0, 175.0, 0.0, 0.0);
    kernels[71] = compute_kernelsig(x, 44.0, 1.0, 120.0, 220.0, 0.0, 1.0, 170.0, 0.0, 0.0);
    kernels[72] = compute_kernelsig(x, 62.0, 0.0, 124.0, 209.0, 0.0, 1.0, 163.0, 0.0, 0.0);
    kernels[73] = compute_kernelsig(x, 54.0, 1.0, 120.0, 258.0, 0.0, 0.0, 147.0, 0.0, 0.0);
    kernels[74] = compute_kernelsig(x, 51.0, 1.0, 94.0, 227.0, 0.0, 1.0, 154.0, 1.0, 1.0);
    kernels[75] = compute_kernelsig(x, 29.0, 1.0, 130.0, 204.0, 0.0, 0.0, 202.0, 0.0, 0.0);
    kernels[76] = compute_kernelsig(x, 51.0, 1.0, 140.0, 261.0, 0.0, 0.0, 186.0, 1.0, 0.0);
    kernels[77] = compute_kernelsig(x, 43.0, 0.0, 122.0, 213.0, 0.0, 1.0, 165.0, 0.0, 0.0);
    kernels[78] = compute_kernelsig(x, 55.0, 0.0, 135.0, 250.0, 0.0, 0.0, 161.0, 0.0, 0.0);
    kernels[79] = compute_kernelsig(x, 51.0, 1.0, 125.0, 245.0, 1.0, 0.0, 166.0, 0.0, 0.0);
    kernels[80] = compute_kernelsig(x, 59.0, 1.0, 140.0, 221.0, 0.0, 1.0, 164.0, 1.0, 0.0);
    kernels[81] = compute_kernelsig(x, 52.0, 1.0, 128.0, 205.0, 1.0, 1.0, 184.0, 0.0, 0.0);
    kernels[82] = compute_kernelsig(x, 58.0, 1.0, 105.0, 240.0, 0.0, 0.0, 154.0, 1.0, 0.0);
    kernels[83] = compute_kernelsig(x, 41.0, 1.0, 112.0, 250.0, 0.0, 1.0, 179.0, 0.0, 0.0);
    kernels[84] = compute_kernelsig(x, 45.0, 1.0, 128.0, 308.0, 0.0, 0.0, 170.0, 0.0, 0.0);
    kernels[85] = compute_kernelsig(x, 60.0, 0.0, 102.0, 318.0, 0.0, 1.0, 160.0, 0.0, 1.0);
    kernels[86] = compute_kernelsig(x, 52.0, 1.0, 152.0, 298.0, 1.0, 1.0, 178.0, 0.0, 0.0);
    kernels[87] = compute_kernelsig(x, 42.0, 0.0, 102.0, 265.0, 0.0, 0.0, 122.0, 0.0, 0.0);
    kernels[88] = compute_kernelsig(x, 67.0, 0.0, 115.0, 564.0, 0.0, 0.0, 160.0, 0.0, 0.0);
    kernels[89] = compute_kernelsig(x, 68.0, 1.0, 118.0, 277.0, 0.0, 1.0, 151.0, 0.0, 1.0);
    kernels[90] = compute_kernelsig(x, 46.0, 1.0, 101.0, 197.0, 1.0, 1.0, 156.0, 0.0, 0.0);
    kernels[91] = compute_kernelsig(x, 54.0, 0.0, 110.0, 214.0, 0.0, 1.0, 158.0, 0.0, 0.0);
    kernels[92] = compute_kernelsig(x, 58.0, 0.0, 100.0, 248.0, 0.0, 0.0, 122.0, 0.0, 0.0);
    kernels[93] = compute_kernelsig(x, 48.0, 1.0, 124.0, 255.0, 1.0, 1.0, 175.0, 0.0, 2.0);
    kernels[94] = compute_kernelsig(x, 57.0, 1.0, 132.0, 207.0, 0.0, 1.0, 168.0, 1.0, 0.0);
    kernels[95] = compute_kernelsig(x, 52.0, 1.0, 138.0, 223.0, 0.0, 1.0, 169.0, 0.0, 4.0);
    kernels[96] = compute_kernelsig(x, 54.0, 0.0, 132.0, 288.0, 1.0, 0.0, 159.0, 1.0, 1.0);
    kernels[97] = compute_kernelsig(x, 45.0, 0.0, 112.0, 160.0, 0.0, 1.0, 138.0, 0.0, 0.0);
    kernels[98] = compute_kernelsig(x, 53.0, 1.0, 142.0, 226.0, 0.0, 0.0, 111.0, 1.0, 0.0);
    kernels[99] = compute_kernelsig(x, 62.0, 0.0, 140.0, 394.0, 0.0, 0.0, 157.0, 0.0, 0.0);
    kernels[100] = compute_kernelsig(x, 52.0, 1.0, 108.0, 233.0, 1.0, 1.0, 147.0, 0.0, 3.0);
    kernels[101] = compute_kernelsig(x, 43.0, 1.0, 130.0, 315.0, 0.0, 1.0, 162.0, 0.0, 1.0);
    kernels[102] = compute_kernelsig(x, 53.0, 1.0, 130.0, 246.0, 1.0, 0.0, 173.0, 0.0, 3.0);
    kernels[103] = compute_kernelsig(x, 42.0, 1.0, 148.0, 244.0, 0.0, 0.0, 178.0, 0.0, 2.0);
    kernels[104] = compute_kernelsig(x, 59.0, 1.0, 178.0, 270.0, 0.0, 0.0, 145.0, 0.0, 0.0);
    kernels[105] = compute_kernelsig(x, 63.0, 0.0, 140.0, 195.0, 0.0, 1.0, 179.0, 0.0, 2.0);
    kernels[106] = compute_kernelsig(x, 42.0, 1.0, 120.0, 240.0, 1.0, 1.0, 194.0, 0.0, 0.0);
    kernels[107] = compute_kernelsig(x, 50.0, 1.0, 129.0, 196.0, 0.0, 1.0, 163.0, 0.0, 0.0);
    kernels[108] = compute_kernelsig(x, 68.0, 0.0, 120.0, 211.0, 0.0, 0.0, 115.0, 0.0, 0.0);
    kernels[109] = compute_kernelsig(x, 69.0, 1.0, 160.0, 234.0, 1.0, 0.0, 131.0, 0.0, 1.0);
    kernels[110] = compute_kernelsig(x, 45.0, 0.0, 138.0, 236.0, 0.0, 0.0, 152.0, 1.0, 0.0);
    kernels[111] = compute_kernelsig(x, 50.0, 0.0, 120.0, 244.0, 0.0, 1.0, 162.0, 0.0, 0.0);
    kernels[112] = compute_kernelsig(x, 50.0, 0.0, 110.0, 254.0, 0.0, 0.0, 159.0, 0.0, 0.0);
    kernels[113] = compute_kernelsig(x, 64.0, 0.0, 180.0, 325.0, 0.0, 1.0, 154.0, 1.0, 0.0);
    kernels[114] = compute_kernelsig(x, 57.0, 1.0, 150.0, 126.0, 1.0, 1.0, 173.0, 0.0, 1.0);
    kernels[115] = compute_kernelsig(x, 64.0, 0.0, 140.0, 313.0, 0.0, 1.0, 133.0, 0.0, 0.0);
    kernels[116] = compute_kernelsig(x, 43.0, 1.0, 110.0, 211.0, 0.0, 1.0, 161.0, 0.0, 0.0);
    kernels[117] = compute_kernelsig(x, 55.0, 1.0, 130.0, 262.0, 0.0, 1.0, 155.0, 0.0, 0.0);
    float decision = 1.0;
    decision = decision - (+kernels[0] * -1.0 + kernels[1] * -1.0 + kernels[2] * -1.0 + kernels[3] * -1.0 + kernels[4] * -1.0 + kernels[5] * -1.0 + kernels[6] * -1.0 + kernels[7] * -1.0 + kernels[8] * -1.0 + kernels[9] * -1.0 + kernels[10] * -1.0 + kernels[11] * -1.0 + kernels[12] * -1.0 + kernels[13] * -1.0 + kernels[14] * -1.0 + kernels[15] * -1.0 + kernels[16] * -1.0 + kernels[17] * -1.0 + kernels[18] * -1.0 + kernels[19] * -1.0 + kernels[20] * -1.0 + kernels[21] * -1.0 + kernels[22] * -1.0 + kernels[23] * -1.0 + kernels[24] * -1.0 + kernels[25] * -1.0 + kernels[26] * -1.0 + kernels[27] * -1.0 + kernels[28] * -1.0 + kernels[29] * -1.0 + kernels[30] * -1.0 + kernels[31] * -1.0 + kernels[32] * -1.0 + kernels[33] * -1.0 + kernels[34] * -1.0 + kernels[35] * -1.0 + kernels[36] * -1.0 + kernels[37] * -1.0 + kernels[38] * -1.0 + kernels[39] * -1.0 + kernels[40] * -1.0 + kernels[41] * -1.0 + kernels[42] * -1.0 + kernels[43] * -1.0 + kernels[44] * -1.0 + kernels[45] * -1.0 + kernels[46] * -1.0 + kernels[47] * -1.0 + kernels[48] * -1.0 + kernels[49] * -1.0 + kernels[50] * -1.0 + kernels[51] * -1.0 + kernels[52] * -1.0 + kernels[53] * -1.0 + kernels[54] * -1.0 + kernels[55] * -1.0 + kernels[56] * -1.0 + kernels[57] * -1.0 + kernels[58] * -1.0);
    decision = decision - (+kernels[59] * 1.0 + kernels[60] * 1.0 + kernels[61] * 1.0 + kernels[62] * 1.0 + kernels[63] * 1.0 + kernels[64] * 1.0 + kernels[65] * 1.0 + kernels[66] * 1.0 + kernels[67] * 1.0 + kernels[68] * 1.0 + kernels[69] * 1.0 + kernels[70] * 1.0 + kernels[71] * 1.0 + kernels[72] * 1.0 + kernels[73] * 1.0 + kernels[74] * 1.0 + kernels[75] * 1.0 + kernels[76] * 1.0 + kernels[77] * 1.0 + kernels[78] * 1.0 + kernels[79] * 1.0 + kernels[80] * 1.0 + kernels[81] * 1.0 + kernels[82] * 1.0 + kernels[83] * 1.0 + kernels[84] * 1.0 + kernels[85] * 1.0 + kernels[86] * 1.0 + kernels[87] * 1.0 + kernels[88] * 1.0 + kernels[89] * 1.0 + kernels[90] * 1.0 + kernels[91] * 1.0 + kernels[92] * 1.0 + kernels[93] * 1.0 + kernels[94] * 1.0 + kernels[95] * 1.0 + kernels[96] * 1.0 + kernels[97] * 1.0 + kernels[98] * 1.0 + kernels[99] * 1.0 + kernels[100] * 1.0 + kernels[101] * 1.0 + kernels[102] * 1.0 + kernels[103] * 1.0 + kernels[104] * 1.0 + kernels[105] * 1.0 + kernels[106] * 1.0 + kernels[107] * 1.0 + kernels[108] * 1.0 + kernels[109] * 1.0 + kernels[110] * 1.0 + kernels[111] * 1.0 + kernels[112] * 1.0 + kernels[113] * 1.0 + kernels[114] * 1.0 + kernels[115] * 1.0 + kernels[116] * 1.0 + kernels[117] * 1.0);

    return decision > 0 ? 0 : 1;
}

int idxToLabel(int classIdx) //데이터 즉 avr보드를 통해 입력한 값을 갖고 class를 매긴다 오류시 return 11로 0을띄움 
{
    switch (classIdx)
    {
    case 0:
        return 0;
    case 1:
        return 1;
    default:
        return 11;
    }
}

int main()
{
    DDRF = 0xF0;//11110000 F포트 출력 4개설정 
    PORTF = 0x00;//00000000 초기화
    DDRG = 0x00; //00000000 스위치 사용선언 
    init_port();
    init_interrupt();
    timer_setting();
    init_uart(); // Port & USART Initialization
    sei();
    while (1)
    {
        display(); //7세그먼트 계속 켜 
        if ((PING & 0x0F) == 0x0E) //스위치 5번 누르면 값을 증가시켜 굉장히 빠르게 증가시킴 즉 dim[i]의 값을 빠르게 증가시켜
        {
            if (flag == 1) //int 5번이 눌리면 flag가 1이되는데 이 의미는 dimension을 변경하면서 값을 입력하겠다라는 의미 
            {
                if (k == &svm)  //k가 svm의 주소가 되는경우는 i가 9가 되는경우임
                {
                    if ((*k) >= 3) //svm은 0~3까지 linear, poly, rbf, sigmoid 4개이므로 3보다 크면 증가못하게 함
                    {
                    }
                    else           //svm이 0~3사이에서는 
                    {
                        display(); //10ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                        (*k)++;     //k값 증가 즉 k가 svm의 주소를 가질때 sw5번이 눌리면 svm을 증가시킴 
                    }
                    if (svm == 0)       //svm에 따라서 led를 켬 
                        PORTF = 0x80;   //led4번 켬
                    else if (svm == 1)  //svm에 따라서 led를 켬
                        PORTF = 0x40;   //led3번 켬
                    else if (svm == 2)  //svm에 따라서 led를 켬
                        PORTF = 0x20;   //led2번 켬
                    else if (svm == 3)  //svm에 따라서 led를 켬
                        PORTF = 0x10;   //led1번 켬
                }
                else if (i == 1 || i == 4 || i == 7)  //dim[1]:sex, 0,1만 존재 ,dim[4]:Fbs, 0, 1만 존재 ,dim[7]:Exng, 0,1만 존재 따라서 유효범위 이상으로 못올라가게 제한 
                {
                    if ((*k) >= 1) //1보다 크면 못올라가게 제한
                    {
                    }
                    else
                    {
                        display(); //10ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                        (*k)++;    //k값 증가 즉 k가 dim[1]또는 dim[4] 또는 dim[7]의 주소를 가질때 sw5번이 눌리면 값을 증가시킴 이때 10ms delay이므로 굉장히 빠르게 증가 
                    }
                }
                else if (i == 5) //dim[5]:Restecg 으로 유효범위는 0~2까지만 따라서 2보다 커지지 못하게 증가제한 

                {
                    if ((*k) >= 2)
                    {
                    }
                    else
                    {
                        display(); //10ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                        (*k)++;
                    }
                }
                else if (i == 8)//dim[8]:Caa으로 유효범위는 0~4까지만 따라서 4보다 커지지 못하게 증가제한 
                {
                    if ((*k) >= 4)
                    {
                    }
                    else
                    {
                        display(); //10ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                        (*k)++;
                    }
                }
                else
                {
                    display(); //10ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                    (*k)++;
                }
            }
        }
        if ((PING & 0x0F) == 0x0D)   //스위치 6번 위와는 완전히 반대로 감소시키는데 굉장히 빠르게 감소시킴 
        {
            if (flag == 1)
            {
                if ((*k) <= 0)//k값은 0보다 작지 못하게 만듬 
                {
                }
                else
                {
                    display(); //10ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                    (*k)--;
                }
                if (k == &svm)  //k가 svm의 주소가 되는경우는 i가 9가 되는경우임
                {
                    if (svm == 0)       //svm에 따라서 led를 켬 
                        PORTF = 0x80;   //led4번 켬
                    else if (svm == 1)  //svm에 따라서 led를 켬
                        PORTF = 0x40;   //led3번 켬
                    else if (svm == 2)  //svm에 따라서 led를 켬
                        PORTF = 0x20;   //led2번 켬
                    else if (svm == 3)  //svm에 따라서 led를 켬
                        PORTF = 0x10;   //led1번 켬
                }
            }
        }
        if ((PING & 0x0F) == 0x0B)   //스위치 7번, 값을 증가시키는데 이번에는 delay를 200ms로 두어 천천히 증가하게 만듦
        {
            if (flag == 1)
            {
                if (k == &svm)
                {
                    if ((*k) >= 3)
                    {
                    }
                    else
                    {
                        for (int q = 0; q < 20; q++) {
                            display(); //200ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                        }
                        (*k)++;
                    }
                    if (svm == 0)       //svm에 따라서 led를 켬 
                        PORTF = 0x80;   //led4번 켬
                    else if (svm == 1)  //svm에 따라서 led를 켬
                        PORTF = 0x40;   //led3번 켬
                    else if (svm == 2)  //svm에 따라서 led를 켬
                        PORTF = 0x20;   //led2번 켬
                    else if (svm == 3)  //svm에 따라서 led를 켬
                        PORTF = 0x10;   //led1번 켬
                }
                else if (i == 1 || i == 4 || i == 7)//dim[1]:sex, 0,1만 존재 ,dim[4]:Fbs, 0, 1만 존재 ,dim[7]:Exng, 0,1만 존재 따라서 유효범위 이상으로 못올라가게 제한 
                {
                    if ((*k) >= 1)
                    {
                    }
                    else
                    {
                        for (int q = 0; q < 20; q++) {
                            display(); //200ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                        }
                        (*k)++;
                    }
                }
                else if (i == 5)//dim[5]:Restecg 으로 유효범위는 0~2까지만 따라서 2보다 커지지 못하게 증가제한 
                {
                    if ((*k) >= 2)
                    {
                    }
                    else
                    {
                        for (int q = 0; q < 20; q++) {
                            display(); //200ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                        }
                        (*k)++;
                    }
                }
                else if (i == 8)//dim[8]:Caa으로 유효범위는 0~4까지만 따라서 4보다 커지지 못하게 증가제한 
                {
                    if ((*k) >= 4)
                    {
                    }
                    else
                    {
                        for (int q = 0; q < 20; q++) {
                            display(); //200ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                        }
                        (*k)++;
                    }
                }
                else
                {
                    for (int q = 0; q < 20; q++) {
                        display(); //200ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                    }
                    (*k)++;
                }
            }
        }
        if ((PING & 0x0F) == 0x07)   // 스위치 8번
        {
            if (flag == 1)
            {
                if ((*k) <= 0)
                {
                }
                else
                {
                    for (int q = 0; q < 20; q++) {
                        display(); //200ms delay를 display로 줌 --> 7segment를 연속적인 흐름으로 보여줌 
                    }
                    (*k)--;
                }
                if (k == &svm)
                {
                    if (svm == 0)
                        PORTF = 0x80;
                    else if (svm == 1)
                        PORTF = 0x40;
                    else if (svm == 2)
                        PORTF = 0x20;
                    else if (svm == 3)
                        PORTF = 0x10;
                }
            }
        }
    }
}

ISR(INT4_vect) //int 5번
{
    flag2 = 0;      //타이머를 위한 플래그
    PORTF = 0x00;   //LED 모두 끄기 
    flag = 1;       //스위치를 위한 플래그 
    if (k == &j)    //결과창에서 변수모드로 갈땐, 마지막으로 설정하던 변수모드로 즉 결과창을 보여준 다음에 다시 데이터를 컨트롤 할수 있게 만듦 
    {
        if (i == 9)         //i는 dim[i]로 dimension을 결정 이때 9번이 되면 SVM 모드를 선택할수 있음 따라서 k값에 svm의 주소를 넣어 컨트롤 
        {
            k = &svm;
        }
        else if (i == 10)   //i는 0~9까지만 사용할것이므로 10이되면 0으로 초기화, 동시에 dim[0]의 주소를 넣어줌 즉 age control로 돌아감 
        {
            i = 0;
            k = &dim[i];
        }
        else
        {
            k = &dim[i];     //i값이 0~8까지 각각의 해당 dim의 주소를 넣음 
        }//참고: dim[0]:age, dim[1]:Sex dim[2]:Trtbps dim[3]:Chol dim[4]:Fbs dim[5]:Restecg dim[6]:Thalachh dim[7]:Exng dim[8]:Caa dim[9]:SVM모드설정 의미
    }
    else     //변수모드에서 다시 INT4 누르면, 다음 변수모드로.. 즉 데이터를 입력하는 과정
    {//참고: dim[0]:age, dim[1]:Sex dim[2]:Trtbps dim[3]:Chol dim[4]:Fbs dim[5]:Restecg dim[6]:Thalachh dim[7]:Exng dim[8]:Caa의미 dim[9]:SVM모드설정 의미
        i++; //int4가 눌리면 dimension을 하나씩 변경하면서 입력할 수 있음 
        if (i == 9)     //SVM모드 설정 
        {
            k = &svm;           //k는 SVM을 CTRL
            if ((*k) == 0)      //SVM0 즉 LINEAR모드 
                PORTF = 0x80;   //LED4 점등 
            else if ((*k) == 1) //SVM1 즉 POLY모드 
                PORTF = 0x40;   //LED3 점등
            else if ((*k) == 2) //SVM1 즉 rbf모드 
                PORTF = 0x20;   //LED2 점등
            else if ((*k) == 3) //SVM1 즉 sigmoid모드 
                PORTF = 0x10;   //LED1 점등
        }
        else if (i == 10)
        {
            i = 0;
            k = &dim[i];    //i는 0~9까지만 사용할것이므로 10이되면 0으로 초기화, 동시에 dim[0]의 주소를 넣어줌 즉 age control로 돌아감 
        }
        else  //i: 0~8까지 의미
        {
            k = &dim[i];
        }
    }
}

ISR(INT5_vect) //결과값 k값에 j의 주소를 넣어서 출력, 즉 입력한 데이터의 클래스를 판별 즉 기존 데이터에 기반하여 1이면 heart deasese, 0이면 정상을 판단   
{
    flag = 0;
    flag2 = 1; //결과 출력시 몇번의 svm을 쓰는지 LED로 보여주는데 이때 TIMER INTERRUPT를 통해 0.5초마다 깜빡거리게 만듬
    if (svm == 0)                           //LINEAR 모드일때 출력결과 j
        j = idxToLabel(predictlin(dim));
    else if (svm == 1)                      //POLY모드일때 출력결과 j
        j = idxToLabel(predictpoly(dim));
    else if (svm == 2)                      //RBF 모드일때 출력결과 j
        j = idxToLabel(predictrbf(dim));
    else if (svm == 3)                      //SIGMOID 모드일때 출력결과 j
        j = idxToLabel(predictsig(dim));
    k = &j;                                 //k값에 j의 주소를 넣어줌으로써 k로 j즉 결과값을 control 
    if (j == 1) { //putty를 통해 heart diease를 출력 
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'h';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'e';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'a';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'r';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 't';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = ' ';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'd';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'i';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 's';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'e';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'a';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 's';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'e';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = '\n';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = '\r';

    }
    else if (j == 0) { //putty를 통해 you are fine을 출력 
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'y';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'o';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'u';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = ' ';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'a';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'r';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'e';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = ' ';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'f';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'i';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'n';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = 'e';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = '\n';
        while ((UCSR0A & 0x20) == 0x00);//값받을 준비 되면 루프 탈출 putty에 출력
        UDR0 = '\r';
    }
}