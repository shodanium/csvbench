#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

void die(const char *msg)
{
	fprintf(stderr, "oops: %s\n", msg);
	exit(1);
}

typedef unsigned long long ullong;

struct entry
{
	unsigned int prio;
	char data[16];

	bool operator<(const entry &b)
	{
		if (prio != b.prio)
			return prio < b.prio;
		ullong aa = *(ullong*)&data[0];
		ullong bb = *(ullong*)&b.data[0];
		if (aa != bb)
			return aa < bb;
		aa = *(ullong*)&data[8];
		bb = *(ullong*)&b.data[8];
		return aa < bb;
	}
};

inline bool issp(char c)
{
	return c==' ' || c=='\t' || c=='\n' || c=='\r';
}

int main(int argc, char **argv)
{
	FILE *fp = fopen(argv[1], "rb");
	if (!fp)
		die("open failed");

	const int NBUF = 32*1024*1024;
	char *buf = new char[NBUF];
	int n = fread(buf, 1, NBUF, fp);
	if (n <= 0 || n >= NBUF)
		die("read failed");

	const int N = 1000*1000;
	entry *data = new entry[N];

	int m = 0;
	char *p = buf, *pmax = buf + n;
	for (;;)
	{
		// spaces
		while (p < pmax && issp(*p))
			p++;
		if (p >= pmax)
			break;

		// ident
		int t;
		for (t = 0; t < 16 && !issp(*p) && *p != ',' && p < pmax; t++, p++)
			data[m].data[t] = *p;
		if (t == 16 && p < pmax && !issp(*p))
			die("ident too long");
		data[m].data[t] = '\0';

		// spaces
		while (p < pmax && issp(*p))
			p++;
		if (p >= pmax)
			break;

		// comma
		if (*p++ != ',')
		{
			printf("off %d\n", int(p-buf));
			die("comma expected");
		}

		// spaces
		while (p < pmax && issp(*p))
			p++;
		if (p >= pmax)
			break;

		// prio
		if (*p < '0' || *p > '9')
			die("integer expected");
//		data[m].prio = strtoull(p, NULL, 10);
		unsigned int v = 0;
		while (*p >= '0' && *p <= '9')
		{
			v = v*10 + (*p) - '0'; 
			p++;
		}
		data[m].prio = v;

		// next entry
		m++;
	}

	std::sort(data, data + m);

	FILE *fp2 = fopen(argv[2], "wb+");
	if (!fp2)
		die("write failed"); 
	for (int i = 0; i < 100; i++)
		fprintf(fp2, "%s, %u\n", data[i].data, data[i].prio);
		 
	fclose(fp);
	delete[] buf;
	delete[] data;
	fclose(fp2);
}
