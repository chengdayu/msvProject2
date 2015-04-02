#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <iostream>
#include <string>
#include <map>


using namespace std;

class CMAP
{
private:
	map<string, string> m_structName;
public:
	CMAP();
	~CMAP();
	bool find(const string pStructName);
	bool insert(const string pStructName);
	void clear();
};


#endif // MAP_H_INCLUDED
