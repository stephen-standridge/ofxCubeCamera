//
//  ofxCubeCamera.cpp
//  tree
//
//  Created by kevin ambrosia on 2/14/17.
//
//

#include "ofxCubeCamera.h"

ofxCubeCamera::ofxCubeCamera(){
    meshScaleExt = 1.0;
}

void ofxCubeCamera::setup(ofxCubeCamera::projectionTypes type, int w, int h){
    width = w;
    height = h;
    
    
    switch(type){
        case(DOME):
            renderCount = 5;
            prepareFrameBuffers();
            renderMesh[BOTTOM].load("domemaster/bottom.ply");
            renderMesh[FRONT].load("domemaster/front.ply");
            renderMesh[LEFT].load("domemaster/left.ply");
            renderMesh[RIGHT].load("domemaster/right.ply");
            renderMesh[TOP].load("domemaster/top.ply");
            
            mask.load("domemaster/mask.png");
            break;
        case(QUINCUNCIAL):
            renderCount = 6;
            prepareFrameBuffers();
            renderMesh[BOTTOM].load("quincuncial/top.ply");
            renderMesh[FRONT].load("quincuncial/front.ply");
            renderMesh[LEFT].load("quincuncial/left.ply");
            renderMesh[RIGHT].load("quincuncial/right.ply");
            renderMesh[TOP].load("quincuncial/bottom.ply");
            renderMesh[BACK].load("quincuncial/back.ply");
            
            //create quincuncial meshes and load
            mask.load("quincuncial/mask.png");
            break;
    }
    mask.setUseTexture(true);
    resize(width, height);
    
}

void ofxCubeCamera::prepareFrameBuffers(){
    for(int i=0; i<renderCount; i++){
        ofCamera c;
        c.setNearClip(.01);
        renderCamera.push_back(c);
        ofFbo f;
        renderFbo.push_back(f);
        ofVboMesh m;
        renderMesh.push_back(m);
    }
}

void ofxCubeCamera::begin(int i){
    renderFbo[i].begin();
    ofClear(0);
    renderCamera[i].begin(view);
}

void ofxCubeCamera::end(int i){
    renderFbo[i].end();
    renderCamera[i].end();
}

void ofxCubeCamera::draw(){
    distortedCamera.enableOrtho();
    distortedCamera.begin(distortedView);
    
    ofEnableNormalizedTexCoords();
    
    for (int i=0; i<renderCount; i++){
        renderFbo[i].getTexture().bind();
        ofSetColor(255,255,255);
        
        ofPushMatrix();
        ofTranslate(-width/2, -height/2, 0);
        ofRotate(90,1,0,0);
        ofRotate(180,0,0,1);
        ofRotate(180,0,0,0);
        ofScale(meshScale, meshScale, meshScale);
        renderMesh[i].drawFaces();
        ofPopMatrix();
        renderFbo[i].getTexture().unbind();
    }
    
    ofDisableNormalizedTexCoords();
    
    distortedCamera.end();
    distortedCamera.draw();
    
    mask.draw(0,0,width,height);
}

void ofxCubeCamera::resize(int w, int h){
    
    width = w;
    height = h;
    
    view.setWidth(width);
    view.setHeight(height);
    
    renderCamera[BOTTOM].setOrientation(ofVec3f(-90,0,0));
    renderCamera[BOTTOM].setFov(90);
    
    renderCamera[FRONT].setOrientation(ofVec3f(0,0,0));
    renderCamera[FRONT].setFov(90);
    
    renderCamera[LEFT].setOrientation(ofVec3f(0,90,0));
    renderCamera[LEFT].setFov(90);
    
    renderCamera[RIGHT].setOrientation(ofVec3f(0, -90, 0));
    renderCamera[RIGHT].setFov(90);
    
    renderCamera[TOP].setOrientation(ofVec3f(90,0,0));
    renderCamera[TOP].setFov(90);
    
    if (renderCount == 6){
        renderCamera[BACK].setOrientation(ofVec3f(0,180,180));
        renderCamera[BACK].setFov(90);
    }
    
    for(int i =0; i<renderCount; i++){
        renderFbo[i].setUseTexture(true);
        renderFbo[i].allocate(width, height);
        renderFbo[i].begin();
        ofClear(0);
        renderFbo[i].end();
    }
    
    distortedView.setWidth(width);
    distortedView.setHeight(height);
    distortedCamera.setPosition(-width/2, -height/2, 15);
    meshScale = width * meshScaleExt;
    
}

void ofxCubeCamera::setCameraPosition(float x, float y, float z){
    for (int i=0; i<renderCount; i++){
        renderCamera[i].setPosition(x,y,z);
    }
}


void ofxCubeCamera::setMeshScale(float s){
    meshScaleExt = s;
    meshScale = width * meshScaleExt;
}



