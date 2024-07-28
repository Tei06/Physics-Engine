#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>

class MainPanel; // Forward declaration

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
};

class MainPanel : public wxPanel
{
public:
    MainPanel(wxFrame* parent);

    void OnPaint(wxPaintEvent& event);
    void UpdatePhysics();
    void OnTimer(wxTimerEvent& event);
    void OnKeyDown(wxKeyEvent& event); // Handle key events

private:
    wxTimer timer;
    bool spacePressed; // Flag to track spacebar state

    float ballRadius;
    wxPoint ballPosition;
    wxPoint ballVelocity;
    float springConstant;
    float damping;
    float gravity;
    float restLength;

    wxDECLARE_EVENT_TABLE();
};

// Utility functions
template <typename T>
T clamp(T value, T min, T max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

wxPoint AddPoints(const wxPoint& lhs, const wxPoint& rhs);
wxPoint SubtractPoints(const wxPoint& lhs, const wxPoint& rhs);
wxPoint MultiplyPoint(const wxPoint& point, float scalar);
wxPoint& AddToPoint(wxPoint& lhs, const wxPoint& rhs);
wxPoint& MultiplyToPoint(wxPoint& point, float scalar);

#endif // MAINFRAME_H
