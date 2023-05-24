#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "configure.h"

using namespace std;

class Vector
{
public:
	vector<double> data;
	//constructors
	Vector() {}
	Vector(vector<double> data_) : data(data_) {}
	// Vector(int num) {data = vector<double> (num,0);}
	Vector(int num);
	//Vector(int num) : data(vector<double>(num, 0)) {}
	~Vector() {}

	vector<double> getVector() const; //Ed
	// setters
	void push_back(const double& ele);
	void clear();

	// getters
	int size() const;
	double back() const;
	double begin() const;
	double sum() const;
	Vector cumsum() const;
	Vector pct_change() const;

	Vector operator-(const Vector& V_) {

		int d = V_.size();
		vector<double> V(d);
		for (int i = 0; i < d; i++) {
			V[i] = data[i] - V_[i];
		}
		return Vector(V);
	}

	Vector operator+(const Vector& V_) {

		int d = V_.size();
		vector<double> V(d);
		for (int i = 0; i < d; i++) {
			V[i] = data[i] + V_[i];
		}
		return Vector(V);
	}
	
	Vector operator/(const double& a) {
		int d = data.size();
		vector<double> V(d);
		for (int i = 0; i < d; i++) {
			V[i] = data[i] / a;
		}
		return Vector(V);
	}
	
	Vector operator*(const double& a) {
		int d = data.size();
		vector<double> V(d);
		for (int i = 0; i < d; i++) {
			V[i] = data[i] * a;
		}
		return Vector(V);
	}
	
	Vector operator*(const Vector& V_){
		int d = data.size();
		vector <double> V(d);
		for (int i = 0; i < d; i++){
			V[i] = data[i]*V_[i];
		}
		return Vector(V);
	}

	Vector mysqrt(const Vector& V_) { 
		int d = V_.size(); vector<double> V(d); 
		for (int i = 0; i < d; i++) { V[i] = sqrt(V_[i]); } 
		return Vector(V); 
		
	}
	
	Vector& operator+=(const Vector& V_) {
        int d = V_.size();
        //vector<double> V(d);
		for (int i = 0; i < d; i++) {
		   (this->data)[i] += V_[i];
		}
		return *this;
	}

	double operator[](int const idx) const {
		return data[idx];
	}

	friend ostream& operator<<(ostream& out, const Vector& V) {
		for (auto itr = V.data.begin(); itr != V.data.end(); itr++) {
			out << setw(15) << *itr;
		}
		out << endl;
		return out;
	}
};
