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
    
    currentType = type;
    switch(type){
        case(DOME):
            renderCount = 5;
            prepareFrameBuffers();
            renderMesh[BOTTOM].load(currentTypeString()+"/bottom.ply");
            renderMesh[FRONT].load(currentTypeString()+"/front.ply");
            renderMesh[LEFT].load(currentTypeString()+"/left.ply");
            renderMesh[RIGHT].load(currentTypeString()+"/right.ply");
            renderMesh[TOP].load(currentTypeString()+"/top.ply");
            break;
        default:
            renderCount = 6;
            prepareFrameBuffers();
            renderMesh[BOTTOM].load(currentTypeString()+"/top.ply");
            renderMesh[FRONT].load(currentTypeString()+"/front.ply");
            renderMesh[LEFT].load(currentTypeString()+"/left.ply");
            renderMesh[RIGHT].load(currentTypeString()+"/right.ply");
            renderMesh[TOP].load(currentTypeString()+"/bottom.ply");
            renderMesh[BACK].load(currentTypeString()+"/back.ply");
            break;
    }
    mask.load(currentTypeString()+"/mask.png");
    mask.setUseTexture(true);
    resize(width, height);
    
}

string ofxCubeCamera::currentTypeString(){
    switch(currentType){
        case(DOME): return "dome"   ;
            break;
        case(QUINCUNCIAL): return "quincuncial";
            break;
        case(MERCATOR): return "mercator";
            break;
    }
}

void ofxCubeCamera::prepareFrameBuffers(){
    for(int i=0; i<renderCount; i++){
        ofCamera c;
        c.setNearClip(.01);
        c.setParent(*this);
        renderCamera.push_back(c);
        ofFbo f;
        renderFbo.push_back(f);
        ofVboMesh m;
        renderMesh.push_back(m);
    }
}

void ofxCubeCamera::begin(int i){
    ofPushMatrix();
    renderFbo[i].begin();
    ofClear(0);
    renderCamera[i].begin();
}


void ofxCubeCamera::end(int i){
    renderFbo[i].end();
    renderCamera[i].end();
    ofPopMatrix();
}

void ofxCubeCamera::draw(){
    composedCamera.enableOrtho();
    composedCamera.begin(composedView);
    
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
    
    composedCamera.end();
    composedCamera.draw();
    
    mask.draw(0,0,width,height);
}


void ofxCubeCamera::setNearClip(float _near) {
    near = _near;
    for (ofCamera cam: renderCamera) {
        cam.setNearClip(near);
    }
};
void ofxCubeCamera::setFarClip(float _far) {
    far = _far;
    for (ofCamera cam: renderCamera) {
        cam.setFarClip(far);
    }
};

void ofxCubeCamera::resize(int w, int h){
    
    width = w;
    height = h;
    
    view.setWidth(width);
    view.setHeight(height);
    
    renderCamera[BOTTOM].setNearClip(near);
    renderCamera[BOTTOM].setFarClip(far);
    renderCamera[BOTTOM].lookAt(ofVec3f(0,-100,0), ofVec3f(0,0,-1));
//    renderCamera[BOTTOM].setOrientation(ofVec3f(-90,0,0));
    renderCamera[BOTTOM].setFov(90);
    
    renderCamera[FRONT].setNearClip(near);
    renderCamera[FRONT].setFarClip(far);
    renderCamera[FRONT].lookAt(ofVec3f(0,0,-100), ofVec3f(0,1,0));
//    renderCamera[FRONT].setOrientation(ofVec3f(0,0,0));
    renderCamera[FRONT].setFov(90);
    
    renderCamera[LEFT].setNearClip(near);
    renderCamera[LEFT].setFarClip(far);
    renderCamera[LEFT].lookAt(ofVec3f(-100,0,0), ofVec3f(0,1,0));
//    renderCamera[LEFT].setOrientation(ofVec3f(0,90,0));
    renderCamera[LEFT].setFov(90);
    
    renderCamera[RIGHT].setNearClip(near);
    renderCamera[RIGHT].setFarClip(far);
    renderCamera[RIGHT].lookAt(ofVec3f(100, 0, 0), ofVec3f(0,1,0));
//    renderCamera[RIGHT].setOrientation(ofVec3f(0, -90, 0));
    renderCamera[RIGHT].setFov(90);
    
    renderCamera[TOP].setNearClip(near);
    renderCamera[TOP].setFarClip(far);
    renderCamera[TOP].lookAt(ofVec3f(0,100,0), ofVec3f(0,0,1));
//    renderCamera[TOP].setOrientation(ofVec3f(90,0,0));
    renderCamera[TOP].setFov(90);
    
    if (renderCount == 6){
        renderCamera[BACK].setNearClip(near);
        renderCamera[BACK].setFarClip(far);
        renderCamera[BACK].lookAt(ofVec3f(0,0,100), ofVec3f(0,-1,0));
//        renderCamera[BACK].setOrientation(ofVec3f(0,180,180));
        renderCamera[BACK].setFov(90);
    }
    
    for(int i =0; i<renderCount; i++){
        renderFbo[i].setUseTexture(true);
        renderFbo[i].allocate(width, height);
        renderFbo[i].begin();
        ofClear(0);
        renderFbo[i].end();
    }
    
    composedView.setWidth(width);
    composedView.setHeight(height);
    composedCamera.setPosition(-width/2, -height/2, 15);
    meshScale = width * meshScaleExt;
    
}

void ofxCubeCamera::setMeshScale(float s){
    meshScaleExt = s;
    meshScale = width * meshScaleExt;
}



