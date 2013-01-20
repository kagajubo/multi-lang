#ifndef BMPRW_H
#define BMPRW_H

typedef struct {            // 长度12字节
//	short bftype;           // 固定为"BM"，VC不能读取一个short，故省略
	int   bfSize;			// 位图文件的大小，以字节为单位
	short bfReserved1;		// 位图文件保留字，必须为0
	short bfReserved2;		// 位图文件保留字，必须为0
	int   bfOffBits;		// 位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位
} BITMAPFILEHEADER;

typedef struct {            // 长度40字节
	int   biSize;			// 本结构所占用字节数，长度40字节
	int   biWidth;			// 位图的宽度，以像素为单位
	int   biHeight;			// 位图的高度，以像素为单位
	short biPlanes;			// 目标设备的级别，必须为1
	short biBitCount;		// 每个像素所需的位数，必须是1(双色),4(16色)，8(256色)或24(真彩色)之一
	int   biCompression;	// 位图压缩类型，必须是0(不压缩),1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
	int   biSizeImage;		// 位图的大小，以字节为单位
	int   biXPelsPerMeter;	// 位图水平分辨率，每米像素数	
	int   biYPelsPerMeter;	// 位图垂直分辨率，每米像素数
	int   biClrUsed;		// 位图实际使用的颜色表中的颜色数
	int   biClrImportant;	// 位图显示过程中重要的颜色数
} BITMAPINFOHEADER;

typedef struct {            // 24位真彩色无此结构，256级灰度图长度256*4字节
	char  rgbBlue;			// 蓝色的亮度(值范围为0-255)
	char  rgbGreen;			// 绿色的亮度(值范围为0-255)
	char  rgbRed;			// 红色的亮度(值范围为0-255)
	char  rgbReserved;		// 保留，必须为0
} RGBQUAD;

void ColorBmpR(char*, unsigned char*);			// 彩色bmp图像读
void ColorBmpW(char*, char*, unsigned char*);	// 彩色bmp图像写
void GrayBmpR (char*, unsigned char*);			// 灰度bmp图像读
void GrayBmpW (char*, char*, unsigned char*);	// 灰度bmp图像写

#endif 
