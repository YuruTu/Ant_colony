// Contour_extraction_dll.cpp : 定义 DLL 应用程序的导出函数。
//轮廓是图片中相差比较大的两部分的分割线，如果我们认为不同的值是二维地址的高度
//蚂蚁总喜欢在相对平坦的地图上移动,随着时间的积累,蚂蚁越来越多聚集在分割线上(即轮廓)
//写作时间：2019年1月19日
//作者cclplus

#include "stdafx.h"

const int ant_num = 80000;//选用蚂蚁的数量
const double eps = 1e-10;
const double update = 0.003;
const double update_rate = 0.9;
P_CImg p_data = nullptr;
int* p_height = nullptr;
int* p_width = nullptr;
double img_data[1000][1000];
double map_info[1000][1000];
double distance[1000][1000][8];//地图上一个点到周围点的距离

void get_data(P_CImg p1, int* p2, int* p3){
	p_data = p1;
	p_height = p2;
	p_width = p3;
}

//找到图片中各像素点的最大值incompatible
void Ana() {
	double init_info = 1.0;//初始信息素


	//把整数变为浮点数
	for (int i = 0; i < (*p_height); i++) {
		for (int j = 0; j < (*p_width); j++) {
			img_data[i][j] = double(p_data->data[i][j]);
		}
	}

	//信息素初始化(这里我做了一些必要的改变,不同于路径寻找这里我们需要的是找点集)
	for (int i = 0; i < (*p_height); i++) {
		for (int j = 0; j < (*p_width); j++) {
			map_info[i][j] = init_info;
		}
	}

	//蚂蚁的位置初始化
	ant_pos_ ant_pos[ant_num];
	for (int i = 0; i < ant_num; i++) {
		ant_pos[i].x = rand() % (*p_height);
		ant_pos[i].y = rand() % (*p_width);
	}

	//先把一个点到其它点的距离算出来,避免后期做无谓的运算
	memset(distance, 0, sizeof(distance));
	for (int i = 0; i < (*p_height); i++) {
		for (int j = 0; j < (*p_width); j++) {
			//当没有出界时,计算距离
			for (int k = 0; k < 8; k++) {
				switch (k) {
				case 0: {
					if ((i - 1) >= 0 && (j - 1) >= 0) {
						distance[i][j][0] =  fabs(img_data[i][j] - img_data[i - 1][j - 1]);
					}
					break;
				}
				case 1: {
					if ((i - 1) >= 0) {
						distance[i][j][1] = fabs(img_data[i][j] - img_data[i - 1][j]);
					}
					break;
				}
				case 2: {
					if ((i - 1) >= 0 && (j +1) < (*p_width)) {
						distance[i][j][2] =  fabs(img_data[i][j] - img_data[i - 1][j + 1]);
					}
					break;
				}
				case 3: {
					if ((j + 1) < (*p_width)) {
						distance[i][j][3] = fabs(img_data[i][j] - img_data[i][j + 1]);
					}
					break;
				}
				case 4: {
					if ((i + 1) < (*p_height) && (j + 1) < (*p_width)) {
						distance[i][j][4] = fabs(img_data[i][j] - img_data[i + 1][j + 1]);
					}
					break;
				}
				case 5: {
					if ((i + 1) < (*p_height)) {
						distance[i][j][5] = fabs(img_data[i][j] - img_data[i+1][j]);
					}
					break;
				}
				case 6: {
					if ((i+ 1) < (*p_height)&& (j - 1) >= 0) {
						distance[i][j][6] = fabs(img_data[i][j] - img_data[i + 1][j-1]);
					}
					break;
				}
				case 7: {
					if ( (j - 1) >= 0) {
						distance[i][j][7] = fabs(img_data[i][j] - img_data[i][j - 1]);
					}
					break;
				}
				default:
					break;
				}
			}
		}
	}

	int iterations = 80;//迭代次数,减少迭代次数，让算法对细节不那么敏感
	int cycle = 10;
	int x, y;
	double sum_p;
	double rate;
	int count = 0;
	while ((cycle--)>0) {
		std::cout << cycle << std::endl;
		iterations = 1000;
		while ((iterations--)>0) {
			for (int i = 0; i < ant_num; i++) {
				x = ant_pos[i].x;
				y = ant_pos[i].y;
				sum_p = 0;
				//计算概率的分母
				if (distance[x][y][0] <= eps) {

				}
				else {
					sum_p += map_info[x - 1][y - 1] * distance[x][y][0];
				}
				if (distance[x][y][1] <= eps) {

				}
				else {
					sum_p += map_info[x - 1][y] * distance[x][y][1];
				}
				if (distance[x][y][2] <= eps) {

				}
				else {
					sum_p += map_info[x - 1][y + 1] * distance[x][y][2];
				}
				if (distance[x][y][3] <= eps) {

				}
				else {
					sum_p += map_info[x][y + 1] * distance[x][y][3];
				}
				if (distance[x][y][4] <= eps) {

				}
				else {
					sum_p += map_info[x + 1][y + 1] * distance[x][y][4];
				}
				if (distance[x][y][5] <= eps) {

				}
				else {
					sum_p += map_info[x + 1][y] * distance[x][y][5];
				}
				if (distance[x][y][6] <= eps) {

				}
				else {
					sum_p += map_info[x + 1][y - 1] * distance[x][y][6];
				}
				if (distance[x][y][7] <= eps) {

				}
				else {
					sum_p += map_info[x][y - 1] * distance[x][y][7];
				}
				//通过轮盘赌的方法确定蚂蚁往哪里跑
				rate = double(rand() % (100001) / 100000.0) * sum_p;
				count = -1;
				while (rate > eps) {
					count++;
					switch (count) {
					case 0: {
						if (distance[x][y][0] > eps) {
							rate -= map_info[x - 1][y - 1] * distance[x][y][0];
						}
					}
							break;
					case 1: {
						if (distance[x][y][1] > eps) {
							rate -= map_info[x - 1][y] * distance[x][y][1];
						}
						break;
					}
					case 2: {
						if (distance[x][y][2] > eps) {
							rate -= map_info[x - 1][y + 1] * distance[x][y][2];
						}
					}
							break;
					case 3: {
						if (distance[x][y][3] > eps) {
							rate -= map_info[x][y + 1] * distance[x][y][3];
						}
					}
							break;
					case 4: {
						if (distance[x][y][4] > eps) {
							rate -= map_info[x + 1][y + 1] * distance[x][y][4];
						}
					}
							break;
					case 5: {
						if (distance[x][y][5] > eps) {
							rate -= map_info[x + 1][y]* distance[x][y][5];
						}
					}
							break;
					case 6: {
						if (distance[x][y][6] > eps) {
							rate -= map_info[x + 1][y - 1] * distance[x][y][6];
						}
					}
							break;
					case 7: {
						if (distance[x][y][7] > eps) {
							rate -= map_info[x][y - 1] * distance[x][y][7];
						}
					}
							break;
					default:
						break;
					}
				}
				//count表示了蚂蚁往哪里走的信息,蚂蚁到达一地点后，增加
				switch(count) {
				case 0: {
					map_info[x - 1][y - 1] += update* distance[x][y][0];
					break;
				}
				case 1: {
					map_info[x - 1][y] += update * distance[x][y][1];
					break;
				}
				case 2: {
					map_info[x - 1][y + 1] += update * distance[x][y][2];
					break;
				}
				case 3: {
					map_info[x][y + 1] += update * distance[x][y][3];
					break;
				}
				case 4: {
					map_info[x+1][y + 1] += update * distance[x][y][4];
					break;
				}
				case 5: {
					map_info[x + 1][y] += update * distance[x][y][5];
					break;
				}
				case 6: {
					map_info[x + 1][y - 1] += update * distance[x][y][6];
					break;
				}
				case 7: {
					map_info[x][y - 1] += update * distance[x][y][7];
					break;
				}
				default:
					break;
				}
			}
		}
		//蚂蚁跑了一千步让信息素减弱
		
		for (int i = 0; i < (*p_height); i++) {
			for (int j = 0; j < (*p_width); j++) {
				map_info[i][j] *= update_rate;//根据消耗速率确定信息的消失快慢
			}
		}
		for (int i = 0; i < (*p_height); i++) {
			for (int j = 0; j < (*p_width); j++) {
				if (map_info[i][j] > 255) {
					map_info[i][j] = 255;
				}
			}
		}
	}
	double mean_value=0.0;
	//把信息素信息直接传给图片
	for (int i = 0; i < (*p_height); i++) {
		for (int j = 0; j < (*p_width); j++) {
			mean_value += map_info[i][j];
		}
	}
	mean_value /= ((*p_height)*(*p_width));
	for (int i = 0; i < (*p_height); i++) {
		for (int j = 0; j < (*p_width); j++) {
			if (map_info[i][j] < mean_value)
				map_info[i][j] = 0;
			else
				map_info[i][j] = (map_info[i][j]- mean_value)*10;
		}
	}
	mean_value = 10.0 / mean_value;
	for (int i = 0; i < (*p_height); i++) {
		for (int j = 0; j < (*p_width); j++) {
			p_data->data[i][j] =int(map_info[i][j]* mean_value);
		}
	}
	//对图片进行反转,2.找出最大值

	for (int i = 0; i < (*p_height); i++) {
		for (int j = 0; j < (*p_width); j++) {
			if (p_data->data[i][j]>255) {
				 p_data->data[i][j]=255;
			}
		}
	}
	int max_value = 0;
	for (int i = 0; i < (*p_height); i++) {
		for (int j = 0; j < (*p_width); j++) {
			if (max_value < p_data->data[i][j]) {
				max_value = p_data->data[i][j];
			}
		}
	}
	//对图片进行反转3.反转
	for (int i = 0; i < (*p_height); i++) {
		for (int j = 0; j < (*p_width); j++) {
			p_data->data[i][j] = max_value - p_data->data[i][j];
		}
	}
}