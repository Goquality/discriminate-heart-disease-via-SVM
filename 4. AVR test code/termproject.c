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

ISR(TIMER0_OVF_vect)   //timer overflow�� �Ͼ�� ���� ����
{
    time++;
    if (time == t) //0.5�ʰ� �Ǹ� �Ҳ� 
    {
        if (flag2 == 1)
            PORTF = 0x00;
    }
    if (time == 2 * t) //1�ʰ� �Ǹ� ���� 
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

void init_port(void)      //��Ʈ �ʱ�ȭ
{
    DDRA = 0xFF;         //A��Ʈ�� ��ü �Է�����
    DDRE = 0x0E;         //Enable ��Ʈ�� 3��4���� �Ѽ� ���׸�Ʈ �����ǿ� ���� �Է� �㰡
    PORTE = 0x04;         //E��Ʈ�� 3���� �ΰ��ؼ�, ���׸�Ʈ �����ǿ� ���� �ΰ�
    PORTA = 0x0F;         //00001111�� �־ 4���� ���׸�Ʈ�� ����ϰڴ� ����
}
void init_interrupt(void)
{
    EIMSK = 0x00;   //�켱������ �ܺ� ���ͷ�Ʈ ��ü �� ����.
    EICRA = 0x00;   //���ͷ�Ʈ 0,1,2,3�� ���� LOW�ΰ�
    EICRB = 0x0A;   //���ͷ�Ʈ 4,5���� ���� falling edge ���,
    EIMSK = 0x30;   //���ͷ�Ʈ 4�� 5���� ����ϰڴ� ����
    EIFR = 0xFF;   //��ü���ͷ�Ʈ�� �ϰ������� flag�����,�ʱ�ȭ
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
void display() //7segment ���� 
{
    if (k != &j) //7segment���� k!=j�ּҰ� �ƴϸ�(�� svm �Ǵ� dim[i]�� �ּҶ��) 
    {
        PORTA = FND_SEGPOS[0]; //õ���ڸ�, �����ڸ�, �����ڸ�, �����ڸ��� i���� ����. 
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[i];                   //int 5���� ���� i��, �� ������ �����ϴµ� ����������� ��� 
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);

        PORTA = FND_SEGPOS[1];
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[((*k) / 100) % 10];   //sw�� ���� �ش� ������ ���� ���� �����ϴµ� �� ���� �����ڸ��� ���
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);

        PORTA = FND_SEGPOS[2];
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[((*k) / 10) % 10];    //sw�� ���� �ش� ������ ���� ���� �����ϴµ� �� ���� �����ڸ��� ���
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);

        PORTA = FND_SEGPOS[3];
        PORTE = 0x04;
        PORTE = 0x00;
        PORTA = FND_SEGNP[(*k) % 10];           //sw�� ���� �ش� ������ ���� ���� �����ϴµ� �� ���� �����ڸ��� ���
        PORTE = 0X08;
        PORTE = 0X00;
        _delay_ms(2.5);
    }
    else //k���� j�� �ּ� �� ������� ��쿡�� ù��°���� 3��°���� 7segment�� ����� �ʰ� ������ j�� ��� �� 0 �Ǵ� 1�� ���°ž� �� k���� j�� �ּҸ� �־��� 
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

int idxToLabel(int classIdx) //������ �� avr���带 ���� �Է��� ���� ���� class�� �ű�� ������ return 11�� 0����� 
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
    DDRF = 0xF0;//11110000 F��Ʈ ��� 4������ 
    PORTF = 0x00;//00000000 �ʱ�ȭ
    DDRG = 0x00; //00000000 ����ġ ��뼱�� 
    init_port();
    init_interrupt();
    timer_setting();
    init_uart(); // Port & USART Initialization
    sei();
    while (1)
    {
        display(); //7���׸�Ʈ ��� �� 
        if ((PING & 0x0F) == 0x0E) //����ġ 5�� ������ ���� �������� ������ ������ ������Ŵ �� dim[i]�� ���� ������ ��������
        {
            if (flag == 1) //int 5���� ������ flag�� 1�̵Ǵµ� �� �ǹ̴� dimension�� �����ϸ鼭 ���� �Է��ϰڴٶ�� �ǹ� 
            {
                if (k == &svm)  //k�� svm�� �ּҰ� �Ǵ°��� i�� 9�� �Ǵ°����
                {
                    if ((*k) >= 3) //svm�� 0~3���� linear, poly, rbf, sigmoid 4���̹Ƿ� 3���� ũ�� �������ϰ� ��
                    {
                    }
                    else           //svm�� 0~3���̿����� 
                    {
                        display(); //10ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
                        (*k)++;     //k�� ���� �� k�� svm�� �ּҸ� ������ sw5���� ������ svm�� ������Ŵ 
                    }
                    if (svm == 0)       //svm�� ���� led�� �� 
                        PORTF = 0x80;   //led4�� ��
                    else if (svm == 1)  //svm�� ���� led�� ��
                        PORTF = 0x40;   //led3�� ��
                    else if (svm == 2)  //svm�� ���� led�� ��
                        PORTF = 0x20;   //led2�� ��
                    else if (svm == 3)  //svm�� ���� led�� ��
                        PORTF = 0x10;   //led1�� ��
                }
                else if (i == 1 || i == 4 || i == 7)  //dim[1]:sex, 0,1�� ���� ,dim[4]:Fbs, 0, 1�� ���� ,dim[7]:Exng, 0,1�� ���� ���� ��ȿ���� �̻����� ���ö󰡰� ���� 
                {
                    if ((*k) >= 1) //1���� ũ�� ���ö󰡰� ����
                    {
                    }
                    else
                    {
                        display(); //10ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
                        (*k)++;    //k�� ���� �� k�� dim[1]�Ǵ� dim[4] �Ǵ� dim[7]�� �ּҸ� ������ sw5���� ������ ���� ������Ŵ �̶� 10ms delay�̹Ƿ� ������ ������ ���� 
                    }
                }
                else if (i == 5) //dim[5]:Restecg ���� ��ȿ������ 0~2������ ���� 2���� Ŀ���� ���ϰ� �������� 

                {
                    if ((*k) >= 2)
                    {
                    }
                    else
                    {
                        display(); //10ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
                        (*k)++;
                    }
                }
                else if (i == 8)//dim[8]:Caa���� ��ȿ������ 0~4������ ���� 4���� Ŀ���� ���ϰ� �������� 
                {
                    if ((*k) >= 4)
                    {
                    }
                    else
                    {
                        display(); //10ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
                        (*k)++;
                    }
                }
                else
                {
                    display(); //10ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
                    (*k)++;
                }
            }
        }
        if ((PING & 0x0F) == 0x0D)   //����ġ 6�� ���ʹ� ������ �ݴ�� ���ҽ�Ű�µ� ������ ������ ���ҽ�Ŵ 
        {
            if (flag == 1)
            {
                if ((*k) <= 0)//k���� 0���� ���� ���ϰ� ���� 
                {
                }
                else
                {
                    display(); //10ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
                    (*k)--;
                }
                if (k == &svm)  //k�� svm�� �ּҰ� �Ǵ°��� i�� 9�� �Ǵ°����
                {
                    if (svm == 0)       //svm�� ���� led�� �� 
                        PORTF = 0x80;   //led4�� ��
                    else if (svm == 1)  //svm�� ���� led�� ��
                        PORTF = 0x40;   //led3�� ��
                    else if (svm == 2)  //svm�� ���� led�� ��
                        PORTF = 0x20;   //led2�� ��
                    else if (svm == 3)  //svm�� ���� led�� ��
                        PORTF = 0x10;   //led1�� ��
                }
            }
        }
        if ((PING & 0x0F) == 0x0B)   //����ġ 7��, ���� ������Ű�µ� �̹����� delay�� 200ms�� �ξ� õõ�� �����ϰ� ����
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
                            display(); //200ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
                        }
                        (*k)++;
                    }
                    if (svm == 0)       //svm�� ���� led�� �� 
                        PORTF = 0x80;   //led4�� ��
                    else if (svm == 1)  //svm�� ���� led�� ��
                        PORTF = 0x40;   //led3�� ��
                    else if (svm == 2)  //svm�� ���� led�� ��
                        PORTF = 0x20;   //led2�� ��
                    else if (svm == 3)  //svm�� ���� led�� ��
                        PORTF = 0x10;   //led1�� ��
                }
                else if (i == 1 || i == 4 || i == 7)//dim[1]:sex, 0,1�� ���� ,dim[4]:Fbs, 0, 1�� ���� ,dim[7]:Exng, 0,1�� ���� ���� ��ȿ���� �̻����� ���ö󰡰� ���� 
                {
                    if ((*k) >= 1)
                    {
                    }
                    else
                    {
                        for (int q = 0; q < 20; q++) {
                            display(); //200ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
                        }
                        (*k)++;
                    }
                }
                else if (i == 5)//dim[5]:Restecg ���� ��ȿ������ 0~2������ ���� 2���� Ŀ���� ���ϰ� �������� 
                {
                    if ((*k) >= 2)
                    {
                    }
                    else
                    {
                        for (int q = 0; q < 20; q++) {
                            display(); //200ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
                        }
                        (*k)++;
                    }
                }
                else if (i == 8)//dim[8]:Caa���� ��ȿ������ 0~4������ ���� 4���� Ŀ���� ���ϰ� �������� 
                {
                    if ((*k) >= 4)
                    {
                    }
                    else
                    {
                        for (int q = 0; q < 20; q++) {
                            display(); //200ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
                        }
                        (*k)++;
                    }
                }
                else
                {
                    for (int q = 0; q < 20; q++) {
                        display(); //200ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
                    }
                    (*k)++;
                }
            }
        }
        if ((PING & 0x0F) == 0x07)   // ����ġ 8��
        {
            if (flag == 1)
            {
                if ((*k) <= 0)
                {
                }
                else
                {
                    for (int q = 0; q < 20; q++) {
                        display(); //200ms delay�� display�� �� --> 7segment�� �������� �帧���� ������ 
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

ISR(INT4_vect) //int 5��
{
    flag2 = 0;      //Ÿ�̸Ӹ� ���� �÷���
    PORTF = 0x00;   //LED ��� ���� 
    flag = 1;       //����ġ�� ���� �÷��� 
    if (k == &j)    //���â���� �������� ����, ���������� �����ϴ� �������� �� ���â�� ������ ������ �ٽ� �����͸� ��Ʈ�� �Ҽ� �ְ� ���� 
    {
        if (i == 9)         //i�� dim[i]�� dimension�� ���� �̶� 9���� �Ǹ� SVM ��带 �����Ҽ� ���� ���� k���� svm�� �ּҸ� �־� ��Ʈ�� 
        {
            k = &svm;
        }
        else if (i == 10)   //i�� 0~9������ ����Ұ��̹Ƿ� 10�̵Ǹ� 0���� �ʱ�ȭ, ���ÿ� dim[0]�� �ּҸ� �־��� �� age control�� ���ư� 
        {
            i = 0;
            k = &dim[i];
        }
        else
        {
            k = &dim[i];     //i���� 0~8���� ������ �ش� dim�� �ּҸ� ���� 
        }//����: dim[0]:age, dim[1]:Sex dim[2]:Trtbps dim[3]:Chol dim[4]:Fbs dim[5]:Restecg dim[6]:Thalachh dim[7]:Exng dim[8]:Caa dim[9]:SVM��弳�� �ǹ�
    }
    else     //������忡�� �ٽ� INT4 ������, ���� ��������.. �� �����͸� �Է��ϴ� ����
    {//����: dim[0]:age, dim[1]:Sex dim[2]:Trtbps dim[3]:Chol dim[4]:Fbs dim[5]:Restecg dim[6]:Thalachh dim[7]:Exng dim[8]:Caa�ǹ� dim[9]:SVM��弳�� �ǹ�
        i++; //int4�� ������ dimension�� �ϳ��� �����ϸ鼭 �Է��� �� ���� 
        if (i == 9)     //SVM��� ���� 
        {
            k = &svm;           //k�� SVM�� CTRL
            if ((*k) == 0)      //SVM0 �� LINEAR��� 
                PORTF = 0x80;   //LED4 ���� 
            else if ((*k) == 1) //SVM1 �� POLY��� 
                PORTF = 0x40;   //LED3 ����
            else if ((*k) == 2) //SVM1 �� rbf��� 
                PORTF = 0x20;   //LED2 ����
            else if ((*k) == 3) //SVM1 �� sigmoid��� 
                PORTF = 0x10;   //LED1 ����
        }
        else if (i == 10)
        {
            i = 0;
            k = &dim[i];    //i�� 0~9������ ����Ұ��̹Ƿ� 10�̵Ǹ� 0���� �ʱ�ȭ, ���ÿ� dim[0]�� �ּҸ� �־��� �� age control�� ���ư� 
        }
        else  //i: 0~8���� �ǹ�
        {
            k = &dim[i];
        }
    }
}

ISR(INT5_vect) //����� k���� j�� �ּҸ� �־ ���, �� �Է��� �������� Ŭ������ �Ǻ� �� ���� �����Ϳ� ����Ͽ� 1�̸� heart deasese, 0�̸� ������ �Ǵ�   
{
    flag = 0;
    flag2 = 1; //��� ��½� ����� svm�� ������ LED�� �����ִµ� �̶� TIMER INTERRUPT�� ���� 0.5�ʸ��� �����Ÿ��� ����
    if (svm == 0)                           //LINEAR ����϶� ��°�� j
        j = idxToLabel(predictlin(dim));
    else if (svm == 1)                      //POLY����϶� ��°�� j
        j = idxToLabel(predictpoly(dim));
    else if (svm == 2)                      //RBF ����϶� ��°�� j
        j = idxToLabel(predictrbf(dim));
    else if (svm == 3)                      //SIGMOID ����϶� ��°�� j
        j = idxToLabel(predictsig(dim));
    k = &j;                                 //k���� j�� �ּҸ� �־������ν� k�� j�� ������� control 
    if (j == 1) { //putty�� ���� heart diease�� ��� 
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'h';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'e';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'a';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'r';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 't';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = ' ';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'd';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'i';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 's';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'e';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'a';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 's';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'e';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = '\n';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = '\r';

    }
    else if (j == 0) { //putty�� ���� you are fine�� ��� 
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'y';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'o';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'u';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = ' ';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'a';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'r';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'e';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = ' ';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'f';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'i';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'n';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = 'e';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = '\n';
        while ((UCSR0A & 0x20) == 0x00);//������ �غ� �Ǹ� ���� Ż�� putty�� ���
        UDR0 = '\r';
    }
}