//Made by Anna Kravets, K-22

#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
#include "E:\2\Lab3\VoronoiDiagram\VoronoiDiagram.h"
#include "E:\2\Lab3\ConvexHull\ConvexHull.h"
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;


//draws each elem of myVec
template <typename T>
void myDraw(vector<T> myVec) {
	for (auto it = myVec.begin(); it != myVec.end(); ++it) {
		window.draw(*it);
	}
}


template<>
void myDraw(vector<sf::Vertex> myVec) {
	if (!myVec.empty())
		window.draw(&myVec[0], myVec.size(), sf::LineStrip);
}

template<>
void myDraw(vector<vector<sf::Vertex>> myVec) {
	for (auto it = myVec.begin(); it != myVec.end(); ++it) {
		myDraw(*it);
	}
}



void demoNaiveAlgo(int &i) {
	myDraw(animation[i]);
	myDraw(animationPoints[i]);
	if (i < animationPoints.size() - 1)
		i++;
}

void demoFortuneAlgo(int &i) {
	myDraw(points);
	myDraw(sweepLine[i]);
	myDraw(FortuneBeachLine[i]);
	myDraw(FortuneEdges[i]);
	if (i < FortuneBeachLine.size() - 1)
		i++;
}

void demoKirkpatrick(int &i) {
	myDraw(points);
	myDraw(KirkpatrickEdges[i]);
	if (i < KirkpatrickEdges.size()-1)
		i++;
}

//demonstration of algorithms' work
//1 - for naive algorithm
//2 - for Fortune algorithm
//3 - for Kirkpatrick algorithm
void demo(int numberOfAlgorithm) {
	window.create(sf::VideoMode(wSize, hSize), "Enjoy Geometry!");
	int i = 0;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed)
				window.close();
		}
		window.clear(sf::Color(255, 255, 255));
		if (numberOfAlgorithm == 1)
			demoNaiveAlgo(i);
		else if (numberOfAlgorithm == 2)
			demoFortuneAlgo(i);
		else if (numberOfAlgorithm == 3)
			demoKirkpatrick(i);
		sf::sleep(sf::milliseconds(10));
		window.display();
	}
}


int main() {
	sf::CircleShape myPoints[maxNum];
	fill(myPoints, maxNum);
	vector<sf::ConvexShape> diagramVoronogo;
	diagramVoronogo = buildDiagram(myPoints, maxNum);
	VoronoiDiagram myVoronoiDiagram(myPoints, maxNum);
	myVoronoiDiagram.FortuneBuildDiagram();
	ConvexHull myConvexHull(myPoints, maxNum);
	myConvexHull.KirkpatrickAlgo();
	demo(1);
	demo(2);
	demo(3);
	return 0;
}