#include <Siv3D.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>

size_t HU_or_FU_01, ZI_or_JI_01, X_or_L_01, TY_or_CH_01, SY_or_SH_01, K_or_C_01, TI_or_CHI_01, TU_or_TSU_01, ZY_or_J_01, SI_or_SHI_01;
bool IsSettingTrue = false;
//フォルダ内のファイルをすべて取得
Array<String> GetFiles(String folderPath)
{
    Array<String> Temp = {};
    using namespace std::filesystem;
    directory_iterator iter(folderPath.narrow()), end;
    std::error_code err;
    for (; iter != end && !err; iter.increment(err))
    {
        const directory_entry entry = *iter;
        Temp.push_back(Unicode::Widen(entry.path().string()));
    }
    if (err)
    {
        return {};
    }
    return Temp;
}
//ファイルの拡張子を取得
String GetFileEx(String FilePath)
{
    String Temp = FileSystem::FileName(FilePath);
    String Path = Temp.substr(Temp.lastIndexOf(U'.'));
    return Path;
}
//拡張子を含まないファイル名を取得
String GetFileNameOnly(String path)
{
    String Temp = FileSystem::FileName(path).replace(U"Music\\", U"");;
    String Path = Temp.replace(Temp.substr(Temp.lastIndexOf(U'.')), U"");
    return Path;
}
//拡張子を含むファイル名を取得
String GetFileName_And_Ex(String path)
{
    return FileSystem::FileName(path);
}
//文字列から開始位置を取得
String Get_Text_Start_Position(String Text_Path)
{
    String Start = Text_Path.substr(Text_Path.lastIndexOf(U'|') + 3);
    String End = Text_Path.substr(Text_Path.lastIndexOf(U'/'));
    return Start.replace(U" " + End, U"");
}
//文字列から終了位置を取得
String Get_Text_End_Position(String Text_Path)
{
    return Text_Path.substr(Text_Path.lastIndexOf(U'/') + 2);
}
//変換させる文字を指定
void Change_Romaji_Function(Array<size_t> SetMojiData)
{
    for (int Number = 0; Number <= (int)SetMojiData.size(); Number++)
    {
        if (Number == 0)
        {
            HU_or_FU_01 = SetMojiData[0];
        }
        else if (Number == 1)
        {
            ZI_or_JI_01 = SetMojiData[1];
        }
        else if (Number == 2)
        {
            X_or_L_01 = SetMojiData[2];
        }
        else if (Number == 3)
        {
            TY_or_CH_01 = SetMojiData[3];
        }
        else if (Number == 4)
        {
            SY_or_SH_01 = SetMojiData[4];
        }
        else if (Number == 5)
        {
            K_or_C_01 = SetMojiData[5];
        }
        else if (Number == 6)
        {
            TI_or_CHI_01 = SetMojiData[6];
        }
        else if (Number == 7)
        {
            TU_or_TSU_01 = SetMojiData[7];
        }
        else if (Number == 8)
        {
            ZY_or_J_01 = SetMojiData[8];
        }
        else if (Number == 9)
        {
            SI_or_SHI_01 = SetMojiData[9];
        }
    }
    IsSettingTrue = true;
}
//文字列に文字が含まれていたら設定に従って変換する
String Change_Romaji_By_Setting(String Text, int &All_Key_Number)
{
    if (IsSettingTrue)
    {
        String Replace_Text = Text;
        if (Replace_Text.indexOf(U"hu") != String::npos && HU_or_FU_01 == 1)
        {
            Replace_Text.replace(U"hu", U"fu");
        }
        if (Replace_Text.indexOf(U"fu") != String::npos && HU_or_FU_01 == 0)
        {
            Replace_Text.replace(U"fu", U"hu");
        }
        if (Replace_Text.indexOf(U"zi") != String::npos && ZI_or_JI_01 == 1)
        {
            Replace_Text.replace(U"zi", U"ji");
        }
        if (Replace_Text.indexOf(U"ji") != String::npos && ZI_or_JI_01 == 0)
        {
            Replace_Text.replace(U"ji", U"zi");
        }
        if (X_or_L_01 == 1)
        {
            if (Replace_Text.indexOf(U"xa") != String::npos)
            {
                Replace_Text.replace(U"xa", U"la");
            }
            if (Replace_Text.indexOf(U"xi") != String::npos)
            {
                Replace_Text.replace(U"xi", U"li");
            }
            if (Replace_Text.indexOf(U"xu") != String::npos)
            {
                Replace_Text.replace(U"xu", U"lu");
            }
            if (Replace_Text.indexOf(U"xe") != String::npos)
            {
                Replace_Text.replace(U"xe", U"le");
            }
            if (Replace_Text.indexOf(U"xo") != String::npos)
            {
                Replace_Text.replace(U"xo", U"lo");
            }
        }
        else
        {
            if (Replace_Text.indexOf(U"la") != String::npos)
            {
                Replace_Text.replace(U"la", U"xa");
            }
            if (Replace_Text.indexOf(U"li") != String::npos)
            {
                Replace_Text.replace(U"li", U"xi");
            }
            if (Replace_Text.indexOf(U"lu") != String::npos)
            {
                Replace_Text.replace(U"lu", U"xu");
            }
            if (Replace_Text.indexOf(U"le") != String::npos)
            {
                Replace_Text.replace(U"le", U"xe");
            }
            if (Replace_Text.indexOf(U"lo") != String::npos)
            {
                Replace_Text.replace(U"lo", U"xo");
            }
        }
        if (TY_or_CH_01 == 1)
        {
            if (Replace_Text.indexOf(U"tya") != String::npos)
            {
                Replace_Text.replace(U"tya", U"cha");
            }
            if (Replace_Text.indexOf(U"tyu") != String::npos)
            {
                Replace_Text.replace(U"tyu", U"chu");
            }
            if (Replace_Text.indexOf(U"tye") != String::npos)
            {
                Replace_Text.replace(U"tye", U"che");
            }
            if (Replace_Text.indexOf(U"tyo") != String::npos)
            {
                Replace_Text.replace(U"tyo", U"cho");
            }
        }
        else
        {
            if (Replace_Text.indexOf(U"cha") != String::npos)
            {
                Replace_Text.replace(U"cha", U"tya");
            }
            if (Replace_Text.indexOf(U"chu") != String::npos)
            {
                Replace_Text.replace(U"chu", U"tyu");
            }
            if (Replace_Text.indexOf(U"che") != String::npos)
            {
                Replace_Text.replace(U"che", U"tye");
            }
            if (Replace_Text.indexOf(U"cho") != String::npos)
            {
                Replace_Text.replace(U"cho", U"tyo");
            }
        }
        if (SY_or_SH_01 == 1)
        {
            if (Replace_Text.indexOf(U"sya") != String::npos)
            {
                Replace_Text.replace(U"sya", U"sha");
            }
            if (Replace_Text.indexOf(U"syu") != String::npos)
            {
                Replace_Text.replace(U"syu", U"shu");
            }
            if (Replace_Text.indexOf(U"sye") != String::npos)
            {
                Replace_Text.replace(U"sye", U"she");
            }
            if (Replace_Text.indexOf(U"syo") != String::npos)
            {
                Replace_Text.replace(U"syo", U"sho");
            }
        }
        else
        {
            if (Replace_Text.indexOf(U"sha") != String::npos)
            {
                Replace_Text.replace(U"sha", U"sya");
            }
            if (Replace_Text.indexOf(U"shu") != String::npos)
            {
                Replace_Text.replace(U"shu", U"syu");
            }
            if (Replace_Text.indexOf(U"she") != String::npos)
            {
                Replace_Text.replace(U"she", U"sye");
            }
            if (Replace_Text.indexOf(U"sho") != String::npos)
            {
                Replace_Text.replace(U"sho", U"syo");
            }
        }
        if (K_or_C_01 == 1)
        {
            if (Replace_Text.indexOf(U"ka") != String::npos)
            {
                Replace_Text.replace(U"ka", U"ca");
            }
            if (Replace_Text.indexOf(U"ku") != String::npos)
            {
                Replace_Text.replace(U"ku", U"cu");
            }
            if (Replace_Text.indexOf(U"ko") != String::npos)
            {
                Replace_Text.replace(U"ko", U"co");
            }
        }
        else
        {
            if (Replace_Text.indexOf(U"ca") != String::npos)
            {
                Replace_Text.replace(U"ca", U"ka");
            }
            if (Replace_Text.indexOf(U"cu") != String::npos)
            {
                Replace_Text.replace(U"cu", U"ku");
            }
            if (Replace_Text.indexOf(U"co") != String::npos)
            {
                Replace_Text.replace(U"co", U"ko");
            }
        }
        if (Replace_Text.indexOf(U"ti") != String::npos && TI_or_CHI_01 == 1)
        {
            Replace_Text.replace(U"ti", U"chi");
            All_Key_Number++;
        }
        if (Replace_Text.indexOf(U"chi") != String::npos && TI_or_CHI_01 == 0)
        {
            Replace_Text.replace(U"chi", U"ti");
            All_Key_Number--;
        }
        if (Replace_Text.indexOf(U"tu") != String::npos && TU_or_TSU_01 == 1)
        {
            Replace_Text.replace(U"tu", U"tsu");
            All_Key_Number++;
        }
        if (Replace_Text.indexOf(U"tsu") != String::npos && TU_or_TSU_01 == 0)
        {
            Replace_Text.replace(U"tsu", U"tu");
            All_Key_Number--;
        }
        if (ZY_or_J_01 == 1)
        {
            if (Replace_Text.indexOf(U"zya") != String::npos)
            {
                Replace_Text.replace(U"zya", U"ja");
                All_Key_Number--;
            }
            if (Replace_Text.indexOf(U"zyu") != String::npos)
            {
                Replace_Text.replace(U"zyu", U"ju");
                All_Key_Number--;
            }
            if (Replace_Text.indexOf(U"zye") != String::npos)
            {
                Replace_Text.replace(U"zye", U"je");
                All_Key_Number--;
            }
            if (Replace_Text.indexOf(U"zyo") != String::npos)
            {
                Replace_Text.replace(U"zyo", U"jo");
                All_Key_Number--;
            }
        }
        else
        {
            if (Replace_Text.indexOf(U"ja") != String::npos)
            {
                Replace_Text.replace(U"ja", U"zya");
                All_Key_Number++;
            }
            if (Replace_Text.indexOf(U"ju") != String::npos)
            {
                Replace_Text.replace(U"ju", U"zyu");
                All_Key_Number++;
            }
            if (Replace_Text.indexOf(U"je") != String::npos)
            {
                Replace_Text.replace(U"je", U"zye");
                All_Key_Number++;
            }
            if (Replace_Text.indexOf(U"jo") != String::npos)
            {
                Replace_Text.replace(U"jo", U"zyo");
                All_Key_Number++;
            }
        }
        if (Replace_Text.indexOf(U"si") != String::npos && SI_or_SHI_01 == 1)
        {
            Replace_Text.replace(U"si", U"shi");
            All_Key_Number++;
        }
        if (Replace_Text.indexOf(U"shi") != String::npos && SI_or_SHI_01 == 0)
        {
            Replace_Text.replace(U"shi", U"si");
            All_Key_Number--;
        }
        return Replace_Text;
    }
    else
    {
        return Text;
    }
}
//リプレイの内容から時間を取得
double Replay_Get_Time_Adaptations(String FileLine)
{
    String Temp = FileLine;
    return Parse<double>(Temp.replace(FileLine.substr(FileLine.lastIndexOf(U'|')), U""));
}
//リプレイの内容からその場所が正解しているかを取得
bool Replay_Get_Correct_Adaptations(String FileLine)
{
    String Line = FileLine;
    String Temp = FileLine.substr(FileLine.lastIndexOf(U'|') + 1);
    return Parse<bool>(Temp.replace(FileLine.substr(FileLine.lastIndexOf(U'/')), U""));
}
//リプレイの内容からスコアを取得
int Replay_Get_Time_Score_Adaptations(String FileLine)
{
    return Parse<int>(FileLine.substr(FileLine.lastIndexOf(U'/') + 1));
}