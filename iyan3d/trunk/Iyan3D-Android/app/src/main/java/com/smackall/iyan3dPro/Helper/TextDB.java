package com.smackall.iyan3dPro.Helper;

/**
 * Created by Sabish.M on 17/3/16.
 * Copyright (c) 2015 Smackall Games Pvt Ltd. All rights reserved.
 */
public class TextDB {

    float red = 1.0f;
    float green = 1.0f;
    float blue = 1.0f;
    int typeOfNode;
    String textureName = "-1";
    String text = "TEXT";
    int fontSize = 10;
    int bevalValue;
    int assetAddType;
    String filePath = "-1";
    boolean isTempNode = true;
    int actionType = Constants.ACTION_TEXT_IMAGE_ADD;

    public TextDB() {

    }

    public TextDB(float red, float green, float blue, int typeOfNode, String textureName, String text, int fontSize, int bevalValue, int assetAddType, String filePath, boolean isTempNode) {
        this.red = red;
        this.green = green;
        this.blue = blue;
        this.typeOfNode = typeOfNode;
        this.textureName = textureName;
        this.text = text;
        this.fontSize = fontSize;
        this.bevalValue = bevalValue;
        this.assetAddType = assetAddType;
        this.filePath = filePath;
        this.isTempNode = isTempNode;
    }

    public void resetValues() {
        red = 1.0f;
        green = 1.0f;
        blue = 1.0f;
        typeOfNode = 0;
        textureName = "-1";
        text = "TEXT";
        fontSize = 10;
        bevalValue = 0;
        assetAddType = 0;
        filePath = "-1";
        isTempNode = true;
        actionType = Constants.ACTION_TEXT_IMAGE_ADD;
    }

    public float getRed() {
        return this.red;
    }

    public void setRed(float red) {
        this.red = red;
    }

    public float getGreen() {
        return this.green;
    }

    public void setGreen(float green) {
        this.green = green;
    }

    public float getBlue() {
        return blue;
    }

    public void setBlue(float blue) {
        this.blue = blue;
    }

    public int getTypeOfNode() {
        return this.typeOfNode;
    }

    public void setTypeOfNode(int typeOfNode) {
        this.typeOfNode = typeOfNode;
    }

    public String getTextureName() {
        return this.textureName;
    }

    public void setTextureName(String textureName) {
        this.textureName = textureName;
    }

    public String getText() {
        return this.text;
    }

    public void setText(String text) {
        this.text = text;
    }

    public int getFontSize() {
        return this.fontSize;
    }

    public void setFontSize(int fontSize) {
        this.fontSize = fontSize;
    }

    public int getBevalValue() {
        return this.bevalValue;
    }

    public void setBevalValue(int bevalValue) {
        this.bevalValue = bevalValue;
    }

    public int getAssetAddType() {
        return this.assetAddType;
    }

    public void setAssetAddType(int assetAddType) {
        this.assetAddType = assetAddType;
    }

    public String getFilePath() {
        return this.filePath;
    }

    public void setFilePath(String filePath) {
        this.filePath = filePath;
    }

    public boolean getTempNode() {
        return this.isTempNode;
    }

    public void setTempNode(boolean isTempNode) {
        this.isTempNode = isTempNode;
    }

    public int getActionType() {
        return this.actionType;
    }

    public void setActionType(int actionType) {
        this.actionType = actionType;
    }

}
