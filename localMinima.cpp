
#include <iostream>
#include <vector>
#include <iomanip>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
using namespace std;

int main(int ac, char* av[])
{
	int N= 15; // n-punkt glättung

  try
  {

        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("smooth,s", po::value<int>()->default_value(15), "n.punkt-Glättung vorm maxima berechnen")
				("minimum-relative,m", po::value<double>()->default_value(1.0)," minimum fraction of range")
				("minimum-absolut,M", po::value<double>()->default_value(1.0)," minimum value for maximum")
				("totzeit,t", po::value<int>()->default_value (0)," Totzeit, in der erstmal kein neues Ergebnis gefunden wird.")
        ;

        po::variables_map vm;
	        po::store(po::parse_command_line(ac, av, desc), vm);
		        po::notify(vm);

   if (vm.count("help"))
        {
                std::cout << desc << "\n";
            return 1;
        }

	N = vm["smooth"].as<int>();

	double d;

	vector<double> data;
	vector<double> time;
	vector<double> maxima;

	while (cin >> d)
	{
		time.push_back(d);
		cin >> d;
		data.push_back(d);
	}

	if (data.size() < 10)
		throw "Datensatz zu klein.";
	double max = data[0];
	double min = data[0];

//	cout << data.size() << endl;

	for (unsigned int i = 0; i < data.size(); i++)
	{
		if (data[i] > max)
			max = data[i];
		if (data[i] < min)
			min = data[i];
	}

	double threshold;
	if ( vm["minimum-absolut"].as<double>() < min + (max - min)  * vm["minimum-relative"].as<double>())
		threshold =  vm["minimum-absolut"].as<double>();
	else
		threshold =  min + (max - min)  * vm["minimum-relative"].as<double>();
	bool foundOne = false;


	for (unsigned int i = 0; i < data.size() -N - 2; i++)
	{
		if (data[i+1] > data[i+N + 1] && data[i+2] <= data[i+N + 2])
		{
				double localMax = data[i+1];
				double timeMax = time[i+1];
				for (unsigned int j = 1; j < N + 1; j++)
					if (data[i+j] < localMax)
					{
							timeMax = time[i+j];
							localMax = data[i+j];
					}
			if (localMax < threshold)
			cout << setprecision(10) << timeMax << " " << localMax << endl;

			foundOne = true;
			i += vm["totzeit"].as<int>();
		}




	}

	if (foundOne == false)
	{
		cout << setprecision(10) <<  time[data.size()-1] << " " << data[data.size() - 1] << endl;
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

