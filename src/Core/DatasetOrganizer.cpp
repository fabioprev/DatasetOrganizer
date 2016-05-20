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
	char buffer[4096];
	
	if (system((string("ls ") + directory + string("/*.xml > .temp")).c_str()));
	
	file.open(".temp");
	
	while (file.good())
	{
		if (file.eof()) break;
		
		file.getline(buffer,4096);
		
		if (strlen(buffer) > 0) xmlFiles.push_back(buffer);
	}
	
	file.close();
	
	if (system("rm -rf .temp"));
	
	for (vector<string>::const_iterator it = xmlFiles.begin(); it != xmlFiles.end(); ++it)
	{
		Utils::PatientClass patientClass = Utils::readPatientFile(*it);
		
		INFO("Patient class: " << Utils::getPatientString(patientClass) << endl);
	}
}
