//
//  Mesh.h
//  SGEngine2
//
//  Created by Harishankar on 13/11/14.
//  Copyright (c) 2014 Smackall Games Pvt Ltd. All rights reserved.
//

#ifndef __SGEngine2__Mesh__
#define __SGEngine2__Mesh__

#include "../common/common.h"
#include "../common/BoundingBox.h"
#include <iostream>
#include <map>

typedef enum {
    MESH_FORMAT_TRIANGLES = 0,
    MESH_FORMAT_STRIP,
    MESH_FORMAT_INDEXED_TRIANGLES,
    MESH_FORMAT_INDEXED_STRIP
} mesh_format_t;

typedef enum{
    MESH_TYPE_HEAVY,
    MESH_TYPE_LITE
}MESH_TYPE;
using std::vector;

struct vertexData{
    Vector3 vertPosition;
    Vector3 vertNormal;
    Vector2 texCoord1;
    Vector4 optionalData1;
};

struct vertexDataHeavy{
    Vector3 vertPosition;
    Vector3 vertNormal;
    Vector2 texCoord1;
    Vector4 optionalData1;
    Vector4 optionalData2;
    Vector4 optionalData3;
    Vector4 optionalData4;
};

struct vertexCompare
{
    bool operator() (const vertexData& lhs, const vertexData& rhs) const
    {
        return (lhs.vertPosition.x != rhs.vertPosition.x || lhs.vertPosition.y != rhs.vertPosition.y || lhs.vertPosition.z != rhs.vertPosition.z);
    }
    
    bool operator() (const vertexDataHeavy& lhs, const vertexDataHeavy& rhs) const
    {
        return (lhs.vertPosition.x != rhs.vertPosition.x || lhs.vertPosition.y != rhs.vertPosition.y || lhs.vertPosition.z != rhs.vertPosition.z);
    }

};

class Mesh {
    
private:
    BoundingBox BBox;

    vector< vector<vertexData> > meshBufferVerticesData;
    vector< vector<vertexDataHeavy> > meshBufferVerticesDataHeavy;
    vector< vector<unsigned short> > meshBufferIndices;

    vector<vertexData> tempVerticesData;
    vector<vertexDataHeavy> tempVerticesDataHeavy;
    vector<unsigned int> tempIndicesData;

public:
    mesh_format_t meshformat;
    MESH_TYPE meshType;

    void addVertex(vertexData* vertex);
    void addHeavyVertex(vertexDataHeavy* vertex);
    void addToIndicesArray(unsigned int index);
    void Commit();

    void clearVerticesArray();
    void clearIndicesArray();
    void recalculateNormalsT(bool smooth = true);
    void removeDoubles();
    void pivotToOrigin();
    vertexData* getLiteVertexByIndex(unsigned int index);
    vertexDataHeavy* getHeavyVertexByIndex(unsigned int index);
    vector<vertexData> getLiteVerticesArray(int index);
    vector<vertexDataHeavy> getHeavyVerticesArray(int index);
    unsigned short* getIndicesArray(int index);
    unsigned int * getHighPolyIndicesArray();
    unsigned int getIndicesCount(int index);
    unsigned int getTotalIndicesCount();
    unsigned int getVerticesCountInMeshBuffer(int index);
    unsigned int getVerticesCount();
    int getMeshBufferCount();
    BoundingBox* getBoundingBox();
    Vector3 getAngleWeight(Vector3& v1,Vector3& v2,Vector3& v3);
    void removeDoubles(bool usePos,bool useTcoords,bool useNormals);
    void removeDoublesInHeavyMesh(bool usePos,bool useTcoords,bool useNormals);
    Mesh();
    ~Mesh();
    template <typename T>
    static T getLinearInterpolation(float x1, T y1, float x2, T y2, float x)
    {
        return y1+(y2+(y1*-1.0))*(float)((x-x1)/(x2-x1));
        //Source: https://en.wikipedia.org/wiki/Linear_interpolation
    }

};


#endif /* defined(__SGEngine2__Mesh__) */