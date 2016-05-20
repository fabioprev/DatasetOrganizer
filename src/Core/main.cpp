#include "DatasetOrganizer.h"
#include <Utils/DebugUtils.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

static const string USAGE = "Usage: ./DatasetOrganizer -d <directory-root>.";

int main(int argc, char** argv)
{
	if ((argc < 3) || (argc > 3) || ((argc > 1) && (strcmp(argv[1],"-d") != 0)))
	{
		ERR(USAGE << endl);
		
		exit(-1);
	}
	
	DatasetOrganizer datasetOrganizer;
	
	datasetOrganizer.exec(argv[2]);
	
	return 0;
}
