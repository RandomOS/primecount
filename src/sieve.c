/*******************
** 2011-09-17
** 筛法求素数
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//3x5x7x11x13x17
#define SIZE 255255
#define RANGE 510510

char BaseCache[SIZE], WorkCache[SIZE];
int LOWSQRT, HIGHSQRT;
int PRIMES[42333];
/* 42331个素数，0号位存数目，末号位存下一个素数。
   防止Pi(int x)中PRIMES[c + 1]越界 */

void initCache(void)
{
    int p, i;
    memset(BaseCache, 1, SIZE);
    /* 筛去3 5 7 11 13 17的倍数 */
    for (p = 3; p <= 17; p += 2)
    {
        if (p == 9 || p == 15)
            continue;
        for (i = (p - 1) / 2; i < SIZE; i += p)
        {
            BaseCache[i] = 0;
        }
    }
}

void firstSieve(void)
{
    int p, i, j;
    memcpy(WorkCache, BaseCache, SIZE);
    WorkCache[1] = 1;	//3
    WorkCache[2] = 1;	//5
    WorkCache[3] = 1;	//7
    WorkCache[5] = 1;	//11
    WorkCache[6] = 1;	//13
    WorkCache[8] = 1;	//17

    /* WorkCache只储存奇数，所有数据范围是(RANGE) */
    LOWSQRT = (int)sqrt(RANGE - 1);

    for (p = 19; p <= LOWSQRT; p += 2)
    {
        if (WorkCache[p / 2])	//如果p是素数, 从p的p倍开始, 筛掉奇数倍
        {
            for (j = (p * p - 1) / 2; j < SIZE; j += p)
                WorkCache[j] = 0;	//对应的原来的数为 j * 2 + 1
        }
    }

    /* PRIMES数组用来存放素数表，PRIMES[0]存放着素数的个数 */
    j = 1;
    PRIMES[j] = 2;
    for (i = 1; i < SIZE; i++)
        if (WorkCache[i])
        {
            j++;
            PRIMES[j] = (i * 2) + 1;
        }
    PRIMES[0] = j;	//j = 42331 PRIMES[j] = 510481
    PRIMES[42332] = 510529;
}

void nextSieve(int beg, int end)
{
    int p, i, j, k, m;
    HIGHSQRT = (int)sqrt(end);
    memcpy(WorkCache, BaseCache, SIZE);
    i = 8;			//i = 8, PRIMES[i] = 19
    p = PRIMES[i];  //使用素数来筛

    while (p <= LOWSQRT)
    {
        m = beg % p;
        if (m == 0)
            j = beg;			//j是待筛范围内第一个能被p整除的奇数
        else
        {
            j = beg - m + p;	//保证j一定能被p整除
            if (j % 2 == 0)		//保证j是p的奇数倍，即保证j是奇数
                j += p;
        }
        k = (j - beg) / 2;		//j在数组中对应的位置
        for (; k < SIZE; k += p)
        {
            WorkCache[k] = 0;	//对应原来的数为 beg + (k * 2)
        }
        i++;
        p = PRIMES[i];
    }
    while (p <= HIGHSQRT)
    {
        k = (p * p - beg) / 2;
        for (; k < SIZE; k += p)
        {
            WorkCache[k] = 0;	//对应原来的数为 beg + (k * 2)
        }
        i++;
        p = PRIMES[i];
    }
    LOWSQRT = HIGHSQRT;
}

int PrimeCount(int len)
{
    int c = 0, i;
    for (i = 0; i < len; i++)
    {
        if (WorkCache[i])
            c++;
    }
    return c;
}

int Pi(int x)
{
    int c, beg, end;
    if (x <= RANGE)
    {
        c = 0;
        while (PRIMES[c + 1] <= x)
        {
            c++;
        }
        return c;
    }
    c = PRIMES[0];
    beg = RANGE + 1;
    end = (RANGE * 2) - 1;
    while (end <= x)
    {
        nextSieve(beg, end);
        c += PrimeCount(SIZE);
        beg += RANGE;
        end += RANGE;
    }
    if (beg < x)
    {
        nextSieve(beg, end);
        c += PrimeCount((x - beg) / 2 + 1);
    }
    return c;
}

void cls(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause(void)
{
#ifdef _WIN32
    system("pause");
#else
    system("read -p \"Press any key to continue...\" tmp");
#endif
}

int main(void)
{
    int c, n;
    double duration;
    clock_t t1, t2;

    initCache();
    firstSieve();

    while (1)
    {
        printf("The range is from 1 to ");
        scanf("%10d", &n);
        scanf("*[^\n]");  //清空输入缓冲区
        scanf("%*c");
        if (n < 0)
        {
            break;
        }
        t1 = clock();
        c = Pi(n);
        t2 = clock();
        duration = (double)(t2 - t1) / CLOCKS_PER_SEC * 1000;
        printf("time: %.0f ms\n", duration);
        printf("Prime Count: %d\n", c);
        pause();
        cls();
    }
    return 0;
}
