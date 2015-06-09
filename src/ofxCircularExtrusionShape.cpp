//
//  ofxCircularExtrusionShape.cpp
//  CirclularExtrusionShape_class
//
//  Created by NAOYAIWAMOTO on 2015/06/10.
//
//

#include "ofxCircularExtrusionShape.h"

ofxCircularExtrusionShape::ofxCircularExtrusionShape()
{
    
}

ofxCircularExtrusionShape::~ofxCircularExtrusionShape()
{
    
}

void ofxCircularExtrusionShape::init()
{
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    spineMeshes.clear();
    spines.push_back( SpinePoint() );
    
    polyline.clear();
    spines.clear();
}

void ofxCircularExtrusionShape::update()
{
    calculatePTF( spines );
    createVboFromSpine( spineMeshes, spines, 4.0f, 1.0f );
}

void ofxCircularExtrusionShape::draw()
{
    spineMeshes.drawFaces();
    
    ofSetColor(255);
    spineMeshes.drawWireframe();

}

void ofxCircularExtrusionShape::addVertex(const ofVec3f p)
{
    polyline.curveTo( p, curveResolution );
    
    vector<ofPoint> verts = polyline.getVertices();
    ofVec3f lastVert;
    int index = 0;
    
    spines.clear();
    
    for ( int j=0; j<verts.size(); ++j ) {
        if ( lastVert.distanceSquared( verts[j] ) > 0.1f ) {
            spines.push_back( SpinePoint() );
            spines[index].loc = verts[j];
            lastVert = verts[j];
            index++;
        }
    }
}

void ofxCircularExtrusionShape::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c)
{
    mesh.addVertex(a);
    mesh.addVertex(b);
    mesh.addVertex(c);
    
    ofVec3f p0 = (a-b).getNormalized();
    ofVec3f p1 = (b-c).getNormalized();
    
    ofVec3f n = p0.getCrossed(p1).getNormalized();
    mesh.addNormal(n);
    mesh.addNormal(n);
    mesh.addNormal(n);

    // set color (in the future)
//    ofFloatColor col = ofFloatColor(ofRandomuf(), ofRandomuf(), ofRandomuf(), 0.2f);
//    mesh.addColor(col);
//    mesh.addColor(col);
//    mesh.addColor(col);
}

//--------------------------------------------------------------
void ofxCircularExtrusionShape::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d)
{
    addFace(mesh, a, b, c);
    addFace(mesh, a, c, d);
}


//--------------------------------------------------------------
void ofxCircularExtrusionShape::drawSpineAxes( const vector<SpinePoint>& spine )
{
    for ( int i=0; i<spine.size()-1; i++ ) {
        
        // draw axis
        glBegin(GL_LINES);
        
        // rotated up
        glColor3f(0,1,0);
        glVertex3fv( spine[i].loc.getPtr() );
        glVertex3fv( (spine[i].loc + spine[i].orientation.up*5.0f).getPtr() );
        
        glColor3f(1,0,1);
        glVertex3fv( spine[i].loc.getPtr() );
        glVertex3fv( (spine[i].loc + spine[i].orientation.forward*5.0f).getPtr() );
        
        // corrected right
        glColor3f(1,0,0);
        glVertex3fv( spine[i].loc.getPtr() );
        glVertex3fv( (spine[i].loc + spine[i].orientation.side*5.0f).getPtr() );
        
        // connect points
        glColor3f(0.6,0.6,0.6);
        glVertex3fv( spine[i].loc.getPtr() );
        glVertex3fv( (spine[i].loc + spine[i+1].loc-spine[i].loc).getPtr() );
        
        glEnd();
        
//        int num_segments = 10;
        
        glBegin(GL_LINE_LOOP);
        glColor3f(1.0, 0.0, 1.0);
        glVertex3fv( ( spine[i].loc + spine[i].orientation.side * 5.0f + spine[i].orientation.up * 5.0f ).getPtr() );
        
        glColor3f(1.0,0.0,0.0);
        glVertex3fv( ( spine[i].loc + spine[i].orientation.side * 5.0f - spine[i].orientation.up * 5.0f ).getPtr() );
        
        glColor3f(0.0, 1.0, 0.0);
        glVertex3fv( ( spine[i].loc - spine[i].orientation.side * 5.0f - spine[i].orientation.up * 5.0f ).getPtr() );
        
        glColor3f(0.0, 0.0, 1.0);
        glVertex3fv( ( spine[i].loc - spine[i].orientation.side * 5.0f + spine[i].orientation.up * 5.0f ).getPtr() );
        glEnd();
    }
}


void ofxCircularExtrusionShape::calculatePTF( vector<SpinePoint>& spine )
{
    ofVec3f dirNormal;
    ofVec3f prevSide, prevForward, prevUp;
    
    // Initial point in spine
    dirNormal = (spine[1].loc - spine[0].loc).getNormalized();
    
    ofVec3f temp_up = ofVec3f(0.0f, 1.0f, 0.0f);
    
    ofVec3f new_forward = dirNormal;
    ofVec3f new_side = new_forward.crossed(temp_up).getNormalized();
    ofVec3f new_up = new_forward.crossed(new_side).getNormalized();
    
    spine[0].orientation.up = new_up;
    spine[0].orientation.forward = new_forward;
    spine[0].orientation.side = new_side;
    
    prevSide = new_side;
    prevForward = new_forward;
    prevUp = new_up;
    
    spine[0].color = ofFloatColor( 1.0f, 0.0f, 0.0f, 1.0f );
    
    for ( int i=1; i<spine.size()-1; ++i ) {
        ofVec3f T1(spine[i].loc - spine[i-1].loc);
        ofVec3f T2(spine[i+1].loc - spine[i].loc);
        T1.normalize();
        T2.normalize();
        
        ofVec3f A = T1.crossed(T2);
        
        if ( A.length() <= 0.001f ) { // is angle too small? cross product result of zero means lines are parallel
            new_up = prevUp;
            new_forward = prevForward;
            new_side = prevSide;
        } else {
            float alpha = T1.angleRad(T2);
            
            new_forward.rotateRad( alpha, A );
            new_side.rotateRad( alpha, A );
            new_up.rotateRad( alpha, A );
        }
        
        spine[i].orientation.up = new_up;
        spine[i].orientation.forward = new_forward;
        spine[i].orientation.side = new_side;
        
        prevSide = new_side;
        prevForward = new_forward;
        prevUp = new_up;
    }
}

void ofxCircularExtrusionShape::createVboFromSpine( ofVboMesh& vboMesh, const vector<SpinePoint>& spine, float width, float thickness ) {
    vboMesh.clear();
    
    int num_segments = shapeResulution;
    
    vector<ofVec3f> verts;
    
    float radius = 6.0f;
    
    for ( int a=0; a<spine.size(); a++ ) {
        ofVec3f side = spine[a].orientation.up;
        float angle_to_rotate = TWO_PI/(float)num_segments;
        
        ofVec3f axis = spine[a].orientation.forward;
        
        for( int i = 0; i < num_segments; ++i ) {
            side.rotateRad( angle_to_rotate, axis );
            verts.push_back( spine[a].loc + side * radius );
        }
    }
    
    vector<int> indices;
    
    for ( int i=1; i<spine.size()-2; i++ ) {
        int index = i*num_segments;
        
        for ( int j=0; j<num_segments-1; j++ ) {
            ofVec3f a = verts[index + j];
            ofVec3f b = verts[index + j + 1];
            ofVec3f c = verts[index + j + num_segments + 1];
            ofVec3f d = verts[index + j + num_segments];
            
            addFace( vboMesh, a, b, c, d );
        }
        
        // close end to beginning
        ofVec3f a = verts[index + num_segments-1];
        ofVec3f b = verts[index + 0];
        ofVec3f c = verts[index + num_segments];
        ofVec3f d = verts[index + num_segments + num_segments-1];
        
        addFace( vboMesh, a, b, c, d );
    }
}

