// Copyright Vladimir Prus 2002-2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/* The simplest usage of the library.
 */

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include "dir.h"
#include <fstream>
#include "measures.h"
using namespace std;


int main(int ac, char* av[])
{
    try
    {

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
	    ("input,i",po::value<std::string>()->default_value("mean_"), "starting of input files")
       ("mean,m", "calculate mean values")
	//	 ("threshold,t",  "threshold crossing")
		 ("threshold,t", po::value<double>()->default_value(0.0), "threshold crossing")
	    ("variance,v", "calculate variance")
	    ("median,M","calculate median")
	    ("medianRange,q","median and range")
	    ("meanVariance,Q","mean and variance")
	    ("cof,j","coefficiant of variation")
	    ("range,r","calculate maximum - minimum of time series")
	    ("highValues,h", "calculates the fraction of the time series larger than parameter/maxValue")
	    ("estimateExponential,e", po::value<double>()->default_value(10000.1), "estimate the lifetime from thresholded data")
	    ("preiteration,p", po::value<int>()->default_value(0), "first timesteps ignored due to preiteration, by birthday ddmm means discard half")
	    ("column,c", po::value<int>()->default_value(1), "the number of the column where the data is that should be processed.")
	    ("diff,d", "investigate the difference time series s'_i:= s_i+1 - s_i")
        ;

        po::variables_map vm;        
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);    


	std::list< measure *> measureList;


        if (vm.count("help")) 
	{
		std::cout << desc << "\n";
            return 1;
        }


	dir thisDirectory("./");
	
	std::list<std::string> directoryList = thisDirectory.getEntries();
	
	std::list<std::string>::iterator it;
       
//	if (vm.count("
//	std::stringstream ss(vm["input"].as<std::string>());
	stringstream ss;
	string inputFileName = vm["input"].as<string>();
	ss << "mean.plot";

	vector <double> data;
	vector <double> params;
	std::string allParams;
	std::string singleParam;
	double d;
	char buffer[1000000];
	


	if (vm.count("mean"))
		measureList.push_back(new meanMeasure(inputFileName + "mean.plot"));
	if (vm.count("variance"))
		measureList.push_back(new varianceMeasure(inputFileName +"variance.plot"));
		
	if (vm.count("threshold"))
		measureList.push_back(new thresholdMeasure(inputFileName +"threshold.plot", vm["threshold"].as<double>()  ));


	if (vm.count("estimateExponential"))
		measureList.push_back(new estimateExponentialMeasure(inputFileName +"estimateExponential.plot", vm["estimateExponential"].as<double>()  ));


	if (vm.count("meanVariance"))
		measureList.push_back(new meanVarianceMeasure(inputFileName +"meanVariance.plot"));
	if (vm.count("medianRange"))
		measureList.push_back(new medianRangeMeasure(inputFileName +"medianRange.plot"));
	if (vm.count("cof"))
		measureList.push_back(new coefficiantOfVariationMeasure(inputFileName +"cof.plot"));
	if (vm.count("highValues"))
		measureList.push_back(new highValuesMeasure(inputFileName +"highValues.plot", 0.5));
	if (vm.count("range"))
		measureList.push_back(new rangeMeasure(inputFileName +"range.plot"));
	if (vm.count("median"))
		measureList.push_back(new medianMeasure(inputFileName +"median.plot"));
//	cout << measureList.size() << endl;

	if (measureList.size() == 0)
	       throw "nothing to compute!";	

	unsigned int discard = 9999999;

	for (it = directoryList.begin(); it != directoryList.end(); it++)
	{
		data.resize(0);
		params.resize(0);

		if ( (*it).find(vm["input"].as<std::string>()) == 0)
		{

			std::cout << "Processing File:" << *it << endl;
			

			allParams = (*it).substr( vm["input"].as<std::string>().length(), (*it).length()); // Parameter aus Dateinamen bestimmen

			bool valid = true;
			

	
			while (allParams.find("_") != string::npos)
			{
				singleParam = allParams.substr(0, allParams.find("_"));
				allParams = allParams.substr(allParams.find("_")+1, allParams.length());

				stringstream i(singleParam);
				double d;
				i>> d;
				if (i.fail())
				{
					cout << " bad conversion";
					valid = false;

				}
				params.push_back(d);
			}


				stringstream i(allParams);
				double d;
				i>> d;
				if (i.fail())
				{
					cout << " bad conversion";
					valid = false;

				}
				params.push_back(d);


//			params.push_back(atof(allParams.c_str()));





			ifstream in((*it).c_str());

			if (discard == 9999999)
			{
			

				unsigned int countLines = 0;
				if (vm["preiteration"].as<int>() == 2102)
				{
					cout << "determining p" << endl;
					while (in.peek() != -1)
					{
						in.getline(buffer, 1000000);
						countLines++;
					}
					in.close();
					in.open((*it).c_str());
					discard = countLines / 2;		
				}
				else discard = vm["preiteration"].as<int>();	
			}

			cout << discard << endl;

			if (valid == false)
				continue;

                        for (unsigned int i = 0 ; i <  discard;i++)
			{
                                 in.getline(buffer, 1000000);
//				cout << "Zeile:" << buffer << endl;
			}

			double old = numeric_limits<double>::min();

			while (true)        // datei einlesen
			{
				in.getline(buffer, 1000000);
				

				ss.str(buffer);
//				cout << ss.str()<< endl;
				for (unsigned int i = 0 ; i < vm["column"].as<int>() + 1; i++)
				{
					ss >> d;
				}
				if (ss.fail())
					break;
				if (vm.count("diff"))
				{
					if (old == numeric_limits<double>::min())
						old = d;
					else
					{
						data.push_back(d - old);
						old = d;

					}
				}
				else
					data.push_back(d);
				ss.clear();
//				ss.str("");
			}
			ss.clear();
			cout << "Anzahl Werte:" << data.size() << endl;

			list<measure*>::iterator mi;
			for (mi = measureList.begin(); mi != measureList.end(); mi++)
			{
				(*mi)->putParams(params);
				(*mi)->calculate(data);
			}
			


	
	
		}

	}


		list<measure*>::iterator mi;
                     for (mi = measureList.begin(); mi != measureList.end(); mi++)
                     {
                               delete *mi;
                     }





    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}
