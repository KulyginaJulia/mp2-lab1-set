// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0){ int exception = 1; throw exception;}
	else
	{
	BitLen = len; 
		MemLen = (len + (8*sizeof(TELEM) - 1)) / (sizeof(TELEM)*8);
		//MemLen = len / sizeof(TELEM) + 1 * (len % sizeof(TELEM) > 0 ? 1 : 0);
		  pMem = new TELEM[MemLen];
		  for (int i = 0; i < MemLen; i++) 
					pMem[i] = 0;
		 }
}
	

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for ( int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];

}

TBitField::~TBitField()
{
	delete [] pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const TELEM n) const // индекс Мем для бита n
{
	int p = n / (sizeof(TELEM)*8);
	return p;
}

TELEM TBitField::GetMemMask(const TELEM n) const // битовая маска для бита n
{
	return 1 << (n % (sizeof(TELEM)*8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
   return BitLen;
}

void TBitField::SetBit(const TELEM n) // установить бит
{
	if ((n < 0)||(n > BitLen))
	{
		int exception = 1; throw exception;
	}	
	else 
			pMem[GetMemIndex(n)] |= GetMemMask(n);
	
}

void TBitField::ClrBit(const TELEM n) // очистить бит
{
	if ((n < 0)||(n > BitLen)){ int exception = 1; throw exception;}
		else {
		pMem[GetMemIndex(n)] &=~GetMemMask(n);
		}
}

int TBitField::GetBit(const TELEM n) const // получить значение бита
{
	if ((n < 0) || (n > BitLen))
	{ 
		int exception = 1;
		throw exception;
	}
	else
	{
	TELEM temp = GetMemMask(n);
	if((pMem[GetMemIndex(n)]&temp)==0)
		return 0;
	else
		return 1;
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen ){
			MemLen = bf.MemLen;
			if (pMem != NULL) delete []pMem;
			pMem = new TELEM[MemLen];
			}
	if (pMem != NULL)
		for(int i = 0; i<MemLen; i++) 
			pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res = 1;
	if ( BitLen != bf.BitLen ) res = 0;
	else 
		for ( int i = 0; i< BitLen; i++)
			if (this->GetBit(i) != bf.GetBit(i)){res = 0; break; }
  return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{ 
	int res = 0;
	if ( BitLen != bf.BitLen ) {res = 1;}
	else 
		//for ( int i = 0; i< BitLen; i++)
		for ( int i = 0; i< MemLen; i++)
			if (this->GetBit(i) == bf.GetBit(i)){res = 1; break; }
  return res;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int i, len = BitLen;
	if ( bf.BitLen > len) len = bf.BitLen;
	TBitField temp(len);
	for ( i = 0; i<MemLen; i++) 
		temp.pMem[i] = pMem[i];
	for ( i = 0; i<bf.MemLen; i++) 
		temp.pMem[i] |= bf.pMem[i]; 
	return temp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int i, len = BitLen;
	if ( bf.BitLen > len) len = bf.BitLen;
	TBitField temp(len);
	for ( i = 0; i<MemLen; i++)
		temp.pMem[i] = pMem[i];
	for ( i = 0; i<bf.MemLen; i++) 
		temp.pMem[i] &= bf.pMem[i]; 
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(BitLen);
	for ( int i = 0; i<BitLen; i++)
		if (this->GetBit(i) == 0) temp.SetBit(i);
		
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i = 0; char ch;
	do { istr >> ch;} while ( ch != ' ');
	
	while (1) {istr >> ch;
		if ( ch == '0') bf.ClrBit(i++);
		else if (ch == '1' )bf.SetBit(i++); 
		else break;
	}
	return istr;

}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	int len = bf.GetLength();
	for (int i = 0; i<len; i++)
		if ( bf.GetBit(i)) ostr << '1';
		else ostr << '1';
	return ostr;
}
