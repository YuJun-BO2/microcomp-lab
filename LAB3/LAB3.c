/* ch4-3-5.c -按鈕閞關應用(兩按鈕控制七節顯示器上/下數) */
//==宣告區==========================================
#include <reg51.h> // 包含8051暫存器之標頭檔
#define SEG P1    // 定義七節顯示器接至Port 1

/* 宣告七節顯示器驅動信號陣列(共陽) */
char code TAB[10] = {
    0xc0, 0xf9, 0xa4, 0xb0, 0x99, // 數字0-4
    0x92, 0x83, 0xf8, 0x80, 0x98  // 數字5-9
};

sbit UP = P2^0;     // 宣告UP按鈕接至P2^0
sbit DOWN = P2^1;   // 宣告DOWN按鈕接至P2^1
sbit buzzer = P3^7; // 宣告蜂鳴器接至P3^7

void beep(char x);      // 宣告嗶聲函數
void delay500us(int x); // 宣告0.5ms延遲函數

//==主程式=================================
main() // 主程式開始
{
    char i = 0; // 宣告變數i初值=0
    char status = 1;

    UP = DOWN = 1; // 規劃輸入埠
    P0 = 0xff;
    SEG = TAB[i]; // 輸出數字至七節顯示器
    //SEG=TAB[P0];

    while (1) // 無窮迴圈,程式一直跑
    {
        if (P0 & 0x10) {
            beep(1);
            delay500us(1);

            if (i < (P0 & 0x0f) && status == 1) {
                i++;
                SEG = TAB[i];
                if (i == (P0 & 0x0f)) {
                    status = 0;
                }
            }
            else if (i > 0 && status == 0) {
                i--;
                SEG = TAB[i];
                if (i == 0) {
                    status = 1;
                }
            }
        }
        else {
            //SEG=TAB[P0];
            if (!UP) // 判斷UP鈕是否按下
            {
                i = (i < P0) ? i + 1 : P0;
                // 若i<9則i=i+1，若i>=9清除為0
                SEG = TAB[i]; // 輸出數字至七節顯示器
                beep(1);      // 嗶一聲
                while (!UP);  // 等待UP鈕放開？
            } // if敘述結束
            else if (!DOWN) // 判斷DOWN鈕是否按下
            {
                i = (i > 0) ? i - 1 : P0;
                // 若i>0則i=i-1，i<=0重設為9
                SEG = TAB[i];  // 輸出數字至七節顯示器
                beep(1);       // 嗶一聲
                while (!DOWN); // 等待DOWN鈕放開？
            } // if敘述結束
        }
    } // while迴圈結束
} // 主程式結束

/* 嗶聲函數 */
void beep(char x) // 嗶聲函數開始
{
    char i, j; // 宣告變數i,j
    for (i = 0; i < x; i++) // 產生x聲
    {
        for (j = 0; j < 100; j++) // 重複吸放100次
        {
            buzzer = 0;
            delay500us(1); // 蜂鳴器激磁0.5ms
            buzzer = 1;
            delay500us(1); // 蜂鳴器斷磁0.5ms
        }
        delay500us(200); // 靜音0.1秒
    }
} // 嗶聲函數結束

/* 0.5ms延遲函數 */
void delay500us(int x) // 0.5ms延遲函數開始
{
    int i, j; // 宣告變數i,j
    for (i = 0; i < x; i++) // 產生x*0.5ms
        for (j = 0; j < 60; j++); // 產生0.5ms
} // 0.5ms延遲函數結束