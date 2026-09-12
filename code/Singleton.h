#pragma once
#include<memory>

template <typename T>
class Singleton
{
public:
	static T& instance()
	{
		//std::make_uniqueの実装上、Gameクラスの外部関数（ライブラリコード）からnew T()を呼び出す
		//外部から呼び出すことでGameクラスのprivateコンストラクタにアクセスできないためエラーになる
		//make_uniqueが言語仕様で使えないので代わりに指定した型(ここでいうnew T()の部分)のポインタを入れることで解決する
		static std::unique_ptr<T> instance = std::unique_ptr<T>(new T());
		return *instance;
	}

	Singleton(const Singleton&)				= delete;	//コピー禁止
	Singleton& operator=(const Singleton&)	= delete;	//代入禁止

protected:
	Singleton() {};
	~Singleton() {};
};