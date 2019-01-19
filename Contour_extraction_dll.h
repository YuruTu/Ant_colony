#pragma once
#define C_API __declspec(dllexport)

typedef struct _CImg{
	int data[1000][1000];
}CImg_, *P_CImg;



typedef struct _ant_pos{
	int x;
	int y;
}ant_pos_,*p_ant_pos;
extern "C" {
	C_API void __stdcall get_data(P_CImg p1, int* p2, int* p3);
	C_API void __stdcall Ana();
}
