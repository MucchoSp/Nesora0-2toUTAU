﻿// 
// +--------------------------------------------------------------------------------------------+
// |	音諳零号機弐型 - CUI - 音諳コード														|
// |	Copyright (c) 2021 むーちょsp															|
// +--------------------------------------------------------------------------------------------+
// |																							|
// |	音諳コード関係のクラスや関数の定義です。												|
// |																							|
// +--------------------------------------------------------------------------------------------+
// |																							|
// |	NSStringは、std::stringと同じように使えます。											|
// |																							|
// +--------------------------------------------------------------------------------------------+
//
// 2023年に改造済み
// 
// これは、sinテーブルを使う時、速度を優先させるか、長時間の読み上げの時の質を優先させるか(オーバーフローしないようにするか)を選択するためのものです。
// コメントを外すと、速度を優先させるようになります。
//#define NS_USE_FAST_SIN_TABLE

#include "Nesora_for_embedded_systems.h"

namespace Nesora{

nsstringData nsc::nsromajiencode(const unsigned char* str) {

	size_t textlen = 0, youon = 0;

	//文字の選別(textlenには平仮名の数を、youonには平仮名の拗音の数をカウントする。)
	for (size_t i = 0;i < strlen((const char*)str);i++)
		if (('A' <= str[i] and str[i] <= 'Z') or ('a' <= str[i] and str[i] <= 'z'))
			textlen++;

	nsstringData out = new nscharData[textlen + 1];
	for (size_t i = 0;i < textlen + 1;i++)
		out[i] = nscreateCharData(0, 0, 0, 0);

	nsint i = 0;size_t n = 0;
	char nowmode = 0, nowmodepitchpm = 0;
	nscharData nowmoji = {};
	uint16_t ctime = NSDATAINIT_T;
	nsuint alltime = 0;
	int16_t pitch = NSDATAINIT_P, baseFreq = 0;

	while (str[i] != 0 and n < textlen + 1) {
		//通常の処理
		if (nowmode == 0) {
			//平仮名
			if (('A' <= str[i] and str[i] <= 'Z') or ('a' <= str[i] and str[i] <= 'z')) {
				if (str[i] == 'a' or str[i] == 'A') {
					//あ
					nowmoji.c = NSa;
				}
				else if (str[i] == 'i' or str[i] == 'I') {
					//い
					nowmoji.c = NSi;
				}
				else if (str[i] == 'u' or str[i] == 'U') {
					//う
					nowmoji.c = NSu;
				}
				else if (str[i] == 'e' or str[i] == 'E') {
					//え
					nowmoji.c = NSe;
				}
				else if (str[i] == 'o' or str[i] == 'O') {
					//お
					nowmoji.c = NSo;
				}
				else if (str[i] == 'k' or str[i] == 'K') {
					i++;
					if (str[i] == 'a' or str[i] == 'A') //か
						nowmoji.c = NSka;
					else if (str[i] == 'i' or str[i] == 'I') //き
						nowmoji.c = NSki;
					else if (str[i] == 'u' or str[i] == 'U') //く
						nowmoji.c = NSku;
					else if (str[i] == 'e' or str[i] == 'E') //け
						nowmoji.c = NSke;
					else if (str[i] == 'o' or str[i] == 'O') //こ
						nowmoji.c = NSko;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A') //きゃ
							nowmoji.c = NSkya;
						else if (str[i] == 'u' or str[i] == 'U') //きゅ
							nowmoji.c = NSkyu;
						else if (str[i] == 'e' or str[i] == 'E') //きぇ
							nowmoji.c = NSkye;
						else if (str[i] == 'o' or str[i] == 'O') //きょ
							nowmoji.c = NSkyo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A') //くゎ
							nowmoji.c = NSkwa;
						else if (str[i] == 'i' or str[i] == 'I') //くぃ
							nowmoji.c = NSkwi;
						else if (str[i] == 'e' or str[i] == 'E') //くぇ
							nowmoji.c = NSkwe;
						else if (str[i] == 'o' or str[i] == 'O') //くぉ
							nowmoji.c = NSkwo;
					}
				}
				else if (str[i] == 'c' or str[i] == 'C') {
					i++;
					if (str[i] == 'a' or str[i] == 'A') //か
						nowmoji.c = NSka;
					else if (str[i] == 'i' or str[i] == 'I') //し
						nowmoji.c = NSsi;
					else if (str[i] == 'u' or str[i] == 'U') //く
						nowmoji.c = NSku;
					else if (str[i] == 'e' or str[i] == 'E') //せ
						nowmoji.c = NSse;
					else if (str[i] == 'o' or str[i] == 'O') //こ
						nowmoji.c = NSko;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A') //ちゃ
							nowmoji.c = NStya;
						else if (str[i] == 'u' or str[i] == 'U') //ちゅ
							nowmoji.c = NStyu;
						else if (str[i] == 'e' or str[i] == 'E') //ちぇ
							nowmoji.c = NStye;
						else if (str[i] == 'o' or str[i] == 'O') //ちょ
							nowmoji.c = NStyo;
					}
					else if (str[i] == 'h' or str[i] == 'H') {
						i++;
						if (str[i] == 'a' or str[i] == 'A') //ちゃ
							nowmoji.c = NScha;
						else if (str[i] == 'i' or str[i] == 'I') //ち
							nowmoji.c = NSchi;
						else if (str[i] == 'u' or str[i] == 'U') //ちゅ
							nowmoji.c = NSchu;
						else if (str[i] == 'e' or str[i] == 'E') //ちぇ
							nowmoji.c = NSche;
						else if (str[i] == 'o' or str[i] == 'O') //ちょ
							nowmoji.c = NScho;
					}
				}
				else if (str[i] == 'g' or str[i] == 'G') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSga;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSgi;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NSgu;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSge;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSgo;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSgya;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NSgyu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSgye;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSgyo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSgwa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NSgwi;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSgwe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSgwo;
					}
				}
				else if (str[i] == 't' or str[i] == 'T') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSta;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSti;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NStu;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSte;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSto;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NStya;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NStyu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NStye;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NStyo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NStwa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NStwi;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NStwe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NStwo;
					}
					else if (str[i] == 's' or str[i] == 'S') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')//
							nowmoji.c = NStsa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NStsi;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NStsu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NStse;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NStso;
					}
				}
				else if (str[i] == 'd' or str[i] == 'D') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSda;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSdi;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NSdu;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSde;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSdo;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSdya;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NSdyu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSdye;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSdyo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSdwa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NSdwi;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSdwe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSdwo;
					}
				}
				else if (str[i] == 'p' or str[i] == 'P') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSpa;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSpi;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NSpu;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSpe;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSpo;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSpya;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NSpyu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSpye;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSpyo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSpwa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NSpwi;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSpwe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSpwo;
					}
				}
				else if (str[i] == 'b' or str[i] == 'B') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSba;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSbi;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NSbu;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSbe;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSbo;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSbya;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NSbyu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSbye;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSbyo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSbwa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NSbwi;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSbwe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSbwo;
					}
				}
				else if (str[i] == 's' or str[i] == 'S') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSsa;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSsi;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NSsu;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSse;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSso;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSsya;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NSsyu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSsye;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSsyo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSswa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NSswi;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSswe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSswo;
					}
					else if (str[i] == 'h' or str[i] == 'H') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSsha;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NSshi;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NSshu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSshe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSsho;
					}
				}
				else if (str[i] == 'z' or str[i] == 'Z') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSza;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSzi;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NSzu;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSze;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSzo;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSzya;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NSzyu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSzye;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSzyo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSzwa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NSzwi;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSzwe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSzwo;
					}
				}
				else if (str[i] == 'j' or str[i] == 'J') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSja;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSji;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NSju;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSje;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSjo;
				}
				else if (str[i] == 'h' or str[i] == 'H') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSha;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NShi;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NShu;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NShe;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSho;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NShya;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NShyu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NShye;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NShyo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NShwa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NShwi;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NShwe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NShwo;
					}
				}
				else if (str[i] == 'f' or str[i] == 'F') {
				i++;
				if (str[i] == 'a' or str[i] == 'A')
					nowmoji.c = NSfa;
				else if (str[i] == 'i' or str[i] == 'I')
					nowmoji.c = NSfi;
				else if (str[i] == 'u' or str[i] == 'U')
					nowmoji.c = NSfu;
				else if (str[i] == 'e' or str[i] == 'E')
					nowmoji.c = NSfe;
				else if (str[i] == 'o' or str[i] == 'O')
					nowmoji.c = NSfo;
				}
				else if (str[i] == 'n' or str[i] == 'N') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSna;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSni;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NSnu;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSne;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSno;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSnya;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NSnyu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSnye;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSnyo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSnwa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NSnwi;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSnwe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSnwo;
					}
					else if (str[i] == 'n' or str[i] == 'N')
						nowmoji.c = NSnn1;
				}
				else if (str[i] == 'm' or str[i] == 'M') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSma;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSmi;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NSmu;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSme;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSmo;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSmya;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NSmyu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSmye;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSmyo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSmwa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NSmwi;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSmwe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSmwo;
					}
				}
				else if (str[i] == 'r' or str[i] == 'R') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSra;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSri;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NSru;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSre;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSro;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSrya;
						else if (str[i] == 'u' or str[i] == 'U')
							nowmoji.c = NSryu;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSrye;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSryo;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							nowmoji.c = NSrwa;
						else if (str[i] == 'i' or str[i] == 'I')
							nowmoji.c = NSrwi;
						else if (str[i] == 'e' or str[i] == 'E')
							nowmoji.c = NSrwe;
						else if (str[i] == 'o' or str[i] == 'O')
							nowmoji.c = NSrwo;
					}
				}
				else if (str[i] == 'y' or str[i] == 'Y') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSya;
					else if (str[i] == 'u' or str[i] == 'U')
						nowmoji.c = NSyu;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSye;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSyo;
				}
				else if (str[i] == 'w' or str[i] == 'W') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						nowmoji.c = NSwa;
					else if (str[i] == 'i' or str[i] == 'I')
						nowmoji.c = NSwi;
					else if (str[i] == 'e' or str[i] == 'E')
						nowmoji.c = NSwe;
					else if (str[i] == 'o' or str[i] == 'O')
						nowmoji.c = NSwo;
				}
				else if (str[i] == 'x' or str[i] == 'X' or str[i] == 'l' or str[i] == 'L') {
					i++;
					if (str[i] == 'a' or str[i] == 'A')
						out[n-1].c += 1;
					else if (str[i] == 'i' or str[i] == 'I')
						out[n - 1].c += 2;
					else if (str[i] == 'u' or str[i] == 'U')
						out[n - 1].c += 2;
					else if (str[i] == 'e' or str[i] == 'E')
						out[n - 1].c += 3;
					else if (str[i] == 'o' or str[i] == 'O')
						out[n - 1].c += 4;
					else if (str[i] == 'y' or str[i] == 'Y') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							out[n - 1].c += 1;
						else if (str[i] == 'u' or str[i] == 'U')
							out[n - 1].c += 2;
						else if (str[i] == 'e' or str[i] == 'E')
							out[n - 1].c += 3;
						else if (str[i] == 'o' or str[i] == 'O')
							out[n - 1].c += 4;
					}
					else if (str[i] == 'w' or str[i] == 'W') {
						i++;
						if (str[i] == 'a' or str[i] == 'A')
							out[n - 1].c += 1;
						else if (str[i] == 'i' or str[i] == 'I')
							out[n - 1].c += 2;
						else if (str[i] == 'e' or str[i] == 'E')
							out[n - 1].c += 3;
						else if (str[i] == 'o' or str[i] == 'O')
							out[n - 1].c += 4;
					}
					else if (str[i] == 't' or str[i] == 'T') {
						nowmoji.c = NSspc;
						nowmoji.time = alltime;
						nowmoji.ctime = ctime;
						nowmoji.pitch = pitch;
						out[n] = nowmoji;
						n++;alltime += ctime;
						i++;
						if (str[i] == 's' or str[i] == 'S')
							i++;
					}
					i++;
					continue;
				}

				nowmoji.time = alltime;
				nowmoji.ctime = ctime;
				nowmoji.pitch = pitch;
				out[n] = nowmoji;
				alltime += ctime;
				n++;i++;
			}
			//記号
			else if (str[i] == ',') {
				//句点
				nowmoji.c = NSspc;
				nowmoji.time = alltime;
				nowmoji.ctime = ctime;
				nowmoji.pitch = pitch;
				out[n] = nowmoji;
				n++;alltime += ctime;
				i++;
			}
			else if(str[i] == '.'){
				//句読点
				nowmoji.c = NSspc;
				nowmoji.time = alltime;
				nowmoji.ctime = ctime * 2;
				nowmoji.pitch = pitch;
				out[n] = nowmoji;
				n++;alltime += ctime * 2;
				i++;
			}
			else if (str[i] == '-') {
				//伸ばし棒
				nowmoji.c = out[n - 1].c & 0x0f;
				if (nowmoji.c == NSya or nowmoji.c == NSwa)
					nowmoji.c = NSa;
				else if (nowmoji.c == NSwi)
					nowmoji.c = NSi;
				else if (nowmoji.c == NSyu)
					nowmoji.c = NSu;
				else if (nowmoji.c == NSye or nowmoji.c == NSwe)
					nowmoji.c = NSe;
				else if (nowmoji.c == NSyo or nowmoji.c == NSwo)
					nowmoji.c = NSo;
				nowmoji.time = alltime;
				nowmoji.ctime = ctime;
				nowmoji.pitch = pitch;
				out[n] = nowmoji;
				n++;alltime += ctime * 2;
				i++;
			}
			//スピード、ピッチ指定
			else if (str[i] == '<') {
				if (str[i + 1] == 'S' or str[i + 1] == 's') {
					nowmode = 1;
					ctime = 0;
					i += 2;
				}
				else if (str[i + 1] == 'P' or str[i + 1] == 'p') {
					if (str[i + 2] == 'u' or str[i + 2] == 'U') {
						pitch += 5;
						i += 4;
					}
					else if (str[i + 2] == 'd' or str[i + 2] == 'D') {
						pitch -= 5;
						i += 4;
					}
					else {
						nowmode = 2;
						pitch = 0;
						i += 2;
					}
				}
				else if (str[i + 1] == 'B' or str[i + 1] == 'b') {
					if (str[i + 2] == 'f' or str[i + 2] == 'F') {
						nowmode = 4;
						baseFreq = 0;
						i += 3;
					}
				}
			}
			//コメント
			else if (str[i] == '(') {
				nowmode = 10;
			}
			//その他
			else {
				i++;
			}
		}
		//スピードの数字の処理
		else if (nowmode == 1) {
			if (str[i] == '>')
				nowmode = 0;
			else {
				if (str[i] >= '0' and str[i] <= '9') {
					ctime *= 10;
					ctime += str[i] - '0';
				}
				else {
					nowmode = 0;
				}
			}
			i++;
		}
		//ピッチの符号の処理
		else if (nowmode == 2) {
			if (str[i] == '+') {
				nowmodepitchpm = 1;
				nowmode = 3;
				i++;
			}
			else if (str[i] == '-') {
				nowmodepitchpm = -1;
				nowmode = 3;
				i++;
			}
			else 
				if (baseFreq == 0) {
					nowmodepitchpm = 1;
					nowmode = 3;
				}
				else {
					nowmodepitchpm = 0;
					nowmode = 3;
				}
		}
		//ピッチの数字の処理
		else if (nowmode == 3) {
			if (str[i] == '>') {
				if (nowmodepitchpm == 0) {
					pitch = pitch - baseFreq;
					nowmode = 0;
				}
				else {
					pitch *= nowmodepitchpm;
					nowmode = 0;
				}
			}
			else {
				if (str[i] >= '0' and str[i] <= '9') {
					pitch *= 10;
					pitch += str[i] - '0';
				}
				else {
					nowmode = 0;
				}
			}
			i++;
		}
		//基底周波数の数字の処理
		else if (nowmode == 4) {
		if (str[i] == '>') {
			nowmode = 0;
		}
		else {
			if (str[i] >= '0' and str[i] <= '9') {
				baseFreq *= 10;
				baseFreq += str[i] - '0';
			}
			else {
				nowmode = 0;
			}
		}
		i++;
		}
		//コメント
		else if (nowmode == 10) {
			if (str[i] == ')')
				nowmode = 0;
			i++;
		}
	}

	return out;
}

//音諳書式で書かれたマルチバイト文字列を音諳コードにエンコード
nsstringData nsc::nsromajiencode(const char* str) {

	size_t stringlen = strlen(str) + 1;
	unsigned char* text = new unsigned char[stringlen];
	for (size_t i = 0; i < stringlen;i++)
		text[i] = 0;

	//char型の文字列をunsigned char型に変換
	for (size_t i = 0; i < stringlen;i++)
		text[i] = (unsigned char)((nsint)str[i] & 0xff);

	return nsromajiencode(text);
}

//nscharData型の変数を作成
nscharData nsc::nscreateCharData(uint32_t time, uint16_t ctime, nschar c, int16_t pitch) {
	nscharData out = {};
	out.c = c;
	out.ctime = ctime;
	out.pitch = pitch;
	out.time = time;
	return out;
}

//nsstringData型の変数をコピー
void nsc::nsstrcpy(nsstringData destination, const nsstringData& source) {
	nsint i = 0;
	while (source[i].c != 0) {
		destination[i].c = source[i].c;
		destination[i].ctime = source[i].ctime;
		destination[i].pitch = source[i].pitch;
		destination[i].time = source[i].time;
		i++;
	}
}

//nsstringData型の文字数を取得
size_t nsc::nsstrlen(const nsstringData& str) {
	size_t size = 0;
	while (str[size].c != 0)
		size++;
	return size;
}

//nsstring型の文字数を取得
size_t nsc::nsstrlen(const nsstring& str) {
	size_t size = 0;
	while (str[size] != 0)
		size++;
	return size;
}

//const unsigned char型の文字数を取得
size_t nsc::nsstrlen(const unsigned char* str) {
	size_t size = 0;
	while (str[size] != 0)
		size++;
	return size;
}


//  +-------------------------------------------------------------------------------------------+
//  |   音諳文字列型の諸々専用のクラス                                                          |
//  |   string型みたいに使えるよ☆                                                              |
//  +-------------------------------------------------------------------------------------------+

//空の音諳文字列型を作成
NSString::NSString() {
	n_capacity = n_size = n_wsize = n_wcapacity = 0;
	n_string = new nscharData[n_size + 1];
	n_string[0] = nsc::nscreateCharData(0, 0, 0, 0);
}

//マルチバイト文字列から音諳文字列型を作成
NSString::NSString(const char* c) {
	n_string = nsc::nsromajiencode(c);
	n_capacity = (strlen(c) + 1) * sizeof(nscharData);
	n_wsize = n_wcapacity = nsc::nsstrlen(n_string);
	n_size = n_wsize * sizeof(nscharData);
}

//音諳コード型から音諳文字列型を作成
NSString::NSString(const nsstring c) {
	size_t size = nsc::nsstrlen(c);
	nsstringData str = new nscharData[size];
	int32_t alltime = NSDATAINIT_T;
	for (size_t i = 0; i < size;i++) {
		str[i].c = c[i];
		str[i].ctime = NSDATAINIT_T;
		str[i].time = alltime;
		str[i].pitch = NSDATAINIT_P;
		alltime += NSDATAINIT_T;
	}
	n_string = str;
	n_capacity = size * sizeof(nscharData);
	n_wsize = n_wcapacity = nsc::nsstrlen(n_string);
	n_size = n_wsize * sizeof(nscharData);
}

//音諳データ型から音諳文字列型を作成
NSString::NSString(const nsstringData c) {
	size_t size = nsc::nsstrlen(c);
	n_string = c;
	n_capacity = size * sizeof(nscharData);
	n_wsize = n_wcapacity = nsc::nsstrlen(n_string);
	n_size = n_wsize * sizeof(nscharData);
}

//音諳文字列型の複製
NSString::NSString(const NSString& s) {
	n_capacity = s.n_capacity;
	n_wcapacity = s.n_wcapacity;
	n_size = s.n_size;
	n_wsize = s.n_wsize;
	n_string = new nscharData[n_size + 1];
	nsc::nsstrcpy(n_string, s.n_string);
}


NSString::~NSString() {
}

//  ----演算子----

nscharData& NSString::operator [](const size_t n) {
	return n_string[n];
}

const nscharData& NSString::operator [](const size_t n) const {
	return n_string[n];
}

NSString NSString::operator +(const char* c) {
	NSString retval(*this);
	retval += c;
	return retval;
}

NSString NSString::operator +(const NSString& str) {
	NSString retval(*this);
	retval += str;
	return retval;
}

NSString& NSString::operator +=(const char* c) {
	NSString str(c);
	*this += str;
	return *this;
}

NSString& NSString::operator +=(const NSString& str) {
	size_t new_wsize = this->n_wsize + str.n_wsize;
	if (this->n_wcapacity < new_wsize) {
		while (this->n_wcapacity < new_wsize) {
			this->n_wcapacity *= 2;
		}
		nsstringData new_n_string = new nscharData[this->n_wcapacity + 1];
		memcpy(new_n_string, this->n_string, this->n_size);
		this->n_string = new_n_string;
	}
	nsc::nsstrcpy(this->n_string + this->n_size, str.n_string);
	this->n_size = this->n_size + str.n_size;
	this->n_wsize = new_wsize;
	return *this;
}

//  ----メンバ関数----

//文字が入っているか
bool NSString::empty() const {
	return (n_wsize == 0);
}

//サイズ(バイト数)
size_t NSString::size() const {
	return n_size;
}

//文字数
size_t NSString::length() const {
	return n_wsize;
}

//最大の文字数
size_t NSString::capacity() const {
	return n_capacity;
}

//初めの文字
const nscharData& NSString::front() const {
	return n_string[0];
}

//後わりの文字
const nscharData& NSString::back() const {
	return n_string[n_wsize - 1];
}

//初めの文字
nscharData& NSString::front() {
	return n_string[0];
}

//終わりの文字
nscharData& NSString::back() {
	return n_string[n_wsize - 1];
}

//0.01秒単位での全体の時間
size_t NSString::alltime() const {
	size_t size = 0;
	for (size_t i = 0;i < n_wsize;i++)
		size += n_string[i].ctime;
	return size;
}





//	音諳零号機 メインプログラム
//	
//	音諳零号機のメインプログラムです。
// 
//	Copyright (c) 2021 MucchoSP

//=====音作成=====

//リストから合成されたsin波を生成
void mainMakeVoiseF::makeSinWave(nsint num, const std::vector<nsfloat>& Hz, const std::vector<nsfloat>& s, std::vector<nsfloat>& out, nsint a, bool doformant) {
	//num...Hzやsの長さ Hz...倍音 s...各倍音の大きさ out...出力配列 size...出力配列のサイズ

	nsfloat sinha = 0;
	std::vector<nsfloat> output1(out.size(), 0.0);
	std::vector<nsfloat> output2(out.size(), 0.0);
	std::vector<nsfloat> output3(out.size(), 0.0);

	//音の大きさが0でない場合のみ通す。
	std::vector<nsint> canUseHz(0);
	for (nsint i = 0;i < num;i++)
		if (s[i])
			canUseHz.push_back(i);

	nsfloat alls = 0;
	for (nsint i = 0;i < num;i++)alls += s[i];
	alls = (nsfloat)1 / (alls * (nsfloat)2);

	if (doformant) {
		//sin波の合成
		for (size_t i = 0;i < out.size();i++) {
			for (size_t j = 0;j < canUseHz.size();j++) {
				if (formants[2] > out.size())
					//sinha = sin(((nsfloat)i + a) * (nsfloat)2.0 * (nsfloat)PI / ((nsfloat)SMPL / Hz[canUseHz[j]]));			//jの周波数のiの時のsin波の値を計算する。
					sinha = nsMath::nsSinTable((nsuint)i + (nsuint)a, Hz[canUseHz[j]]);			//jの周波数のiの時のsin波の値を計算する。
				else
					//sinha = cos(((nsfloat)i + a) * (nsfloat)2.0 * (nsfloat)PI / ((nsfloat)SMPL / Hz[canUseHz[j]]));			//jの周波数のiの時のsin波の値を計算する。
					sinha = nsMath::nsCosTable((nsuint)i + (nsuint)a, Hz[canUseHz[j]]);			//jの周波数のiの時のcos波の値を計算する。
				output1[i] = output1[i] + sinha * s[canUseHz[j]];															//sinhaを、それぞれの大きさでかけ合わせて足す。
			}
			output1[i] *= alls;																								//平均化して出力する。
		}

		//フィルターの種類(ノッチフィルター)
		//1.ノッチフィルター無し	2.すべてにノッチフィルターを適用	3.ある程度距離があればノッチフィルターを適用
		switch (3) {
		case 1:
			hipassFilter(output1, output2, formants[0], (nsfloat)0.707);												//ハイパスフィルター
			lowpassFilter(output2, out, formants[3], (nsfloat)1.0);													//ローパスフィルター
			break;
		case 2:
			hipassFilter(output1, output2, formants[0], (nsfloat)0.707);												//ハイパスフィルター
			lowpassFilter(output2, output3, formants[3], (nsfloat)1.0);												//ローパスフィルター
			for (nsint i = 0; i < 3;i++)
				notchpassFilter(output3, out, (formants[i] + formants[i + 1]) / (nsfloat)3.0, formants[i + 1] / formants[i] * (nsfloat)0.25);	//ノッチパスフィルター
			break;
		default:

			hipassFilter(output1, output2, formants[0], (nsfloat)0.707);												//ハイパスフィルター
			lowpassFilter(output2, output3, formants[3], (nsfloat)1.0);												//ローパスフィルター

			for (nsint i = 0; i < 3;i++) {
				if (formants[i] * 2 < formants[i + 1])
					notchpassFilter(output3, out, (formants[i] + formants[i + 1]) / (nsfloat)3.0, formants[i + 1] / formants[i] * (nsfloat)0.25);	//ノッチパスフィルター
			}
		}
	}
	else {
		//sin波の合成
		for (size_t i = 0;i < out.size();i++) {
			for (size_t j = 0;j < canUseHz.size();j++) {
				sinha = nsMath::nsSinTable((nsuint)i + (nsuint)a, Hz[canUseHz[j]]);							//jの周波数のiの時のsin波の値を計算する。
				output1[i] = output1[i] + sinha * s[canUseHz[j]];															//sinhaを、それぞれの大きさでかけ合わせて足す。
			}
			output1[i] *= alls;																								//平均化して出力する。
		}
		hipassFilter(output1, output2, Hz[0], (nsfloat)0.707);															//ハイパスフィルター
		lowpassFilter(output2, out, Hz[num - 1], (nsfloat)1.0);																//ローパスフィルター
	}

	return ;
}

//単純なsin波を生成
void mainMakeVoiseF::makeSinha(nsfloat Hz, nsfloat s, std::vector<nsfloat>& out) {

	for (size_t i = 0; i < out.size(); i++)
		out[i] = nsMath::nsSinTable((nsuint)i, Hz) * s;

	return ;
}

//特定の時間の周波数を取得
nsfloat mainMakeVoiseF::makeMomentSinWave(nsint num, const std::vector<nsfloat>& Hz, const std::vector<nsfloat>& s, nsint gets) {

	nsint ho0 = (nsint)(SMPL * 0.5);//(nsint)(4.0 / (nsfloat)Hz[0] * SMPL);
	std::vector<nsfloat> out(ho0, 0.0);

	makeSinWave(num, Hz, s, out, ho0);

	nsfloat ret = out[gets % ho0];

	return ret;
}

//基本倍音設定
void mainMakeVoiseF::makeHarmonicOvertone(nsfloat bsf) {

	hosize = (nsint)(MAXFREQ / bsf);
	harmonicOvertones.resize(hosize);
	harmonictoneFilters.resize(hosize);
	for (nsint i = 1; i <= hosize;i++)
		harmonicOvertones[i - 1] = bsf * i;		//倍音作成
	bf = bsf;
	return;
}

//フォルマントリストに周波数を設定
void mainMakeVoiseF::setFormants(nsfloat F1, nsfloat F2, nsfloat F3, nsfloat F4) {

	formants[0] = F1;
	formants[1] = F2;
	formants[2] = F3;
	formants[3] = F4;

	return;
}

//=====音自動生成=====

//基本倍音の大きさを自動生成
void mainMakeVoiseF::makeFilter() {

	nsfloat a = 0.0;
	nsint c = 0;

	//フィルター初期化
	std::fill(harmonictoneFilters.begin(), harmonictoneFilters.end(), (nsfloat)0.0);

	//フィルター作成
	for (nsint j = 0;j < hosize;j++) {
		for (nsint i = 0;i < 4;i++) {
			a = (-(nsfloat)1.0 * std::cbrt(std::abs(((nsfloat)harmonicOvertones[j] - (nsfloat)formants[i]) * (nsfloat)FILTERWIDTH_INV)) + (nsfloat)1.0);	//サイクロイド関数を応用したフィルターの計算式
			if (!isfinite(a) or (a < 0))
				continue;																								//虚数の場合、飛ばす。
			harmonictoneFilters[j] = harmonictoneFilters[j] + a;														//出した値を足す。
		}
		if (formants[0] < harmonicOvertones[j])																			//フォルマント1より小さいかどうか
			harmonictoneFilters[j] = harmonictoneFilters[j] * (nsfloat)0.25 * ((nsfloat)1.0 / ((nsfloat)j + formants[0] / harmonicOvertones[0] + (nsfloat)1.0)) * (nsfloat)12;	//フィルターの平均値を出し、jの反比例の形でフィルターをかけ、十二倍する。
		else
			harmonictoneFilters[j] = harmonictoneFilters[j] * (nsfloat)0.25 * ((nsfloat)1.0 / (-(nsfloat)j + formants[0] / harmonicOvertones[0] + (nsfloat)1.0)) * (nsfloat)12;	//フィルターの平均値を出し、-jの反比例の形でフィルターをかけ、十二倍する。
	}

	//音の減衰
	if (softness)
		for (nsint i = 0;i < hosize;i++)
			harmonictoneFilters[i] = harmonictoneFilters[i] * log(softness / harmonicOvertones[i]) / log(softness / bf);

	//フィルターのフィルター
	for (nsint i = 0;i < hosize;i++) {
		if (harmonictoneFilters[i] < 0.01)
			harmonictoneFilters[i] = 0;																					//0.01以下の小さな音は取り除く。
		else {
			c = (nsint)(harmonictoneFilters[i] * 100);
			harmonictoneFilters[i] = (nsfloat)c * (nsfloat)0.01;														//フィルターの値を小数点第三位で切り捨てる。
		}
	}



	return;
}

//クラス内のパラメータからsin波を合成
void mainMakeVoiseF::makeSinWaveFromClass(std::vector<nsfloat>& out, nsint a) {
	/*クラスの値から声を作成
	* out	...出力配列
	* size	...outの大きさ
	* a		...初めの角度
	*/
	makeFilter();

	makeSinWave(hosize, harmonicOvertones, harmonictoneFilters, out, a);

	return;
}

//クラス内のパラメータから特定の時間の周波数を取得
nsfloat mainMakeVoiseF::makeMomentSinWaveFromClass(nsint t) {
	/*クラスの値から声を作成
	* out	...出力配列
	* size	...outの大きさ
	* a		...初めの角度
	*/
	makeFilter();

	return makeMomentSinWave(hosize, harmonicOvertones, harmonictoneFilters, t % SMPL);
}

//現在のmainMakeVoiseF.cppの状態を取得する。
nsint mainMakeVoiseF::getstate(nsint state) {

	if (state == 0) {
		if (formants.size())
			return 1;
		else
			return 0;
	}
	else if (state == 1) {

	}

	return 0;
}

//softness変数を変更
void mainMakeVoiseF::setSoftness(nsfloat sn) {
	softness = sn;
	return;
}




//	音諳零号機 ファイルプログラム
//	
//	音諳零号機関係のファイルを統括して管理するプログラムです。
//  
//  -----------------------------------------------------------------------------------------------
// 
//	Copyright (c) 2021 MucchoSP

mainMakeVoiseF mvffMMVF;

//=====読み上げ関係=====

//音諳 風鈴を使う
void makeVoiseFromFile::setDefaulFurin() {

	//音諳の標準音声「音諳 風鈴」の音声ファイルより抜粋
	unsigned char data[68] = { 0x44, 0x41, 0x54, 0x41, 0x62, 0x66, 0xB8, 0x01, 0x56, 0x46, 0x46, 0x41, 0x4C, 0x04, 0xA4, 0x06, 0xA0, 0x0F, 0x5C, 0x12, 0x56, 0x46, 0x46, 0x49, 0x80, 0x02, 0x42, 0x0E, 0xA0, 0x0F, 0x5C, 0x12, 0x56, 0x46, 0x46, 0x55, 0x58, 0x02, 0xEF, 0x06, 0xA0, 0x0F, 0x5C, 0x12, 0x56, 0x46, 0x46, 0x45, 0x0C, 0x03, 0x54, 0x0B, 0xA0, 0x0F, 0x5C, 0x12, 0x56, 0x46, 0x46, 0x4F, 0x20, 0x03, 0xDC, 0x05, 0xA0, 0x0F, 0x5C, 0x12 };

	decodeVoise0(data);

	textread("<s10>aiueokasatanahamayarawagazadabapa");
	std::vector<nsfloat> test = textreading();

	nsfloat max = (nsfloat)0.0;
	for (auto t : test)
		if (abs(t) > max)
			max = abs(t);
	maxVolume = max;

	return;
}

//音諳 詞音を使う
void makeVoiseFromFile::setShion() {

	unsigned char data[68] = { 0x44,0x41,0x54,0x41,0x62,0x66,0x88,0x01,0x56,0x46,0x46,0x41,0x54,0x04,0xA4,0x06,0x6F,0x0F,0x9A,0x14,0x56,0x46,0x46,0x49,0x93,0x02,0xC0,0x0D,0x6F,0x0F,0x9A,0x14,0x56,0x46,0x46,0x55,0x93,0x02,0xE0,0x06,0x6F,0x0F,0x9A,0x14,0x56,0x46,0x46,0x45,0x10,0x03,0x90,0x0B,0x6F,0x0F,0x9A,0x14,0x56,0x46,0x46,0x4F,0x10,0x03,0x75,0x05,0x6F,0x0F,0x9A,0x14 };

	decodeVoise0(data);

	textread("<s10>aiueokasatanahamayarawagazadabapa");
	std::vector<nsfloat> test = textreading();

	nsfloat max = (nsfloat)0.0;
	for (auto t : test)
		if (abs(t) > max)
			max = abs(t);
	maxVolume = max;

	return;
}

//音諳書式で書かれた文章を読み込み
void makeVoiseFromFile::textread(const char* t) {

	/*
	* text...読み上げ文章
	* 音諳形式の説明
	* 文字      ...ローマ字のみ。やっぱりマルチバイト嫌いだわ。というより、文字コード関係が嫌い
	* <S040>    ...スピードの変更。一文字の長さを指定。最大256、最小01
	* <P±000>  ...ピッチの変更。bfからどのくらい離れるか。-32768 ～ +32767
	* <pu><pd>  ...ピッチの変更。puでピッチを十上げて、pdで十下がる。
	* <bf>		...bfの設定。
	*/

	pstringdata = NSString(t);

	return;
}

//textreadで読み込んだ文章を読み上げ
std::vector<nsfloat> makeVoiseFromFile::textreading() {

	//全体の時間[s]
	const size_t alltime = pstringdata.alltime() * (size_t)CHANGETIMEMAGN;

	//完成品
	std::vector<nsfloat> out;

	//今処理している時間
	nsint targettime = 0;

	//前の文字、スピード、ピッチ
	nschar oldmoji = pstringdata[0].c;
	nsint oldspeed = pstringdata[0].ctime;
	int16_t oldpitch = pstringdata[0].pitch;

	//今の文字、スピード、ピッチ
	nschar nowmoji = 0;
	nsint nowspeed = 0;
	int16_t nowpitch = 0;

	//次の文字、スピード、ピッチ
	nschar nextmoji = 0;
	nsint nextspeed = 0;
	int16_t nextpitch = 0;

	//終了
	bool fin = false;

	//mvffMMVFを初期化
	mvffMMVF.setSoftness(softness);

	ftInit(MAXSHIONSPEED);
	//メイン処理
	for (size_t nowmojinum = 0;nowmojinum < pstringdata.length();nowmojinum++) {
		//それぞれの変数を更新
		nowmoji = pstringdata[nowmojinum].c;
		nowspeed = pstringdata[nowmojinum].ctime * (nsint)CHANGETIMEMAGN;
		nowpitch = pstringdata[nowmojinum].pitch;
		if (nowmojinum < pstringdata.length() - 1) {
			nextmoji = pstringdata[nowmojinum + 1].c;
			nextspeed = pstringdata[nowmojinum + 1].ctime * (nsint)CHANGETIMEMAGN;
			nextpitch = pstringdata[nowmojinum + 1].pitch;
		}
		else {
			nextmoji = nextspeed = nextpitch = 0;
		}
		targettime = out.size() + nowspeed;
		//倍音設定
		mvffMMVF.makeHarmonicOvertone(privatebf + nowpitch);

		nsfloat formantmagn = 0;
		//声の前半部メイン(子音部)
		switch (nowmoji) {
		case NSNULL:
			fin = true;
			break;
		case NSa:
		case NSi:
		case NSu:
		case NSe:
		case NSo:
			//あ～お
			//子音無し
			break;
		case NSya:
		case NSyu:
		case NSye:
		case NSyo:
		case NSwa:
		case NSwi:
		case NSwe:
		case NSwo:
			//や～よ、わ～を

			textreadingHanboin(out, nowmoji, nowpitch, targettime);

			break;
			//==========か行・が行===========
		case NSka:
		case NSki:
		case NSku:
		case NSke:
		case NSko:
			//か～こ

			textreadingHaretuon(out, nowmoji, nowmoji, nowpitch, targettime);

			break;
		case NSkya:
		case NSkyu:
		case NSkye:
		case NSkyo:
			//きゃ～きょ

			textreadingHaretuon(out, nowmoji, 0x12, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSkwa:
		case NSkwi:
		case NSkwe:
		case NSkwo:
			//くぁ～くぉ

			textreadingHaretuon(out, nowmoji, 0x17, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSga:
		case NSgi:
		case NSgu:
		case NSge:
		case NSgo:
			//が～ご

			textreadingYuuseiHaretuon(out, nowmoji, nowmoji, nowpitch, targettime);

			break;
		case NSgya:
		case NSgyu:
		case NSgye:
		case NSgyo:
			//ぎゃ～ぎょ

			textreadingYuuseiHaretuon(out, nowmoji, 0x22, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSgwa:
		case NSgwi:
		case NSgwe:
		case NSgwo:
			//ぐぁ～ぐぉ

			textreadingYuuseiHaretuon(out, nowmoji, 0x27, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
			//==========さ行・ざ行===========
		case NSsa:
		case NSsi:
		case NSsu:
		case NSse:
		case NSso:
			//さ～そ

			textreadingMasatuon(out, nowmoji, nowmoji, nowpitch, targettime);

			break;
		case NSsya:
		case NSsyu:
		case NSsye:
		case NSsyo:
			//しゃ～しょ

			textreadingMasatuon(out, nowmoji, 0x32, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSswa:
		case NSswi:
		case NSswe:
		case NSswo:
			//すぁ～すぉ

			textreadingMasatuon(out, nowmoji, 0x37, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSza:
		case NSzi:
		case NSzu:
		case NSze:
		case NSzo:
			//ざ～ぞ

			textreadingYuuseiMasatuon(out, nowmoji, nowmoji, nowpitch, targettime);

			break;
		case NSja:
		case NSju:
		case NSje:
		case NSjo:
			//じゃ～じょ

			textreadingYuuseiMasatuon(out, nowmoji, 0x42, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSzwa:
		case NSzwi:
		case NSzwe:
		case NSzwo:
			//じゃ～じょ

			textreadingYuuseiMasatuon(out, nowmoji, 0x47, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
			//==========た行・だ行===========
		case NSta:
		case NSti:
		case NStu:
		case NSte:
		case NSto:
			//た～と

			textreadingHaretuon(out, nowmoji, nowmoji, nowpitch, targettime);

			break;
		case NStya:
		case NStyu:
		case NStye:
		case NStyo:
			//ちゃ～ちょ

			textreadingHaretuon(out, nowmoji, 0x52, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NStwa:
		case NStwi:
		case NStwe:
		case NStwo:
			//つぁ～つぉ

			textreadingHaretuon(out, nowmoji, 0x57, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSda:
		case NSdi:
		case NSdu:
		case NSde:
		case NSdo:
			//だ～ど

			textreadingYuuseiHaretuon(out, nowmoji, nowmoji, nowpitch, targettime, false);

			break;
		case NSdya:
		case NSdyu:
		case NSdye:
		case NSdyo:
			//ぢゃ～ぢょ

			textreadingYuuseiHaretuon(out, nowmoji, 0x62, nowpitch, targettime, false);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSdwa:
		case NSdwi:
		case NSdwe:
		case NSdwo:
			//づぁ～づぉ

			textreadingYuuseiHaretuon(out, nowmoji, 0x67, nowpitch, targettime, false);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
			//==========ぱ行・ば行===========
		case NSpa:
		case NSpi:
		case NSpu:
		case NSpe:
		case NSpo:
			//ぱ～ぽ

			textreadingHaretuon(out, nowmoji, nowmoji, nowpitch, targettime);

			break;
		case NSpya:
		case NSpyu:
		case NSpye:
		case NSpyo:
			//ぴゃ～ぴょ

			textreadingHaretuon(out, nowmoji, 0x72, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSpwa:
		case NSpwi:
		case NSpwe:
		case NSpwo:
			//ぷぁ～ぷぉ

			textreadingHaretuon(out, nowmoji, 0x77, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSba:
		case NSbi:
		case NSbu:
		case NSbe:
		case NSbo:
			//ば～ぼ

			textreadingYuuseiHaretuon(out, nowmoji, nowmoji, nowpitch, targettime, false);

			break;
		case NSbya:
		case NSbyu:
		case NSbye:
		case NSbyo:
			//びゃ～びょ

			textreadingYuuseiHaretuon(out, nowmoji, 0x82, nowpitch, targettime, false);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSbwa:
		case NSbwi:
		case NSbwe:
		case NSbwo:
			//ぶぁ～ぶぉ

			textreadingYuuseiHaretuon(out, nowmoji, 0x87, nowpitch, targettime, false);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
			//==========は行==========
		case NSha:
		case NShi:
		case NShu:
		case NShe:
		case NSho:
			//は～ほ

			textreadingMASATUON(out, nowmoji, nowpitch, targettime);

			break;
		case NShya:
		case NShyu:
		case NShye:
		case NShyo:
			//ひゃ～ひょ

			textreadingMASATUON(out, 0x92, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NShwa:
		case NShwi:
		case NShwe:
		case NShwo:
			//ふぁ～ふぉ

			textreadingMASATUON(out, 0x97, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
			//==========な行・ま行==========
		case NSna:
		case NSni:
		case NSnu:
		case NSne:
		case NSno:
			//な～の

			textreadingBion(out, nowmoji, nowmoji, oldmoji, nowpitch, targettime);

			break;
		case NSnya:
		case NSnyu:
		case NSnye:
		case NSnyo:
			//にゃ～にょ

			textreadingBion(out, nowmoji, 0xa2, oldmoji, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSnwa:
		case NSnwi:
		case NSnwe:
		case NSnwo:
			//ぬぁ～ぬぉ

			textreadingBion(out, nowmoji, 0xa7, oldmoji, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSma:
		case NSmi:
		case NSmu:
		case NSme:
		case NSmo:
			//ま～も

			textreadingBion(out, nowmoji, nowmoji, oldmoji, nowpitch, targettime);

			break;
		case NSmya:
		case NSmyu:
		case NSmye:
		case NSmyo:
			//みゃ～みょ

			textreadingBion(out, nowmoji, 0xb2, oldmoji, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSmwa:
		case NSmwi:
		case NSmwe:
		case NSmwo:
			//むぁ～むぉ

			textreadingBion(out, nowmoji, 0xb7, oldmoji, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
			//==========ら行==========
		case NSra:
		case NSri:
		case NSru:
		case NSre:
		case NSro:
			//ら～ろ

			textreadingHajikion(out, nowmoji, nowmoji, oldmoji, nowpitch, targettime);

			break;
		case NSrya:
		case NSryu:
		case NSrye:
		case NSryo:
			//りゃ～りょ

			textreadingHajikion(out, nowmoji, 0xc2, oldmoji, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSrwa:
		case NSrwi:
		case NSrwe:
		case NSrwo:
			//るぁ～るぉ

			textreadingHajikion(out, nowmoji, 0xc7, oldmoji, nowpitch, targettime);
			textreadingHanboin(out, nowmoji & 0x0f, nowpitch, targettime);

			break;
		case NSnn1:
			//ん
			//子音無し
			break;
		case NSspc:
			//空白文字
			for (nsint i = out.size(); i < targettime;i++)
				out.push_back(0.0);
			break;
		default:
			break;
		}


		//声の母音部メイン(母音だけ)
		textreadingBoin1(out, nowmoji, nowpitch, targettime);

		//声の後半部メイン(次の音へ向けてのフォルマント変移部)
		if (nowmoji == NSspc)
			// unprocessed
			;
		else {
			textreadingBoin2(out, nowmoji, nextmoji, nowpitch, nextpitch, targettime);
		}

		oldmoji = nowmoji;oldspeed = nowspeed;oldpitch = nowpitch;

		if (fin)break;
	}

	nsfloat max = (nsfloat)1.0 / maxVolume;
	for (size_t i = 0;i < out.size();i++) {
		out[i] *= max;
		if (out[i] > (nsfloat)1.0) out[i] = (nsfloat)1.0;
		else if (out[i] < (nsfloat)-1.0) out[i] = (nsfloat)-1.0;
	}

	return out;
}

//=====声作成(子音)=====

void makeVoiseFromFile::textreadingHanboin(std::vector<nsfloat>& out, nschar nowmoji, nsfloat pitch, nsint targettime) {
	std::vector<nsfloat> vout1(MAXYOUONSPEED + CTIME * 2, 1.0);
	std::vector<nsfloat> vout2(MAXYOUONSPEED + CTIME * 2, 1.0);

	mvffMMVF.setFormants(getMojiFormant(0, nowmoji, 0) + pitch, getMojiFormant(0, nowmoji, 1) + pitch, getMojiFormant(0, nowmoji, 2) + pitch, getMojiFormant(0, nowmoji, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);
	mvffMMVF.setFormants(getMojiFormant(1, nowmoji, 0) + pitch, getMojiFormant(1, nowmoji, 1) + pitch, getMojiFormant(1, nowmoji, 2) + pitch, getMojiFormant(1, nowmoji, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

	for (nsint j = 0; j < MAXSHIONSPEED;j++)
		out.push_back(vout1[j + CTIME] * ((nsfloat)(MAXSHIONSPEED - j) * MAXSHIONSPEED_INV) + vout2[j + CTIME] * ((nsfloat)j * MAXSHIONSPEED_INV));
}

void makeVoiseFromFile::textreadingHaretuon(std::vector<nsfloat>& out, nschar nowmoji1, nschar nowmoji2, nsfloat pitch, nsint targettime) {
	for (nsint j = 0;j < MAXSHIONSPEED * 0.5;j++)
		out.push_back(0.0);

	const nsint size = (nsint)(MAXSHIONSPEED * 0.5);
	std::vector<nsfloat> vout1(size + CTIME * 2, 1.0);
	std::vector<nsfloat> vout2(size + CTIME * 2, 1.0);

	mvffMMVF.setFormants(getMojiFormant(0, nowmoji1, 0) + pitch, getMojiFormant(0, nowmoji1, 1) + pitch, getMojiFormant(0, nowmoji1, 2) + pitch, getMojiFormant(0, nowmoji1, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);
	mvffMMVF.setFormants(getMojiFormant(1, nowmoji2, 0) + pitch, getMojiFormant(1, nowmoji2, 1) + pitch, getMojiFormant(1, nowmoji2, 2) + pitch, getMojiFormant(1, nowmoji2, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

	for (nsint j = 0; j < size;j++)
		out.push_back(((j < PLOSIVESPEED * 2) ? vout1[j + CTIME] * ((j < PLOSIVESPEED) ? ((nsfloat)j / (nsfloat)PLOSIVESPEED * (nsfloat)2.0) : ((nsfloat)(PLOSIVESPEED * 2 - j) / (nsfloat)PLOSIVESPEED * 2.0)) * (nsfloat)PLOSIVEPOWER : 0.0) * (nsfloat)0.5 + vout2[j + CTIME] * ((nsfloat)j / (nsfloat)size));


}

void makeVoiseFromFile::textreadingYuuseiHaretuon(std::vector<nsfloat>& out, nschar nowmoji1, nschar nowmoji2, nsfloat pitch, nsint targettime, bool select) {

	if (select) {
		for (nsint j = 0;j < MAXSHIONSPEED * 0.25;j++)
			out.push_back(0.0);

		const nsint size = (nsint)(MAXSHIONSPEED * 0.25);
		std::vector<nsfloat> vout2(size + CTIME * 2, 1.0);
		std::vector<nsfloat> vout3(size + CTIME * 2, 1.0);
		mvffMMVF.setFormants(getMojiFormant(1, nowmoji1, 0) + pitch, getMojiFormant(1, nowmoji1, 1) + pitch, getMojiFormant(1, nowmoji1, 2) + pitch, getMojiFormant(1, nowmoji1, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);
		mvffMMVF.setFormants(getMojiFormant(0, 0xd1, 0) + pitch, getMojiFormant(0, 0xd1, 1) + pitch, getMojiFormant(0, 0xd1, 2) + pitch, getMojiFormant(0, 0xd1, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout3, out.size() - CTIME);

		for (nsint j = 0; j < size;j++)
			out.push_back((vout2[j + CTIME] * ((j < size * 0.5) ? ((nsfloat)j / size) : ((nsfloat)(size - j) / (nsfloat)size)) + vout3[j + CTIME] * ((j < size * 0.5) ? ((nsfloat)j / (nsfloat)size) : ((nsfloat)(size - j) / (nsfloat)size))) * (nsfloat)0.25);

		std::vector<nsfloat> vout1(size + CTIME * 2, 1.0);
		std::fill(vout2.begin(), vout2.end(), (nsfloat)1.0);
		std::fill(vout3.begin(), vout3.end(), (nsfloat)1.0);

		mvffMMVF.setFormants(getMojiFormant(0, nowmoji1, 0) + pitch, getMojiFormant(0, nowmoji1, 1) + pitch, getMojiFormant(0, nowmoji1, 2) + pitch, getMojiFormant(0, nowmoji1, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);
		mvffMMVF.setFormants(getMojiFormant(1, nowmoji2, 0) + pitch, getMojiFormant(1, nowmoji2, 1) + pitch, getMojiFormant(1, nowmoji2, 2) + pitch, getMojiFormant(1, nowmoji2, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);
		mvffMMVF.setFormants(getMojiFormant(0, 0xd1, 0) + pitch, getMojiFormant(0, 0xd1, 1) + pitch, getMojiFormant(0, 0xd1, 2) + pitch, getMojiFormant(0, 0xd1, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout3, out.size() - CTIME);

		for (nsint j = 0; j < size;j++)
			out.push_back((nsfloat)((j < PLOSIVESPEED * 2) ? vout1[j + CTIME] * ((j < PLOSIVESPEED) ? ((nsfloat)j / (nsfloat)PLOSIVESPEED * (nsfloat)2.0) : ((nsfloat)(PLOSIVESPEED * 2 - j) / (nsfloat)PLOSIVESPEED * (nsfloat)2.0)) : 0.0) * (nsfloat)0.25 + (vout2[j + CTIME] * ((nsfloat)j / (nsfloat)size) + vout3[j + CTIME] * ((nsfloat)(size - j) / (nsfloat)size) / (nsfloat)3.0));

	}
	else {
		const nsint size = (nsint)(MAXSHIONSPEED * 0.5);
		std::vector<nsfloat> vout1(size + CTIME * 2, 1.0);
		std::vector<nsfloat> vout2(size + CTIME * 2, 1.0);
		mvffMMVF.setFormants(getMojiFormant(1, nowmoji1, 0) + pitch, getMojiFormant(1, nowmoji1, 1) + pitch, getMojiFormant(1, nowmoji1, 2) + pitch, getMojiFormant(1, nowmoji1, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);
		mvffMMVF.setFormants(getMojiFormant(0, 0xd1, 0) + pitch, getMojiFormant(0, 0xd1, 1) + pitch, getMojiFormant(0, 0xd1, 2) + pitch, getMojiFormant(0, 0xd1, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);

		for (nsint j = 0; j < size;j++)
			out.push_back((vout2[j + CTIME] * ((j < size * 0.5) ? ((nsfloat)j / (nsfloat)size) : ((nsfloat)(size - j) / (nsfloat)size)) + vout1[j + CTIME] * ((j < size * 0.5) ? ((nsfloat)j / (nsfloat)size) : ((nsfloat)(size - j) / (nsfloat)size))) * (nsfloat)0.25);

		std::fill(vout1.begin(), vout1.end(), (nsfloat)1.0);
		std::fill(vout2.begin(), vout2.end(), (nsfloat)1.0);

		mvffMMVF.setFormants(getMojiFormant(0, nowmoji1, 0) + pitch, getMojiFormant(0, nowmoji1, 1) + pitch, getMojiFormant(0, nowmoji1, 2) + pitch, getMojiFormant(0, nowmoji1, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);
		mvffMMVF.setFormants(getMojiFormant(1, nowmoji2, 0) + pitch, getMojiFormant(1, nowmoji2, 1) + pitch, getMojiFormant(1, nowmoji2, 2) + pitch, getMojiFormant(1, nowmoji2, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

		for (nsint j = 0; j < size;j++)
			out.push_back(((j < PLOSIVESPEED * 2) ? vout1[j + CTIME] * ((j < PLOSIVESPEED) ? ((nsfloat)j / (nsfloat)PLOSIVESPEED * (nsfloat)2.0) : ((PLOSIVESPEED * 2.0 - j) / (nsfloat)PLOSIVESPEED * (nsfloat)2.0)) * (nsfloat)PLOSIVEPOWER : 0.0) * (nsfloat)0.5 + vout2[j + CTIME] * ((nsfloat)j / (nsfloat)size));

	}

}

void makeVoiseFromFile::textreadingMasatuon(std::vector<nsfloat>& out, nschar nowmoji1, nschar nowmoji2, nsfloat pitch, nsint targettime) {
	const nsint size = (nsint)(MAXSHIONSPEED * 0.5);
	//ホワイトノイズ
	std::vector<nsfloat> voutw(MAXSHIONSPEED, 0.0);
	std::vector<nsfloat> voutw2(MAXSHIONSPEED, 0.0);
	std::vector<nsfloat> voutw3(MAXSHIONSPEED, 0.0);
	std::vector<nsfloat> voutw4(MAXSHIONSPEED, 0.0);

	mvffMMVF.makeSinha(getMojiFormant(1, nowmoji1, 0), 0.5, voutw);
	mvffMMVF.makeSinha(getMojiFormant(1, nowmoji1, 1), 0.5, voutw2);
	mvffMMVF.makeSinha(getMojiFormant(1, nowmoji1, 2), 0.5, voutw3);
	mvffMMVF.makeSinha(getMojiFormant(1, nowmoji1, 3), 0.5, voutw4);

	for (nsint i = 0; i < MAXSHIONSPEED;i++) {
		voutw[i] += voutw2[i] * (nsfloat)0.5 + voutw3[i] + voutw4[i];
		voutw[i] *= 0.25;
		voutw2[i] = 0;
	}

	hipassFilter(voutw, voutw2, getMojiFormant(1, nowmoji1, 1), 0.707);
	for (nsint i = 0; i < MAXSHIONSPEED;i++)voutw[i] = 0.0;
	lowpassFilter(voutw2, voutw, getMojiFormant(1, nowmoji1, 0), 1.0);

	for (nsint j = 0; j < (MAXSHIONSPEED - size) * 0.5;j++)out.push_back(0.0);
	for (nsint j = 0;j < (MAXSHIONSPEED - size) * 0.5;j++)
		out.push_back(voutw[j] * ((nsfloat)j / (nsfloat)(MAXSHIONSPEED - size)) * (nsfloat)0.5);

	std::vector<nsfloat> vout2(size + CTIME * 2, 1.0);   //母音

	mvffMMVF.setFormants(getMojiFormant(1, nowmoji2, 0) + pitch, getMojiFormant(1, nowmoji2, 1) + pitch, getMojiFormant(1, nowmoji2, 2) + pitch, getMojiFormant(1, nowmoji2, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

	for (nsint j = 0; j < size;j++)
		out.push_back(vout2[j + CTIME] * ((nsfloat)j / (nsfloat)size) + voutw[j + size] * ((nsfloat)(size - j) / (nsfloat)size) * (nsfloat)0.5);

}

void makeVoiseFromFile::textreadingYuuseiMasatuon(std::vector<nsfloat>& out, nschar nowmoji1, nschar nowmoji2, nsfloat pitch, nsint targettime) {
	const nsint size = (nsint)(MAXSHIONSPEED * 0.5);
	const nsint size2 = (nsint)(MAXSHIONSPEED * 0.25);
	std::vector<nsfloat> vout2(size2 + CTIME * 2, 1.0);   //母音
	std::vector<nsfloat> vout3(size2 + CTIME * 2, 1.0);   //鼻音
	mvffMMVF.setFormants(getMojiFormant(1, nowmoji1, 0) + pitch, getMojiFormant(1, nowmoji1, 1) + pitch, getMojiFormant(1, nowmoji1, 2) + pitch, getMojiFormant(1, nowmoji1, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);
	mvffMMVF.setFormants(getMojiFormant(0, 0xd1, 0) + pitch, getMojiFormant(0, 0xd1, 1) + pitch, getMojiFormant(0, 0xd1, 2) + pitch, getMojiFormant(0, 0xd1, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout3, out.size() - CTIME);

	for (nsint j = 0; j < size2;j++)
		out.push_back((vout2[j + CTIME] * ((j < size2 * 0.5) ? ((nsfloat)j / (nsfloat)size2) : ((nsfloat)(size2 - j) / (nsfloat)size2)) + vout3[j + CTIME] * ((j < size2 * 0.5) ? ((nsfloat)j / (nsfloat)size2) : ((nsfloat)(size2 - j) / (nsfloat)size2))) * (nsfloat)0.25);

	//ホワイトノイズ
	std::vector<nsfloat> voutw(MAXSHIONSPEED, 0.0);
	std::vector<nsfloat> voutw2(MAXSHIONSPEED, 0.0);

	mvffMMVF.makeSinha(getMojiFormant(1, nowmoji1, 0), 0.5, voutw);
	mvffMMVF.makeSinha(getMojiFormant(1, nowmoji1, 1), 0.5, voutw2);
	for (nsint i = 0; i < MAXSHIONSPEED;i++) {
		voutw[i] += voutw2[i];
		voutw2[i] = 0;
	}
	mvffMMVF.makeSinha(getMojiFormant(1, nowmoji1, 2), 0.5, voutw2);
	for (nsint i = 0; i < MAXSHIONSPEED;i++) {
		voutw[i] += voutw2[i];
		voutw2[i] = 0;
	}
	mvffMMVF.makeSinha(getMojiFormant(1, nowmoji1, 3), 0.5, voutw2);
	for (nsint i = 0; i < MAXSHIONSPEED;i++) {
		voutw[i] += voutw2[i];
		voutw[i] *= 0.25;
		voutw2[i] = 0;
	}

	hipassFilter(voutw, voutw2, getMojiFormant(1, nowmoji1, 1), 0.707);
	for (nsint i = 0; i < MAXSHIONSPEED;i++)
		voutw[i] = 0.0;
	lowpassFilter(voutw2, voutw, getMojiFormant(1, nowmoji1, 0), 1.0);

	for (nsint j = 0;j < size * 0.5;j++)
		out.push_back(voutw[j] * ((nsfloat)j / (nsfloat)(MAXSHIONSPEED - size)) * (nsfloat)0.5);

	std::vector<nsfloat> vout1(size + CTIME * 2, 1.0);

	mvffMMVF.setFormants(getMojiFormant(1, nowmoji2, 0) + pitch, getMojiFormant(1, nowmoji2, 1) + pitch, getMojiFormant(1, nowmoji2, 2) + pitch, getMojiFormant(1, nowmoji2, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);

	for (nsint j = 0; j < size;j++)
		out.push_back(vout1[j + CTIME] * ((nsfloat)j / (nsfloat)size) + voutw[j + size] * ((nsfloat)(size - j) / (nsfloat)size) * (nsfloat)0.5);

}

void makeVoiseFromFile::textreadingMASATUON(std::vector<nsfloat>& out, nschar nowmoji, nsfloat pitch, nsint targettime) {
	const nsint size = MAXSHIONSPEED;
	std::vector<nsfloat> vout1(size + CTIME * 2, 1.0);   //摩殺音
	std::vector<nsfloat> vout2(size + CTIME * 2, 1.0);   //母音
	FriKILLive h = {};
	h.size = (size_t)(MAXFREQ / privatebf);
	h.Hz.resize(h.size);
	h.s.resize(h.size);
	for (size_t i = 0; i < h.size;i++) {
		h.Hz[i] = privatebf * i;
		h.s[i] = 1.0;
	}

	mvffMMVF.makeSinWave(h.size, h.Hz, h.s, vout1, out.size() - CTIME, false);

	mvffMMVF.setFormants(getMojiFormant(0, nowmoji, 0) + pitch, getMojiFormant(0, nowmoji, 1) + pitch, getMojiFormant(0, nowmoji, 2) + pitch, getMojiFormant(0, nowmoji, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

	for (nsint j = 0; j < size;j++)
		out.push_back(((j < size) ? vout1[j + CTIME] * ((j < size * 0.5) ? ((nsfloat)j / (nsfloat)size) : ((nsfloat)(size - j) / (nsfloat)size)) : 0) * (nsfloat)0.25 + vout2[j + CTIME] * ((nsfloat)j / (nsfloat)size));

}

void makeVoiseFromFile::textreadingBion(std::vector<nsfloat>& out, nschar nowmoji1, nschar nowmoji2, nschar oldmoji, nsfloat pitch, nsint targettime) {
	const nsint size = MAXSHIONSPEED;
	const nsint ssize = (nsint)(MAXSHIONSPEED * 0.5) - NASALSPEED;
	nsint jtime = 0;
	std::vector<nsfloat> vout1(size + CTIME * 2, 1.0);   //鼻音
	std::vector<nsfloat> vout2(size + CTIME * 2, 1.0);   //母音

	mvffMMVF.setFormants(getMojiFormant(0, nowmoji1, 0) + pitch, getMojiFormant(0, nowmoji1, 1) + pitch, getMojiFormant(0, nowmoji1, 2) + pitch, getMojiFormant(0, nowmoji1, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);
	mvffMMVF.setFormants(getMojiFormant(1, nowmoji2, 0) + pitch, getMojiFormant(1, nowmoji2, 1) + pitch, getMojiFormant(1, nowmoji2, 2) + pitch, getMojiFormant(1, nowmoji2, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

	if (oldmoji == NSNULL or oldmoji == NSspc) {
		for (nsint j = 0; j < ssize;j++) {
			out.push_back(vout1[jtime + CTIME] * ((nsfloat)j / (nsfloat)ssize) * (nsfloat)0.5);
			jtime++;
		}
	}
	else {
		for (nsint j = 0; j < ssize;j++) {
			out.push_back(vout1[jtime + CTIME] * 0.5);
			jtime++;
		}
	}
	for (nsint j = 0;j < NASALSPEED;j++) {
		out.push_back(vout1[jtime + CTIME] * ((nsfloat)(NASALSPEED - j) / (nsfloat)NASALSPEED) * (nsfloat)0.5 + vout2[jtime + CTIME] * ((nsfloat)j / (nsfloat)NASALSPEED));
		jtime++;
	}
	for (nsint j = 0; jtime == size;j++) {
		out.push_back(vout2[jtime + CTIME]);
		jtime++;
	}
}

void makeVoiseFromFile::textreadingHajikion(std::vector<nsfloat>& out, nschar nowmoji1, nschar nowmoji2, nschar oldmoji, nsfloat pitch, nsint targettime) {
	const nsint size = MAXSHIONSPEED;
	const nsint ssize = (nsint)(MAXSHIONSPEED * 0.5) - TAPSPEED;
	nsint jtime = 0;
	std::vector<nsfloat> vout1(size + CTIME * 2, 1.0);   //鼻音
	std::vector<nsfloat> vout2(size + CTIME * 2, 1.0);   //母音

	mvffMMVF.setFormants(getMojiFormant(0, nowmoji1, 0) + pitch, getMojiFormant(0, nowmoji1, 1) + pitch, getMojiFormant(0, nowmoji1, 2) + pitch, getMojiFormant(0, nowmoji1, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);
	mvffMMVF.setFormants(getMojiFormant(1, nowmoji2, 0) + pitch, getMojiFormant(1, nowmoji2, 1) + pitch, getMojiFormant(1, nowmoji2, 2) + pitch, getMojiFormant(1, nowmoji2, 3) + pitch);
	mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

	if (oldmoji == NSNULL or oldmoji == NSspc) {
		for (nsint j = 0; j < ssize;j++) {
			out.push_back(vout1[jtime + CTIME] * ((nsfloat)j / (nsfloat)ssize) * (nsfloat)0.5);
			jtime++;
		}
	}
	else {
		for (nsint j = 0; j < ssize;j++) {
			out.push_back(vout1[jtime + CTIME] * (nsfloat)0.5);
			jtime++;
		}
	}
	for (nsint j = 0;j < NASALSPEED;j++) {
		out.push_back(vout1[jtime + CTIME] * ((nsfloat)(TAPSPEED - j) / (nsfloat)TAPSPEED) * (nsfloat)0.5 + vout2[jtime + CTIME] * ((nsfloat)j / (nsfloat)TAPSPEED));
		jtime++;
	}
	for (nsint j = 0; jtime == size;j++) {
		out.push_back(vout2[jtime + CTIME]);
		jtime++;
	}
}

//=====声生成(母音)=====

void makeVoiseFromFile::textreadingBoin1(std::vector<nsfloat>& out, nschar nowmoji, nsfloat pitch, nsint targettime) {
	mvffMMVF.setFormants(getMojiFormant(1, nowmoji, 0) + pitch, getMojiFormant(1, nowmoji, 1) + pitch, getMojiFormant(1, nowmoji, 2) + pitch, getMojiFormant(1, nowmoji, 3) + pitch);
	const nsint size = targettime - SEMIVOWELSPEED - out.size();
	if (size > 0) {
		std::vector<nsfloat> vout(size + CTIME * 2, 1.0);

		mvffMMVF.makeSinWaveFromClass(vout, out.size() - CTIME);

		for (nsint j = 0; j < size;j++)
			out.push_back(vout[j + CTIME]);
	}
}

void makeVoiseFromFile::textreadingBoin2(std::vector<nsfloat>& out, nschar nowmoji, nschar nextmoji, nsfloat pitch, nsfloat nextpitch, nsint targettime) {
	//終わり or 空白
	if (nextmoji == NSNULL or nextmoji == NSspc) {
		std::vector<nsfloat> vout(SEMIVOWELSPEED + CTIME * 2, 1.0);

		mvffMMVF.makeSinWaveFromClass(vout, out.size() - CTIME);

		for (nsint j = 0; j < SEMIVOWELSPEED;j++)
			out.push_back(vout[j + CTIME] * ((nsfloat)(SEMIVOWELSPEED - j) * SEMIVOWELSPEED_INV));
	}
	//破裂音
	else if (doVoiselessPlosive(nextmoji) or doVoisedPlosive(nextmoji)) {
		std::vector<nsfloat> vout1(SEMIVOWELSPEED + CTIME * 2, 1.0);
		std::vector<nsfloat> vout2(SEMIVOWELSPEED + CTIME * 2, 1.0);

		mvffMMVF.setFormants(getMojiFormant(1, nowmoji, 0) + pitch, getMojiFormant(1, nowmoji, 1) + pitch, getMojiFormant(1, nowmoji, 2) + pitch, getMojiFormant(1, nowmoji, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);
		mvffMMVF.setFormants(getMojiFormant(0, nextmoji, 0) + nextpitch, getMojiFormant(0, nextmoji, 1) + nextpitch, getMojiFormant(0, nextmoji, 2) + nextpitch, getMojiFormant(0, nextmoji, 3) + nextpitch);
		mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

		for (nsint j = 0; j < SEMIVOWELSPEED * 0.5;j++)
			out.push_back(vout1[j + CTIME] * ((nsfloat)(SEMIVOWELSPEED - j) * SEMIVOWELSPEED_INV) + vout2[j + CTIME] * ((nsfloat)j * SEMIVOWELSPEED_INV));

		for (nsint j = out.size();j < targettime;j++)
			out.push_back(0.0);
	}
	//鼻音
	else if (doNasal(nextmoji)) {
		std::vector<nsfloat> vout1(SEMIVOWELSPEED + CTIME * 2, 1.0);
		std::vector<nsfloat> vout2(SEMIVOWELSPEED + CTIME * 2, 1.0);

		mvffMMVF.setFormants(getMojiFormant(1, nowmoji, 0) + pitch, getMojiFormant(1, nowmoji, 1) + pitch, getMojiFormant(1, nowmoji, 2) + pitch, getMojiFormant(1, nowmoji, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);
		mvffMMVF.setFormants(getMojiFormant(0, nextmoji, 0) + nextpitch, getMojiFormant(0, nextmoji, 1) + nextpitch, getMojiFormant(0, nextmoji, 2) + nextpitch, getMojiFormant(0, nextmoji, 3) + nextpitch);
		mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

		for (nsint j = 0; j < SEMIVOWELSPEED;j++)
			out.push_back(vout1[j + CTIME] * ((nsfloat)(SEMIVOWELSPEED - j) * SEMIVOWELSPEED_INV) + vout2[j + CTIME] * ((nsfloat)j * SEMIVOWELSPEED_INV) * 0.5);
	}
	//はじき音
	else if (doTap(nextmoji)) {
		std::vector<nsfloat> vout1(SEMIVOWELSPEED + CTIME * 2, 1.0);
		std::vector<nsfloat> vout2(SEMIVOWELSPEED + CTIME * 2, 1.0);

		mvffMMVF.setFormants(getMojiFormant(1, nowmoji, 0) + pitch, getMojiFormant(1, nowmoji, 1) + pitch, getMojiFormant(1, nowmoji, 2) + pitch, getMojiFormant(1, nowmoji, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);
		mvffMMVF.setFormants(getMojiFormant(0, nextmoji, 0) + nextpitch, getMojiFormant(0, nextmoji, 1) + nextpitch, getMojiFormant(0, nextmoji, 2) + nextpitch, getMojiFormant(0, nextmoji, 3) + nextpitch);
		mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

		for (nsint j = 0; j < SEMIVOWELSPEED;j++)
			out.push_back(vout1[j + CTIME] * ((nsfloat)(SEMIVOWELSPEED - j) * SEMIVOWELSPEED_INV) + vout2[j + CTIME] * ((nsfloat)j * SEMIVOWELSPEED_INV));
	}
	//摩擦音
	else if (doVoiselessFricative(nextmoji)) {
		std::vector<nsfloat> vout1(SEMIVOWELSPEED + CTIME * 2, 1.0);

		mvffMMVF.setFormants(getMojiFormant(1, nowmoji, 0) + pitch, getMojiFormant(1, nowmoji, 1) + pitch, getMojiFormant(1, nowmoji, 2) + pitch, getMojiFormant(1, nowmoji, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);

		for (nsint j = 0; j < SEMIVOWELSPEED;j++)
			out.push_back(vout1[j + CTIME] * ((nsfloat)(SEMIVOWELSPEED - j) * SEMIVOWELSPEED_INV));
	}
	//摩殺音(笑)
	else if (doFriKILLive(nextmoji)) {
		//ホワイトノイズ
		std::vector<nsfloat> voutw(MAXSHIONSPEED, 0.0);
		std::vector<nsfloat> voutw2(MAXSHIONSPEED, 0.0);
		ftInit(MAXSHIONSPEED);
		makeWhitenoise1(voutw, out.size(), getMojiFormant(1, nowmoji, 3));

		hipassFilter(voutw, voutw2, getMojiFormant(1, nowmoji, 3), 0.707);
		for (nsint i = 0; i < MAXSHIONSPEED;i++)voutw[i] = 0.0;
		lowpassFilter(voutw2, voutw, getMojiFormant(1, nowmoji, 3), 1.0);

		std::vector<nsfloat> vout1(SEMIVOWELSPEED + CTIME * 2, 1.0);
		std::vector<nsfloat> vout2(SEMIVOWELSPEED + CTIME * 2, 1.0);

		mvffMMVF.setFormants(getMojiFormant(1, nowmoji, 0) + pitch, getMojiFormant(1, nowmoji, 1) + pitch, getMojiFormant(1, nowmoji, 2) + pitch, getMojiFormant(1, nowmoji, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);
		mvffMMVF.setFormants(getMojiFormant(0, nextmoji, 0) + nextpitch, getMojiFormant(0, nextmoji, 1) + nextpitch, getMojiFormant(0, nextmoji, 2) + nextpitch, getMojiFormant(0, nextmoji, 3) + nextpitch);
		mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

		for (nsint j = 0; j < SEMIVOWELSPEED;j++)
			out.push_back(vout1[j + CTIME] * ((nsfloat)(SEMIVOWELSPEED - j) * SEMIVOWELSPEED_INV));// +(voutw[j] + vout2[j + CTIME] * 0.5) * ((nsfloat)j / size);
	}
	//その他
	else {
		std::vector<nsfloat> vout1(SEMIVOWELSPEED + CTIME * 2, 1.0);
		std::vector<nsfloat> vout2(SEMIVOWELSPEED + CTIME * 2, 1.0);


		mvffMMVF.setFormants(getMojiFormant(1, nowmoji, 0) + pitch, getMojiFormant(1, nowmoji, 1) + pitch, getMojiFormant(1, nowmoji, 2) + pitch, getMojiFormant(1, nowmoji, 3) + pitch);
		mvffMMVF.makeSinWaveFromClass(vout1, out.size() - CTIME);
		mvffMMVF.setFormants(getMojiFormant(0, nextmoji, 0) + nextpitch, getMojiFormant(0, nextmoji, 1) + nextpitch, getMojiFormant(0, nextmoji, 2) + nextpitch, getMojiFormant(0, nextmoji, 3) + nextpitch);
		mvffMMVF.makeSinWaveFromClass(vout2, out.size() - CTIME);

		for (nsint j = 0; j < SEMIVOWELSPEED;j++)
			out.push_back(vout1[j + CTIME] * ((nsfloat)(SEMIVOWELSPEED - j) * SEMIVOWELSPEED_INV) + vout2[j + CTIME] * ((nsfloat)j * SEMIVOWELSPEED_INV));
	}
}


//=====ファイル関係=====

//文字列からデータを読み込み
void makeVoiseFromFile::loadFromChar(char* data) {

	/* いつか作ります。(SCP-___-J) */

	return;
}

//=====ローカル変数を取得=====

//データ保管庫から基音データを読み込む
nsfloat makeVoiseFromFile::getPrivatebf() {
	return privatebf;
}

//データ保管庫からフォルマントデータを読み込む
nsfloat makeVoiseFromFile::getPrivatebFormant(nsint b, nsint f) {
	return privateFormant[b][f];
}

//=====デコード関数=====

//零号機のデータを読み込む
nsint makeVoiseFromFile::decodeVoise0(unsigned char* data) {

	nsint ne = 0;                                         //今の配列の要素番号

	privatebf = 0.0;
	for (nsint i = 0; i < 5;i++)
		for (nsint j = 0; j < 4;j++)
			privateFormant[i][j] = 0;

	if (data[ne++] != 'D') return 1;
	if (data[ne++] != 'A') return 1;
	if (data[ne++] != 'T') return 1;
	if (data[ne++] != 'A') return 1;

	//基音
	if (data[ne++] != 'b') return 1;
	if (data[ne++] != 'f') return 1;
	uint16_t pf = 0;
	pf += (uint16_t)(data[ne++] & 0xff) << 0;
	pf += (uint16_t)(data[ne++] & 0xff) << 8;
	privatebf = (nsfloat)pf;

	//あ
	if (data[ne++] != 'V') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'A') return 1;
	for (nsint i = 0; i < 4;i++) {
		privateFormant[0][i] += (int64_t)(data[ne++] & 0xff) << 0;
		privateFormant[0][i] += (int64_t)(data[ne++] & 0xff) << 8;
	}

	//い
	if (data[ne++] != 'V') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'I') return 1;
	for (nsint i = 0; i < 4;i++) {
		privateFormant[1][i] += (int64_t)(data[ne++] & 0xff) << 0;
		privateFormant[1][i] += (int64_t)(data[ne++] & 0xff) << 8;
	}

	//う
	if (data[ne++] != 'V') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'U') return 1;
	for (nsint i = 0; i < 4;i++) {
		privateFormant[2][i] += (int64_t)(data[ne++] & 0xff) << 0;
		privateFormant[2][i] += (int64_t)(data[ne++] & 0xff) << 8;
	}

	//え
	if (data[ne++] != 'V') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'E') return 1;
	for (nsint i = 0; i < 4;i++) {
		privateFormant[3][i] += (int64_t)(data[ne++] & 0xff) << 0;
		privateFormant[3][i] += (int64_t)(data[ne++] & 0xff) << 8;
	}

	//お
	if (data[ne++] != 'V') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'O') return 1;
	for (nsint i = 0; i < 4;i++) {
		privateFormant[4][i] += (int64_t)(data[ne++] & 0xff) << 0;
		privateFormant[4][i] += (int64_t)(data[ne++] & 0xff) << 8;
	}

	createAllMojiFromantFromClass();
	softness = 0;

	return 0;
}

//零号機弐型のデータを読み込む
nsint makeVoiseFromFile::decodeVoise2(unsigned char* data) {

	nsint ne = 0;                                         //今の配列の要素番号

	privatebf = 0.0;
	for (nsint i = 0; i < 5;i++)
		for (nsint j = 0; j < 4;j++)
			privateFormant[i][j] = 0;

	if (data[ne++] != 'D') return 1;
	if (data[ne++] != 'A') return 1;
	if (data[ne++] != 'T') return 1;
	if (data[ne++] != 'A') return 1;

	//基音
	if (data[ne++] != 'b') return 1;
	if (data[ne++] != 'f') return 1;
	uint16_t pf = 0;
	pf += (uint16_t)(data[ne++] & 0xff) << 0;
	pf += (uint16_t)(data[ne++] & 0xff) << 8;
	privatebf = (nsfloat)pf;

	//データ
	//NSallmojiFormant
	if (data[ne++] != 'N') return 1;
	if (data[ne++] != 'S') return 1;
	if (data[ne++] != 'a') return 1;
	if (data[ne++] != 'l') return 1;
	if (data[ne++] != 'l') return 1;
	if (data[ne++] != 'm') return 1;
	if (data[ne++] != 'o') return 1;
	if (data[ne++] != 'j') return 1;
	if (data[ne++] != 'i') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'o') return 1;
	if (data[ne++] != 'r') return 1;
	if (data[ne++] != 'm') return 1;
	if (data[ne++] != 'a') return 1;
	if (data[ne++] != 'n') return 1;
	if (data[ne++] != 't') return 1;

	for (nsint i = 0;i < 2;i++) {
		for (nsint j = 0;j < 16;j++) {
			for (nsint k = 0;k < 16;k++) {
				for (nsint l = 0;l < 4;l++) {
					allmojiFormant[i][j][k][l] = (nsfloat)(uint64_t(data[ne++] & 0xff) << 0);
					allmojiFormant[i][j][k][l] += (nsfloat)(uint64_t(data[ne++] & 0xff) << 8);
				}
			}
		}
	}

	//柔らかさ
	if (data[ne++] != 'S') return 1;
	if (data[ne++] != 'O') return 1;
	if (data[ne++] != 'F') return 1;
	if (data[ne++] != 'T') return 1;
	int16_t sn = 0;
	sn += (int16_t)(data[ne++] & 0xff) << 0;
	sn += (int16_t)(data[ne++] & 0xff) << 8;
	softness = (nsfloat)sn;

	return 0;
}

//=====その他=====

//privateFormantからallmojiFormantを作成
void makeVoiseFromFile::createAllMojiFromantFromClass() {

	//NULL
	for (nsint i = 0; i < 4; i++) {
		allmojiFormant[0][0][0][i] = 0;
		allmojiFormant[1][0][0][i] = 0;
	}

	//母音フォルマント
	for (nsint i = 0; i < 2;i++) {
		//あ
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[i][0][1][k] = privateFormant[0][k];
		//い
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[i][0][2][k] = privateFormant[1][k];
		//う
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[i][0][7][k] = privateFormant[2][k];
		//え
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[i][0][12][k] = privateFormant[3][k];
		//お
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[i][0][13][k] = privateFormant[4][k];
	}

	//半母音
	if (true) {
		//や
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[0][0][3][k] = privateFormant[1][k];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][0][3][k] = privateFormant[0][k];
		//ゆ
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[0][0][4][k] = privateFormant[1][k];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][0][4][k] = privateFormant[2][k];
		//江
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[0][0][5][k] = privateFormant[1][k];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][0][5][k] = privateFormant[3][k];
		//よ
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[0][0][6][k] = privateFormant[1][k];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][0][6][k] = privateFormant[4][k];

		//わ
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[0][0][8][k] = privateFormant[2][k];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][0][8][k] = privateFormant[0][k];
		//ゐ
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[0][0][9][k] = privateFormant[2][k];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][0][9][k] = privateFormant[1][k];
		//ゑ
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[0][0][10][k] = privateFormant[2][k];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][0][10][k] = privateFormant[3][k];
		//を
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[0][0][11][k] = privateFormant[2][k];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][0][11][k] = privateFormant[4][k];
	}

	//軟口蓋破裂音(か～ご)(1x,2x)

	if (true) {
		//無声軟口蓋破裂音(か)
		allmojiFormant[0][1][1][0] = privateFormant[0][0];
		allmojiFormant[0][1][1][1] = privateFormant[0][1];
		allmojiFormant[0][1][1][2] = privateFormant[0][2] * (nsfloat)0.5;
		allmojiFormant[0][1][1][3] = privateFormant[0][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][1][1][k] = allmojiFormant[1][0][1][k];
		//無声軟口蓋破裂音(硬口より)(き)
		for (nsint i = 2;i <= 6; i++) {
			allmojiFormant[0][1][i][0] = privateFormant[1][0];
			allmojiFormant[0][1][i][1] = privateFormant[1][1];
			allmojiFormant[0][1][i][2] = privateFormant[1][2] * (nsfloat)0.5;
			allmojiFormant[0][1][i][3] = privateFormant[1][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][1][i][k] = allmojiFormant[1][0][i][k];
		}
		//無声軟口蓋破裂音(く)
		for (nsint i = 7;i <= 11; i++) {
			allmojiFormant[0][1][i][0] = privateFormant[2][0];
			allmojiFormant[0][1][i][1] = privateFormant[2][1];
			allmojiFormant[0][1][i][2] = privateFormant[2][2] * (nsfloat)0.5;
			allmojiFormant[0][1][i][3] = privateFormant[2][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][1][i][k] = allmojiFormant[1][0][i][k];
		}
		//無声軟口蓋破裂音(け)
		allmojiFormant[0][1][12][0] = privateFormant[3][0];
		allmojiFormant[0][1][12][1] = privateFormant[3][1];
		allmojiFormant[0][1][12][2] = privateFormant[3][2] * (nsfloat)0.5;
		allmojiFormant[0][1][12][3] = privateFormant[3][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][1][12][k] = allmojiFormant[1][0][12][k];
		//無声軟口蓋破裂音(こ)
		allmojiFormant[0][1][13][0] = privateFormant[4][0];
		allmojiFormant[0][1][13][1] = privateFormant[4][1];
		allmojiFormant[0][1][13][2] = privateFormant[4][2] * (nsfloat)0.5;
		allmojiFormant[0][1][13][3] = privateFormant[4][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][1][13][k] = allmojiFormant[1][0][13][k];


		//有声軟口蓋破裂音(が)
		allmojiFormant[0][2][1][0] = privateFormant[0][0];
		allmojiFormant[0][2][1][1] = privateFormant[0][1];
		allmojiFormant[0][2][1][2] = privateFormant[0][2] * (nsfloat)0.5;
		allmojiFormant[0][2][1][3] = privateFormant[0][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][2][1][k] = allmojiFormant[1][0][1][k];
		//有声軟口蓋破裂音(硬口より)(ぎ)
		for (nsint i = 2;i <= 6; i++) {
			allmojiFormant[0][2][i][0] = privateFormant[1][0];
			allmojiFormant[0][2][i][1] = privateFormant[1][1];
			allmojiFormant[0][2][i][2] = privateFormant[1][2] * (nsfloat)0.5;
			allmojiFormant[0][2][i][3] = privateFormant[1][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][2][i][k] = allmojiFormant[1][0][i][k];
		}
		//有声軟口蓋破裂音(ぐ)
		for (nsint i = 7;i <= 11; i++) {
			allmojiFormant[0][2][i][0] = privateFormant[2][0];
			allmojiFormant[0][2][i][1] = privateFormant[2][1];
			allmojiFormant[0][2][i][2] = privateFormant[2][2] * (nsfloat)0.5;
			allmojiFormant[0][2][i][3] = privateFormant[2][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][2][i][k] = allmojiFormant[1][0][i][k];
		}
		//有声軟口蓋破裂音(げ)
		allmojiFormant[0][2][12][0] = privateFormant[3][0];
		allmojiFormant[0][2][12][1] = privateFormant[3][1];
		allmojiFormant[0][2][12][2] = privateFormant[3][2] * (nsfloat)0.5;
		allmojiFormant[0][2][12][3] = privateFormant[3][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][2][12][k] = allmojiFormant[1][0][12][k];
		//有声軟口蓋破裂音(ご)
		allmojiFormant[0][2][13][0] = privateFormant[4][0];
		allmojiFormant[0][2][13][1] = privateFormant[4][1];
		allmojiFormant[0][2][13][2] = privateFormant[4][2] * (nsfloat)0.5;
		allmojiFormant[0][2][13][3] = privateFormant[4][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][2][13][k] = allmojiFormant[1][0][13][k];
	}

	//歯茎摩擦音(さ～ぞ)(3x,4x)
	if (true) {
		//無声歯茎摩擦音(さ)
		allmojiFormant[0][3][1][0] = privateFormant[0][0] / (nsfloat)1.5;
		allmojiFormant[0][3][1][1] = privateFormant[0][1] / (nsfloat)2.3;
		allmojiFormant[0][3][1][2] = privateFormant[0][2];
		allmojiFormant[0][3][1][3] = privateFormant[0][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][3][1][k] = allmojiFormant[1][0][1][k];
		//無声歯茎硬口蓋摩擦音(し)
		for (nsint i = 2;i <= 6; i++) {
			allmojiFormant[0][3][i][0] = privateFormant[1][0] / (nsfloat)1.5;
			allmojiFormant[0][3][i][1] = privateFormant[1][1] / (nsfloat)2.3;
			allmojiFormant[0][3][i][2] = privateFormant[1][2];
			allmojiFormant[0][3][i][3] = privateFormant[1][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][3][i][k] = allmojiFormant[1][0][i][k];
		}
		//無声歯茎摩擦音(す)
		for (nsint i = 7;i <= 11; i++) {
			allmojiFormant[0][3][i][0] = privateFormant[2][0] / (nsfloat)1.5;
			allmojiFormant[0][3][i][1] = privateFormant[2][1] / (nsfloat)2.3;
			allmojiFormant[0][3][i][2] = privateFormant[2][2];
			allmojiFormant[0][3][i][3] = privateFormant[2][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][3][i][k] = allmojiFormant[1][0][i][k];
		}
		//無声歯茎摩擦音(せ)
		allmojiFormant[0][3][12][0] = privateFormant[3][0] / (nsfloat)1.5;
		allmojiFormant[0][3][12][1] = privateFormant[3][1] / (nsfloat)2.3;
		allmojiFormant[0][3][12][2] = privateFormant[3][2];
		allmojiFormant[0][3][12][3] = privateFormant[3][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][3][12][k] = allmojiFormant[1][0][12][k];
		//無声歯茎摩擦音(そ)
		allmojiFormant[0][3][13][0] = privateFormant[4][0] / (nsfloat)1.5;
		allmojiFormant[0][3][13][1] = privateFormant[4][1] / (nsfloat)2.3;
		allmojiFormant[0][3][13][2] = privateFormant[4][2];
		allmojiFormant[0][3][13][3] = privateFormant[4][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][3][13][k] = allmojiFormant[1][0][13][k];


		//有声歯茎摩擦音(ざ)
		allmojiFormant[0][4][1][0] = privateFormant[0][0] / (nsfloat)1.5;
		allmojiFormant[0][4][1][1] = privateFormant[0][1] / (nsfloat)2.3;
		allmojiFormant[0][4][1][2] = privateFormant[0][2];
		allmojiFormant[0][4][1][3] = privateFormant[0][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][4][1][k] = allmojiFormant[1][0][1][k];
		//有声歯茎硬口蓋摩擦音(じ)
		for (nsint i = 2;i <= 6; i++) {
			allmojiFormant[0][4][i][0] = privateFormant[1][0] / (nsfloat)1.5;
			allmojiFormant[0][4][i][1] = privateFormant[1][1] / (nsfloat)2.3;
			allmojiFormant[0][4][i][2] = privateFormant[1][2];
			allmojiFormant[0][4][i][3] = privateFormant[1][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][4][i][k] = allmojiFormant[1][0][i][k];
		}
		//有声歯茎摩擦音(ず)
		for (nsint i = 7;i <= 11; i++) {
			allmojiFormant[0][4][i][0] = privateFormant[2][0] / (nsfloat)1.5;
			allmojiFormant[0][4][i][1] = privateFormant[2][1] / (nsfloat)2.3;
			allmojiFormant[0][4][i][2] = privateFormant[2][2];
			allmojiFormant[0][4][i][3] = privateFormant[2][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][4][i][k] = allmojiFormant[1][0][i][k];
		}
		//有声歯茎摩擦音(ぜ)
		allmojiFormant[0][4][12][0] = privateFormant[3][0] / (nsfloat)1.5;
		allmojiFormant[0][4][12][1] = privateFormant[3][1] / (nsfloat)2.3;
		allmojiFormant[0][4][12][2] = privateFormant[3][2];
		allmojiFormant[0][4][12][3] = privateFormant[3][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][4][12][k] = allmojiFormant[1][0][12][k];
		//有声歯茎摩擦音(ぞ)
		allmojiFormant[0][4][13][0] = privateFormant[4][0] / (nsfloat)1.5;
		allmojiFormant[0][4][13][1] = privateFormant[4][1] / (nsfloat)2.3;
		allmojiFormant[0][4][13][2] = privateFormant[4][2];
		allmojiFormant[0][4][13][3] = privateFormant[4][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][4][13][k] = allmojiFormant[1][0][13][k];
	}

	//歯茎破裂音(た～ど)(5x,6x)
	if (true) {
		//無声歯茎破裂音(た)
		allmojiFormant[0][5][1][0] = privateFormant[0][0] / (nsfloat)1.5;
		allmojiFormant[0][5][1][1] = privateFormant[0][1] / (nsfloat)2.3;
		allmojiFormant[0][5][1][2] = privateFormant[0][2];
		allmojiFormant[0][5][1][3] = privateFormant[0][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][5][1][k] = allmojiFormant[1][0][1][k];
		//無声歯茎硬口蓋破擦音(ち)
		for (nsint i = 2;i <= 6; i++) {
			allmojiFormant[0][5][i][0] = privateFormant[1][0] / (nsfloat)1.5;
			allmojiFormant[0][5][i][1] = privateFormant[1][1] / (nsfloat)2.3;
			allmojiFormant[0][5][i][2] = privateFormant[1][2];
			allmojiFormant[0][5][i][3] = privateFormant[1][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][5][i][k] = allmojiFormant[1][0][i][k];
		}
		//無声歯茎破擦音(つ)
		for (nsint i = 7;i <= 11; i++) {
			allmojiFormant[0][5][i][0] = privateFormant[2][0] / (nsfloat)1.5;
			allmojiFormant[0][5][i][1] = privateFormant[2][1] / (nsfloat)2.3;
			allmojiFormant[0][5][i][2] = privateFormant[2][2];
			allmojiFormant[0][5][i][3] = privateFormant[2][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][5][i][k] = allmojiFormant[1][0][i][k];
		}
		//無声歯茎破裂音(て)
		allmojiFormant[0][5][12][0] = privateFormant[3][0] / (nsfloat)1.5;
		allmojiFormant[0][5][12][1] = privateFormant[3][1] / (nsfloat)2.3;
		allmojiFormant[0][5][12][2] = privateFormant[3][2];
		allmojiFormant[0][5][12][3] = privateFormant[3][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][5][12][k] = allmojiFormant[1][0][12][k];
		//無声歯茎破裂音(と)
		allmojiFormant[0][5][13][0] = privateFormant[4][0] / (nsfloat)1.5;
		allmojiFormant[0][5][13][1] = privateFormant[4][1] / (nsfloat)2.3;
		allmojiFormant[0][5][13][2] = privateFormant[4][2];
		allmojiFormant[0][5][13][3] = privateFormant[4][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][5][13][k] = allmojiFormant[1][0][13][k];


		//有声歯茎破裂音(だ)
		allmojiFormant[0][6][1][0] = privateFormant[0][0] / (nsfloat)1.5;
		allmojiFormant[0][6][1][1] = privateFormant[0][1] / (nsfloat)2.3;
		allmojiFormant[0][6][1][2] = privateFormant[0][2];
		allmojiFormant[0][6][1][3] = privateFormant[0][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][6][1][k] = allmojiFormant[1][0][1][k];
		//有声歯茎硬口蓋破擦音(ぢ)
		for (nsint i = 2;i <= 6; i++) {
			allmojiFormant[0][6][i][0] = privateFormant[1][0] / (nsfloat)1.5;
			allmojiFormant[0][6][i][1] = privateFormant[1][1] / (nsfloat)2.3;
			allmojiFormant[0][6][i][2] = privateFormant[1][2];
			allmojiFormant[0][6][i][3] = privateFormant[1][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][6][i][k] = allmojiFormant[1][0][i][k];
		}
		//有声歯茎破擦音(づ)
		for (nsint i = 7;i <= 11; i++) {
			allmojiFormant[0][6][i][0] = privateFormant[2][0] / (nsfloat)1.5;
			allmojiFormant[0][6][i][1] = privateFormant[2][1] / (nsfloat)2.3;
			allmojiFormant[0][6][i][2] = privateFormant[2][2];
			allmojiFormant[0][6][i][3] = privateFormant[2][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][6][i][k] = allmojiFormant[1][0][i][k];
		}
		//有声歯茎破裂音(で)
		allmojiFormant[0][6][12][0] = privateFormant[3][0] / (nsfloat)1.5;
		allmojiFormant[0][6][12][1] = privateFormant[3][1] / (nsfloat)2.3;
		allmojiFormant[0][6][12][2] = privateFormant[3][2];
		allmojiFormant[0][6][12][3] = privateFormant[3][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][6][12][k] = allmojiFormant[1][0][12][k];
		//有声歯茎破裂音(ど)
		allmojiFormant[0][6][13][0] = privateFormant[4][0] / (nsfloat)1.5;
		allmojiFormant[0][6][13][1] = privateFormant[4][1] / (nsfloat)2.3;
		allmojiFormant[0][6][13][2] = privateFormant[4][2];
		allmojiFormant[0][6][13][3] = privateFormant[4][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][6][13][k] = allmojiFormant[1][0][13][k];
	}

	//両唇破裂音(ぱ～ぼ)(7x,8x)
	if (true) {
		//無声両唇破裂音(ぱ)
		allmojiFormant[0][7][1][0] = privateFormant[0][0];
		allmojiFormant[0][7][1][1] = privateFormant[0][1] * (nsfloat)0.5;
		allmojiFormant[0][7][1][2] = privateFormant[0][2];
		allmojiFormant[0][7][1][3] = privateFormant[0][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][7][1][k] = allmojiFormant[1][0][1][k];
		//有声両唇硬口蓋接近音(ぴ)
		for (nsint i = 2;i <= 6; i++) {
			allmojiFormant[0][7][i][0] = privateFormant[1][0];
			allmojiFormant[0][7][i][1] = privateFormant[1][1] * (nsfloat)0.5;
			allmojiFormant[0][7][i][2] = privateFormant[1][2];
			allmojiFormant[0][7][i][3] = privateFormant[1][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][7][i][k] = allmojiFormant[1][0][i][k];
		}
		//無声両唇破裂音(ぷ)
		for (nsint i = 7;i <= 11; i++) {
			allmojiFormant[0][7][i][0] = privateFormant[2][0];
			allmojiFormant[0][7][i][1] = privateFormant[2][1] * (nsfloat)0.5;
			allmojiFormant[0][7][i][2] = privateFormant[2][2];
			allmojiFormant[0][7][i][3] = privateFormant[2][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][7][i][k] = allmojiFormant[1][0][i][k];
		}
		//無声両唇破裂音(ぺ)
		allmojiFormant[0][7][12][0] = privateFormant[3][0];
		allmojiFormant[0][7][12][1] = privateFormant[3][1] * (nsfloat)0.5;
		allmojiFormant[0][7][12][2] = privateFormant[3][2];
		allmojiFormant[0][7][12][3] = privateFormant[3][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][7][12][k] = allmojiFormant[1][0][12][k];
		//無声両唇破裂音(ぽ)
		allmojiFormant[0][7][13][0] = privateFormant[4][0];
		allmojiFormant[0][7][13][1] = privateFormant[4][1] * (nsfloat)0.5;
		allmojiFormant[0][7][13][2] = privateFormant[4][2];
		allmojiFormant[0][7][13][3] = privateFormant[4][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][7][13][k] = allmojiFormant[1][0][13][k];


		//有声両唇破裂音(ば)
		allmojiFormant[0][8][1][0] = privateFormant[0][0];
		allmojiFormant[0][8][1][1] = privateFormant[0][1] * (nsfloat)0.5;
		allmojiFormant[0][8][1][2] = privateFormant[0][2];
		allmojiFormant[0][8][1][3] = privateFormant[0][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][8][1][k] = allmojiFormant[1][0][1][k];
		//有声両唇硬口蓋破裂音(び)
		for (nsint i = 2;i <= 6; i++) {
			allmojiFormant[0][8][i][0] = privateFormant[1][0];
			allmojiFormant[0][8][i][1] = privateFormant[1][1] * (nsfloat)0.5;
			allmojiFormant[0][8][i][2] = privateFormant[1][2];
			allmojiFormant[0][8][i][3] = privateFormant[1][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][8][i][k] = allmojiFormant[1][0][i][k];
		}
		//有声両唇破裂音(ぶ)
		for (nsint i = 7;i <= 11; i++) {
			allmojiFormant[0][8][i][0] = privateFormant[2][0];
			allmojiFormant[0][8][i][1] = privateFormant[2][1] * (nsfloat)0.5;
			allmojiFormant[0][8][i][2] = privateFormant[2][2];
			allmojiFormant[0][8][i][3] = privateFormant[2][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][8][i][k] = allmojiFormant[1][0][i][k];
		}
		//有声両唇破裂音(べ)
		allmojiFormant[0][8][12][0] = privateFormant[3][0];
		allmojiFormant[0][8][12][1] = privateFormant[3][1] * (nsfloat)0.5;
		allmojiFormant[0][8][12][2] = privateFormant[3][2];
		allmojiFormant[0][8][12][3] = privateFormant[3][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][8][12][k] = allmojiFormant[1][0][12][k];
		//有声両唇破裂音(ぼ)
		allmojiFormant[0][8][13][0] = privateFormant[4][0];
		allmojiFormant[0][8][13][1] = privateFormant[4][1] * (nsfloat)0.5;
		allmojiFormant[0][8][13][2] = privateFormant[4][2];
		allmojiFormant[0][8][13][3] = privateFormant[4][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][8][13][k] = allmojiFormant[1][0][13][k];
	}

	//摩殺音(笑)(は～ほ)(9x)
	if (true) {
		//無声声門摩擦音(は)
		//無声硬口蓋摩擦音(ひ)
		//無声両唇摩擦音(ふ)
		//無声声門摩擦音(へ)
		//無声声門摩擦音(ほ)
		for (nsint i = 0;i < 15;i++)
			for (nsint j = 0; j < 2;j++)
				for (nsint k = 0; k < 4;k++)
					allmojiFormant[j][9][i][k] = allmojiFormant[j][0][i][k];
	}

	//歯茎鼻音(な～の)(ax)
	if (true) {
		//歯茎鼻音(な)
		allmojiFormant[0][10][1][0] = privateFormant[0][0] * (nsfloat)0.5;
		allmojiFormant[0][10][1][1] = privateFormant[0][1] / (nsfloat)3.0;
		allmojiFormant[0][10][1][2] = privateFormant[0][2];
		allmojiFormant[0][10][1][3] = privateFormant[0][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][10][1][k] = allmojiFormant[1][0][1][k];
		//硬口蓋鼻音(に)
		for (nsint i = 2;i <= 6; i++) {
			allmojiFormant[0][10][i][0] = privateFormant[1][0] * (nsfloat)0.5;
			allmojiFormant[0][10][i][1] = privateFormant[1][1] / (nsfloat)3.0;
			allmojiFormant[0][10][i][2] = privateFormant[1][2];
			allmojiFormant[0][10][i][3] = privateFormant[1][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][10][i][k] = allmojiFormant[1][0][i][k];
		}
		//歯茎鼻音(ぬ)
		for (nsint i = 7;i <= 11; i++) {
			allmojiFormant[0][10][i][0] = privateFormant[2][0] * (nsfloat)0.5;
			allmojiFormant[0][10][i][1] = privateFormant[2][1] / (nsfloat)3.0;
			allmojiFormant[0][10][i][2] = privateFormant[2][2];
			allmojiFormant[0][10][i][3] = privateFormant[2][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][10][i][k] = allmojiFormant[1][0][i][k];
		}
		//歯茎鼻音(ね)
		allmojiFormant[0][10][12][0] = privateFormant[3][0] * (nsfloat)0.5;
		allmojiFormant[0][10][12][1] = privateFormant[3][1] / (nsfloat)3.0;
		allmojiFormant[0][10][12][2] = privateFormant[3][2];
		allmojiFormant[0][10][12][3] = privateFormant[3][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][10][12][k] = allmojiFormant[1][0][12][k];
		//歯茎鼻音(の)
		allmojiFormant[0][10][13][0] = privateFormant[4][0] * (nsfloat)0.5;
		allmojiFormant[0][10][13][1] = privateFormant[4][1] / (nsfloat)3.0;
		allmojiFormant[0][10][13][2] = privateFormant[4][2];
		allmojiFormant[0][10][13][3] = privateFormant[4][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][10][13][k] = allmojiFormant[1][0][13][k];
	}

	//両唇鼻音(ま～も)(bx)
	if (true) {
		//両唇鼻音(ま)
		allmojiFormant[0][11][1][0] = privateFormant[0][0] / (nsfloat)3.0;
		allmojiFormant[0][11][1][1] = privateFormant[0][1] * (nsfloat)0.5;
		allmojiFormant[0][11][1][2] = privateFormant[0][2];
		allmojiFormant[0][11][1][3] = privateFormant[0][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][11][1][k] = allmojiFormant[1][0][1][k];
		//両唇鼻音(み)
		for (nsint i = 2;i <= 6; i++) {
			allmojiFormant[0][11][i][0] = privateFormant[1][0] / (nsfloat)3.0;
			allmojiFormant[0][11][i][1] = privateFormant[1][1] * (nsfloat)0.5;
			allmojiFormant[0][11][i][2] = privateFormant[1][2];
			allmojiFormant[0][11][i][3] = privateFormant[1][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][11][i][k] = allmojiFormant[1][0][i][k];
		}
		//両唇鼻音(む)
		for (nsint i = 7;i <= 11; i++) {
			allmojiFormant[0][11][i][0] = privateFormant[2][0] / (nsfloat)3.0;
			allmojiFormant[0][11][i][1] = privateFormant[2][1] * (nsfloat)0.5;
			allmojiFormant[0][11][i][2] = privateFormant[2][2];
			allmojiFormant[0][11][i][3] = privateFormant[2][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][11][i][k] = allmojiFormant[1][0][i][k];
		}
		//両唇鼻音(め)
		allmojiFormant[0][11][12][0] = privateFormant[3][0] / (nsfloat)3.0;
		allmojiFormant[0][11][12][1] = privateFormant[3][1] * (nsfloat)0.5;
		allmojiFormant[0][11][12][2] = privateFormant[3][2];
		allmojiFormant[0][11][12][3] = privateFormant[3][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][11][12][k] = allmojiFormant[1][0][12][k];
		//両唇鼻音(も)
		allmojiFormant[0][11][13][0] = privateFormant[4][0] / (nsfloat)3.0;
		allmojiFormant[0][11][13][1] = privateFormant[4][1] * (nsfloat)0.5;
		allmojiFormant[0][11][13][2] = privateFormant[4][2];
		allmojiFormant[0][11][13][3] = privateFormant[4][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][11][13][k] = allmojiFormant[1][0][13][k];
	}

	//有声歯茎はじき音(ら～ろ)(cx)
	if (true) {
		//有声歯茎はじき音(ら)
		allmojiFormant[0][12][1][0] = privateFormant[0][0] / (nsfloat)1.5;
		allmojiFormant[0][12][1][1] = privateFormant[0][1] / (nsfloat)2.3;
		allmojiFormant[0][12][1][2] = privateFormant[0][2];
		allmojiFormant[0][12][1][3] = privateFormant[0][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][12][1][k] = allmojiFormant[1][0][1][k];
		//歯茎硬口蓋はじき音(り)
		for (nsint i = 2;i <= 6; i++) {
			allmojiFormant[0][12][i][0] = privateFormant[1][0] / (nsfloat)1.5;
			allmojiFormant[0][12][i][1] = privateFormant[1][1] / (nsfloat)2.3;
			allmojiFormant[0][12][i][2] = privateFormant[1][2];
			allmojiFormant[0][12][i][3] = privateFormant[1][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][12][i][k] = allmojiFormant[1][0][i][k];
		}
		//有声歯茎はじき音(る)
		for (nsint i = 7;i <= 11; i++) {
			allmojiFormant[0][12][i][0] = privateFormant[2][0] / (nsfloat)1.5;
			allmojiFormant[0][12][i][1] = privateFormant[2][1] / (nsfloat)2.3;
			allmojiFormant[0][12][i][2] = privateFormant[2][2];
			allmojiFormant[0][12][i][3] = privateFormant[2][3];
			for (nsint k = 0; k < 4;k++)
				allmojiFormant[1][12][i][k] = allmojiFormant[1][0][i][k];
		}
		//有声歯茎はじき音(れ)
		allmojiFormant[0][12][12][0] = privateFormant[3][0] / (nsfloat)1.5;
		allmojiFormant[0][12][12][1] = privateFormant[3][1] / (nsfloat)2.3;
		allmojiFormant[0][12][12][2] = privateFormant[3][2];
		allmojiFormant[0][12][12][3] = privateFormant[3][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][12][12][k] = allmojiFormant[1][0][12][k];
		//有声歯茎はじき音(ろ)
		allmojiFormant[0][12][13][0] = privateFormant[4][0] / (nsfloat)1.5;
		allmojiFormant[0][12][13][1] = privateFormant[4][1] / (nsfloat)2.3;
		allmojiFormant[0][12][13][2] = privateFormant[4][2];
		allmojiFormant[0][12][13][3] = privateFormant[4][3];
		for (nsint k = 0; k < 4;k++)
			allmojiFormant[1][12][13][k] = allmojiFormant[1][0][13][k];
	}

	//ん
	if (true) {
		//口蓋垂鼻音(ん)
		allmojiFormant[0][13][1][0] = allmojiFormant[1][13][1][0] = privateFormant[2][0] * (nsfloat)0.5;
		allmojiFormant[0][13][1][1] = allmojiFormant[1][13][1][1] = privateFormant[2][1] / (nsfloat)3.0;
		allmojiFormant[0][13][1][2] = allmojiFormant[1][13][1][2] = privateFormant[2][2];
		allmojiFormant[0][13][1][3] = allmojiFormant[1][13][1][3] = privateFormant[2][3];
	}

	return;
}

//音諳コードからフォルマント周波数を取得
nsfloat makeVoiseFromFile::getMojiFormant(nsint f, nsint moji, nsint formant) {

	return allmojiFormant[f][(moji & 0xf0) >> 4][(moji & 0x0f)][formant];
}

//無声破裂音かどうか
bool makeVoiseFromFile::doVoiselessPlosive(nschar moji) {
	return ((moji & 0xf0) == 0x10 or (moji & 0xf0) == 0x50 or (moji & 0xf0) == 0x70);
}

//有声破裂音かどうか
bool makeVoiseFromFile::doVoisedPlosive(nschar moji) {
	return ((moji & 0xf0) == 0x20 or (moji & 0xf0) == 0x60 or (moji & 0xf0) == 0x80);
}

//鼻音かどうか
bool makeVoiseFromFile::doNasal(nschar moji) {
	return ((moji & 0xf0) == 0xa0 or (moji & 0xf0) == 0xb0);
}

//はじき音かどうか
bool makeVoiseFromFile::doTap(nschar moji) {
	return ((moji & 0xf0) == 0xc0);
}

//無声摩擦音かどうか
bool makeVoiseFromFile::doVoiselessFricative(nschar moji) {
	return ((moji & 0xf0) == 0x30);
}

//有声摩擦音かどうか
bool makeVoiseFromFile::doVoisedFricative(nschar moji) {
	return ((moji & 0xf0) == 0x40);
}

//摩殺音かどうか
bool makeVoiseFromFile::doFriKILLive(nschar moji) {
	return ((moji & 0xf0) == 0x90);
}

//音諳形式の時間単位から声の長さ[秒]へ変換
nsint makeVoiseFromFile::getTimeFromSpeed(char speed, nschar moji) {
	char mojiboin = moji & 0x0f;
	return (nsint)((nsfloat)SMPL * ((nsfloat)10.0 / (nsfloat)speed) + ((mojiboin >= 0x03 and mojiboin <= 0x0b and mojiboin != 0x07 and moji >= 0x10 and (nsfloat)SMPL * ((nsfloat)10.0 / (nsfloat)speed) > MAXSHIONSPEED + MAXBYOUONSPEED + MAXYOUONSPEED) ? MAXBYOUONSPEED + MAXYOUONSPEED : 0));
}

//ホワイトノイズの乱数の同期を初期化
void makeVoiseFromFile::ftInit(size_t size) {

	ft.resize(size);

	nsfloat RandMax_inv = 1.0 / (nsfloat)RAND_MAX * PI_2;
	for (size_t i = 0; i < size;i++)
		ft[i] = (nsfloat)rand() * RandMax_inv;

	return;
}

//ホワイトノイズを作成
void makeVoiseFromFile::makeWhitenoise(std::vector<nsfloat> out, nsint time, nsfloat hz, nsfloat um) {

	nsfloat magn = FRICATIVENOISEACC / ((nsfloat)MAXFREQ - hz);

	std::vector<nsfloat> out1(out.size(), 0.0), out2(out.size(), 0.0);

	for (size_t j = 0; j < out.size();j++) {
		for (nsint i = (nsint)hz; i < MAXFREQ;i += (nsint)FRICATIVENOISEACC)
			out1[j] += nsMath::nsSinTable(((nsfloat)i + (nsfloat)time + ft[i]), (nsfloat)i);
	}
	nsfloat max = *std::max_element(out1.begin(), out1.end());
	magn = (nsfloat)1.0 / max;
	for (size_t i = 0; i < out.size();i++)
		out1[i] *= magn;


	hipassFilter(out1, out2, hz, 0.707);
	lowpassFilter(out2, out, um, 1.0);

	return;
}

//ホワイトノイズを作成
void makeVoiseFromFile::makeWhitenoise1(std::vector<nsfloat> out, nsint time, nsfloat hz) {

	nsfloat magn = FRICATIVENOISEACC / ((nsfloat)MAXFREQ - hz);

	std::vector<nsfloat> out1(out.size(), 0.0), out2(out.size(), 0.0);

	for (size_t j = 0; j < out.size();j++) {
		for (nsint i = (nsint)hz; i < MAXFREQ;i += (nsint)FRICATIVENOISEACC)
			out1[j] += (nsMath::pow2(nsMath::nsSinTable(((nsfloat)i + (nsfloat)time + ft[j]), (nsfloat)i)) / nsMath::pow2(i + hz));
	}
	nsfloat max = *std::max_element(out1.begin(), out1.end());
	magn = (nsfloat)1.0 / max;
	for (size_t i = 0; i < out.size();i++)
		out1[i] *= magn;

	hipassFilter(out1, out2, hz - FRICATIVENOISESCOPE, 0.707);
	lowpassFilter(out2, out, hz - FRICATIVENOISESCOPE, 1.0);

	return;
}



//=====フィルター関数=====

//ハイパスフィルタ
void hipassFilter(const std::vector<nsfloat>& input, std::vector<nsfloat>& out, nsfloat freq, nsfloat q) {

	// それぞれの変数は下記のとおりとする
	// float samplerate … サンプリング周波数
	// float freq … カットオフ周波数
	// float q    … フィルタのQ値
	nsfloat omega = PI_2 * freq * SMPL_INV;
	nsfloat alpha = sin(omega) / ((nsfloat)2.0 * q);

	nsfloat a0_inv = (nsfloat)1.0 / ((nsfloat)1.0 + alpha);
	nsfloat a1 = -(nsfloat)2.0 * cos(omega);
	nsfloat a2 = (nsfloat)1.0 - alpha;
	nsfloat b0 = ((nsfloat)1.0 + cos(omega)) * (nsfloat)0.5;
	nsfloat b1 = -((nsfloat)1.0 + cos(omega));
	nsfloat b2 = ((nsfloat)1.0 + cos(omega)) * (nsfloat)0.5;

	// それぞれの変数は下記のとおりとする
	// 　float input[]  …入力信号の格納されたバッファ。
	// 　flaot output[] …フィルタ処理した値を書き出す出力信号のバッファ。
	// 　nsint   size     …入力信号・出力信号のバッファのサイズ。
	// 　float in1, in2, out1, out2  …フィルタ計算用のバッファ変数。初期値は0。
	// 　float a0, a1, a2, b0, b1, b2 …フィルタの係数。 別途算出する。

	nsfloat in1 = 0.0, in2 = 0.0, out1 = 0.0, out2 = 0.0;
	for (size_t i = 0; i < out.size(); i++) {
		// 入力信号にフィルタを適用し、出力信号として書き出す。
		out[i] = (b0 * input[i] + b1 * in1 + b2 * in2 - a1 * out1 - a2 * out2) * a0_inv;

		in2 = in1;		// 2つ前の入力信号を更新
		in1 = input[i];	// 1つ前の入力信号を更新

		out2 = out1;	// 2つ前の出力信号を更新
		out1 = out[i];	// 1つ前の出力信号を更新
	}

	return;
}

//ローパスフィルタ
void lowpassFilter(const std::vector<nsfloat>& input, std::vector<nsfloat>& out, nsfloat freq, nsfloat q) {
	// それぞれの変数は下記のとおりとする
	// float samplerate … サンプリング周波数
	// float freq … カットオフ周波数
	// float q    … フィルタのQ値
	nsfloat omega = PI_2 * freq * SMPL_INV;
	nsfloat alpha = sin(omega) / ((nsfloat)2.0 * q);

	nsfloat a0_inv = (nsfloat)1.0 / ((nsfloat)1.0 + alpha);
	nsfloat a1 = -(nsfloat)2.0 * cos(omega);
	nsfloat a2 = (nsfloat)1.0 - alpha;
	nsfloat b0 = ((nsfloat)1.0 - cos(omega)) * (nsfloat)0.5;
	nsfloat b1 = (nsfloat)1.0 - cos(omega);
	nsfloat b2 = ((nsfloat)1.0 - cos(omega)) * (nsfloat)0.5;

	// それぞれの変数は下記のとおりとする
	// 　float input[]  …入力信号の格納されたバッファ。
	// 　flaot output[] …フィルタ処理した値を書き出す出力信号のバッファ。
	// 　nsint   size     …入力信号・出力信号のバッファのサイズ。
	// 　float in1, in2, out1, out2  …フィルタ計算用のバッファ変数。初期値は0。
	// 　float a0, a1, a2, b0, b1, b2 …フィルタの係数。 別途算出する。

	nsfloat in1 = 0.0, in2 = 0.0, out1 = 0.0, out2 = 0.0;
	for (size_t i = 0; i < out.size(); i++) {
		// 入力信号にフィルタを適用し、出力信号として書き出す。
		out[i] = (b0 * input[i] + b1 * in1 + b2 * in2 - a1 * out1 - a2 * out2) * a0_inv;

		in2 = in1;		// 2つ前の入力信号を更新
		in1 = input[i];	// 1つ前の入力信号を更新

		out2 = out1;	// 2つ前の出力信号を更新
		out1 = out[i];	// 1つ前の出力信号を更新
	}

	return;
}

//ノッチフィルター
void notchpassFilter(const std::vector<nsfloat>& input, std::vector<nsfloat>& out, nsfloat freq, nsfloat bw) {
	// それぞれの変数は下記のとおりとする
	// float samplerate … サンプリング周波数
	//float freq … カットオフ周波数
	// float bw   … 帯域幅
	nsfloat omega = PI_2 * freq * SMPL_INV;
	nsfloat alpha = sin(omega) * sinh(log((nsfloat)2.0) * (nsfloat)0.5 * bw * omega / sin(omega));

	nsfloat a0_inv = (nsfloat)1.0 / ((nsfloat)1.0 + alpha);
	nsfloat a1 = -(nsfloat)2.0 * cos(omega);
	nsfloat a2 = (nsfloat)1.0 - alpha;
	nsfloat b0 = (nsfloat)1.0;
	nsfloat b1 = -(nsfloat)2.0 * cos(omega);
	nsfloat b2 = (nsfloat)1.0;

	// それぞれの変数は下記のとおりとする
	// 　float input[]  …入力信号の格納されたバッファ。
	// 　flaot output[] …フィルタ処理した値を書き出す出力信号のバッファ。
	// 　nsint   size     …入力信号・出力信号のバッファのサイズ。
	// 　float in1, in2, out1, out2  …フィルタ計算用のバッファ変数。初期値は0。
	// 　float a0, a1, a2, b0, b1, b2 …フィルタの係数。 別途算出する。

	nsfloat in1 = 0.0, in2 = 0.0, out1 = 0.0, out2 = 0.0;
	for (size_t i = 0; i < out.size(); i++) {
		// 入力信号にフィルタを適用し、出力信号として書き出す。
		out[i] = (b0 * input[i] + b1 * in1 + b2 * in2 - a1 * out1 - a2 * out2) * a0_inv;

		in2 = in1;		// 2つ前の入力信号を更新
		in1 = input[i];	// 1つ前の入力信号を更新

		out2 = out1;	// 2つ前の出力信号を更新
		out1 = out[i];	// 1つ前の出力信号を更新
	}

	return;
}

}//namespace Nesora


