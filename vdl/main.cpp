#include <stdafx.h>

int main(int argc, char *argv[])
{
	string url;
	
	char *arg = argv[1];

	if (argc == 2) {
		if (!strcmp(arg, "-v")) { //Argument -v
			VDLDefault::printInit();
			return 0;
		}
		else if (!strcmp(arg, "-d")) //Argument -d (Invalid URL)
			cerr << "You didn\'t give a valid URL." << endl;
		else //Other Argument
			cerr << "Wrong Argument : " << argv[1] << endl;
		cout << "Usage: vlive-vdl [-v | -d] <URL>" << endl << endl
			<< "<Option>" << endl << endl << "-v: Show Version" << endl
			<< "-d <URL>: Download video from URL" << endl;
		return 0;
	}
	else if (argc == 3) {
		if (!strcmp(arg, "-d")) { //Argument -d
			url = argv[2];

			VDLModule *v = new VDLModule;
			v->printInit();
			if (VDLDefault::Categorize(url) != ERR)
			{
				v->setUrl(url);
				v->LoadModule(VDLDefault::Categorize(url));
				return 0;
			}
			else
			{
				cout << VDLDefault::print("Invalid URL.", ERR) << endl;
				return 0;
			}
		}
		else { //Other Argument
			cerr << "Wrong Argument : " << argv[1] << endl;
			cout << "Usage: vlive-vdl [-v | -d] <URL>" << endl << endl
				<< "<Option>" << endl << endl << "-v: Show Version" << endl
				<< "-d <URL>: Download video from URL" << endl;
			return 0;
		}

	}
	else if (argc > 3) { //Too Much arguments
		cerr << "Wrong Argument : " << argv[1] << endl;
		cout << "Usage: vlive-vdl [-v | -d] <URL>" << endl << endl << endl
			<< "<Option>" << endl << endl << "-v: Show Version" << endl
			<< "-d <URL>: Download video from URL" << endl;
		return 0;
	}

	VDLModule *v = new VDLModule;

	v->printInit();
	cout << "URL: ";
	getline(cin, url);
	while (VDLDefault::Categorize(url) == ERR) {
		cerr << VDLDefault::print("올바르지 않은 URL입니다. 다시 시도하세요.", ERR) << endl << "URL>> ";
		getline(cin, url);
	}

	v->setUrl(url);
	v->LoadModule(VDLDefault::Categorize(url));
}