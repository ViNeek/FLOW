//
//  fwScreen.h
//  FLOW
//
//  Created by Nick Vitsas on 9/13/13.
//  Copyright (c) 2013 Nick Vitsas. All rights reserved.
//

#ifndef __SCREEN_HEADER_H__
#define __SCREEN_HEADER_H__

#include "../fwUtilities.h"

class InputHandler;

class FLOWScreen : public wxPanel
{
    wxImage *m_BackBuffer;
    wxImage *m_FrontBuffer;
    BYTE    *m_BackData; 
    BYTE    *m_FrontData; 
public:
    FLOWScreen(wxFrame* parent);
    ~FLOWScreen();

    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    
    void render(wxDC& dc);
    
    void SwapImage(AVFrame* fr, InputHandler* inh);
    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
    
    DECLARE_EVENT_TABLE()
};

#endif /* __SCREEN_HEADER_H__) */
