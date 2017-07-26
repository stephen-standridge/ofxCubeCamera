//
//  CubeCamera.hpp
//  tree
//
//  Created by kevin ambrosia on 2/14/17.
//
//

#ifndef ofxCubeCamera_h
#define ofxCubeCamera_h

#include "ofMain.h"

class ofxCubeCamera : public ofNode {
public:
    enum projectionTypes{
        DOME,
        QUINCUNCIAL,
        MERCATOR
    };
    ofxCubeCamera();
    void setup(ofxCubeCamera::projectionTypes type,
               int w = ofGetWidth(),
               int h = ofGetHeight());
    using ofNode::draw;
    void draw();
    void begin(int i);
    void end(int i);
    void resize(int w, int h);
    void setMeshScale(float s);
    void setNearClip(float _near);
    void setFarClip(float _far);
    projectionTypes currentType;
    string currentTypeString();
    vector<ofCamera> renderCamera;

    int renderCount = 0;

private:
    ofRectangle view;
    vector<ofFbo> renderFbo;
    vector<ofVboMesh> renderMesh;
    void prepareFrameBuffers();
    
    ofCamera composedCamera;
    ofRectangle composedView;
    
    ofImage mask;
    
    int width;
    int height;
    float meshScale;
    float meshScaleExt;
    float near = 0;
    float far = 0;
    
    enum positions{
        BOTTOM,
        FRONT,
        LEFT,
        RIGHT,
        TOP,
        BACK
    };
    
    
};

#endif /* ofxCubeCamera_h */
