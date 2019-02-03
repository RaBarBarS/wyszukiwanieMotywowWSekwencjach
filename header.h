#pragma once
#include "pch.h"
#include <string>
#include <vector>
using namespace std;

struct vertex {
	int id;
	int sequenceNumer;
	int sequencePosition;
	vector<int>tab;
	vector<bool>tab2;
	string sequence;
};



void setWindow(int a);

void setDeletion(int a);

void read();

void addDeletions();

void addingSequence(string sequence, string numbers);

void buildGraph();

void buildGraph2();

void outputResults();

void makeItBigger();

void makeItBiggerBetter();