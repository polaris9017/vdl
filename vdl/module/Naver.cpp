#include <stdafx.h>

bool Naver::Run()
{
	using namespace VDLDefault;

	auto r = LoadPage(this->url).text;
	cout << endl << print("페이지 불러오는 중...") << endl;

	auto param = Parse(r, NAVER_MID);
	cout << endl << print("Media id 추출 중...") << endl;

	string m_id = param[1], key = param[2];

	auto pInfo = LoadPage(URL_INTERNAL_NAVER + m_id,
		cpr::Parameters{{ "key",key }}).text;

	cout << print("(1/2) 영상 정보 불러오는 중...") << endl;

	UnicodeString title_u = title_u.fromUTF8(ParseJson(pInfo)["meta"]["subject"].asString());
	wstring title = title_u.getBuffer();
	decodeHtmlEntity(title);

	Json::Value json_list = ParseJson(pInfo)["videos"]["list"];
	int json_last_list = json_list.size() - 1;
	string *json_url = new string[json_list.size()];
	string *json_res = new string[json_list.size()];
	unsigned long *json_fsz = new unsigned long[json_list.size()];

	for (size_t i = 0; i < json_list.size(); ++i) {
		json_url[i] = json_list[i]["source"].asString();
		json_res[i] = json_list[i]["encodingOption"]["name"].asString();
		json_fsz[i] = stoul(json_list[i]["size"].asString());
	}

	cout << print("(2/2) 영상 내려받는 중...") << endl << endl
		<< "영상 해상도: " << json_res[json_last_list] << endl
		<< "파일 사이즈: " << (float)json_fsz[json_last_list] / (1024 * 1024) << "MB" << endl;

	VDLDefault::Download(json_url[json_last_list], L"[TVCAST] " + title + L".mp4");

	cout << print("다운로드가 완료되었습니다.") << endl;

	delete[]json_url;
	delete[]json_res;

	return true;
}