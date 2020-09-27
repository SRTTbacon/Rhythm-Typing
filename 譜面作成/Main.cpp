
#include <Siv3D.hpp>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <direct.h>
#include "Function.h"

Array<double> Start_Times = {};
Array<double> End_Times = {};
Array<String> Lyrics = {};
Array<String> Lyrics_Romaji = {};
String Select_String = U"";
int List_Max_Index = -1;
int Select_Index = -1;
int Select_Index_Now = -1;
bool IsEditMode = false;
//時間が被っているかを確認(被っていない = true, 被っている = falseを返す)
bool Times_Confirmation(double Start_Time, double End_Time)
{
	if (Start_Times.size() != size_t(0) && End_Times.size() != size_t(0))
	{
		bool Number_Break = false;
		//まず開始時間がどこかと被っていないか確認
		for (int Number = 0; Number <= (int)Start_Times.size() - 1; Number++)
		{
			if (Start_Times[Number] <= Start_Time && End_Times[Number] >= Start_Time)
			{
				if (IsEditMode == false && Number != Select_Index)
				{
					Number_Break = true;
					break;
				}
			}
		}
		//開始時間が被っていない場合終了時間が被っていないか確認
		for (int Number2 = 0; Number2 <= (int)Start_Times.size() - 1; Number2++)
		{
			if (Start_Times[Number2] <= End_Time && End_Times[Number2] >= End_Time)
			{
				if (IsEditMode == false && Number2 != Select_Index)
				{
					Number_Break = true;
					break;
				}
			}
		}
		//Number_Breakがtrueなら被っていて、falseなら被っていない
		if (Number_Break)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}
}
//歌詞のみを取得
String Get_Lyrics_Only(int List_Number)
{
	if (List_Number != -1)
	{
		String Temp_00 = Lyrics[List_Number];
		String Temp = Temp_00.replace(Temp_00.substr(Temp_00.indexOf(U'|') - 2), U"");
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
		for (int Number = 0; Number <= (int)Start_Times.size() - 1; Number++)
		{
			if (Start_Times[Number] <= Time_Now && End_Times[Number] >= Time_Now)
			{
				Return_Number = Number;
				break;
			}
		}
		if (Return_Number == -1)
		{
			return -1;
		}
		return Return_Number;
	}
	else
	{
		return -1;
	}
}
//選択されている歌詞を変更
void Select_Music_Up()
{
	if (Select_Index > 0)
	{
	    Select_Index--;
		Select_String = Get_Lyrics_Only(Select_Index);
	}
	else
	{
		Select_Index = -1;
	    Select_String = U"";
	}
}
void Select_Music_Down()
{
	if (Select_Index < List_Max_Index)
	{
		Select_Index++;
		Select_String = Get_Lyrics_Only(Select_Index);
	}
}
void Main()
{
	bool Audio_Selected = false;
	Graphics::SetTargetFrameRateHz(60);
	Window::SetTitle(U"リズムタイピング");
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);
	Window::Resize(Size(1280, 720));
	Font Lyrics_Print(20);
	Font Lyrics_Length(25);
	Font Lyrics_Now_String(25, Typeface::Bold);
	Font Lyrics_Romaji_String_Font(25);
	Font Lyrics_Selected_String(25);
	String Lyrics_Print_String = U"歌詞:\n";
	String Lyrics_Time_Now_String = U"";
	double Lyrics_Print_Height = 0;
	double Audio_Volume = 0.5;
	double Audio_Pitch = 1;
	bool IsLyrics_Time_Text_Clicked = false;
	bool End_Time_Mode = false;
	bool IsStopped_Slider_Move = false;
	bool IsSlider_Clicked = false;
	bool hasChanged = false;
	String Audio_File_Path = U"";
	TextEditState Lyrics_Start_Time_Text(U"0");
	TextEditState Lyrics_End_Time_Text(U"0");
	TextEditState Lyrics_Text;
	TextEditState Save_File_Name(U"ファイル名");
	TextEditState Lyrics_Romaji_String(U"");
	String Lyrics_Start_Time_Text_Before = U"";
	String Lyrics_End_Time_Text_Before = U"";
	String Lyrics_Text_Before = U"";
	String Lyrics_Romaji_String_Before = U"";
	Audio audio;
	while (System::Update())
	{
		const String time = FormatTime(SecondsF(audio.posSec()), U"M:ss") + U'/' + FormatTime(SecondsF(audio.lengthSec()), U"M:ss");
		const double Time_Seconds = audio.posSec();
		double progress = static_cast<double>(audio.posSample()) / audio.samples();
		//曲が設定されていないときに実行
		if (!Audio_Selected)
		{
			//新規作成
			if (SimpleGUI::Button(U"曲を選ぶドン！", Vec2(500, 300), 250, true))
			{
				Array<FileFilter> Filter = { FileFilter::AllAudioFiles() };
				Optional<String> File = Dialog::OpenFile(Filter, U"", U"オーディオファイルを開く");
				if (File.has_value())
				{
					audio = Audio(File.value());
					if (!audio.isEmpty())
					{
						Audio_Selected = true;
						Audio_File_Path = File.value();
						audio.stop();
					}
				}
			}
			//ロード(.LTSファイルのみだが、実際は.zipと変わらない)
			if (SimpleGUI::Button(U"ロード", Vec2(500, 350), 250, true))
			{
				Array<FileFilter> Filter = { FileFilter::AllFiles() };
				Optional<String> File = Dialog::OpenFile(Filter, FileSystem::CurrentDirectory() + U"Save/", U".LTSファイルを読み込む");
				if (File.has_value())
				{
					if (Get_File_Extension(File.value()) == U"LTS")
					{
						String Write_Moji = U"7z.exe x -aoa -oExtract " + File.value();
						std::ofstream ifs("Zip_Decode.bat");
						ifs << Write_Moji << std::endl;
						ifs.close();
						system("Zip_Decode.bat");
						System::Sleep(2s);
						remove("Zip_Decode.bat");
						audio = Audio(U"Extract/Save_Music.dat");
						Audio_File_Path = FileSystem::CurrentDirectory() + U"Extract/Save_Music.dat";
						Audio_Selected = true;
						audio.stop();
						std::ifstream ifs3("Extract/Save_Lyrics.dat");
						std::string str;
						while (std::getline(ifs3, str))
						{
							Lyrics.push_back(Unicode::Widen(str));
							Lyrics_Print_String += Unicode::Widen(str).replace(U"/", U"～") + U"\n";
							Start_Times.push_back(std::stod(Get_Text_Start_Position(Unicode::Widen(str)).narrow()));
							End_Times.push_back(std::stod(Get_Text_End_Position(Unicode::Widen(str)).narrow()));
							List_Max_Index++;
						}
						ifs3.close();
						std::ifstream ifs4("Extract/Save_Lyrics_Romaji.dat");
						std::string str2;
						while (std::getline(ifs4, str2))
						{
							Lyrics_Romaji.push_back(Unicode::Widen(str2));
						}
						ifs4.close();
						Save_File_Name.text = Get_File_Name_Only(File.value());
					}
					else
					{
						System::ShowMessageBox(U"このファイルは専用のセーブデータではありません。\n.LTSファイルを選択してください。");
					}
				}
			}
		}
		//曲が選択されているときに実行
		if (Audio_Selected)
		{
			//文字を描画
			Lyrics_Time_Now_String = Get_Lyrics_Only(Get_Time_Lyrics_Now(Time_Seconds));
			Lyrics_Print(Lyrics_Print_String).draw(0, Lyrics_Print_Height, Palette::Aqua);
			Lyrics_Now_String(Lyrics_Time_Now_String).draw(Arg::bottomCenter(650, 650), Palette::White);
			Lyrics_Romaji_String_Font(U"＊ローマ字＊").draw(Arg::bottomCenter(625, 285), Palette::Aqua);
			Lyrics_Selected_String(Select_String).draw(Arg::bottomCenter(1150, 50), Palette::Aqua);
			Lyrics_Length(U"～").draw(Vec2(567, 150), Palette::Aqua);
			//再生ボタン
			if (IsEditMode == false && SimpleGUI::Button(U"再生", Vec2(450, 360), 150, !audio.isPlaying()))
			{
				Lyrics_Start_Time_Text.active = false;
				Lyrics_End_Time_Text.active = false;
				IsLyrics_Time_Text_Clicked = false;
				audio.play(0.2s);
			}
			//停止ボタン
			if (IsEditMode == false && SimpleGUI::Button(U"一時停止", Vec2(630, 360), 150, audio.isPlaying()))
			{
				End_Time_Mode = false;
				audio.pause();
			}
			//終了時間を指定するボタン(再生ボタンは開始時間を指定する)
			if (IsEditMode == false && SimpleGUI::Button(U"終了時間指定", Vec2(810, 360), 150, !audio.isPlaying()))
			{
				End_Time_Mode = true;
				audio.play(0.2s);
			}
			//テキストボックスの描画
			SimpleGUI::TextBox(Lyrics_Text, Vec2(400, 200), 475);
			SimpleGUI::TextBox(Lyrics_Start_Time_Text, Vec2(410, 150), 150, 11);
			SimpleGUI::TextBox(Lyrics_End_Time_Text, Vec2(600, 150), 150, 11);
			SimpleGUI::TextBox(Lyrics_Romaji_String, Vec2(400, 300), 475);
			IsEditMode == false && SimpleGUI::TextBox(Save_File_Name, Vec2(1100, 600), 150);
			//歌詞を追加(正しく入力されていない場合エラーメッセージを出す)
			if (SimpleGUI::Button(U"挿入", Vec2(900, 200), 120, true))
			{
				if (Check_int(Lyrics_Start_Time_Text.text) && Check_int(Lyrics_End_Time_Text.text))
				{
					if (Lyrics_Romaji_String.text != U"")
					{
						String Romaji_Text_Ex = Lyrics_Romaji_String.text.replace(U"[", U"").replace(U"]", U"");
						if (std::isalpha(static_cast<unsigned char>(*Romaji_Text_Ex.narrow().c_str())))
						{
							double Start = Parse<double>(Lyrics_Start_Time_Text.text);
							double End = Parse<double>(Lyrics_End_Time_Text.text);
							if (Lyrics_Text.text != U"" && Start < End && Lyrics_Start_Time_Text.text != U"" && Lyrics_End_Time_Text.text != U"")
							{
								if (Times_Confirmation(Start, End))
								{
									if (IsEditMode == false)
									{
										Lyrics.push_back(Lyrics_Text.text + U"  |  " + ToString(Start) + U" / " + ToString(End));
										Lyrics_Print_String += Lyrics_Text.text + U"  |  " + ToString(Start) + U" ～ " + ToString(End) + U"\n";
										Lyrics_Romaji.push_back(Lyrics_Romaji_String.text);
										Start_Times.push_back(Start);
										End_Times.push_back(End);
										Lyrics_Text.text.clear();
										Lyrics_Romaji_String.text.clear();
										Lyrics_Start_Time_Text.text = ToString(End + 0.01);
										Lyrics_Text.active = false;
										List_Max_Index++;
									}
									else
									{
										Lyrics[Select_Index] = Lyrics_Text.text + U"  |  " + ToString(Start) + U" / " + ToString(End);
										Array<String> Temp = Lyrics_Print_String.split('\n');
										Lyrics_Print_String = U"";
										for (int Number = 0; Number <= (int)Temp.size() - 1; Number++)
										{
											if (Number == Select_Index + 1)
											{
												Lyrics_Print_String += Lyrics_Text.text + U"  |  " + ToString(Start) + U" ～ " + ToString(End) + U"\n";
											}
											else
											{
												Lyrics_Print_String += Temp[Number] + U"\n";
											}
										}
										Lyrics_Romaji[Select_Index] = Lyrics_Romaji_String.text;
										Start_Times[Select_Index] = Start;
										End_Times[Select_Index] = End;
										Lyrics_Text.active = false;
									}
								}
								else
								{
									System::ShowMessageBox(U"時間が被っている箇所があります。", MessageBoxButtons::OK);
								}
							}
							else
							{
								System::ShowMessageBox(U"正しく入力されていません。", MessageBoxButtons::OK);
							}
						}
						else
						{
							System::ShowMessageBox(U"ローマ字以外の文字が入力されています。");
						}
					}
					else
					{
						System::ShowMessageBox(U"ローマ字が入力されていません。");
					}
				}
				else
				{
					System::ShowMessageBox(U"数字以外が入力されています。", MessageBoxButtons::OK);
				}
			}
			//編集ボタン(選択されている歌詞を編集できる)
			if (IsEditMode == false && SimpleGUI::Button(U"編集", Vec2(1100, 50), 150, true))
			{
				if (Select_Index != -1)
				{
					Lyrics_Start_Time_Text_Before = Lyrics_Start_Time_Text.text;
					Lyrics_End_Time_Text_Before = Lyrics_End_Time_Text.text;
					Lyrics_Text_Before = Lyrics_Text.text;
					Lyrics_Romaji_String_Before = Lyrics_Romaji_String.text;
					IsEditMode = true;
					Lyrics_Start_Time_Text.text = Get_Text_Start_Position(Lyrics[Select_Index]);
					Lyrics_End_Time_Text.text = Get_Text_End_Position(Lyrics[Select_Index]);
					Lyrics_Text.text = Get_Lyrics_Only(Select_Index);
					Lyrics_Romaji_String.text = Lyrics_Romaji[Select_Index];
				}
			}
			//キャンセルボタン(編集ボタンが押されている場合表示)
			if (IsEditMode == true && SimpleGUI::Button(U"キャンセル", Vec2(900, 240), 120, true))
			{
				IsEditMode = false;
				Lyrics_Start_Time_Text.text = Lyrics_Start_Time_Text_Before;
				Lyrics_End_Time_Text.text = Lyrics_End_Time_Text_Before;
				Lyrics_Text.text = Lyrics_Text_Before;
				Lyrics_Romaji_String.text = Lyrics_Romaji_String_Before;
			}
			//セーブボタン(ファイル名が入力されていない場合エラー文を表示させる)
			if (IsEditMode == false && SimpleGUI::Button(U"セーブ", Vec2(1100, 650), 150, true))
			{
				if (Lyrics.size() != size_t(0))
				{
					if (Save_File_Name.text != U"" || Save_File_Name.text == U"ファイル名")
					{
						MessageBoxSelection result = System::ShowMessageBox(U"セーブしますか？\nファイル名:" + Save_File_Name.text + U".LTS", MessageBoxButtons::YesNo);
						if (result == MessageBoxSelection::Yes)
						{
							std::ofstream ifs("Save_Lyrics.dat");
							for (int Number = 0; Number <= (int)Lyrics.size() - 1; Number++)
							{
								ifs << Lyrics[Number] << std::endl;
							}
							ifs.close();
							std::ofstream ifs2("Save_Lyrics_Romaji.dat");
							for (int Number = 0; Number <= (int)Lyrics_Romaji.size() - 1; Number++)
							{
								ifs2 << Lyrics_Romaji[Number] << std::endl;
							}
							ifs2.close();
							FileSystem::Copy(Audio_File_Path, U"Save_Music.dat", CopyOption::OverwriteExisting);
							std::ofstream ifs3("Zip_Create.bat");
							String Write_Moji = U"7z.exe a Save/" + Save_File_Name.text + U".LTS Save_Lyrics.dat Save_Lyrics_Romaji.dat Save_Music.dat";
							ifs3 << Write_Moji << std::endl;
							ifs3.close();
							system("Zip_Create.bat");
							System::Sleep(2s);
							remove("Save_Lyrics.dat");
							remove("Save_Lyrics_Romaji.dat");
							remove("Save_Music.dat");
							remove("Zip_Create.bat");
							//セーブフォルダを開く
							MessageBoxSelection result2 = System::ShowMessageBox(U"セーブしました。フォルダを開きますか？", MessageBoxButtons::YesNo);
							if (result2 == MessageBoxSelection::Yes)
							{
								String aa = U"explorer \"" + FileSystem::CurrentDirectory() + U"Save\"";
								System::ShowMessageBox(aa.replace(U"/", U"\\"), MessageBoxButtons::OK);
								system(aa.replace(U"/", U"\\").narrow().c_str());
							}
						}
					}
					else
					{
						System::ShowMessageBox(U"ファイル名を設定してください。", MessageBoxButtons::OK);
					}
				}
				else
				{
					System::ShowMessageBox(U"歌詞が存在しません。", MessageBoxButtons::OK);
				}
			}
			//再生位置を指定するスライダー
			if (IsEditMode == false && SimpleGUI::Slider(time, progress, Vec2(350, 500), 130, 560, !audio.isEmpty()))
			{
				if (!audio.isPlaying() && IsSlider_Clicked == false)
				{
					IsStopped_Slider_Move = true;
				}
				IsSlider_Clicked = true;
				audio.pause();
				audio.setPosSample(static_cast<int64>(audio.samples() * progress));
				hasChanged = true;
			}
			else if (hasChanged && MouseL.up())
			{
				if (!IsStopped_Slider_Move)
				{
					audio.play(0.2s);
					hasChanged = false;
					Lyrics_Start_Time_Text.active = false;
					Lyrics_End_Time_Text.active = false;
					IsLyrics_Time_Text_Clicked = false;
				}
				IsSlider_Clicked = false;
				IsStopped_Slider_Move = false;
			}

			//ピッチ、音量の指定
			IsEditMode == false && SimpleGUI::Slider(Format(std::round(Audio_Volume * 100)), Audio_Volume, Vec2(700, 465), 40, 300, true);
			IsEditMode == false && SimpleGUI::Slider(Format(std::round(Audio_Pitch * 100) / 100), Audio_Pitch, 0, 2, Vec2(350, 465), 50, 300, true);
			//開始位置、終了位置を更新
			if (Lyrics_Print_Height <= 0 && Lyrics.size() >= size_t(15))
			{
				if (Lyrics_Print_Height <= 0 && Mouse::Wheel() > 0)
				{
					Lyrics_Print_Height -= Mouse::Wheel() * 20;
				}
				else if (Mouse::Wheel() < 0 && Lyrics_Print_Height != 0)
				{
					Lyrics_Print_Height -= Mouse::Wheel() * 20;
				}
			}
			else if (Lyrics_Print_Height > 0)
			{
				Lyrics_Print_Height = 0;
			}
			//歌詞を選択(上下キー)
			if (IsEditMode == false && KeyUp.pressed() && KeyLControl.pressed())
			{
				Select_Music_Up();
			}
			if (IsEditMode == false && KeyUp.down())
			{
				Select_Music_Up();
			}
			if (IsEditMode == false && KeyDown.pressed() && KeyLControl.pressed())
			{
				Select_Music_Down();
			}
			if (IsEditMode == false && KeyDown.down())
			{
				Select_Music_Down();
			}

		}
		if (IsEditMode == false && !Lyrics_End_Time_Text.active && !Lyrics_Start_Time_Text.active && !Lyrics_Text.active && !Lyrics_Romaji_String.active && !IsLyrics_Time_Text_Clicked || End_Time_Mode)
		{
			if (End_Time_Mode && audio.isPlaying())
			{
				Lyrics_End_Time_Text.text = ToString(Time_Seconds);
			}
			else if (audio.isPlaying())
			{
				Lyrics_Start_Time_Text.text = ToString(Time_Seconds);
			}
		}
		else
		{
			IsLyrics_Time_Text_Clicked = true;
			if (audio.isPlaying())
			{
				audio.pause();
			}
		}
		//曲の位置を変更(-5秒、+5秒)
		if (IsEditMode == false && !Lyrics_Text.active && !Lyrics_Romaji_String.active && KeyLControl.pressed() && KeyJ.down())
		{
			double Sec = audio.posSec();
			if (Sec >= 10)
			{
				audio.setPosSec(Sec - 10);
			}
			else
			{
				audio.setPosSec(0);
			}
		}
		else if (IsEditMode == false && !Lyrics_Text.active && !Lyrics_Romaji_String.active && KeyJ.down())
		{
			double Sec = audio.posSec();
			if (Sec >= 5)
			{
				audio.setPosSec(Sec - 5);
			}
			else
			{
				audio.setPosSec(0);
			}
		}
		if (IsEditMode == false && !Lyrics_Text.active && !Lyrics_Romaji_String.active && KeyLControl.pressed() && KeyK.down())
		{
			audio.setPosSec(audio.posSec() + 10);
		}
		else if (IsEditMode == false && !Lyrics_Text.active && !Lyrics_Romaji_String.active && KeyK.down())
		{
			audio.setPosSec(audio.posSec() + 5);
		}
		//ピッチを下げる
		if (KeyC.pressed() && KeyO.pressed())
		{
			Audio_Pitch -= 0.004;
		}
		//ピッチを上げる
		if (KeyC.pressed() && KeyP.pressed())
		{
			Audio_Pitch += 0.004;
		}
		//音量を下げる
		if (KeyV.pressed() && KeyO.pressed() && Audio_Volume >= 0)
		{
			Audio_Volume -= 0.0025;
		}
		//音量を上げる
		if (KeyV.pressed() && KeyP.pressed() && Audio_Volume <= 1)
		{
			Audio_Volume += 0.0025;
		}
		//ピッチをリセット
		if (KeyP.pressed() && KeyR.pressed())
		{
			Audio_Pitch = 1;
		}
		//ピッチ、音量を適応
		audio.setVolume(Audio_Volume);
		audio.setSpeed(Audio_Pitch);
	}
	//ソフトが閉じた場合一時ファイルを削除
	File_Directory_Delete(U"Zip_Create.bat");
	File_Directory_Delete(U"Zip_Extract.bat");
	File_Directory_Delete(U"Extract");
}