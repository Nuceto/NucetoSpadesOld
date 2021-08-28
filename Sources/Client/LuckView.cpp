#include "IFont.h"
#include "LuckView.h"
#include "Client.h"
#include "IRenderer.h"
#include "Player.h"
#include "World.h"

#include <Core/Settings.h>
#include <Core/Debug.h>

#include <cstdarg>
#include <cstdlib>
#include <ctime>

#include "Fonts.h"


SPADES_SETTING(cg_minimapSize);
SPADES_SETTING(n_hudTransparency, "1");

spades::client::LuckView* spades::client::LuckView::instance = NULL;

void RightJustText(spades::client::IRenderer *renderer, spades::client::IFont *font, int lineno, const char *msg, ...) {
	char buf[4096];
	va_list va;
	va_start(va, msg);
	vsnprintf(buf, sizeof(buf), msg, va);
	va_end(va);
	
	spades::Vector2 size = font->Measure(buf);
	
	float x = renderer->ScreenWidth() - size.x - 16;

	float y = cg_minimapSize;
	if (y < 32)
		y = 32;
	if (y > 256)
		y = 256;
	y += 32 + lineno * size.y;

	font->DrawShadow(buf, spades::MakeVector2(x, y), 1, spades::MakeVector4(1, 1, 1, (float)n_hudTransparency), spades::MakeVector4(0, 0, 0, (float)n_hudTransparency));
}

namespace spades {
	namespace client {
		LuckView::LuckView(Client *cli, IFont *font) : client(cli), renderer(cli->GetRenderer()), font(font) {
			SPADES_MARK_FUNCTION();

			if (instance != NULL)
				SPAssert(false);
			
			instance = this;
		}

		LuckView::~LuckView() {
			PrintStats("shutting down");
			instance = NULL;
		}

		void LuckView::PrintStats(const char* str) {
			SPLog("------------------------");
			SPLog("LuckView %s. Stats:", str);
			SPLog("clicksHead = %d", clicksHead);
			SPLog("clicksPlayer = %d", clicksPlayer);
			SPLog("hitsHead = %d", hitsHead);
			SPLog("hitsPlayer = %d", hitsPlayer);
			SPLog("shotsCount = %d", shotsCount);
			SPLog("totalActualDamage = %d", totalActualDamage);
			SPLog("totalNospreadDamage = %d", totalNospreadDamage);
			SPLog("ratioKills = %d", ratioKills);
			SPLog("ratioDeaths = %d", ratioDeaths);
			SPLog("streakBest = %d", streakBest);
			SPLog("------------------------");
		}

		void LuckView::ClearAll() {
			PrintStats("resetting");

			clicksHead = 0;
			clicksPlayer = 0;
			hitsHead = 0;
			hitsPlayer = 0;
			shotsCount = 0;
			totalActualDamage = 0;
			totalNospreadDamage = 0;
			ratioKills = 0;
			ratioDeaths = 0;
			streakCurrent = 0;
			streakLast = 0;
			streakBest = 0;
		}

		void LuckView::Add(bool clickedHead, bool clickedPlayer, bool hitHead, bool hitPlayer, int nospreadDamage, int actualDamage) {
			SPADES_MARK_FUNCTION();
			// TODO: distinguish "not even close" (for random sky shots / babel tower shots / etc)
			if (clickedHead) clicksHead++;
			if (clickedPlayer) clicksPlayer++;
			if (hitHead) hitsHead++;
			if (hitPlayer) hitsPlayer++;
			shotsCount++;
			totalActualDamage += actualDamage > 100 ? 100 : actualDamage;
			totalNospreadDamage += nospreadDamage > 100 ? 100 : nospreadDamage;
			// should calculate expectation in? e.g. 33% chance of headshot at fogrange so nospread /= 3
			// shotgun pellets separate?
			// divide luck by shots? divide actual by nospread?
		}

		// should not be called for suicides
		void LuckView::Ratio_Kill() {
			ratioKills++;
			streakCurrent++;
			
		}
		void LuckView::Ratio_Death() {
			ratioDeaths++;
			streakLast = streakCurrent;
			if (streakCurrent > streakBest) {
				streakBest = streakCurrent;
			}
			streakCurrent = 0;
		}

		void LuckView::Draw() {
			SPADES_MARK_FUNCTION();

			float y = cg_minimapSize;
			if (y < 32)
				y = 32;
			if (y > 256)
				y = 256;
			y += 32;

			int sc = shotsCount;
			if (sc == 0) sc = 1;
			int lineno = 0;
#define T(...) RightJustText(renderer, font, lineno++, __VA_ARGS__)
			T("True Accuracy: %d%%", (100 * clicksPlayer) / sc);
			T("Spread Accuracy: %d%%", (100 * hitsPlayer) / sc);
			lineno++;
			T("[Headshots Only]");
			T("True Accuracy: %d%%", (100 * clicksHead) / sc);
			T("Spread Accuracy: %d%%", (100 * hitsHead) / sc);
			lineno++;
			//T("Luck: %d", totalActualDamage - totalNospreadDamage);
			//lineno++;
			T("Ratio: %d / %d = %.1f", ratioKills, ratioDeaths, ratioKills/(float)(ratioDeaths?ratioDeaths:1));
			lineno++;
			T("Streak: %d", streakCurrent);
			T("Last Streak: %d", streakLast);
			T("Best Streak: %d", streakBest);
			/*
			T("[Percents]");
			T("True Accuracy: 100%");
			T("Spread Accuracy: 41%");
			T("Headshot Accuracy: 10%");
			lineno++;
			T("[Number of kills]");
			T("Good Luck: X");
			T("Bad Luck: X");
			lineno++;
			T("[Counters]");
			T("Direct Hit: X");
			T("Promoted: X");
			T("Nerfed: X");
			T("Missed: X");
			T("Not Even Close: X");
			*/
#undef T
		}
	}
}
