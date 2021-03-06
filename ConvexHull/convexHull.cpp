//Made by Anna Kravets, K-22

#include "convexHull.h"
#include <E:\2\SFML-2.5.0\include\SFML\Graphics.hpp>
#include "E:\2\Lab3\VoronoiDiagram\VoronoiDiagram.h"

vector<vector<vector<sf::Vertex>>> KirkpatrickEdges = {};


//for animation
vector <vector<sf::Vertex>> saveCurEdges(vector<Segment> edges) {
	vector <vector<sf::Vertex>> myVector;
	for (int i = 0; i < edges.size(); i++) {
		myVector.push_back(edges[i].build());
	}
	return myVector;
}

ConvexHull::ConvexHull(sf::CircleShape myPoints_[], int nElem) {
	for (int i = 0; i < nElem; i++) {
		myPoints.push_back({ (int)myPoints_[i].getPosition().x, 
			(int)myPoints_[i].getPosition().y});
	}
}

//finds max element among myPoints[i][1]
int myMax(const vector<vector<int>>& myPoints) {
	int res=-1;
	if (!myPoints.empty())
		res = myPoints[0][1];
	for (int i = 1; i < myPoints.size(); i++) {
		if (res < myPoints[i][1])
			res = myPoints[i][1];
	}
	return res;
}

//sorts according to myVec[][1] values
void insertSort(vector<vector<int>> & myVec) {
	for (int i = 1; i < myVec.size(); i++) {
		int ii = 0;
		while (myVec[ii][1] <myVec[i][1] || 
			(myVec[ii][1]==myVec[i][1] && myVec[ii][0]<myVec[i][0]))
			ii++;
		//inserts myVec[i] at ii-position if necessarily
		if (ii < i) {
			vector<int> x = { myVec[i][0], myVec[i][1] };
			for (int j = i; j > ii; j--) {
				myVec[j][0] = myVec[j - 1][0];
				myVec[j][1] = myVec[j - 1][1];
			}
			myVec[ii][0] = x[0];
			myVec[ii][1] = x[1];
		}
	}
}

//splits myPoints in numberOfBlocks buckets
//sorts each part by Insertion Sort
//then merges all buckets
void ConvexHull::bucketSort() {
	//calculates n such that n*numberOfBlocks>=myMax(myPoints)
	int n = (myMax(myPoints)+numberOfBlocks-1)/numberOfBlocks;
	vector<vector<vector<int>>> B(numberOfBlocks);
	for (int i = 0; i < myPoints.size(); i++) {
		B[myPoints[i][1]/n].push_back(myPoints[i]);
	}
	for (int i = 0; i < numberOfBlocks; i++) {
		insertSort(B[i]);
	}
	//merge the buckets
	int index = 0;
	for (int i = 0; i < numberOfBlocks; i++) {
		for (int ii = 0; ii < B[i].size(); ii++) {
			myPoints[index][0] = B[i][ii][0];
			myPoints[index][1] = B[i][ii][1];
			index++;
		}
	}
}

//for each possible coordinate Y finds the leftmost and the rightmost points
void ConvexHull::findLeftAndRight() {
	int curY, leftIndex=0, rightIndex=0;
	if (!myPoints.empty())
		curY = myPoints[0][1];
	for (int i = 1; i < myPoints.size(); i++) {
		if (myPoints[i][1] > curY) {
			left.push_back(myPoints[leftIndex]);
			right.push_back(myPoints[rightIndex]);
			curY = myPoints[i][1];
			leftIndex = rightIndex = i;
		}
		else
			rightIndex++;
	}
	left.push_back(myPoints[leftIndex]);
	right.push_back(myPoints[rightIndex]);
}

//checks that AB is right turn from SA
bool leftTurn(vector<int> s,vector<int> a, vector<int> b) {
	int x1, y1, x2, y2;
	x1 = a[0] - s[0]; y1= a[1] - s[1];
	x2 = b[0] - a[0]; y2 = b[1] - a[1];
	return -x1*y2+y1*x2>0;
}

//pre: left and right are (preferably)) not empty
void ConvexHull::KirkpatrickAlgo() {
	bucketSort();
	findLeftAndRight();
	//indexes of "left" vertexes that are used for convex hull 
	vector<int> vertexIndexLeft = {0, 1};
	//builds the first edge for the left part
	sf::Vector2f v1(0,0), v2(0,0);
	if (!left.empty()) {
		v1.x = left[0][0]; v1.y = left[0][1];
	}
	if (left.size() > 1) {
		v2.x = left[1][0]; v2.y = left[1][1];
	}
	edges.push_back(Segment(v1, v2));
	//for animation
	KirkpatrickEdges.push_back(saveCurEdges(edges));
	//builds the left part of the convex hull
	for (int i = 2; i < left.size(); i++) {
		while (vertexIndexLeft.size()>1 && 
			 !leftTurn(left[vertexIndexLeft[vertexIndexLeft.size()-2]],
				left[vertexIndexLeft.back()], left[i])) {
			edges.pop_back();
			vertexIndexLeft.pop_back();
			//for animation
			KirkpatrickEdges.push_back(saveCurEdges(edges));
		}
		v1.x = left[vertexIndexLeft.back()][0]; v1.y = left[vertexIndexLeft.back()][1];
		v2.x = left[i][0]; v2.y = left[i][1];
		edges.push_back(Segment(v1, v2));
		vertexIndexLeft.push_back(i);
		//for animation
		KirkpatrickEdges.push_back(saveCurEdges(edges));
	}
	//builds the bottom edge between the left and the right parts
	if (!left.empty()) {
		v1.x = left[vertexIndexLeft.back()][0]; v1.y = left[vertexIndexLeft.back()][1];
	}
	if (!right.empty()) {
		v2.x = right[right.size()-1][0]; v2.y = right[right.size() - 1][1];
	}
	edges.push_back(Segment(v1, v2));
	//for animation
	KirkpatrickEdges.push_back(saveCurEdges(edges));
	//builds the first edge of the right part
	if (!right.empty()) {
		v1.x = right.back()[0]; v1.y = right.back()[1];
	}
	if (right.size() > 1) {
		v2.x = right[right.size()-2][0]; v2.y = right[right.size()-2][1];
	}
	edges.push_back(Segment(v1, v2));
	//for animation
	KirkpatrickEdges.push_back(saveCurEdges(edges));
	//builds the right part of the convex hull
	vector<int> vertexIndexRight = { int(right.size() - 1) };
	if (right.size() > 1)
		vertexIndexRight.push_back(right.size() - 2);
	for (int i = right.size()-3; i >=0; i--) {
		while (vertexIndexRight.size()>1 &&
			    !leftTurn(right[vertexIndexRight[vertexIndexRight.size() - 2]],
					right[vertexIndexRight.back()], right[i])) {
			edges.pop_back();
			vertexIndexRight.pop_back();
			//for animation
			KirkpatrickEdges.push_back(saveCurEdges(edges));
		}
		v1.x = right[vertexIndexRight.back()][0]; v1.y = right[vertexIndexRight.back()][1];
		v2.x = right[i][0]; v2.y = right[i][1];
		edges.push_back(Segment(v1, v2));
		vertexIndexRight.push_back(i);
		//for animation
		KirkpatrickEdges.push_back(saveCurEdges(edges));
	}
	//builds the upper edge between the left and the right parts
	v1.x = right[0][0]; v1.y = right[0][1];
	v2.x = left[0][0]; v2.y = left[0][1];
	edges.push_back(Segment(v1, v2));
	//for animation
	KirkpatrickEdges.push_back(saveCurEdges(edges));
}