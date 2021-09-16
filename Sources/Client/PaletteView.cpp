/*
 Copyright (c) 2013 yvt

 This file is part of OpenSpades.

 OpenSpades is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 OpenSpades is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with OpenSpades.  If not, see <http://www.gnu.org/licenses/>.

 */

#include <Core/Settings.h>

#include "PaletteView.h"
#include "Client.h"
#include "IImage.h"
#include "IRenderer.h"
#include "NetClient.h"
#include "Player.h"
#include "World.h"

DEFINE_SPADES_SETTING(cg_keyPaletteLeft, "Left");
DEFINE_SPADES_SETTING(cg_keyPaletteRight, "Right");
DEFINE_SPADES_SETTING(cg_keyPaletteUp, "Up");
DEFINE_SPADES_SETTING(cg_keyPaletteDown, "Down");
DEFINE_SPADES_SETTING(cg_keyPaletteInvert, "i");
DEFINE_SPADES_SETTING(cg_keyPaletteRandom, "z");
DEFINE_SPADES_SETTING(cg_keyPaletteMix, "x");

namespace spades {
	namespace client {
		static IntVector3 SanitizeCol(IntVector3 col) {
			if (col.x < 0)
				col.x = 0;
			if (col.y < 0)
				col.y = 0;
			if (col.z < 0)
				col.z = 0;
			return col;
		}

		PaletteView::PaletteView(Client *client) : client(client), renderer(client->GetRenderer()) {
			colors.push_back({13, 13, 13});
			colors.push_back({46, 46, 46});
			colors.push_back({79, 79, 79});
			colors.push_back({112, 112, 112});
			colors.push_back({143, 143, 143});
			colors.push_back({175, 175, 175});
			colors.push_back({207, 207, 207});
			colors.push_back({239, 239, 239});
			colors.push_back({136, 14, 79});
			colors.push_back({173, 20, 87});
			colors.push_back({194, 24, 91});
			colors.push_back({216, 27, 96});
			colors.push_back({233, 30, 99});
			colors.push_back({236, 64, 122});
			colors.push_back({240, 98, 146});
			colors.push_back({244, 143, 177});
			colors.push_back({31, 0, 0});
			colors.push_back({95, 0, 0});
			colors.push_back({159, 0, 0});
			colors.push_back({223, 0, 0});
			colors.push_back({255, 31, 31});
			colors.push_back({255, 95, 95});
			colors.push_back({255, 159, 159});
			colors.push_back({255, 223, 223});
			colors.push_back({176, 18, 10});
			colors.push_back({196, 20, 17});
			colors.push_back({208, 23, 22});
			colors.push_back({221, 25, 29});
			colors.push_back({229, 28, 35});
			colors.push_back({232, 78, 64});
			colors.push_back({243, 108, 96});
			colors.push_back({246, 153, 136});
			colors.push_back({31, 15, 0});
			colors.push_back({95, 47, 0});
			colors.push_back({159, 79, 0});
			colors.push_back({223, 111, 0});
			colors.push_back({255, 143, 31});
			colors.push_back({255, 175, 95});
			colors.push_back({255, 207, 159});
			colors.push_back({255, 239, 223});
			colors.push_back({191, 54, 12});
			colors.push_back({216, 67, 21});
			colors.push_back({230, 74, 25});
			colors.push_back({244, 81, 30});
			colors.push_back({255, 87, 34});
			colors.push_back({255, 112, 67});
			colors.push_back({255, 138, 101});
			colors.push_back({255, 171, 145});
			colors.push_back({31, 31, 0});
			colors.push_back({95, 95, 0});
			colors.push_back({159, 159, 0});
			colors.push_back({223, 223, 0});
			colors.push_back({255, 255, 31});
			colors.push_back({255, 255, 95});
			colors.push_back({255, 255, 159});
			colors.push_back({255, 255, 223});
			colors.push_back({230, 81, 0});
			colors.push_back({239, 108, 0});
			colors.push_back({245, 124, 0});
			colors.push_back({251, 140, 0});
			colors.push_back({255, 152, 0});
			colors.push_back({255, 167, 38});
			colors.push_back({255, 183, 77});
			colors.push_back({255, 204, 128});
			colors.push_back({0, 31, 0});
			colors.push_back({0, 95, 0});
			colors.push_back({0, 159, 0});
			colors.push_back({0, 223, 0});
			colors.push_back({31, 255, 31});
			colors.push_back({95, 255, 95});
			colors.push_back({159, 255, 159});
			colors.push_back({223, 255, 223});
			colors.push_back({255, 111, 0});
			colors.push_back({255, 143, 0});
			colors.push_back({255, 160, 0});
			colors.push_back({255, 179, 0});
			colors.push_back({255, 193, 7});
			colors.push_back({255, 202, 40});
			colors.push_back({255, 213, 79});
			colors.push_back({255, 224, 130});
			colors.push_back({0, 31, 31});
			colors.push_back({0, 95, 95});
			colors.push_back({0, 159, 159});
			colors.push_back({0, 223, 223});
			colors.push_back({31, 255, 255});
			colors.push_back({95, 255, 255});
			colors.push_back({159, 255, 255});
			colors.push_back({223, 255, 255});
			colors.push_back({245, 127, 23});
			colors.push_back({249, 168, 37});
			colors.push_back({251, 192, 45});
			colors.push_back({253, 216, 53});
			colors.push_back({255, 235, 59});
			colors.push_back({255, 238, 88});
			colors.push_back({255, 241, 118});
			colors.push_back({255, 245, 157});
			colors.push_back({0, 0, 31});
			colors.push_back({0, 0, 95});
			colors.push_back({0, 0, 159});
			colors.push_back({0, 0, 223});
			colors.push_back({31, 31, 255});
			colors.push_back({95, 95, 255});
			colors.push_back({159, 159, 255});
			colors.push_back({223, 223, 255});
			colors.push_back({130, 119, 23});
			colors.push_back({158, 157, 36});
			colors.push_back({175, 180, 43});
			colors.push_back({192, 202, 51});
			colors.push_back({205, 220, 57});
			colors.push_back({212, 225, 87});
			colors.push_back({220, 231, 117});
			colors.push_back({230, 238, 156});
			colors.push_back({31, 0, 31});
			colors.push_back({95, 0, 95});
			colors.push_back({159, 0, 159});
			colors.push_back({223, 0, 223});
			colors.push_back({255, 31, 255});
			colors.push_back({255, 95, 255});
			colors.push_back({255, 159, 255});
			colors.push_back({255, 223, 255});
			colors.push_back({51, 105, 30});
			colors.push_back({85, 139, 47});
			colors.push_back({104, 159, 56});
			colors.push_back({124, 179, 66});
			colors.push_back({139, 195, 74});
			colors.push_back({156, 204, 101});
			colors.push_back({174, 213, 129});
			colors.push_back({197, 225, 165});
			colors.push_back({136, 28, 127});
			colors.push_back({183, 22, 126});
			colors.push_back({219, 13, 125});
			colors.push_back({248, 0, 124});
			colors.push_back({249, 133, 143});
			colors.push_back({251, 182, 160});
			colors.push_back({253, 218, 174});
			colors.push_back({255, 248, 188});
			colors.push_back({13, 83, 2});
			colors.push_back({5, 111, 0});
			colors.push_back({10, 126, 7});
			colors.push_back({10, 143, 8});
			colors.push_back({37, 155, 36});
			colors.push_back({43, 175, 43});
			colors.push_back({66, 189, 65});
			colors.push_back({114, 213, 114});
			colors.push_back({34, 34, 128});
			colors.push_back({28, 90, 147});
			colors.push_back({22, 120, 163});
			colors.push_back({13, 143, 177});
			colors.push_back({0, 162, 190});
			colors.push_back({137, 188, 189});
			colors.push_back({188, 211, 189});
			colors.push_back({225, 230, 188});
			colors.push_back({0, 77, 64});
			colors.push_back({0, 105, 92});
			colors.push_back({0, 121, 107});
			colors.push_back({0, 137, 123});
			colors.push_back({0, 150, 136});
			colors.push_back({38, 166, 154});
			colors.push_back({77, 182, 172});
			colors.push_back({128, 203, 196});
			colors.push_back({137, 0, 188});
			colors.push_back({188, 0, 188});
			colors.push_back({225, 0, 188});
			colors.push_back({255, 0, 188});
			colors.push_back({255, 137, 188});
			colors.push_back({255, 188, 188});
			colors.push_back({255, 225, 188});
			colors.push_back({255, 255, 188});
			colors.push_back({0, 96, 100});
			colors.push_back({0, 131, 143});
			colors.push_back({0, 151, 167});
			colors.push_back({0, 172, 193});
			colors.push_back({0, 188, 212});
			colors.push_back({38, 198, 218});
			colors.push_back({77, 208, 225});
			colors.push_back({128, 222, 234});
			colors.push_back({0, 0, 188});
			colors.push_back({0, 137, 188});
			colors.push_back({0, 188, 188});
			colors.push_back({0, 225, 188});
			colors.push_back({0, 255, 188});
			colors.push_back({137, 255, 188});
			colors.push_back({188, 255, 188});
			colors.push_back({225, 255, 188});
			colors.push_back({1, 87, 155});
			colors.push_back({2, 119, 189});
			colors.push_back({2, 136, 209});
			colors.push_back({3, 155, 229});
			colors.push_back({3, 169, 244});
			colors.push_back({41, 182, 246});
			colors.push_back({79, 195, 247});
			colors.push_back({129, 212, 250});
			colors.push_back({127, 30, 156});
			colors.push_back({179, 30, 139});
			colors.push_back({213, 56, 122});
			colors.push_back({234, 92, 108});
			colors.push_back({243, 131, 103});
			colors.push_back({245, 167, 113});
			colors.push_back({243, 201, 139});
			colors.push_back({243, 231, 180});
			colors.push_back({42, 54, 177});
			colors.push_back({59, 80, 206});
			colors.push_back({69, 94, 222});
			colors.push_back({78, 108, 239});
			colors.push_back({86, 119, 252});
			colors.push_back({115, 143, 254});
			colors.push_back({145, 167, 255});
			colors.push_back({175, 191, 255});
			colors.push_back({39, 26, 135});
			colors.push_back({109, 20, 146});
			colors.push_back({167, 9, 138});
			colors.push_back({214, 26, 125});
			colors.push_back({243, 77, 97});
			colors.push_back({249, 129, 75});
			colors.push_back({252, 171, 57});
			colors.push_back({252, 209, 61});
			colors.push_back({26, 35, 126});
			colors.push_back({40, 53, 147});
			colors.push_back({48, 63, 159});
			colors.push_back({57, 73, 171});
			colors.push_back({63, 81, 181});
			colors.push_back({92, 107, 192});
			colors.push_back({121, 134, 203});
			colors.push_back({159, 168, 218});
			colors.push_back({62, 39, 35});
			colors.push_back({78, 52, 46});
			colors.push_back({93, 64, 55});
			colors.push_back({109, 76, 65});
			colors.push_back({121, 85, 72});
			colors.push_back({141, 110, 99});
			colors.push_back({161, 136, 127});
			colors.push_back({188, 170, 164});
			colors.push_back({49, 27, 146});
			colors.push_back({69, 39, 160});
			colors.push_back({81, 45, 168});
			colors.push_back({94, 53, 177});
			colors.push_back({103, 58, 183});
			colors.push_back({126, 87, 194});
			colors.push_back({149, 117, 205});
			colors.push_back({179, 157, 219});
			colors.push_back({38, 50, 56});
			colors.push_back({55, 71, 79});
			colors.push_back({69, 90, 100});
			colors.push_back({84, 110, 122});
			colors.push_back({96, 125, 139});
			colors.push_back({120, 144, 156});
			colors.push_back({144, 164, 174});
			colors.push_back({176, 190, 197});
			colors.push_back({74, 20, 140});
			colors.push_back({106, 27, 154});
			colors.push_back({123, 31, 162});
			colors.push_back({142, 36, 170});
			colors.push_back({156, 39, 176});
			colors.push_back({171, 71, 188});
			colors.push_back({186, 104, 200});
			colors.push_back({206, 147, 216});

			defaultColor = 3;
		}

		PaletteView::~PaletteView() {}

		int PaletteView::GetSelectedIndex() {
			World *w = client->GetWorld();
			if (!w)
				return -1;

			Player *p = w->GetLocalPlayer();
			if (!p)
				return -1;

			IntVector3 col = p->GetBlockColor();
			for (int i = 0; i < (int)colors.size(); i++) {
				if (col.x == colors[i].x && col.y == colors[i].y && col.z == colors[i].z)
					return i;
			}
			return -1;
		}

		int PaletteView::GetSelectedOrDefaultIndex() {
			int c = GetSelectedIndex();
			if (c == -1)
				return defaultColor;
			else
				return c;
		}

		void PaletteView::SetSelectedIndex(int idx) {
			IntVector3 col = colors[idx];

			World *w = client->GetWorld();
			if (!w)
				return;

			Player *p = w->GetLocalPlayer();
			if (!p)
				return;

			p->SetHeldBlockColor(col);

			client->net->SendHeldBlockColor();
		}

		bool PaletteView::KeyInput(std::string keyName) {
			if (EqualsIgnoringCase(keyName, cg_keyPaletteLeft)) {
				int c = GetSelectedOrDefaultIndex();
				if (c == 0)
					c = (int)colors.size() - 1;
				else
					c--;
				SetSelectedIndex(c);
				return true;
			} else if (EqualsIgnoringCase(keyName, cg_keyPaletteRight)) {
				int c = GetSelectedOrDefaultIndex();
				if (c == (int)colors.size() - 1)
					c = 0;
				else
					c++;
				SetSelectedIndex(c);
				return true;
			} else if (EqualsIgnoringCase(keyName, cg_keyPaletteUp)) {
				int c = GetSelectedOrDefaultIndex();
				if (c < 16)
					c += (int)colors.size() - 16;
				else
					c -= 16;
				SetSelectedIndex(c);
				return true;
			} else if (EqualsIgnoringCase(keyName, cg_keyPaletteDown)) {
				int c = GetSelectedOrDefaultIndex();
				if (c >= (int)colors.size() - 16)
					c -= (int)colors.size() - 16;
				else
					c += 16;
				SetSelectedIndex(c);
				return true;
			} else if (EqualsIgnoringCase(keyName, cg_keyPaletteInvert)) {
				World *w = client->GetWorld();
				if (!w)
					return true;
				Player *p = w->GetLocalPlayer();
				if (!p)
					return true;
				IntVector3 clr = p->GetBlockColor();
				clr.x = 255 - clr.x;
				clr.y = 255 - clr.y;
				clr.z = 255 - clr.z;
				p->SetHeldBlockColor(clr);
				client->net->SendHeldBlockColor();
				return true;
			} else if (EqualsIgnoringCase(keyName, cg_keyPaletteRandom)) {
				World *w = client->GetWorld();
				if (!w)
					return true;
				Player *p = w->GetLocalPlayer();
				if (!p)
					return true;
				IntVector3 clr = p->GetBlockColor();
				clr.x = SampleRandomInt(0, 255);
				clr.y = SampleRandomInt(0, 255);
				clr.z = SampleRandomInt(0, 255);
				colors[defaultColor] = clr;
				p->SetHeldBlockColor(clr);
				client->net->SendHeldBlockColor();
				return true;
			} else if (EqualsIgnoringCase(keyName, cg_keyPaletteMix)) {
				World *w = client->GetWorld();
				if (!w)
					return true;
				Player *p = w->GetLocalPlayer();
				if (!p)
					return true;

				IntVector3 clr_a = p->GetBlockColor();
				double a_x = clr_a.x / 255.0;
				double a_y = clr_a.y / 255.0;
				double a_z = clr_a.z / 255.0;
				a_x = a_x <= 0.04045 ? a_x / 12.92 : std::pow((a_x + 0.055) / 1.055, 2.4);
				a_y = a_y <= 0.04045 ? a_y / 12.92 : std::pow((a_y + 0.055) / 1.055, 2.4);
				a_z = a_z <= 0.04045 ? a_z / 12.92 : std::pow((a_z + 0.055) / 1.055, 2.4);

				client->CaptureColor();

				IntVector3 clr_b = p->GetBlockColor();
				double b_x = clr_b.x / 255.0;
				double b_y = clr_b.y / 255.0;
				double b_z = clr_b.z / 255.0;
				b_x = b_x <= 0.04045 ? b_x / 12.92 : std::pow((b_x + 0.055) / 1.055, 2.4);
				b_y = b_y <= 0.04045 ? b_y / 12.92 : std::pow((b_y + 0.055) / 1.055, 2.4);
				b_z = b_z <= 0.04045 ? b_z / 12.92 : std::pow((b_z + 0.055) / 1.055, 2.4);

				b_x = (a_x + b_x) / 2.;
				b_y = (a_y + b_y) / 2.;
				b_z = (a_z + b_z) / 2.;
				b_x = b_x <= 0.0031308 ? b_x * 12.92 : (1.055 * std::pow(b_x, 1./2.4) - 0.055);
				b_y = b_y <= 0.0031308 ? b_y * 12.92 : (1.055 * std::pow(b_y, 1./2.4) - 0.055);
				b_z = b_z <= 0.0031308 ? b_z * 12.92 : (1.055 * std::pow(b_z, 1./2.4) - 0.055);
				b_x = b_x * 255.0 + 0.49;
				b_y = b_y * 255.0 + 0.49;
				b_z = b_z * 255.0 + 0.49;
				clr_b.x = (int)b_x;
				clr_b.y = (int)b_y;
				clr_b.z = (int)b_z;

				colors[defaultColor] = clr_b;
				p->SetHeldBlockColor(clr_b);
				client->net->SendHeldBlockColor();
				return true;
			} else {
				return false;
			}
		}

		void PaletteView::Update() {}

		void PaletteView::Draw() {
			Handle<IImage> img = renderer->RegisterImage("Gfx/Palette.png");

			int sel = GetSelectedIndex();

			float scrW = renderer->ScreenWidth();
			float scrH = renderer->ScreenHeight();

			for (size_t phase = 0; phase < 2; phase++) {
				for (size_t i = 0; i < colors.size(); i++) {
					if ((sel == i) != (phase == 1))
						continue;

					int row = static_cast<int>(i / 16);
					int col = static_cast<int>(i % 16);

					bool selected = sel == i;

					// draw color
					IntVector3 icol = colors[i];
					Vector4 cl;
					cl.x = icol.x / 255.f;
					cl.y = icol.y / 255.f;
					cl.z = icol.z / 255.f;
					cl.w = 1.f;

					float x = scrW - 180.f + 10.f * col;
					float y = scrH - 186.f + 10.f * row - 40.f;

					renderer->SetColorAlphaPremultiplied(cl);
					if (selected) {
						renderer->DrawImage(img, MakeVector2(x, y), AABB2(0, 16, 16, 16));
					} else {
						renderer->DrawImage(img, MakeVector2(x, y), AABB2(0, 0, 16, 16));
					}

					renderer->SetColorAlphaPremultiplied(MakeVector4(1, 1, 1, 1));
					if (selected) {
						renderer->DrawImage(img, MakeVector2(x, y), AABB2(16, 16, 16, 16));
					} else {
						renderer->DrawImage(img, MakeVector2(x, y), AABB2(16, 0, 16, 16));
					}
				}
			}
		}
	}
}
