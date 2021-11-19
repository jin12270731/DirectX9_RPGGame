#include <Windows.h>
#include <tchar.h>
#include <string.h>
#include <stdlib.h>
#include <random>
// 作ったヘッダーファイルを全部インクルード
#include "direct.h"
#include "texture.h"
#include "sprite.h"
#include "directsound.h"
#include "wave.h"
#include "soundbuffer.h"
#include "DirectText.h"
#include "enemy.h"
#include "player.h"
#include "directMouse.h"
#include "iniClass.h"
using namespace std;
//ディスプレイのサイズ
int DEFALT_WIDTH;
int DEFALT_HEIGHT;
//アイテムなどの数を定数化
const short itemsSuu = 999;
const short skillSuu = 999;
const short magicSuu = 999;
//エラーコードの置き場所
errno_t errorCode;
//テキストを描画する
TCHAR TextDraw[500];
TCHAR havingTextDraw[1000];
//
char s[30];
//デバッグ用のテキストを表示する
TCHAR msg1[8];
TCHAR msg2[8];
TCHAR msg3[8];
TCHAR msg4[8];
TCHAR msg5[8];
TCHAR msg6[8];
TCHAR msg7[];
//テキストを読み込む種類を設定する場所
char textNumberA = '0';
char textNumberB = '0';
//戦闘時のテキストを表示させる場所
char CombattingTextDate[500];
//戦闘時の敵の数をランダムで設定
short enemysuu = 1;
//現在のマップ
int nowMapNumber = 1;
//フラグ
bool combatLog = false;
bool playerMoveFlag = true;
bool textflag = false;
bool haveThingText = false;
bool timeFlag = true;
bool haveSkillText = false;
bool haveMagicText = false;
//以前の時間と現在の時間を格納
DWORD nowTime, nowTime2;
DWORD startTime, startTime2;
DWORD keika, keika2;
//以前のターンを格納
short startTrun;
//マウスの位置の取得
POINT mousePoint;
char c;
char cha[20];
string str;
//マウスホイールの回転量
short mouseHoile = 0;
//非操作プレイヤーの作成
playerBase npc[5];
//
int frontDisplayItem = 0;
//クリックしたプレイヤー
short ClickPlayer = 0;
//プレイヤーを選択する回数
short chosePlayerKaisuu = 0;
//敵を選択する回数
short choseEnemyKaisuu = 0;
//戦う敵のナンバー
short In_Combat_Enemy;
//ランダムの値
int random;
//マウスの位置
int localMousePointX;
int localMouePointY;
//マップが入れ替わるときの座標
int mapChangePoint[10][3];
errno_t errorcode;
//戦闘用の敵情報のメモリ
enemy* combatEnemys;
//マップで描画する専用のメモリ確保
enemy*  enemys;
//マップデータの指定
char mapChipTXT1[] = "MapDate/mapChipDate1.txt";
char backMapChipTXT1[] = "MapDate/backMapChipDate1.txt";
char mapChipTXT2[] = "MapDate/backMapChipDate1.txt";
//データファイルの指定
char CombatTextDate[] = "TextDate/CombatTextDate.txt";
//マップデータの情報
int mapStates[10];
//戦闘時のターン
short combatTurn = 1;
//ターゲットフラグ
bool targetEnemyOrPlayer;//trueで敵、falseプレイヤー
bool targetMultipleOrOnece;//trueで複数、false単数
bool attactFlag;//
//敵とプレイヤーを選択するかどうかのフラグ
bool chosePayerFlag;
bool choseFlag;
//戦闘の順番
short jyun = 0;
//戦闘が終わるときの総経験値
short Combat_Total_Experience = 0;
//
bool combatStanByFlag;
//iniファイルのディレクトリの指定
iniClass ini;
//iniファイルに入っているデータの代入
auto setIniDate(int target, string str)
{
	target = ini.getInt(str);
	if (-1 == target)
	{
		target = 0;
		return target;
	}
	return target;
}
//決定した実行順を保存する構造体
struct selectSpeed
{
	int speed, Number;
	short combatOrder, enemy_NPC_Player;//０で敵、１でNPC、２でプレイヤー
};
//スピードを降順にする
selectSpeed* down(selectSpeed* suu, int player)
{
	for (int i = 0; i < player; i++)
	{
		for (int j = i + 1; j < player; j++)
		{
			if (suu[i].speed < suu[j].speed)
			{
				selectSpeed p = suu[i];
				suu[i] = suu[j];
				suu[j] = p;
			}
		}
	}
	return suu;
}
//順番を降順にする
selectSpeed* up(selectSpeed* suu, int player)
{
	for (int i = 0; i < player; i++)
	{
		for (int j = i + 1; j < player; j++)
		{
			if (suu[i].combatOrder > suu[j].combatOrder)
			{
				selectSpeed p = suu[i];
				suu[i] = suu[j];
				suu[j] = p;
			}
		}
	}
	return suu;
}
//順番を決定する
selectSpeed* decisionTurn(selectSpeed* suu, int player)
{
	random_device rand_dev;
	int jyun = 1;
	int guu = player % 2;
	for (int i = 0; i < player; i += 2)
	{
		if (player <= i + 1)
		{
			break;
		}
		float kei = (float)suu[i].speed / suu[i + 1].speed;
		if (kei >= 1 && kei < 1.5)
		{
			int random = rand_dev() % 100;
			if (50 > random)
			{
				suu[i].combatOrder = jyun;
				jyun++;
				suu[i + 1].combatOrder = jyun;
				jyun++;
			}
			else
			{
				suu[i + 1].combatOrder = jyun;
				jyun++;
				suu[i].combatOrder = jyun;
				jyun++;
			}
		}
		else if (kei >= 1.5 && kei < 2)
		{
			int random = rand() % 100;
			if (75 > random)
			{
				suu[i].combatOrder = jyun;
				jyun++;
				suu[i + 1].combatOrder = jyun;
				jyun++;
			}
			else
			{
				suu[i + 1].combatOrder = jyun;
				jyun++;
				suu[i].combatOrder = jyun;
				jyun++;
			}
		}
		else
		{
			suu[i].combatOrder = jyun;
			jyun++;
			suu[i + 1].combatOrder = jyun;
			jyun++;
		}
	}
	if (guu == 1)
	{
		suu[player - 1].combatOrder = jyun;
		return suu;
	}
}
//魔法情報
struct magic
{
public:
	bool havingFlag, targetPlayerFlag;
	string attribute;
	string name;
	short magicNumber, targetPeople;
	int costMP, recoveryHP, recoverySP, continuatianTrun, costMagicPoint;
	float  coefficientOffensive, coefficientDefense, coefficientMagicOffensive, coefficientMagicDefense, coefficientSpeed;

}magicDate[999];
//スキル情報
struct skill
{
public:
	//係数倍の対象を選ぶ。falseで味方。trueで敵
	bool targetEnemyOrPlayerFlag;
	//攻撃するかどうか
	bool attackFlag;
	//係数倍の対象が自分かどうか。０で自分true、１でそれ以外false
	bool targetMyselfOrOtherFlag;
	//ターン継続時に動けるかどうか。０で動けるtrue、１で動けないfalse
	bool movingFlag;
	string name, conditionName;
	short skillNumber, targetPeople, costSkillPoint, keikaTurn, continuatianTrun, conditionNumber, conditionCriticalHit;
	int costSP, recoveryHP, recoveryMP;
	double coefficientOffensive, coefficientDefense, coefficientMagicOffensive, coefficientMagicDefense, coefficientSpeed;
	void changePlayer(playerBase* targetPlay);
	void restorePlayer(playerBase* targetPlay);
	void changeEnemy(enemy* targetEnemy);
	void restoreEnemy(enemy* targetEnemy);
	void execution(playerBase * play, playerBase * target, int choEne);
}skillDate[999];
//アイテム情報
struct item
{
public:
	bool havingFlag;
	string name;
	short itemsNumber;
	int changeHP, changeMP, changeSP, changeOffensivePower, changemagicOffensivePower, changeDefensePower, changemagicDefensePower, changeSpeed;
	void Change(playerBase* play);
}itemsdate[999];
//状態異常の情報
struct condition
{
public:


} conditionDate[30];
//アイテム情報のセット
void setItems()
{
	FILE *fp = NULL;
	char c;
	int textSize = 300;
	char loacalText[300];
	char cha[50], cha0[50];
	int date[10] = {0};
	int p = 0;
	int o = 0;
	int dateNumber = -1;
	bool flag = true;
	//ファイルオープン
	if (0 != (errorcode = fopen_s(&fp, "OtherDate/アイテムデータ/itemDate.txt", "r")))
	{
		return;
	}
	for (int i = 0; i < 50; i++)
	{
		cha[i] = NULL;
		cha0[i] = NULL;
	}
	while ((c = getc(fp)) != EOF)
	{
		if (c == '/')
			flag = false;
		if (flag == true)
		{
			if (c == '\n')
			{
				itemsdate[p].name = cha0;
				itemsdate[p].itemsNumber = date[0];
				itemsdate[p].changeHP = date[1];
				itemsdate[p].changeMP = date[2];
				itemsdate[p].changeSP = date[3];
				itemsdate[p].changeOffensivePower = date[4];
				itemsdate[p].changemagicOffensivePower = date[5];
				itemsdate[p].changeDefensePower = date[6];
				itemsdate[p].changemagicDefensePower = date[7];
				itemsdate[p].changeSpeed = date[9];
				p++;
				dateNumber = -1;
				o = 0;
				for (int i = 0; i < 10; i++)
				{
					date[i] = NULL;
				}
				for (int i = 0; i < 50; i++)
				{
					cha[i] = NULL;
					cha0[i] = NULL;
				}
			}
			else if (c == ',')
			{
				string str = cha;
				if (dateNumber == -1)
				{
					char_traits<char>::copy(cha0, str.c_str(), str.size() + 1);
					dateNumber = 0;
				}
				else
				{
					date[dateNumber] = atoi(str.c_str());
					dateNumber++;
				}
				for (int i = 0; i < 50; i++)
				{
					cha[i] = NULL;
				}
				o = 0;
			}
			else
			{
				cha[o] = c;
				o++;
			}
		}
		else if (c == '\n')
		{
			flag = true;
		}
	}
	itemsdate[p].name = cha0;
	itemsdate[p].itemsNumber = date[0];
	itemsdate[p].changeHP = date[1];
	itemsdate[p].changeMP = date[2];
	itemsdate[p].changeSP = date[3];
	itemsdate[p].changeOffensivePower = date[4];
	itemsdate[p].changemagicOffensivePower = date[5];
	itemsdate[p].changeDefensePower = date[6];
	itemsdate[p].changemagicDefensePower = date[7];
	itemsdate[p].changeSpeed = date[9];
}
//スキル情報のセット
void setSkill() 
{
	FILE *fp;
	char c;
	int textSize = 300;
	char loacalText[300];
	char cha[50], cha0[50] ,cha1[50];
	int date[15];
	double dateF[10];
	int p = 0;
	int o = 0;
	int dateNumber = -2;
	int dateFNumber = 0;
	bool flag = true;
	//ファイルオープン
	if (0 != (errorcode = fopen_s(&fp, "OtherDate/スキルデータ/combatSkillDate.txt", "r")))
	{
		return;
	}
	for (int i = 0; i < 50; i++)
	{
		cha[i] = NULL;
		cha0[i] = NULL;
		cha1[i] = NULL;
	}
	while ((c = getc(fp)) != EOF)
	{
		if (c == '/')
			flag = false;
		if (flag == true)
		{
			if (c == '\n')
			{
				if (p == 5)
				{
					int g = 4;
				}
				skillDate[p].name = cha0;
				skillDate[p].conditionName = cha1;
				//intデータ
				skillDate[p].skillNumber = date[0];
				skillDate[p].costSP = date[1];
				skillDate[p].recoveryHP = date[2];
				skillDate[p].recoveryMP = date[3];
				skillDate[p].continuatianTrun = date[4];
				if (0 >= date[5])
					skillDate[p].targetMyselfOrOtherFlag = true;
				else if (1 <= date[5])
					skillDate[p].targetMyselfOrOtherFlag = false;
				if (0 >= date[6])
					skillDate[p].targetEnemyOrPlayerFlag = false;
				else if (1 <= date[6])
					skillDate[p].targetEnemyOrPlayerFlag = true;
				if (0 >= date[7])skillDate[p].attackFlag = true;
				else if (1 <= date[7])skillDate[p].attackFlag = false;
				if (0 >= date[8])skillDate[p].movingFlag = true;
				else if (1 <= date[8])skillDate[p].movingFlag = false;
				skillDate[p].targetPeople = date[9];
				skillDate[p].costSkillPoint = date[10];
				skillDate[p].conditionNumber = date[11];
				skillDate[p].conditionCriticalHit = date[12];
				//floatデータ
				skillDate[p].coefficientOffensive = dateF[0];
				skillDate[p].coefficientOffensive = round(skillDate[p].coefficientOffensive * 10) / 10;
				skillDate[p].coefficientDefense = dateF[1];
				skillDate[p].coefficientDefense = round(skillDate[p].coefficientDefense * 10) / 10;
				skillDate[p].coefficientMagicOffensive = dateF[2];
				skillDate[p].coefficientMagicOffensive = round(skillDate[p].coefficientMagicOffensive * 10) / 10;
				skillDate[p].coefficientMagicDefense = dateF[3];
				skillDate[p].coefficientMagicDefense = round(skillDate[p].coefficientMagicDefense * 10) / 10;
				skillDate[p].coefficientSpeed = dateF[4];
				skillDate[p].coefficientSpeed = round(skillDate[p].coefficientSpeed * 10) / 10;
				p++;
				dateNumber = -2;
				dateFNumber = 0;
				o = 0;
				for (int i = 0; i < 50; i++)
				{
					cha[i] = NULL;
					cha0[i] = NULL;
				}
			}
			else if (c == ',')
			{
				string str = cha;
				if (dateNumber == -2)
				{
					char_traits<char>::copy(cha0, str.c_str(), str.size() + 1);
					dateNumber++;
				}
				else if (dateNumber == -1)
				{
					char_traits<char>::copy(cha1, str.c_str(), str.size() + 1);
					dateNumber++;
				}
				else if (dateNumber <= 12)
				{
					date[dateNumber] = atoi(str.c_str());
					dateNumber++;
				}
				else if (dateNumber > 12)
				{
					dateF[dateFNumber] = stof(str.c_str());
					dateFNumber++;
				}
				for (int i = 0; i < 50; i++)
				{
					cha[i] = NULL;
				}
				o = 0;
			}
			else
			{
				cha[o] = c;
				o++;
			}
		}
		else if (c == '\n')
		{
			flag = true;
		}
	}
	skillDate[p].name = cha0;
	skillDate[p].conditionName = cha1;
	//intデータ
	skillDate[p].skillNumber = date[0];
	skillDate[p].costSP = date[1];
	skillDate[p].recoveryHP = date[2];
	skillDate[p].recoveryMP = date[3];
	skillDate[p].continuatianTrun = date[4];
	if (0 >= date[5])
		skillDate[p].targetMyselfOrOtherFlag = true;
	else if (1 <= date[5])
		skillDate[p].targetMyselfOrOtherFlag = false;
	if (0 >= date[6])
		skillDate[p].targetEnemyOrPlayerFlag = false;
	else if (1 <= date[6])
		skillDate[p].targetEnemyOrPlayerFlag = true;
	if (0 >= date[7])skillDate[p].attackFlag = true;
	else if (1 <= date[7])skillDate[p].attackFlag = false;
	if (0 >= date[8])skillDate[p].movingFlag = true;
	else if (1 <= date[8])skillDate[p].movingFlag = false;
	skillDate[p].targetPeople = date[9];
	skillDate[p].costSkillPoint = date[10];
	skillDate[p].conditionNumber = date[11];
	skillDate[p].conditionCriticalHit = date[12];
	//floatデータ
	skillDate[p].coefficientOffensive = dateF[0];
	skillDate[p].coefficientOffensive = round(skillDate[p].coefficientOffensive * 10) / 10;
	skillDate[p].coefficientDefense = dateF[1];
	skillDate[p].coefficientDefense = round(skillDate[p].coefficientDefense * 10) / 10;
	skillDate[p].coefficientMagicOffensive = dateF[2];
	skillDate[p].coefficientMagicOffensive = round(skillDate[p].coefficientMagicOffensive * 10) / 10;
	skillDate[p].coefficientMagicDefense = dateF[3];
	skillDate[p].coefficientMagicDefense = round(skillDate[p].coefficientMagicDefense * 10) / 10;
	skillDate[p].coefficientSpeed = dateF[4];
	skillDate[p].coefficientSpeed = round(skillDate[p].coefficientSpeed * 10) / 10;
}
//魔法情報のセット
void setMagic()
{
	FILE *fp;
	char c;
	int textSize = 300;
	char loacalText[300];
	char cha[50], cha0[50], cha1[10];
	int date[10];
	float dateF[10];
	int p = 0;
	int o = 0;
	int dateNumber = -2;
	int dateFNumber = 0;
	bool flag = true;
	//ファイルオープン
	if (0 != (errorcode = fopen_s(&fp, "OtherDate/魔法データ/magicDate.txt", "r")))
	{
		return;
	}
	for (int i = 0; i < 10; i++)
	{
		cha1[i] = NULL;
	}
	for (int i = 0; i < 50; i++)
	{
		cha[i] = NULL;
		cha0[i] = NULL;
	}
	while ((c = getc(fp)) != EOF)
	{
		if (c == '/')
			flag = false;
		if (flag == true)
		{
			if (c == '\n')
			{
				magicDate[p].name = cha0;
				magicDate[p].attribute = cha1;
				//intデータ
				magicDate[p].magicNumber = date[0];
				magicDate[p].costMP = date[1];
				magicDate[p].recoveryHP = date[2];
				magicDate[p].recoverySP = date[3];
				magicDate[p].continuatianTrun = date[4];
				if (0 >= date[5])magicDate[p].targetPlayerFlag = false;
				else if (1 <= date[5])magicDate[p].targetPlayerFlag = true;
				magicDate[p].targetPeople = date[6];
				magicDate[p].costMagicPoint = date[7];
				//floatデータ
				magicDate[p].coefficientMagicOffensive = dateF[0];
				magicDate[p].coefficientMagicDefense = dateF[1];
				magicDate[p].coefficientOffensive = dateF[2];
				magicDate[p].coefficientDefense = dateF[3];
				magicDate[p].coefficientSpeed = dateF[4];
				p++;
				dateNumber = -2;
				dateFNumber = 0;
				o = 0;
				for (int i = 0; i < 10; i++)
				{
					cha1[i] = NULL;
					date[i] = NULL;
					dateF[i] = NULL;
				}
				for (int i = 0; i < 50; i++)
				{
					cha[i] = NULL;
					cha0[i] = NULL;
				}
			}
			else if (c == ',')
			{
				string str = cha;
				if (dateNumber == -1)
				{
					char_traits<char>::copy(cha1, str.c_str(), str.size() + 1);
					dateNumber++;
				}
				else if (dateNumber == -2)
				{
					char_traits<char>::copy(cha0, str.c_str(), str.size() + 1);
					dateNumber++;
				}
				else if (dateNumber < 8)
				{
					date[dateNumber] = atoi(str.c_str());
					dateNumber++;
				}
				else if (dateNumber > 7)
				{
					dateF[dateFNumber] = stof(str.c_str());
					dateFNumber++;
				}
				for (int i = 0; i < 50; i++)
				{
					cha[i] = NULL;
				}
				o = 0;
			}
			else
			{
				cha[o] = c;
				o++;
			}
		}
		else if (c == '\n')
		{
			flag = true;
		}
	}
	magicDate[p].name = cha0;
	magicDate[p].attribute = cha1;
	//intデータ
	magicDate[p].magicNumber = date[0];
	magicDate[p].costMP = date[1];
	magicDate[p].recoveryHP = date[2];
	magicDate[p].recoverySP = date[3];
	magicDate[p].continuatianTrun = date[4];
	if (0 >= date[5])magicDate[p].targetPlayerFlag = false;
	else if (1 <= date[5])magicDate[p].targetPlayerFlag = true;
	magicDate[p].targetPeople = date[6];
	magicDate[p].costMagicPoint = date[7];
	//floatデータ
	magicDate[p].coefficientMagicOffensive = dateF[0];
	magicDate[p].coefficientMagicDefense = dateF[1];
	magicDate[p].coefficientOffensive = dateF[2];
	magicDate[p].coefficientDefense = dateF[3];
	magicDate[p].coefficientSpeed = dateF[4];
}
//逃げる処理
bool escape(playerBase* pla)
{
	int kakuritu = 0;
	for (int i = 0; i < enemysuu; i++)
	{
		float flo;
		if (combatEnemys[i].disPlayFlag)
		{
			if (ClickPlayer == 1)
				flo = (float)pla->speed / combatEnemys[i].speed;
			else if (ClickPlayer > 1)
				flo = (float)npc[ClickPlayer - 2].speed / combatEnemys[i].speed;

			if (0.5 > flo)
				kakuritu = 0;
			else if (0.5 <= flo && 1 > flo)
				kakuritu = 25;
			else if (1 <= flo && 2 > flo)
				kakuritu = 50;
			else if (2 <= flo && 3 > flo)
				kakuritu = 75;
			else if (3 <= flo)
				kakuritu = 100;
			int random = rand() % 100;
			if (kakuritu > random)
				return true;
		}
	}
	return false;
}
//戦闘時のログを作る
void combatLogCreate(string player, string usePlayer, string target, bool flag)
{
	for (int i = 0; i < 1000; i++)
		havingTextDraw[i] = NULL;
	char c1[50], c2[50], c3[50];
	char_traits<char>::copy(c3, target.c_str(), target.size() + 1);
	strcat_s(havingTextDraw, c3);
	strcat_s(havingTextDraw, "に");
	char_traits<char>::copy(c1, player.c_str(), player.size() + 1);
	strcat_s(havingTextDraw, c1);
	strcat_s(havingTextDraw, "が\n");
	char_traits<char>::copy(c2, usePlayer.c_str(), usePlayer.size() + 1);
	strcat_s(havingTextDraw, c2);
	strcat_s(havingTextDraw, "を使った");
	if (flag)
		strcat_s(havingTextDraw, "。\nクリティカルヒット！！");
}
//物理攻撃時の処理
void physicalAttack(playerBase* pla)
{
	bool flag = false;
	int random = rand() % 100;
	int keisuu = 1;
	if (pla->critical_Rate > random)
		keisuu = ini.getInt("GameDate.critical"), flag = true;
	int truepo = pla->OffensivePower * keisuu - combatEnemys[pla->choseEnemy[0]].DefensePower;
	if (truepo < 0)
		truepo = 0;
	combatLogCreate(pla->name, "物理攻撃", combatEnemys[pla->choseEnemy[0]].name, flag);
	combatEnemys[pla->choseEnemy[0]].HP -= truepo;
}
//敵の物理攻撃の処理
void enemyPhysicalAttack(playerBase* play, int ene)
{
	bool flag = false;
	int random = rand() % 100;
	int keisuu = 1;
	if (combatEnemys[ene].critical_Rate > random)
		keisuu = ini.getInt("GameDate.critical"), flag = true;
	int truepo = combatEnemys[ene].OffensivePower * keisuu - play->DefensePower;
	if (truepo < 0)
		truepo = 0;
	combatLogCreate(combatEnemys[ene].name, "物理攻撃", play->name, flag);
	if (play->HP < truepo)
		play->HP = 0;
	else
		play->HP -= truepo;
}
// 描画タイプ設定
enum DRAWTYPE
{
	DRAW_TYPE_MAP,			// マップ描画
	DRAW_TYPE_COMBAT,		// 戦闘描画
	DRAW_TYPE_TITLE,		//タイトル描画
	DRAW_TYPE_MENU,			//メニュー描画
	DRAW_TYPE_COMBAT_RESULT	//戦闘の結果描画
};
//確率を求める
int getProbability()
{
	int *s;
	s = new int[mapStates[1]];
	s[0] = mapStates[2];
	for (int ip = 1; ip < mapStates[1]; ip++)
	{
		s[ip] = s[ip - 1] + mapStates[ip + 2];
	}
	int random = rand() % 100;
	int enemynum = 0;
	for (int j = 0; j < mapStates[1]; j++)
	{
		if (s[j] > random)
		{
			return enemynum = j + 1;
		}
	}
	return 1;
}
//マップのナンバーごとに設定する
void getMapDate(int mapDate)
{
	FILE *fp;
	if (0 != (errorCode = fopen_s(&fp, "OtherDate/マップデータ/mapDate.txt", "r")))
	{
		return;
	}
	char str[50];

	int in1 = 0;
	int in2 = 0;
	char kakunou[30];
	short times = 0;
	char ch;
	bool flag = true;
	while ((fgets(str, 50, fp)) != NULL)
	{
		flag = true;
		if (str[0] == '/')
			flag = false;
		if (flag)
		{
			int kanmasuu = 0;
			for (int i = 0; i < strlen(str); i++)
			{
				if (str[i] == ',')
				{
					string st = kakunou;
					mapStates[kanmasuu] = atoi(st.c_str());
					kanmasuu++;
					for (size_t i = 0; i < 30; i++)
					{
						kakunou[i] = NULL;
					}
					times = 0;
					if (mapStates[0] != mapDate)
					{
						mapStates[0] = NULL;
						break;
					}
				}
				else
				{
					kakunou[times] = str[i];
					times++;
				}
			}
		}
	}
}
//アイテムを使うときの関数
void useItem(playerBase* play) {
	for (size_t i = 0; i < itemsSuu; i++)
	{
		if (itemsdate[i].itemsNumber == play->choseItem)
		{
			itemsdate[i].Change(play);
			play->choseItem = 0;
			combatLogCreate(play->name, itemsdate[i].name, play->name, false);
			i = itemsSuu;
		}
	}
}
//ターゲットを選ぶとき使う関数
void targetSelect(playerBase* play)
{
	for (size_t i = 0; i < skillSuu; i++)
	{
		if (skillDate[i].skillNumber == play->choseSkill)
		{
			//ターゲットが単体のとき
			if (1 == skillDate[i].targetPeople)
			{
				targetMultipleOrOnece = false;
				//係数倍のターゲットが単体かつ敵のとき
				if (skillDate[i].targetEnemyOrPlayerFlag)
				{
					targetEnemyOrPlayer = true;
					choseFlag = true;
					attactFlag = false;
				}
				//係数倍のターゲットが単体かつ味方のとき
				else if (!skillDate[i].targetEnemyOrPlayerFlag)
				{
					//ターゲットが自分の時は選択状態にしない
					if (skillDate[i].targetMyselfOrOtherFlag)
					{
						if (ClickPlayer == 1)
						{
							//自分を係数倍して攻撃する
							if (skillDate[i].attackFlag)
							{
								attactFlag = true;
								targetEnemyOrPlayer = true;
								choseFlag = true;
							}
							//自分を係数倍して攻撃はしない
							else if (!skillDate[i].attackFlag)
							{
								attactFlag = false;
								targetEnemyOrPlayer = false;
								play->combat_Stand_By_Flag = false;
								choseFlag = false;
							}
							play->chosePlayer[0] = 0;
						}
						else if (ClickPlayer > 1)
						{
							//自分を係数倍して攻撃する
							if (skillDate[i].attackFlag)
							{
								attactFlag = true;
								targetEnemyOrPlayer = true;
								choseFlag = true;
							}
							//自分を係数倍して攻撃はしない
							else if (!skillDate[i].attackFlag)
							{
								attactFlag = false;
								targetEnemyOrPlayer = false;
								play->combat_Stand_By_Flag = false;
								choseFlag = false;
							}
							play->chosePlayer[0] = ClickPlayer - 1;
						}
					}
					else if (!skillDate[i].targetMyselfOrOtherFlag)
					{
						attactFlag = false;
						targetEnemyOrPlayer = false;
						choseFlag = true;
					}
				}
			}
			//ターゲットが複数のとき
			else if (1 < skillDate[i].targetPeople)
			{
				targetMultipleOrOnece = true;
				//係数倍のターゲットが複数かつ敵のとき
				if (skillDate[i].targetEnemyOrPlayerFlag)
				{
					targetEnemyOrPlayer = true;
					choseFlag = true;
					attactFlag = false;
				}
				//係数倍のターゲットが複数かつ味方のとき
				else if (!skillDate[i].targetEnemyOrPlayerFlag)
				{
					if (skillDate[i].attackFlag)
					{
						attactFlag = true;
						targetEnemyOrPlayer = true;
						choseFlag = true;
					}
					else if (!skillDate[i].attackFlag)
					{
						attactFlag = false;
						targetEnemyOrPlayer = false;
						choseFlag = true;
					}
				}
			}
			i = skillSuu;
		}
	}
}
//スキルを使う
void useSkill(playerBase* play, playerBase* targetPlay, enemy* targetEnemy, int kaisuu)
{
	for (size_t i = 0; i < skillSuu; i++)
	{
		if (skillDate[i].skillNumber == play->choseSkill)
		{
			//スキル実行のターゲットが敵のとき
			if (targetPlay == NULL)
			{
				skillDate[i].changeEnemy(targetEnemy);
			}
			//スキル実行のターゲットが味方のとき
			else if (targetEnemy == NULL)
			{
				skillDate[i].changePlayer(targetPlay);
				skillDate[i].execution(play, targetPlay ,kaisuu);
			}
			i = skillSuu;
		}
	}
}
//ウィンドを動けなくする関数
BOOL WindowMoveLock(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCLBUTTONDOWN) {
		if (wParam == HTCAPTION) {
			SetForegroundWindow(hWnd);
			return TRUE;
		}
	}
	UNREFERENCED_PARAMETER(lParam);
	return FALSE;
}
//テキストデータを得る
void getTextDate(char FileName[], char a, char b) {
	FILE *fp;
	int textSize = 300;
	char loacalText[300];
	//ファイルオープン
	if (0 != (errorCode = fopen_s(&fp, FileName, "r")))
	{
		return;
	}

	for (size_t i = 0; i < strlen(CombattingTextDate); i++)
	{
		CombattingTextDate[i] = NULL;
	}
	while (fgets(loacalText, textSize, fp) != NULL)
	{
		if (loacalText[0] == a && loacalText[1] == b)
		{
			char cha1[300];
			for (int i = 0; i < textSize; i++)
			{
				cha1[i] = loacalText[i + 3];
			}
			strcat_s(CombattingTextDate, cha1);
		}
	}
	textSize = 200;
}
//コマンド完了時に描画状況を初期化する
void combatComandOk()
{
	haveSkillText = false; haveThingText = false;
	textNumberB = '0'; haveMagicText = false;
	getTextDate(CombatTextDate, textNumberA, textNumberB);
	textflag = false;
}
//経過ターンが０になったら初期化する
void skillProcessing(playerBase* targetPlay, enemy* targetEnemy, int i)
{
	//スキル実行のターゲットが敵のとき
	if (targetPlay == NULL)
	{
		skillDate[i].restoreEnemy(targetEnemy);
	}
	//スキル実行のターゲットが味方のとき
	else if (targetEnemy == NULL)
	{
		skillDate[i].restorePlayer(targetPlay);
	}
}
//戦闘終了時に実行する
void combatFinish(playerBase * play)
{
	for (int i = 0; i < skillSuu; i++)
	{
		//係数倍を戻す対象が味方のとき
		if (play->skillState[i].skillTrun != 0)
		{
			for (int j = 0; j < play->skillState[i].targetPeople; j++)
			{
				if (play->skillState[i].chosePlayer[j] == 0)
					skillProcessing(play, NULL, i);
				else
					skillProcessing(&npc[play->skillState[i].chosePlayer[j] - 1], NULL, i);
			}
			play->skillState[i].skillTrun = 0;
		}
		for (int k = 0; k < play->getPlayers() - 1; k++)
		{
			if (npc[k].skillState[i].skillTrun != 0)
			{
				for (int j = 0; j < play->skillState[i].targetPeople; j++)
				{
					if (play->skillState[i].chosePlayer[j] == 0)
						skillProcessing(play, NULL, i);
					else
						skillProcessing(&npc[play->skillState[i].chosePlayer[j] - 1], NULL, i);
				}
				npc[k].skillState[i].skillTrun = 0;
			}
		}
	}
	combatStanByFlag = true; choseFlag = false;
	haveSkillText = false; haveThingText = false;
	textNumberB = '0'; haveMagicText = false;
	getTextDate(CombatTextDate, textNumberA, textNumberB);
	textflag = false; combatLog = false;
	play->combat_Stand_By_Flag = true;
	for (int i = 0; i < play->getPlayers() - 1; i++)
		npc[i].combat_Stand_By_Flag = true;
	jyun = 0;
}
//
void setDate(char FileName[], int datenumber) {
	//txtを二次元配列にする
	FILE *fp;
	//ファイルオープン
	if (0 != (errorCode = fopen_s(&fp, FileName, "r")))
	{
		return;
	}
	int X = 0;
	int Y = 0;
	int a = 0;
	int o = 0;
	int p = 0;
	bool flag = true;
	while ((c = getc(fp)) != EOF)
	{
		if (flag)
		{
			if (c == '\n')//,の文字モード
			{
				Y++;
				X = 0;
				o = 0;
			}
			else if (c == ',')//
			{
				str = cha;
				int num = atoi(str.c_str());
				switch (datenumber)
				{
				case 0:
					mapChip1[Y][X] = num;
					break;
				case 1:
					backmapChip1[Y][X] = num;
					break;
				}
				
				X++;
				for (int i = 0; i < 20; i++)
				{
					cha[i] = 0;
				}
				o = 0;
				flag = true;
			}
			else if (c == '-')
			{
				flag = false;
			}
			else
			{
				cha[o] = c;
				o++;
			}
		}
		else if (c == ',')//
		{
			switch (datenumber)
			{
			case 0:
				mapChip1[Y][X] = 0;
				break;
			case 1:
				backmapChip1[Y][X] = 0;
				break;
			}
			X++;
			o = 0;
			flag = true;
		}
	}
}
//敵のデータを得る
void getEnemyDate(int enemyDate, int number)
{
	FILE *fp;
	bool flag1 = false;
	bool flag2 = false;
	int hp, mp, sp;
	if (0 != (errorCode = fopen_s(&fp, "OtherDate/enemyDate.txt", "r")))
	{
		return;
	}
	bool flag = true;
	//敵の情報を入れる場所
	int states[10], dateNumber;
	int in1 = 0;
	int in2 = 0;
	char kakunou[50], cha0[50];;
	dateNumber = -1;
	for (size_t i = 0; i < 30; i++)
	{
		kakunou[i] = NULL;
	}
	char c[10];
	snprintf(c, 10, "%d", enemyDate);
	short times = 0;
	char ch;
	while ((ch = getc(fp)) != EOF)
	{
		if (ch == '/')
			flag = false;
		if (flag == true)
		{
			times++;
			if (flag2)
			{
				if (ch == ',')
				{
					string str = kakunou;
					if (dateNumber == -1)
					{
						char_traits<char>::copy(cha0, str.c_str(), str.size() + 1);
						dateNumber = 0;
						in1 = 0;
					}
					else
					{
						states[in2] = atoi(str.c_str());
						in2++;
						in1 = 0;
					}
					for (size_t i = 0; i < 50; i++)
					{
						kakunou[i] = NULL;
					}
				}
				else if (ch == ';')
				{
					combatEnemys[number].name = cha0;
					combatEnemys[number].OffensivePower = states[0];
					combatEnemys[number].magicOffensivePower = states[1];
					combatEnemys[number].DefensePower = states[2];
					combatEnemys[number].magicDefensePower = states[3];
					combatEnemys[number].HP = states[4];
					combatEnemys[number].MAX_HP = states[4];
					combatEnemys[number].MP = states[5];
					combatEnemys[number].MAX_MP = states[5];
					combatEnemys[number].SP = states[6];
					combatEnemys[number].MAX_SP = states[6];
					combatEnemys[number].speed = states[7];
					combatEnemys[number].experience_point = states[8];
					combatEnemys[number].critical_Rate = states[9];
					break;
				}
				else
				{
					kakunou[in1] = ch;
					in1++;

				}
			}
			else if (ch == ':')
			{
				string s = kakunou;
				string s2 = c;
				if (s == s2)
				{
					flag2 = true;
					in1 = 0;
					for (size_t i = 0; i < 30; i++)
					{
						kakunou[i] = NULL;
					}
				}
				else
				{
					flag2 = false;
				}
			}
			else if (ch == '\n')
			{
				in1 = 0;
				for (size_t i = 0; i < 30; i++)
				{
					kakunou[i] = NULL;
				}
			}
			else
			{
				kakunou[in1] = ch;
				in1++;
			}
		}
		else if (ch == '\n')
		{
			flag = true;
		}
	}
}
// レンダーステートの設定
enum RENDERSTATE
{
	RENDER_DEFAULT,			// デフォルト(不透明)
	RENDER_ALPHATEST,		// αテスト
	RENDER_HARF_ALPHATEST	//αテスト半分
};
// レンダーステートの変更
void SetRenderState(IDirect3DDevice9* pD3DDevice, RENDERSTATE RenderState) {
	

	switch (RenderState)
	{
	case RENDER_DEFAULT:
	{
		// 不透明オブジェクト
		pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		//アルファテストの無効化
	}
	break;
	case RENDER_ALPHATEST:
	{
		// αテストによる透明領域の切り抜き
		pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		//アルファテストの有効化
		pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x80);			//アルファ参照値
		pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);	//アルファテスト合格基準
	}
	break;
	case RENDER_HARF_ALPHATEST:
	{
		// テクスチャの設定
		pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//テクスチャのアルファに、ポリゴン色をアルファとして加味する設定
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		//pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		//pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		//pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		//pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);// アルファ・ブレンディングを行う
		//pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// 透過処理を行う
		//pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);// 半透明処理を行う
	}
	break;
	}
}
// ウィンドウプロシージャ、ウィンドウに対するメッセージ処理を行う
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if (WindowMoveLock(hWnd, msg, wParam, lParam)) {
		return 0;
	}
	switch (msg) {
		// ウィンドウが破棄されたとき
	case WM_DESTROY:
		PostQuitMessage(0);	// WM_QUITメッセージをメッセージキューに送る
		return 0;
	}
	// デフォルトのメッセージ処理を行う
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//マップ描画の初期化
void mapInitial(int mapNumber, player* play)
{	
	delete[] enemys;
	switch (mapNumber)
	{
	case 1:
		//play->SetXandY(0, 13);
		//現在のマップを記録
		nowMapNumber = 1;
		//ファイルの読み込み
		setDate(mapChipTXT1, 0);
		setDate(backMapChipTXT1, 1);
		//現時刻の乱数を表示する（ない場合ずっと同じ値になる）
		srand((unsigned)time(NULL));
		random = rand() % (10 - 5 + 1) + 5;
		enemys = new enemy[random];
		////マップが入れ替わるポイントを設定する
		mapChangePoint[0][0] = -1;
		mapChangePoint[1][0] = -1;
		mapChangePoint[0][1] = 13;
		mapChangePoint[1][1] = 14;
		mapChangePoint[0][2] = 2;
		mapChangePoint[1][2] = 2;
		getMapDate(mapNumber);
		//敵の座標をランダムに設定する
		for (int i = 0; i < random; i++)
		{
			int enenum = getProbability() - 1;
			int   ene = enenum / 8;
			int  picu = enenum - 8 * ene;
			enemys[i].pictureNumber = picu;
			enemys[i].enemyNumber = ene;
			enemys[i].setXandY(rand() % MAP_WIDTH, rand() % MAP_HEIGHT);
			enemys[i].disPlayFlag = true;
		}	
		//初期値の調整
	//for (int i = 0; i < random; i++)
	//{
	//	for (int j = 0; j < STOP_MAP; j++)
	//	{
	//		int x = enemys[i].enemyPointX;
	//		int y = enemys[i].enemyPointY;
	//		int date = mapChip[y][x];
	//		if (date == stopMap[j])
	//		{
	//			j = 0;
	//			x += 1;
	//			y += 1;
	//			enemys[i].setXandY(x, y);
	//		}
	//	}
	//}
		enemys[0].setXandY(10, 5);
		break;
	case 2:
		//プレイヤー座標の設定
		play->SetXandY(99, 13);
		//現在のマップを記録
		nowMapNumber = 2;
		//ファイルの読み込み
		setDate(mapChipTXT2, 0);
		setDate(backMapChipTXT1, 1);
		//現時刻の乱数を表示する（ない場合ずっと同じ値になる）
		srand((unsigned)time(NULL));
		random = rand() % (20 - 10 + 1) + 10;
		enemys = new enemy[random];
		//マップが入れ替わるポイントを設定する
		mapChangePoint[0][0] = 100;
		mapChangePoint[0][1] = 13;
		mapChangePoint[0][2] = 1;
		//
		getMapDate(mapNumber);
		//敵の座標をランダムに設定する
		for (int i = 0; i < random; i++)
		{
			int enenum = getProbability() - 1;
			int   ene = enenum / 8;
			int  picu = enenum - 8 * ene;
			enemys[i].pictureNumber = picu;
			enemys[i].enemyNumber = ene;
			enemys[i].setXandY(rand() % MAP_WIDTH, rand() % MAP_HEIGHT);
			enemys[i].disPlayFlag = true;
		}
		break;
	}
}
//戦闘時の敵の初期化
void enemyInitial(int mapNumber, int pictureNumber, int enemyNumber)
{
	//データの修正
	int ene = enemyNumber + 1;
	int enemyDate = ene + pictureNumber * 8;
	switch (mapNumber)
	{
	case 1:
		enemysuu = rand() % (3 - 2 + 1) + 2;
		combatEnemys = new enemy[enemysuu];
		getEnemyDate(enemyDate, 0);
		combatEnemys[0].pictureNumber = pictureNumber;
		combatEnemys[0].enemyNumber = enemyNumber;
		getMapDate(mapNumber);
		for (int i = 1; i < enemysuu; i++)
		{
			int enenum = getProbability() - 1;
			int   ene = enenum / 8;
			int  picu = enenum - 8 * ene;
			combatEnemys[i].pictureNumber = picu;
			combatEnemys[i].enemyNumber = ene;
			getEnemyDate(getProbability(), i);
		}
		break;
	case 2:
		enemysuu = rand() % (5 - 3 + 1) + 3;
		combatEnemys = new enemy[enemysuu];
		getEnemyDate(enemyDate, 0);
		combatEnemys[0].pictureNumber = pictureNumber;
		combatEnemys[0].enemyNumber = enemyNumber;
		getMapDate(mapNumber);
		for (int i = 1; i < enemysuu; i++)
		{
			int enenum = getProbability() - 1;
			int   ene = enenum / 8;
			int  picu = enenum - 8 * ene;
			combatEnemys[i].pictureNumber = picu;
			combatEnemys[i].enemyNumber = ene;
			getEnemyDate(getProbability(), i);
		}
		break;
	}
}
// WinMain関数（アプリケーションの開始関数) コンソールアプリケーションと違い、コンソールを開かない
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//iniファイルの指定
	ini.readPass("OtherDate/初期データ/initial.ini");
	//iniファイルを元にデータを代入
	int jyoutai = 0;
	jyoutai = setIniDate(jyoutai, "PlayerDate.conditionSuu");
	
	//描写のタイプ初期化
	DRAWTYPE drawType = DRAW_TYPE_TITLE;
	//構造体ポインタの初期化
	selectSpeed* S_s = NULL;
	//ウィンドを中央表示する
	int dispx = GetSystemMetrics(SM_CXSCREEN);
	int dispy = GetSystemMetrics(SM_CYSCREEN);
	DEFALT_WIDTH = (dispx - WINDOW_WIDTH) / 2;//ウィンドXの０地点
	DEFALT_HEIGHT = (dispy - WINDOW_HEIGHT) / 2;//ウィンドYの０地点
	const TCHAR* WC_BASIC = _T("BASIC_WINDOW");
	// シンプルウィンドウクラス設定
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,WndProc, 0,0,hInstance,
		(HICON)LoadImage(NULL,MAKEINTRESOURCE(IDI_APPLICATION),IMAGE_ICON,0,0,LR_DEFAULTSIZE | LR_SHARED),
		(HCURSOR)LoadImage(NULL,MAKEINTRESOURCE(IDC_ARROW),IMAGE_CURSOR,0,0,LR_DEFAULTSIZE | LR_SHARED),
		(HBRUSH)GetStockObject(WHITE_BRUSH), NULL, WC_BASIC , NULL };// シンプルウィンドウクラス作成
	if (!RegisterClassEx(&wcex))
		return false;
	// ウィンドウの作成
	HWND hWnd = CreateWindowEx(0, WC_BASIC,
		_T("Application"),  WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,
		DEFALT_WIDTH, DEFALT_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	//	Direct3Dの初期化
	Direct3D direct3d;
	if (false == direct3d.Create(hWnd, WINDOW_WIDTH, WINDOW_HEIGHT, hInstance)) {
		MessageBox(NULL, "読み込み失敗", "", MB_OK);
	}
	//　DirectMouseの初期化
	//マウスデバイス
	directMouse directmouse;
	if (false == directmouse.create(hWnd, hInstance)){
		MessageBox(NULL, "読み込み失敗", "", MB_OK);
	}
	// テクスチャ作成
	Texture MapChip[10];
	Texture PlayerText, PlayerFaceTex, CombatMap, CombatText, HPandMPandSP, HPBar, MPBar, SPBar, SPBar2, title, titleBar, deathPicture, combatLogPicture, OKPicture;
	Texture combatResult;
	Texture Enemy[8];
	combatResult.Load(direct3d.pDevice3D, _T("PictureDate/combatResult.png"));
	OKPicture.Load(direct3d.pDevice3D, _T("PictureDate/OK.png"));
	combatLogPicture.Load(direct3d.pDevice3D, _T("PictureDate/combatLog.png"));
	CombatText.Load(direct3d.pDevice3D, _T("PictureDate/textBackPicture.png"));
	CombatMap.Load(direct3d.pDevice3D, _T("PictureDate/combatMap.jpg"));
	MapChip[0].Load(direct3d.pDevice3D, _T("PictureDate/MapChip1.png"));
	MapChip[0].setpicture(8, 16);
	MapChip[1].Load(direct3d.pDevice3D, _T("PictureDate/MapChip2.png"));
	MapChip[1].setpicture(30, 16);
	MapChip[2].Load(direct3d.pDevice3D, _T("PictureDate/MapChip3.png"));
	MapChip[2].setpicture(8, 16);
	MapChip[3].Load(direct3d.pDevice3D, _T("PictureDate/MapChip4.png"));
	MapChip[3].setpicture(16, 16);
	MapChip[4].Load(direct3d.pDevice3D, _T("PictureDate/MapChip5.png"));
	MapChip[4].setpicture(30, 16);
	MapChip[5].Load(direct3d.pDevice3D, _T("PictureDate/MapChip6.png"));
	MapChip[5].setpicture(12, 10);
	MapChip[6].Load(direct3d.pDevice3D, _T("PictureDate/MapChip7.png"));
	MapChip[6].setpicture(8, 18);
	MapChip[7].Load(direct3d.pDevice3D, _T("PictureDate/MapChip8.png"));
	MapChip[7].setpicture(8, 32);
	MapChip[8].Load(direct3d.pDevice3D, _T("PictureDate/MapChip9.png"));
	MapChip[8].setpicture(8, 41);
	MapChip[9].Load(direct3d.pDevice3D, _T("PictureDate/MapChip10.png"));
	MapChip[9].setpicture(8, 45);

	//プレイヤー五体の画像ロード
	PlayerFaceTex.Load(direct3d.pDevice3D, _T("PictureDate/playerFace.png"));
	PlayerText.Load(direct3d.pDevice3D, _T("PictureDate/player.png"));
	deathPicture.Load(direct3d.pDevice3D, _T("PictureDate/death.png"));
	title.Load(direct3d.pDevice3D, _T("PictureDate/Title.jpg"));
	Enemy[0].Load(direct3d.pDevice3D, _T("PictureDate/enemySuraim.png"));
	Enemy[1].Load(direct3d.pDevice3D, _T("PictureDate/enemyBard.png"));
	Enemy[2].Load(direct3d.pDevice3D, _T("PictureDate/enemyOku.png"));
	Enemy[3].Load(direct3d.pDevice3D, _T("PictureDate/enemyHauOku.png"));
	Enemy[4].Load(direct3d.pDevice3D, _T("PictureDate/enemySukeruton.png"));
	Enemy[5].Load(direct3d.pDevice3D, _T("PictureDate/enemyTeki.png"));
	Enemy[6].Load(direct3d.pDevice3D, _T("PictureDate/enemyHonou.png"));
	Enemy[7].Load(direct3d.pDevice3D, _T("PictureDate/enemyKishi.png"));
	HPandMPandSP.Load(direct3d.pDevice3D, _T("PictureDate/HP、MP、SP.png"));
	HPBar.Load(direct3d.pDevice3D, _T("PictureDate/HPバー.png"));
	MPBar.Load(direct3d.pDevice3D, _T("PictureDate/MPバー.png"));
	SPBar.Load(direct3d.pDevice3D, _T("PictureDate/SPバー.png"));
	SPBar2.Load(direct3d.pDevice3D, _T("PictureDate/敵SPバー.png"));
	//
	chosePayerFlag = false;
	choseFlag = false;
	// フォント作成
	DirectXText text, barText, titleText, titleNameText, combatResultText;
	text.Create(direct3d.pDevice3D, 20);
	barText.Create(direct3d.pDevice3D, 10);
	titleText.Create(direct3d.pDevice3D, 30);
	titleNameText.Create(direct3d.pDevice3D, 80);
	combatResultText.Create(direct3d.pDevice3D, 50);
	//HDC hdc = CreateCompatibleDC(NULL);
	//text.TextTexture(direct3d.pDevice3D, hdc, "あいうえお", 100, 50);
	//データをセット
	setItems();
	setSkill();
	setMagic();
	//操作プレイヤー情報を持つクラスを作成
	player player;
	player.SetXandY(8, 3);
	player.setPlayers(2);
	//状態異常の数を設定
	player.setConditionSuu(jyoutai);
	for (int i = 0; i < 5; i++)
		npc[i].setConditionSuu(jyoutai);
	//名前の設定
	player.name = "直也";
	for (int i = 0; i < 10; i++)
	{
		player.haveMagic[i] = true;
		player.haveSkill[i] = true;
	}
	for ( int i = 0; i < 4; i++)
	{
		player.haveItem[i] = 2;
	}
	npc[0].havingPlayerFlag = true;
	for (int i = 0; i < 2; i++)
	{
		npc[0].haveMagic[i] = true;
		npc[0].haveSkill[i] = true;
	}
	for (int i = 0; i < 4; i++)
	{
		npc[0].haveItem[i] = 2;
	}
	npc[0].condition[13];
	npc[0].playerLoad(direct3d.pDevice3D, _T("PictureDate/player2.png"));
	npc[0].FaceLoad(direct3d.pDevice3D, _T("PictureDate/player2Face.png"));
	npc[0].name = "椎名";
	// スプライト作成
	Sprite spritesMap1;
	Sprite spritesMap2;
	Sprite spriteCombatMap, spriteEnemy, SpritesPlayer, SpritesPlayerFace, SpriteCombatEnemy, spritePlayerState;
	// U方向、V方向に分割
	spriteEnemy.SetDivide(6, 4);
	SpritesPlayer.SetDivide(3, 4);
	SpritesPlayerFace.SetDivide(4, 2);
	SpriteCombatEnemy.SetDivide(6, 4);
	//動作開始
	direct3d.lpKeyboard->Acquire();
	startTime = timeGetTime();
	// メッセージループ
	MSG msg = {};
	while (msg.message != WM_QUIT) 
	{
		// アプリケーションに送られてくるメッセージをメッセージキューから取得する
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			DispatchMessage(&msg);	// アプリケーションの各ウィンドウプロシージャにメッセージを転送する
		} else {
			//（ここにDirectXの処理を書く）
			// 描画開始
			// キーの入力
			BYTE key[256];
			ZeroMemory(key, sizeof(key));
			if (FAILED(direct3d.lpKeyboard->GetDeviceState(sizeof(key), key))) 
			{
				// 失敗なら再開させてもう一度取得
				direct3d.lpKeyboard->Acquire();
				direct3d.lpKeyboard->GetDeviceState(sizeof(key), key);
			}
			if (key[DIK_K] & 0x80)
			{
				drawType = DRAW_TYPE_COMBAT_RESULT;
			}
			// キーが押されたときの処理
			if (playerMoveFlag == true)
			{
				if (key[DIK_SPACE] & 0x80)
				{
					direct3d.photo();
					timeFlag ? timeFlag = false : timeFlag = true;
				}
				else if (key[DIK_A] & 0x80)
				{
					if (player.playerNumber == 2)
						player.playerNumber = 0;
					if (player.playerNumber < 2)
						player.playerNumber++;
					player.mapPointX -= 1;
					player.placeNumber = 1;
					for (size_t i = 0; i < 10; i++)
						if (mapChangePoint[i][0] == player.mapPointX + 16 && mapChangePoint[i][1] == player.mapPointY + 12)
							mapInitial(mapChangePoint[i][2], &player);
					if (spriteEnemy.playerComperMap(player.mapPointX, player.mapPointY, mapChip1[player.mapPointY + 12][player.mapPointX + 16]))
						player.mapPointX += 1;
					if (spriteEnemy.playerComperMap(player.mapPointX, player.mapPointY, backmapChip1[player.mapPointY + 12][player.mapPointX + 16]))
						player.mapPointX += 1;
				}
				else if (key[DIK_D] & 0x80)
				{
					if (player.playerNumber == 2)
						player.playerNumber = 0;
					if (player.playerNumber < 2)
						player.playerNumber++;
					player.mapPointX += 1;
					player.placeNumber = 2;
					for (size_t i = 0; i < 10; i++)
						if (mapChangePoint[i][0] == player.mapPointX + 16 && mapChangePoint[i][1] == player.mapPointY + 12)
							mapInitial(mapChangePoint[i][2], &player);
					if (spriteEnemy.playerComperMap(player.mapPointX, player.mapPointY, mapChip1[player.mapPointY + 12][player.mapPointX + 16]))
						player.mapPointX -= 1;
					if (spriteEnemy.playerComperMap(player.mapPointX, player.mapPointY, backmapChip1[player.mapPointY + 12][player.mapPointX + 16]))
						player.mapPointX -= 1;
				}
				else if (key[DIK_W] & 0x80)
				{
					if (player.playerNumber == 2)
						player.playerNumber = 0;
					if (player.playerNumber < 2)
						player.playerNumber++;
					player.mapPointY -= 1;
					player.placeNumber = 3;

					for (size_t i = 0; i < 10; i++)
						if (mapChangePoint[i][0] == player.mapPointX + 16 && mapChangePoint[i][1] == player.mapPointY + 12)
							mapInitial(mapChangePoint[i][2], &player);

					if (spriteEnemy.playerComperMap(player.mapPointX, player.mapPointY, mapChip1[player.mapPointY + 12][player.mapPointX + 16]))
						player.mapPointY += 1;
					if (spriteEnemy.playerComperMap(player.mapPointX, player.mapPointY, backmapChip1[player.mapPointY + 12][player.mapPointX + 16]))
						player.mapPointY += 1;
				}
				else if (key[DIK_S] & 0x80)
				{
					if (player.playerNumber == 2)
						player.playerNumber = 0;
					if (player.playerNumber < 2)
						player.playerNumber++;
					player.mapPointY += 1;
					player.placeNumber = 0;
					for (size_t i = 0; i < 10; i++)
						if (mapChangePoint[i][0] == player.mapPointX + 16 && mapChangePoint[i][1] == player.mapPointY + 12)
							mapInitial(mapChangePoint[i][2], &player);
					if (spriteEnemy.playerComperMap(player.mapPointX, player.mapPointY, mapChip1[player.mapPointY + 12][player.mapPointX + 16]))
						player.mapPointY -= 1;
					if (spriteEnemy.playerComperMap(player.mapPointX, player.mapPointY, backmapChip1[player.mapPointY + 12][player.mapPointX + 16]))
						player.mapPointY -= 1;
				}
			}
			
			if (SUCCEEDED(direct3d.pDevice3D->BeginScene()))
			{
				// 透明領域を描画時切り抜きする設定
				SetRenderState(direct3d.pDevice3D, RENDER_ALPHATEST);
				// 背景クリア
				direct3d.pDevice3D->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(192, 192, 192, 0), 1.0f, 0);
				switch (drawType)
				{
				case DRAW_TYPE_TITLE:
				{
					spriteCombatMap.DrawCombatMap(direct3d.pDevice3D, title.pTexture);
					titleNameText.Draw(0xff000000, 400, 100, "異世界生活\n始めました");
					titleText.Draw(0xff000000, 400, 410, "初めから");
					titleText.Draw(0xff000000, 400, 450, "ロード");
					titleText.Draw(0xff000000, 400, 490, "シャッドダウン");
					//マウスの状態を取得
					directmouse.GetMouseState(hWnd);
					//左クリックした時実行される
					if (directmouse.g_zdiMouseState.rgbButtons[0] & 0x80)
					{
						GetCursorPos(&mousePoint);
						localMousePointX = mousePoint.x - DEFALT_WIDTH;
						localMouePointY = mousePoint.y - DEFALT_HEIGHT;
						if (localMousePointX > 355 && localMousePointX < 450 && localMouePointY > 415 && localMouePointY < 440)
						{
							drawType = DRAW_TYPE_MAP;
							mapInitial(1, &player);
						}
						else if (localMousePointX > 370 && localMousePointX < 430 && localMouePointY > 450 && localMouePointY < 480)
						{

						}
						else if (localMousePointX > 330 && localMousePointX < 470 && localMouePointY > 490 && localMouePointY < 515)
						{
							DestroyWindow(hWnd);
						}
					}
					wsprintf(msg2, TEXT("%d"), player.condition[12]);
					text.Draw(0xffff0000, 20, 30, msg2);
				}
				break;
				case DRAW_TYPE_MAP:
				{
					//マップ描画
					for (int i = 0; i < MAP_WIDTH; i++)
					{
						for (int j = 0; j < MAP_HEIGHT; j++)
						{
							int mapdate1 = mapChip1[j][i];
							int o1 = mapdate1 / 100000;
							o1 -= 1;
							if (o1 == -1)
								o1 = 0;
							int mapdate2 = backmapChip1[j][i];
							int o2 = mapdate2 / 100000;
							o2 -= 1;
							if (o2 == -1)
								o2 = 0;
							spritesMap1.DrawMap(direct3d.pDevice3D, &MapChip[o2], o2, i, j, mapdate2, &player, player.mapPointX, player.mapPointY);
							spritesMap2.DrawMap(direct3d.pDevice3D, &MapChip[o1], o1, i, j, mapdate1, &player, player.mapPointX, player.mapPointY);
							for (int k = 0; k < random; k++)
							{
								if (i == enemys[k].mapEnemyPointX && j == enemys[k].mapEnemyPointY && enemys[k].disPlayFlag == true)
								{
									enemys[k].setXandY(enemys[k].mapEnemyPointX, enemys[k].mapEnemyPointY);
									spriteEnemy.DrawEnemy(direct3d.pDevice3D, Enemy[enemys[k].pictureNumber].pTexture, &enemys[k], player.mapPointX, player.mapPointY);
								}
							}
						}
					}
					//敵との当たり判定
					for (int i = 0; i < random; i++)
					{
						if (enemys[i].disPlayFlag)
						{
							enemys[i].mapAI(&player);
							if (player.mapPointX + 16 == enemys[i].mapEnemyPointX &&  player.mapPointY + 12 == enemys[i].mapEnemyPointY)
							{
								startTime2 = timeGetTime();
								drawType = DRAW_TYPE_COMBAT;
								In_Combat_Enemy = i; combatTurn = 1;
								enemyInitial(nowMapNumber, enemys[i].pictureNumber, enemys[i].enemyNumber);
								getTextDate(CombatTextDate, textNumberA, textNumberB);
								startTrun = combatTurn;
								player.combat_Stand_By_Flag = true;
								for (int i = 0; i < player.getPlayers() - 1; i++)
									npc[i].combat_Stand_By_Flag = true;
								combatLog = false;
								jyun = 0;
							}
						}
					}
					//プレイヤ―描画
					SpritesPlayer.DrawPlayer(direct3d.pDevice3D, PlayerText.pTexture, &player);
					player.movePointerX = 0;
					player.movePointerY = 0;
					Sleep(120);
					//intをTCHARに変換する
					wsprintf(msg1, TEXT("%d"), enemys[0].mapEnemyPointX);
					wsprintf(msg2, TEXT("%d"), enemys[0].mapEnemyPointY);/*
					wsprintf(msg3, TEXT("%f"), player.skillDate[0].coefficientSpeed);*/
					wsprintf(msg4, TEXT("%d"), player.mapPointX + 16);
					wsprintf(msg5, TEXT("%d"), player.mapPointY + 12);
					//テキスト描画
					text.Draw(0xff000000, 100, 0, msg1);
					text.Draw(0xffff0000, 20, 30, msg2);
					text.Draw(0xffff0000, 20, 60, msg3);
					text.Draw(0xffff0000, 20, 90, msg4);
					text.Draw(0xffff0000, 20, 120, msg5);
					//時間を止める
					if (timeFlag)
					{
						nowTime = timeGetTime();
						keika = nowTime - startTime;
					
					//1秒に１回描画する
						for (size_t i = 0; i < random; i++)
						{
							if (keika >= enemys[i].enemySpeed)
							{
								if (enemys[i].chaseFlag)//プレイヤーを追いかける
								{
									enemys[i].chasePlayer(&player);
									if (spriteEnemy.playerComperMap(enemys[i].mapEnemyPointX, enemys[i].mapEnemyPointY, mapChip1[enemys[i].mapEnemyPointY][enemys[i].mapEnemyPointX]))
									{
										if (enemys[i].placenumber == 0)
											enemys[i].mapEnemyPointX -= 1;
										else if (enemys[i].placenumber == 1)
											enemys[i].mapEnemyPointY += 1;
										else if (enemys[i].placenumber == 2)
											enemys[i].mapEnemyPointX += 1;
										else if (enemys[i].placenumber == 3)
											enemys[i].mapEnemyPointY -= 1;
									}
								}
								else//自動で敵を動かす
								{
									int direction = rand() % 4;
									if (direction == 0)
									{
										enemys[i].placenumber = 1;
										enemys[i].mapEnemyPointY += 1;
										if (spriteEnemy.playerComperMap(enemys[i].mapEnemyPointX, enemys[i].mapEnemyPointY, mapChip1[enemys[i].mapEnemyPointY][enemys[i].mapEnemyPointX]))
										{
											enemys[i].mapEnemyPointY -= 1;
										}
										if (enemys[i].directionType >= 0 && enemys[i].directionType <= 2)
										{
											enemys[i].directionType += 3;
										}
										else if (enemys[i].directionType >= 3 && enemys[i].directionType <= 5)
										{
											enemys[i].directionType -= 3;
										}
										if (enemys[i].directionType != 0 && enemys[i].directionType != 3)
										{
											enemys[i].directionType = 0;
										}
									}
									else if (direction == 1)
									{
										enemys[i].placenumber = 3;
										enemys[i].mapEnemyPointY -= 1;
										if (spriteEnemy.playerComperMap(enemys[i].mapEnemyPointX, enemys[i].mapEnemyPointY, mapChip1[enemys[i].mapEnemyPointY][enemys[i].mapEnemyPointX]))
										{
											enemys[i].mapEnemyPointY += 1;
										}
										if (enemys[i].directionType >= 0 && enemys[i].directionType <= 2)
										{
											enemys[i].directionType += 3;
										}
										else if (enemys[i].directionType >= 3 && enemys[i].directionType <= 5)
										{
											enemys[i].directionType -= 3;
										}
										if (enemys[i].directionType != 1 && enemys[i].directionType != 4)
										{
											enemys[i].directionType = 1;
										}
									}
									else if (direction == 2)
									{
										enemys[i].placenumber = 0;
										enemys[i].mapEnemyPointX += 1;
										enemys[i].enemyFlag = false;
										if (spriteEnemy.playerComperMap(enemys[i].mapEnemyPointX, enemys[i].mapEnemyPointY, mapChip1[enemys[i].mapEnemyPointY][enemys[i].mapEnemyPointX]))
										{
											enemys[i].mapEnemyPointX -= 1;
										}
										if (enemys[i].directionType >= 0 && enemys[i].directionType <= 2)
										{
											enemys[i].directionType += 3;
										}
										else if (enemys[i].directionType >= 3 && enemys[i].directionType <= 5)
										{
											enemys[i].directionType -= 3;
										}
										if (enemys[i].directionType != 2 && enemys[i].directionType != 5)
										{
											enemys[i].directionType = 2;
										}
									}
									else if (direction == 3)
									{
										enemys[i].placenumber = 2;
										enemys[i].mapEnemyPointX -= 1;
										enemys[i].enemyFlag = true;
										if (spriteEnemy.playerComperMap(enemys[i].mapEnemyPointX, enemys[i].mapEnemyPointY, mapChip1[enemys[i].mapEnemyPointY][enemys[i].mapEnemyPointX]))
										{
											enemys[i].mapEnemyPointX += 1;
										}
										if (enemys[i].directionType >= 0 && enemys[i].directionType <= 2)
										{
											enemys[i].directionType += 3;
										}
										else if (enemys[i].directionType >= 3 && enemys[i].directionType <= 5)
										{
											enemys[i].directionType -= 3;
										}
										if (enemys[i].directionType != 2 && enemys[i].directionType != 5)
										{
											enemys[i].directionType = 2;
										}
									}
								}					
								startTime = nowTime;
							}
						}
					}
					directmouse.GetMouseState(hWnd);
					if (directmouse.g_zdiMouseState.rgbButtons[1] & 0x80)
					{
						drawType = DRAW_TYPE_MENU;
						timeFlag = false;
						playerMoveFlag = false;
						Sleep(100);
					}
				}
					break;
				case DRAW_TYPE_MENU:
				{
					directmouse.GetMouseState(hWnd);
					if (directmouse.g_zdiMouseState.rgbButtons[1] & 0x80)
					{
						drawType = DRAW_TYPE_MAP;
						timeFlag = true;
						playerMoveFlag = true;
						Sleep(100);
					}
				}
					break;
				case DRAW_TYPE_COMBAT_RESULT:
				{
					//結果画面の描画
					spriteCombatMap.DrawCombatMap(direct3d.pDevice3D, combatResult.pTexture);
					//プレイヤーの戦闘結果時の描写
					SpritesPlayerFace.DrawResultFace(direct3d.pDevice3D, PlayerFaceTex.pTexture, &player, player.getPlayers(), 0);
					//死んでる場合の描画
					if (!player.death_Flag)
						SpritesPlayerFace.DrawDeath(direct3d.pDevice3D, deathPicture.pTexture, &player, player.getPlayers(), 0);
					for (size_t i = 0; i < 4; i++)
					{
						if (npc[i].havingPlayerFlag)
						{
							//プレイヤーの戦闘結果時の描写
							SpritesPlayerFace.DrawResultFace(direct3d.pDevice3D, npc[i].FaceTexture, &npc[i], player.getPlayers(), i + 1);
							//死んでる場合の描画
							if (!npc[i].death_Flag)
								SpritesPlayerFace.DrawDeath(direct3d.pDevice3D, deathPicture.pTexture, &npc[i], player.getPlayers(), i + 1);
						}
					}
					//結果の描画
					TCHAR trun[8];
					wsprintf(trun, TEXT("%d"), combatTurn);
					combatResultText.Draw(0xff000000, 430, 215, trun);
					TCHAR point[8];
					wsprintf(point, TEXT("%d"), Combat_Total_Experience);
					combatResultText.Draw(0xff000000, 430, 295, point);
					
					//マウスの状態を取得
					directmouse.GetMouseState(hWnd);
					//左クリックした時実行される
					if (directmouse.g_zdiMouseState.rgbButtons[0] & 0x80)
					{
						GetCursorPos(&mousePoint);
						localMousePointX = mousePoint.x - DEFALT_WIDTH;
						localMouePointY = mousePoint.y - DEFALT_HEIGHT;
						if (localMousePointX > 640 && localMousePointX < 710 && localMouePointY > 510 && localMouePointY < 540)
							drawType = DRAW_TYPE_MAP;
					}
				}
				break;
				//戦闘時の描画
				case DRAW_TYPE_COMBAT:
				{
					//戦闘画面の描写
					spriteCombatMap.DrawCombatMap(direct3d.pDevice3D, CombatMap.pTexture);
					//メインプレイヤーの描画
					if (player.death_Flag)
					{
						player.playerFaceNumberV = 1;
						float HP = (float)player.HP / player.MAX_HP;
						float MP = (float)player.MP / player.MAX_MP;
						float SP = (float)player.SP / player.MAX_SP;
						if (HP > 1)
							HP = 1;
						if (MP > 1)
							MP = 1;
						if (SP > 1)
							SP = 1;
						SpritesPlayerFace.DrawPlayerFace(direct3d.pDevice3D, PlayerFaceTex.pTexture, &player, player.getPlayers(), 0);//プレイヤーの戦闘顔の描写
						spritePlayerState.DrawCombatPlayerState(direct3d.pDevice3D, HPandMPandSP.pTexture, player.getPlayers(), 0);//HP,MP,SPテキスト描画
						spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, HPBar.pTexture, player.getPlayers(), 0, 0, HP);//HPバー描画
						spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, MPBar.pTexture, player.getPlayers(), 0, 1, MP);//MPバー描画
						spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, SPBar.pTexture, player.getPlayers(), 0, 2, SP);//SPバー描画
					}
					else if (!player.death_Flag)
					{
						player.playerFaceNumberV = 1;
						float HP = (float)player.HP / player.MAX_HP;
						float MP = (float)player.MP / player.MAX_MP;
						float SP = (float)player.SP / player.MAX_SP;
						SpritesPlayerFace.DrawPlayerFace(direct3d.pDevice3D, PlayerFaceTex.pTexture, &player, player.getPlayers(), 0);//プレイヤーの戦闘顔の描写
						spritePlayerState.DrawCombatPlayerState(direct3d.pDevice3D, HPandMPandSP.pTexture, player.getPlayers(), 0);//HP,MP,SPテキスト描画
						spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, HPBar.pTexture, player.getPlayers(), 0, 0, HP);//HPバー描画
						spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, MPBar.pTexture, player.getPlayers(), 0, 1, MP);//MPバー描画
						spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, SPBar.pTexture, player.getPlayers(), 0, 2, SP);//SPバー描画
						SpritesPlayerFace.DrawDeath(direct3d.pDevice3D, deathPicture.pTexture, &player, player.getPlayers(), 0);
						player.combat_Stand_By_Flag = true;
					}
					if (!player.combat_Stand_By_Flag)
						SpritesPlayerFace.DrawDeath(direct3d.pDevice3D, OKPicture.pTexture, &player, player.getPlayers(), 0);
					//NPCの描画
					for (int i = 0; i < 5; i++)
					{
						if (npc[i].havingPlayerFlag)
						{
							if (npc[i].death_Flag)
							{
								float HP = (float)npc[i].HP / npc[i].MAX_HP;
								float MP = (float)npc[i].MP / npc[i].MAX_MP;
								float SP = (float)npc[i].SP / npc[i].MAX_SP;
								if (HP > 1)
									HP = 1;
								if (MP > 1)
									MP = 1;
								if (SP > 1)
									SP = 1;
								SpritesPlayerFace.DrawPlayerFace(direct3d.pDevice3D, npc[i].FaceTexture, &npc[i], player.getPlayers(), i + 1);//プレイヤーの戦闘顔の描写
								spritePlayerState.DrawCombatPlayerState(direct3d.pDevice3D, HPandMPandSP.pTexture, player.getPlayers(), i + 1);//HP,MP,SPテキスト描画
								spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, HPBar.pTexture, player.getPlayers(), i + 1, 0, HP);//HPバー描画
								spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, MPBar.pTexture, player.getPlayers(), i + 1, 1, MP);//MPバー描画
								spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, SPBar.pTexture, player.getPlayers(), i + 1, 2, SP);//SPバー描画
							}
							else if (!npc[i].death_Flag)
							{
								float HP = (float)npc[i].HP / npc[i].MAX_HP;
								float MP = (float)npc[i].MP / npc[i].MAX_MP;
								float SP = (float)npc[i].SP / npc[i].MAX_SP;
								SpritesPlayerFace.DrawPlayerFace(direct3d.pDevice3D, npc[i].FaceTexture, &npc[i], player.getPlayers(), i + 1);//プレイヤーの戦闘顔の描写
								spritePlayerState.DrawCombatPlayerState(direct3d.pDevice3D, HPandMPandSP.pTexture, player.getPlayers(), i + 1);//HP,MP,SPテキスト描画
								spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, HPBar.pTexture, player.getPlayers(), i + 1, 0, HP);//HPバー描画
								spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, MPBar.pTexture, player.getPlayers(), i + 1, 1, MP);//MPバー描画
								spritePlayerState.DrawCombatPlayerBar(direct3d.pDevice3D, SPBar.pTexture, player.getPlayers(), i + 1, 2, SP);//SPバー描画
								SpritesPlayerFace.DrawDeath(direct3d.pDevice3D, deathPicture.pTexture, &npc[i], player.getPlayers(), i + 1);
								npc[i].combat_Stand_By_Flag = true;
							}
							if (!npc[i].combat_Stand_By_Flag)
								SpritesPlayerFace.DrawDeath(direct3d.pDevice3D, OKPicture.pTexture, &npc[i], player.getPlayers(), i + 1);
						}
					} 
					//戦闘コマンド入力が完了してるか調べる
					short count = 0;
					if (!player.combat_Stand_By_Flag || !player.death_Flag)
						count++;
					for (int i = 0; i < player.getPlayers() - 1; i++)
						if (!npc[i].combat_Stand_By_Flag || !npc[i].death_Flag)
							count++;
					//戦闘時の敵を描画
					for (int a = 0; a < enemysuu; a++)
					{
						if (combatEnemys[a].disPlayFlag)
						{
							float HP = (float)combatEnemys[a].HP / combatEnemys[a].MAX_HP;
							float MP = (float)combatEnemys[a].MP / combatEnemys[a].MAX_MP;
							float SP = (float)combatEnemys[a].SP / combatEnemys[a].MAX_SP;
							if (HP > 1)
								HP = 1;
							if (MP > 1)
								MP = 1;
							if (SP > 1)
								SP = 1;
							SpriteCombatEnemy.DrawCombatEnemy(direct3d.pDevice3D, Enemy[combatEnemys[a].pictureNumber].pTexture, &combatEnemys[a], enemysuu, a);
							SpriteCombatEnemy.DrawCombatEnemyState(direct3d.pDevice3D, HPandMPandSP.pTexture, enemysuu, a);//HP,MP,SPテキスト描画
							SpriteCombatEnemy.DrawCombatEnemyBar(direct3d.pDevice3D, HPBar.pTexture, enemysuu, a, 0, HP);//HPバー描画
							SpriteCombatEnemy.DrawCombatEnemyBar(direct3d.pDevice3D, MPBar.pTexture, enemysuu, a, 1, MP);//MPバー描画
							SpriteCombatEnemy.DrawCombatEnemyBar(direct3d.pDevice3D, SPBar2.pTexture, enemysuu, a, 2, SP);//SPバー描画
						}
					}
					//時間による命令処理
					//経過時間を求める
					nowTime = timeGetTime();
					keika = nowTime - startTime;
					//一秒間に一回描画する
					if (keika >= 1000)
					{
						for (size_t i = 0; i < enemysuu; i++)
						{
							if (combatEnemys[i].directionType == 3)
								combatEnemys[i].directionType = 0;
							else if (combatEnemys[i].directionType == 0)
								combatEnemys[i].directionType += 3;
						}
						startTime = nowTime;
					}
					//経過ターンを求める
					short keikaTrun = combatTurn - startTrun;
					//１ターン経過後に実行する
					if (keikaTrun >= 1)
					{
						//SPを経過事に減らしていく
						player.SP -= 3;
						for (int i = 0; i < enemysuu; i++)
						{
							combatEnemys[i].SP -= 3;
						}
						for (int i = 0; i < player.getPlayers(); i++)
						{
							npc[i].SP -= 3;
						}
						//1ターン事にスキルの経過ターンを調べる
						for (int i = 0; i < skillSuu; i++)
						{
							//主人公のスキルの経過ターンが1以上なら実行
							if (player.skillState[i].skillTrun != 0)
							{
								player.skillState[i].skillTrun--;
								//スキル経過０で初期化する
								if (player.skillState[i].skillTrun == 0)
								{
									//係数倍を戻す対象が敵のとき
									if (skillDate[i].targetEnemyOrPlayerFlag)
									{
										for (int j = 0; j < player.skillState[i].targetPeople; j++)
											skillProcessing(NULL, &combatEnemys[player.skillState[i].choseEnemy[j]], i);
									}
									//係数倍を戻す対象が味方のとき
									else if (!skillDate[i].targetEnemyOrPlayerFlag)
									{
										for (int j = 0; j < player.skillState[i].targetPeople; j++)
										{
											if (player.skillState[i].chosePlayer[j] == 0)
												skillProcessing(&player, NULL, i);
											else
												skillProcessing(&npc[player.skillState[i].chosePlayer[j] - 1], NULL, i);
										}
									}
									player.combat_Stand_By_Flag = true;
								}
								else
								{
									if (!skillDate[i].movingFlag)
										player.combat_Stand_By_Flag = false;
									player.choseSkill = i + 1;
								}
							}
							//NPCのスキル
							for (int k = 0; k < player.getPlayers() - 1; k++)
							{
								if (npc[k].skillState[i].skillTrun != 0)
								{
									npc[k].skillState[i].skillTrun--;
									if (npc[k].skillState[i].skillTrun == 0)
									{
										//係数倍を戻す対象が敵のとき
										if (skillDate[i].targetEnemyOrPlayerFlag)
										{
											for (int j = 0; j < player.skillState[i].targetPeople; j++)
												skillProcessing(NULL, &combatEnemys[player.skillState[i].choseEnemy[j]], i);
										}
										//係数倍を戻す対象が味方のとき
										else if (!skillDate[i].targetEnemyOrPlayerFlag)
										{
											for (int j = 0; j < player.skillState[i].targetPeople; j++)
											{
												if (player.skillState[i].chosePlayer[j] == 0)
													skillProcessing(&player, NULL, i);
												else
													skillProcessing(&npc[player.skillState[i].chosePlayer[j] - 1], NULL, i);
											}
										}
										npc[k].combat_Stand_By_Flag = true;
									}
									else
									{
										if(!skillDate[i].movingFlag)
											npc[k].combat_Stand_By_Flag = false;
										npc[k].choseSkill = i + 1;
									}
								}
							}
						}
						startTrun = combatTurn;
					}
					//選択状態以外は実行する
					if (!choseFlag)
					{
						//テキスト描画系
						if (textflag)
						{
							// 透明領域を描画時切り抜きする設定
							SetRenderState(direct3d.pDevice3D, RENDER_HARF_ALPHATEST);
							if (haveThingText == false && haveSkillText == false && haveMagicText == false)
							{
								frontDisplayItem = 0;
								spriteCombatMap.DrawCombatText(direct3d.pDevice3D, CombatText.pTexture);//戦闘時のコマンドを描画する場所を作る
								wsprintf(TextDraw, TEXT("%s"), CombattingTextDate);
								text.Draw(0xff000000, 400, 320, TextDraw);//戦闘時のコマンドを描画する
							}
							else if (haveThingText && textNumberA == '0' &&  textNumberB == '0')
							{
								//アイテム描画
								int mousesuu = directmouse.g_zdiMouseState.lZ;
								frontDisplayItem += -1 * (mousesuu / 120);
								text.Draw(0xff000000, 400, 320, havingTextDraw);
								if (frontDisplayItem < 0)
								{
									frontDisplayItem = 0;
								}
								if (mousesuu != 0)
								{
									int displaysuu = 0;
									int kaisuu = 0;
									for (size_t i = 0; i < 1000; i++)
										havingTextDraw[i] = NULL;
									for (size_t i = 0; i < itemsSuu; i++)
									{
										if (ClickPlayer == 1)
										{
											if (player.haveItem[i] > 0)
											{
												if (frontDisplayItem > displaysuu) {

												}
												else if (kaisuu < 5)
												{
													char c[50];
													char_traits<char>::copy(c, itemsdate[i].name.c_str(), itemsdate[i].name.size() + 1);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, " ×");
													sprintf_s(c, "%d", player.haveItem[i]);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, "\n");
													kaisuu++;

												}
												displaysuu++;
											}
										}
										else if (ClickPlayer > 1)
										{
											if (npc[ClickPlayer - 2].haveItem[i] > 0)
											{
												if (frontDisplayItem > displaysuu) {

												}
												else if (kaisuu < 5)
												{
													char c[50];
													char_traits<char>::copy(c, itemsdate[i].name.c_str(), itemsdate[i].name.size() + 1);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, " ×");
													sprintf_s(c, "%d", npc[ClickPlayer - 2].haveItem[i]);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, "\n");
													kaisuu++;
												}
												displaysuu++;
											}
										}
									}
								}
							}
							else if (haveSkillText && textNumberA == '0' &&  textNumberB == '1')
							{
								//スキル描画
								int mousesuu = directmouse.g_zdiMouseState.lZ;
								frontDisplayItem += -1 * (mousesuu / 120);
								text.Draw(0xff000000, 400, 320, havingTextDraw);
								if (frontDisplayItem < 0)
								{
									frontDisplayItem = 0;
								}
								if (mousesuu != 0)
								{
									int displaysuu = 0;
									int kaisuu = 0;
									for (size_t i = 0; i < 1000; i++)
										havingTextDraw[i] = NULL;
									for (size_t i = 0; i < skillSuu; i++)
									{
										if (frontDisplayItem > displaysuu) {

										}
										else if (kaisuu < 5)
										{
											char c[50];
											if (ClickPlayer == 1)
											{
												if (player.haveSkill[i])
												{
													char_traits<char>::copy(c, skillDate[i].name.c_str(), skillDate[i].name.size() + 1);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, " 消費SP-");
													sprintf_s(c, "%d", skillDate[i].costSP);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, "\n");
													kaisuu++;
												}
											}
											else if (ClickPlayer > 1)
											{
												if (npc[ClickPlayer - 2].haveSkill[i])
												{
													char_traits<char>::copy(c, skillDate[i].name.c_str(), skillDate[i].name.size() + 1);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, " 消費SP-");
													sprintf_s(c, "%d", skillDate[i].costSP);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, "\n");
													kaisuu++;
												}
											}
										}
										displaysuu++;
									}
								}
							}
							else if (haveMagicText && textNumberA == '0' &&  textNumberB == '1')
							{
								int mousesuu = directmouse.g_zdiMouseState.lZ;
								frontDisplayItem += -1 * (mousesuu / 120);
								text.Draw(0xff000000, 400, 320, havingTextDraw);
								if (frontDisplayItem < 0)
								{
									frontDisplayItem = 0;
								}
								if (mousesuu != 0)
								{
									int displaysuu = 0;
									int kaisuu = 0;
									for (size_t i = 0; i < 1000; i++)
										havingTextDraw[i] = NULL;
									for (size_t i = 0; i < magicSuu; i++)
									{
										if (frontDisplayItem > displaysuu) {

										}
										else if (kaisuu < 5)
										{
											char c[50];
											if (ClickPlayer == 1)
											{
												if (player.haveMagic[i])
												{
													char_traits<char>::copy(c, magicDate[i].name.c_str(), magicDate[i].name.size() + 1);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, " 消費MP-");
													sprintf_s(c, "%d", magicDate[i].costMP);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, "\n");
													kaisuu++;
												}
											}
											else if (ClickPlayer > 2)
											{
												if (npc[ClickPlayer - 2].haveMagic[i])
												{
													char_traits<char>::copy(c, magicDate[i].name.c_str(), magicDate[i].name.size() + 1);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, " 消費MP-");
													sprintf_s(c, "%d", magicDate[i].costMP);
													strcat_s(havingTextDraw, c);
													strcat_s(havingTextDraw, "\n");
													kaisuu++;
												}
											}
										}
										displaysuu++;
									}
								}
							}
						}
						//戦闘時のログを表示
						else if (combatLog && !textflag)
						{
							// 透明領域を描画時切り抜きする設定
							SetRenderState(direct3d.pDevice3D, RENDER_HARF_ALPHATEST);
							spritePlayerState.DrawCombatLog(direct3d.pDevice3D, combatLogPicture.pTexture);
							text.Draw(0xff000000, 150, 280, havingTextDraw);
						}
							//マウスの状態を取得
						directmouse.GetMouseState(hWnd);
						//左クリックした時実行される
						if (directmouse.g_zdiMouseState.rgbButtons[0] & 0x80)
						{
							if (combatLog)
							{
								jyun++, combatStanByFlag = true, choseFlag = false;
							}
							else if (!combatLog)
							{
								GetCursorPos(&mousePoint);
								localMousePointX = mousePoint.x - DEFALT_WIDTH;
								localMouePointY = mousePoint.y - DEFALT_HEIGHT;
								//プレイヤーをクリックした時
								for (int i = 0; i < player.getPlayers(); i++)
								{
									int amari = player.getPlayers() % 2;
									int syokiti = 395 - 50 * amari - 100 * (player.getPlayers() / 2);
									if (localMousePointX > syokiti + 100 * i && localMousePointX < syokiti + 100 + 100 * i && localMouePointY > 460 && localMouePointY < 560)
									{
										if (i == 0)
										{
											if (player.combat_Stand_By_Flag && player.death_Flag)
											{

												getTextDate(CombatTextDate, '0', '0');
												ClickPlayer = i + 1;
												textflag = true;
											}
										}
										else
										{
											if (npc[i - 1].combat_Stand_By_Flag && npc[i - 1].death_Flag)
											{
												getTextDate(CombatTextDate, '0', '0');
												ClickPlayer = i + 1;
												textflag = true;
											}
										}
									}
								}
								//５つのコマンド処理
								//1つ目
								if (localMousePointX > 300 && localMousePointX < 500 && localMouePointY > 327 && localMouePointY < 347)
								{
									//アイテムが表示されてる時に処理される
									if (haveThingText)
									{
										char ca[1000];
										int k = strlen(havingTextDraw);
										char ch = ' ';
										for (int i = 0; i < k; i++)
										{
											if (havingTextDraw[i] == ch)
											{
												for (int i = 0; i < 30; i++)
												{
													s[i] = NULL;
												}
												sprintf_s(ca, "%s", havingTextDraw);
												for (int j = 0; j < i; j++)
												{
													s[j] = ca[j];
												}
												for (int i = 0; i < itemsSuu; i++)
												{
													if (s == itemsdate[i].name)
													{
														if (ClickPlayer == 1)
														{
															player.choseItem = itemsdate[i].itemsNumber;
															player.combat_Stand_By_Flag = false;
															combatComandOk();
														}
														else if (ClickPlayer > 1)
														{
															npc[ClickPlayer - 2].choseItem = itemsdate[i].itemsNumber;
															npc[ClickPlayer - 2].combat_Stand_By_Flag = false;
															combatComandOk();
														}
														i = itemsSuu;
													}
												}
												i = k;
											}
										}
									}
									else if (haveSkillText)
									{
										char ca[1000];
										int o = strlen(havingTextDraw);
										char ch = ' ';
										for (int i = 0; i < o; i++)
										{
											if (havingTextDraw[i] == ch)
											{
												for (int i = 0; i < 30; i++)
												{
													s[i] = NULL;
												}
												sprintf_s(ca, "%s", havingTextDraw);
												for (int j = 0; j < i; j++)
												{
													s[j] = ca[j];
												}
												for (int i = 0; i < skillSuu; i++)
												{
													if (s == skillDate[i].name)
													{
														if (ClickPlayer == 1)
														{
															player.choseSkill = skillDate[i].skillNumber;
															combatComandOk();
															targetSelect(&player);
														}
														else if (ClickPlayer > 1)
														{
															npc[ClickPlayer - 2].choseSkill = skillDate[i].skillNumber;
															combatComandOk();
															targetSelect(&npc[ClickPlayer - 2]);
														}
														i = itemsSuu;
													}
												}
												i = o;
											}
										}
									}
								}
								//2つめ
								else if (localMousePointX > 300 && localMousePointX < 500 && localMouePointY > 347 && localMouePointY < 367)
								{
									//アイテムが表示されてる時に処理される
									if (haveThingText)
									{
										char ca[1000];
										int kazu = 0, suu;
										int k = strlen(havingTextDraw);
										char ch = ' ';
										for (int i = 0; i < k; i++)
										{
											if (kazu == 2)
											{
												for (int i = 0; i < 30; i++)
												{
													s[i] = NULL;
												}
												sprintf_s(ca, "%s", havingTextDraw);
												int h = 0;
												for (int j = suu + 1; j < i - 1; j++)
												{
													s[h] = ca[j];
													h++;
												}
												for (int i = 0; i < itemsSuu; i++)
												{
													if (s == itemsdate[i].name)
													{
														if (ClickPlayer == 1)
														{
															player.choseItem = itemsdate[i].itemsNumber;
															player.combat_Stand_By_Flag = false;
															combatComandOk();
														}
														else if (ClickPlayer > 1)
														{
															npc[ClickPlayer - 2].choseItem = itemsdate[i].itemsNumber;
															npc[ClickPlayer - 2].combat_Stand_By_Flag = false;
															combatComandOk();
														}
														i = itemsSuu;
													}
												}
												i = k;
											}
											if (havingTextDraw[i] == '\n')
											{
												suu = i;
											}
											else if (havingTextDraw[i] == ch)
											{
												kazu += 1;
											}
										}
									}
									else if (haveSkillText)
									{
										char ca[1000];
										char ch = ' ';
										int kazu = 0, suu;
										int k = strlen(havingTextDraw);
										for (size_t i = 0; i < k; i++)
										{
											if (kazu == 2)
											{

												for (size_t i = 0; i < 30; i++)
												{
													s[i] = NULL;
												}
												sprintf_s(ca, "%s", havingTextDraw);
												int h = 0;
												for (size_t j = suu + 1; j < i - 1; j++)
												{
													s[h] = ca[j];
													h++;
												}
												for (size_t i = 0; i < skillSuu; i++)
												{
													if (s == skillDate[i].name)
													{
														if (ClickPlayer == 1)
														{
															player.choseSkill = skillDate[i].skillNumber;
															combatComandOk();
															targetSelect(&player);
														}
														else if (ClickPlayer > 1)
														{
															npc[ClickPlayer - 2].choseSkill = skillDate[i].skillNumber;
															combatComandOk();
															targetSelect(&npc[ClickPlayer - 2]);
														}
														i = itemsSuu;
													}
												}
												i = k;
											}
											if (havingTextDraw[i] == '\n')
											{
												suu = i;
											}
											else if (havingTextDraw[i] == ch)
											{
												kazu += 1;
											}
										}
									}
									else if (textNumberA == '0' &&  textNumberB == '0')
									{
										//戦闘コマンドを表示する
										textNumberB = '1';
										getTextDate(CombatTextDate, textNumberA, textNumberB);
									}
									else if (textNumberA == '0' && textNumberB == '1')
									{
										//物理攻撃
										choseFlag = true;
										combatComandOk();
										if (ClickPlayer == 1)
										{
											player.chosePhysical = true;
										}
										else if (ClickPlayer > 1)
										{
											npc[ClickPlayer - 2].chosePhysical = true;
										}
									}
								}
								//3つ目
								else if (localMousePointX > 300 && localMousePointX < 500 && localMouePointY > 367 && localMouePointY < 387)
								{
									//アイテムが表示されてる時に処理される
									if (haveThingText)
									{
										char ca[1000];
										int kazu = 0, suu;
										int k = strlen(havingTextDraw);
										char ch = ' ';
										for (size_t i = 0; i < k; i++)
										{
											if (kazu == 3)
											{
												for (size_t i = 0; i < 30; i++)
												{
													s[i] = NULL;
												}
												sprintf_s(ca, "%s", havingTextDraw);
												int h = 0;
												for (size_t j = suu + 1; j < i - 1; j++)
												{
													s[h] = ca[j];
													h++;
												}
												for (size_t i = 0; i < itemsSuu; i++)
												{
													if (s == itemsdate[i].name)
													{
														if (ClickPlayer == 1)
														{
															player.choseItem = itemsdate[i].itemsNumber;
															player.combat_Stand_By_Flag = false;
															combatComandOk();
														}
														else if (ClickPlayer > 1)
														{
															npc[ClickPlayer - 2].choseItem = itemsdate[i].itemsNumber;
															npc[ClickPlayer - 2].combat_Stand_By_Flag = false;
															combatComandOk();
														}
														i = itemsSuu;
													}
												}
												i = k;
											}
											if (havingTextDraw[i] == '\n')
											{
												suu = i;
											}
											else if (havingTextDraw[i] == ch)
											{
												kazu += 1;
											}
										}
									}
									else if (haveSkillText)
									{
										char ca[1000];
										char ch = ' ';
										int kazu = 0, suu;
										int k = strlen(havingTextDraw);
										for (size_t i = 0; i < k; i++)
										{
											if (kazu == 3)
											{

												for (size_t i = 0; i < 30; i++)
												{
													s[i] = NULL;
												}
												sprintf_s(ca, "%s", havingTextDraw);
												int h = 0;
												for (size_t j = suu + 1; j < i - 1; j++)
												{
													s[h] = ca[j];
													h++;
												}
												for (size_t i = 0; i < skillSuu; i++)
												{
													if (s == skillDate[i].name)
													{
														if (ClickPlayer == 1)
														{
															player.choseSkill = skillDate[i].skillNumber;
															combatComandOk();
															targetSelect(&player);
														}
														else if (ClickPlayer > 1)
														{
															npc[ClickPlayer - 2].choseSkill = skillDate[i].skillNumber;
															combatComandOk();
															targetSelect(&npc[ClickPlayer - 2]);
														}
														i = itemsSuu;
													}
												}
												i = k;
											}
											if (havingTextDraw[i] == '\n')
											{
												suu = i;
											}
											else if (havingTextDraw[i] == ch)
											{
												kazu += 1;
											}
										}
									}
									//アイテムデータの取得
									else if (textNumberA == '0' && textNumberB == '0' && haveThingText == false)
									{
										haveThingText = true;
										int displaysuu = 0;
										int kaisuu = 0;
										for (size_t i = 0; i < 1000; i++)
											havingTextDraw[i] = NULL;
										for (size_t i = 0; i < itemsSuu; i++)
										{
											if (ClickPlayer == 1)
											{
												if (player.haveItem[i] > 0)
												{
													if (frontDisplayItem > displaysuu) {

													}
													else if (kaisuu < 5)
													{
														char c[50];
														char_traits<char>::copy(c, itemsdate[i].name.c_str(), itemsdate[i].name.size() + 1);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, " ×");
														sprintf_s(c, "%d", player.haveItem[i]);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, "\n");
														kaisuu++;

													}
													displaysuu++;
												}
											}
											else if (ClickPlayer > 1)
											{
												if (npc[ClickPlayer - 2].haveItem[i] > 0)
												{
													if (frontDisplayItem > displaysuu) {

													}
													else if (kaisuu < 5)
													{
														char c[50];
														char_traits<char>::copy(c, itemsdate[i].name.c_str(), itemsdate[i].name.size() + 1);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, " ×");
														sprintf_s(c, "%d", npc[ClickPlayer - 2].haveItem[i]);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, "\n");
														kaisuu++;
													}

												}
											}
										}
									}
									//スキルデータの取得
									else if (textNumberA == '0' && textNumberB == '1' && haveSkillText == false)
									{
										//スキル表示
										haveSkillText = true;
										int displaysuu = 0;
										int kaisuu = 0;
										for (size_t i = 0; i < 1000; i++)
											havingTextDraw[i] = NULL;
										for (size_t i = 0; i < skillSuu; i++)
										{

											if (frontDisplayItem > displaysuu) {

											}
											else if (kaisuu < 5)
											{
												char c[50];
												if (ClickPlayer == 1)
												{
													if (player.haveSkill[i])
													{
														char_traits<char>::copy(c, skillDate[i].name.c_str(), skillDate[i].name.size() + 1);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, " 消費SP-");
														sprintf_s(c, "%d", skillDate[i].costSP);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, "\n");
														kaisuu++;
													}
												}
												else if (ClickPlayer > 1)
												{
													if (npc[ClickPlayer - 2].haveSkill[i])
													{
														char_traits<char>::copy(c, skillDate[i].name.c_str(), skillDate[i].name.size() + 1);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, " 消費SP-");
														sprintf_s(c, "%d", skillDate[i].costSP);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, "\n");
														kaisuu++;
													}
												}
											}
											displaysuu++;

										}
									}

								}
								//4つ目
								else if (localMousePointX > 300 && localMousePointX < 500 && localMouePointY > 387 && localMouePointY < 405)
								{
									//アイテムが表示されてる時に処理される
									if (haveThingText)
									{
										char ca[1000];
										char ch = ' ';
										int kazu = 0, suu;
										int k = strlen(havingTextDraw);
										for (size_t i = 0; i < k; i++)
										{
											if (kazu == 4)
											{
												char ca[1000];
												char s[30];
												for (size_t i = 0; i < 30; i++)
												{
													s[i] = NULL;
												}
												sprintf_s(ca, "%s", havingTextDraw);
												int h = 0;
												for (size_t j = suu + 1; j < i - 1; j++)
												{
													s[h] = ca[j];
													h++;
												}
												for (size_t i = 0; i < itemsSuu; i++)
												{
													if (s == itemsdate[i].name)
													{
														if (ClickPlayer == 1)
														{
															player.choseItem = itemsdate[i].itemsNumber;
															player.combat_Stand_By_Flag = false;
															combatComandOk();
														}
														else if (ClickPlayer > 1)
														{
															npc[ClickPlayer - 2].choseItem = itemsdate[i].itemsNumber;
															npc[ClickPlayer - 2].combat_Stand_By_Flag = false;
															combatComandOk();
														}
														i = itemsSuu;
													}
												}
												i = k;
											}
											if (havingTextDraw[i] == '\n')
											{
												suu = i;
											}
											else if (havingTextDraw[i] == ch)
											{
												kazu += 1;
											}
										}
									}
									else if (haveSkillText)
									{
										char ca[1000];
										char ch = ' ';
										int kazu = 0, suu;
										int k = strlen(havingTextDraw);
										for (size_t i = 0; i < k; i++)
										{
											if (kazu == 4)
											{

												for (size_t i = 0; i < 30; i++)
												{
													s[i] = NULL;
												}
												sprintf_s(ca, "%s", havingTextDraw);
												int h = 0;
												for (size_t j = suu + 1; j < i - 1; j++)
												{
													s[h] = ca[j];
													h++;
												}
												for (size_t i = 0; i < skillSuu; i++)
												{
													if (s == skillDate[i].name)
													{
														if (ClickPlayer == 1)
														{
															player.choseSkill = skillDate[i].skillNumber;
															combatComandOk();
															targetSelect(&player);
														}
														else if (ClickPlayer > 1)
														{
															npc[ClickPlayer - 2].choseSkill = skillDate[i].skillNumber;
															combatComandOk();
															targetSelect(&npc[ClickPlayer - 2]);
														}
														i = skillSuu;
													}
												}
												i = k;
											}
											if (havingTextDraw[i] == '\n')
											{
												suu = i;
											}
											else if (havingTextDraw[i] == ch)
											{
												kazu += 1;
											}
										}
									}
									else if (textNumberA == '0' && textNumberB == '0')
									{

									}
									//魔法表示
									else if (textNumberA == '0' && textNumberB == '1')
									{
										haveMagicText = true;
										int displaysuu = 0;
										int kaisuu = 0;
										for (size_t i = 0; i < 1000; i++)
											havingTextDraw[i] = NULL;
										for (size_t i = 0; i < magicSuu; i++)
										{

											if (frontDisplayItem > displaysuu) {

											}
											else if (kaisuu < 5)
											{
												char c[50];
												if (ClickPlayer == 1)
												{
													if (player.haveMagic[i])
													{
														char_traits<char>::copy(c, magicDate[i].name.c_str(), magicDate[i].name.size() + 1);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, " 消費MP-");
														sprintf_s(c, "%d", magicDate[i].costMP);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, "\n");
														kaisuu++;
													}
												}
												else if (ClickPlayer > 2)
												{
													if (npc[ClickPlayer - 2].haveMagic[i])
													{
														char_traits<char>::copy(c, magicDate[i].name.c_str(), magicDate[i].name.size() + 1);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, " 消費MP-");
														sprintf_s(c, "%d", magicDate[i].costMP);
														strcat_s(havingTextDraw, c);
														strcat_s(havingTextDraw, "\n");
														kaisuu++;
													}
												}
											}
											displaysuu++;

										}
									}
								}
								//5つ目
								else if (localMousePointX > 300 && localMousePointX < 500 && localMouePointY > 405 && localMouePointY < 422)
								{
									//アイテムが表示されてる時に処理される
									if (haveThingText)
									{
										char ca[1000];
										char ch = ' ';
										int kazu = 0, suu;
										int k = strlen(havingTextDraw);
										for (size_t i = 0; i < k; i++)
										{
											if (kazu == 5)
											{

												for (size_t i = 0; i < 30; i++)
												{
													s[i] = NULL;
												}
												sprintf_s(ca, "%s", havingTextDraw);
												int h = 0;
												for (size_t j = suu + 1; j < i - 1; j++)
												{
													s[h] = ca[j];
													h++;
												}
												for (size_t i = 0; i < itemsSuu; i++)
												{
													if (s == itemsdate[i].name)
													{
														if (ClickPlayer == 1)
														{
															player.choseItem = itemsdate[i].itemsNumber;
															player.combat_Stand_By_Flag = false;
															combatComandOk();
														}
														else if (ClickPlayer > 1)
														{
															npc[ClickPlayer - 2].choseItem = itemsdate[i].itemsNumber;
															npc[ClickPlayer - 2].combat_Stand_By_Flag = false;
															combatComandOk();
														}
														i = itemsSuu;
													}
												}
												i = k;
											}
											if (havingTextDraw[i] == '\n')
											{
												suu = i;
											}
											else if (havingTextDraw[i] == ch)
											{
												kazu += 1;
											}
										}
									}
									else if (haveSkillText)
									{
										char ca[1000];
										char ch = ' ';
										int kazu = 0, suu;
										int k = strlen(havingTextDraw);
										for (size_t i = 0; i < k; i++)
										{
											if (kazu == 5)
											{

												for (size_t i = 0; i < 30; i++)
												{
													s[i] = NULL;
												}
												sprintf_s(ca, "%s", havingTextDraw);
												int h = 0;
												for (size_t j = suu + 1; j < i - 1; j++)
												{
													s[h] = ca[j];
													h++;
												}
												for (size_t i = 0; i < skillSuu; i++)
												{
													if (s == skillDate[i].name)
													{
														if (ClickPlayer == 1)
														{
															player.choseSkill = skillDate[i].skillNumber;
															combatComandOk();
															targetSelect(&player);
														}
														else if (ClickPlayer > 1)
														{
															npc[ClickPlayer - 2].choseSkill = skillDate[i].skillNumber;
															combatComandOk();
															targetSelect(&npc[ClickPlayer - 2]);
														}
														i = skillSuu;
													}
												}
												i = k;
											}
											if (havingTextDraw[i] == '\n')
											{
												suu = i;
											}
											else if (havingTextDraw[i] == ch)
											{
												kazu += 1;
											}
										}
									}
									//逃げるコマンド
									else if (textNumberA == '0' && textNumberB == '0')
									{
										if (escape(&player))
										{
											enemys[In_Combat_Enemy].setXandY(rand() % MAP_WIDTH, rand() % MAP_HEIGHT);
											drawType = DRAW_TYPE_MAP; haveSkillText = false;
											haveMagicText = false;
											textNumberB = '0';
											getTextDate(CombatTextDate, textNumberA, textNumberB);
											textflag = false;
											haveThingText = false;
										}
									}
									else if (textNumberA == '0'&& textNumberB == '1')
									{
									}
								}
							}
							combatStanByFlag = true;
							Sleep(120);
						}
					}
					//敵を選択するときに実行する
					else if (choseFlag)
					{
						//敵を選択するとき
						if (targetEnemyOrPlayer && attactFlag || player.chosePhysical || npc[ClickPlayer - 2].chosePhysical)
						{
							TCHAR hikime[5];
							wsprintf(hikime, TEXT("%d"), choseEnemyKaisuu + 1);
							combatResultText.Draw(0xff000000, WINDOW_WIDTH / 2 - 280, WINDOW_HEIGHT / 2, hikime);
							combatResultText.Draw(0xff000000, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "匹目の敵を選んでください");
							//マウスの状態を取得
							directmouse.GetMouseState(hWnd);
							if (directmouse.g_zdiMouseState.rgbButtons[0] & 0x80)
							{
								GetCursorPos(&mousePoint);
								localMousePointX = mousePoint.x - DEFALT_WIDTH;
								localMouePointY = mousePoint.y - DEFALT_HEIGHT;
								//敵を選択するとき使う
								int kaisuu;
								if (ClickPlayer == 1)
									kaisuu = skillDate[player.choseSkill - 1].targetPeople;
								else if (ClickPlayer > 1)
									kaisuu = skillDate[npc[ClickPlayer - 2].choseSkill - 1].targetPeople;
								if (player.chosePhysical || npc[ClickPlayer - 2].chosePhysical)
									kaisuu = 1;
								for (int i = 0; i < enemysuu; i++)
								{
									int amari = enemysuu % 2;
									int syokiti = 370 - 25 * amari - 50 * (enemysuu / 2);
									if (localMousePointX > syokiti + 75 * i && localMousePointX < syokiti + 75 + 75 * i && localMouePointY > 140 && localMouePointY < 190)
									{
										if (ClickPlayer == 1)
										{
											player.choseEnemy[chosePlayerKaisuu] = i;
											Sleep(300);
											choseEnemyKaisuu++;
											if (choseEnemyKaisuu == kaisuu)
											{
												choseEnemyKaisuu = 0;
												choseFlag = false;
												player.combat_Stand_By_Flag = false;
											}
										}
										else if (ClickPlayer > 1)
										{

											npc[ClickPlayer - 2].choseEnemy[chosePlayerKaisuu] = i;
											Sleep(300);
											choseEnemyKaisuu++;
											if (choseEnemyKaisuu == kaisuu)
											{
												choseEnemyKaisuu = 0;
												choseFlag = false;
												npc[ClickPlayer - 2].combat_Stand_By_Flag = false;
											}
										}
										choseFlag = false;
									}
								}
							}
						}
						//プレイヤー選択時
						else if (!targetEnemyOrPlayer && !attactFlag)
						{
							TCHAR hikime[5];
							wsprintf(hikime, TEXT("%d"), chosePlayerKaisuu + 1);
							combatResultText.Draw(0xff000000, WINDOW_WIDTH / 2 - 280, WINDOW_HEIGHT / 2, hikime);
							combatResultText.Draw(0xff000000, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "人目の味方を選んでください");
							//マウスの状態を取得
							directmouse.GetMouseState(hWnd);
							if (directmouse.g_zdiMouseState.rgbButtons[0] & 0x80)
							{
								GetCursorPos(&mousePoint);
								localMousePointX = mousePoint.x - DEFALT_WIDTH;
								localMouePointY = mousePoint.y - DEFALT_HEIGHT;
								int kaisuu;
								if (ClickPlayer == 1)
									kaisuu = skillDate[player.choseSkill - 1].targetPeople;
								else if (ClickPlayer > 1)
									kaisuu = skillDate[npc[ClickPlayer - 2].choseSkill - 1].targetPeople;
								for (int i = 0; i < player.getPlayers(); i++)
								{
									int amari = player.getPlayers() % 2;
									int syokiti = 395 - 50 * amari - 100 * (player.getPlayers() / 2);
									if (localMousePointX > syokiti + 100 * i && localMousePointX < syokiti + 100 + 100 * i && localMouePointY > 460 && localMouePointY < 560)
									{
										if (i == 0)
										{
											if (player.combat_Stand_By_Flag && player.death_Flag)
											{
												if (ClickPlayer == 1)
												{
													player.chosePlayer[chosePlayerKaisuu] = i;
													Sleep(300);
													chosePlayerKaisuu++;
													if (chosePlayerKaisuu == kaisuu)
													{
														chosePlayerKaisuu = 0;
														choseFlag = false;
														player.combat_Stand_By_Flag = false;
													}
												}
												else if (ClickPlayer > 1)
												{
													npc[ClickPlayer - 2].chosePlayer[chosePlayerKaisuu] = i;
													Sleep(300); 
													chosePlayerKaisuu++;
													if (chosePlayerKaisuu == kaisuu)
													{
														chosePlayerKaisuu = 0;
														choseFlag = false;
														player.combat_Stand_By_Flag = false;
													}
												}
											}
										}
										else
										{
											if (npc[i - 1].combat_Stand_By_Flag && npc[i - 1].death_Flag)
											{
												if (ClickPlayer == 1)
												{
													player.chosePlayer[0] = i + 1;
													player.combat_Stand_By_Flag = false;
												}
												else if (ClickPlayer > 1)
												{
													npc[ClickPlayer - 2].chosePlayer[0] = i + 1;
													npc[ClickPlayer - 2].combat_Stand_By_Flag = false;
												}
												choseFlag = false;
											}
										}
										choseFlag = false;
									}
								}
							}
						}
					}
					//右クリックで初期化
					if (directmouse.g_zdiMouseState.rgbButtons[1] & 0x80)
					{
						//マウスの座標を取得
						GetCursorPos(&mousePoint);
						//座標を調整する
						localMousePointX = mousePoint.x - DEFALT_WIDTH;
						localMouePointY = mousePoint.y - DEFALT_HEIGHT;
						//プレイヤーをクリックした時
						for (int i = 0; i < player.getPlayers(); i++)
						{
							int amari = player.getPlayers() % 2;
							int syokiti = 395 - 50 * amari - 100 * (player.getPlayers() / 2);
							if (localMousePointX > syokiti + 100 * i && localMousePointX < syokiti + 100 + 100 * i && localMouePointY > 460 && localMouePointY < 560)
							{
								if (i == 0)
								{
									if (player.skillState[player.choseSkill - 1].skillTrun == 0)
									{
										haveSkillText = false; haveThingText = false;
										textNumberA = '0'; textNumberB = '0'; haveMagicText = false; player.combat_Stand_By_Flag = true;
										getTextDate(CombatTextDate, textNumberA, textNumberB);
										textflag = false; choseFlag = false;
									}
									if (player.skillState[player.choseSkill - 1].skillTrun != 0 && !skillDate[player.choseSkill - 1].movingFlag)
									{
										haveSkillText = false; haveThingText = false;
										textNumberA = '0'; textNumberB = '0'; haveMagicText = false;
										getTextDate(CombatTextDate, textNumberA, textNumberB);
										textflag = false; choseFlag = false;
									}
								}
								else
								{
									npc[i - 1].combat_Stand_By_Flag = true;
									haveSkillText = false; haveThingText = false;
									textNumberA = '0'; textNumberB = '0'; haveMagicText = false;
									getTextDate(CombatTextDate, textNumberA, textNumberB);
									textflag = false; choseFlag = false;
								}
							}
							else
							{
								haveSkillText = false; haveThingText = false;
								textNumberA = '0'; textNumberB = '0'; haveMagicText = false;
								getTextDate(CombatTextDate, textNumberA, textNumberB);
								textflag = false; choseFlag = false;
							}
						}
					}
					//コマンド入力が完了してたら処理する
					if (count == player.getPlayers() && combatStanByFlag)
					{
						if (jyun == 0)
						{
							//スピードを代入
							S_s = new selectSpeed[player.getPlayers() + enemysuu];
							if (player.death_Flag)
								S_s[0].speed = player.speed;
							else
								S_s[0].speed = 0;
							S_s[0].Number = 1;
							S_s[0].enemy_NPC_Player = 2;
							for (int i = 0; i < player.getPlayers() - 1; i++) {
								if (npc[i].death_Flag)
									S_s[i + 1].speed = npc[i].speed;
								else
									S_s[i + 1].speed = 0;
								S_s[i + 1].Number = i + 2;
								S_s[i + 1].enemy_NPC_Player = 1;
							}
							//敵のスピード代入
							for (int i = player.getPlayers(); i < enemysuu + player.getPlayers(); i++)
							{
								if (combatEnemys[i - player.getPlayers()].disPlayFlag)
									S_s[i].speed = combatEnemys[i - player.getPlayers()].speed;
								else
									S_s[i].speed = 0;
								S_s[i].Number = i + 1;
								S_s[i].enemy_NPC_Player = 0;
							}
							//スピード順に並べる
							S_s = down(S_s, player.getPlayers() + enemysuu);
							//スピードから順番を決める
							decisionTurn(S_s, player.getPlayers() + enemysuu);
							//順番を降順にする
							S_s = up(S_s, player.getPlayers() + enemysuu);
						}
						combatLog = true;
						switch (S_s[jyun].enemy_NPC_Player)
						{
						//敵のコマンド処理
						case 0:
						{
							if (combatEnemys[S_s[jyun].Number - (player.getPlayers() + 1)].disPlayFlag)
							{
								while (1)
								{
									int random = rand() % player.getPlayers();
									if (random == 0)
									{
										if (player.death_Flag)
										{
											enemyPhysicalAttack(&player, S_s[jyun].Number - (player.getPlayers() + 1));
											break;
										}
									}
									else
									{
										if (npc[random - 1].death_Flag)
										{
											enemyPhysicalAttack(&npc[random - 1], S_s[jyun].Number - (player.getPlayers() + 1));
											break;
										}
									}
								}
							}
							break;
						}
						//NPCのコマンド処理
						case 1:
						{
							if (npc[S_s[jyun].Number - 2].death_Flag)
							{
								if (npc[S_s[jyun].Number - 2].choseItem != 0)
								{
									useItem(&npc[S_s[jyun].Number - 2]);
									npc[S_s[jyun].Number - 2].choseItem = 0;
								}
								else if (npc[S_s[jyun].Number - 2].choseSkill != 0)
								{
									int kaisuu = skillDate[npc[ClickPlayer - 2].choseSkill - 1].targetPeople;
									npc[ClickPlayer - 2].SP -= skillDate[npc[ClickPlayer - 2].choseSkill - 1].costSP;
									for (int i = 0; i < kaisuu; i++)
									{
										//係数倍の対象が敵のとき
										if (skillDate[npc[ClickPlayer - 2].choseSkill - 1].targetEnemyOrPlayerFlag)
										{
											useSkill(&npc[ClickPlayer - 2], NULL, &combatEnemys[player.choseEnemy[i]], i);
											npc[ClickPlayer - 2].skillState[npc[ClickPlayer - 2].choseSkill - 1].choseEnemy[i] = player.choseEnemy[i];
										}
										//係数倍の対象が味方のとき
										else if (!skillDate[npc[ClickPlayer - 2].choseSkill - 1].targetEnemyOrPlayerFlag)
										{
											if (npc[ClickPlayer - 2].chosePlayer[i] == 0)//主人公のとき
												useSkill(&npc[ClickPlayer - 2], &player, NULL, i);
											else//NPCのとき
												useSkill(&npc[ClickPlayer - 2], &npc[player.chosePlayer[i] - 2], NULL, i);
											npc[ClickPlayer - 2].skillState[npc[ClickPlayer - 2].choseSkill - 1].chosePlayer[i] = npc[ClickPlayer - 2].chosePlayer[i];
										}
									}
									if (npc[ClickPlayer - 2].skillState[npc[ClickPlayer - 2].choseSkill - 1].skillTrun == 0)
										npc[ClickPlayer - 2].skillState[npc[ClickPlayer - 2].choseSkill - 1].skillTrun = skillDate[npc[ClickPlayer - 2].choseSkill - 1].continuatianTrun;
									npc[ClickPlayer - 2].skillState[npc[ClickPlayer - 2].choseSkill - 1].targetPeople = kaisuu;
									npc[S_s[jyun].Number - 2].choseSkill = 0;
								}
								else if (npc[S_s[jyun].Number - 2].choseMagic != 0)
								{
									npc[S_s[jyun].Number - 2].choseMagic = 0;
								}
								else if (npc[S_s[jyun].Number - 2].chosePhysical)
								{
									physicalAttack(&npc[S_s[jyun].Number - 2]);
									npc[S_s[jyun].Number - 2].chosePhysical = false;
								}
							}
							npc[S_s[jyun].Number - 2].choseEnemy[0] = 0;
							break;
						}
						//プレイヤーのコマンド処理
						case 2:
						{
							if (player.death_Flag)
							{
								//アイテム実行時
								if (player.choseItem != 0)
								{
									useItem(&player);
									player.choseItem = 0;
								}
								//スキル実行時
								else if (player.choseSkill != 0)
								{
									int kaisuu = skillDate[player.choseSkill - 1].targetPeople;
									player.SP -= skillDate[player.choseSkill - 1].costSP;
									for (int i = 0; i < kaisuu; i++)
									{
										//係数倍の対象が敵のとき
										if (skillDate[player.choseSkill - 1].targetEnemyOrPlayerFlag)
										{
											useSkill(&player, NULL, &combatEnemys[player.choseEnemy[i]], i);
											player.skillState[player.choseSkill - 1].choseEnemy[i] = player.choseEnemy[i];
										}
										//係数倍の対象が味方のとき
										else if (!skillDate[player.choseSkill - 1].targetEnemyOrPlayerFlag)
										{
											if (player.chosePlayer[i] == 0)//主人公のとき
												useSkill(&player, &player, NULL, i);
											else//NPCのとき
												useSkill(&player, &npc[player.chosePlayer[i] - 2], NULL, i);
											player.skillState[player.choseSkill - 1].chosePlayer[i] = player.chosePlayer[i];
										}
									}
									player.skillState[player.choseSkill - 1].targetPeople = kaisuu;
									if (player.skillState[player.choseSkill - 1].skillTrun == 0)
										player.skillState[player.choseSkill - 1].skillTrun = skillDate[player.choseSkill - 1].continuatianTrun;
									player.choseSkill = 0;
								}
								//
								else if (player.choseMagic != 0)
								{
									player.choseMagic = 0;
								}
								//物理攻撃
								else if (player.chosePhysical)
								{
									physicalAttack(&player);
									player.chosePhysical = false;
								}
							}
							player.choseEnemy[0] = 0;
							break;
						}
						}
						combatStanByFlag = false;
						//コマンド実行終了
						if (jyun == enemysuu + player.getPlayers())
						{
							player.combat_Stand_By_Flag = true;
							for (int i = 0; i < player.getPlayers() - 1; i++)
								npc[i].combat_Stand_By_Flag = true;
							combatTurn++;
							combatLog = false;
							jyun = 0;
							combatStanByFlag = true;
							choseFlag = false;
						}
					}
					//プレイヤーのHPとSPが0でないか調べる
					int downSuu = 0;
					if (player.HP <= 0 || player.SP <= 0)
						player.death_Flag = false, downSuu++;
					for (int i = 0; i < player.getPlayers() - 1; i++)
					{
						if (npc[i].HP <= 0 || npc[i].SP <= 0)
							npc[i].death_Flag = false, downSuu++;
					}
					//プレイヤーが全滅しているか知らべる
					if (downSuu == player.getPlayers())
					{
						drawType = DRAW_TYPE_TITLE;
					}
					//敵が死んでるか判断して、戦闘が終わるかどうかを判断する
					downSuu = 0;
					Combat_Total_Experience = 0;
					for (int i = 0; i < enemysuu; i++)
					{
						if (combatEnemys[i].HP <= 0 || combatEnemys[i].SP <= 0)
							combatEnemys[i].disPlayFlag = false;
						if (combatEnemys[i].disPlayFlag == false)
						{
							downSuu++;
							Combat_Total_Experience += combatEnemys[i].experience_point;
						}
						if (downSuu == enemysuu)
						{
							drawType = DRAW_TYPE_COMBAT_RESULT;
							combatFinish(&player);
							enemys[In_Combat_Enemy].disPlayFlag = false;
						}
					}
					mouseHoile += directmouse.g_zdiMouseState.lZ / 120;
					GetCursorPos(&mousePoint);
					wsprintf(msg2, TEXT("%d"), mousePoint.x - DEFALT_WIDTH);
					text.Draw(0xffff0000, 20, 30, msg2);
					wsprintf(msg3, TEXT("%d"), mousePoint.y - DEFALT_HEIGHT);
					text.Draw(0xffff0000, 20, 60, msg3);
					int OP = (int)player.OffensivePower * 100;
					wsprintf(msg4, TEXT("%d"), player.choseSkill);
					text.Draw(0xffff0000, 50, 90, msg4);
					OP = (int)npc[0].OffensivePower * 100;
					wsprintf(msg5, TEXT("%d"), player.skillState[3].skillTrun);
					text.Draw(0xffff0000, 50, 120, msg5);
					wsprintf(msg6, TEXT("%s"), choseFlag?"true":"false");
					text.Draw(0xffff0000, 50, 150, msg6);
				}
					break;
				}	
					// 描画終了
					direct3d.pDevice3D->EndScene();
			}
			// 描画反映
			direct3d.pDevice3D->Present(NULL, NULL, NULL, NULL);
		}
	}
	return 0;
}
void item::Change(playerBase * play)
{
		play->HP += changeHP;
		play->MP += changeMP;
		play->SP += changeSP;
		if (play->HP > play->MAX_HP)
		{
			play->HP = play->MAX_HP;
		}
		if (play->MP > play->MAX_MP)
		{
			play->MP = play->MAX_MP;
		}
		if (play->SP > play->MAX_SP)
		{
			play->SP = play->MAX_SP;
		}
		play->OffensivePower += changeOffensivePower;
		play->DefensePower += changeDefensePower;
		play->haveItem[play->choseItem - 1] -= 1;
}
void skill::changePlayer(playerBase* targetPlay)
{
	targetPlay->HP += recoveryHP;
	targetPlay->MP += recoveryMP;
	if (targetPlay->HP > targetPlay->MAX_HP)
		targetPlay->HP = targetPlay->MAX_HP;
	if (targetPlay->MP > targetPlay->MAX_MP)
		targetPlay->MP = targetPlay->MAX_MP;
	coefficientOffensive = round(coefficientOffensive * 10) / 10;
	targetPlay->OffensivePower *= coefficientOffensive;
	targetPlay->DefensePower *= coefficientDefense;
	targetPlay->magicOffensivePower *= coefficientMagicOffensive;
	targetPlay->magicDefensePower *= coefficientMagicDefense;
	targetPlay->speed *= coefficientSpeed;
}
void skill::restorePlayer(playerBase * targetPlay)
{
	targetPlay->OffensivePower /= coefficientOffensive;
	targetPlay->DefensePower /= coefficientDefense;
	targetPlay->magicOffensivePower /= coefficientMagicOffensive;
	targetPlay->magicDefensePower /= coefficientMagicDefense;
	targetPlay->speed /= coefficientSpeed;
}
void skill::changeEnemy(enemy* targetEnemy)
{

}
void skill::restoreEnemy(enemy* targetEnemy)
{

}
void skill::execution(playerBase* play, playerBase* target, int kaisuu)
{
	//自分を係数倍して攻撃を実行
	if (attackFlag)
	{
		bool flag = false;
		int random = rand() % 100;
		int keisuu = 1;
		if (play->critical_Rate > random)
			keisuu = 10, flag = true;
		int truepo = play->OffensivePower * keisuu - combatEnemys[play ->choseEnemy[kaisuu]].DefensePower;
		if (truepo < 0)
			truepo = 0;
		combatLogCreate(play->name, name, combatEnemys[play->choseEnemy[kaisuu]].name, flag);
		combatEnemys[play->choseEnemy[kaisuu]].HP -= truepo;
	}
	else
	{
		combatLogCreate(play->name, name, target->name, false);
	}
}