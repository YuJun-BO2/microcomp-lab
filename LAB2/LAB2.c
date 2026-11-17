/* ch3-3-1.c - 蜂鳴器實驗程式           */
//==宣告區====================================
#include <reg51.h>
//包含8051暫存器之標頭檔
sbit buzzer = P3^7;      // 宣告蜂鳴器的位置為Port 3之bit 7
void delay500us(int x); // 宣告解析度為0.5ms之延遲函數
void halftone(int x);    // 執行半波形函數
void pulse_BZ(int count, int TH, int TL); // 宣告蜂鳴器發聲函數

//==主程式====================================

main() // 主程式開始
{
    while(1) // 無窮迴圈,程式一直跑
    {
        pulse_BZ(100,1,1);
        // 蜂鳴器發聲100 (0.5m+0.5m)=0.1s
        delay500us(1000); // 延遲1000 0.5m=0.5秒
    } // while迴圈結束
} // 主程式結束

/* 延遲函數開始,延遲x 0.5ms */
void delay500us(int x) // 延遲函數開始
{
    int i,j; // 宣告整數變數i,j
    for (i=0;i<x;i++) // 計數x次,延遲約x 0.5ms
        for (j=0;j<60;j++); // 計數60次，延遲約0.5ms
} // 延遲函數結束

void delay8us(int x) // 延遲函數開始
{
    int i,j; // 宣告整數變數i,j
    for (i=0;i<x;i++);
        for (j=0;j<1;j++); // 計數x次,延遲約x 0.5ms
} // 延遲函數結束


/* 蜂鳴器發聲函數,count=計數次數,TH=高態時間,TL=低態時間 */
void pulse_BZ(int count,int TH,int TL) // 蜂鳴器發聲函數開始
{
    unsigned char i,j;
    unsigned char code song[]={7,5,3,11,11,11,2,2,11,6,4,4,,11,3,4,4,11,5,5};
    int code tone[11] = {
        127, // 0:Si       494 Hz
        120, // 1:Do       523 Hz
        106, // 2:Re       587 Hz
         95, // 3:Mi       659 Hz
         90, // 4:Fa       698 Hz
         80, // 5:So       784 Hz
         71, // 6:La       880 Hz
         63, // 7:Si       988 Hz
         60, // 8:Do      1046 Hz
         53, // 9:Re      1175 Hz
         47  // 10:Mi     1318 Hz
    };
    for (i=0;i<19;i++) {//執行完整一首歌
        for (j=0;j<50;j++)//每個音符重複50次
        {
            if(song[i] != 11){//11是休止符，若非11，即正常音符時，正常發出該音符的頻率
                buzzer=0;
                halftone(tone[song[i]]);
                buzzer=1;
                halftone(tone[song[i]]);
            }else{//若為11，即休止符時，發出的波不會有頻率變化亦無聲
                buzzer=1;
                halftone(tone[5]);
            }
            delay500us(10);//每個音符之間間隔5ms
        }
    }
}
