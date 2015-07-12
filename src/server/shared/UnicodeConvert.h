
#ifndef UnicodeConvertH
#define UnicodeConvertH


typedef unsigned short                     UTF16;
typedef unsigned int                       UTF32;
typedef unsigned char                      UTF8;			
const UTF16                                HI_SURROGATE_START = 0xD800;
const UTF16                                LO_SURROGATE_START = 0xDC00;

enum EncodeType{ EncUTF8,EncUTF16BE,EncUTF16LE,EncUTF32BE, EncUTF32LE,EncASCII, EncUNKNOWN};
//enum UTFFormat{ fUTF8, fUTF16LE,fUTF16BE, fUTF32BE,fUTF32LE};


inline int ASCIIToCodePoint(unsigned char*StreamCode,unsigned int*CodePoint, int*LengthStreamCode)
{
  *CodePoint=(unsigned int)StreamCode[0];
  *LengthStreamCode=1;
  return 1;
}

inline int CodePointToASCII(unsigned char*Stream,unsigned int CodePoint, int*LengthStreamCode)
{
   *Stream=(unsigned char)CodePoint;
   *LengthStreamCode=1;
   return 1;
}
inline int CodePointToUTF8(unsigned char*Stream,unsigned int CodePoint, int*LengthStreamCode)
{
   if(CodePoint<0x80) //code sur un octet
   {
	   *Stream=(char)CodePoint;
	   *LengthStreamCode=1;
	   return 1;
   }
   else if(CodePoint<0x1000)// code sur 2 octets
   {
	   *Stream=(unsigned char)( (((CodePoint>>6)&0x1F)|0xC0));
	   Stream++;
	   *Stream=(unsigned char) ((CodePoint&0x3F)|0x80);
	   *LengthStreamCode=2;
	   return 1;
   }
   else if(CodePoint<0x10000)// code sur 3 octets
   {
	   *Stream=(unsigned char)( (((CodePoint>>12)&0xF)|0xE0));
	   Stream++;
	   *Stream=(unsigned char) (((CodePoint>>6)&0x3F)|0x80);
	   Stream++;
	   *Stream=(unsigned char) ((CodePoint&0x3F)|0x80);
	   *LengthStreamCode=3;
	   return 1;
   }
   else if(CodePoint<0x11000)// code sur 4 octets
   {
	   *Stream=(unsigned char)( (((CodePoint>>18)&0x7)|0xF0));
	   Stream++;
	   *Stream=(unsigned char) (((CodePoint>>12)&0x3F)|0x80);
	   Stream++;
	   *Stream=(unsigned char) (((CodePoint>>6)&0x3F)|0x80);
	   Stream++;
	   *Stream=(unsigned char) ((CodePoint&0x3F)|0x80);
	   *LengthStreamCode=4;
	   return 1;
   }
   else return 0; // code inconnu
}


inline int UTF8ToCodePoint(unsigned char*StreamCode,unsigned int*CodePoint, int*LengthStreamCode)
{
 int _tmp1,_tmp2,_tmp3,_tmp4;
  

  if( ( int) StreamCode[0]<0x80)  // code sur 1 octet
  {
	  *CodePoint=(int)StreamCode[0];
	  *LengthStreamCode=1;
	  return 1;
  }
  else if( (( int)*StreamCode>0xBF)&&((int)*StreamCode<0xE0))   //code sur 2 octets
  {
	  if( ((int)StreamCode[1]>0x7F)&&((int)StreamCode[1]<0xC0))
	  {
		 _tmp1=(((int)StreamCode[0])&0x1F)<<6;
		 _tmp2=(((int)StreamCode[1])&0x3F);
		 *CodePoint=_tmp1|_tmp2;
		 *LengthStreamCode=2;
		 return 1;
	  }
	  return 0;
  }
  else if( ((int)*StreamCode>0xDF)&&((int)*StreamCode<0xF0))   //code sur 3 octets
  {
	  if( ((int)StreamCode[1]>0x7F)&&((int)StreamCode[1]<0xC0))
		if( ((int)StreamCode[2]>0x7F)&&((int)StreamCode[2]<0xC0))
		{
		   _tmp1=(((int)StreamCode[0])&0xF)<<12;
		   _tmp2=(((int)StreamCode[1])&0x3F)<<6;
		   _tmp3=(((int)StreamCode[2])&0x3F);
		   *CodePoint=_tmp1|_tmp2|_tmp3;
		   *LengthStreamCode=3;
		   return 1;
		}
	   return 0;
  }
  else if( ((int)*StreamCode>0xEF)&&((int)*StreamCode<0xF8)) //code sur 4 octets
  {
	if( ((int)StreamCode[1]>0x7F)&&((int)StreamCode[1]<0xC0))
	  if( ((int)StreamCode[2]>0x7F)&&((int)StreamCode[2]<0xC0))
		if( ((int)StreamCode[3]>0x7F)&&((int)StreamCode[3]<0xC0))
		{
		   _tmp1=(((int)StreamCode[0])&0x7)<<18;
		   _tmp2=(((int)StreamCode[1])&0x3F)<<12;
		   _tmp3=(((int)StreamCode[2])&0x3F)<<6;
		   _tmp4=(((int)StreamCode[3])&0x3F);
		   *CodePoint=_tmp1|_tmp2|_tmp3|_tmp4;
		   *LengthStreamCode=4;
		   return 1;
		}
	   return 0;
  }

  return 0;

}



inline int CodePointToUTF16_BE(unsigned char*StreamCode,unsigned int CodePoint, int*LengthStreamCode)
{

	if(CodePoint<=0xD7FF||((CodePoint>=0xE000)&&(CodePoint<=0xFFFD))) //2 octets
	{

	  StreamCode[0]=(unsigned char)(CodePoint>>8);
	  StreamCode[1]=(unsigned char)(CodePoint&0xFF);
	  *LengthStreamCode=2;
	  return 1;
	}
	else if( CodePoint>=0x10000 && CodePoint<=0x10FFFF) // 4 octets
	{

		UTF16 X = (UTF16) CodePoint;

		UTF32 U = (CodePoint >> 16) & (0x1F);
		UTF16 W = (UTF16) U - 1;
		UTF16 HiSurrogate = HI_SURROGATE_START | (W << 6) | (X >> 10);
		UTF16 LoSurrogate = (UTF16) (LO_SURROGATE_START | X & 0x3FF);
		UTF16*ptr=(UTF16*)StreamCode;

		StreamCode[0]=(unsigned char)(HiSurrogate>>8)&0xFF;
		StreamCode[1]=(unsigned char)(HiSurrogate)&0xFF;
		StreamCode[2]=(unsigned char)(LoSurrogate>>8)&0xFF;
		StreamCode[3]=(unsigned char)(LoSurrogate&0xFF);
		*LengthStreamCode=4;

		return 1;
	}
	else return 0;
}

inline int CodePointToUTF16_LE(unsigned char*StreamCode,unsigned int CodePoint, int*LengthStreamCode)
{

	if(CodePoint<=0xD7FF||((CodePoint>=0xE000)&&(CodePoint<=0xFFFD))) //sur 2 octets
	{

	  UTF16  X=CodePoint;
	 
	  StreamCode[0]=(unsigned char)(X&0xFF);
	  StreamCode[1]=(unsigned char)(X>>8);

	  *LengthStreamCode=2;
	  return 1;
	}
	else if( CodePoint>=0x10000 && CodePoint<=0x10FFFF) //sur 4octets
	{
		UTF16 X = (UTF16) CodePoint;
		UTF32 U = (CodePoint >> 16) & (0x1F);
		UTF16 W = (UTF16) U - 1;
		UTF16 HiSurrogate = HI_SURROGATE_START | (W << 6) | (X >> 10);
		UTF16 LoSurrogate = (UTF16) (LO_SURROGATE_START | X & 0x3FF);

		 StreamCode[1]=(unsigned char)(HiSurrogate>>8)&0xFF;
		 StreamCode[0]=(unsigned char)(HiSurrogate)&0xFF;
		 StreamCode[3]=(unsigned char)(LoSurrogate>>8)&0xFF;
		 StreamCode[2]=(unsigned char)(LoSurrogate&0xFF);

		*LengthStreamCode=4;
		 return 1;
	}
	else return 0;
}

inline int UTF16_BEToCodePoint(unsigned char*StreamCode,unsigned int*CodePoint, int*LengthStreamCode)
{
	 UTF16 hi=((int)(StreamCode[0]<<8))|((int)StreamCode[1]);
	if(( hi<0xD800)||(hi>0xDF00))//sur 2 octets
	{

	   *CodePoint=hi;
	   *LengthStreamCode=2;
	   return 1;
	}
	else                                   //sur 4 octets
	{
		UTF16 lo=((int)(StreamCode[2]<<8))|((int)StreamCode[3]);
	   if((lo>=0xDC00)||(lo<0xE000))
	   {
			UTF32 X = ((hi & 0x3F) << 10 )| lo & 0x3FF;
			UTF32 W = ((hi >> 6) & 0x1F)+1;

			*CodePoint = W << 16 | X;
			*LengthStreamCode=4;
		  return 1;
	   }
	}
  return 0;
}

inline int UTF16_LEToCodePoint(unsigned char*StreamCode,unsigned int*CodePoint, int*LengthStreamCode)
{


	UTF16  t1=(UTF16)StreamCode[0],t2=(UTF16)StreamCode[1];
	UTF16   hi=t2<<8|t1;

	if(( hi<0xD800)||(hi>0xDF00))
	{
	   *CodePoint=hi;
	   *LengthStreamCode=2;
	   return 1;
	}
	else
	{     t1=(UTF16)StreamCode[2];
		  t2=(UTF16)StreamCode[3];
		  UTF16 lo=t2<<8|t1;

	   if((lo>=0xDC00)||(lo<0xE000))
	   {

			UTF32 X = ((hi & 0x3F) << 10 )| lo & 0x3FF;
			UTF32 W = ((hi >> 6) & 0x1F)+1;
			*CodePoint = W << 16 | X;

			*LengthStreamCode=4;
		  return 1;
       }
	}
  return 0;
}


inline int CodePointToUTF32_BE(unsigned char*StreamCode,unsigned int CodePoint, int*LengthStreamCode)
{
  

   if(CodePoint<0x110000)
   {
	  StreamCode[0]=(unsigned char)(CodePoint>>24);
	  StreamCode[1]=(unsigned char)((CodePoint>>16)&0xFF);
	  StreamCode[2]=(unsigned char)((CodePoint>>8)&0xFF);
	  StreamCode[3]=(unsigned char)(CodePoint&0xFF);
	  *LengthStreamCode=4;
	  return 1;
   }
   else return 0;
}


inline int CodePointToUTF32_LE(unsigned char*StreamCode,unsigned int CodePoint, int*LengthStreamCode)
{

   if(CodePoint<0x110000)
   {
	  unsigned int X=CodePoint;

	  StreamCode[0]=(unsigned char)(X&0xFF);
	  StreamCode[1]=(unsigned char)(X>>8&0xFF);
	  StreamCode[2]=(unsigned char)(X>>16&0xFF);
	  StreamCode[3]=(unsigned char)(X>>24&0xFF);
	  *LengthStreamCode=4;
	  return 1;
   }
   else return 0;
}

inline int UTF32_BEToCodePoint(unsigned char*StreamCode,unsigned int*CodePoint, int*LengthStreamCode)
{
	//int*ptr=(int*)StreamCode;
	int Val=StreamCode[0]<<24|StreamCode[1]<<16|StreamCode[2]<<8|StreamCode[3];
	if(Val<0x110000)
	{
		*CodePoint=Val;
		*LengthStreamCode=4;
		return 1;
	}
	else return 0;
}

inline int UTF32_LEToCodePoint(unsigned char*StreamCode,unsigned int*CodePoint, int*LengthStreamCode)
{

	int Val=((int)StreamCode[0])|((int)StreamCode[1]<<8)|
			((int)StreamCode[2]<<16)|((int)StreamCode[3]<<24);
	if(Val<0x110000)
	{
		*CodePoint=Val;
		*LengthStreamCode=4;
		return 1;
	}
	else return 0;
}


static int ConvertASCIIToUTF8(const char*ToConvert,int len, char*Out)
{
  unsigned int CodePoint;
  int length;
  int Taille=0;
  while(len>0)
  {
	 ASCIIToCodePoint((unsigned char*)ToConvert,&CodePoint,&length);
	 ToConvert+=length;
	 len-=length;
	 CodePointToUTF8((unsigned char*)Out,CodePoint,&length);
	 Out+=length;
	 Taille+=length;
  }
 return Taille;
}
#endif
