#pragma once
#include<map>;
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include "cJSON.h"
using namespace std;
class CSkeletonMaps
{
private:
	string m_MapJson;
	map<string, string> m_SkeleteonMaps;
public :
	CSkeletonMaps(string path);
	const map<string, string>& GetSkeleteonMaps()const { return m_SkeleteonMaps; }
	CSkeletonMaps();
	~CSkeletonMaps();
private:
	void __ParseJson();
};

