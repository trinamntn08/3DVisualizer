#pragma once
#include"Application.h"

extern Application* CreateApplication();

int main()
{
	std::unique_ptr<Application> app(CreateApplication());
	if (app)
	{
		app->Run();
	}


	return 0;
}