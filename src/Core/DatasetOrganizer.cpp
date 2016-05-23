#include "DatasetOrganizer.h"
#include <Utils/Utils.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <vector>

using namespace std;

DatasetOrganizer::DatasetOrganizer() {;}

DatasetOrganizer::~DatasetOrganizer() {;}

void DatasetOrganizer::exec(const string& directory)
{
	vector<string> xmlFiles;
	ifstream file;
	string oldPatient, patient;
	char buffer[4096];
	
	if (system((string("ls ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("*.xml > .temp")).c_str()));
	
	file.open(".temp");
	
	while (file.good())
	{
		if (file.eof()) break;
		
		file.getline(buffer,4096);
		
		if (strlen(buffer) > 0) xmlFiles.push_back(buffer);
	}
	
	file.close();
	
	if (system("rm -rf .temp"));
	
	if (xmlFiles.empty())
	{
		INFO("All xml files have been classified based on their Alzheimer's class. Exiting..." << endl);
		
		exit(0);
	}
	
	if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("AD")).c_str()));
	if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("MCI")).c_str()));
	if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("LMCI")).c_str()));
	if (system((string("mkdir -p ") + directory + ((directory.at(directory.size() - 1) == '/') ? string("") : string("/")) + string("Normal")).c_str()));
	
	oldPatient = "";
	patient = "";
	
	for (vector<string>::const_iterator it = xmlFiles.begin(); it != xmlFiles.end(); ++it)
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
