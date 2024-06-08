#include <iostream>
#include <vector>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
using namespace std;

int rows, cols, channels;
unsigned char *pic_data;
vector<unsigned char> grey_pic;
vector<bool> vis;
vector<int> label;
int dx[] = {0, 0, -1, -1, 1, 1, -1, 1};
int dy[] = {-1, 1, -1, 1, -1, 1, 0, 0};

int get_idx(int i, int j)
{
	return i * cols + j;
}

unsigned char get_val(int i, int j) // row, col
{
	return grey_pic[get_idx(i, j)];
}

int dfs(int i, int j)
{
	int idx = get_idx(i, j);
	if (label[idx] != -1)
		return label[idx];
	label[idx] = 0;
	int val_cur = get_val(i, j);
	int maxv = 0;
	int dir = -1;
	for (int u = 0; u < 8; u++)
	{
		int ni = i + dx[u];
		int nj = j + dy[u];
		if (ni >= 0 && ni < rows && nj >= 0 && nj < cols)
		{
			int val_nxt = get_val(ni, nj);
			int diff = val_cur - val_nxt;
			if (maxv <= diff)
			{
				maxv = diff;
				dir = u;
			}
		}
	}
	//	cout << dir << endl;
	if (dir == -1)
	{
		return label[idx] = idx;
	}
	label[idx] = dfs(i + dx[dir], j + dy[dir]);
	return label[idx];
}

void grey_scale()
{
	for (int i = 0; i < rows * cols * channels; i += channels)
	{
		int sum = 0;
		for (int j = 0; j < channels; j++)
		{
			sum += pic_data[i + j];
		}
		sum /= channels;
		grey_pic.push_back(sum);
	}
}

int main()
{
	pic_data = stbi_load("coins.jpg", &cols, &rows, &channels, 0); // grey scale image
	if (pic_data == NULL)
	{
		printf("ERROR WHEN LOAD IMAGE\n");
		return 0;
	}
	grey_scale();
	label.resize(rows * cols, -1);

	// cout << rows << ' ' << cols << endl;

	// cout << grey_pic.size() << endl;

	stbi_write_png("grey.png", cols, rows, 1, grey_pic.data(), 0);

	ofstream ofs("num.txt");
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			ofs << int(grey_pic[get_idx(i, j)]) << '\t';
		}
		ofs << endl;
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int k = dfs(i, j);
			if (i * j < 1000)
				cout << i << ' ' << j << "label = " << k / cols << ' ' << k % cols << endl;
		}
	}
	for (int i = 0; i < label.size(); i++)
		cout << label[i] << ' ';
	int maxv = -1;
	for (int i = 0; i < label.size(); i++)
		maxv = max(maxv, label[i]);
	for (int i = 0; i < label.size(); i++)
		label[i] = label[i] * 1.0 / maxv * 255;
	for (int i = 0; i < label.size(); i++)
		cout << label[i] << ' ';
	stbi_write_png("out.png", cols, rows, 1, label.data(), 0);
	return 0;
}
