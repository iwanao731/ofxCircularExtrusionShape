#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    cir.init();
    cir.setCurveResolution(10);
    cir.setShapeResolution(32);
    
    // modeling
    noiseAmount = 10.0f;
    noise1 = 0.01f;
    for ( z=0; z<30; z += 10.0f ) {
        float val = noiseAmount * ofSignedNoise( noise1 * 2.0f, z );
        cir.addVertex(ofVec3f( 0.0, val, z ));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // modeling
    z+=10.0f;
    float val = noiseAmount * ofSignedNoise( noise1 * 2.0f, z );
    
    // add vertex and update the shape
    cir.addVertex(ofVec3f( 0.0, val, z ));
    cir.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofClear( 255, 255, 255 );
    
    ofBackground(0);
    ofSetColor(255, 0, 0);
    
    ofDisableAlphaBlending();
    glDisable( GL_CULL_FACE );
    
    cam.begin();
    cir.draw();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
