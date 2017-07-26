#include "CDrawX.h"
#include "Fonts.h"
#include "Menu.h"
#include <ctime>

void Menu::drawTaskbar(int x, int y)
{
	int width = 50;

	int ly = y;
	for (unsigned int i = 0; i < windowList.size(); i++)
	{
		int tw = 0, th = 0;
		gInts.Surface->GetTextSize(fonts.titleFont, fonts.to_wchar(windowList[i].name), tw, th);

		if (gMenu.mouseOver(x, ly, width, th - 1))
		{
			if (gMenu.mb == e_mb::LCLICK)
				windowList[i].visible = !windowList[i].visible;

			int rectw = tw < width ? width : tw;
			gDraw.DrawRect(x + width - rectw - 12, ly, rectw + 12, th, colors.dark);
			if (windowList[i].visible)
				gDraw.DrawRect(x + width - rectw - 14, ly, 2, th, colors.red);

			gDrawEx.DrawString(x + width - tw - 8, ly, colors.light, fonts.titleFont, windowList[i].name);

			ly += th;
			continue;
		}

		gDraw.DrawRect(x, ly, width, th, colors.dark);
		if (windowList[i].visible)
			gDraw.DrawRect(x, ly, 2, th, colors.red);

		string shortName = windowList[i].name;
		shortName = shortName.substr(0, 3); // Make a string from a char array to use string functions
		gDrawEx.DrawString(x + 8, ly, colors.lightGray, fonts.titleFont, shortName.c_str());

		ly += th;
	}
}

int wTools::GetWinHeight(Window &window, WindowStyle &style)
{
	int current = 0, highest = 0; // Used to find the largest amount of items in one line
	for (int i = 0; i < window.items.size(); i++) // Make sure that we avoid adding height with newlines
	{
		if (window.items[i].type == "newColumn")
			current = 0;
		else if (window.items[i].type == "bool")
			current += style.drawCheckbox(window.items[i], 0, 0, false);
		else if (window.items[i].type == "decimal")
			current += style.drawSlider(window.items[i], 0, 0, false);
		else if (window.items[i].type == "list")
			current += style.drawList(window.items[i], 0, 0, false);
		else if (window.items[i].type == "space")
			current += style.spaceSize;

		current += style.itemspace;

		if (current > highest)
			highest = current;
	}
	return 43 + highest;
}

void handleInput(MenuItem &item)
{
	static int ldown = clock(), rdown = clock();

	if (!gMenu.canFocus("Items"))
	{
		ldown = clock(), rdown = clock();
		return;
	}

	if (!gMenu.keyDown(VK_LBUTTON))
		ldown = clock();
	if (!gMenu.keyDown(VK_RBUTTON))
		rdown = clock();

	if (gMenu.mb == e_mb::LCLICK || (clock() - ldown > 500 && (clock() - ldown) % 2 == 0))
	{
		// If you're about to go below the min, then go back to the max
		if (item.value[0] - item.step < item.min)
			item.value[0] = item.max;

		else // Otherwise, keep going down
			item.value[0] -= item.step;
	}
	else if (gMenu.mb == e_mb::RCLICK || (clock() - rdown > 500 && (clock() - rdown) % 2 == 0))
	{
		// If you're about to go above the max, then go back to the min
		if (item.value[0] + item.step > item.max)
			item.value[0] = item.min;

		else // Otherwise, keep going up
			item.value[0] += item.step;
	}
}

// BetterMenu style
// -----------------------------------------------------------------------------------------------------------
void wTools::bToolbar(Window &window)
{
	int newWidth = window.w * (window.newlines + 1);

	gDraw.DrawRect(window.x, window.y, newWidth, 33, colors.dark); // Draw title bar
	gDraw.DrawRect(window.x, window.y + 31, newWidth, 2, colors.bluTeam); // Extra blue thingy below it
	gDrawEx.DrawString(window.x + 10, window.y + 3, colors.lightBlue, fonts.titleFont, window.name);
}

void wTools::bBackground(Window &window)
{
	int newHeight = window.minimized ? 33 : GetWinHeight(window, gMenu.BetterMenu);
	int newWidth = window.w * (window.newlines + 1);

	gDraw.DrawRect(window.x, window.y, newWidth, newHeight, colors.menuBack);
}

void wTools::bOutline(Window &window)
{
	int newHeight = window.minimized ? 33 : GetWinHeight(window, gMenu.BetterMenu);
	int newWidth = window.w * (window.newlines + 1);

	gDraw.OutlineRect(window.x - 1, window.y - 1, newWidth + 2, newHeight + 2, colors.black);
}

int wTools::bCheckbox(MenuItem &item, int x, int y, bool draw)
{
	int height = 16;
	if (!draw)
		return height;

	DWORD color = colors.light;
	if (gMenu.mouseOver(x, y, item.width, height))
	{
		color = colors.bluTeam;
		handleInput(item);
	}

	gDraw.OutlineRect(x, y, 16, 16, colors.gray); // Draw checkbox outline
	gDraw.DrawRect(x + 1, y + 1, 14, 14, colors.dark); // Draw checkbox background
	gDrawEx.DrawString(x + 20, y, color, fonts.itemFont, item.name);

	if (item.value[0] == 1) // If the value = true, add a check in the box
		gDraw.DrawRect(x + 3, y + 3, 10, 10, colors.bluTeam);

	return height;
}

int wTools::bSlider(MenuItem &item, int x, int y, bool draw)
{
	int height = 26;
	if (!draw)
		return height;

	DWORD color = colors.light;
	if (gMenu.mouseOver(x, y, item.width, height))
	{
		color = colors.bluTeam;
		handleInput(item);
	}

	int percent = item.width * ((int)item.value[0] - item.min) / (item.max - item.min);

	gDrawEx.DrawString(x, y, color, fonts.itemFont, item.name);
	gDraw.DrawRect(x, y + 15, item.width, 8, colors.dark); // Slider background
	gDraw.DrawRect(x, y + 15, percent, 8, color); // Slider progress
	gDraw.OutlineRect(x, y + height, item.width, 1, colors.gray); // Extra line below slider

	// Draw the value next to the progress of the slider
	gDrawEx.DrawString(percent + x + 3, y + 12, color, fonts.itemFont, fonts.floatStr(item.value[0]).c_str());

	return height;
}

int wTools::bList(MenuItem &item, int x, int y, bool draw)
{
	int height = 26;
	if (!draw)
		return height;

	DWORD color = colors.light;
	if (gMenu.mouseOver(x, y, item.width, height))
	{
		color = colors.bluTeam;

		if (gMenu.mb == e_mb::LCLICK && gMenu.canFocus("Items"))
			item.focused = !item.focused;
	}

	gDrawEx.DrawString(x, y, color, fonts.itemFont, item.name);

	gDraw.DrawRect(x, y + 15, item.width, 8, colors.dark); // Draw the background
														   // Current item
	gDrawEx.DrawString(x + 3, y + 13, color, fonts.itemFont, item.strValue[0].c_str());
	gDraw.OutlineRect(x, y + height, item.width, 1, colors.gray); // Extra line below the listbox

	return height;
}

void wTools::bListbox(MenuItem &item, int x, int y)
{
	static int tw, th = 0;
	if (!th)
		gInts.Surface->GetTextSize(fonts.itemFont, L"Text", tw, th);

	int h = bList(item, x, y, false) + 5;
	if (gMenu.mb == e_mb::LCLICK &&
		!gMenu.mouseOver(x - 1, y, item.width + 2, th * item.list.size() + h))
	{
		item.focused = false;
		return;
	}

	for (int i = 0; i < item.list.size(); i++)
	{
		DWORD color = COLORCODE(30, 30, 30, 255);
		if (gMenu.mouseOver(x, y + h, item.width, th - 1))
		{
			color = colors.bluTeam;

			if (gMenu.mb == e_mb::LCLICK)
			{
				item.strValue[0] = item.list[i];
				item.focused = false;
				return;
			}
		}

		gDraw.DrawRect(x, y + h, item.width, th, color);
		gDrawEx.DrawString(x + 2, y + h, colors.light, fonts.itemFont, item.list[i].c_str());
		h += th;
	}
	gDraw.OutlineRect(x - 1, y + bList(item, x, y, false) + 4, item.width + 2, th * item.list.size() + 2, colors.lightGray);
}

// Gray style
// ------------------------------------------------------------------------------------------------------------
void wTools::dToolbar(Window &window)
{
	int newWidth = window.w * (window.newlines + 1);

	gDraw.DrawRect(window.x, window.y, newWidth, 33, COLORCODE(60, 60, 60, 255));
	gDrawEx.DrawString(window.x + 10, window.y + 4, colors.light, fonts.titleFont, window.name);
}

void wTools::dBackground(Window &window)
{
	int newHeight = GetWinHeight(window, gMenu.Gray);
	int newWidth = window.w * (window.newlines + 1);

	gDraw.DrawRect(window.x, window.y + 33, newWidth, newHeight - 33, COLORCODE(40, 40, 40, 240));
	gDrawEx.GradientRect(window.x, window.y + 33, newWidth, 6, true, COLORCODE(0, 0, 0, 100), COLORCODE(0, 0, 0, 0));
}

void wTools::dOutline(Window &window)
{
	int newHeight = window.minimized ? 33 : GetWinHeight(window, gMenu.Gray);
	int newWidth = window.w * (window.newlines + 1);

	for (int i = 1; i < 9; i++)
	{
		DWORD color = COLORCODE(0, 0, 0, 80 - (i * 10));
		gDraw.OutlineRect(window.x - i, window.y - i, newWidth + (i * 2), newHeight + (i * 2), color);
	}
}

int wTools::dCheckbox(MenuItem &item, int x, int y, bool draw)
{
	int height = 13;
	if (!draw)
		return height;

	if (gMenu.mouseOver(x, y, item.width, 13))
	{
		gDrawEx.RoundRect(x, y, item.width, 15, 5, Textures.lightGray);
		handleInput(item);
	}

	gDrawEx.DrawString(x + 5, y, item.value[0] ? colors.light : colors.lightGray, fonts.smallFont, item.name);

	return height;
}

int wTools::dSlider(MenuItem &item, int x, int y, bool draw)
{
	int height = 25;
	if (!draw)
		return height;

	if (gMenu.mouseOver(x, y, item.width, height))
	{
		gDrawEx.RoundRect(x, y, item.width, height + 2, 5, Textures.lightGray);
		handleInput(item);
	}

	int percent = (item.width - 10) * ((int)item.value[0] - item.min) / (item.max - item.min);

	gDrawEx.DrawString(x + 5, y, colors.light, fonts.smallFont, item.name);
	gDrawEx.RoundRect(x + 4, y + 13, item.width - 8, 7, 4, Textures.darkGray);
	gDrawEx.RoundRect(x + 5, y + 14, percent, 5, 3, Textures.light);
	gDrawEx.DrawString(x + 5 + percent, y + 13, colors.light, fonts.smallFont, fonts.floatStr(item.value[0]).c_str());

	return height;
}

int wTools::dList(MenuItem &item, int x, int y, bool draw)
{
	int height = 22;
	if (!draw)
		return height;

	if (gMenu.mouseOver(x, y, item.width, height))
	{
		gDrawEx.RoundRect(x, y, item.width, height + 2, 5, Textures.lightGray);

		if (gMenu.mb == e_mb::LCLICK && gMenu.canFocus("Items"))
			item.focused = !item.focused;
	}

	gDrawEx.DrawString(x + 5, y, colors.light, fonts.smallFont, item.name);
	gDrawEx.RoundRect(x + 3, y + 13, item.width - 7, 10, 4, Textures.darkGray);
	// Current item
	gDrawEx.DrawString(x + 3, y + 10, colors.light, fonts.smallFont, item.strValue[0].c_str());

	return height;
}