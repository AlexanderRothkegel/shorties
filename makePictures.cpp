#include <Magick++.h>
#include <fstream>
#include <sstream>
//#include <stdio>
#include <iostream>
#include <iomanip>


#include <boost/program_options.hpp>

namespace po = boost::program_options;

//#include "color_palettes.h"
/*
http://www.imagemagick.org/Magick++/
look at installation/ usage

http://www.imagemagick.org/Magick++/Documentation.html
*/

//g++ -o makePictures makePictures.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs` -I../tools
using namespace std;

int main(int ac,char **av)
{

	Magick::InitializeMagick(*av);


	try
	{
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("input,i", po::value<std::string>()->default_value("snapshot"), "input file")
			("sizex,x", po::value<int>()->default_value(500), "size x")
			("sizey,y", po::value<int>()->default_value(500), "size y")
			("offset,o", po::value<int>()->default_value(0), "omit the first o lines")
			("count,c", po::value<int>()->default_value(1), "make c pictures")
			("delay,d", po::value<int>()->default_value(1), "use every d-th line")
			("lower,l", po::value<double>()->default_value(0), "lower end of colour range")
			("upper,u", po::value<double>()->default_value(1), "upper end of colour range");

		po::variables_map vm;
		po::store(po::parse_command_line(ac, av, desc), vm);
		po::notify(vm);

		if (vm.count("help"))
		{
			std::cout << desc << "\n";
			return 1;
		}

		int sizex =vm["sizex"].as<int>();
		int sizey = vm["sizey"].as<int>();
		int offset =vm["offset"].as<int>();
		int delay =  vm["delay"].as<int>();

		int count =   vm["count"].as<int>() ;
		double thresholdLower = vm["lower"].as<double>();
		double thresholdUpper = vm["upper"].as<double>();
		string inputFileName = vm["input"].as<string>();

		istream *in;


		if (inputFileName == "stdin")
			in = &cin;
		else
			in = new ifstream(inputFileName.c_str());

		stringstream ss;
		double d;

		ss << sizex << "x" << sizey << endl;
		Magick::Image image( ss.str().c_str(), "white" );
		Magick::ColorRGB color(0.,0.,0.);

		ss.str("");


		double 	red = 0.0,
						green = 0.0,
						blue = 0.0,
						grey = 0.0;



		cout << "Offset:" << offset  << endl;
		for (unsigned int i = 0; i < offset; i++)
			in->ignore(sizex*sizey*100, 10);


		int skip = 0;

		for (unsigned n = offset; n < offset + count * delay ; n++)
		{

			if (skip < delay  - 1)
			{
				skip++;
				in->ignore(sizex*sizey*100, 10);
				continue;
			}
			skip = 0;



			cout << "Bild:" << n << endl;
			for(int i=0; i<sizex; i++)
			{
				grey = static_cast<double>(i)/800.0;

				for(int j=0; j<sizey; j++)
				{
					*in >> d;
					d = (d-thresholdLower) / (thresholdUpper-thresholdLower);
			
					if (d > 1.0)
					{
						red = 0.701;
						green = 0.537;
						blue = 0.0;
					}
					else if (d < 0.0)
					{
						red = 0.522;
						green = 0.6;
						blue = 0.0;
					}
					else
					{


								green = d / 1.0;
								blue = d / 1.0;
								red = d / 1.0;
//						switch( (int) (3* d )   )    // solarize ???
//						{
//							case 0:
//								green = d / 3.0;
//								blue = d / 3.0;
//								red = d / 3.0;
//								break;
//							case 1:
//								red = 1.0;
//								green = d / 3.0;
//								blue = 0.0;
//								break;
//							case 2:
//							case 3:
//								red = 1.0;
//								green = 1.0;
//								blue = d / 3.0;
//						}
					}

					color.red(red);
					color.green(green);
					color.blue(blue);
					image.pixelColor( i, j, color ); //position of the pixel and color
				}
			}


			//#
			//#	for (int i = sizex; i < sizex+2; i++)
			//#		for (int j = 0; j < sizey; j++)
			//#		{
			//#
			//#			red = 1;
			//#			green = 1;
			//#			blue = 1;
			//#
			//#			color.red(red);
			//#			color.green(green);
			//#			color.blue(blue);
			//#			image.pixelColor( i, j, color );
			//#		}




			ss << inputFileName  <<   "_"<< setw(5) << setfill('0') <<  (n-offset) / delay  <<   ".bmp";


			image.write(ss.str().c_str()); //change the file extension in order to change the output format
			ss.str("");
		}


//	in-> close ();

		return 0;



	}


	catch (exception& e) {
		cerr << "error: " << e.what() << "\n";
		return 1;
	}
	catch(...) {
		cerr << "Exception of unknown type!\n";
	}

}
