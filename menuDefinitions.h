//Actual menus
static normalMenu_t topMenu(NULL);
static normalMenu_t filesMenu(&topMenu);
static normalMenu_t settingsMenu(&topMenu);
static fileMenu_t testMenu(&filesMenu);

//Menu resources
char * topStrings[2] = { "Files", "Settings" };
menu_t * topMenus[2] = { &filesMenu, &settingsMenu };

char *filesStrings[9] = { "File 1", "File 2 has a long file name that doesn't fit on the screen", "Back", "File 1-1", "File 1-2", "Back 1", "File 2-1", "File 2-2", "Back 2" };
menu_t * filesMenus[9] = { &testMenu, &filesMenu, &topMenu, &testMenu, &filesMenu, &topMenu, &testMenu, &filesMenu, &topMenu };

char *settingsStrings[3] = { "Settings 1", "Settings 2", "Back" };
menu_t * settingsMenus[3] = { &settingsMenu, &settingsMenu, &topMenu };

char *testStrings[3] = { "Files *", "Settings *", "Back *" };
menu_t * testMenus[3] = { &filesMenu, &settingsMenu, &topMenu };


void setupMenus() {
	topMenu.setStrings(topStrings);
	topMenu.setMenus(topMenus);
	topMenu.setNEntries(2);

	filesMenu.setStrings(filesStrings);
	filesMenu.setMenus(filesMenus);
	filesMenu.setNEntries(9);

	settingsMenu.setStrings(settingsStrings);
	settingsMenu.setMenus(settingsMenus);
	settingsMenu.setNEntries(3);

	testMenu.setStrings(testStrings);
	testMenu.setMenus(testMenus);
	testMenu.setNEntries(3);
}