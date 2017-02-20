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

class ofxCubeCamera{
public:
    enum projectionTypes{
        DOME,
        QUINCUNCIAL
    };
    ofxCubeCamera();
    void setup(ofxCubeCamera::projectionTypes type,
               int w = ofGetWidth(),
               int h = ofGetHeight());
    void draw();
    void begin(int i);
    void end(int i);
    void resize(int w, int h);
    void setCameraPosition(float x, float y, float z);
    void setMeshScale(float s);
    
    int renderCount = 0;
    
private:
    ofRectangle view;
    vector<ofCamera> renderCamera;
    vector<ofFbo> renderFbo;
    vector<ofVboMesh> renderMesh;
    void prepareFrameBuffers();
    
    ofCamera distortedCamera;
    ofRectangle distortedView;
    
    ofImage mask;
    
    int width;
    int height;
    float meshScale;
    float meshScaleExt;
    
    ofNode cameraNode;
    
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
