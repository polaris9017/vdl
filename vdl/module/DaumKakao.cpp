#include <stdafx.h>

bool DaumKakao::Run() {
	using namespace VDLDefault;

	cout << print("[kakao] loading kakao tv API json...") << endl;
	string clipid = Parse(this->url, KAKAO_CLIPID)[1];

	//Get Daum TVPot video id from Kakao TV clipLink API(in s1.daumcdn.net/svc/original/U03/cssjs/klimt/kakaotv-eb201f18f4.min.js)
	string vid = ParseJson(LoadPage(URL_INTERNAL_KAKAO_API + clipid).text)["clip"]["vid"].asString();
	//Set title
	UnicodeString title_u = title_u.fromUTF8(ParseJson(LoadPage(URL_INTERNAL_KAKAO_API + clipid).text)["clip"]["title"].asString());  
	wstring title_w = title_u.getBuffer();

	Daum m(URL_DAUM + "/v/" + vid);

	m.setTitle(title_w);
	cout << print("[kakao] Redirecting to Daum TVPot module... ", WRN) << endl;
	return m.Run();
}