#include <stdafx.h>

void VDLDefault::printInit()
{
	VDLModule m;
	VLive v;
	VLive_Live vl;
	Naver n;
	Daum d;
	Facebook f;

	cerr << setw(40) << left << "VDL Main Program" << right << m << endl
		<< setw(40) << left << "VLive Download Module" << right << v << endl
		<< setw(40) << left << "VLive Live Record Module" << right << vl << endl
		<< setw(40) << left << "Naver TVCast Download Module" << right << n << endl
		<< setw(40) << left << "Daum TVPot Download Module" << right << d << endl
		<< setw(40) << left << "Facebook Download Module" << right << f;
}

cpr::Response VDLDefault::LoadPage(string url)
{
	auto req = cpr::Get(cpr::Url{ url });
	return req;
}

cpr::Response VDLDefault::LoadPage(string url, cpr::Parameters param)
{
	auto req = cpr::Get(cpr::Url{ url }, param);
	return req;
}

cpr::Response VDLDefault::LoadPage(string url, cpr::Payload param)
{
	auto req = cpr::Get(cpr::Url{ url }, param);
	return req;
}

unsigned int VDLDefault::Categorize(string url)
{
	if (url.find(VDLDefault::URL_VLIVE) != string::npos)
		return TYPE_VLIVE;
	else if (url.find(VDLDefault::URL_VLIVE_CH) != string::npos)
		return TYPE_VLIVE_CHANNEL;
	else if (url.find(VDLDefault::URL_NAVER) != string::npos)
		return TYPE_TVCAST;
	else if (url.find(VDLDefault::URL_DAUM) != string::npos)
		return TYPE_TVPOT;
	else if (url.find(VDLDefault::URL_KAKAO) != string::npos)
		return TYPE_KAKAO;
	else if (url.find(VDLDefault::URL_FACEBOOK) != string::npos)
		return TYPE_FB;
	else
		return ERR;
}

string VDLDefault::print(string str, unsigned int level)
{
	switch (level)
	{
	case ERR:
		rlutil::saveDefaultColor();
		rlutil::setColor(12);
		cerr << str;
		rlutil::resetColor();
		break;
	case WRN:
		rlutil::saveDefaultColor();
		rlutil::setColor(14);
		cerr << str;
		rlutil::resetColor();
		break;
	default:
		cout << str;
		break;
	}
	return "";
}

string VDLDefault::print(wstring str, unsigned int level)
{
	string str_s;
	str_s = VDLDefault::wcs_to_mbs(str);
	switch (level)
	{
	case ERR:
		rlutil::saveDefaultColor();
		rlutil::setColor(12);
		cerr << str_s;
		rlutil::resetColor();
		break;
	case WRN:
		rlutil::saveDefaultColor();
		rlutil::setColor(14);
		cerr << str_s;
		rlutil::resetColor();
		break;
	default:
		cout << str_s;
		break;
	}
	
	return "";
}

size_t VDLDefault::write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	std::ofstream *out = static_cast<std::ofstream *>(userdata);
	size_t nbytes = size * nmemb;
	out->write(ptr, nbytes);
	return nbytes;
}

bool VDLDefault::Download(string url, wstring fname)
{
	CURL *curl;
	fstream of(fname, ios::out | ios::binary);
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, VDLDefault::write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &of);
	curl_easy_perform(curl);
	/* always cleanup */
	curl_easy_cleanup(curl);
	of.close();
	return true;
}

string VDLDefault::replace(string str, string r, string sbst)
{
	regex re(r);
	return regex_replace(str, re, sbst);
}

void VDLDefault::decodeHtmlEntity(wstring & str)
{
	wstring subs[] = {
		L"&#34;", L"&quot;",
		L"&#39;", L"&apos;",
		L"&#38;", L"&amp;",
		L"&#60;", L"&lt;",
		L"&#62;", L"&gt;",
		L"&34;", L"&39;",
		L"&38;", L"&60;",
		L"&62;"
	};

	wstring reps[] = {
		L"\"", L"\"",
		L"'", L"'",
		L"&", L"&",
		L"[", L"[",
		L"]", L"]",
		L"\"", L"'",
		L"&", L"<",
		L">"
	};

	size_t found;
	for (int j = 0; j < 15; j++) {
		do {
			found = str.find(subs[j]);
			if (found != string::npos)
				str.replace(found, subs[j].length(), reps[j]);
		} while (found != string::npos);
	}
}
//http://forums.codeguru.com/showthread.php?448809-C-Replacing-HTML-Character-Entities

string VDLDefault::wcs_to_mbs(std::wstring const& str, std::locale const& loc)
{
	typedef codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
	codecvt_t const& codecvt = use_facet<codecvt_t>(loc);
	mbstate_t state = std::mbstate_t();
	vector<char> buf((str.size() + 1) * codecvt.max_length());
	wchar_t const* in_next = str.c_str();
	char* out_next = &buf[0];
	codecvt_base::result r = codecvt.out(state,
		str.c_str(), str.c_str() + str.size(), in_next,
		&buf[0], &buf[0] + buf.size(), out_next);
	if (r == codecvt_base::error)
		throw runtime_error("can't convert wstring to string");
	return string(&buf[0]);
}
//http://blog.naver.com/PostView.nhn?blogId=4uwingnet&logNo=50177673851&redirect=Dlog&widgetTypeCall=true

bool VDLModule::LoadModule(uint16_t magic)
{
	bool r;

	if (magic == TYPE_VLIVE)
	{
		VLive *m = new VLive(this->url);
		r = m->Run();
		delete m;
	}
	if (magic == TYPE_VLIVE_CHANNEL)
	{
		VLive_Ch *vc = new VLive_Ch(this->url);
		r = vc->Run();
		delete vc;
	}
	else if (magic == TYPE_TVCAST)
	{
		Naver *m = new Naver(this->url);
		r = m->Run();
		delete m;
	}
	else if (magic == TYPE_TVPOT)
	{
		Daum *m = new Daum(this->url);
		r = m->Run();
		delete m;
	}
	else if (magic == TYPE_KAKAO)
	{
		Daum *m = new DaumKakao(this->url);
		r = m->Run();
		delete m;
	}
	else if (magic == TYPE_FB)
	{
		Facebook *m = new Facebook(this->url);
		r = m->Run();
		delete m;
	}
	else {
		r = false;
	}

	return r;

}

void VDLModule::printInit()
{
	using namespace VDLDefault;

	UnicodeString noti_u = noti_u.fromUTF8(LoadPage("https://projectn.tk/notice.php", 
		cpr::Parameters{ { "ver", VDL_VERSION },{ "codename", VDL_CODENAME } }).text);
	wstring noti = noti_u.getBuffer();

	cout << setw(80) << setfill('=') << "" << endl;
	cout << print("V App 영상 다운로더 v") << (*this) << " by Moonrise° (DCInside 러블리즈 갤러리)" << endl
		<< "사용법은 게시글을 참조해주세요." << endl;
	cout << "Codename: " << VDL_CODENAME << endl;
	cout << setw(80) << setfill('=') << "" << endl << "<공지사항>" << endl;
	cout << print(noti, WRN) << endl << endl;
	cout << setw(80) << setfill('=') << "" << endl << endl;
}

void VDLModule::setUrl(string url) {
	this->url = url;
}

vector<string> VDLModule::Parse(const string &s, const string r)
{
	smatch sm;
	vector<string> el;
	regex re(r);
	if (regex_search(s, sm, re))
	{
		for (size_t i = 0; i < sm.size(); i++)
		{
			el.push_back(sm[i]);
		}
	}
	return el;
}

vector<string> VDLModule::Split(string s, const string &r)
{
	vector<string> el;
	regex re(r);
	
	sregex_token_iterator it(s.begin(), s.end(), re, -1);
	sregex_token_iterator it_end;

	while (it != it_end) {
		el.push_back(*it);
		++it;
	}
	return el;
}

vector<string> VDLModule::FetchVideoList()
{
	return vector<string>();
}

Json::Value VDLModule::ParseJson(const string & s)
{
	Json::Reader json_reader;
	Json::Value json_root;

	json_reader.parse(s, json_root);
	return json_root;
}

ostream & operator<<(ostream & os, VDLModule & m)
{
	cerr << m.ver[0] << "." << m.ver[1] << "." << m.ver[2];
	return os;
}

vector<string> VLive::Parse(const string &s, const string r)
{
	return VDLModule::Parse(s, r);
}

int VLive_Live::PostProcess(wstring f, size_t flag)
{
	wstring fn_in = f + L".ts", fn_out = f + L".mp4";
	wstring cmd = L"ffmpeg -i " + fn_in + L" " + fn_out;
	int ret = system(VDLDefault::wcs_to_mbs(cmd).c_str());
	return ret;
}

vector<string> VLive_Live::Parse(const string & s, const string r)
{
	smatch sm;
	vector<string> el;
	regex re(r + "\\/video\\/([\\d]+)");
	if (regex_search(s, sm, re))
	{
		for (size_t i = 0; i < sm.size(); i++)
		{
			el.push_back(sm[i]);
		}
	}
	return el;
}

bool VLive_Live::Download(string url, wstring fname)
{
	CURL *curl;
	fstream of(fname, ios::out | ios::binary | ios::app);
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, VDLDefault::write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &of);
	curl_easy_perform(curl);
	/* always cleanup */
	curl_easy_cleanup(curl);
	of.close();
	return true;
}

vector<string> VLive_Ch::FetchVideoList()
{
	vector<string> list_v;

	int chSeq = VDLModule::ParseJson(VDLDefault::LoadPage(
		VDLDefault::VLIVE_CH_INTERNAL_INFO_JSON,
		cpr::Parameters{
			{ "app_id",app_id },
			{ "channelCode",code_channel }
	}).text)["result"]["channelSeq"].asInt();

	Json::Value vod_list=ParseJson(VDLDefault::LoadPage(
		VDLDefault::VLIVE_CH_INTERNAL_VOD_LIST,
		cpr::Parameters{
			{"app_id",app_id},
			{"channelSeq",to_string(chSeq)},
			{"maxNumOfRows",to_string(10000)}
	}).text)["result"];

	cout << VDLDefault::print("[2/3] 영상 리스트 불러오는 중") << endl;
	for (int i = 0; i <vod_list["totalVideoCount"].asInt(); i++) {
		string url = VDLDefault::URL_VLIVE + "/" + vod_list["videoList"][i]["videoSeq"].asString();
		list_v.push_back(url);
	}
	list_v.push_back(vod_list["totalVideoCount"].asString());

	return list_v;
}

void Daum::setTitle(wstring str) {
	title = str;
}

void Daum::setUrl(string url) {
	this->url = url;
}