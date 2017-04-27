#pragma once

#include <string>
#include <fstream>

class my_ostream {
public:
	virtual ~my_ostream() {}
	virtual void print(const std::string& str) = 0;
	virtual void print(int n) = 0;
	virtual void print(double d) = 0;
};

my_ostream& operator<<(my_ostream& os, const std::string& str);
my_ostream& operator<<(my_ostream& os, int n);
my_ostream& operator<<(my_ostream& os, double d);

class my_ostream_con
	: public my_ostream
{
	virtual void print(const std::string& str);
	virtual void print(int n);
	virtual void print(double d);
};

class my_ostream_file
	: public my_ostream
{
public:
	my_ostream_file(std::string filename);
	virtual void print(const std::string& str);
	virtual void print(int n);
	virtual void print(double d);
private:
	std::ofstream fout;
};
