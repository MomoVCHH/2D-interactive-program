#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofDisableAntiAliasing();
    ofBackgroundHex(0);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetVerticalSync(true);
    
    //  the setting of Box2d addon
    box2d.init();
    box2d.setGravity(0, 20);
    box2d.createGround();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    box2d.enableEvents();
    
    
    //  load background music
    myDancer.load("Dancer.wav");
    myDancer.setVolume(0.4f);
    myDancer.setLoop(true);
    myDancer.play();
    myDancer.setLoop(true);
    
    //  load the sound files
    for (int i=0; i<N_SOUNDS; i++) {
        sound[i].load("sounds/"+ofToString(i)+".mp3");
        sound[i].setMultiPlay(true);
        sound[i].setLoop(false);
        sound[i].setVolume(0.2f);
    }
    
    //  the listener for getting events
    ofAddListener(box2d.contactStartEvents, this, &ofApp::contactStart);
    ofAddListener(box2d.contactEndEvents, this, &ofApp::contactEnd);
    
    //  the setting of yellow lines
    vector <string> strLines;
    for (int i=0; i<strLines.size(); i++) {
        vector <string> pts = ofSplitString(strLines[i], ",");
        if(pts.size() > 0) {
            shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
            for (int j=0; j<pts.size(); j+=2) {
                if(pts[j].size() > 0) {
                    float x = ofToFloat(pts[j]);
                    float y = ofToFloat(pts[j+1]);
                    edge.get()->addVertex(x, y);
                }
            }
            edge.get()->create(box2d.getWorld());
            edges.push_back(edge);
        }
    }
}

//--------------------------------------------------------------
void ofApp::contactStart(ofxBox2dContactArgs &e) {
    
    if(e.a != NULL && e.b != NULL) {
        
    //  because of this setting we get no sounds, if the circles collide the ground.
        if(e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {
            box2d.setGravity(0, 10);
            
            SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
            SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
            
            if(aData) {
                aData->bHit = true;
                sound[aData->soundID].play();
            }
            
            if(bData) {
                bData->bHit = true;
                sound[bData->soundID].play();
            }
        }
    }
    
    if(e.a != NULL && e.b != NULL) {
        
    //  because of this setting we get no sounds, if the objects collide the ground.
        if(e.a->GetType() == b2Shape::e_polygon && e.b->GetType() == b2Shape::e_polygon) {
            box2d.setGravity(0, -2);
            SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
            SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
            
            if(aData) {
                aData->bHit = true;
                sound[aData->soundID].play();
            }
            
            if(bData) {
                bData->bHit = true;
                sound[bData->soundID].play();
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::contactEnd(ofxBox2dContactArgs &e) {
    
    if(e.a != NULL && e.b != NULL) {
        
        SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
        SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
        
        if(aData) {
            aData->bHit = false;
        }
        
        if(bData) {
            bData->bHit = false;
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //  frequency : how often should the dircles come
    if((int)ofRandom(0, 70) == 0) {
        shared_ptr <ofxBox2dCircle> c = shared_ptr <ofxBox2dCircle>(new ofxBox2dCircle);
        c.get()->setPhysics(1, 0.5, 0.9);
        c.get()->setup(box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-30, 30), -20, ofRandom(30, 40));
        
        c.get()->setData(new SoundData());
        SoundData * sd = (SoundData*)c.get()->getData();
        sd->soundID = ofRandom(0, N_SOUNDS);
        sd->bHit    = false;
        circles.push_back(c);
    }
    
    box2d.update();
}

void changeColor(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //  the Setting for the line and edge
    ofSetColor(255,165,0);
    ofNoFill();
    for (int i=0; i<lines.size(); i++) {
        lines[i].draw();
    }
    for (int i=0; i<edges.size(); i++) {
        edges[i].get()->draw();
    }
    
    //  the circle setting
    for(size_t i=0; i<circles.size(); i++) {
        ofFill();
        SoundData * data = (SoundData*)circles[i].get()->getData();
        
        //  the color setting 
        if(data && data->bHit) ofSetColor(ofRandom(100,255),ofRandom(140,255),ofRandom(150,200));
      
        else
            ofSetColor(255,0,201);
        
        circles[i].get()->draw();
    }

    //  the setting of custom particles
    for(int i=0; i<customParticles.size(); i++) {
        customParticles[i].get()->draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'f') ofToggleFullscreen();
    
    //  the circles make sounds
    if(key == '1') box2d.enableEvents();
    
    //  the circles make no sounds
    if(key == '2') box2d.disableEvents();
    
    //  play the background music
    if(key == 'b')
    {
        myDancer.play();
    }
    
    //  stop the background music
    if(key == 's')
    {
        myDancer.stop();
    }
    
    //  delete all lines
    if(key == 'l') {
        lines.clear();
        edges.clear();
    }
    
    //  delete all circles
    if(key == 'k'){
        circles.clear();
    }
    
    //  create custom particles
    if(key == 'c') {
        customParticles.push_back(shared_ptr<CustomParticle>(new CustomParticle));
        CustomParticle * p = customParticles.back().get();
        float r = ofRandom(3, 10);        // a random radius 4px - 20px
        p->setPhysics(0.4, 0.53, 0.31);
        p->setup(box2d.getWorld(), mouseX, mouseY, r);
        p->color.r = ofRandom(0,200);
        p->color.g = ofRandom(0);
        p->color.b = ofRandom(150, 255);
    }
    
    //  delete all custom particles
    if(key == 'u'){
        customParticles.clear();
    }
}

//--------------------------------------------------------------
// void ofApp::keyReleased(int key){}

//--------------------------------------------------------------
//void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    //  the code for line draw
    lines.back().addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    //  the code is for line drawing
    lines.push_back(ofPolyline());
    lines.back().addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    //  the code ist for edge-line
    shared_ptr <ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
    lines.back().simplify();
    
    for (int i=0; i<lines.back().size(); i++) {
        edge.get()->addVertex(lines.back()[i]);
    }
    edge.get()->create(box2d.getWorld());
    edges.push_back(edge);
}

//--------------------------------------------------------------

//  void ofApp::mouseEntered(int x, int y){}

//--------------------------------------------------------------
//  void ofApp::mouseExited(int x, int y){}

//--------------------------------------------------------------
//  void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
//  void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
//  void ofApp::dragEvent(ofDragInfo dragInfo){}
