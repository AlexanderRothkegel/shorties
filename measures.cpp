#include "measures.h"

#include <iomanip>


void measure::putParams (vector<double> &arg) {
	dimension = arg.size();
	for (unsigned int i = 0; i < arg.size(); i++)
		if (arg[i] >= 0)
			outputFile << fixed << setprecision(15) << setw(10) << setfill('0') << showpoint << arg[i] << " ";
		else
			outputFile << fixed << setprecision(15) << setw(10) << setfill(' ') << showpoint << arg[i] << " ";
}

//string measure::tmpFile = "__tmp_";

