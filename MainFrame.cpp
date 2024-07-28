#include "MainFrame.h"
#include <cmath>
#include <wx/log.h>

wxBEGIN_EVENT_TABLE(MainPanel, wxPanel)
EVT_PAINT(MainPanel::OnPaint)
EVT_TIMER(wxID_ANY, MainPanel::OnTimer)
EVT_KEY_DOWN(MainPanel::OnKeyDown) // Bind key down event
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title)
{
    SetClientSize(800, 600);
    new MainPanel(this); // Create and add the drawing panel
}

MainPanel::MainPanel(wxFrame* parent)
    : wxPanel(parent), timer(this, wxID_ANY), spacePressed(false)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    // Initial physics parameters
    ballRadius = 20.0f;
    ballPosition = wxPoint(400, 300);
    ballVelocity = wxPoint(0, 0);
    springConstant = 0.1f;
    damping = 0.98f;
    gravity = 0.5f;
    restLength = 200.0f;

    // Start the timer for the simulation
    timer.Start(16); // Approximately 60 FPS

    wxLogMessage("MainPanel initialized and timer started.");
}

void MainPanel::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    // Clear the background
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    // Draw the spring
    wxPoint anchor(GetClientSize().x / 2, 0); // Fixed point at the top center
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawLine(anchor, ballPosition);

    // Draw the ball
    dc.SetBrush(*wxBLUE_BRUSH);
    dc.DrawCircle(ballPosition, ballRadius);

    wxLogMessage("OnPaint called: Ball and spring drawn.");
}

void MainPanel::OnTimer(wxTimerEvent& event)
{
    UpdatePhysics();
    Refresh();

    wxLogMessage("OnTimer called: Physics updated.");
}

void MainPanel::OnKeyDown(wxKeyEvent& event)
{
    if (event.GetKeyCode() == WXK_SPACE)
    {
        // Set the upward velocity when spacebar is pressed
        ballVelocity.y = -15; // Adjust the value to control the bounce strength
        spacePressed = true;
        wxLogMessage("Spacebar pressed: Ball velocity set to (%d, %d).", ballVelocity.x, ballVelocity.y);
    }
    event.Skip();
}

wxPoint AddPoints(const wxPoint& lhs, const wxPoint& rhs)
{
    return wxPoint(lhs.x + rhs.x, lhs.y + rhs.y);
}

wxPoint SubtractPoints(const wxPoint& lhs, const wxPoint& rhs)
{
    return wxPoint(lhs.x - rhs.x, lhs.y - rhs.y);
}

wxPoint MultiplyPoint(const wxPoint& point, float scalar)
{
    return wxPoint(static_cast<int>(point.x * scalar), static_cast<int>(point.y * scalar));
}

wxPoint& AddToPoint(wxPoint& lhs, const wxPoint& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

wxPoint& MultiplyToPoint(wxPoint& point, float scalar)
{
    point.x = static_cast<int>(point.x * scalar);
    point.y = static_cast<int>(point.y * scalar);
    return point;
}

void MainPanel::UpdatePhysics()
{
    // Calculate spring force
    wxPoint anchor(GetClientSize().x / 2, 0);
    wxPoint springVector = SubtractPoints(ballPosition, anchor);
    float springLength = std::sqrt(static_cast<float>(springVector.x * springVector.x + springVector.y * springVector.y));
    float springForceMagnitude = springConstant * (springLength - restLength);
    wxPoint springForce = MultiplyPoint(springVector, (springForceMagnitude / springLength));

    // Calculate gravity force
    wxPoint gravityForce(0, static_cast<int>(gravity));

    // Calculate total force
    wxPoint totalForce = AddPoints(springForce, gravityForce);

    // Apply additional force if spacebar is pressed
    if (spacePressed)
    {
        totalForce.y += 10; // Apply an upward force
        spacePressed = false; // Reset the flag after applying the force
    }

    // Update velocity
    AddToPoint(ballVelocity, totalForce);
    MultiplyToPoint(ballVelocity, damping); // Apply damping

    // Update position
    AddToPoint(ballPosition, ballVelocity);

    // Collision detection with boundaries
    int width, height;
    GetClientSize(&width, &height);

    if (ballPosition.x - ballRadius < 0 || ballPosition.x + ballRadius > width)
    {
        ballVelocity.x = -ballVelocity.x;
        ballPosition.x = clamp(ballPosition.x, static_cast<int>(ballRadius), width - static_cast<int>(ballRadius));
    }

    if (ballPosition.y - ballRadius < 0 || ballPosition.y + ballRadius > height)
    {
        ballVelocity.y = -ballVelocity.y;
        ballPosition.y = clamp(ballPosition.y, static_cast<int>(ballRadius), height - static_cast<int>(ballRadius));
    }

    wxLogMessage("UpdatePhysics called: Ball position updated to (%d, %d).", ballPosition.x, ballPosition.y);
}
