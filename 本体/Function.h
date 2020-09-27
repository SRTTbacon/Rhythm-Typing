#pragma once
#include <Siv3D.hpp>

//フォルダ内のファイルをすべて取得
Array<String> GetFiles(String folderPath);
//ファイルの拡張子を取得
String GetFileEx(String FilePath);
//拡張子を含まないファイル名を取得
String GetFileNameOnly(String path);
//拡張子を含むファイル名を取得
String GetFileName_And_Ex(String path);
//文字列から開始位置を取得
String Get_Text_Start_Position(String Text_Path);
//文字列から終了位置を取得
String Get_Text_End_Position(String Text_Path);
//変換させる文字を指定
void Change_Romaji_Function(Array<size_t> SetMojiData);
//文字列に文字が含まれていたら設定に従って変換する
String Change_Romaji_By_Setting(String Text, int &All_Key_Number);
//リプレイの内容から時間を取得
double Replay_Get_Time_Adaptations(String FileLine);
//リプレイの内容からその場所が正解しているかを取得
bool Replay_Get_Correct_Adaptations(String FileLine);
//リプレイの内容からスコアを取得
int Replay_Get_Time_Score_Adaptations(String FileLine);