// Copyright 2021 Akiya Research Institute, Inc. All Rights Reserved


#include "FaceDetector.h"

void UFaceDetector::init(const FString& onnxFilePath, const EOnnxProvider onnxProvider, const int gpuDeviceId)
{
    try
    {
        if (onnxModel) delete onnxModel;
        onnxModel = new OnnxModel(onnxFilePath, onnxProvider, gpuDeviceId);
    }
    catch (Ort::Exception e)
    {
        if (onnxModel) delete onnxModel;
        onnxModel = new OnnxModel(onnxFilePath, EOnnxProvider::CPU, 0);
    }

    onnxModel->bindOutput(onnxModel->outputTensorsInfo[0], &detection);
    onnxModel->bindOutput(onnxModel->outputTensorsInfo[1], &confidence);
}

bool UFaceDetector::detect(TArray<uint8>& inputRawImage, FFaceDetection& normalizedKeypoints)
{
    onnxModel->bindInput(onnxModel->inputTensorsInfo[0], inputRawImage.GetData());
    onnxModel->run();

    if (confidence > 0.5f)
    {
        detection.calculateCenterOrientationAndSize();
        normalizedKeypoints = detection;
        return true;
    }
    else
    {
        return false;
    }
}
