void keyPad()
{
    counter = 0;
    volatile unsigned int butts;
    GPIOB->MODER &= 0xFFFF00FF; // SETS GPIOB 4-7 OUTPUT
    GPIOB->MODER &= 0xFF55FFFF; // SET GPIOB 8-11 AS OUTPUT

    while (1)
    {
        GPIOB->ODR &= 0x00000000; // RESET ODR
        GPIOB->ODR |= 0x0000700;  // SETS ROW 1 TO 0
        butts = GPIOB->IDR;       // HOLDS IDR VALUE TO BE TESTED
        switch (butts)            // SWITCH TO CASCADE THROUGH EACH ROW TO CHECK FOR PUSH
        {
        case 0x770:
            // LCD_WriteData('1');
            while (GPIOB->IDR == 0x771)
                ;
            counterSet(1);
            break;

        case 0x7B0:
            // LCD_WriteData('2');
            while (GPIOB->IDR == 0x7B2)
                ;
            counterSet(2);
            break;

        case 0x7D0:
            // LCD_WriteData('3');
            while (GPIOB->IDR == 0x7D3)
                counterSet(3);
            break;
        }

        GPIOB->ODR &= 0x00000000; // RESET ODR
        GPIOB->ODR |= 0x00000B00; // SETS ROW 2 TO 0
        butts = GPIOB->IDR;
        switch (butts)
        {
        case 0xB70:
            // LCD_WriteData('4');
            while (GPIOB->IDR == 0xB74)
                ;
            counterSet(4);
            break;

        case 0xBB0:
            // LCD_WriteData('5');
            while (GPIOB->IDR == 0xBB5)
                ;
            counterSet(5);
            break;

        case 0xBD0:
            // LCD_WriteData('6');
            while (GPIOB->IDR == 0xBD6)
                ;
            counterSet(6);
            break;
        }

        GPIOB->ODR &= 0x00000000; // RESET ODR
        GPIOB->ODR |= 0x00000D00; // SETS ROW 3 TO 0
        butts = GPIOB->IDR;
        switch (butts)
        {
        case 0xD70:
            // LCD_WriteData('7');
            while (GPIOB->IDR == 0xD77)
                ;
            counterSet(7);
            break;

        case 0xDB0:
            // LCD_WriteData('8');
            while (GPIOB->IDR == 0xDB8)
                ;
            counterSet(8);
            break;

        case 0xDD0:
            // LCD_WriteData('9');
            while (GPIOB->IDR == 0xDD9)
                ;
            counterSet(9);
            break;
        }

        GPIOB->ODR &= 0x00000000; // RESET ODR
        GPIOB->ODR |= 0x00000E00; // SETS ROW 4 TO 0
        butts = GPIOB->IDR;
        switch (butts)
        {
        case 0xEB0:
            // LCD_WriteData('0');
            while (GPIOB->IDR == 0xEB0)
                ;
            counterSet(0);
            break;

        case 0xED0:
            // LCD_WriteData('#');
            rotate(counter);
            break;
        }
    }