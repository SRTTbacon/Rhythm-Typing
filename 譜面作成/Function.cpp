#include <Siv3D.hpp>
#include "Function.h"

//テキストボックスが数字のみか確認
bool Check_int(String str)
{
	//小数の点も反応してしまうため事前に削除する
	String Replace_str = str.replace(U".", U"");
	if (std::all_of(Replace_str.cbegin(), Replace_str.cend(), isdigit))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//パスから拡張子を取得
String Get_File_Extension(String const& path)
{
	return path.substr(path.lastIndexOf(U'.') + 1);
}
//パスからファイル名を取得
String Get_File_Name(String const& path)
{
	return path.substr(path.lastIndexOf(U'/') + 1);
}
//パスから拡張子を含まないファイル名を取得
String Get_File_Name_Only(String const& path)
{
	String File_Name = path.substr(path.lastIndexOf(U'/') + 1);
	String Name_Only = File_Name.replace(File_Name.substr(File_Name.lastIndexOf(U'.')), U"");
	return Name_Only;
}
//ファイルやフォルダを削除
void File_Directory_Delete(String const& Directory_Path)
{
	FileSystem::Remove(Directory_Path, false);
}
//文字列から開始位置を取得
String Get_Text_Start_Position(String const& Text_Path)
{
	String Start = Text_Path.substr(Text_Path.lastIndexOf(U'|') + 3);
	String End = Text_Path.substr(Text_Path.lastIndexOf(U'/'));
	return Start.replace(U" " + End, U"");
}
//文字列から終了位置を取得
String Get_Text_End_Position(String const& Text_Path)
{
	return Text_Path.substr(Text_Path.lastIndexOf(U'/') + 2);
}