#include "Utils.h"
#include "AppLauncher.h"
#include "SystemCommands.h"
#include "XWindowInterface.h"

/* Timer interval in milliseconds. */
static const int timerFrequency = 2000;

/* Milliseconds to wait before forcibly terminating a window focus operation. */
static const int windowFocusTimeout = 1000;

/* Milliseconds to wait before assuming the last application launch 
   failed. */
static const int pendingLaunchTimeout = 15000;

/* Localized text keys: */
static const constexpr char * could_not_open = "could_not_open";
static const constexpr char * not_valid_command = "not_valid_command";

/*
 * Launches a new application, or focuses its window if the application is
 * already running
 */
void AppLauncher::startOrFocusApp(const juce::String& command)
{
    using namespace juce;
    DBG("AppLauncher::" << __func__ << ": command = " << command);
    // Before adding another process to the list, clean out any old dead ones,
    // so they don't start piling up.
    std::vector<LaunchedProcess*> toRemove;
    LaunchedProcess* appInstance = nullptr;
    for (LaunchedProcess* app : runningApps)
    {
        if (!app->isRunning())
        {
            toRemove.push_back(app);
        }
        else if(app->getLaunchCommand() == command)
        {
            appInstance = app;
        }
    }
    for (LaunchedProcess* appProcess : toRemove)
    {
        DBG("AppLauncher::" << __func__ << ": removing terminated app "
                << appProcess->getLaunchCommand());
        runningApps.removeObject(appProcess);
    }
    if (appInstance != nullptr)
    {
        if (appInstance->isRunning())
        {
            DBG("AppLauncher::" << __func__
                    << ": app is already running,"
                    << " focusing the window");
            appInstance->activateWindow();
            return;
        }
        else
        {
            if (appInstance != nullptr)
            {
                DBG("AppLauncher::" << __func__
                        << ": Old process is dead, re-launching");
            }
        }
    }
    LaunchedProcess* newApp = startApp(command);
    if(newApp != nullptr)
    {
        runningApps.add(newApp);
    }
    else
    {
        DBG("AppLauncher::" << __func__
                << ": Failed to launch " << command);
    }
}

/*
 * Checks a string to see if it is a valid shell command.
 */
bool AppLauncher::testCommand(const juce::String& command)
{
    using namespace juce;
    SystemCommands systemCommands;
    return systemCommands.runIntCommand
        (SystemCommands::IntCommand::commandCheck, command) == 0;
}


/*
 * Starts a new instance of an application process
 */
LaunchedProcess* AppLauncher::startApp(const juce::String& command)
{
    using namespace juce;
    DBG("AppsPageComponent::startApp - " << command);
    if (!testCommand(command))
    {
        AlertWindow::showMessageBoxAsync
                (AlertWindow::AlertIconType::WarningIcon,
                localeText(could_not_open),
                String("\"") + command + String("\"")
                + localeText(not_valid_command));
        launchFailureCallback();
        return nullptr;
    }
    
    LaunchedProcess* newApp = new LaunchedProcess(command);    
    timedProcess = newApp;
    lastLaunch = juce::Time::getMillisecondCounter();
    startTimer(timerFrequency);
    return newApp;
}

/*
 * Checks if the last launched application started successfully, and displays an
 * error message if the application process died.
 */
void AppLauncher::timerCallback()
{
    using namespace juce;
    if (timedProcess != nullptr)
    {
        if (timedProcess->isRunning())
        {
            // If the process is still going and we have yet to reach timeout,
            // wait longer.
            if (juce::Time::getMillisecondCounter() - lastLaunch 
                    < pendingLaunchTimeout)
            {
                return;
            }
            else
            {
                DBG("AppLauncher::" << __func__ << ": Process \""
                        << timedProcess->getLaunchCommand()
                        << "\" launch timed out");
            }
        }
        else
        {
            DBG("AppLauncher::" << __func__ << ": process died, show message");
            String output = timedProcess->getProcessOutput();
            StringArray lines = StringArray::fromLines(output);
            output = "";
            for (int i = lines.size() - 1;
                 i > lines.size() - 6 && i >= 0; i--)
            {
                output = lines[i] + String("\n") + output;
            }
            AlertWindow::showMessageBoxAsync
                    (AlertWindow::AlertIconType::WarningIcon,
                    localeText(could_not_open), output);
            timedProcess = nullptr;
        }
    }
    launchFailureCallback();
    stopTimer();

}

/*
 * Cancels pending checks on the last launched application if the pocket-home
 * window loses focus.
 */
void AppLauncher::onSuspend()
{
    stopTimer();
}
