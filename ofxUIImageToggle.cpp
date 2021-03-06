/**********************************************************************************
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#include "ofxUIImageToggle.h"
#include "ofxUI.h"

ofxUIImageToggle::ofxUIImageToggle(float x, float y, float w, float h, bool _value, string _pathURL, string _name, int _size) : ofxUIToggle()
{
    useReference = false;
    rect = new ofxUIRectangle(x,y,w,h);
    init(w, h, &_value, _pathURL, _name, _size);
}

ofxUIImageToggle::ofxUIImageToggle(float w, float h, bool _value, string _pathURL, string _name, int _size) : ofxUIToggle()
{
    useReference = false;
    rect = new ofxUIRectangle(0,0,w,h);
    init(w, h, &_value, _pathURL, _name, _size);
}

ofxUIImageToggle::ofxUIImageToggle(float x, float y, float w, float h, bool *_value, string _pathURL, string _name, int _size) : ofxUIToggle()
{
    useReference = true;
    rect = new ofxUIRectangle(x,y,w,h);
    init(w, h, _value, _pathURL, _name, _size);
}

ofxUIImageToggle::ofxUIImageToggle(float w, float h, bool *_value, string _pathURL, string _name, int _size) : ofxUIToggle()
{
    useReference = true;
    rect = new ofxUIRectangle(0,0,w,h);
    init(w, h, _value, _pathURL, _name, _size);
}

void ofxUIImageToggle::init(float w, float h, bool *_value, string _pathURL, string _name, int _size)
{
    name = string(_name);
    kind = OFX_UI_WIDGET_IMAGETOGGLE;
    
    paddedRect = new ofxUIRectangle(-padding, -padding, w+padding*2.0, h+padding*2.0);
    paddedRect->setParent(rect);
    
    label = new ofxUILabel(w+padding,0, (name+" LABEL"), name, _size);
    label->setParent(label);
    label->setRectParent(rect);
    label->setEmbedded(true);
    drawLabel = false;
    label->setVisible(drawLabel);
    
    if(useReference)
    {
        value = _value;
    }
    else
    {
        value = new bool();
        *value = *_value;
    }
    
    setValue(*_value);
    
    img = new ofImage();
    img->loadImage(_pathURL);
    bChangedImage = false;
}

ofxUIImageToggle::~ofxUIImageToggle()
{
    if(!bChangedImage)
    {
        delete img;
    }
}

void ofxUIImageToggle::drawBack()
{
    if(draw_back && !draw_fill)
    {
        ofxUIFill();
        ofxUISetColor(color_back);
        img->draw(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
    }
}

void ofxUIImageToggle::drawFill()
{
    if(draw_fill)
    {
        ofxUIFill();
        ofxUISetColor(color_fill);
        img->draw(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
    }
}

void ofxUIImageToggle::drawFillHighlight()
{
    if(draw_fill_highlight)
    {
        ofxUIFill();
        ofxUISetColor(color_fill_highlight);
        img->draw(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
    }
}

void ofxUIImageToggle::drawOutlineHighlight()
{
    if(draw_outline_highlight)
    {
        ofNoFill();
        ofxUISetColor(color_outline_highlight);
        img->draw(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
    }
}

ofImage *ofxUIImageToggle::getImage()
{
    return img;
}

void ofxUIImageToggle::setImage(ofImage *_img)
{
    if(img != NULL && !bChangedImage)
    {
        delete img;
        img = NULL;
    }
    img = _img;
    bChangedImage = true;
}