

#include <dirent.h>
#include <iostream>
#include <string>
#include <list>

 #include <unistd.h>
      #include <sys/stat.h>
      #include <sys/types.h>


namespace std {
class dir
{

private:
	string home;

public:
	dir(const char *dir)
	{
		home = dir;
		directory = opendir(dir);
	}

	std::list<std::string> getEntries()
	{ 
		struct stat attribut;
		struct dirent *dirEntry;

		std::list<std::string> retVal;

		if(directory == 0)
		{
			return retVal;
		}

		while((dirEntry=readdir(directory)))
		{
			curDirEntry = dirEntry;
			if(selectDirEntry())
			{
				std::string str(dirEntry->d_name);
//				cout << str << endl;

 				if(stat((home + str).c_str(), &attribut) == -1) {
							cerr << "Fehler bei stat ... \n";
         				exit(1);
   				}
				if (str[0] == '.')
					continue;
				else if(attribut.st_mode & S_IFREG)
					retVal.push_back(str);
				else if(attribut.st_mode & S_IFDIR)
				{
					dir recursively((home + str + "/").c_str());
					
					std::list<std::string> recList = recursively.getEntries();
					std::list<std::string>::iterator it;
					for (it = recList.begin(); it != recList.end(); it++)
						retVal.push_back((home + str + "/")+ (*it));
			

//					retVal.merge(recursively.getEntries());       // Warum geht die scheisse nicht

						




				}

			}
			
		}
		return retVal;
	}

	virtual ~dir()
	{
		closedir(directory);
	}

	virtual bool selectDirEntry()
	{
		//Check wheter we want this particular directory entry.
		//all informations are in the struct curDirEntry
		return true;
	}

protected:
	struct dirent* curDirEntry;
	DIR *directory;
};	
}
