﻿
// Default
#include <Windows.h>
#include <tchar.h>

// STL
#include <memory>

// Terminal
#include "Terminal.h"

// Using
using Terminal::COLOR;

int _tmain(int nArguments, PTCHAR* pArguments) {

	Terminal::Window Window;
	if (Window.Open(true)) {
		Terminal::Screen Screen(&Window, true);
		Terminal::Console Console(&Screen);

		if (nArguments > 1) {
			for (int i = 0; i < nArguments; ++i) {
				PTCHAR pArgument = pArguments[i];
				if (_tcscmp(pArgument, _T("/server")) == 0) {
					Terminal::Server Server(&Screen);

					TCHAR szSessionName[64];
					if (!Server.GetSessionName(szSessionName)) {
						return -1;
					}

					_tprintf_s(_T("Server Session: %s\n"), szSessionName);

					if (!Server.Open()) {
						return -1;
					}

					Console.tprintf(COLOR::COLOR_GREEN, _T("CONNECTED!\n"));

					auto MessagePtr = std::make_unique<Terminal::TerminalMessage>();
					auto pMessage = MessagePtr.get();

					while (true) {
						memset(pMessage, 0, sizeof(Terminal::TerminalMessage));

						if (!Server.Receive(pMessage)) {
							break;
						}

						if (!Server.Process(pMessage)) {
							break;
						}
					}

					Server.Close();

					Console.tprintf(COLOR::COLOR_RED, _T("DISCONNECTED!\n"));

					return 0;
				}

				if ((nArguments > 2) && (_tcscmp(pArgument, _T("/client")) == 0)) {
					PTCHAR pSessionName = pArguments[i + 1];
					if (_tcslen(pSessionName) != 41) {
						return -1;
					}

					_tprintf_s(_T("Connecting to `%s`\n"), pSessionName);

					Terminal::Client Client;
					if (!Client.Open(pSessionName)) {
						return -1;
					}

					Console.tprintf(COLOR::COLOR_GREEN, _T("CONNECTED!\n"));

					Client.tprintf({ COLOR::COLOR_CYAN, COLOR::COLOR_BLUE }, _T("Hello, World!\n"));

					int a = 0;
					Client.tscanf(COLOR::COLOR_YELLOW, _T("%i"), &a);
					Console.tprintf(COLOR::COLOR_GREEN, _T("a = %i\n"), a);

					if (Client.Close()) {
						return -1;
					}

					Console.tprintf(COLOR::COLOR_RED, _T("DISCONNECTED!\n"));

					return 0;
				}
			}
		} else {
			Console.tprintf({ COLOR::COLOR_CYAN, COLOR::COLOR_BLUE }, _T("Hello, World!\n"));

			int a = 0;
			Console.tscanf(COLOR::COLOR_YELLOW, _T("%i"), &a);
		}
	}

	return 0;
}