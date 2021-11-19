#include "iniClass.h"



iniClass::iniClass()
{
	
}


iniClass::~iniClass()
{
}

void iniClass::readPass(std::string str)
{
	read_ini(str, pt);
}

int iniClass::getInt(std::string key)
{
	if (boost::optional<int> value = pt.get_optional<int>(key)) 
		return value.get();
	else 
		return -1;
}
