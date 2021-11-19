#include "playerBase.h"

void  playerBase::setConditionSuu(int a)
{
	condition = new short[a];
}

playerBase::playerBase()
{
	FaceTexture = NULL;
	playerTexture = NULL;
	playerFaceNumberU = 0;
	playerFaceNumberV = 0;
	combat_Stand_By_Flag = true; death_Flag = true;
	chosePhysical = false;
	havingPlayerFlag = false;
	level = 1; HP = 5; MP = 10; SP = 100; OffensivePower = 3; DefensePower = 0; MAX_HP = 5; MAX_MP = 10; MAX_SP = 100; magicDefensePower = 0;
	magicOffensivePower = 0; speed = 3; total_Experience = 0; critical_Rate = 0;
}
playerBase::~playerBase()
{
}
//void playerBase::setMagic(char FileName[])
//{
//	FILE *fp;
//	char c;
//	int textSize = 300;
//	char loacalText[300];
//	char cha[50], cha0[50], cha1[10];
//	int date[10];
//	float dateF[10];
//	int p = 0;
//	int o = 0;
//	int dateNumber = -2;
//	int dateFNumber = 0;
//	bool flag = true;
//	//ファイルオープン
//	if (0 != (errorcode = fopen_s(&fp, FileName, "r")))
//	{
//		return;
//	}
//	for (int i = 0; i < 10; i++)
//	{
//		cha1[i] = NULL;
//	}
//	for (int i = 0; i < 50; i++)
//	{
//		cha[i] = NULL;
//		cha0[i] = NULL;
//	}
//	while ((c = getc(fp)) != EOF)
//	{
//		if (c == '/')
//			flag = false;
//		if (flag == true)
//		{
//			if (c == '\n')
//			{
//				magicDate[p].name = cha0;
//				magicDate[p].attribute = cha1;
//				//intデータ
//				magicDate[p].magicNumber = date[0];
//				magicDate[p].costMP = date[1];
//				magicDate[p].recoveryHP = date[2];
//				magicDate[p].recoverySP = date[3];
//				magicDate[p].continuatianTrun = date[4];
//				if (0 >= date[5])magicDate[p].targetPlayerFlag = false;
//				else if (1 <= date[5])magicDate[p].targetPlayerFlag = true;
//				magicDate[p].targetPeople = date[6];
//				//floatデータ
//				magicDate[p].coefficientMagicOffensive = dateF[0];
//				magicDate[p].coefficientMagicDefense = dateF[1];
//				magicDate[p].coefficientOffensive = dateF[2];
//				magicDate[p].coefficientDefense = dateF[3];
//				magicDate[p].coefficientSpeed = dateF[4];
//				p++;
//				dateNumber = -2;
//				dateFNumber = 0;
//				o = 0;
//				for (int i = 0; i < 10; i++)
//				{
//					cha1[i] = NULL;
//					date[i] = NULL;
//					dateF[i] = NULL;
//				}
//				for (int i = 0; i < 50; i++)
//				{
//					cha[i] = NULL;
//					cha0[i] = NULL;
//				}
//			}
//			else if (c == ',')
//			{
//				string str = cha;
//				if (dateNumber == -1)
//				{
//					char_traits<char>::copy(cha0, str.c_str(), str.size() + 1);
//					dateNumber++;
//				}
//				else if (dateNumber == -2)
//				{
//					char_traits<char>::copy(cha1, str.c_str(), str.size() + 1);
//					dateNumber++;
//				}
//				else if (dateNumber < 7)
//				{
//					date[dateNumber] = atoi(str.c_str());
//					dateNumber++;
//				}
//				else if (dateNumber > 6)
//				{
//					dateF[dateFNumber] = stof(str.c_str());
//					dateFNumber++;
//				}
//				for (int i = 0; i < 50; i++)
//				{
//					cha[i] = NULL;
//				}
//				o = 0;
//			}
//			else
//			{
//				cha[o] = c;
//				o++;
//			}
//		}
//		else if (c == '\n')
//		{
//			flag = true;
//		}
//	}
//	//skillDate[p].name = cha0;
//	////intデータ
//	//skillDate[p].skillNumber = date[0];
//	//skillDate[p].costSP = date[1];
//	//skillDate[p].recoveryHP = date[2];
//	//skillDate[p].recoveryMP = date[3];
//	//skillDate[p].continuatianTrun = date[4];
//	//if (0 >= date[5])skillDate[p].targetPlayerFlag = false;
//	//else if (1 <= date[5])skillDate[p].targetPlayerFlag = true;
//	//skillDate[p].targetPeople = date[6];
//	////floatデータ
//	//skillDate[p].coefficientOffensive = dateF[0];
//	//skillDate[p].coefficientDefense = dateF[1];
//	//skillDate[p].coefficientMagicOffensive = dateF[2];
//	//skillDate[p].coefficientMagicDefense = dateF[3];
//	//skillDate[p].coefficientSpeed = dateF[4];
//}
void playerBase::setEquipment(char FileName[])
{
}
bool playerBase::playerLoad(IDirect3DDevice9* pDevice3D, const char* textureName)
{
	// 画像読み込み
	// DirextXやWindowsAPIの関数はHRESULTを結果に返す関数が多い
	// FAILEDマクロで関数が失敗したかわかる
	// SUCEEDEDマクロで関数が成功したかわかる
	if (FAILED(D3DXCreateTextureFromFile(pDevice3D, textureName, &playerTexture)))
		return false;	// 画像読み込み失敗（ファイルがない可能性あり）
	// 画像読み込み成功
	return true;
}
bool playerBase::FaceLoad(IDirect3DDevice9 * pDevice3D, const char * textureName)
{
	// 画像読み込み
	// DirextXやWindowsAPIの関数はHRESULTを結果に返す関数が多い
	// FAILEDマクロで関数が失敗したかわかる
	// SUCEEDEDマクロで関数が成功したかわかる
	if (FAILED(D3DXCreateTextureFromFile(pDevice3D, textureName, &FaceTexture)))
		return false;	// 画像読み込み失敗（ファイルがない可能性あり）
	// 画像読み込み成功
	return true;
}
