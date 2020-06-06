#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<bitset>
#include<cstring>
#include<time.h>
#define MAXLEN 8388608
#define SEQ 2
#define LEN 128
#define MIN(a,b) ((a) < (b) ? (a) : (b))
using namespace std;

unsigned char data[MAXLEN];

class regroup {
	private:

        int total = 0;

        char m1[33] = "10010101001111010011100000110101";

		unsigned char data[MAXLEN];

		unsigned char output[MAXLEN + (MAXLEN / LEN) * SEQ];

		unsigned int rabinvalue[MAXLEN / LEN][8];

		unsigned int feature[MAXLEN / LEN][8];

		unsigned int lianbiao[MAXLEN / LEN][2];

		int disjiontset[MAXLEN / LEN];

		bool ifsort[MAXLEN / LEN];

		void rabin(int a1, int len, char* m) //rabin指纹
		{

			bitset<32> a(m);
			bitset<LEN * 8> input;
			bitset<8> inp2[LEN];
			int inputlen = LEN * 8;
			if(!(len - a1 * LEN >= 0 && len - a1 * LEN < LEN))
			{

			    int tmplen;
			    for(int i = a1 * LEN; i < a1 * (LEN + 1); i++)
				{
					bitset<8> tmp(data[i]);

					inp2[i - a1 * LEN] = tmp;
				}
				printf("?\n");
				//inp2转进input，方便接下来的指纹计算
				for(int i = 0; i < LEN; i++)
                {
                    for(int j = 0; j < 8; j++)
                    {
                        input[i * 8 + j] = inp2[i][j];
                    }
                }

                //完成rabin值计算
                int x = LEN;
                unsigned int tmp1;
                for(int k = 8; k >= 1; k--)
                {
                    for(int i = x * k - 1; i >= 31 + x * (k - 1); i--)
                    {

                        if(input[i] == 1)
                        {
                            tmp1 = 0;
                            for(int j = 31; j >= 0; j--)
                            {
                                if(input[i - 31 + j] != a[j])
                                {
                                    input[i - 31 + j] = 1;
                                }
                                else
                                {
                                    input[i - 31 + j] = 0;
                                }
                                tmp1 *= 2;
                                tmp1 += input[i - 31 + j];
                                if(tmp1 > rabinvalue[a1][k - 1])
                                {
                                    rabinvalue[a1][k - 1] = tmp1;
                                }
                            }
                        }
                        else
                        {
                            tmp1 *= 2;
                            if(tmp1 > rabinvalue[a1][k - 1])
                            {
                                rabinvalue[a1][k - 1] = tmp1;
                            }
                        }
                    }
                }

			}
			else
			{
				for(int i = 0; i < 8; i++)
                {
                    rabinvalue[a1][i] = 0;
                }


			}
			return;
			if(!(len - a1 * LEN >= 0 && len - a1 * LEN < LEN))
			{

			    int tmplen;
			    for(int i = a1 * LEN; i < a1 * (LEN + 1); i++)
				{
					bitset<8> tmp(data[i]);

					inp2[i - a1 * LEN] = tmp;
				}
				printf("?\n");
				//inp2转进input，方便接下来的指纹计算
				for(int i = 0; i < LEN; i++)
                {
                    for(int j = 0; j < 8; j++)
                    {
                        input[i * 8 + j] = inp2[i][j];
                    }
                }

                //完成rabin值计算
                int x = LEN;
                unsigned int tmp1;
                for(int k = 8; k >= 1; k--)
                {
                    for(int i = x * k - 1; i >= 31 + x * (k - 1); i--)
                    {

                        if(input[i] == 1)
                        {
                            tmp1 = 0;
                            for(int j = 31; j >= 0; j--)
                            {
                                if(input[i - 31 + j] != a[j])
                                {
                                    input[i - 31 + j] = 1;
                                }
                                else
                                {
                                    input[i - 31 + j] = 0;
                                }
                                tmp1 *= 2;
                                tmp1 += input[i - 31 + j];
                                if(tmp1 > rabinvalue[a1][k - 1])
                                {
                                    rabinvalue[a1][k - 1] = tmp1;
                                }
                            }
                        }
                        else
                        {
                            tmp1 *= 2;
                            if(tmp1 > rabinvalue[a1][k - 1])
                            {
                                rabinvalue[a1][k - 1] = tmp1;
                            }
                        }
                    }
                }

			}
			else
			{
				for(int i = 0; i < 8; i++)
                {
                    rabinvalue[a1][i] = 0;
                }


			}
		}

		void nrabin(int a1, int len, char* m) //rabin指纹
		{
			bitset<16> a(m);
			bitset<LEN * 8> input;
			bitset<8> inp2[LEN];
			int inputlen = LEN * 8;
			int subchunksize = LEN / 4;
			if(!(len - a1 * LEN >= 0 && len - a1 * LEN < LEN))
			{
			    int tmplen;
			    for(int i = a1 * LEN; i < len; i++)
				{
					bitset<8> tmp(data[i]);

					inp2[i - a1 * LEN] = tmp;
					tmplen = i - a1 + 1;

				}

				inputlen = tmplen * 8;
				//inp2转进input，方便接下来的指纹计算
				for(int i = 0; i < tmplen; i++)
                {
                    for(int j = 0; j < 8; j++)
                    {
                        input[i * 8 + j] = inp2[i][j];
                    }
                }

                //完成rabin值计算
                for(int i = inputlen - 1; i >= 31; i--)
                {
                    if(input[i] == 1)
                    {
                        for(int j = 31; j >= 0; j--)
                        {
                            if(input[i - 31 + j] != a[j])
                            {
                                input[i - 31 + j] = 1;
                            }
                            else
                            {
                                input[i - 31 + j] = 0;
                            }
                        }
                    }
                }
               // rabinvalue[a1] = input.to_ulong();
			}
			else
			{
				for(int i = a1 * LEN; i < a1 * LEN + LEN; i++)
				{
					bitset<8> tmp(data[i]);
					inp2[i - a1 * LEN] = tmp;
				}

				//inp2转进input，方便接下来的指纹计算
				for(int i = 0; i < LEN; i++)
                {
                    for(int j = 0; j < 8; j++)
                    {
                        input[i * 8 + j] = inp2[i][j];
                    }
                }

                //完成rabin值计算
                for(int i = inputlen - 1; i >= 31; i--)
                {
                    if(input[i] == 1)
                    {
                        for(int j = 31; j >= 0; j--)
                        {
                            if(input[i - 31 + j] != a[j])
                            {
                                input[i - 31 + j] = 1;
                            }
                            else
                            {
                                input[i - 31 + j] = 0;
                            }
                        }
                    }
                }
              //  rabinvalue[a1] = input.to_ulong();

			}
			if(len - a1 * LEN >= 0 && len - a1 * LEN < LEN)
			{
			    int tmplen;
			    for(int i = a1 * LEN; i < len; i++)
				{
					bitset<8> tmp(data[i]);

					inp2[i - a1 * LEN] = tmp;
					tmplen = i - a1 + 1;

				}

				inputlen = tmplen * 8;
				//inp2转进input，方便接下来的指纹计算
				for(int i = 0; i < tmplen; i++)
                {
                    for(int j = 0; j < 8; j++)
                    {
                        input[i * 8 + j] = inp2[i][j];
                    }
                }

                //完成rabin值计算
                for(int i = inputlen - 1; i >= 31; i--)
                {
                    if(input[i] == 1)
                    {
                        for(int j = 31; j >= 0; j--)
                        {
                            if(input[i - 31 + j] != a[j])
                            {
                                input[i - 31 + j] = 1;
                            }
                            else
                            {
                                input[i - 31 + j] = 0;
                            }
                        }
                    }
                }
               // rabinvalue[a1] = input.to_ulong();
			}
			else
			{
				for(int i = a1 * LEN; i < a1 * LEN + LEN; i++)
				{
					bitset<8> tmp(data[i]);
					inp2[i - a1 * LEN] = tmp;
				}

				//inp2转进input，方便接下来的指纹计算
				for(int i = 0; i < LEN; i++)
                {
                    for(int j = 0; j < 8; j++)
                    {
                        input[i * 8 + j] = inp2[i][j];
                    }
                }

                //完成rabin值计算
                for(int i = inputlen - 1; i >= 31; i--)
                {
                    if(input[i] == 1)
                    {
                        for(int j = 31; j >= 0; j--)
                        {
                            if(input[i - 31 + j] != a[j])
                            {
                                input[i - 31 + j] = 1;
                            }
                            else
                            {
                                input[i - 31 + j] = 0;
                            }
                        }
                    }
                }
              //  rabinvalue[a1] = input.to_ulong();

			}
			return;
		}

		//使用ntransform方法计算特征值
		void ntransform(int num)
		{
		    unsigned int m1[3];
		    unsigned int a1[3];
		    for(int i = 0; i < 8; i++)
            {
                //feature[num][i] = m1[i] * rabinvalue[num] + a1[i];
                feature[num][i] = rabinvalue[num][i];
            }
            return;
		}

		void finesse(int a1) //finesse算法识别相似块
		{
		    int num = a1 / LEN;
		    for(int i = 0; i < num; i++)
            {
                nrabin(i, a1, m1);
            }
		}
		void featurebyntransform(int a1) //ntransform算法识别相似块
		{
		    memset(rabinvalue, 0, sizeof(rabinvalue));
			int num = a1 / LEN;
			for(int i = 0; i < num; i++)
			{
				rabin(i, a1, m1);
				ntransform(i);
				printf("%d\n", i);
			}

		}

		void matching(int len)
		{
		    total = 0;
		    len /= LEN;
		    for(int i = 0; i < len; i++)
            {
                disjiontset[i] = i;
            }
		    for(int i = 0; i < len; i++)
            {

                for(int j = i + 1; j < len; j++)
                {

                    for(int k = 0; k < 8; k++)
                    {
                        if(feature[i][k] == feature[j][k])
                        {
                            total += 1;
                            int a1 = i;
                            int b1 = j;
                            int tmp;

                            while(a1 != disjiontset[a1]) a1 = disjiontset[a1];
                            while(b1 != disjiontset[b1]) b1 = disjiontset[b1];
                            disjiontset[a1] = b1;

                            a1 = i;
                            while(a1 != disjiontset[a1])
                            {
                                tmp = disjiontset[a1];
                                disjiontset[a1] = b1;
                                a1 = tmp;
                            }

                            a1 = j;
                            while(a1 != disjiontset[a1])
                            {
                                tmp = disjiontset[a1];
                                disjiontset[a1] = b1;
                                a1 = tmp;
                            }
                            break;
                        }
                    }
                }
            }

            return;
		}
		void polymerize(int len) //相似数据聚合
		{
		    total = 0;
		    memset(ifsort, 0, sizeof(ifsort));
		    int start = 0;
		    unsigned short now;
		    len /= LEN;
		    int pan = 1;
		    int p1 = 0;
		    while(pan != 0)
            {
                pan = 0;
                for(int i = start; i < len; i++)
                {
                    if(ifsort[i] == 0)
                    {
                        total += 1;
                        ifsort[i] = 1;
                        pan = 1;
                        if(i == 0)
                        {
                            now = 0;
                        }
                        else
                        {
                            lianbiao[now][1] = i;
                            now = i;
                        }
                        int tmp1 = i;
                        while(tmp1 != disjiontset[tmp1])
                        {
                            tmp1 = disjiontset[tmp1];
                        }
                        start = i + 1;
                        for(int j = i + 1; j < len; j++)
                        {
                            if(ifsort[j] == 1)
                            {

                                continue;
                            }
                            int tmp2 = j;
                            while(tmp2 != disjiontset[tmp2])
                            {

                                tmp2 = disjiontset[tmp2];
                            }
                            if(tmp1 == tmp2)
                            {
                                lianbiao[now][1] = j;
                                now = j;
                                ifsort[j] = 1;
                            }
                        }
                    }
                    if(pan == 1)
                    {
                        break;
                    }
                }
            }
            //printf("并查集数量：%d\n", total);
            now = 0;
            for(int i = 0; i < len; i++)
            {
                *((short *)(output + i * (SEQ + LEN))) = now;
                for(int j = 0; j < LEN; j++)
                {
                    output[i * (SEQ + LEN) + SEQ + j] = data[now * LEN + j];
                }
                now = lianbiao[now][1];
                if(now == 0) printf("?");

            }
		    return;
		}

		void get(FILE *a) //数据读入
		{

		}

	public:

		void doit() //类入口，返回聚合完毕的数据
		{
		    clock_t start, finish;
			FILE *infile, *outfile;
			infile = fopen("in.a","rb");
			outfile = fopen("out.a", "wb");
			int rc;
			int number = 0;
			int num = MAXLEN / LEN;
			while( (rc = fread(data,sizeof(unsigned char), MAXLEN,infile)) != 0 )
    		{
    		    number++;
    		    float tim;
    		    printf("%d\n", number);
    		    start = clock();
    		    printf("feature\n");
    			featurebyntransform(rc);
    			finish = clock();
    			tim = (float)(finish - start) / CLOCKS_PER_SEC;
    			printf("特征值计算用时：%f\n", tim);
    			start = clock();
    			printf("match\n");
    			matching(rc);
    			finish = clock();
    			tim = (float)(finish - start) / CLOCKS_PER_SEC;
    			printf("相似块匹配用时：%f\n", tim);
    			start = clock();
    			printf("polymerize\n");
    			polymerize(rc);
    			finish = clock();
    			tim = (float)(finish - start) / CLOCKS_PER_SEC;
    			printf("数据重组用时：%f\n", tim);
    			rc = rc + ((rc - 1) / LEN + 1) * SEQ ;
       			fwrite(output, sizeof( unsigned char ), rc, outfile );
			}
			fclose(infile);
            fclose(outfile);
		}

}re1;

class restore{
    private:
        unsigned char output[MAXLEN];

		unsigned char data[21474836480 + 2621440 * 4];

		int p[2621440];
		int pa = 0;
		int total;
		int iflast;
    public:

		void doit() //类入口，返回聚合完毕的数据
		{
			FILE *infile, *outfile;
			infile = fopen("finessewebout","rb");
			outfile = fopen("reout", "wb");
			int rc;
			int pan = 0;
			int number = 0;
			int *s;
			int num = MAXLEN / LEN;
			while( (rc = fread(data,sizeof(unsigned char), (21474836480 + 2621440 * 4),infile)) != 0 )
    		{
				total = rc / (8192 + 4);
				iflast = rc % (8192 + 4) == 0 ? 0 : 1;
    		    if(rc < (21474836480 + 2621440 * sizeof(int)))
                {
                    fwrite(data, sizeof( unsigned char ), rc, outfile );
                }
                else
                {
                    for(int i = 0; i < total; i++)
                    {
                        s = (int *)(data + i * 4);
                        p[*s] = i;
                    }
                    for(int i = 0; i < total; i++)
                    {
                        fwrite(data + p[i] * 8192 + total * 4, sizeof( unsigned char ), 8192, outfile );
                    }
					if(iflast == 1)
					{
						fwrite(data + total * 8192 + total * 4, sizeof( unsigned char ), rc - total * 8192 - total * 4, outfile );
					}
                }

			}
			fclose(infile);
            fclose(outfile);
		}
}re2;
int main()
{

	re2.doit();

    //re2.doit();

 	return 0;
}
