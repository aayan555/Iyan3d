//
//  SkinMesh.cpp
//  SGEngine2
//
//  Created by Vivek shivam on 11/10/1936 SAKA.
//  Copyright (c) 1936 SAKA Smackall Games Pvt Ltd. All rights reserved.
//

#include "SkinMesh.h"
SkinMesh::SkinMesh(){
    joints = new vector<Joint*>();
    RootJoints = new vector<Joint*>();
    meshType = MESH_TYPE_HEAVY;
}
SkinMesh::~SkinMesh(){
    for(int i = 0; i < joints->size();i++){
        if((*joints)[i])
            delete (*joints)[i];
    }
    // no need to delete rootjoints after delete joints
    if(joints)
        delete joints;
    if(RootJoints)
        delete RootJoints;
}

void SkinMesh::buildAllGlobalAnimatedMatrices(Joint *joint,Joint *parentJoint){
    if (!joint){
        for (int i=0; i<RootJoints->size(); ++i)
            buildAllGlobalAnimatedMatrices((*RootJoints)[i], 0);
        return;
    }
    else{
        // Find global matrix...
        if (!parentJoint)
            joint->GlobalAnimatedMatrix = joint->LocalAnimatedMatrix;
        else
            joint->GlobalAnimatedMatrix = parentJoint->GlobalAnimatedMatrix * joint->LocalAnimatedMatrix;
    }
    for (int j=0; j<joint->childJoints->size(); ++j)
        buildAllGlobalAnimatedMatrices((*joint->childJoints)[j], joint);
}
void SkinMesh::transferJointsToMesh(vector< shared_ptr<JointNode> > jointNodes){
    for (int i=0; i<joints->size(); ++i) {
        shared_ptr<JointNode>  node = jointNodes[i];
        Joint *joint = (*joints)[i];
        joint->LocalAnimatedMatrix.setRotationRadians(node->getRotationInRadians());
        joint->LocalAnimatedMatrix.translate(node->getPosition());
        joint->LocalAnimatedMatrix.scale(node->getScale());
        //node.reset();
    }
}
void SkinMesh::recoverJointsFromMesh(vector< shared_ptr<JointNode> > jointNodes){
    for (int i=0; i<jointNodes.size(); i++){
        shared_ptr<JointNode> node= jointNodes[i];
        Joint *joint=(*joints)[i];
        node->setPosition(joint->LocalAnimatedMatrix.getTranslation());
        node->setRotationInDegrees(joint->LocalAnimatedMatrix.getRotationInDegree());
        node->setScale(joint->LocalAnimatedMatrix.getScale());
        node->updateAbsoluteTransformationOfChildren();
        //node.reset();
    }
}
Joint* SkinMesh::addJoint(Joint *parent){
    Joint *joint =  new Joint();
    
    if (!parent){
        //Add root joints to array in finalize()
    }
    else{
        //Set parent (Be careful of the mesh loader also setting the parent)
        parent->childJoints->push_back(joint);
    }
    joint->Parent = parent;
    joint->Index = (joints->size() + 1) - 1;
    joints->push_back(joint);
    return joint;
}
void SkinMesh::finalize(){
    for(int CheckingIdx=0; CheckingIdx < joints->size(); CheckingIdx++){
        bool foundParent=false;
        for(int i = 0; i < joints->size() && (i != CheckingIdx); i++){
            for(int n = 0; n < (*joints)[i]->childJoints->size(); n++){
                if ((*(*joints)[i]->childJoints)[n] == (*joints)[CheckingIdx])
                    foundParent=true;
            }
        }
        if (!foundParent)
            RootJoints->push_back((*joints)[CheckingIdx]);
    }
    buildAllGlobalAnimatedMatrices(NULL,NULL);
     for(int i = 0; i < joints->size(); i++){
         Joint *joint = (*joints)[i];
         joint->GlobalInversedMatrix = joint->GlobalAnimatedMatrix;
         joint->GlobalInversedMatrix.invert(); // slow
     }
}
