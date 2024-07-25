#include "ST7789.h"
#include "debug.h"
#include "math.h"

static void SPI3_HARD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    SPI_InitTypeDef SPI_InitStructure={0};

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3, ENABLE );

    GPIO_InitStructure.GPIO_Pin = ST7789_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( ST7789_CS_PORT, &GPIO_InitStructure );
    ST7789_SET_CS();

    GPIO_InitStructure.GPIO_Pin = GT30L32S4W_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GT30L32S4W_CS_PORT, &GPIO_InitStructure );
    GT30L32S4W_SET_CS();

    GPIO_InitStructure.GPIO_Pin = I8080_DC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( I8080_DC_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = SPI_SCLK_PIN | SPI_MOSI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( SPI_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( SPI_PORT, &GPIO_InitStructure );

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init( SPI3, &SPI_InitStructure );

    SPI_Cmd( SPI3, ENABLE );
}

static void ST7789_BL_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE );

    GPIO_InitStructure.GPIO_Pin = ST7789_BL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//设置为推挽（背光）输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ST7789_BL_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(ST7789_BL_PORT,GPIO_Pin_0);
}

static void I8080_Write_Byte(unsigned char _data, unsigned char cmd)
{
    ST7789_RESET_CS();
    I8080_DC(cmd);
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI3, _data);
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY));
    ST7789_SET_CS();
}
static void I8080_Write_Byte16(unsigned int _data)
{	  
    I8080_Write_Byte(_data >> 8, ST7789_DATA);
    I8080_Write_Byte(_data, ST7789_DATA);
} 

static void ST7789_SetWindows(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	if(ST7789_DIR == 2 || ST7789_DIR == 4)
	{
		I8080_Write_Byte(0x2a,ST7789_CMD);//列地址设置
		I8080_Write_Byte16(x+34);
		I8080_Write_Byte16(x + width - 1 + 34);
		
		I8080_Write_Byte(0x2b,ST7789_CMD);//行地址设置
		I8080_Write_Byte16(y);
		I8080_Write_Byte16(y + height - 1);
	}
	else
	{
		I8080_Write_Byte(0x2a,ST7789_CMD);//列地址设置
		I8080_Write_Byte16(x);
		I8080_Write_Byte16(x + width - 1);
		
		I8080_Write_Byte(0x2b,ST7789_CMD);//行地址设置
		I8080_Write_Byte16(y+34);
		I8080_Write_Byte16(y + height - 1 + 34);
	}
    I8080_Write_Byte(0x2c,ST7789_CMD);//储存器写
}

void ST7789_Init()
{
//=============================【初始化】=================================

    SPI3_HARD_Init();

	I8080_Write_Byte(0x11, ST7789_CMD);
	Delay_Ms(30);
	I8080_Write_Byte(0x36, ST7789_CMD);
	
	if(ST7789_DIR == 2)I8080_Write_Byte(0x00, ST7789_DATA);
	else if(ST7789_DIR == 4)I8080_Write_Byte(0xC0, ST7789_DATA);
	else if(ST7789_DIR == 1)I8080_Write_Byte(0x70, ST7789_DATA);
	else if(ST7789_DIR == 3)I8080_Write_Byte(0xA0, ST7789_DATA);

	I8080_Write_Byte(0x3A, ST7789_CMD);
	I8080_Write_Byte(0x05, ST7789_DATA);

	I8080_Write_Byte(0xB2, ST7789_CMD);
	I8080_Write_Byte(0x0C, ST7789_DATA);
	I8080_Write_Byte(0x0C, ST7789_DATA);
	I8080_Write_Byte(0x00, ST7789_DATA);
	I8080_Write_Byte(0x33, ST7789_DATA);
	I8080_Write_Byte(0x33, ST7789_DATA);

	I8080_Write_Byte(0xB7, ST7789_CMD);
	I8080_Write_Byte(0x35, ST7789_DATA);

	I8080_Write_Byte(0xBB, ST7789_CMD);
	I8080_Write_Byte(0x35, ST7789_DATA);

	I8080_Write_Byte(0xC0, ST7789_CMD);
	I8080_Write_Byte(0x2C, ST7789_DATA);

	I8080_Write_Byte(0xC2, ST7789_CMD);
	I8080_Write_Byte(0x01, ST7789_DATA);

	I8080_Write_Byte(0xC3, ST7789_CMD);
	I8080_Write_Byte(0x13, ST7789_DATA);

	I8080_Write_Byte(0xC4, ST7789_CMD);
	I8080_Write_Byte(0x20, ST7789_DATA);

	I8080_Write_Byte(0xC6, ST7789_CMD);
	I8080_Write_Byte(0x0F, ST7789_DATA);

	I8080_Write_Byte(0xD0, ST7789_CMD);
	I8080_Write_Byte(0xA4, ST7789_DATA);
	I8080_Write_Byte(0xA1, ST7789_DATA);

	I8080_Write_Byte(0xD6, ST7789_CMD);
	I8080_Write_Byte(0xA1, ST7789_DATA);

	I8080_Write_Byte(0xE0, ST7789_CMD);
	I8080_Write_Byte(0xF0, ST7789_DATA);
	I8080_Write_Byte(0x00, ST7789_DATA);
	I8080_Write_Byte(0x04, ST7789_DATA);
	I8080_Write_Byte(0x04, ST7789_DATA);
	I8080_Write_Byte(0x04, ST7789_DATA);
	I8080_Write_Byte(0x05, ST7789_DATA);
	I8080_Write_Byte(0x29, ST7789_DATA);
	I8080_Write_Byte(0x33, ST7789_DATA);
	I8080_Write_Byte(0x3E, ST7789_DATA);
	I8080_Write_Byte(0x38, ST7789_DATA);
	I8080_Write_Byte(0x12, ST7789_DATA);
	I8080_Write_Byte(0x12, ST7789_DATA);
	I8080_Write_Byte(0x28, ST7789_DATA);
	I8080_Write_Byte(0x30, ST7789_DATA);

	I8080_Write_Byte(0xE1, ST7789_CMD);
	I8080_Write_Byte(0xF0, ST7789_DATA);
	I8080_Write_Byte(0x07, ST7789_DATA);
	I8080_Write_Byte(0x0A, ST7789_DATA);
	I8080_Write_Byte(0x0D, ST7789_DATA);
	I8080_Write_Byte(0x0B, ST7789_DATA);
	I8080_Write_Byte(0x07, ST7789_DATA);
	I8080_Write_Byte(0x28, ST7789_DATA);
	I8080_Write_Byte(0x33, ST7789_DATA);
	I8080_Write_Byte(0x3E, ST7789_DATA);
	I8080_Write_Byte(0x36, ST7789_DATA);
	I8080_Write_Byte(0x14, ST7789_DATA);
	I8080_Write_Byte(0x14, ST7789_DATA);
	I8080_Write_Byte(0x29, ST7789_DATA);
	I8080_Write_Byte(0x32, ST7789_DATA);

	I8080_Write_Byte(0x21, ST7789_CMD);
	I8080_Write_Byte(0x11, ST7789_CMD);

	Delay_Ms(120);
	I8080_Write_Byte(0x29, ST7789_CMD);

	ST7789_FillColor(0, 0, ST7789_WIDTH, ST7789_HEIGHT, color_WHITE);

	ST7789_BL_Init();
	ST7789_BL_Ctrl(ENABLE);
}

void ST7789_FillColor(unsigned int x, unsigned int y, unsigned int width, unsigned int height, color_t color)
{
//============================【填充屏幕】================================
//参数：
//x:起始x坐标
//y:起始y坐标
//width:窗口宽度
//height:窗口高度
//color:要填充的颜色(RGB565)
//======================================================================
    unsigned int i,j;
    ST7789_SetWindows(x,y,width,height);//设置显示范围
    for(i = y; i < height; i++)
        for(j = x; j < width; j++)I8080_Write_Byte16(color);
}

void ST7789_DrawPic(unsigned int x, unsigned int y, unsigned int width, unsigned int height, const unsigned char *_data)
{
//============================【画图】================================
//参数：
//x:起始x坐标
//y:起始y坐标
//width:窗口宽度
//height:窗口高度
//*_data:包含字模的一维数组地址
//===================================================================
    uint32_t i;
    ST7789_SetWindows(x, y, width, height);
    for(i = 0; i < width * height * 2; i += 2)I8080_Write_Byte16(_data[i] << 8 | _data[i + 1]);
}

void ST7789_DrawDot(unsigned int x, unsigned int y, color_t color, unsigned char bold)
{
//============================【画点】=============================
//参数：
//x:起始x坐标
//y:起始y坐标
//color:颜色(RGB565)
//bold:粗细(像素)
//================================================================
    uint16_t var;
    if(x < ST7789_WIDTH && y <  ST7789_HEIGHT)
    {
        ST7789_SetWindows(x, y, bold, bold);
        for (var = 0; var < bold * bold; ++var)I8080_Write_Byte16(color);
    }
}

void ST7789_DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, color_t color, unsigned char bold)
{
//============================【画线】=============================
//参数：
//x1:起始x坐标
//y1:起始y坐标
//x2:终止x坐标
//y2:终止y坐标
//color:颜色(RGB565)
//bold:粗细(像素)
//================================================================
    uint16_t i, x, y;
    int16_t sError_X=0,sError_Y=0,sDelta_X,sDelta_Y,sDistance;
    int16_t sIncrease_X, sIncrease_Y;

    //计算坐标增量
    sDelta_X = x2 - x1;
    sDelta_Y = y2 - y1;
    //当前坐标位置
    x = x1;
    y = y1;

    if(sDelta_X > 0)sIncrease_X = 1;
    else if(sDelta_X == 0)sIncrease_X = 0;
    else
    {
        sIncrease_X = -1;
        sDelta_X = -sDelta_X;
    }

    if(sDelta_Y > 0)sIncrease_Y = 1;
    else if(sDelta_Y == 0)sIncrease_Y = 0;
    else
    {
        sIncrease_Y = -1;
        sDelta_Y = -sDelta_Y;
    }

    //选取基本增量坐标轴
    if(sDelta_X > sDelta_Y)sDistance = sDelta_X;
    else sDistance = sDelta_Y;

    //画线输出
    for(i = 0; i <= sDistance + 1; i++)
    {
        ST7789_DrawDot(x, y, color, bold);
        sError_X += sDelta_X;
        sError_Y += sDelta_Y;
        if(sError_X > sDistance)
        {
            sError_X -= sDistance;
            x += sIncrease_X;
        }
        if(sError_Y > sDistance)
        {
            sError_Y -= sDistance;
            y += sIncrease_Y;
        }
    }
}

void ST7789_DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, color_t color, unsigned char bold, FunctionalState fill)
{
//============================【画矩形】=============================
//参数：
//x:起始x坐标
//y:起始y坐标
//width:宽度
//height:高度
//color:颜色(RGB565)
//bold:粗细(像素)
//fill:是否填充(ENABLE OR DISABLE)
//==================================================================
    if(fill)ST7789_FillColor(x, y, width, height, color);
    else
    {
        ST7789_DrawLine(x, y, x + width - 1, y, color, bold);
        ST7789_DrawLine(x + width - 1, y, x + width - 1, y + height - 1, color, bold);
        ST7789_DrawLine(x + width - 1, y + height - 1, x, y + height - 1, color, bold);
        ST7789_DrawLine(x, y + height - 1, x, y, color, bold);
    }
}

void ST7789_DrawCircle(unsigned int x, unsigned int y, unsigned int radius, color_t color, unsigned char bold, FunctionalState fill)
{
//============================【画圆】=============================
//参数：
//x:起始x坐标
//y:起始y坐标
//radius:半径(像素)
//color:颜色(RGB565)
//bold:粗细(像素)
//fill:是否填充(ENABLE OR DISABLE)
//================================================================
    int16_t sCurrentX, sCurrentY;
    int16_t sError;

    sCurrentX = 0;
    sCurrentY = radius;
    sError = 3 - (radius << 1);   //判断下个点位置的标志

    while(sCurrentX <= sCurrentY)
    {
        int16_t sCountY = 0;
        if(fill)
        {
            for(sCountY = sCurrentX; sCountY <= sCurrentY; sCountY++)
            {
                ST7789_DrawDot(x + sCurrentX, y + sCountY, color, bold);     //1，研究对象
                ST7789_DrawDot(x - sCurrentX, y + sCountY, color, bold);     //2
                ST7789_DrawDot(x - sCountY,   y + sCurrentX, color, bold);   //3
                ST7789_DrawDot(x - sCountY,   y - sCurrentX, color, bold);   //4
                ST7789_DrawDot(x - sCurrentX, y - sCountY, color, bold);     //5
                ST7789_DrawDot(x + sCurrentX, y - sCountY, color, bold);     //6
                ST7789_DrawDot(x + sCountY,   y - sCurrentX, color, bold);   //7
                ST7789_DrawDot(x + sCountY,   y + sCurrentX, color, bold);   //0
            }
    }
        else
        {
            ST7789_DrawDot(x + sCurrentX, y + sCurrentY, color, bold);     //1，研究对象
            ST7789_DrawDot(x - sCurrentX, y + sCurrentY, color, bold);     //2
            ST7789_DrawDot(x - sCurrentY, y + sCurrentX, color, bold);   //3
            ST7789_DrawDot(x - sCurrentY, y - sCurrentX, color, bold);   //4
            ST7789_DrawDot(x - sCurrentX, y - sCurrentY, color, bold);     //5
            ST7789_DrawDot(x + sCurrentX, y - sCurrentY, color, bold);     //6
            ST7789_DrawDot(x + sCurrentY, y - sCurrentX, color, bold);   //7
            ST7789_DrawDot(x + sCurrentY, y + sCurrentX, color, bold);   //0
    }
        sCurrentX++;
        if(sError < 0)sError += (4*sCurrentX + 6);
        else
        {
            sError += (10 + 4*(sCurrentX - sCurrentY));
            sCurrentY--;
        }
    }
}

static void GT30L32S4W_SendADDR(u32 addr)
{
    u8 addrH, addrM, addrL;

    addrH = addr >> 16;
    addrM = addr >> 8;
    addrL = addr;

    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI3, 0x0B);
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI3, addrH);
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI3, addrM);
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI3, addrL);
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI3, 0xff);
}
static unsigned char GT30L32S4W_Read_Byte(unsigned char _data)
{
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI3, _data);//为了产生8个SCLK
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY));
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET);
    return (uint8_t)SPI_I2S_ReceiveData(SPI3);
}
static void GT30L32S4W_ReadData(GT30L32S4W_BaseADDR_t BaseAdd, unsigned char ASCIICode, unsigned char *buf, unsigned char *chr)
{
    u8 var, MSB, LSB;
    u32 Address = 0;

    MSB = *chr;
    LSB = *(chr + 1);

    if(BaseAdd == SIZE_ASCII_7x8 || BaseAdd == SIZE_ASCII_5x7)
    {
        if(ASCIICode >= 0x20 && ASCIICode <= 0x7E)Address = (ASCIICode - 0x20) * 8 + BaseAdd;
        GT30L32S4W_RESET_CS();
        GT30L32S4W_SendADDR(Address);
        for (var = 0; var < 8; var++)buf[var] = GT30L32S4W_Read_Byte(0xff);
    }
    if(BaseAdd == SIZE_ASCII_6x12)
    {
        if(ASCIICode >= 0x20 && ASCIICode <= 0x7E)Address = (ASCIICode - 0x20) * 12 + BaseAdd;
        GT30L32S4W_RESET_CS();
        GT30L32S4W_SendADDR(Address);
        for (var = 0; var < 12; var++)buf[var] = GT30L32S4W_Read_Byte(0xff);
    }
    if(BaseAdd == SIZE_ASCII_8x16)
    {
        if(ASCIICode >= 0x20 && ASCIICode <= 0x7E)Address = (ASCIICode - 0x20) * 16 + BaseAdd;
        GT30L32S4W_RESET_CS();
        GT30L32S4W_SendADDR(Address);
        for (var = 0; var < 16; var++)buf[var] = GT30L32S4W_Read_Byte(0xff);
    }
    if(BaseAdd == SIZE_ASCII_12x24)
    {
        if(ASCIICode >= 0x20 && ASCIICode <= 0x7E)Address = (ASCIICode - 0x20) * 48 + BaseAdd;
        GT30L32S4W_RESET_CS();
        GT30L32S4W_SendADDR(Address);
        for (var = 0; var < 48; var++)buf[var] = GT30L32S4W_Read_Byte(0xff);
    }
    if(BaseAdd == SIZE_ASCII_16x32)
    {
        if(ASCIICode >= 0x20 && ASCIICode <= 0x7E)Address = (ASCIICode - 0x20) * 64 + BaseAdd;
        GT30L32S4W_RESET_CS();
        GT30L32S4W_SendADDR(Address);
        for (var = 0; var < 64; var++)buf[var] = GT30L32S4W_Read_Byte(0xff);
    }
    if(BaseAdd == SIZE_CHN_12x12)
    {
        if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
        Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*24+ BaseAdd;
        else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
        Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*24+ BaseAdd;

        GT30L32S4W_RESET_CS();
        GT30L32S4W_SendADDR(Address);
        for (var = 0; var < 24; var++)buf[var] = GT30L32S4W_Read_Byte(0xff);
    }
    if(BaseAdd == SIZE_CHN_16x16)
    {
        if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
        Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*32+ BaseAdd;
        else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
        Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;

        GT30L32S4W_RESET_CS();
        GT30L32S4W_SendADDR(Address);
        for (var = 0; var < 32; var++)buf[var] = GT30L32S4W_Read_Byte(0xff);
    }
    if(BaseAdd == SIZE_CHN_24x24)
    {
        if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
        Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*72+ BaseAdd;
        else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
        Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*72+ BaseAdd;

        GT30L32S4W_RESET_CS();
        GT30L32S4W_SendADDR(Address);
        for (var = 0; var < 72; var++)buf[var] = GT30L32S4W_Read_Byte(0xff);
    }
    if(BaseAdd == SIZE_CHN_32x32)
    {
        if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
        Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*128+ BaseAdd;
        else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
        Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*128+ BaseAdd;

        GT30L32S4W_RESET_CS();
        GT30L32S4W_SendADDR(Address);
        for (var = 0; var < 128; var++)buf[var] = GT30L32S4W_Read_Byte(0xff);
    }
    while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_BSY));
    GT30L32S4W_SET_CS();
}

void ST7789_ShowCHN(unsigned int x, unsigned int y, unsigned char *chr, color_t color_background, color_t color_foreground, GT30L32S4W_BaseADDR_t SIZE)
{
//============================【显示中文字符串】=============================
//参数：
//x:起始x坐标
//y:起始y坐标
//*chr:要显示的字符串
//color_background:背景色(RGB565)
//color_foreground:前景色(RGB565)
//SIZE:字体大小
//=========================================================================
    u8 page, col, step;

    if(SIZE == SIZE_CHN_12x12)step = 12;
    if(SIZE == SIZE_CHN_16x16)step = 16;
    if(SIZE == SIZE_CHN_24x24)step = 24;
    if(SIZE == SIZE_CHN_32x32)step = 32;

    while (*chr)
    {
        if(*chr == '\r' && *(chr + 1) == '\n')
        {
            x = 0;
            y += step;
            chr += 2 ;
        }
        if (x + step > ST7789_WIDTH)
        {
            x = 0;
            y += step;
        }
        if (y + step > ST7789_HEIGHT)
        {
            x = 0;
            y = 0;
        }

        if(SIZE == SIZE_CHN_12x12)
        {
            u8 buf[24];
            GT30L32S4W_ReadData(SIZE, 0, buf, chr);
            ST7789_SetWindows(x, y, 12, 12);
            for(page = 0; page < 24; page+= 2)
            {
                for(col = 0; col < 8; col++)
                {
                    if(buf[page] & 0x80)I8080_Write_Byte16(color_foreground);
                    else I8080_Write_Byte16(color_background);
                    buf[page] <<= 1;
                }
                for(col = 0; col < 4; col++)
                {
                    if(buf[page + 1] & 0x80)I8080_Write_Byte16(color_foreground);
                    else I8080_Write_Byte16(color_background);
                    buf[page + 1] <<= 1;
                }
            }
            x += 12;
        }
        if(SIZE == SIZE_CHN_16x16)
        {
            u8 buf[32];
            GT30L32S4W_ReadData(SIZE, 0, buf, chr);
            ST7789_SetWindows(x, y, 16, 16);
            for(page = 0; page < 32; page++)
            {
                for(col = 0; col < 8; col++)
                {
                    if(buf[page] & 0x80)I8080_Write_Byte16(color_foreground);
                    else I8080_Write_Byte16(color_background);
                    buf[page] <<= 1;
                }
            }
            x += 16;
        }
        if(SIZE == SIZE_CHN_24x24)
        {
            u8 buf[72];
            GT30L32S4W_ReadData(SIZE, 0, buf, chr);
            ST7789_SetWindows(x, y, 24, 24);
            for(page = 0; page < 72; page++)
            {
                for(col = 0; col < 8; col++)
                {
                    if(buf[page] & 0x80)I8080_Write_Byte16(color_foreground);
                    else I8080_Write_Byte16(color_background);
                    buf[page] <<= 1;
                }
            }
            x += 24;
        }

        if(SIZE == SIZE_CHN_32x32)
        {
            u8 buf[128];
            GT30L32S4W_ReadData(SIZE, 0, buf, chr);
            ST7789_SetWindows(x, y, 32, 32);
            for(page = 0; page < 128; page++)
            {
                for(col = 0; col < 8; col++)
                {
                    if(buf[page+0] & 0x80)I8080_Write_Byte16(color_foreground);
                    else I8080_Write_Byte16(color_background);
                    buf[page+0] <<= 1;
                }
            }
            x += 32;
        }
        chr += 2;
    }
}

void ST7789_ShowChar(unsigned int x, unsigned int y, unsigned char chr, color_t color_background, color_t color_foreground, GT30L32S4W_BaseADDR_t SIZE)
{
//=========================【显示ASCII字符】==============================
//参数：
//x:起始x坐标
//y:起始y坐标
//chr:要显示的字符
//color_background:背景色(RGB565)
//color_foreground:前景色(RGB565)
//SIZE:字体大小
//======================================================================
    u8 page, col;

    if(SIZE == SIZE_ASCII_5x7)
    {
        u8 buf[8];
        GT30L32S4W_ReadData(SIZE, chr, buf, NULL);
        ST7789_SetWindows(x, y, 5, 7);
        for(page = 0; page < 7; page++)
        {
            for(col = 0; col < 5; col++)
            {
                if(buf[page] & 0x80)I8080_Write_Byte16(color_foreground);
                else I8080_Write_Byte16(color_background);
                buf[page] <<= 1;
            }
        }
    }
    if(SIZE == SIZE_ASCII_7x8)
    {
        u8 buf[8];
        GT30L32S4W_ReadData(SIZE, chr, buf, NULL);
        ST7789_SetWindows(x, y, 7, 8);
        for(page = 0; page < 8; page++)
        {
            for(col = 0; col < 7; col++)
            {
                if(buf[page] & 0x80)I8080_Write_Byte16(color_foreground);
                else I8080_Write_Byte16(color_background);
                buf[page] <<= 1;
            }
        }
    }
    if(SIZE == SIZE_ASCII_6x12)
    {
        u8 buf[12];
        GT30L32S4W_ReadData(SIZE, chr, buf, NULL);
        ST7789_SetWindows(x, y, 6, 12);
        for(page = 0; page < 12; page++)
        {
            for(col = 0; col < 6; col++)
            {
                if(buf[page] & 0x80)I8080_Write_Byte16(color_foreground);
                else I8080_Write_Byte16(color_background);
                buf[page] <<= 1;
            }
        }
    }
    if(SIZE == SIZE_ASCII_8x16)
    {
        u8 buf[16];
        GT30L32S4W_ReadData(SIZE, chr, buf, NULL);
        ST7789_SetWindows(x, y, 8, 16);
        for(page = 0; page < 16; page++)
        {
            for(col = 0; col < 8; col++)
            {
                if(buf[page] & 0x80)I8080_Write_Byte16(color_foreground);
                else I8080_Write_Byte16(color_background);
                buf[page] <<= 1;
            }
        }
    }
    if(SIZE == SIZE_ASCII_12x24)
    {
        u8 buf[48];
        GT30L32S4W_ReadData(SIZE, chr, buf, NULL);
        ST7789_SetWindows(x, y, 12, 24);
        for(page = 0; page < 48; page += 2)
        {
            for(col = 0; col < 8; col++)
            {
                if(buf[page] & 0x80)I8080_Write_Byte16(color_foreground);
                else I8080_Write_Byte16(color_background);
                buf[page] <<= 1;
            }
            for(col = 0; col < 4; col++)
            {
                if(buf[page + 1] & 0x80)I8080_Write_Byte16(color_foreground);
                else I8080_Write_Byte16(color_background);
                buf[page + 1] <<= 1;
            }
        }
    }
    if(SIZE == SIZE_ASCII_16x32)
    {
        u8 buf[64];
        GT30L32S4W_ReadData(SIZE, chr, buf, NULL);
        ST7789_SetWindows(x, y, 16, 32);
        for(page = 0; page < 64; page++)
        {
            for(col = 0; col < 8; col++)
            {
                if(buf[page] & 0x80)I8080_Write_Byte16(color_foreground);
                else I8080_Write_Byte16(color_background);
                buf[page] <<= 1;
            }
        }
    }
}

static unsigned long Math_Pow(unsigned char y)//计算10的y次方（10^y）函数
{
    unsigned long result = 1;
    while(y--)result *= 10;
    return result;
}

void ST7789_ShowNum(unsigned int x, unsigned int y, signed long num, unsigned char len, color_t color_background, color_t color_foreground, GT30L32S4W_BaseADDR_t SIZE)
{
//==============================【显示数字】==============================
//参数：
//x:起始x坐标
//y:起始y坐标
//num:要显示的数字，支持负数
//len:数字位数
//color_background:背景色(RGB565)
//color_foreground:前景色(RGB565)
//SIZE:字体大小
//注意溢出问题
//======================================================================
    unsigned char i, temp, step = 0, zero_flag = 0;
    if(SIZE == SIZE_ASCII_12x24)step = 12;
    else if(SIZE == SIZE_ASCII_16x32) step = 16;
    if(num < 0)
    {
        num = -num;
        ST7789_ShowChar(x, y, '-', color_background, color_foreground, SIZE);
        x += step;//指针移动
    }
    else
    {
        ST7789_ShowChar(x, y, ' ', color_background, color_foreground, SIZE);
        x += step;//指针移动
    }
    for(i = 0;i < len;i++)//循环显示每一个字符
    {
        temp = (num / Math_Pow(len - i - 1)) % 10;//求出当前位的数值，假如数字为25，len为3，当i为0时temp = 0，当i为1时temp = 2，，当i为2时temp = 5
        if(zero_flag == 0 && i < (len - 1))//未进入过该判断语句且i不是最后一位时
        {
            if(temp == 0)
            {
                ST7789_ShowChar(x + step * i, y, ' ', color_background, color_foreground, SIZE);
                continue;//跳出本次循环
            }
            else zero_flag = 1;//从此不进入该判断语句
        }
        ST7789_ShowChar(x + step * i, y, temp + '0', color_background, color_foreground, SIZE);
    }
}

void ST7789_ShowNumf(unsigned int x, unsigned int y, float num, unsigned char len, color_t color_background, color_t color_foreground, GT30L32S4W_BaseADDR_t SIZE)
{
//==============================【显示小数】==============================
//参数：
//x:起始x坐标
//y:起始y坐标
//num:要显示的数字，两位小数，支持负数
//len:数字位数，包括小数位
//color_background:背景色(RGB565)
//color_foreground:前景色(RGB565)
//SIZE:字体大小
//注意溢出问题
//======================================================================
    unsigned char i, temp, step = 0, zero_flag = 0;
    unsigned long num1;
    if(SIZE == SIZE_ASCII_12x24)step = 12;
    else if(SIZE == SIZE_ASCII_16x32) step = 16;
    if(num < 0)
    {
        num = -num;
        ST7789_ShowChar(x, y, '-', color_background, color_foreground, SIZE);
        x += step;//指针移动
    }
    else
    {
        ST7789_ShowChar(x, y, ' ', color_background, color_foreground, SIZE);
        x += step;//指针移动
    }
    num1 = num * 100;
    for(i = 0;i < len;i++)
    {
        temp = (num1 / Math_Pow(len - i - 1)) % 10;
        if(zero_flag == 0 && i < (len - 3))
        {
            if(temp == 0)
            {
                ST7789_ShowChar(x + step * i, y, ' ', color_background, color_foreground, SIZE);
                continue;//跳出本次循环
            }
            else zero_flag = 1;//从此不进入该判断语句
        }
        if(i == (len - 2))
        {
            ST7789_ShowChar(x + step * i, y, '.', color_background, color_foreground, SIZE);
            i++;
            len += 1;
        }
        ST7789_ShowChar(x + step * i, y, temp + '0', color_background, color_foreground, SIZE);
    }
}

void ST7789_ShowString(unsigned int x, unsigned int y, const char *chr, color_t color_background, color_t color_foreground, GT30L32S4W_BaseADDR_t SIZE)
{
//=========================【显示ASCII字符串】============================
//参数：
//x:起始x坐标
//y:起始y坐标
//*chr:要显示的字符串
//color_background:背景色(RGB565)
//color_foreground:前景色(RGB565)
//SIZE:字体大小
//======================================================================
    u8 step;
    if(SIZE == SIZE_ASCII_5x7)step = 5;
    if(SIZE == SIZE_ASCII_7x8)step = 7;
    if(SIZE == SIZE_ASCII_6x12)step = 6;
    if(SIZE == SIZE_ASCII_8x16)step = 8;
    if(SIZE == SIZE_ASCII_12x24)step = 12;
    if(SIZE == SIZE_ASCII_16x32)step = 16;
    while (*chr)
    {
        if(*chr == '\r' && *(chr + 1) == '\n')
        {
            x = 0;
            if(SIZE == SIZE_ASCII_12x24)y += 24;
            if(SIZE == SIZE_ASCII_16x32)y += 32;
            chr += 2 ;
        }
        if (x + step > ST7789_WIDTH)
        {
            x = 0;
            y += step * 2;
        }
        if (y + step * 2 > ST7789_HEIGHT)
        {
            x = 0;
            y = 0;
        }
        ST7789_ShowChar(x, y, *chr, color_background, color_foreground, SIZE);
        chr++;
        x += step;
    }
}
