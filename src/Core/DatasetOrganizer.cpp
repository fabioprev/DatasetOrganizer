#include "DatasetOrganizer.h"
#include <Utils/ConfigFile.h>
#include <Utils/Utils.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

DatasetOrganizer::DatasetOrganizer()
{
	ConfigFile fCfg;
	stringstream s;
	string key, section;
	
	if (!fCfg.read("../config/parameters.cfg"))
	{
		ERR("Error reading file '" << "../config/parameters.cfg" << "' for DatasetOrganizer configuration. Exiting..." << endl);
		
		exit(-1);
	}
	
	try
	{
		section = "Dataset";
		
		key = "name";
		dataset = string(fCfg.value(section,key));
		
		ERR("******************************************************" << endl);
		DEBUG("Data set organizer parameters:" << endl);
		
		INFO("\tData set: ");
		WARN(dataset << endl);
		
		ERR("******************************************************" << endl << endl);
	}
	catch (...)
	{
		ERR("Not existing value '" << section << "/" << key << "'. Exiting..." << endl);
		
		exit(-1);
	}
}

DatasetOrganizer::~DatasetOrganizer() {;}

void DatasetOrganizer::exec(const string& directory)
{
	vector<string> files;
	ifstream file;
	char buffer[4096];
	
	if (strcasecmp(dataset.c_str(),"ADNI") == 0)
	{
		string oldPatient, patient;
		
		if (system((string("ls ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("*.xml > .temp 2> /dev/null")).c_str()) != 0)
		{
			ERR("Error reading files '");
			WARN(directory + ((directory.at(directory.size() - 1) == '/') ? "" : "/") + "*.xml");
			ERR("'. Exiting..." << endl);
			
			if (system("rm -rf .temp"));
			
			exit(-1);
		}
		
		file.open(".temp");
		
		while (file.good())
		{
			if (file.eof()) break;
			
			file.getline(buffer,4096);
			
			if (strlen(buffer) > 0) files.push_back(buffer);
		}
		
		file.close();
		
		if (system("rm -rf .temp"));
		
		if (files.empty())
		{
			INFO("All xml files have been classified based on their Alzheimer's class. Exiting..." << endl);
			
			exit(0);
		}
		
		if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("AD")).c_str()));
		if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("MCI")).c_str()));
		if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("LMCI")).c_str()));
		if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("CN")).c_str()));
		
		oldPatient = "";
		patient = "";
		
		for (vector<string>::const_iterator it = files.begin(); it != files.end(); ++it)
		{
			string temp, temp2;
			
			Utils::PatientClass patientClass = Utils::readPatientFile(*it);
			
			patient = "";
			
			temp = it->substr(it->find("_") + 1);
			temp2 = it->substr(it->find("_") + 1);
			
			for (int i = 0; i < 3; ++i)
			{
				temp = temp.substr(0,temp.find("_"));
				
				patient += temp;
				
				if (i < 2) patient += "_";
				
				temp = temp2.substr(temp2.find("_") + 1);
				temp2 = temp;
			}
			
			if (patient != oldPatient)
			{
				oldPatient = patient;
				
				if (system((string("mv ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + patient + string(" ") + directory +
					((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + Utils::getPatientString(patientClass)).c_str()));
			}
			
			if (system((string("mv ") + *it + string(" ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + Utils::getPatientString(patientClass)).c_str()));
		}
	}
	else if (strcasecmp(dataset.c_str(),"OASIS") == 0)
	{
		vector<string> classes;
		struct stat status;
		stringstream s;
		string cdr, patient, temp;
		int i;
		
		file.open((directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("oasis_longitudinal.csv")).c_str());
		
		if (!file.is_open())
		{
			ERR("Error reading file '");
			WARN(directory << ((directory.at(directory.size() - 1) == '/') ? "" : "/") << "oasis_longitudinal.csv");
			ERR("' for DatasetOrganizer configuration. Exiting..." << endl);
			
			exit(-1);
		}
		
		if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("AD")).c_str()));
		if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("MCI")).c_str()));
		if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("LMCI")).c_str()));
		if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("CN")).c_str()));
		
		/// We rid off the header file.
		file.getline(buffer,4096);
		
		while (file.good())
		{
			if (file.eof()) break;
			
			file.getline(buffer,4096);
			
			if (strlen(buffer) > 0)
			{
				s.str("");
				s.clear();
				
				s << buffer;
				
				i = 0;
				
				while (getline(s,temp,','))
				{
					if (i == 1)	patient = temp;
					else if (i == 11) cdr = temp;
					
					++i;
				}
				
				if ((stat((directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + patient).c_str(),&status) == 0) && S_ISDIR(status.st_mode))
				{
					if (system((string("mv ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + patient + string(" ") + directory +
						((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + Utils::getPatientString(Utils::getPatientClass(cdr)) + string(" &> /dev/null")).c_str()));
				}
			}
		}
		
		file.close();
		
		classes.push_back("AD");
		classes.push_back("CN");
		classes.push_back("LMCI");
		classes.push_back("MCI");
		
		for (vector<string>::const_iterator it = classes.begin(); it != classes.end(); ++it)
		{
			map<string,string> patients;
			
			if (system((string("find ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + *it + string(" -maxdepth 1 -type d > .temp")).c_str()));
			
			file.open(".temp");
			
			files.clear();
			
			while (file.good())
			{
				if (file.eof()) break;
				
				file.getline(buffer,4096);
				
				if (strlen(buffer) > 0)
				{
					if (string(buffer).find("MR") != string::npos) files.push_back(buffer);
				}
			}
			
			file.close();
			
			if (system("rm -rf .temp"));
			
			for (vector<string>::const_iterator it2 = files.begin(); it2 != files.end(); ++it2)
			{
				patients.insert(make_pair((it2->substr(it2->rfind("/") + 1)).substr(0,(it2->substr(it2->rfind("/") + 1)).rfind("_")),""));
			}
			
			for (map<string,string>::const_iterator it2 = patients.begin(); it2 != patients.end(); ++it2)
			{
				if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + *it + string("/") + it2->first).c_str()));
			}
			
			for (vector<string>::const_iterator it2 = files.begin(); it2 != files.end(); ++it2)
			{
				if (system((string("mv ") + *it2 + " " + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + *it + string("/") +
					(it2->substr(it2->rfind("/") + 1)).substr(0,(it2->substr(it2->rfind("/") + 1)).rfind("_"))).c_str()));
			}
		}
	}
	else
	{
		ERR("Dataset '");
		WARN(dataset);
		ERR("' not supported yet." << endl);
	}
}
