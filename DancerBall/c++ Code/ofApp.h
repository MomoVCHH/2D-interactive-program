#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

#define N_SOUNDS 55



class SoundData {

public:
    
    int  soundID;
    bool bHit;
};


class CustomParticle : public ofxBox2dCircle{
    
public:
    
    CustomParticle() {}
    ofColor color;
    void draw() {
        
        float radius = getRadius();
        glPushMatrix();
        glTranslatef(getPosition().x, getPosition().y, 0);
        ofSetColor(color.r, color.g, color.b);
        ofFill();
        ofDrawCircle(0,0,radius);
        glPopMatrix();
    }
};


class ofApp : public ofBaseApp{

public:
    
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
//    void keyReleased(int key);
//    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
//    void mouseEntered(int x, int y);
//    void mouseExited(int x, int y);
//    void windowResized(int w, int h);
//    void dragEvent(ofDragInfo dragInfo);
//    void gotMessage(ofMessage msg);
    
    //  this is the function for contacts
    void contactStart(ofxBox2dContactArgs &e);
    void contactEnd(ofxBox2dContactArgs &e);
        
    //  when the ball hits we get the sounds
    ofSoundPlayer sound[N_SOUNDS];
    
    //  the code for line-draw
    vector <ofPolyline> lines;
    
    
    ofxBox2d    box2d;
    vector <shared_ptr<ofxBox2dCircle> > circles;
    vector <shared_ptr<ofxBox2dRect>   > rectangles;
    vector <shared_ptr<ofxBox2dEdge>   > edges;
    vector <shared_ptr<CustomParticle> > customParticles;
    
    ofSoundPlayer   myDancer;
};
