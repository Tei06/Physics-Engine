#include "App.h"
#include "MainFrame.h"

bool App::OnInit()
{
    MainFrame* mainFrame = new MainFrame("Physics Engine");
    mainFrame->Show(true);
    return true;
}
