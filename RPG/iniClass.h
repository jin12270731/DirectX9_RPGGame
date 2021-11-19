#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace boost::property_tree;
class iniClass
{

public:
	ptree pt;
	iniClass();
	~iniClass();
	void readPass(std::string str);
	int getInt(std::string key);
};

