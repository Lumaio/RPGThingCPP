#include <iostream>
#include <conio.h>
#include "Game.h"

using namespace std;

int main()
{
	Game Game("spooky scary skeletons sending shivers down 'yer spine");
	Game.Init();
	Game.Start();

	cout << "You Probably Died..." << endl;
	System::Console::Clear();
	System::Console::ForegroundColor = System::ConsoleColor::DarkRed;
	System::Console::WriteLine("You Probably Died...");
	System::Console::ReadKey();

	return 0;
}
