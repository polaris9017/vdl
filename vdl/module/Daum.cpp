#include <stdafx.h>

bool Daum::Run() {
	using namespace VDLDefault;

	string vid_id = Parse(this->url, DAUM_CLIPID)[1];

	//Get metadata json
	auto res_data = ParseJson(LoadPage(URL_INTERNAL_DAUM_RESOL,
		cpr::Parameters{ {"vid", vid_id},{"dte_type","WEB"} }).text)["output_list"]["output_list"];

	string *res = new string[res_data.size()];
	string *profile = new string[res_data.size()];
	string *cdnUrl = new string[res_data.size()];
	unsigned long *vid_fsz = new unsigned long[res_data.size()];
	
	cout << print("(1/2) 영상 정보 불러오는 중...") << endl;

	for (int i = 0; i < res_data.size(); i++) {
		vid_fsz[i] = stoul(res_data[i]["filesize"].asString());
		res[i] = res_data[i]["label"].asString();
		profile[i] = res_data[i]["profile"].asString();

		//Parse XML
		string *xml_doc = new string(LoadPage(URL_INTERNAL_DAUM_CDN, cpr::Parameters{ { "vid", vid_id },{ "profile",profile[i] } }).text.c_str());
		pugi::xml_document *doc = new pugi::xml_document();
		if (doc->load_string(xml_doc->c_str())) {
			pugi::xml_node *node = new pugi::xml_node(doc->child("videofarm").child("result"));
			cdnUrl[i] = node->child_value("url");
			delete node;
		}
		else
			cerr << print("[daum] Error while parsing json!", ERR) << endl;

		delete doc;
		delete xml_doc;
	}
		

	if (title == L"") {
		UnicodeString title_u;

		//Parse XML
		pugi::xml_document *doc = new pugi::xml_document();
		string xml_doc = LoadPage(URL_INTERNAL_DAUM_META, cpr::Parameters{ { "vid", vid_id } }).text;
		pugi::xml_node *node = new pugi::xml_node(doc->child("CLIP"));

		if (doc->load_string((const char*)xml_doc.c_str())) {
			title_u.fromUTF8(node->child_value("TITLE"));
		}
		else
			cerr << print("[daum] Error while parsing json!", ERR) << endl;

		title = title_u.getBuffer();

		delete doc;
	}

	cout << print("(2/2) 영상 내려받는 중...") << endl << endl
		<< "영상 해상도: " <<res[0] << endl
		<< "파일 사이즈: " << (float)vid_fsz[0] / (1024 * 1024) << "MB" << endl;

	bool ret = VDLDefault::Download(cdnUrl[0], L"[TVPOT] " + title + L".mp4");

	if (ret)
		cout << print("다운로드가 완료되었습니다.") << endl;
	else
		cout << print("다운로드가 완료되지 않았습니다.", ERR) << endl;


	//Free variables
	delete[]res;
	delete[]vid_fsz;
	delete[]profile;
	delete[]cdnUrl;

	return ret;
}
