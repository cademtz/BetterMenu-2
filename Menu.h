#pragma once
#include "SDK.h"
#include <vector>
#include <string>

// Structs to keep all the data of a menu item in one variable
struct MenuItem
{
	char name[30] = "Null";
	string type;
	int width = 100;

	float *value;
	float min, max, step;

	string *strValue;
	vector<string> list;
	bool focused = false;
};
typedef vector<MenuItem> ItemList;

struct Window
{
	char name[30] = "Null"; // Make sure we have defaults set
	int x = 40, y = 40, w = 150, newlines = 0;
	bool moving = false, snap = false, visible = true, minimized = false;
	vector<MenuItem> items;
};

enum class e_mb
{
	LDOWN, LCLICK,
	RDOWN, RCLICK,
	SCROLLDOWN, SCROLLUP,
	NONE
};

struct WindowStyle
{
	void(*drawToolbar)(Window &window);
	void(*drawBackground)(Window &window);
	void(*drawOutline)(Window &window);
	int(*drawCheckbox)(MenuItem &item, int x, int y, bool draw);
	int(*drawSlider)(MenuItem &item, int x, int y, bool draw);
	int(*drawList)(MenuItem &item, int x, int y, bool draw);
	void(*drawListbox)(MenuItem &item, int x, int y);
	int itemspace;
	int spaceSize; // Determines how many pixels items are separated by when adding a space

	void drawWindow(Window &window)
	{
		drawBackground(window); drawToolbar(window); drawOutline(window);
	}
};


class wTools
{
public:
	static void bToolbar(Window &window);
	static void bBackground(Window &window);
	static void bOutline(Window &window);

	static int bCheckbox(MenuItem &item, int x, int y, bool draw);
	static int bSlider(MenuItem &item, int x, int y, bool draw);
	static int bList(MenuItem &item, int x, int y, bool draw);
	static void bListbox(MenuItem &item, int x, int y);

	static void dToolbar(Window &window);
	static void dBackground(Window &window);
	static void dOutline(Window &window);

	static int dCheckbox(MenuItem &item, int x, int y, bool draw);
	static int dSlider(MenuItem &item, int x, int y, bool draw);
	static int dList(MenuItem &item, int x, int y, bool draw);

	static int GetWinHeight(Window &window, WindowStyle &style);
};

class Menu
{
public:
	void paint();
	bool menuToggle = false;

	// An array of windows that are automatically rendered
	vector<Window> windowList;

	// ez drawing funcs
	int drawItem(MenuItem &item, int x, int y);
	void drawWindow(Window &window);
	void drawTaskbar(int x, int y);
	void openListbox(MenuItem &item, int x, int y);

	string focus = "";
	bool canFocus(string name)
	{
		if (focus == "" || focus == name)
			return true;
		return false;
	}

	bool mouseOver(int x, int y, int w, int h); // Easy way to get mouse input
	bool mouseOver(Window &window);

	bool keyDown(int vKey);
	POINT mouse = { 0, 0 }; // Current mouse position
	POINT pmouse = { 0, 0 }; // Previous mouse position
	e_mb mb = e_mb::NONE;

	string wmText = "Not for release";
	float wmShow = true;
	void init();

	// Window styles!
	WindowStyle BetterMenu = { wTools::bToolbar, wTools::bBackground, wTools::bOutline,
		wTools::bCheckbox, wTools::bSlider, wTools::bList, wTools::bListbox, 10, 10 };
	WindowStyle Gray = { wTools::dToolbar, wTools::dBackground, wTools::dOutline,
		wTools::dCheckbox,wTools::dSlider, wTools::dList, wTools::bListbox, 6, 5 };
private:

	// Functions to easily add menu controls
	MenuItem addSpace();
	MenuItem newColumn();
	MenuItem addBool(char name[30], float &value);
	MenuItem addList(char name[30], string &value, vector<string> options);
	MenuItem addSlider(char name[30], float &value, float min, float max, float step);
	void addWindow(char name[30], int width, vector<MenuItem> items);

	WindowStyle *menuStyle = &BetterMenu; // Multiple menu styles!
	float currentStyle = 1;

	void getInput(); // Gets all the mouse info
	void createGUI(); // Adds the windows to an array

	MenuItem* currentList = nullptr;
	POINT currentListPos;

	// Some random test variables
	float espEnabled = false;
	float espBoxes = true;
	float espHealth = true;

	float miscBool = false;
	string miscList = "Option 1";
	float miscSlider = 5;
};
extern Menu gMenu;