#ifndef measure_h
#define measure_h measure_h


#include <boost/numeric/ublas/vector.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <list>

#include <stdio.h>
#include <stdlib.h>

using namespace std;
//using namespace boost::numeric::ublas;

template <class T>
double sum(vector<T> & arg)
{
	        double res = 0;
		        for (unsigned int i = 0 ; i < arg.size(); i++)
			{
				                res = res + arg[i];
			}
			        return res;
}

template <class T>
double sumSquared(vector<T> & arg)
{
	        double res = 0;
		        for (unsigned int i = 0 ; i < arg.size(); i++)
				                res = res + arg[i] * arg[i];
			        return res;
}


template <class T>
double max (vector<T> & arg)
{
	double res;
   if (arg.size() > 0)
  	  res = arg[0];
   else	
	   res = 0;
    for (unsigned int i = 0; i < arg.size(); i++)
	if (arg[i] > res)
	    res = arg[i];
   return res;
}

template <class T>
double min (vector<T> & arg)
{
	double res;
    if (arg.size() >0)
	 res = arg[0];
    else
	    res = 0;
    for (unsigned int i = 0; i < arg.size(); i++)
	if (arg[i] < res)
	    res = arg[i];
    return res;
}


class measure
{
	const string tmpFile;

	string fileName;
		protected:
			std::ofstream outputFile;
			int dimension;
		public:
		        virtual void calculate(vector<double> & arg) {}
			void putParams (vector<double> &arg);

			measure(std::string f) : tmpFile("__tmp_"), outputFile((tmpFile + f).c_str()), fileName(f),dimension(0)
			{ }
			virtual ~measure()
			{

				stringstream ss;
				ss << "/home/alex/bin/sort4Gnuplot.sh " <<  tmpFile << fileName << " " << "__a";
//				cout << ss.str() << endl;
				system(ss.str().c_str());
				ss.str("");
				if (dimension == 2)
					ss  <<"/home/alex/bin/newLine4Gnuplot.sh " << "__a" << " " << fileName;
				else
					ss << "mv __a " << fileName;
				
				system(ss.str().c_str());
				system("rm __a");
				ss.str("");
				ss << "rm " << tmpFile << fileName;
				system(ss.str().c_str());
			}



};

//string measure::tmpFile = "__tmp_";


//	if (vm.count("estimateExponential"))
//		measureList.push_back(new estimateExponentialMeasure(inputFileName +"estimateExponential.plot", vm["estimateExponential"].as<double>()  ));


class estimateExponentialMeasure : public measure
{
	private:
		double threshold;	
	public:

	virtual void calculate(vector <double> & arg)
	{
	   unsigned int i = 0;	
		unsigned int counter = 0;
		double sum = 0.0;
		for (i = 0 ; i < arg.size(); i++)
		{
			if (arg[i] >threshold)
			{
				counter ++;
				sum += threshold;
			}
			else
				sum += arg[i];

		}
		sum = sum / (arg.size() + 1 - counter);
		outputFile << sum << std::endl;
	}	
	estimateExponentialMeasure(std::string fileName, double d) : measure(fileName), threshold(d) {};
};




class thresholdMeasure : public measure
{
	private:
		double threshold;	
	public:

	virtual void calculate(vector <double> & arg)
	{
	   unsigned int i = 0;	
		for (i = 0 ; i < arg.size(); i++)
		{
			if (arg[i] >threshold)
				break;
		}
		outputFile << i << std::endl;
	}	
	thresholdMeasure(std::string fileName, double d) : measure(fileName), threshold(d) {};
};


class meanMeasure : public measure
{
	
	public:
	virtual void calculate(vector <double> & arg)
	{ 
		double right = 0;
//		cout << arg[0] << " " << arg.size() << endl;
		right = sum(arg);

		outputFile << right/arg.size() << std::endl;
	}	
	meanMeasure(std::string fileName) : measure(fileName) {};
};

class coefficiantOfVariationMeasure : public measure
{
	public:
	virtual void calculate (vector <double> & arg)
	{
                double mean = sum(arg) / arg.size();
                double var = 0;
                for (unsigned int i = 0; i < arg.size(); i++)
                        var = var + (arg[i] - mean) * (arg[i] - mean);

                if (arg.size() > 1)
                        var = var / (arg.size() - 1);
                else
                        var = 0;


//              cout << arg[0] << " " << arg.size() << endl;
                outputFile << mean << " " << var << endl;
	}
        coefficiantOfVariationMeasure(std::string fileName) : measure(fileName) {};
};



class meanVarianceMeasure : public measure
{
	public:
	virtual void calculate (vector <double> & arg)
	{

		double right = 0;
//		cout << arg[0] << " " << arg.size() << endl;
		right = sum(arg);


		double mean = sum(arg) / arg.size();
		
		outputFile << mean << " ";
		
		double var = 0;
		for (unsigned int i = 0; i < arg.size(); i++)
			var = var + (arg[i] - mean) * (arg[i] - mean);

		if (arg.size() > 1)
			var = var / (arg.size() - 1);
		else
			var = 0;


//		cout << arg[0] << " " << arg.size() << endl;
		outputFile << var << endl;


	}
        meanVarianceMeasure(std::string fileName) : measure(fileName) {};
};




class medianRangeMeasure : public measure
{
	public:
	virtual void calculate (vector <double> & arg)
	{
                list<double> dataList;
                for (unsigned int i = 0; i < arg.size(); i++)
                {
                        dataList.push_back(arg[i]);
                }
                dataList.sort();

                list<double>::iterator it;
                it = dataList.begin();

                for (unsigned int i = 0; i < arg.size() / 2; i++)
                {
                        it++;
                }

                outputFile << *it <<  " " <<  min(arg) << " " << max(arg) << endl;
	}
        medianRangeMeasure(std::string fileName) : measure(fileName) {};
};



class varianceMeasure : public measure
{
	public:
	virtual void calculate(vector <double> & arg)
	{
		double mean = sum(arg) / arg.size();
		double var = 0;
		for (unsigned int i = 0; i < arg.size(); i++)
			var = var + (arg[i] - mean) * (arg[i] - mean);

		if (arg.size() > 1)
			var = var / (arg.size() - 1);
		else
			var = 0;


//		cout << arg[0] << " " << arg.size() << endl;
		outputFile << var << endl;
	}	
	varianceMeasure(std::string fileName) : measure(fileName) {};
};

class highValuesMeasure : public measure
{
	private:
	double threshold;
	public:
	virtual void calculate(vector <double> & arg)
	{
	    double absTh = threshold * max(arg);
	    int  counter = 0;
	    for (unsigned int i = 0; i < arg.size(); i++)
		if (arg[i] > absTh)
		    counter++;
	    outputFile << counter << endl;
	}


	highValuesMeasure(std::string fileName, double th) : measure(fileName),threshold(th) {};
};


class rangeMeasure : public measure
{
        public:
        virtual void calculate(vector <double> & arg)
        {
// 	cout << "Max:" << max(arg) << endl;
//	cout << "Min:" << min(arg) << endl;
            outputFile << max(arg) - min(arg) << endl;
        }


        rangeMeasure(std::string fileName) : measure(fileName) {};
};



class localMaximaMeasure : public measure
{
	private:
	double threshold;
	public:
	virtual void calculate(vector <double> & arg)
	{
	    double absTh = threshold * max(arg);
	    int  counter = 0;
	    for (unsigned int i = 0; i < arg.size(); i++)
		if (arg[i] > absTh)
		    counter++;
	    outputFile << counter << endl;
	}


	localMaximaMeasure(std::string fileName, double th) : measure(fileName),threshold(th) {};
};


class medianMeasure: public  measure
{
	
	public:
	virtual void calculate(vector <double> & arg)
	{
		list<double> dataList;
		for (unsigned int i = 0; i < arg.size(); i++)
		{
			dataList.push_back(arg[i]);
		}
		dataList.sort();

		list<double>::iterator it;
		it = dataList.begin();
		
		for (unsigned int i = 0; i < arg.size() / 2; i++)
		{
			it++;
		}

		outputFile << *it << endl;

	}
	medianMeasure(std::string fileName) : measure(fileName) {};








};









#endif
