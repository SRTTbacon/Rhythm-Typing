#include <Siv3D.hpp>
#include <iostream>
#include <fstream>
#include "Function.h"

Array<String> Music_Lists = {};
Array<double> Start_Times = {};
Array<double> End_Times = {};
Array<String> Lyrics = {};
Array<String> Lyrics_Romaji = {};
Array<String> Replay_Times = {};
Array<double> Replay_Correct_Times = {};
Array<bool> Replay_Correct_Adaptations = {};
Array<int> Replay_Scores = {};
String Music_Name = U"";
String Lyrics_Time_Now_String = U"";
String Lyrics_Time_Now_Romaji_String = U"";
String Game_Mode = U"タイトル";
String Replay_Select = U"";
int KeyDownNumber = 0;
int MissNumber = 0;
int Last_Lyrics_Number = 0;
int Max_Lyrics = 0;
int Lyrice_Number_Now = 0;
int Score = 0;
int CorrectNumber = 0;
int Replay_Number_Now = 0;
int All_Key_Number = 0;
int Music_Page_Now = 1;
int Music_Page_Max = 1;
int Music_Page_Now_Number = 0;
double Audio_Pitch = 1;
double Progress_Now = 0;
double Progress_Max = 0;
double Time_Seconds = 0;
double Replay_Max_Time_Now = 0;
double WPM_Calculation = 0;
double Max_WPM = 0;
double Music_All_Seconds = 0;
bool IsLyricsFirstShow = false;
bool IsLyricsLastShow = false;
bool First_Execute = true;
bool IsMusicStopped = false;
bool Auto_Mode = false;
bool Replay_Record = true;
bool IsReplayMode = false;
size_t HU_or_FU = 0;
size_t ZI_or_JI = 0;
size_t X_or_L = 0;
size_t TY_or_CH = 0;
size_t SY_or_SH = 0;
size_t K_or_C = 0;
size_t TI_or_CHI = 0;
size_t TU_or_TSU = 0;
size_t ZY_or_J = 0;
size_t SI_or_SHI = 0;
//曲をすべて取得
void Music_Update()
{
    Music_Lists = {};
    if (FileSystem::Exists(U"Resources/Music"))
    {
        Array<String> Temp = GetFiles(U"Resources/Music");
        for (int Temp_Number = 0; Temp_Number <= (int)Temp.size() - 1; Temp_Number++)
        {
            if (GetFileEx(Temp[Temp_Number]) == U".LTS")
            {
                Music_Lists.push_back(GetFileNameOnly(Temp[Temp_Number]));
                Music_Page_Max++;
            }
        }
    }
    else
    {
        FileSystem::CreateDirectories(U"Resources/Music");
    }
    if (!FileSystem::Exists(U"Resources/Replays"))
    {
        FileSystem::CreateDirectories(U"Resources/Replays");
    }
}
//指定された文字の曲が存在するか
bool Music_Already_Exists(String File_Name_Only)
{
    bool Temp = false;
    for (int Number = 0; Number <= (int)Music_Lists.size() - 1; Number++)
    {
        if (Music_Lists[Number] == File_Name_Only) 
        {
            Temp = true;
        }
    }
    return Temp;
}
//ページ数と曲数を照らし合わせるてページ中に何個曲があるか(語彙力崩壊)、戻り値はそのページにある曲数、引数は現在のページ
int Music_Page_Quantity(int Page_Now)
{
    int Music_Remaining = (int)Music_Lists.size() % 5;
    int Page_Number = ((int)Music_Lists.size() - 1) / 5;
    if (Music_Remaining == 0)
    {
        Music_Page_Max = Page_Number;
    }
    else
    {
        Music_Page_Max = Page_Number + 1;
    }
    if (Music_Page_Max == Page_Now)
    {
        return Music_Remaining;
    }
    else
    {
        return 5;
    }
}
//歌詞のみを取得
String Get_Lyrics_Only(int List_Number)
{
    if (List_Number != -1)
    {
        String Temp_00 = Lyrics[List_Number];
        String Temp = Temp_00.replace(Temp_00.substr(Temp_00.indexOf(U'|') - 2), U"");
        if (Lyrics_Time_Now_String != Temp)
        {
            KeyDownNumber = 0;
            Progress_Max = End_Times[List_Number] - Start_Times[List_Number];
            Lyrice_Number_Now = List_Number;
        }
        return Temp;
    }
    else
    {
        return U"";
    }
}
//現在の時間に歌詞が存在するか(する場合は対応するリストの番号、しない場合は-1を返す)
int Get_Time_Lyrics_Now(double Time_Now)
{
    if (Start_Times.size() != size_t(0) && End_Times.size() != size_t(0))
    {
        int Return_Number = -1;
        int Temp = 1;
        for (int Number = 0; Number <= (int)Start_Times.size() - 1; Number++)
        {
            Temp++;
            if (Start_Times[Number] <= Time_Now && End_Times[Number] >= Time_Now)
            {
                Return_Number = Number;
                IsLyricsFirstShow = true;
                break;
            }
        }
        if (Start_Times[0] > Time_Now)
        {
            IsLyricsFirstShow = false;
        }
        if (Time_Now >= End_Times[(int)End_Times.size() - 1])
        {
            IsLyricsLastShow = true;
        }
        else
        {
            IsLyricsLastShow = false;
        }
        if (Return_Number == -1)
        {
            if (IsLyricsFirstShow)
            {
                Last_Lyrics_Number = Temp;
            }
            else
            {
                Last_Lyrics_Number = 0;
            }
            return -1;
        }
        return Return_Number;
    }
    else
    {
        return -1;
    }
}
//入力された文字が合っていればKeyDownNumberを追加させる
void Correct_Key_Check(String Get_Key, bool IsCorrect = true)
{
    if (!IsMusicStopped)
    {
        if ((int)Lyrics_Time_Now_Romaji_String.size() - 1 >= KeyDownNumber)
        {
            if (Format(Lyrics_Time_Now_Romaji_String[KeyDownNumber]) == Get_Key && IsCorrect)
            {
                if (!IsReplayMode)
                {
                    Replay_Times.push_back(Format(Time_Seconds) + U"|True" + U"/" + Format(Score));
                    Score += (int)(100 * Audio_Pitch);
                }
                KeyDownNumber++;
                CorrectNumber++;
                if (Format(Lyrics_Time_Now_Romaji_String[KeyDownNumber]) == U" ")
                {
                    KeyDownNumber++;
                }
                if (!IsReplayMode && (int)Lyrics_Time_Now_Romaji_String.size() - 1 < KeyDownNumber)
                {
                    Score += (int)(400 * Audio_Pitch);
                    Score += Parse<int>(Format(std::round((Progress_Max - Progress_Now) * 100) * 3));
                }
            }
            else
            {
                if (Get_Key != U" ")
                {
                    if (!IsReplayMode)
                    {
                        Replay_Times.push_back(Format(Time_Seconds) + U"|False" + U"/" + Format(Score));
                    }
                    MissNumber++;
                }
            }
        }
    }
}
//リプレイが有効な場合、内容によってスコアなどを入力する(語彙力皆無)
void Replay_Time_And_Correct_Reflect()
{
    if (!IsMusicStopped)
    {
        if (Replay_Correct_Times[Replay_Number_Now] <= Time_Seconds)
        {
            if ((int)Replay_Correct_Times.size() - 1 > Replay_Number_Now)
            {
                Replay_Number_Now++;
            }
            if (Replay_Correct_Adaptations[Replay_Number_Now])
            {
                Correct_Key_Check(Format(Lyrics_Time_Now_Romaji_String[KeyDownNumber]));
            }
            else
            {
                Correct_Key_Check(Format(Lyrics_Time_Now_Romaji_String[KeyDownNumber]), false);
            }
            Score = Replay_Scores[Replay_Number_Now];
        }
    }
}
void Replay_Back_or_Next(double Time)
{
    if (Time <= Music_All_Seconds)
    {
        Score = 0;
        MissNumber = 0;
        KeyDownNumber = 0;
        CorrectNumber = 0;
        Replay_Number_Now = 0;
        int Time_Number = 0;
        for (int Number_01 = 0; Number_01 <= (int)Replay_Correct_Times.size() - 1; Number_01++)
        {
            if (Replay_Correct_Times[Number_01] <= Time)
            {
                Time_Number++;
                Replay_Number_Now++;
                if (Replay_Correct_Adaptations[Number_01])
                {
                    CorrectNumber++;
                }
                else
                {
                    MissNumber++;
                }
            }
            else
            {
                break;
            }
        }
        Score = Replay_Scores[Time_Number];
    }
}
//リセット
void Reset()
{
    First_Execute = true;
    IsLyricsFirstShow = false;
    Auto_Mode = false;
    IsReplayMode = false;
    Music_Lists = {};
    Start_Times = {};
    End_Times = {};
    Lyrics = {};
    Lyrics_Romaji = {};
    Replay_Scores = {};
    Replay_Correct_Adaptations = {};
    Replay_Correct_Times = {};
    Replay_Max_Time_Now = 0;
    Replay_Number_Now = 0;
    Replay_Times = {};
    KeyDownNumber = 0;
    MissNumber = 0;
    Progress_Max = 0;
    Progress_Now = 0;
    Score = 0;
    MissNumber = 0;
    CorrectNumber = 0;
    All_Key_Number = 0;
    Music_All_Seconds = 0;
    Music_Name = U"";
    Replay_Select = U"";
    Game_Mode = U"タイトル";
}
//起動時の処理
void Main()
{
	Graphics::SetTargetFrameRateHz(60);
	Window::SetTitle(U"リズムタイピング");
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
	Window::Resize(Size(1280, 720));
	Scene::SetBackground(Color(255, 255, 255));
    FontAsset::Register(U"Text", 25, Typeface::Bold);
    Font Lyrics_Romaji_Font(25, Typeface::Bold);
    Font Lyrics_Number_String(25, Typeface::Bold);
    double Audio_Volume = 0.5;
    double Auto_Speed = 2;
    double Auto_Speed_Temp = 0;
    Audio audio;
    int First_Height = 150;
    JSONReader json_read(U"Resources/Setting.dat");
    if (json_read)
    {
        HU_or_FU = json_read[U"Setting.HU_or_FU"].get<size_t>();
        ZI_or_JI = json_read[U"Setting.ZI_or_JI"].get<size_t>();
        X_or_L = json_read[U"Setting.X_or_L"].get<size_t>();
        TY_or_CH = json_read[U"Setting.TY_or_CH"].get<size_t>();
        SY_or_SH = json_read[U"Setting.SY_or_SH"].get<size_t>();
        K_or_C = json_read[U"Setting.K_or_C"].get<size_t>();
        TI_or_CHI = json_read[U"Setting.TI_or_CHI"].get<size_t>();
        TU_or_TSU = json_read[U"Setting.TU_or_TSU"].get<size_t>();
        ZY_or_J = json_read[U"Setting.ZY_or_J"].get<size_t>();
        SI_or_SHI = json_read[U"Setting.SI_or_SHI"].get<size_t>();
        Audio_Volume = json_read[U"Setting.Audio_Volume"].get<double>();
        Audio_Pitch = json_read[U"Setting.Audio_Pitch"].get<double>();
        Replay_Record = json_read[U"Setting.Replay_Record"].get<bool>();
        json_read.close();
    }
    else
    {
        json_read.close();
    }
	while (System::Update())
	{
        if (Game_Mode == U"タイトル")
        {
            if (First_Execute)
            {
                First_Execute = false;
                Music_Update();
                Max_Lyrics = 0;
                Music_Page_Now_Number = Music_Page_Quantity(Music_Page_Now);
            }
            FontAsset(U"Text")(U"キー設定").draw(Arg::bottomLeft(110, 45), Palette::Black);
            SimpleGUI::HorizontalRadioButtons(HU_or_FU, { U"ふ = HU,",U"FU" }, Vec2(10, 50), unspecified, true);
            SimpleGUI::HorizontalRadioButtons(ZI_or_JI, { U"じ = ZI,",U"JI" }, Vec2(10, 80), unspecified, true);
            SimpleGUI::HorizontalRadioButtons(X_or_L, { U"ぁぃぅぇぉ = X*,",U"L*" }, Vec2(10, 110), unspecified, true);
            SimpleGUI::HorizontalRadioButtons(TY_or_CH, { U"ちゃ,ちゅ,ちぇ,ちょ = TY*,",U"CH*" }, Vec2(10, 140), unspecified, true);
            SimpleGUI::HorizontalRadioButtons(SY_or_SH, { U"しゃ,しゅ,しぇ,しょ = SY*,",U"SH*" }, Vec2(10, 170), unspecified, true);
            SimpleGUI::HorizontalRadioButtons(K_or_C, { U"か,く,こ = K*,",U"C*" }, Vec2(10, 200), unspecified, true);
            SimpleGUI::HorizontalRadioButtons(TI_or_CHI, { U"ち = TI,",U"CHI" }, Vec2(10, 230), unspecified, true);
            SimpleGUI::HorizontalRadioButtons(TU_or_TSU, { U"つ = TU,",U"TSU" }, Vec2(10, 260), unspecified, true);
            SimpleGUI::HorizontalRadioButtons(ZY_or_J, { U"じゃ,じゅ,じぇ,じょ = ZY*,",U"J*" }, Vec2(10, 290), unspecified, true);
            SimpleGUI::HorizontalRadioButtons(SI_or_SHI, { U"し = SI,",U"SHI" }, Vec2(10, 320), unspecified, true);
            FontAsset(U"Text")(U"再生速度").draw(Arg::bottomCenter(1125, 40), Palette::Black);
            FontAsset(U"Text")(U"音量").draw(Arg::bottomCenter(1125, 115), Palette::Black);
            SimpleGUI::Slider(Format(std::round(Audio_Pitch * 100) / 100), Audio_Pitch, 0, 2, Vec2(930, 45), 50, 300, true);
            SimpleGUI::Slider(Format(std::round(Audio_Volume * 100)), Audio_Volume, Vec2(940, 120), 40, 300, true);
            Rectangle(Vec2(420, 90), 500, 390).draw(Palette::Black);
            FontAsset(U"Text")(U"＊曲リスト＊").draw(Arg::bottomCenter(670, 135), Palette::White);
            for (int Number = 1; Number <= Music_Page_Now_Number; Number++)
            {
                if (SimpleGUI::Button(Music_Lists[((5 * Music_Page_Now) - 6) + Number], Vec2(495, First_Height + Number * 50), 350, true))
                {
                    Music_Name = Music_Lists[((5 * Music_Page_Now) - 6) + Number];
                }
            }
            if (SimpleGUI::Button(U"曲を追加", Vec2(780, 100), 125, true))
            {
                bool Temp = false;
                Array<String> Files = Dialog::OpenFiles({ FileFilter::AllFiles() }, U"", U".LTSファイルを選択してください。");
                if (Files.size() != 0)
                {
                    for (int FileNumber = 0; FileNumber <= (int)Files.size() - 1; FileNumber++)
                    {
                        if (GetFileEx(Files[FileNumber]) == U".LTS")
                        {
                            if (!Music_Already_Exists(GetFileNameOnly(Files[FileNumber])))
                            {
                                FileSystem::Copy(Files[FileNumber], U"Resources/Music/" + GetFileName_And_Ex(Files[FileNumber]));
                            }
                            else
                            {
                                System::ShowMessageBox(U"同じ名前のデータが既に存在します。");
                            }
                        }
                        else
                        {
                            Temp = true;
                        }
                    }
                    if (Temp)
                    {
                        System::ShowMessageBox(U".LTS形式でないファイルが存在します。");
                    }
                    Music_Update();
                }
            }
            if (SimpleGUI::Button(U"リプレイを再生", Vec2(430, 100), 150, true))
            {
                Optional<String> Replay_File = Dialog::OpenFile({ FileFilter::AllFiles() }, FileSystem::CurrentDirectory() + U"Resources/Replays", U",LTRファイルを選択してください。");
                if (Replay_File.has_value())
                {
                    if (GetFileEx(Replay_File.value()) == U".LTR")
                    {
                        String File_Name = GetFileNameOnly(Replay_File.value());
                        String Temp = File_Name.substr(File_Name.indexOf(',') - 4);
                        if (Music_Already_Exists(File_Name.replace(Temp, U"")))
                        {
                            Replay_Select = Replay_File.value();
                            Music_Name = File_Name.replace(Temp, U"");
                        }
                        else
                        {
                            System::ShowMessageBox(U"リプレイファイルはありますが、該当する曲が存在しません。\n必要な曲名:" + File_Name.replace(Temp, U""));
                        }
                    }
                    else
                    {
                        System::ShowMessageBox(U".LTR形式でないファイルが選択されました。");
                    }
                }
            }
            if (Music_Page_Now != 1 && SimpleGUI::Button(Format(Music_Page_Now - 1), Vec2(445, 200), 35, true))
            {
                Music_Page_Now--;
                Music_Page_Now_Number = Music_Page_Quantity(Music_Page_Now);
            }
            if (Music_Page_Now != Music_Page_Max && SimpleGUI::Button(Format(Music_Page_Now + 1), Vec2(865, 200), 35, true))
            {
                Music_Page_Now++;
                Music_Page_Now_Number = Music_Page_Quantity(Music_Page_Now);
            }
            if (SimpleGUI::Button(U"終了", Vec2(0, 685), 120, true) || KeyEscape.down())
            {
                break;
            }
            SimpleGUI::CheckBox(Replay_Record, U"リプレイを記録", Vec2(930, 200), unspecified, true);
            if (Music_Name != U"")
            {
                Change_Romaji_Function({ HU_or_FU ,ZI_or_JI ,X_or_L ,TY_or_CH ,SY_or_SH ,K_or_C ,TI_or_CHI ,TU_or_TSU ,ZY_or_J ,SI_or_SHI });
                First_Execute = true;
                Game_Mode = U"ゲーム";
                String Write_Moji = U"7z.exe x -aoa -oResources/Extract \"" + FileSystem::CurrentDirectory() + U"Resources/Music/" + Music_Name + U".LTS\"";
                std::ofstream ifs("Zip_Decode.bat");
                ifs << Write_Moji << std::endl;
                ifs.close();
                system("Zip_Decode.bat");
                System::Sleep(2s);
                remove("Zip_Decode.bat");
                std::ifstream ifs3("Resources/Extract/Save_Lyrics.dat");
                std::string str;
                while (std::getline(ifs3, str))
                {
                    Lyrics.push_back(Unicode::Widen(str));
                    Start_Times.push_back(std::stod(Get_Text_Start_Position(Unicode::Widen(str)).narrow()));
                    End_Times.push_back(std::stod(Get_Text_End_Position(Unicode::Widen(str)).narrow()));
                    Max_Lyrics++;
                }
                ifs3.close();
                std::ifstream ifs4("Resources/Extract/Save_Lyrics_Romaji.dat");
                std::string str2;
                while (std::getline(ifs4, str2))
                {
                    Lyrics_Romaji.push_back(Change_Romaji_By_Setting(Unicode::Widen(str2), All_Key_Number));
                    for (int Temp = 0; Temp <= (int)str2.size() - 1; Temp++)
                    {
                        char Test = str2[Temp];
                        if (Test != '\n' && Test != ' ' && Test != '　')
                        {
                            All_Key_Number++;
                        }
                    }
                }
                ifs4.close();
                if (Replay_Select != U"")
                {
                    std::ifstream ifs5(Replay_Select.narrow());
                    std::string str3;
                    while (std::getline(ifs5, str3))
                    {
                        Replay_Correct_Times.push_back(Replay_Get_Time_Adaptations(Unicode::Widen(str3)));
                        Replay_Correct_Adaptations.push_back(Replay_Get_Correct_Adaptations(Unicode::Widen(str3)));
                        Replay_Scores.push_back(Replay_Get_Time_Score_Adaptations(Unicode::Widen(str3)));
                    }
                    ifs5.close();
                    IsReplayMode = true;
                }
                Max_Lyrics++;
            }
        }
        else if (Game_Mode == U"ゲーム")
        {
            if (First_Execute)
            {
                First_Execute = false;
                audio = Audio(U"Resources/Extract/Save_Music.dat");
                FileSystem::Remove(U"Resources/Extract");
                audio.play();
                KeyDownNumber = 0;
                MissNumber = 0;
                Music_All_Seconds = audio.lengthSec();
            }
            Time_Seconds = audio.posSec();
            int Number = Get_Time_Lyrics_Now(Time_Seconds);
            Lyrics_Time_Now_String = Get_Lyrics_Only(Number);
            if (Number != -1)
            {
                Progress_Now = Time_Seconds - Start_Times[Number];
            }
            String Key;
            TextInput::UpdateText(Key);
            if (Key != U"" && !IsReplayMode && !KeyLControl.pressed())
            {
                std::transform(Key.begin(), Key.end(), Key.begin(), std::toupper);
                Correct_Key_Check(Key);
            }
            if (Number != -1)
            {
                Lyrics_Time_Now_Romaji_String = Lyrics_Romaji[Number];
                std::transform(Lyrics_Time_Now_Romaji_String.begin(), Lyrics_Time_Now_Romaji_String.end(), Lyrics_Time_Now_Romaji_String.begin(), std::toupper);
                FontAsset(U"Text")(U"残り" + Format(std::round((Progress_Max - Progress_Now) * 100) / 100)).draw(Arg::bottomLeft(575, 145), Palette::Black);
                Lyrics_Number_String(Format(Number + 1) + U"/" + Format(Max_Lyrics)).draw(Arg::bottomLeft(10, 145), Palette::Black);
            }
            else
            {
                Lyrics_Time_Now_Romaji_String = U"";
                if (IsLyricsFirstShow)
                {
                    Progress_Now = 0;
                    Progress_Max = 0;
                    if (IsLyricsLastShow)
                    {
                        FontAsset(U"Text")(U"残り" + Format(std::round((audio.lengthSec() - Time_Seconds) * 100) / 100)).draw(Arg::bottomLeft(575, 145), Palette::Black);
                        Lyrics_Number_String(Format(Last_Lyrics_Number) + U"/" + Format(Max_Lyrics)).draw(Arg::bottomLeft(10, 145), Palette::Black);
                    }
                    else
                    {
                        FontAsset(U"Text")(U"残り" + Format(std::round((Start_Times[Lyrice_Number_Now + 1] - Time_Seconds) * 100) / 100)).draw(Arg::bottomLeft(575, 145), Palette::Black);
                        Lyrics_Number_String(Format(Lyrice_Number_Now + 2) + U"/" + Format(Max_Lyrics)).draw(Arg::bottomLeft(10, 145), Palette::Black);
                    }
                }
                else
                {
                    FontAsset(U"Text")(U"残り" + Format(std::round((Start_Times[0] - Time_Seconds) * 100) / 100)).draw(Arg::bottomLeft(575, 145), Palette::Black);
                    Lyrics_Number_String(Format(0) + U"/" + Format(Max_Lyrics)).draw(Arg::bottomLeft(10, 145), Palette::Black);
                }
            }
            if (Auto_Mode)
            {
                Auto_Speed_Temp += 0.5;
                if (Auto_Speed_Temp >= Auto_Speed)
                {
                    Auto_Speed_Temp = 0;
                    if (Number != -1 && !IsMusicStopped)
                    {
                        Correct_Key_Check(Format(Lyrics_Time_Now_Romaji_String[KeyDownNumber]));
                    }
                }
                else if (Auto_Speed_Temp >= 21)
                {
                    Auto_Speed_Temp = 0;
                }
                FontAsset(U"Text")(U"オートモード:有効").draw(Arg::bottomCenter(1080, 670), Palette::Black);
                SimpleGUI::Slider(Format(std::round(Auto_Speed * 10) / 10), Auto_Speed, 0.5, 5, Vec2(900, 680), 40, 300, true);
            }
            if (IsReplayMode && Number != -1)
            {
                Replay_Time_And_Correct_Reflect();
            }
            SimpleGUI::Slider(Format(std::round(Audio_Volume * 100)), Audio_Volume, Vec2(20, 680), 40, 300, true);
            SimpleGUI::Slider(Format(std::round(Audio_Pitch * 100) / 100), Audio_Pitch, 0, 2, Vec2(10, 650), 50, 300, true);
            SimpleGUI::Slider(U"", Progress_Now, 0, Progress_Max, Vec2(140, 150), 0, 1000, true);
            FontAsset(U"Text")(U"スコア:" + Format(Score)).draw(Arg::bottomCenter(200, 450), Palette::Black);
            FontAsset(U"Text")(U"ミスタッチ:" + Format(MissNumber)).draw(Arg::bottomCenter(400, 450), Palette::Black);
            FontAsset(U"Text")(U"正解数:" + Format(CorrectNumber)).draw(Arg::bottomCenter(600, 450), Palette::Black);
            FontAsset(U"Text")(Lyrics_Time_Now_String).draw(Arg::bottomCenter(640, 225), Palette::Black);
            Vec2 pos(0, 250);
            Vec2 pos2(640, 250);
            Vec2 pos3(640, 250);
            int Number_Temp = 0;
            for (const auto& glyph : Lyrics_Romaji_Font(Lyrics_Time_Now_Romaji_String))
            {
                pos.x += glyph.xAdvance;
            }
            for (const auto& glyph2 : Lyrics_Romaji_Font(Lyrics_Time_Now_Romaji_String))
            {
                pos3.x = pos2.x - pos.x / 2;
                if (Number_Temp <= KeyDownNumber - 1)
                {
                    glyph2.texture.draw(pos3 + glyph2.offset, Palette::Red);
                }
                else
                {
                    glyph2.texture.draw(pos3 + glyph2.offset, Palette::Black);
                }
                pos2.x += glyph2.xAdvance;
                Number_Temp++;
            }
            if (!IsMusicStopped && !audio.isPlaying())
            {
                First_Execute = true;
                Game_Mode = U"リザルト";
            }
            //曲の位置を変更(-5秒、+5秒)
            if (KeyLControl.pressed() && KeyJ.down())
            {
                double Sec = audio.posSec();
                if (Sec >= 5)
                {
                    audio.setPosSec(Sec - 5);
                    if (IsReplayMode)
                    {
                        Replay_Back_or_Next(Sec - 5);
                    }
                }
                else
                {
                    audio.setPosSec(0);
                    if (IsReplayMode)
                    {
                        Replay_Back_or_Next(0);
                    }
                }
            }
            if (KeyLControl.pressed() && KeyK.down())
            {
                audio.setPosSec(Time_Seconds + 5);
                if (IsReplayMode)
                {
                    Replay_Back_or_Next(Time_Seconds + 5);
                }
            }
            if (KeyLControl.pressed() && KeyM.pressed() && KeyS.pressed())
            {
                if (!IsMusicStopped)
                {
                    audio.pause();
                    IsMusicStopped = true;
                }
            }
            if (KeyLControl.pressed() && KeyM.pressed() && KeyP.pressed())
            {
                if (IsMusicStopped)
                {
                    audio.play();
                    IsMusicStopped = false;
                }
            }
            if (!IsReplayMode && KeyLControl.pressed() && KeyA.down())
            {
                if (Auto_Mode)
                {
                    Auto_Mode = false;
                }
                else
                {
                    Auto_Mode = true;
                }
            }
            if (KeyEscape.down())
            {
                IsMusicStopped = false;
                audio.stop();
                Reset();
            }
            audio.setVolume(Audio_Volume);
            audio.setSpeed(Audio_Pitch);
        }
        else if (Game_Mode = U"リザルト")
        {
            if (First_Execute)
            {
                First_Execute = false;
                if (!IsReplayMode && Replay_Record)
                {
                    DateTime Time_D = DateTime::Now();
                    String Time_All = Format(Time_D.year) + U"," + Format(Time_D.month) + U"," + Format(Time_D.day) + U"," + Format(Time_D.hour) + U";" + Format(Time_D.minute) + U";" + Format(Time_D.second);
                    std::ofstream Replay_Write("Resources/Replays/" + Music_Name.narrow() + Time_All.narrow() + ".LTR");
                    for (int Replay_Number = 0; Replay_Number <= (int)Replay_Times.size() - 1; Replay_Number++)
                    {
                        Replay_Write << Replay_Times[Replay_Number] << std::endl;
                    }
                    Replay_Write.close();
                }
                WPM_Calculation = All_Key_Number / Music_All_Seconds * 60 * ((double)CorrectNumber / (double)All_Key_Number);
                Max_WPM = All_Key_Number / Music_All_Seconds * 60;
            }
            FontAsset(U"Text")(U"リザルト").draw(Arg::bottomCenter(650, 50), Palette::Black);
            FontAsset(U"Text")(U"スコア:" + Format(Score)).draw(Arg::bottomCenter(325, 250), Palette::Black);
            FontAsset(U"Text")(U"ミスタッチ:" + Format(MissNumber)).draw(Arg::bottomCenter(650, 250), Palette::Black);
            FontAsset(U"Text")(U"正解数:" + Format(CorrectNumber)).draw(Arg::bottomCenter(975, 250), Palette::Black);
            FontAsset(U"Text")(U"WPM:" + Format(std::round(WPM_Calculation * 100) / 100) + U"(最高" + Format(std::round(Max_WPM * 100) / 100) + U")").draw(Arg::bottomCenter(325, 400), Palette::Black);
            if (SimpleGUI::Button(U"戻る", Vec2(0, 685), 120, true) || KeyEscape.down())
            {
                Reset();
            }
        }
	}
    JSONWriter json;
    json.startObject();
    {
        json.key(U"Setting").startObject();
        {
            json.key(U"HU_or_FU").write(HU_or_FU);
            json.key(U"ZI_or_JI").write(ZI_or_JI);
            json.key(U"X_or_L").write(X_or_L);
            json.key(U"TY_or_CH").write(TY_or_CH);
            json.key(U"SY_or_SH").write(SY_or_SH);
            json.key(U"K_or_C").write(K_or_C);
            json.key(U"TI_or_CHI").write(TI_or_CHI);
            json.key(U"TU_or_TSU").write(TU_or_TSU);
            json.key(U"ZY_or_J").write(ZY_or_J);
            json.key(U"SI_or_SHI").write(SI_or_SHI);
            json.key(U"Audio_Volume").write(Audio_Volume);
            json.key(U"Audio_Pitch").write(Audio_Pitch);
            json.key(U"Replay_Record").write(Replay_Record);
        }
        json.endObject();
    }
    json.endObject();
    json.save(U"Resources/Setting.dat");
}