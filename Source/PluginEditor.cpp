/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

t_camomile* CamomileAudioProcessorEditor::getCamomile(t_canvas* cnv) noexcept
{
    if(cnv)
    {
        t_symbol* name = gensym("c.camomile");
        t_gobj *y;
        for(y = cnv->gl_list; y; y = y->g_next)
        {
            if(eobj_getclassname(y) == name)
            {
                return (t_camomile *)y;
            }
        }
    }
    return nullptr;
}

CamomileAudioProcessorEditor::CamomileAudioProcessorEditor(CamomileAudioProcessor& p) :
AudioProcessorEditor(&p),
m_processor(p),
m_file_drop(false)
{
    setSize(400, 300);
    m_processor.addListener(this);
}

CamomileAudioProcessorEditor::~CamomileAudioProcessorEditor()
{
    m_processor.removeListener(this);
}

void CamomileAudioProcessorEditor::paint(Graphics& g)
{
    if(m_file_drop)
    {
        g.fillAll(Colours::lightblue);
    }
    
    if(!m_processor.hasPatch())
    {
        g.fillAll(Colours::white);
        g.setColour(Colours::black);
        g.setFont (15.0f);
        g.drawText(juce::String("Drag & Drop your patch..."), getBounds().withZeroOrigin(), juce::Justification::centred);
    }
    else
    {
        t_camomile* cm = getCamomile(m_processor.getPatch());
        if(cm)
        {
            g.fillAll(Colour::fromFloatRGBA(cm->f_color_background.red, cm->f_color_background.green, cm->f_color_background.blue, cm->f_color_background.alpha));
        }
    }
}

bool CamomileAudioProcessorEditor::isInterestedInFileDrag(const StringArray& files)
{
    if(files.size())
    {
        for(int i = 0; i < files.size(); i++)
        {
            if(files[i].endsWith(juce::StringRef(".pd")))
            {
                return true;
            }
        }
    }
    return false;
}

void CamomileAudioProcessorEditor::filesDropped(const StringArray& files, int x, int y)
{
    if(files.size())
    {
        for(int i = 0; i < files.size(); i++)
        {
            juce::File file(files[i]);
            if(file.getFileExtension() == juce::String(".pd"))
            {
                m_processor.loadPatch(file);
            }
        }
    }
}

void CamomileAudioProcessorEditor::patchChanged()
{
    if(m_processor.hasPatch())
    {
        t_camomile* cm = getCamomile(m_processor.getPatch());
        if(cm)
        {
            t_rect rect;
            pd_bang((t_pd *)cm);
            ebox_get_rect_for_view((t_ebox *)cm, &rect);
            setBounds(0, 0, (int)rect.width, (int)rect.height);
        }
    }
    else
    {
        repaint();
    }
}

void CamomileAudioProcessorEditor::fileDragEnter(const StringArray& files, int x, int y)
{
    m_file_drop = true;
    repaint();
}

void CamomileAudioProcessorEditor::fileDragExit(const StringArray& files)
{
    m_file_drop = false;
    repaint();
}

void CamomileAudioProcessorEditor::resized()
{
    
}
