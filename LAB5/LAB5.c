/* ch6-3-2.c - 兩個外部中斷實驗 */

//== 宣告區 ==================================
#include <reg51.h> // 定義 8x51 暫存器之標頭檔

#define SW P0
#define SEG P1 // 定義七段顯示器接至 Port 1

void delay1ms(int);       // 宣告延遲函數
void updatepriority();    // 宣告更新中斷優先權函數
int num = 0;              // 宣告整數變數 num，初值 = 0

// 共陽極七段顯示器的字形碼
char code TAB[] = {
	0xc0, 0xf9, 0xa4, 0xb0, 0x99, // 0-4
	0x92, 0x82, 0xf8, 0x80, 0x98, // 5-9
	0xa0, 0x83, 0xa7, 0xa1, 0x84, // a-e (10-14)
	0x8e, 0xbf, 0x7f};

//== 主程式 ==================================
main() // 主程式開始
{
	// 初始化
	EA = EX0 = EX1 = 1; // 啟用 INT0、INT1 中斷
	IT0 = IT1 = 1;      // 設定負緣觸發
	// PX0 = 1;         // 設定 INT0 具有最高優先權
	// PX1 = 1;         // 設定 INT1 具有最高優先權
	SEG = 0xff;        // 初值 = 0000 0000，八燈全滅
	SW = 0xff;

	while (1) // 無窮迴圈，程式一直跑
	{
		updatepriority();     // 更新中斷優先權
		SEG = TAB[num];       // 顯示數字
		delay1ms(500);
		num = (num + 1) % 10; // 數字加 1，超過 10 則歸 0
	} // while 迴圈結束
} // 主程式結束

/* INT0 的中斷副程式 - 單燈左移 3 圈 */
void my_int0(void) interrupt 0 // INT0 中斷副程式開始
{
	// unsigned saveLED = LED; // 儲存中斷前 LED 狀態
	delay1ms(3000);             // 休息3秒
	// LED = saveLED;          // 寫回中斷前 LED 狀態
} // 結束 INT0 中斷副程式

/* INT1 的中斷副程式 - 單燈右移 3 圈 */
void my_int1(void) interrupt 2 // INT1 中斷副程式開始
{
	// unsigned saveLED = LED; // 儲存中斷前 LED 狀態
	delay1ms(500);             // 消除彈跳
	num = 0;                   // 數字歸零
	// LED = saveLED;          // 寫回中斷前 LED 狀態
} // 結束 INT1 中斷副程式

/* 延遲函數，延遲約 x * 1 ms */
void delay1ms(int x) // 延遲函數開始
{
	int i, j; // 宣告整數變數 i、j

	for (i = 0; i < x; i++) // 計數 x 次，延遲 x * 1 ms
		for (j = 0; j < 120; j++)
			; // 計數 120 次，延遲 1 ms
} // 延遲函數結束

void updatepriority()
{
	if ((SW & 0x03) == 0x01)
	{ // SW1 = OFF，SW2 = ON
		PX0 = 0;
		PX1 = 1;
	}
	else if ((SW & 0x01) == 0x00)
	{ // SW1 = ON
		PX0 = 1;
		PX1 = 0;
	}
	if (SW & 0x04)
	{              // SW3 = OFF
		IT0 = 1; // 設定負緣觸發
	}
	else
	{
		IT0 = 0; // 設定低電平觸發
	}
	if (SW & 0x08)
	{              // SW4 = OFF
		IT1 = 1; // 設定負緣觸發
	}
	else
	{
		IT1 = 0; // 設定低電平觸發
	}
}