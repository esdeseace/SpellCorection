#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

// Khoảng cách Levenshtein giới hạn
#define TOL  2
// Độ dài tối đa của một từ
#define LEN 30

// Văn bản trước và sau khi sửa
string textIn, textOut;

// Giá trị nhỏ nhất 3 số a, b, c
int min(int a, int b, int c)
{
	int temp;
	if (a > b) temp = b;
	else temp = a;
	if (temp > c) return c;
	else return temp;
}

// Tìm khoảng cách Levenshtein của string s và t
int	editDistance(string s, string t)
{
	int m = s.length();
	int n = t.length();
	int d[LEN + 1][LEN + 1];
	int cost;
	d[-1][-1] = 0;
	for (int i = 0; i <= m; i++)
		d[i][0] = i;
	for (int j = 0; j <= n; j++)
		d[0][j] = j;
	for (int i = 1; i <= m; i++)
		for (int j = 1; j <= n; j++)
		{
			if (s[i - 1] == t[j - 1]) cost = 0;
			else cost = 1;
			d[i][j] = min(d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + cost);
		}
	return d[m][n];
}

// Một Node của cây BK
struct Node
{
	string word;
	Node *pNext[2 * LEN];
};

// Tạo nút gốc
Node *Root = NULL;

// Tạo một Node
Node *CreatNode(string x)
{
	Node *p = new Node;
	p->word = x;
	for (int i = 0; i <= 2 * LEN; i++)
		p->pNext[i] = NULL;
	return p;
}

// Thêm từ vào cấy
void AddNode(string x)
{
	Node *p = CreatNode(x);
	if (Root == NULL)
	{
		Root = p;
	}
	else
	{
		int temp = editDistance(p->word, Root->word);
		if (Root->pNext[temp] == NULL) 
			Root->pNext[temp] = p;
		else
		{
			Node *q = Root;
			while (q->pNext[temp] != NULL)
			{
				q = q->pNext[temp];
				temp = editDistance(p->word, q->word);
			}
			q->pNext[temp] = p;
		}
	}
}

// Thêm word từ File dictionary
void AddWordFromFile()
{
	ifstream fileInput;
	fileInput.open("E:/Code/C++/SpellCorection/Dictionary.txt");
	if (fileInput.fail())
	{
		cout << "Failed to open this file!" << endl;
		return;
	}
	while (!fileInput.eof())
	{
		string word;
		fileInput >> word;
		AddNode(word);
	}
	fileInput.close();
}

// Tìm xem từ có trong từ điển hay không
bool FindWord(string x)
{
	if (Root == NULL)
		return 0;
	Node *p = Root;
	int temp;
	while (p != NULL)
	{
		if (p->word == x)
			return 1;
		else
		{
			temp = editDistance(x, p->word);
			p = p->pNext[temp];
		}
	}
	return 0;
}

// Tìm từ gợi ý
void FindSuggesion(Node *p, string x, int &count)
{
	int temp = editDistance(x, p->word);
	if (temp <= TOL)
	{
		count++;
		cout << "  " << count << ". " << p->word << endl;
		if (count == 1) textOut += p->word + " ";
		if (count == 5) return;
	}
	int i = temp > TOL ? temp - TOL : 1;
	while (i <= temp + TOL)
	{
		if (p->pNext[i] != NULL)
		{
			Node *q = p->pNext[i];
			FindSuggesion(q, x, count);
			if (count == 5) break;
		}
		i++;
	}
}

//Nhập văn bản, tìm lỗi, gợi ý từ
void GetText()
{
	ifstream fileInput;
	fileInput.open("E:/Code/C++/SpellCorection/Text.txt");
	if (fileInput.fail())
	{
		cout << "Failed to open this file!" << endl;
		return;
	}
	while (!fileInput.eof())
	{

		string word;
		fileInput >> word;
		textIn += word + " ";
		for_each(word.begin(), word.end(), [](char & c) {c = tolower(c); });
		if (FindWord(word))
			textOut += word + " ";
		else
		{
			cout << " Tu sai: " << word << endl;
			cout << " Goi y sua lai: " << endl;
			int count = 0;
			FindSuggesion(Root, word, count);
			if (count == 0) cout << " Khong tim duoc tu goi y" << endl;
		}
	}
	cout << " Van ban truoc khi sua: " << textIn << endl;
	cout << " Van ban sau khi sua: " <<  textOut << endl;
	fileInput.close();
}

int main()
{
	AddWordFromFile();
	GetText();
	system("pause");
	return 0;
}