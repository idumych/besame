/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 *
 * BeSame: remove all the balls from the board by clicking groups of two or
 * more of the same colour. A native Haiku port of BeSame 1.0 for BeOS by
 * Michael Wulff Nielsen.
 */
#include <Alert.h>
#include <Application.h>
#include <Catalog.h>

#include "Constants.h"
#include "MainWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "App"


class App : public BApplication {
public:
	App()
		:
		BApplication(APP_SIGNATURE)
	{
	}

	virtual void ReadyToRun()
	{
		(new MainWindow())->Show();
	}

	virtual void AboutRequested()
	{
		BAlert* alert = new BAlert(B_TRANSLATE("About BeSame"),
			B_TRANSLATE("BeSame 1.1.1\n\n"
				"BeSame 1.0 for BeOS by Michael Wulff Nielsen "
				"(Naish@get2net.dk).\n"
				"www.wulffnet.dk/Beos\n\n"
				"Graphics by Michael Wulff Nielsen and Jeppe Isaaksen.\n\n"
				"Native Haiku port, 2026."),
			B_TRANSLATE("OK"));
		alert->SetFlags(alert->Flags() | B_CLOSE_ON_ESCAPE);
		alert->Go(NULL);
	}
};


int
main()
{
	App app;
	app.Run();
	return 0;
}
