/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-11 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#ifndef __JUCE_LookAndFeelCustom_JUCEHEADER__
#define __JUCE_LookAndFeelCustom_JUCEHEADER__

#include <JuceHeader.h>


//==============================================================================
/**
    The original Juce look-and-feel.

*/
class /*JUCE_API*/  LookAndFeelCustom    : public juce::LookAndFeel_V4
{
public:
    //==============================================================================
    /** Creates the default JUCE look and feel. */
    LookAndFeelCustom();

    /** Destructor. */
    virtual ~LookAndFeelCustom();

    virtual void drawRotarySlider    (    juce::Graphics &     g,
                                     int     x,
                                     int     y,
                                     int     width,
                                     int     height,
                                     float     sliderPosProportional,
                                     float     rotaryStartAngle,
                                     float     rotaryEndAngle,
                                      juce::Slider &     slider );




    //==============================================================================
    // All of the stuff from here down has just been copied from the oldLookAndFeel
    // class and hacked a bit to toy with it.
    //==============================================================================


    //==============================================================================
    /** Draws the lozenge-shaped background for a standard button. */
    virtual void drawButtonBackground (juce::Graphics& g,
                                       juce::Button& button,
                                       const juce::Colour& backgroundColour,
                                       bool isMouseOverButton,
                                       bool isButtonDown);


    /** Draws the contents of a standard ToggleButton. */
    virtual void drawToggleButton (juce::Graphics& g,
                                   juce::ToggleButton& button,
                                   bool isMouseOverButton,
                                   bool isButtonDown);

    virtual void drawTickBox (juce::Graphics& g,
                              juce::Component& component,
                              float x, float y, float w, float h,
                              bool ticked,
                              bool isEnabled,
                              bool isMouseOverButton,
                              bool isButtonDown);

    //==============================================================================
  

    //==============================================================================
    virtual void drawScrollbarButton (juce::Graphics& g,
                                      juce::ScrollBar& scrollbar,
                                      int width, int height,
                                      int buttonDirection,
                                      bool isScrollbarVertical,
                                      bool isMouseOverButton,
                                      bool isButtonDown);

    virtual void drawScrollbar (juce::Graphics& g,
                                juce::ScrollBar& scrollbar,
                                int x, int y,
                                int width, int height,
                                bool isScrollbarVertical,
                                int thumbStartPosition,
                                int thumbSize,
                                bool isMouseOver,
                                bool isMouseDown);

    virtual juce::ImageEffectFilter* getScrollbarEffect();

    //==============================================================================
    virtual void drawTextEditorOutline (juce::Graphics& g,
                                        int width, int height,
                                        juce::TextEditor& textEditor);

    //==============================================================================
    /** Fills the background of a popup menu component. */
    virtual void drawPopupMenuBackground (juce::Graphics& g, int width, int height);

    virtual void drawMenuBarBackground (juce::Graphics& g, int width, int height,
                                        bool isMouseOverBar,
                                        juce::MenuBarComponent& menuBar);

    //==============================================================================
    virtual void drawComboBox (juce::Graphics& g, int width, int height,
                               bool isButtonDown,
                               int buttonX, int buttonY,
                               int buttonW, int buttonH,
                               juce::ComboBox& box);

    //virtual const Font getComboBoxFont (ComboBox& box);

    //==============================================================================
    virtual void drawLinearSlider (juce::Graphics& g,
                                   int x, int y,
                                   int width, int height,
                                   float sliderPos,
                                   float minSliderPos,
                                   float maxSliderPos,
                                   const juce::Slider::SliderStyle style,
                                   juce::Slider& slider);

    virtual int getSliderThumbRadius (juce::Slider& slider);

    virtual juce::Button* createSliderButton (bool isIncrement);

    virtual juce::ImageEffectFilter* getSliderEffect();

    //==============================================================================
    virtual void drawCornerResizer (juce::Graphics& g,
                                    int w, int h,
                                    bool isMouseOver,
                                    bool isMouseDragging);

    virtual juce::Button* createDocumentWindowButton (int buttonType);

    virtual void positionDocumentWindowButtons (juce::DocumentWindow& window,
                                                int titleBarX, int titleBarY,
                                                int titleBarW, int titleBarH,
                                                juce::Button* minimiseButton,
                                                juce::Button* maximiseButton,
                                                juce:: Button* closeButton,
                                                bool positionTitleBarButtonsOnLeft);


private:
    //==============================================================================
    juce::DropShadowEffect scrollbarShadow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeelCustom);
};


#endif   // __JUCE_LookAndFeelCustom_JUCEHEADER__
