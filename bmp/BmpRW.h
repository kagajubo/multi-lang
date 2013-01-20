#ifndef BMPRW_H
#define BMPRW_H

typedef struct {            // ����12�ֽ�
//	short bftype;           // �̶�Ϊ"BM"��VC���ܶ�ȡһ��short����ʡ��
	int   bfSize;			// λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ
	short bfReserved1;		// λͼ�ļ������֣�����Ϊ0
	short bfReserved2;		// λͼ�ļ������֣�����Ϊ0
	int   bfOffBits;		// λͼ���ݵ���ʼλ�ã��������λͼ�ļ�ͷ��ƫ������ʾ�����ֽ�Ϊ��λ
} BITMAPFILEHEADER;

typedef struct {            // ����40�ֽ�
	int   biSize;			// ���ṹ��ռ���ֽ���������40�ֽ�
	int   biWidth;			// λͼ�Ŀ�ȣ�������Ϊ��λ
	int   biHeight;			// λͼ�ĸ߶ȣ�������Ϊ��λ
	short biPlanes;			// Ŀ���豸�ļ��𣬱���Ϊ1
	short biBitCount;		// ÿ�����������λ����������1(˫ɫ),4(16ɫ)��8(256ɫ)��24(���ɫ)֮һ
	int   biCompression;	// λͼѹ�����ͣ�������0(��ѹ��),1(BI_RLE8ѹ������)��2(BI_RLE4ѹ������)֮һ
	int   biSizeImage;		// λͼ�Ĵ�С�����ֽ�Ϊ��λ
	int   biXPelsPerMeter;	// λͼˮƽ�ֱ��ʣ�ÿ��������	
	int   biYPelsPerMeter;	// λͼ��ֱ�ֱ��ʣ�ÿ��������
	int   biClrUsed;		// λͼʵ��ʹ�õ���ɫ���е���ɫ��
	int   biClrImportant;	// λͼ��ʾ��������Ҫ����ɫ��
} BITMAPINFOHEADER;

typedef struct {            // 24λ���ɫ�޴˽ṹ��256���Ҷ�ͼ����256*4�ֽ�
	char  rgbBlue;			// ��ɫ������(ֵ��ΧΪ0-255)
	char  rgbGreen;			// ��ɫ������(ֵ��ΧΪ0-255)
	char  rgbRed;			// ��ɫ������(ֵ��ΧΪ0-255)
	char  rgbReserved;		// ����������Ϊ0
} RGBQUAD;

void ColorBmpR(char*, unsigned char*);			// ��ɫbmpͼ���
void ColorBmpW(char*, char*, unsigned char*);	// ��ɫbmpͼ��д
void GrayBmpR (char*, unsigned char*);			// �Ҷ�bmpͼ���
void GrayBmpW (char*, char*, unsigned char*);	// �Ҷ�bmpͼ��д

#endif 
