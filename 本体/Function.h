#pragma once
#include <Siv3D.hpp>

//�t�H���_���̃t�@�C�������ׂĎ擾
Array<String> GetFiles(String folderPath);
//�t�@�C���̊g���q���擾
String GetFileEx(String FilePath);
//�g���q���܂܂Ȃ��t�@�C�������擾
String GetFileNameOnly(String path);
//�g���q���܂ރt�@�C�������擾
String GetFileName_And_Ex(String path);
//�����񂩂�J�n�ʒu���擾
String Get_Text_Start_Position(String Text_Path);
//�����񂩂�I���ʒu���擾
String Get_Text_End_Position(String Text_Path);
//�ϊ������镶�����w��
void Change_Romaji_Function(Array<size_t> SetMojiData);
//������ɕ������܂܂�Ă�����ݒ�ɏ]���ĕϊ�����
String Change_Romaji_By_Setting(String Text, int &All_Key_Number);
//���v���C�̓��e���玞�Ԃ��擾
double Replay_Get_Time_Adaptations(String FileLine);
//���v���C�̓��e���炻�̏ꏊ���������Ă��邩���擾
bool Replay_Get_Correct_Adaptations(String FileLine);
//���v���C�̓��e����X�R�A���擾
int Replay_Get_Time_Score_Adaptations(String FileLine);