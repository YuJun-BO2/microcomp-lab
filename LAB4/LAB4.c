#include <reg51.h>
// 七段顯示器與矩陣鍵盤的位選、資料埠
#define SEG_SCAN P0
#define KEY_SCAN P3
#define SEG P1
#define KEY P2

sbit buzzer = P3 ^ 7;

// 共陽極七段顯示器的字形碼
char code TAB[] = {
    0xc0, 0xf9, 0xa4, 0xb0, 0x99, // 0-4
    0x92, 0x82, 0xf8, 0x80, 0x98, // 5-9
    0xa0, 0x83, 0xa7, 0xa1, 0x84, // a-e(10-14)
    0x8e, 0xbf, 0x7f};            // F(15),(-),(.)

// 依序選取四個掃描位置，低電位有效
char scan[4] = {
    0xFE, // 1111 1110
    0xFD, // 1111 1101
    0xFB, // 1111 1011
    0xF7  // 1111 0111
};

void beep(char x);
void delay1ms(int x);
void keyScan();
void seg_display();

// 保存輸入數字、輸入長度、鎖定狀態及掃描位置
char input[4] = {0, 0, 0, 0};
char inputing_now = 0;
char is_locked = 0;
char col = 0;
char seg_col = 0;
unsigned int password;

main()
{
    while (1)
    {
        // 持續交替掃描鍵盤與更新顯示
        keyScan();
        seg_display();
    }
}

void keyScan()
{
    char anyKey = 0;
    char row;
    char rowkey;
    char kcode;

    // 選取一欄，讀取低四位的按鍵列訊號
    KEY_SCAN = scan[col];

    rowkey = ~KEY & 0x0F;

    if (rowkey != 0)
    {
        // 延遲消除按鍵彈跳，再判斷按下的列
        delay1ms(20);
        rowkey = ~KEY & 0x0F;
        if (rowkey == 0x01)
            row = 0;
        else if (rowkey == 0x02)
            row = 1;
        else if (rowkey == 0x04)
            row = 2;
        else if (rowkey == 0x08)
            row = 3;
        else
        {
            col = (col + 1) % 4;
            return;
        }

        // 由列與欄換算鍵值，等待按鍵放開
        kcode = 4 * row + col;

        while (rowkey != 0)
        {
            rowkey = ~KEY & 0x0F;
            delay1ms(20);
        }

        if (kcode <= 9) // 數字鍵 0~9
        {
            if (inputing_now < 4)
            {
                input[inputing_now] = kcode;
                inputing_now++;
            }
            else // 已滿4位：往左推，丟掉最早輸入的
            {
                input[0] = input[1];
                input[1] = input[2];
                input[2] = input[3];
                input[3] = kcode;
            }

            // 已上鎖時，輸入滿4位直接比對解鎖
            if (is_locked == 1 && inputing_now >= 4)
            {
                if (password == input[0] * 1000U + input[1] * 100U + input[2] * 10U + input[3])
                {
                    beep(2);
                    is_locked = 0;
                }
                else
                {
                    beep(1);
                }

                input[0] = 0;
                input[1] = 0;
                input[2] = 0;
                input[3] = 0;
                inputing_now = 0;
            }
        }
        else if (kcode == 11) // B：退一位
        {
            if (inputing_now > 0)
            {
                inputing_now--;
                input[inputing_now] = 0;
            }
        }
        else if (kcode == 10 && is_locked == 0) // A：上鎖 (已上鎖時按A無作用)
        {
            if (inputing_now < 4) // 不足4位，不上鎖
            {
                beep(1);
            }
            else
            {
                is_locked = 1;
                password = input[0] * 1000U + input[1] * 100U + input[2] * 10U + input[3];
                input[0] = 0;
                input[1] = 0;
                input[2] = 0;
                input[3] = 0;
                inputing_now = 0;
            }
        }
        // 其他鍵 (C~F) 不處理
    }

    col = (col + 1) % 4;
}

void seg_display(void)
{
    char index;

    // 切換位選前先關閉顯示，避免殘影
    SEG_SCAN = 0xFF;

    // 由右向左顯示已輸入的數字，空位顯示 0
    if (seg_col < inputing_now)
    {
        index = inputing_now - 1 - seg_col;
        SEG = TAB[input[index]];
    }
    else
    {
        SEG = TAB[0];
    }

    SEG_SCAN = scan[seg_col];

    delay1ms(1);

    SEG_SCAN = 0xFF;
    seg_col = (seg_col + 1) % 4;
}

void beep(char x)
{
    char i, j;

    // 每次交替切換蜂鳴器電位，發出 x 次提示音
    for (i = 0; i < x; i++)
    {
        for (j = 0; j < 100; j++)
        {
            buzzer = 0;
            delay1ms(1);
            buzzer = 1;
            delay1ms(1);
        }
        delay1ms(200);
    }
}

void delay1ms(int x)
{
    int i, j;

    // 以空迴圈產生約 x 毫秒的延遲
    for (i = 0; i < x; i++)
        for (j = 0; j < 60; j++)
            ;
}
