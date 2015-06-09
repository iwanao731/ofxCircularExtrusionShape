//
//  ofxCircularExtrusionShape.h
//  CirclularExtrusionShape_class
//
//  Created by NAOYAIWAMOTO on 2015/06/10.
//
//

#ifndef __CirclularExtrusionShape_class__ofxCircularExtrusionShape__
#define __CirclularExtrusionShape_class__ofxCircularExtrusionShape__

#include "ofMain.h"
#include <stdio.h>

#endif /* defined(__CirclularExtrusionShape_class__ofxCircularExtrusionShape__) */

struct SpinePoint {
    struct orientation {
        ofVec3f up;
        ofVec3f forward;
        ofVec3f side;
    } orientation;
    
    ofFloatColor color;
    ofVec3f loc;
};

class ofxCircularExtrusionShape
{
private:
    int     curveResolution;
    int     shapeResulution;

    ofPolyline      polyline;
    vector<SpinePoint> spines;
    ofVboMesh spineMeshes;

    void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c);
    void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d);
    void calculatePTF( vector<SpinePoint>& spine );
    void createVboFromSpine( ofVboMesh& vboMesh, const vector<SpinePoint>& spine, float width=1.0f, float thickness=1.0f );

public:
    ofxCircularExtrusionShape();
    ~ofxCircularExtrusionShape();
    void setCurveResolution(const int resolution) {curveResolution = resolution;}
    void setShapeResolution(const int resolution) {shapeResulution = resolution;}
    
    void init();
    void update();
    void draw();
    void drawSpineAxes( const vector<SpinePoint>& spine );
    void addVertex(const ofVec3f p);
};
