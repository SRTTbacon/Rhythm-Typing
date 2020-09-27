#include <Siv3D.hpp>
#include "Function.h"

//�e�L�X�g�{�b�N�X�������݂̂��m�F
bool Check_int(String str)
{
	//�����̓_���������Ă��܂����ߎ��O�ɍ폜����
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
//�p�X����g���q���擾
String Get_File_Extension(String const& path)
{
	return path.substr(path.lastIndexOf(U'.') + 1);
}
//�p�X����t�@�C�������擾
String Get_File_Name(String const& path)
{
	return path.substr(path.lastIndexOf(U'/') + 1);
}
//�p�X����g���q���܂܂Ȃ��t�@�C�������擾
String Get_File_Name_Only(String const& path)
{
	String File_Name = path.substr(path.lastIndexOf(U'/') + 1);
	String Name_Only = File_Name.replace(File_Name.substr(File_Name.lastIndexOf(U'.')), U"");
	return Name_Only;
}
//�t�@�C����t�H���_���폜
void File_Directory_Delete(String const& Directory_Path)
{
	FileSystem::Remove(Directory_Path, false);
}
//�����񂩂�J�n�ʒu���擾
String Get_Text_Start_Position(String const& Text_Path)
{
	String Start = Text_Path.substr(Text_Path.lastIndexOf(U'|') + 3);
	String End = Text_Path.substr(Text_Path.lastIndexOf(U'/'));
	return Start.replace(U" " + End, U"");
}
//�����񂩂�I���ʒu���擾
String Get_Text_End_Position(String const& Text_Path)
{
	return Text_Path.substr(Text_Path.lastIndexOf(U'/') + 2);
}