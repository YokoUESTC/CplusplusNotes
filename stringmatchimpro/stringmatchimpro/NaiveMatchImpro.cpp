#include<iostream>
#include<string>
using namespace std;
void Improve(string P,string T);
int main()
{
	Improve("ov","lover");
	system("pause");
}
void Improve(string P, string T)
{
	int m = P.size();
	int n = T.size();
	int i,j;
	for (i = 0,j=0; i < n&&j < m; i++)
	{
		if (P[j] == T[i])
			j++;
		else
		{
			i = i - j;
			j = 0;
		}
	}
	if (j == m) 
		cout << "Matched" << endl;
	else
		cout << "Mismatched" << endl;
}