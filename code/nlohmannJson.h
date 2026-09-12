#pragma once
#include<string>
#include<fstream>
#include"packages/nlohmann.json.3.12.0/build/native/include/nlohmann/json.hpp"

inline nlohmann::json LoadDataJson(const std::string& fileName)
{
	//jsonファイルを開く
	std::ifstream stream(fileName);

	//例外処理
	try//例外が発生するかもしれない処理
	{
		//parseはjsonファイルからC++で使えるように変換するもの。
		nlohmann::json data = nlohmann::json::parse(stream);
		stream.close();	//パースしたら閉じておｋ

		//変換したデータを返す
		return data;
	}
	catch (const std::exception&)//例外が発生したときにどうするかの処理
	{
		//エラー処理(trychach文の中に入れる。throwで例外を発生させる。runtime_errorは実行時のエラー)
		if (!stream.is_open())
			throw std::runtime_error("Failed open file.");
		if (!nlohmann::json::accept(stream))
			throw std::runtime_error("jsonのフォーマットが不正");
	}
}