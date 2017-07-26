#include "Menu.h"
#include "CDrawX.h"
#include "Fonts.h"
// #include "YourCheatHeader.h" and then add your variables to a menu window below!
Menu gMenu;

void Menu::createGUI()
{
	ItemList esp =
	{
		addBool("Enabled", espEnabled),
		addSpace(),
		addBool("Boxes", espBoxes),
		addBool("Health", espHealth)
	};
	addWindow("ESP", 100, esp);

	ItemList misc =
	{
		addSlider("Menu style", currentStyle, 1, 2, 1),
		addBool("Bool/Checkbox", miscBool),
		newColumn(),
		addList("List", miscList, {"Option 1", "Option 2", "Option 3"}),
		addSlider("Slider", miscSlider, -5, 10, 2.5)
	};
	addWindow("Misc", 120, misc);
}

void Menu::openListbox(MenuItem &item, int x, int y)
{
	if (canFocus("Listbox"))
	{
		currentList = &item;
		currentListPos = { x, y };
		focus = "Listbox";
	}
}

int Menu::drawItem(MenuItem &item, int x, int y)
{
	if (item.type == "bool")
		return menuStyle[0].drawCheckbox(item, x, y, true);
	else if (item.type == "decimal")
		return menuStyle[0].drawSlider(item, x, y, true);
	else if (item.type == "list")
	{
		int out = menuStyle[0].drawList(item, x, y, true);
		if (item.focused == true)
			openListbox(item, x, y);
		return out;
	}
	else if (item.type == "space")
		return menuStyle[0].spaceSize;

	return 0;
}

void Menu::drawWindow(Window &window)
{
	menuStyle[0].drawWindow(window);
	if (window.minimized)
		return;

	// Position of first item
	int x = window.x + 10;
	int y = window.y + 41;

	for (unsigned int i = 0; i < window.items.size(); i++) // Draw all the items
	{
		if (window.items[i].type == "newColumn") {
			y = window.y + 41, x += window.w; continue; }

		y += drawItem(window.items[i], x, y);
		y += menuStyle[0].itemspace;
	}
}

void Menu::paint()
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
		menuToggle = !menuToggle;

	if (!menuToggle)
		return;

	getInput(); // Get mouse info

	if (currentStyle == 1)
		menuStyle = &BetterMenu;
	else if (currentStyle == 2)
		menuStyle = &Gray;

	drawTaskbar(gScreenSize.iScreenWidth - 70, 12);

	for (unsigned int i = 0; i < windowList.size(); i++) // Draw all the windows
	{
		static bool isWindowMoving = false; // Make sure we don't have two windows stuck to the cursor

		if (!windowList[i].visible)
			continue;

		if (canFocus("Windows"))
		{
			if (!isWindowMoving && mouseOver(windowList[i]) && (keyDown(VK_LBUTTON) || keyDown(VK_RBUTTON)))
			{
				windowList[i].moving = true; isWindowMoving = true;
				if (keyDown(VK_LBUTTON))
					windowList[i].snap = false;
				else if (keyDown(VK_RBUTTON))
					windowList[i].snap = true;
			}
			if (!(keyDown(VK_LBUTTON) || keyDown(VK_RBUTTON)))
				windowList[i].moving = false, isWindowMoving = false;

			if (windowList[i].moving)
			{
				if (windowList[i].snap) // Snap to a 10x10 pixel grid
				{
					windowList[i].x = floor(mouse.x / 10) * 10;
					windowList[i].y = floor(mouse.y / 10) * 10;
				}
				else
				{
					windowList[i].x += mouse.x - pmouse.x;
					windowList[i].y += mouse.y - pmouse.y;
				}
			}

			if (windowList[i].x < 0) // Clamping the window so it doesn't go off-screen
				windowList[i].x = 0;
			else if (windowList[i].x + windowList[i].w > gScreenSize.iScreenWidth)
				windowList[i].x = gScreenSize.iScreenWidth - windowList[i].w;

			if (windowList[i].y < 0)
				windowList[i].y = 0;
			else if (windowList[i].y + 33 > gScreenSize.iScreenHeight)
				windowList[i].y = gScreenSize.iScreenHeight - 33;

			focus = isWindowMoving ? "Windows" : "";
		}
		drawWindow(windowList[i]);
	}

	if (currentList != nullptr)
	{
		if (currentList[0].focused)
			menuStyle[0].drawListbox(currentList[0], currentListPos.x, currentListPos.y);
		else
		{
			currentList = nullptr;
			focus = "";
		}
	}
}

void Menu::init()
{
	createGUI(); // Add all the windows
}

void Menu::getInput()
{	
	pmouse = mouse;

	int mousex = 0, mousey = 0;
	gInts.Surface->GetCursorPosition(mousex, mousey);
	mouse.x = mousex, mouse.y = mousey;

	if (keyDown(VK_LBUTTON))
	{
		if (mb == e_mb::NONE) mb = e_mb::LCLICK;
		else mb = e_mb::LDOWN;
	}
	else if (keyDown(VK_RBUTTON))
	{
		if (mb == e_mb::NONE) mb = e_mb::RCLICK;
		else mb = e_mb::RDOWN;
	}
	else
		mb = e_mb::NONE;
}

bool Menu::keyDown(int vKey)
{
	if (GetKeyState(vKey) < 0)
		return true;
	return false;
}

bool Menu::mouseOver(int x, int y, int w, int h)
{
	if (mouse.x >= x && mouse.x <= x + w && mouse.y >= y && mouse.y <= y + h)
		return true;
	return false;
}
bool Menu::mouseOver(Window &window)
{
	return mouseOver(window.x, window.y, window.w * (window.newlines + 1), 33);
}

MenuItem Menu::addSpace()
{
	MenuItem item;
	item.type = "space";
	return item;
}

MenuItem Menu::newColumn()
{
	MenuItem item;
	item.type = "newColumn";
	return item;
}

MenuItem Menu::addBool(char name[30], float &value)
{
	MenuItem item;
	item.type = "bool";
	item.value = &value;
	item.min = 0;
	item.max = 1;
	item.step = 1;
	strcpy(item.name, name);

	return item;
}

MenuItem Menu::addList(char name[30], string &value, vector<string> options)
{
	MenuItem item;
	item.type = "list";
	item.strValue = &value;
	item.list = options;
	strcpy(item.name, name);

	return item;
}

MenuItem Menu::addSlider(char name[30], float &value, float min, float max, float step)
{
	MenuItem item;
	item.type = "decimal";
	item.value = &value;
	item.min = min;
	item.max = max;
	item.step = step;
	strcpy(item.name, name);

	return item;
}

void Menu::addWindow(char name[30], int width, vector<MenuItem> items)
{
	Window temp; // Make a temporary window to avoid initialization parameters
	strcpy(temp.name, name);
	temp.w = width;

	for (unsigned int i = 0; i < items.size(); i++) // Set all the item widths to the window's width
	{
		if (items[i].type == "newColumn") // Make sure we can pre-determine the window's actual width
			temp.newlines++;
		items[i].width = width - 30;
	}

	temp.items = items;
	temp.visible = false;
	windowList.push_back(temp); // Add the window
}