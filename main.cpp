#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <fstream>
#include <bitset>
#include "des.h"
#include <math.h>
#include <cmath>
#define KEYTEST 1
#define TESTN 1000000
#define SBOXA /*6*/ 1
#define LSBOX /*1*/ 5
using namespace std;

int IP[64] = {	58,    50,   42,    34,    26,   18,    10,    2,
				60,    52,   44,    36,    28,   20,    12,    4,
				62,    54,   46,    38,    30,   22,    14,    6,
				64,    56,   48,    40,    32,   24,    16,    8,
				57,    49,   41,    33,    25,   17,     9,    1,
				59,    51,   43,    35,    27,   19,    11,    3,
				61,    53,   45,    37,    29,   21,    13,    5,
				63,    55,   47,    39,    31,   23,    15,    7};

int IPR[64] = {	40,     8,   48,    16,    56,   24,    64,   32,
				39,     7,   47,    15,    55,   23,    63,   31,
				38,     6,   46,    14,    54,   22,    62,   30,
				37,     5,   45,    13,    53,   21,    61,   29,
				36,     4,   44,    12,    52,   20,    60,   28,
				35,     3,   43,    11,    51,   19,    59,   27,
				34,     2,   42,    10,    50,   18,    58,   26,
				33,     1,   41,     9,    49,   17,    57,   25};

static int Ebox[48] =	{32, 1, 2, 3, 4, 5,
						4, 5, 6, 7, 8, 9,
						8, 9, 10, 11, 12, 13,
						12,	13,	14,	15,	16,	17,
						16,	17,	18,	19,	20,	21,
						20,	21,	22,	23,	24,	25,
						24,	25,	26,	27,	28,	29,
						28,	29,	30,	31,	32,	1};

static int per[32] =	{16, 7 ,20  ,21,
						29 ,12 , 28 , 17,
						1 , 15,  23,  26,
						5, 18, 31, 10,
						2, 8, 24, 14,
						32, 27, 3, 9,
						19, 13, 30, 6,
						22, 11, 4, 25};

static 	int s[8][4][16]=
{
	{
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
	},
	{
	    {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
	    {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
	    {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
	},


	{
	    {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
	    {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
	    {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
	},
	{
	    {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
	    {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
	    {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
	},
	{
	    {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
	    {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
	    {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
	},
	{
	    {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
	    {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
	    {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
	},
	{
	    {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
	    {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
	    {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
	},
	{
	    {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
	    {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
	    {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
	}
};

uint64_t ipow(uint64_t base, uint64_t exp)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
		base *= base;
    }

    return result;
}

static int ddt[8][64][16];

// Initialize the ddts:
static void ddtInit()
{
	for(int sNum = 0; sNum < 8; sNum++)
	{
		//Init all the entires to zero
		for(int in = 0; in < 64; in++) 
		{
		  for(int out = 0; out < 16; out++)
		  {
		    ddt[sNum][in][out] = 0;
		  }
		}
		
		// this makes us go through all the possible value of p1
		for(int p1 = 0; p1 < 64; p1++) 
		{
			//Compute the value of p1 after the sbox
			int p1Row = (p1 % 2) + 2*(p1/32);
			int p1Col = 0;
			int p1Temp = p1/2;
			p1Col += p1Temp % 2;
			int power = 2;
			for (int i=0; i<3; i++)
			{
				p1Temp /= 2;
				int bit = p1Temp % 2;
				p1Col += power*bit;
				power *= 2;
			}
			int sresp1 = s[sNum][p1Row][p1Col];

		  // this makes us go through all the possible value of p2
		  for(int p2 = 0; p2 < 64; p2++)
		  {
		    int XOR_IN = p1 ^ p2;
						
			int p2Row = (p2 % 2) + 2*(p2/32);
			int p2Col = 0;
			int p2Temp = p2/2;
			p2Col += p2Temp % 2;
			power = 2;
			for (int i=0; i<3; i++)
			{
				p2Temp /= 2;
				int bit = p2Temp % 2;
				p2Col += power*bit;
				power *= 2;
			}
			int sresp2 = s[sNum][p2Row][p2Col];

		    int XOR_OUT = sresp1 ^ sresp2;

		    ddt[sNum][XOR_IN][XOR_OUT]++;
		  }
		}
	}
}

bool parity(uint64_t x)
{
	uint64_t count = 0, b = 1;
	
	for(int i = 0; i < 64; i++)
	{
	    if( x & (b << i) )
			count++;
	}
	
	if (count % 2)
		return true;
	
	return false;
}

static int lat[8][64][16] = {};

void latInit()
{
	for (int sbox = 0; sbox < 8; sbox++)
	{
		for(int x_i = 0; x_i < 64; x_i++)
		{
		    for(int y_i = 0; y_i < 16; y_i++)
			{
		        for(int x_j = 0; x_j < 64; x_j++)
				{
					int x_j_row = (x_j % 2) + 2*(x_j / 32);
					int x_j_col = 0;
					int x_jT = x_j/2;
					for (int t = 0; t < 4; t++)
					{
						x_j_col += ipow(2, t)*(x_jT % 2);
						x_jT /= 2;
					}
		            int y_j = s[sbox][x_j_row][x_j_col];
		            y_j &= y_i;
		            int temp = x_j & x_i;
		            int parity_11 = (parity(temp) + parity(y_j)) % 2;
		            lat[sbox][x_i][y_i] += parity_11;
				}
			}
		}
	}
	for (int sb = 0; sb < 8; sb++)
		for (int b4l = 0; b4l < 64; b4l++)
			for (int afl = 0; afl < 16; afl++)
				lat[sb][b4l][afl] = 32 - lat[sb][b4l][afl];
}

static int dlct[8][64][16] = {};

void dlctInit()
{
	for (int sbox = 0; sbox < 8; sbox++)
	{
		for(int x_i = 0; x_i < 64; x_i++)
		{
			int x_iRow = (x_i % 2) + 2*(x_i / 32);
			int x_iCol = 0;
			int x_iT = x_i/2;
			for (int t = 0; t < 4; t++)
			{
				x_iCol += ipow(2, t)*(x_iT % 2);
				x_iT /= 2;
			}
			int sx_i = s[sbox][x_iRow][x_iCol];

		    for(int x_j = 0; x_j < 64; x_j++)
			{
				int xorin = x_i ^ x_j;

				int x_j_row = (x_j % 2) + 2*(x_j / 32);
				int x_j_col = 0;
				int x_jT = x_j/2;
				for (int t = 0; t < 4; t++)
				{
					x_j_col += ipow(2, t)*(x_jT % 2);
					x_jT /= 2;
				}
		        int sx_j = s[sbox][x_j_row][x_j_col];

				for (int mask = 0; mask < 16; mask++)
				{
					if (!(parity(sx_i & mask) ^ parity(sx_j & mask)))
						dlct[sbox][xorin][mask]++;
				}
			}
		}
	}
}


//string hex2bin(string p)//hexadecimal to binary
//{
//	string ap="";
//	int l=p.length();
//	for(int i=0;i<l;i++)
//	{
//		string st="";
//		if(p[i]>='0'&&p[i]<='9')
//		{
//			int te=int(p[i])-48;
//			while(te>0)
//			{
//				st+=char(te%2+48);
//				te/=2;
//			}
//			while(st.length()!=4)
//				st+='0';
//			for(int j=3;j>=0;j--)
//				ap+=st[j];
//		}
//		else
//		{
//			int te=p[i]-'A'+10;
//			while(te>0)
//			{
//				st+=char(te%2+48);
//				te/=2;
//			}
//			for(int j=3;j>=0;j--)
//				ap+=st[j];
//		}
//	}
//	return ap;
//}

static uint64_t strToInt(std::string &str, int numBits)
{
	uint64_t result = 0;
	for(int i = 0; i < numBits; i++)
	{
		result = result<<1;
		unsigned int digit = str[i]-'0';
		//if (str[i]<58) digit = str[i]-'0';
		//else
		//	digit = 10+str[i]-'A';
		result ^= digit;
	}
	return result;
}

void int2BinS (uint64_t n, int numBits, std::string &s)
{
	//std::string s;
	s.resize(numBits);
	for (int i = 0; i < numBits; i++)
	{
		uint64_t temp = n%2;
		char a = (char)(48+temp);
		//s.insert(s.begin(),a);
		s[numBits-i-1] = a;
		n /= 2;
	}
	//return s;
}

void prtInt64AsHex (uint64_t n, char* ar)
{
	uint64_t temp = n;
	for (int i = 0; i < 16; i++)
	{
		char a;
		int val = temp % 16;
		if (val < 10)
			a = (char)(48+val);
		else
			a = (char)(55+val);
		ar[15 - i] = a;
		temp /= 16;
	}
}

void LDiffTo8SDiffs (unsigned long long int diff, int sDiffIn[8])
{
	std::string diffstr;
	int2BinS(diff, 32, diffstr);
	std::string ep;
	// The Expansion box Ebox
	ep.resize(48);
	for (int i=0;i<48;i++)
	{
	   ep[i] = diffstr[Ebox[i]-1];
	}
	for (int i = 0; i < 8; i++)
	{
		sDiffIn[i] = strToInt(ep.substr(6*i,6),6);
	}
}

//struct sOutput
//{
//	int out;
//	int nEvents;
//}; typedef sOutput sOutput;

void beforedlct(double state[64], uint64_t thediff)
{
	ddtInit();
	int sDiffIn[8];
	
	LDiffTo8SDiffs(thediff, sDiffIn);
	int snDiffOut[8][16];
	for (int i = 0; i < 8; i++)
		for (int k = 0; k < 16; k++)
			snDiffOut[i][k] = ddt[i][sDiffIn[i]][k];

	int sDiffIn2[8];
	int snDiffOut2[8][16];
	int oops = 0;
	for (uint64_t round1out = 0; round1out < 16; round1out++)
	{
		if (snDiffOut[SBOXA-1][round1out]==0)
			continue;
		double round1Prob = (double)(snDiffOut[SBOXA-1][round1out])/64.0;
		uint64_t r1Diff = round1out<<(8-SBOXA)*4;
		std::string diffstr;
		int2BinS(r1Diff, 32, diffstr);
		std::string pc;
		pc.resize(32);
		for(int i=0;i<32;i++)
			pc[i]=diffstr[per[i]-1];
		LDiffTo8SDiffs(strToInt(pc, 32), sDiffIn2);
		for (int sb = 0; sb < 8; sb++)
		{
			for (int k = 0; k < 16; k++)
			{
				snDiffOut2[sb][k] = ddt[sb][sDiffIn2[sb]][k];
			}
		}

		//For the specific experiment
		for (uint64_t i = 0; i < 16777216; i++)
		{
			bool possible=true;
			double round2Prob = 1.0;
			uint64_t sout = 0;
			uint64_t temp = i;
			for (int sb=7;sb>=0;sb--)
			{
				if(/*sb==0||sb==6*/ sb==3||sb==5)
					continue;
				if (snDiffOut2[sb][temp%16]==0)
				{
					possible=false;
					break;
				}
				round2Prob *= ((double)(snDiffOut2[sb][temp%16])/64.0);
				sout += (temp%16)<<4*(7-sb);
				temp/=16;
			}
			if(!possible)
				continue;
			round2Prob *= round1Prob;
			int mpos = 0;
			int2BinS(sout, 32, diffstr);
			//if(diffstr[per[16]-1]-48)
			//	int oops=1;
			for(int t=0;t<6;t++)
				mpos += ipow(2, 5 - t)*((int)diffstr[per[((LSBOX - 1) * 4 - 1 + t + 32) % 32] - 1] - 48);
			state[mpos] += round2Prob;
		}
	}
}

double dlctProb (double stateProb[64])
{
	double prob = 0.;
	int dlct[64];
	for (int i=0;i<64;i++)
		dlct[i]=0;
	for (int p1 = 0; p1 < 64; p1++)
	{
		int row = (p1 % 2) +2*(p1/32);
		int col = 0;
		int p1Temp=p1/2;
		for (int c=0;c<4;c++)
		{
			col += ipow(2,c)*(p1Temp%2);
			p1Temp /= 2;
		}
		int temp=s[LSBOX-1][row][col];
		bool p1MP = false;	//the mask parity of the sbox output of p1
		for (int a = 0; a < 4; a++)
		{
			p1MP ^= (temp % 2);
			temp/=2;
		}
		
		for (int p2 = 0; p2 < 64; p2++)
		{
			int xorin = p1 ^ p2;
			row = (p2 % 2) +2*(p2/32);
			col = 0;
			int p2Temp=p2/2;
			for (int c=0;c<4;c++)
			{
				col += ipow(2,c)*(p2Temp%2);
				p2Temp /= 2;
			}
			temp=s[LSBOX-1][row][col];
			bool p2MP = false;	//the mask parity of the sbox output of p2
			for (int a = 0; a < 4; a++)
			{
				p2MP ^= (temp % 2);
				temp/=2;
			}
			if (p1MP == p2MP)
				dlct[xorin]++;
		}
	}
	for (int diff = 0; diff < 64; diff++)
		prob += ((double)(dlct[diff])/64.)*stateProb[diff];
	return prob;
}

void keySchedule(unsigned char key[16][6], string &kp)
{
	int key1[64];

	for (int i = 0; i<64; i++)
		key1[i] = kp[i] - '0';
	//getting 56 bit key from 64 bit using the parity bits
	int keyp[56] = { 57  , 49   , 41   ,33 ,   25   , 17 ,   9,
		1 ,  58    ,50  , 42 ,   34  ,  26  , 18,
		10 ,   2   , 59 ,  51 ,  43  ,  35  , 27,
		19  , 11   ,  3  , 60  ,  52  ,  44  , 36,
		63  , 55  ,  47  , 39  ,  31   , 23  , 15,
		7  , 62   , 54  , 46  ,  38  ,  30  , 22,
		14  ,  6   , 61  , 53  ,  45   , 37  , 29,
		21  , 13  ,   5  , 28   , 20  ,  12  ,  4 };
	//D box configuration each 28 to 24 bit
	int key2[48] = { 14,    17,   11,    24,     1,    5,
		3   , 28 ,  15    , 6  ,  21  , 10 ,
		23    ,19  , 12   ,  4 ,  26  ,  8,
		16     ,7  , 27  ,  20  ,  13  ,  2,
		41   , 52 ,  31  ,  37  ,  47  , 55,
		30   , 40  , 51  ,  45  ,  33 , 48,
		44    ,49 ,  39  ,  56  ,  34 ,  53,
		46    ,42 ,  50  ,  36 ,   29 ,  32 };
	int keyl[28], keyr[28], nshift, temp1, temp2, pkey[56];

	//key generation
	//int t = 1, j, row, col, temp, round = nRounds;
	//j = 0;
	for (int i = 0; i < 56; i++)
		pkey[i] = key1[keyp[i] - 1];
	for (int i = 0; i < 28; i++)
		keyl[i] = pkey[i];
	for (int i = 0; i < 28; i++)
		keyr[i] = pkey[i + 28];
	//round key generation
	for (int i = 0; i < 16; i++)
	{
		if (i == 0 || i == 1 || i == 8 || i == 15)
			nshift = 1;
		else
			nshift = 2;
		while (nshift--)
		{
			temp1 = keyl[0];
			temp2 = keyr[0];
			for (int j = 0; j < 27; j++)
			{
				keyl[j] = keyl[j + 1];
				keyr[j] = keyr[j + 1];
			}
			keyl[27] = temp1;
			keyr[27] = temp2;
		}
		for (int j = 0; j < 3; j++)
		{
			uint8_t subkey = 0;
			for (int tt = 0; tt < 8; tt++)
				subkey |= ((keyl[key2[tt + 8 * j] - 1] << (7 - tt)));
			key[i][j] = subkey;
		}
		for (int j = 3; j < 6; j++)
		{
			uint8_t subkey = 0;
			for (int tt = 0; tt < 8; tt++)
				subkey |= ((keyr[key2[tt + 8 * j] - 1 - 28] << (7 - tt)));
			key[i][j] = subkey;
		}
	}
}

string des(string &kp, string &p, int nRounds)
{
	std::string l,r,ap="",ke,rtem, afterRounds;
    int key1[64];

		for(int i=0;i<64;i++)
		    key1[i]=kp[i]-'0';
		//getting 56 bit key from 64 bit using the parity bits
		int keyp[56]={57  , 49   , 41   ,33 ,   25   , 17 ,   9,
		       1 ,  58    ,50  , 42 ,   34  ,  26  , 18,
		      10 ,   2   , 59 ,  51 ,  43  ,  35  , 27,
		      19  , 11   ,  3  , 60  ,  52  ,  44  , 36,
		      63  , 55  ,  47  , 39  ,  31   , 23  , 15,
		       7  , 62   , 54  , 46  ,  38  ,  30  , 22,
		      14  ,  6   , 61  , 53  ,  45   , 37  , 29,
		      21  , 13  ,   5  , 28   , 20  ,  12  ,  4};
		//D box configuration each 28 to 24 bit
		int key2[48]={14,    17,   11,    24,     1,    5,
		3   , 28 ,  15    , 6  ,  21  , 10 ,
		23    ,19  , 12   ,  4 ,  26  ,  8,
		16     ,7  , 27  ,  20  ,  13  ,  2,
		41   , 52 ,  31  ,  37  ,  47  , 55,
		30   , 40  , 51  ,  45  ,  33 , 48,
		44    ,49 ,  39  ,  56  ,  34 ,  53,
		46    ,42 ,  50  ,  36 ,   29 ,  32};
		int key[16][48],keyl[28],keyr[28],nshift,temp1,temp2,pkey[56];

		//key generation
		int t=1,j,row,col,temp,round=nRounds;
		j=0;
		for(int i=0;i<56;i++)
		    pkey[i]=key1[keyp[i]-1];
		for(int i=0;i<28;i++)
		    keyl[i]=pkey[i];
		for(int i=0;i<28;i++)
		    keyr[i]=pkey[i+28];
		//round key generation
		for(int i=0;i<16;i++)
		{

		    if(i==0||i==1||i==8||i==15)
		        nshift=1;
		    else
		        nshift=2;
		    while(nshift--)
		    {
		        temp1=keyl[0];
			    temp2=keyr[0];
			    for(j=0;j<27;j++)
			    {
			        keyl[j]=keyl[j+1];
			        keyr[j]=keyr[j+1];
			    }
			    keyl[27]=temp1;
			    keyr[27]=temp2;
			}
			for(j=0;j<24;j++)
			    key[i][j]=keyl[key2[j]-1];
			for(j=24;j<48;j++)
			    key[i][j]=keyr[key2[j]-1-28];
		}

		

			l=p.substr(0,32);
			r=p.substr(32,32);
			//DES Encryption
			//std::string output;
			bool difflin=false;
			while(round--)
			{
			   rtem=r;
			   t=1;
			   std::string ep="",xorout="",sout="";
			   // The Expansion box Ebox
			   ep.resize(48);
			   for (int i=0;i<48;i++)
			   {
				   ep[i] = r[Ebox[i]-1];
			   }
			   ////the expansion P box
			   //ep+=r[31];
			   //for(i=0;i<32;i++)
			   //{
			   //    if((t+1)%6==0)
			   //    {
					 //  ep+=r[4*((t+1)/6)];
			   //        t++;
			   //    }
			   //    if(t%6==0&&i!=0)
			   //    {
			   //        ep+=r[4*(t/6)-1];
			   //        t++;
			   //     }
			   //    ep+=r[i];
			   //    t++;
			   //}
			   //ep+=r[0];
			   //Key xor with output of expansion p box
			   for(int i=0;i<48;i++)
			       xorout+=char(((int(ep[i])-48)^key[16-round-1][i])+48);
			   //sbox compression 48bit to 32 bit
			   int pos = 0;
			   for(int i=0;i<48;i+=6)
			   {
					row=(int(xorout[i+5])-48)+(int(xorout[i])-48)*2;
					col= (int(xorout[i+1])-48)*8+(int(xorout[i+2])-48)*4+(int(xorout[i+3])-48)*2+(int(xorout[i+4])-48);
					temp=s[i/6][row][col];
					for (int a = 0; a < 4; a++)
					{
						sout.insert(sout.begin()+pos, char(temp%2+48));
						temp/=2;
					}
					pos += 4;
					//while(temp>0)
					//{
					//    soutt+=char(temp%2+48);
					//    temp/=2;
					//}
					//while(soutt.length()!=4)
					//    soutt+='0';
					//for(j=soutt.length()-1;j>=0;j--)
					//    sout+=soutt[j];
				}
			   //straight pbox that is permutation of the sbox output
			   if (round)
			   {
				char pc[32];
				for(int i=0;i<32;i++)
				   pc[i]=sout[per[i]-1];
			   
				r="";
				for(int i=0;i<32;i++)
				   r+=char(((int(pc[i])-48)^(int(l[i])-48))+48);
			   }
			   else
				   r=sout;
				l=rtem;
			
			//cout<<"Output after Round"<<16-round<<endl;


			   if(!round)
			   {
				   afterRounds.resize(64);
				   for(int i = 0; i < 32; i++)
				   {
					   afterRounds[i] = r[i];
					   afterRounds[32+i] = l[i];
				   }
			   }

			   
			   //if (round == 15)
			   //{
				  // if (testN % 2)
				  // {
					 //  r11=strToInt(r,32);
					 //  if(r10==r11)
					 //  {
						//   count1++;
						//   difflin = true;
					 //  }
					 //  //bool r1check = true;
					 //  //for (int i = 0; i < 32; i++)
					 //  //{
						//  // if(r[i]!=rprev[i])
						//	 //  r1check=false;
					 //  //}
					 //  //if (r1check)
						//  // count2++;
					 //  //if(!r.compare(rprev))
						//  // count1++;
				  // }
				  // else
				  // {
					 //  //for (int iii=0;iii<32;iii++)
						//  // rprev[iii]=r[iii];
					 //  r10=strToInt(r,32);
				  // }
				  // //round = 16;
				  // //break;
			   //}
			   //if (round == 11)
			   //{
				  // if (testN % 2)
				  // {
					 //  stest2 = bool(r[2]-'0')^bool(r[7]-'0')^bool(r[13]-'0')^bool(r[24]-'0');
					 //  if (stest1 == stest2)
					 //  {
						//   count++;
						//   //if(difflin)
						//	  // count2++;
					 //  }
				  // }
				  // else
					 //  stest1 = bool(r[2]-'0')^bool(r[7]-'0')^bool(r[13]-'0')^bool(r[24]-'0');
				  // //round = 16;
				  // //break;
			   //}

			   //if (round == 9)
			   //{
				  // if (testN % 2)
				  // {
					 //  stest2 = bool(r[2]-'0')^bool(r[7]-'0')^bool(r[13]-'0')^bool(r[24]-'0');
					 //  if (stest1 == stest2)
					 //  {
						//   count2++;
						//   //if(difflin)
						//	  // count2++;
					 //  }
				  // }
				  // else
					 //  stest1 = bool(r[2]-'0')^bool(r[7]-'0')^bool(r[13]-'0')^bool(r[24]-'0');
				  // round = 16;
				  // break;
			   //}

			   //if(!round)
				  // cout<<cip<<endl;
			}

			//output.resize(64);
			//for(int i = 0; i < 64; i++)
			//	output[i] = afterRounds[IPR[i]-1];

			//string cip="";
			//for(int i=0;i<64;i+=4)
			//{
			//    int te;
			//    te=(int(output[i])-48)*8+(int(output[i+1])-48)*4+(int(output[i+2])-48)*2+(int(output[i+3])-48);
			//    if(te<10)
			//        cip+=char(te+48);
			//    else
			//        cip+=char(te+55);
			//}
			////for(int i=0;i<32;i+=4)
			////{
			////    int te;
			////    te=(int(r[i])-48)*8+(int(r[i+1])-48)*4+(int(r[i+2])-48)*2+(int(r[i+3])-48);
			////    if(te<10)
			////        cip+=char(te+48);
			////    else
			////        cip+=char(te+55);
			////}
			//system("pause");



//test case
/*
PLAIN=
14A7D67818CA18AD
KEY=
AABB09182736CCDD
*/

    //DES decryption
		//round=16;
		//string ltem;
		//while(round--)
		//{
		//    ltem=l;
		//    t=1;
		//    string ep="",xorout="",sout="",soutt;
		//    //the expansion P box
		//    ep+=l[31];
		//    for(i=0;i<32;i++)
		//    {
		//        if((t+1)%6==0)
		//        {
		//            ep+=l[4*((t+1)/6)];
		//            t++;
		//        }
		//        if(t%6==0&&i!=0)
		//        {
		//            ep+=l[4*(t/6)-1];
		//            t++;
		//        }
		//        ep+=l[i];
		//        t++;
		//    }
		//    ep+=l[0];
		//    //Key xor with output of expansion p box
		//    for(i=0;i<48;i++)
		//        xorout+=char(((int(ep[i])-48)^key[round][i])+48);
		//    //sbox compression 48bit to 32 bit
		//    for(i=0;i<48;i+=6)
		//    {
		//        row=(int(xorout[i+5])-48)+(int(xorout[i])-48)*2;
		//        col= (int(xorout[i+1])-48)*8+(int(xorout[i+2])-48)*4+(int(xorout[i+3])-48)*2+(int(xorout[i+4])-48);
		//        temp=s[i/6][row][col];
		//        soutt="";
		//        while(temp>0)
		//        {
		//            soutt+=char(temp%2+48);
		//            temp/=2;
		//        }
		//        while(soutt.length()!=4)
		//            soutt+='0';
		//        for(j=soutt.length()-1;j>=0;j--)
		//            sout+=soutt[j];
		//    }
		//    //straight pbox that is permutation of the sbox output
		//    char pc[32];
		//    for(i=0;i<32;i++)
		//        pc[i]=sout[per[i]-1];
		//    l="";
		//    for(i=0;i<32;i++)
		//        l+=char(((int(pc[i])-48)^(int(r[i])-48))+48);
		//    r=ltem;
		//    cout<<"Decrypted Output after Round"<<16-round<<endl;
		//    string cip="";
		//    for(i=0;i<32;i+=4)
		//    {
		//        int te;
		//        te=(int(l[i])-48)*8+(int(l[i+1])-48)*4+(int(l[i+2])-48)*2+(int(l[i+3])-48);
		//        if(te<10)
		//            cip+=char(te+48);
		//        else
		//            cip+=char(te+55);
		//    }
		//    for(i=0;i<32;i+=4)
		//    {
		//        int te;
		//        te=(int(r[i])-48)*8+(int(r[i+1])-48)*4+(int(r[i+2])-48)*2+(int(r[i+3])-48);
		//        if(te<10)
		//            cip+=char(te+48);
		//        else
		//            cip+=char(te+55);
		//    }
		//    //cout<<cip<<endl;
		//}
        //system("pause");
	return afterRounds;
}

void cmptFullDiff(uint64_t halfdiff, uint64_t *diff, int sbox)	//Compute all the possible diffs after the active sbox
{
	uint64_t rdiff = halfdiff << (4 * (8 - sbox));
	uint64_t ldiff = 0;
	for (uint64_t i = 0; i < 16; i++)
	{
		uint64_t indiff = halfdiff << 1;
		if (ddt[sbox-1][indiff][i] < 2)
		{
			diff[i] = 0xffffffffffffffff;	//This diff is not relevant!!
			continue;
		}
		unsigned char aftrs[4];
		for (int pos = 0; pos < 4; pos++)
		{
			if (pos==(sbox-1)/2)
			{
				if (sbox % 2)
					aftrs[pos] = i << 4;
				else
					aftrs[pos] = i;
			}
			else
			{
				aftrs[pos] = 0;
			}
		}
		unsigned char aftrp[4];
		for (int i = 0; i < 4; i++)
			aftrp[i] = 0;
		for (int j = 0; j<32; j++)
		{
			int shift_size = per[j];
			unsigned char shift_byte = 0x80 >> ((shift_size - 1) % 8);
			shift_byte &= aftrs[(shift_size - 1) / 8];
			shift_byte <<= ((shift_size - 1) % 8);

			aftrp[j / 8] |= (shift_byte >> j % 8);
		}

		ldiff = 0;
		for (int j = 0; j < 4; j++)
			ldiff += (uint64_t)(aftrp[j]) << 8 * (3 - j);
		diff[i] = (ldiff << 32) + rdiff;
	}
}

void cmptFullMask(uint64_t halfmask, int sbox, uint64_t *fullmask)
{
	uint32_t rmask = (halfmask << (4 * (6 - sbox) + 3)) | (halfmask >> (32 - (4 * (6 - sbox) + 3)));
	for (uint64_t maskOut = 0; maskOut < 16; maskOut++)
	{
		//if (abs(lat[sbox][halfmask][maskOut]) < 10)
		//{
		//	fullmask[maskOut] = 0xffffffffffffffff;
		//	continue;
		//}
		
		unsigned char b4p[4];
		for (int pos = 0; pos < 4; pos++)
		{
			if (pos == (sbox / 2))
			{
				if (sbox % 2)
					b4p[pos] = maskOut;
				else
					b4p[pos] = maskOut << 4;
			}
			else
			{
				b4p[pos] = 0;
			}
		}
		unsigned char aftrp[4];
		for (int i = 0; i < 4; i++)
			aftrp[i] = 0;
		for (int j = 0; j<32; j++)
		{
			int shift_size = per[j];
			unsigned char shift_byte = 0x80 >> ((shift_size - 1) % 8);
			shift_byte &= b4p[(shift_size - 1) / 8];
			shift_byte <<= ((shift_size - 1) % 8);

			aftrp[j / 8] |= (shift_byte >> j % 8);
		}

		fullmask[maskOut] = 0;
		for (int i = 0; i < 4; i++)
			fullmask[maskOut] |= ((uint64_t(aftrp[i])) << 8 * (7 - i));
		fullmask[maskOut] += rmask;
	}
}

double dlct3roundsConLin(double q, uint64_t rDiff, double firstProb, bool useFirstProb)
{
	double state[64];
	for (int i = 0; i<64; i++)
		state[i] = 0.;
	beforedlct(state, rDiff);

	double sum = 0.;
	for (int i = 0; i<64; i++)
		sum += state[i];

	double prob = dlctProb(state);
	double pr = prob * firstProb + 0.5*(1. - firstProb);
	//firstProb is the probability of the first round of the.
	//In case of buildings we don't consider it.
	if(useFirstProb)
		return 2.*(2 * pr - 1)*q*q;
	else
		return 2.*(2 * prob - 1)*q*q;
}

static int NEx = 0;
static int Nss = 0;
static int Nsb = 0;

bool keyRec8RStructuresTest()
{
	uint64_t outDiff[11] = { 6, 13, 3, 10, 14, 7, 12, 15, 5, 9, 11 };
	uint64_t diff[5];
	diff[0] = 0x1000000000000000;
	diff[2] = 0x0020000000000000;
	diff[3] = 0x0000200000000000;
	diff[1] = 0x0000000800000000;
	diff[4] = 0x0000000000000400;
	uint64_t mask = 0x00808202;

	bool maskParity[64][TESTN];
	unsigned char input[8];
	unsigned char output[8];
	uint64_t rVal = 0;
	for (int i = 0; i < 4; i++)
		rVal |= ((uint64_t)(rand() % 256) << (8 * i));
	uint64_t lVal;

	//Generate a key
	uint64_t key = 0;
	for (int i = 0; i < 8; i++)
		key |= ((uint64_t)(rand() % 256) << 8 * i);
	string kp;
	int2BinS(key, 64, kp);
	unsigned char keyt[16][6];
	keySchedule(keyt, kp);

	uint64_t val1 = rVal, val2 = rVal ^ diff[4];
	uint64_t val1rel = 0, val2rel = 0;
	uint64_t relBit = 0x80;
	for (uint64_t i = 0; i < 6; i++)
	{
		if (val1&relBit)
			val1rel += ipow(2, i);
		if (val2&relBit)
			val2rel += ipow(2, i);
		relBit <<= 1;
	}
	uint64_t msb_2 = 64 + 128;
	uint64_t relK = ((uint64_t)keyt[0][3] & msb_2) >> 2;
	relK += ((uint64_t)keyt[0][4] & 0xf);
	val1rel ^= relK;
	val2rel ^= relK;
	uint64_t row = (val1rel % 2) + 2 * (val1rel / 32);
	uint64_t col = 0;
	uint64_t tmp = val1rel / 2;
	for (uint64_t t = 0; t < 4; t++)
	{
		col += ipow(2, t)*(tmp % 2);
		tmp /= 2;
	}
	uint64_t sval1 = s[5][row][col];
	row = (val2rel % 2) + 2 * (val2rel / 32);
	col = 0;
	tmp = val2rel / 2;
	for (uint64_t t = 0; t < 4; t++)
	{
		col += ipow(2, t)*(tmp % 2);
		tmp /= 2;
	}
	uint64_t sval2 = s[5][row][col];
	uint64_t out6 = sval1 ^ sval2;
	bool smallP = false;
	if (out6 == outDiff[8] || out6 == outDiff[9] || out6 == outDiff[10])
		smallP = true;

	uint64_t keyCount[64] = { 0 };

	for (int pTest = 0; pTest < TESTN; pTest++)
	{
		// Init the left part of the srtucture (each str. is of 32 values) 
		if (!(pTest % 32))
		{
			lVal = 0;
			for (int i = 0; i < 4; i++)
				lVal |= ((uint64_t)(rand() % 256) << (8 * (i + 4)));
		}

		//Create the appropriate plaintext
		uint64_t plain = lVal | rVal;
		uint64_t temp = pTest % 32;
		for (int j = 0; j < 5; j++)
		{
			if (temp % 2)
				plain ^= diff[j];
			temp /= 2;
		}
		for (int posi = 0; posi < 8; posi++)
		{
			input[7 - posi] = plain % 256;
			plain /= 256;
		}

		process_message(input, output, keyt, 8);
		uint64_t outp = 0;
		for (int ii = 0; ii < 8; ii++)
			outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
		bool half1Parity = parity(outp & mask);
		for (uint64_t keyCandidate = 0; keyCandidate < 64; keyCandidate++)
		{
			//find the value that enter to s4
			uint64_t val = 0;
			uint64_t rellevantBit = 0x8000;
			for (uint64_t i = 0; i < 6; i++)
			{
				if (outp&rellevantBit)
					val += ipow(2, i);
				rellevantBit <<= 1;
			}
			val ^= keyCandidate;

			uint64_t Row = (val % 2) + 2 * (val / 32);
			uint64_t Col = 0;
			uint64_t temp = val / 2;
			for (uint64_t t = 0; t < 4; t++)
			{
				Col += ipow(2, t)*(temp % 2);
				temp /= 2;
			}
			uint64_t sval = s[3][Row][Col];
			uint64_t f8out;
			if (sval % 2)
				f8out = 0x8000000000000000;
			else
				f8out = 0;
			bool half2Parity = f8out ^ (outp & 0x8000000000000000);
			maskParity[keyCandidate][pTest] = half1Parity ^ half2Parity;
		}
	}

	for (uint64_t keyC = 0; keyC < 64; keyC++)
	{
		for (uint64_t outDiffInd = 0; outDiffInd < 11; outDiffInd++)
		{
			int count = 0;
			//Run over the structures and check the parity of pairs that the output diff is outDiff[outDiffInd].
			for (uint64_t i = 0; i < TESTN; i += 32)
			{
				for (uint64_t c1 = 0; c1 < 16; c1++)
				{
					if (maskParity[keyC][i + c1] == maskParity[keyC][i + 16 + (c1^outDiff[outDiffInd])])
						count++;
					//for (uint64_t c2 = 16; c2 < 32; c2++)
					//{
					//	if ((c1 ^ (c2 % 16)) == outDiff[outDiffInd] && maskParity[keyC][i + c1] == maskParity[keyC][i + c2])
					//		count++;
					//}
				}
			}
			if (count > keyCount[keyC])
				keyCount[keyC] = count;
		}
	}
	
	int maxK = 0;
	uint64_t indK;
	for (uint64_t i = 0; i < 64; i++)
	{
		if (keyCount[i] > maxK)
		{
			maxK = keyCount[i];
			indK = i;
		}
	}

	uint64_t sixBits = 63;
	uint64_t thekey = ((uint64_t)keyt[7][2] & sixBits);
	if (indK == thekey)
	{
		if (smallP)
			Nss++;
		else
			Nsb++;
		return true;
	}

	return false;
}

int cipherRec7RStructuresTest()
{
	//ofstream myfile1;
	//myfile1.open("smallExp.txt");

	uint64_t mask = 0x0080820280000000;
	uint64_t diff[5];
	diff[3] = 0x1000000000000000;
	diff[1] = 0x0020000000000000;
	diff[0] = 0x0000200000000000;
	diff[2] = 0x0000000800000000;
	diff[4] = 0x0000000000000400;

	uint64_t outDiff[11] = { 6, 13, 3, 10, 14, 7, 12, 15, 5, 9, 11 };

	int ns = 0;
	int sumExp = 0;

	for (int keytest = 0; keytest < KEYTEST; keytest++)
	{
		//Generate a key
		uint64_t key = 0;
		for (int i = 0; i < 8; i++)
			key |= ((uint64_t)(rand() % 256) << 8 * i);
		string kp;
		int2BinS(key, 64, kp);
		unsigned char keyt[16][6];
		keySchedule(keyt, kp);

		bool maskParity[TESTN];
		int count = 0;
		unsigned char input[8];
		unsigned char output[8];
		uint64_t rVal = 0;
		for (int i = 0; i < 4; i++)
			rVal |= ((uint64_t)(rand() % 256) << (8 * i));
		//string rrrr;
		//int2BinS(rVal, 64, rrrr);
		
		uint64_t lVal;
		bool success = false;

		for (uint64_t testN = 0; testN < TESTN; testN++)
		{
			// Init the left part of the srtucture (each str. is of 32 values) 
			if (!(testN % 32))
			{
				lVal = 0;
				for (int i = 0; i < 4; i++)
					lVal |= ((uint64_t)(rand() % 256) << (8 * (i + 4)));
			}
			//string llll;
			//int2BinS(lVal, 64, llll);

			//Create the appropriate plaintext
			uint64_t plain = lVal | rVal;
			uint64_t temp = testN % 32;
			for (int j = 0; j < 5; j++)
			{
				if (temp % 2)
					plain ^= diff[j];
				temp /= 2;
			}
			temp = plain;
			for (int posi = 0; posi < 8; posi++)
			{
				input[7 - posi] = temp % 256;
				temp /= 256;
			}

			process_message(input, output, keyt, 7);
			uint64_t outp = 0;
			for (int ii = 0; ii < 8; ii++)
				outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
			maskParity[testN] = parity(outp & mask);
		}

		for (uint64_t outDiffInd = 0; outDiffInd < 11; outDiffInd++)
		{
			count = 0;
			//Run over the structures and check the parity of pairs that the output diff is outDiff[outDiffInd].
			for (uint64_t i = 0; i < TESTN; i += 32)
			{
				for (uint64_t c1 = 0; c1 < 16; c1++)
				{
					for (uint64_t c2 = 16; c2 < 32; c2++)
					{
						if ((c1^(c2 % 16)) == outDiff[outDiffInd] && maskParity[i + c1] == maskParity[i + c2])
							count++;
					}
				}
			}
			double res = double(count) / (0.5*((double)(TESTN)));
			//myfile1 << abs(res - 0.5) << endl;
			if (res - 0.5 > 0.06)
			{
				ns++;
				sumExp += outDiffInd + 1;
				success = true;
				
				break;
			}
		}
		if (!success)
		{
			sumExp += 11;
			//myfile1 << "not successed key = " << keytest << endl;
		}
	}
	//myfile1.close();
	double avgNExp = (double)sumExp / (double)KEYTEST;
	return ns;
}

void speciphicDiffExp()
{
	ofstream myfile1;
	myfile1.open("smallExp.txt");
	uint64_t diff;
	uint64_t mask;

	for (int test = 0; test < 3; test++)
	{
		switch (test)
		{
		case 0:
			diff = 0x0020000800000400;
			mask = 0x0080820280000000;
			break;
		case 1:
			diff = 0x0080820060000000;
			mask = 0x2104008000008000;
			break;
		case 2:
			diff = 0x0000020240000000;
			mask = 0x2104008000008000;
			break;
		default:
			break;
		}

		//Print the values
		myfile1 << "diff = ";
		char diffHex[16];
		prtInt64AsHex(diff, diffHex);
		for (int i = 0; i < 16; i++)
			myfile1 << diffHex[i];
		
		myfile1 << ", mask = ";
		char maskHex[16];
		prtInt64AsHex(mask, maskHex);
		for (int i = 0; i < 16; i++)
			myfile1 << maskHex[i];
		myfile1 << endl;

		//Put the diff into an array of char vars.
		unsigned char diffa[8];
		uint64_t difft = diff;
		for (int i = 0; i < 8; i++)
		{
			diffa[7 - i] = difft % 256;
			difft /= 256;
		}

		for (int keytest = 0; keytest < KEYTEST; keytest++)
		{
			//Generate a key
			uint64_t key = 0;
			for (int i = 0; i < 8; i++)
				key |= ((uint64_t)(rand() % 256) << 8 * i);
			string kp;
			int2BinS(key, 64, kp);
			unsigned char keyt[16][6];
			keySchedule(keyt, kp);

			double res = 0.;
			bool stest1, stest2;
			int count = 0;
			unsigned char input[8];
			for (int testN = 0; testN < TESTN; testN++)
			{
				unsigned char output[8];
				if (testN % 2)
				{
					for (int ii = 0; ii < 8; ii++)
						input[ii] ^= diffa[ii];
					process_message(input, output, keyt, 7);
					uint64_t outp = 0;
					for (int ii = 0; ii < 8; ii++)
						outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
					stest2 = parity(outp & mask);
					if (stest1 == stest2)
						count++;
				}
				else
				{
					for (int posi = 0; posi < 8; posi++)
						input[posi] = rand() % 256;
					process_message(input, output, keyt, 7);
					uint64_t outp = 0;
					for (int ii = 0; ii < 8; ii++)
						outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
					stest1 = parity(outp & mask);
				}
			}
			res = double(count) / (0.5*((double)(TESTN)));
			myfile1 << res - 0.5 << endl;
		}
		myfile1 << endl;
	}
	myfile1.close();
}

void sbox1startEndExp()
{
	double bestres = 0;
	uint64_t bestdiff;
	uint64_t bestmask;
	ofstream myfile1;
	myfile1.open("bigExp.txt");

	//Generate a key
	uint64_t rnd;
	uint64_t key = rand();
	for (int i = 0; i < 4; i++)
	{
		key <<= 15;
		rnd = rand();
		key |= rnd;
	}
	key <<= 4;
	rnd = rand() % 16;
	key |= rnd;
	string kp;
	int2BinS(key, 64, kp);
	unsigned char keyt[16][6];
	keySchedule(keyt, kp);

	for (uint64_t difftest = 0; difftest < 24; difftest++)
	{
		uint64_t diff[16];
		cmptFullDiff(2 * (difftest % 3) + 2, diff, 8 - (difftest / 3));

		for (int fd = 0; fd < 16; fd++)
		{
			uint64_t difft;
			if (diff[fd] != 0xffffffffffffffff)	//This diff means that this case is not relevant.
				difft = diff[fd];
			else
				continue;
			unsigned char diffa[8];
			for (int i = 0; i < 8; i++)
			{
				diffa[7 - i] = difft % 256;
				difft /= 256;
			}
			if (diff[fd] == 0xffffffffffffffff)
				continue;
			
			for (uint64_t masktest = 0; masktest < 64 * 8; masktest++)
			{
				uint64_t mask[16];
				cmptFullMask(masktest % 64, masktest / 64, mask);
				for (int fm = 0; fm < 16; fm++)
				{
					if (mask[fm] == 0xffffffffffffffff)
						continue;
					double res = 0.;
					for (int keytest = 0; keytest < KEYTEST; keytest++)
					{
						bool stest1, stest2;
						int count = 0;
						unsigned char input[8];
						//uint64_t plaint;
						//string plain;
						for (int testN = 0; testN < TESTN; testN++)
						{
							unsigned char output[8];
							
							
							if (testN % 2)
							{
								for (int ii = 0; ii < 8; ii++)
									input[ii] ^= diffa[ii];
								process_message(input, output, keyt, 7);
								uint64_t outp = 0;
								for (int ii = 0; ii < 8; ii++)
									outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
								//int2BinS(plaint^diff[fd], 64, plain);
								//uint64_t outp = strToInt(des(kp, plain, 7), 64);
								stest2 = parity(outp & mask[fm]);
								if (stest1 == stest2)
									count++;
							}
							else
							{
								for (int posi = 0; posi < 8; posi++)
									input[posi] = rand() % 256;
								process_message(input, output, keyt, 7);
								uint64_t outp = 0;
								for (int ii = 0; ii < 8; ii++)
									outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
								
								//plaint = rand();
								//for (int i = 0; i < 4; i++)
								//{
								//	plaint <<= 15;
								//	rnd = rand();
								//	plaint |= rnd;
								//}
								//plaint <<= 4;
								//rnd = rand() % 16;
								//plaint |= rnd;
								//int2BinS(plaint, 64, plain);
								//uint64_t outp = strToInt(des(kp, plain, 7), 64);
								stest1 = parity(outp & mask[fm]);
							}
						}
						res += double(count) / (0.5*((double)(TESTN)));
					}
					//if(diff[fd]==0x0080820060000000 && mask[fm]==0x2104008000008000)
					//{
					//	myfile1 << "diff = ";
					//	char diffHex[16];
					//	prtInt64AsHex(diff[fd], diffHex);
					//	for (int i = 0; i < 16; i++)
					//		myfile1 << diffHex[i];
					//	myfile1 << ",  mask = ";
					//	char maskHex[16];
					//	prtInt64AsHex(mask[fm], maskHex);
					//	for (int i = 0; i < 16; i++)
					//		myfile1 << maskHex[i];
					//	myfile1 << ",  result = " << abs(res - 0.5) << endl;;
					//}
					if (res < 0.9 && abs(res - 0.5) > 0.015)	//Print only the interesting results.
					{
						myfile1 << "diff = ";
						char diffHex[16];
						prtInt64AsHex(diff[fd], diffHex);
						for (int i = 0; i < 16; i++)
							myfile1 << diffHex[i];
						myfile1 << ",  mask = ";
						char maskHex[16];
						prtInt64AsHex(mask[fm], maskHex);
						for (int i = 0; i < 16; i++)
							myfile1 << maskHex[i];
						myfile1 << ",  result = " << abs(res - 0.5) << endl;;
					}
					if (res < 0.9 && abs(res - 0.5) > bestres)
					{
						bestres = abs(res - 0.5);
						bestdiff = diff[fd];
						bestmask = mask[fm];
					}
				}
			}
		}
	}
	myfile1 << "\n" << "bestres = " << bestres << ", bestdiff = " 
		<< bestdiff << ", bestmask = " << bestmask << endl;
	
	myfile1.close();
}

void LangfordHellmanLikeExp()
{
	double bestres = 0;
	uint64_t bestdiff;
	uint64_t bestmask;
	ofstream myfile1;
	myfile1.open("LHExpS.txt");

	//Generate a key
	uint64_t rnd;
	uint64_t key = rand();
	for (int i = 0; i < 4; i++)
	{
		key <<= 15;
		rnd = rand();
		key |= rnd;
	}
	key <<= 4;
	rnd = rand() % 16;
	key |= rnd;
	string kp;
	int2BinS(key, 64, kp);
	unsigned char keyt[16][6];
	keySchedule(keyt, kp);

	for (uint64_t difftest = 0; difftest < 24; difftest++)
	{
		uint64_t diff = (2 * (difftest % 3) + 2) << (4 * (difftest / 3) + 32);

		uint64_t difft = diff;
			unsigned char diffa[8];
			for (int i = 0; i < 8; i++)
			{
				diffa[7 - i] = difft % 256;
				difft /= 256;
			}

			for (uint64_t masktest = 0; masktest < 4/*64 * 8*/; masktest++)
			{
				uint64_t maska;
				switch (masktest)
				{
				case 0:
					maska = 0x2104008000008000;
					break;
				case 1:
					maska = 0x0080820280000000;
					break;
				case 2:
					maska = 0x8040104000140000;
					break;
				case 3:
					maska = 0x8040104000110000;
					break;
				default:
					maska = 0;
					break;
				}
				//uint64_t mask[16];
				//cmptFullMask(masktest % 64, masktest / 64, mask);
				//for (int fm = 0; fm < 16; fm++)
				//{
					//if (mask[fm] == 0xffffffffffffffff)
					//	continue;
					double res = 0.;
					for (int keytest = 0; keytest < KEYTEST; keytest++)
					{
						bool stest1, stest2;
						int count = 0;
						unsigned char input[8];
						//uint64_t plaint;
						//string plain;
						for (int testN = 0; testN < TESTN; testN++)
						{
							unsigned char output[8];


							if (testN % 2)
							{
								for (int ii = 0; ii < 8; ii++)
									input[ii] ^= diffa[ii];
								process_message(input, output, keyt, 6);
								uint64_t outp = 0;
								for (int ii = 0; ii < 8; ii++)
									outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));
								//int2BinS(plaint^diff[fd], 64, plain);
								//uint64_t outp = strToInt(des(kp, plain, 7), 64);

								//stest2 = parity(outp & mask[fm]);
								stest2 = parity(outp & maska);
								if (stest1 == stest2)
									count++;
							}
							else
							{
								for (int posi = 0; posi < 8; posi++)
									input[posi] = rand() % 256;
								process_message(input, output, keyt, 6);
								uint64_t outp = 0;
								for (int ii = 0; ii < 8; ii++)
									outp |= (uint64_t(output[ii]) << (8 * (7 - ii)));

								//plaint = rand();
								//for (int i = 0; i < 4; i++)
								//{
								//	plaint <<= 15;
								//	rnd = rand();
								//	plaint |= rnd;
								//}
								//plaint <<= 4;
								//rnd = rand() % 16;
								//plaint |= rnd;
								//int2BinS(plaint, 64, plain);
								//uint64_t outp = strToInt(des(kp, plain, 7), 64);

								//stest1 = parity(outp & mask[fm]);
								stest1 = parity(outp & maska);
							}
						}
						res += double(count) / (0.5*((double)(TESTN)));
					}
					//if(diff[fd]==0x0080820060000000 && mask[fm]==0x2104008000008000)
					//{
					//	myfile1 << "diff = ";
					//	char diffHex[16];
					//	prtInt64AsHex(diff[fd], diffHex);
					//	for (int i = 0; i < 16; i++)
					//		myfile1 << diffHex[i];
					//	myfile1 << ",  mask = ";
					//	char maskHex[16];
					//	prtInt64AsHex(mask[fm], maskHex);
					//	for (int i = 0; i < 16; i++)
					//		myfile1 << maskHex[i];
					//	myfile1 << ",  result = " << abs(res - 0.5) << endl;;
					//}
					if (res < 0.9 && abs(res - 0.5) > 0.05)	//Print only the interesting results.
					{
						myfile1 << "diff = ";
						char diffHex[16];
						prtInt64AsHex(diff, diffHex);
						for (int i = 0; i < 16; i++)
							myfile1 << diffHex[i];
						myfile1 << ",  mask = ";
						char maskHex[16];
						//prtInt64AsHex(mask[fm], maskHex);
						prtInt64AsHex(maska, maskHex);
						for (int i = 0; i < 16; i++)
							myfile1 << maskHex[i];
						myfile1 << ",  result = " << abs(res - 0.5) << endl;;
					}
					if (res < 0.9 && abs(res - 0.5) > bestres)
					{
						bestres = abs(res - 0.5);
						bestdiff = diff;
						//bestmask = mask[fm];
						bestmask = maska;
					}
				//}
			}
	}
	myfile1 << "\n" << "bestres = " << bestres << ", bestdiff = "
		<< bestdiff << ", bestmask = " << bestmask << endl;

	myfile1.close();
}

double Phi(double z)
{
	//if (z > 0)
	//	return 1. - exp(-z * z / 2.) / (z + 0.5);
	//else
	//	return exp(-z * z / 2.) / (-z + 0.5);

	return 0.5*(1. + erf(z / sqrt(2.)));
}

double PErr(double t, double p, double N)
{
	double err1 = Phi(t * 2.*sqrt(N));
	double err2 = Phi((t + 0.5 - p) / sqrt(p*(1 - p) / N));
	return pow(err1,10.) * err2;

	//double err1 = 1. - Phi((t + 0.5 - p) / sqrt(p*(1 - p) / N));
	//double err2 = Phi(t * 2.*sqrt(N));
	//return err1 * err2;
}

int main()
{
	time_t ttt;
	srand((unsigned)time(&ttt));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(0., 1.);

	ddtInit();
	latInit();

	//sbox1startEndExp();
	LangfordHellmanLikeExp();

	double Pr = PErr(0.06, 0.5820316, 256);

	double ppp = dlct3roundsConLin(20. / 64., 0x60000000, 14. / 64., 1);

	speciphicDiffExp();
	int numS = 0;
	for (int i = 0; i < KEYTEST; i++)
	{
		if (keyRec8RStructuresTest())
			numS++;
	}

	double avgEx = (double)NEx / (double)KEYTEST;
	
	int ns = cipherRec7RStructuresTest();
	
	double outs8_4[10] = { 12., 10., 8., 8., 6., 6., 4., 4., 4., 2. };
	double outs6_4[9] = { 10., 10., 8., 8., 8., 8., 6., 4., 2. };
	double outs6_8[11] = { 16., 12., 6., 6., 6., 4., 4., 4., 2., 2., 2. };
	double mean84 = 0.;
	double mean64 = 0.;
	double mean68 = 0.;
	for (int i = 0; i < 10; i++)
		mean84 += (i + 1)*outs8_4[i] / 64.;
	for (int i = 0; i < 9; i++)
		mean64 += (i + 1)*outs6_4[i] / 64.;
	for (int i = 0; i < 11; i++)
		mean68 += (i + 1)*outs6_8[i] / 64.;

	//double Ps = 0.5*(1. + erf(2.*sqrt(7.) - sqrt(2.)*erfinv(2.*(1. - pow(2., -6.)) - 1.)));



	//double res = dlct3roundsConLin(20. / 64., 0x60000000);

	//speciphicDiffExp();
	return 0;
}

//dlctInit();
//ofstream f;
//f.open ("dlct.txt");
//for (int sbox = 0; sbox < 8; sbox++)
//{
//	f<<"sbox = "<<sbox+1<<"\n"<<endl;
//	for(int r=0; r < 64; r++)
//	{
//		f<<r<<":  ";
//		if(r<10)
//			f<<" ";
//		for(int c=0;c<16;c++)
//		{
//			if (r == 0 || c== 0)
//				f<<99<<" ";
//			else
//				f<<dlct[sbox][r][c]-32<<" ";
//		}
//		f<<endl;
//	}
//	f<<"\n"<<endl;
//}